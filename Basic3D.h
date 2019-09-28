#pragma once

#ifndef BASIC3D_H
#define BASIC3D_H
#include <stdlib.h>
#include <glm.hpp>
#include <vector>
#include "qopenglfunctions_4_5_core.h"


class drawFuncs {
 public:
  drawFuncs() {}
  GLfloat* drawArray(std::vector<glm::fvec3> face_vertices, int num_faces, glm::fvec3 color);
  GLfloat* drawArray(std::vector<glm::fvec3> face_vertices, int num_faces);
  GLfloat* drawCylinder(glm::fvec3 from_P, glm::fvec3 to_P, double width, glm::fvec3 color);
};


#endif  // !BASIC3D_H
