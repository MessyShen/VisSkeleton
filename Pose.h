#pragma once

#include <stdlib.h>
#include <glm.hpp>
#include <string>
#include <vector>

#include "Joint.h"

class Pose {
 public:
  Pose();
  Pose(const char* poseConfig);
  ~Pose();

  int jointNum;                       // default 18
  std::vector<int> parents;           // size: jointNum
  std::vector<double> boneLength;     // size: jointNum, bone len to parent
  std::vector<glm::fvec3> positions;  //[x, y, z] in cam
  std::vector<glm::fvec3> offsets;    // s_, point - parent
  std::vector<glm::fvec3> angles;     // euler angles
  std::string ord;                    //"xyz"
  std::vector<Joint> joints;
  std::vector<int> sequence;  // by default [0,1,2,3...], may need to adjust if
                              // tree order is not well defined
  std::vector<glm::mat4x4> rotMatrices; //local frame
  std::vector<glm::fvec3> angleMins;  // angle limit minimum
  std::vector<glm::fvec3> angleMaxs;  // angle limit maximum

  int selectedAngle;

  GLfloat* poseData;
  GLfloat* boneData;
  GLfloat* axisData;

  void setSelectedAngle(int change);
  void FK();  // calculate forward kinematics

 private:
  void readFromConfig(const char* poseConfig);
  void calcHM36TosePositions();
  void calcOffsets();  // calculate offsets by postition
  void loadDefaultConfig();
  void initJoints();
  void initBones();
  void initAxis();
  void draw3DLine(glm::fvec3 from_P, glm::vec3 to_P, double width);
  std::vector<glm::fvec3> calcTargetPose(std::vector<glm::fvec3> angles,
                                         std::vector<glm::fvec3> pos,
                 std::vector<int> parents, std::vector<int> sequence);
};