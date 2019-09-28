#pragma once

#ifndef JOINT_H
#define JOINT_H

#include <stdlib.h>
#include <glm.hpp>
#include <vector>
#include "qopenglfunctions_4_5_core.h"
#include "Basic3D.h"

class Joint {

 public:
  Joint();
  Joint(double x, double y, double z) : x(x), y(y), z(z) {
    calcFaceVertices();
    calcFaceNormals();
  }
  ~Joint();

  int joint_id, parent_id;
  double x, y, z;
  const int num_faces = 8;
  const double drawSize = 0.5;
  GLfloat* vertices_data;
  std::vector<glm::fvec3> face_vertices;
  std::vector<glm::fvec3> face_normals;
  std::vector<glm::ivec3> vertex_indices;  // vertex index for each face.

 private:
  void calcFaceVertices();
  void calcFaceNormals();


};

#endif  // !JOINT_H
