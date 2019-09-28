#include "Joint.h"

#include <qfileinfo.h>

Joint::Joint() {
  x = 0;
  y = 0;
  z = 0;
  calcFaceVertices();
  calcFaceNormals();
}

Joint::~Joint() {
  face_normals.clear();
  face_vertices.clear();
}

void Joint::calcFaceVertices() {
  for (int i = 0; i < num_faces; ++i) {
    std::vector<glm::fvec3> tempv;
    tempv.clear();
    for (int j = 0; j < 3; ++j) {
      glm::fvec3 trianglePoint(0.0f, 0.0f, 0.0f);
      
      trianglePoint[0] = x + ((j == 0) ? ((i & 1) ? drawSize : -drawSize) : 0);
      trianglePoint[1] = y + ((j == 1) ? ((i & 2) ? drawSize : -drawSize) : 0);
      trianglePoint[2] = z + ((j == 2) ? ((i & 4) ? drawSize : -drawSize) : 0);
      tempv.push_back(trianglePoint);
    }
    vertex_indices.push_back(glm::ivec3(i * 3, i * 3 + 1, i * 3 + 2));
    if ((i == 1) | (i == 2) | (i == 4) | (i == 7)) {
      face_vertices.push_back(tempv[0]);
      face_vertices.push_back(tempv[1]);
      face_vertices.push_back(tempv[2]);
    } else {
      face_vertices.push_back(tempv[1]);
      face_vertices.push_back(tempv[0]);
      face_vertices.push_back(tempv[2]);
    }
  }
  
}

/*
void Joint::calcFaceNormals() {
  for (int i = 0; i < num_faces; ++i) {
    glm::fvec3 position_0(face_vertices[i * 3]);
    glm::fvec3 position_1(face_vertices[i * 3 + 1]);
    glm::fvec3 position_2(face_vertices[i * 3 + 2]);

    glm::fvec3 vector_a(0.f, 0.f, 0.f);
    glm::fvec3 vector_b(0.f, 0.f, 0.f);

    vector_a = position_2 - position_0;
    vector_b = position_0 - position_1;

    glm::fvec3 vector_normal(0.f, 0.f, 0.f);
    vector_normal[0] = vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1];
    vector_normal[1] = vector_a[2] * vector_b[0] - vector_a[0] * vector_b[2];
    vector_normal[2] = vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];

    glm::fvec3 normalized_vec = glm::normalize(vector_normal);

    face_normals.push_back(normalized_vec);
  }
  vertices_data = new GLfloat[num_faces * 3 * 9];
  for (int i_num = 0; i_num < num_faces * 3; i_num++) {
    for (int i_data = 0; i_data < 9; i_data++) {
      if (i_data < 3)
        vertices_data[i_num * 9 + i_data] = face_vertices[i_num][i_data];
      else if (i_data < 6) {
        vertices_data[i_num * 9 + i_data] =
            face_normals[int(i_num / 3)][i_data - 3];
      } else {
        vertices_data[i_num * 9 + i_data] = 0.75f;
      }
    }
  }
}
*/

void Joint::calcFaceNormals() { 
  drawFuncs f = drawFuncs(); 
  vertices_data = f.drawArray(face_vertices, num_faces);
};