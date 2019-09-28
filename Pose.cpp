#include "Pose.h"
#include <qfileinfo.h>
#include <QDebug>
#include <cmath>
#include <fstream>
#include <iostream>

Pose::Pose() { loadDefaultConfig(); }

Pose::Pose(const char* poseConfig) { readFromConfig(poseConfig); }

Pose::~Pose() {}

void Pose::readFromConfig(const char* poseConfig) {
  /* example
  3           //jointNum
  0 0 1       //parent
  0 1 2       //sequence
  0.0 0.0 0.0
  3.0 0.0 0.0
  3.0 3.0 0.0
  */
  std::fstream f(poseConfig);
  f >> jointNum;
  parents.clear();
  for (int i = 0; i < jointNum; ++i) {
    int temp;
    f >> temp;
    parents.push_back(temp);
  }

  sequence.clear();
  for (int i = 0; i < jointNum; ++i) {
    int temp;
    f >> temp;
    sequence.push_back(temp);
  }

  for (int i = 0; i < jointNum; ++i) {
    glm::fvec3 point(0.0, 0.0, 0.0);
    f >> point[0] >> point[1] >> point[2];
    positions.push_back(point);
  }

  // init angles
  for (int i = 0; i < jointNum; ++i) {
    angles.push_back(glm::fvec3(0.0, 0.0, 0.0));
    angleMins.push_back(glm::fvec3(-180.0, -180.0, -180.0));
    angleMaxs.push_back(glm::fvec3(180.0, 180.0, 180.0));
  }

  for (int i = 0; i < jointNum; ++i) {
    int idx;
    f >> idx;

    f >> angleMins[idx][0] >> angleMaxs[idx][0];
    f >> angleMins[idx][1] >> angleMaxs[idx][1];
    f >> angleMins[idx][2] >> angleMaxs[idx][2];
  }

  selectedAngle = 0;
  calcOffsets();
  initJoints();
  initBones();
  FK();
  initAxis();
}

void Pose::calcHM36TosePositions() {}

void eulerToRotMat(std::vector<glm::fvec3> angles,
                   std::vector<glm::mat4x4>& rotMat) {
  rotMat.clear();
  int num = angles.size();
  for (int i = 0; i < num; ++i) {
    /* //The Original ZYX order
    double phi = angles[i][0];
    double theta = angles[i][1];
    double sigma = angles[i][2];
    
    double Mt11 = cos(theta) * cos(sigma);
    double Mt12 = sin(phi) * sin(theta) * cos(sigma) - cos(phi) * sin(sigma);
    double Mt13 = cos(phi) * sin(theta) * cos(sigma) + sin(phi) * sin(sigma);
    double Mt21 = cos(theta) * sin(sigma);
    double Mt22 = sin(phi) * sin(theta) * sin(sigma) + cos(phi) * cos(sigma);
    double Mt23 = cos(phi) * sin(theta) * sin(sigma) - sin(phi) * cos(sigma);
    double Mt31 = -sin(theta);
    double Mt32 = sin(phi) * cos(theta);
    double Mt33 = cos(phi) * cos(theta);
    */
    double x = angles[i][0];
    double y = angles[i][1];
    double z = angles[i][2];
    
    double Mt11 = cos(y) * cos(z);
    double Mt12 = -cos(y) * sin(z);
    double Mt13 = sin(y);
    double Mt21 = cos(z)*sin(x)*sin(y) + cos(x)*sin(z);
    double Mt22 = cos(x)*cos(z) - sin(x)*sin(y)*sin(z);
    double Mt23 = -cos(y)*sin(x);
    double Mt31 = -cos(x) * cos(z) * sin(y) + sin(x) * sin(z);
    double Mt32 = cos(z) * sin(x) + cos(x) * sin(y) * sin(z);
    double Mt33 = cos(x) * cos(y);
    
    glm::mat3x3 mt(Mt11, Mt12, Mt13, Mt21, Mt22, Mt23, Mt31, Mt32, Mt33);
    glm::mat4x4 mt4(mt);
    rotMat.push_back(mt4);
  }
}

void catPositions(std::vector<glm::mat4x4>& rotMat,
                  std::vector<glm::fvec3> pos) {
  // pos here is actually offsets
  // assert pos.size() == rotMat.size == jointNum
  for (int i = 0; i < pos.size(); ++i) {
    rotMat[i][0][3] = pos[i][0];
    rotMat[i][1][3] = pos[i][1];
    rotMat[i][2][3] = pos[i][2];
  }
}

std::vector<glm::mat4x4> calcFK(std::vector<glm::mat4x4>& rotMat,
                                std::vector<glm::fvec3> pos,
                                std::vector<int> parents,
                                std::vector<int> sequence) {
  std::vector<glm::mat4x4> globalMat;
  globalMat.push_back(rotMat[0]);
  for (int i = 1; i < pos.size(); ++i) globalMat.push_back(glm::mat4x4(1));

  glm::mat4x4 tempMat, tempRotMat2;
  bool strangeWay = false;
  for (auto& i : sequence) {
    if (i == 0) continue;
    if (strangeWay) {
      tempRotMat2 = rotMat[i];
      tempRotMat2[0][3] = tempRotMat2[1][3] = tempRotMat2[2][3] = 0;
      tempMat = rotMat[i] * tempRotMat2;
      rotMat[i][0][3] = tempMat[0][3];
      rotMat[i][1][3] = tempMat[1][3];
      rotMat[i][2][3] = tempMat[2][3];
    }
    globalMat[i] = rotMat[i] * globalMat[parents[i]];

    // globalMat[i] = globalMat[parents[i]] * rotMat[i];
  }

  return globalMat;
}

std::vector<glm::fvec3> Pose::calcTargetPose(std::vector<glm::fvec3> angles,
                                             std::vector<glm::fvec3> pos,
                                             std::vector<int> parents,
                                             std::vector<int> sequence) {
  std::vector<glm::fvec3> targetPose;
  std::vector<glm::mat4x4> mat;
  eulerToRotMat(angles, mat);
  catPositions(mat, pos);
  std::vector<glm::mat4x4> globalMat = calcFK(mat, pos, parents, sequence);

  rotMatrices.clear();
  rotMatrices.assign(globalMat.begin(), globalMat.end());

  for (int i = 0; i < pos.size(); ++i) {
    targetPose.push_back(
        glm::fvec3(globalMat[i][0][3], globalMat[i][1][3], globalMat[i][2][3]));
  }
  return targetPose;
}

void Pose::FK() {
  // calcOffsets();
  std::vector<glm::fvec3> FKPose =
      calcTargetPose(angles, offsets, parents, sequence);
  positions.clear();
  positions.assign(FKPose.begin(), FKPose.end());
  initJoints();
  initBones();
  initAxis();
}

void Pose::calcOffsets() {
  offsets.clear();
  for (int i = 0; i < jointNum; ++i)
    offsets.push_back(glm::fvec3(0.0, 0.0, 0.0));
  for (auto& i : sequence) {
    offsets[i] = positions[i] - positions[parents[i]];
  }
}

void Pose::loadDefaultConfig() {
  jointNum = 2;
  parents.resize(2);
  parents[0] = 0;
  parents[1] = 0;

  // positions.resize(2);
  glm::fvec3 p(0, 0, 0);
  positions.push_back(p);
  p[0] = 5;
  positions.push_back(p);

  initJoints();
}

void Pose::initJoints() {
  joints.clear();
  for (int i = 0; i < jointNum; ++i) {
    Joint j(positions[i].x, positions[i].y, positions[i].z);
    joints.push_back(j);
  }
  int num_faces = 8;
  poseData = new GLfloat[num_faces * 3 * 9 * jointNum];
  for (int i = 0; i < num_faces * 3 * 9; ++i) {
    for (int j = 0; j < jointNum; ++j) {
      poseData[i + num_faces * 3 * 9 * j] = joints[j].vertices_data[i];
    }
  }
  return;
}

void Pose::draw3DLine(glm::fvec3 p1, glm::fvec3 p2, double width) {}

void Pose::initBones() {
  boneData = new GLfloat[jointNum * 2 * 9];
  for (int i = 0; i < jointNum; ++i) {
    for (int j = 0; j < 2; ++j) {
      if (j == 0) {
        boneData[(i * 2 + j) * 9 + 0] = positions[i][0];
        boneData[(i * 2 + j) * 9 + 1] = positions[i][1];
        boneData[(i * 2 + j) * 9 + 2] = positions[i][2];
      } else {
        boneData[(i * 2 + j) * 9 + 0] = positions[parents[i]][0];
        boneData[(i * 2 + j) * 9 + 1] = positions[parents[i]][1];
        boneData[(i * 2 + j) * 9 + 2] = positions[parents[i]][2];
      }

      // normal
      boneData[(i * 2 + j) * 9 + 3] = 1.0f;
      boneData[(i * 2 + j) * 9 + 4] = 1.0f;
      boneData[(i * 2 + j) * 9 + 5] = 1.0f;

      // color
      boneData[(i * 2 + j) * 9 + 6] = 0.8f;
      boneData[(i * 2 + j) * 9 + 7] = 0.8f;
      boneData[(i * 2 + j) * 9 + 8] = 0.8f;
      if (i == 14 || i == 15 || i == 16 || i == 1 || i == 2 || i == 3) {
        boneData[(i * 2 + j) * 9 + 6] = 0.2f;
        boneData[(i * 2 + j) * 9 + 7] = 0.2f;
        boneData[(i * 2 + j) * 9 + 8] = 0.7f;
      }
      if (i == 11 || i == 12 || i == 13 || i == 4 || i == 5 || i == 6) {
        boneData[(i * 2 + j) * 9 + 6] = 0.7f;
        boneData[(i * 2 + j) * 9 + 7] = 0.2f;
        boneData[(i * 2 + j) * 9 + 8] = 0.2f;
      }
    }
  }
  return;
}

#define AXIS_ON_BONE
void Pose::initAxis() {
  axisData = new GLfloat[jointNum * 3 * 2 * 9 + jointNum * 2 * 9];
  for (int i = 0; i < jointNum; ++i) {
    // i: joint id
    for (int j = 0; j < 3; ++j) {
      // j=0:x, j=1:y, j=2:z
      for (int k = 0; k < 2; ++k) {
        int idx = (i * 6 + j * 2 + k) * 9;
#ifndef AXIS_ON_BONE
        if (k == 0) {
          axisData[idx + 0] = positions[i][0];
          axisData[idx + 1] = positions[i][1];
          axisData[idx + 2] = positions[i][2];
        } else {
          axisData[idx + 0] = positions[i][0] + rotMatrices[i][0][j] * 4;
          axisData[idx + 1] = positions[i][1] + rotMatrices[i][1][j] * 4;
          axisData[idx + 2] = positions[i][2] + rotMatrices[i][2][j] * 4;
        }
#endif  // !AXIS_ON_BONE
#ifdef AXIS_ON_BONE

        if (i == 0) {
          if (k == 0) {
            axisData[idx + 0] = positions[i][0];
            axisData[idx + 1] = positions[i][1];
            axisData[idx + 2] = positions[i][2];
          } else {
            axisData[idx + 0] = positions[i][0] + rotMatrices[i][0][j] * 4;
            axisData[idx + 1] = positions[i][1] + rotMatrices[i][1][j] * 4;
            axisData[idx + 2] = positions[i][2] + rotMatrices[i][2][j] * 4;
          }
        }

        //else if (i == 4 || i == 1 || i == 17 || i == 7) {
        //  axisData[idx + 0] = 0.0;
        //  axisData[idx + 1] = 0.0;
        //  axisData[idx + 2] = 0.0;
        //} 
        
        else {
          if (k == 0) {
            axisData[idx + 0] =
                (positions[i][0] + positions[parents[i]][0]) * 0.5;
            axisData[idx + 1] =
                (positions[i][1] + positions[parents[i]][1]) * 0.5;
            axisData[idx + 2] =
                (positions[i][2] + positions[parents[i]][2]) * 0.5;
          } else {
            axisData[idx + 0] =
                (positions[i][0] + positions[parents[i]][0]) * 0.5
            + rotMatrices[parents[i]][0][j] * 4;
            axisData[idx + 1] =
                (positions[i][1] + positions[parents[i]][1]) * 0.5
            +rotMatrices[parents[i]][1][j] * 4;
            axisData[idx + 2] =
                (positions[i][2] + positions[parents[i]][2]) * 0.5
            +rotMatrices[parents[i]][2][j] * 4;
          }
        }

#endif  // AXIS_ON_BONE

        // normal
        axisData[idx + 3] = axisData[idx + 4] = axisData[idx + 5] = 1.0f;

        // color
        for (int mmp = 0; mmp < 3; ++mmp) {
          axisData[idx + 6 + mmp] = (j == mmp) ? 1.0f : 0.0f;
        }
      }
    }
  }
  for (int i = jointNum * 3 * 2 * 9;
       i < jointNum * 3 * 2 * 9 + jointNum * 2 * 9; ++i) {
    axisData[i] = boneData[i - jointNum * 3 * 2 * 9];
  }
}

void Pose::setSelectedAngle(int n) { selectedAngle = n; }
