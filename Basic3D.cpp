#include "Basic3D.h"
#include <gl/GLU.h>

GLfloat* drawFuncs::drawArray(std::vector<glm::fvec3> face_vertices, int num_faces,
                   glm::fvec3 color) {
  std::vector<glm::fvec3> face_normals;
  GLfloat* vertices_data;

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
        // vertices_data[i_num * 9 + i_data] = 0.75f;
      }
    }
    vertices_data[i_num * 9 + 6] = color[0];
    vertices_data[i_num * 9 + 7] = color[1];
    vertices_data[i_num * 9 + 8] = color[2];
  }
  return vertices_data;
}

GLfloat* drawFuncs::drawArray(std::vector<glm::fvec3> face_vertices,
                              int num_faces) {
  glm::fvec3 color(0.75, 0.75, 0.75);
  return drawArray(face_vertices, num_faces, color);
}

GLfloat* drawFuncs::drawCylinder(glm::fvec3 from_P, glm::fvec3 to_P,
                                 double width, glm::fvec3 color) {
  //gluCylinder()
  return NULL;
}