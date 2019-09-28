#include "MainOpenGLWidget.h"
#include <qcoreapplication.h>
#include <QmouseEvent>

#include <iostream>
#include <string>

#include <QFileDialog>

GLuint VBOObj, VAOObj;
GLuint VBOObj_Line, VAOObj_Line;

bool MainOpenGLWidget::m_transparent = false;

MainOpenGLWidget::MainOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_xRot(0), m_yRot(0), m_zRot(0) {
  if (m_transparent) {
    QSurfaceFormat fmt = format();
    fmt.setAlphaBufferSize(8);
    setFormat(fmt);
  }

  m_joint = new Joint();
  m_pose = new Pose("HM36TPoseConfig.txt");
  //m_pose = new Pose("SimpleConfig.txt");
}

MainOpenGLWidget::~MainOpenGLWidget() { cleanup(); }

QSize MainOpenGLWidget::minimumSizeHint() const { return QSize(256, 256); }

QSize MainOpenGLWidget::sizeHint() const { return QSize(512, 512); }

static void qNormalizeAngle(int &angle) {
  while (angle < 0) {
    angle += 360 * 16;
  }
  while (angle > 360 * 16) {
    angle -= 360 * 16;
  }
}

void MainOpenGLWidget::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_xRot) {
    m_xRot = angle;
    emit xRotationChanged(angle);
    update();
  }
}

void MainOpenGLWidget::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_yRot) {
    m_yRot = angle;
    emit yRotationChanged(angle);
    update();
  }
}

void MainOpenGLWidget::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_zRot) {
    m_zRot = angle;
    emit zRotationChanged(angle);
    update();
  }
}

/*
int MainOpenGLWidget::setXsliderValue(int value) {
  glm::fvec3 v = m_pose->angles[m_pose->selectedAngle];
  return v[0];
}
*/

void MainOpenGLWidget::cleanup() {
  if (&m_shader->shader_program == nullptr) {
    return;
  }
  makeCurrent();
  delete &m_shader->shader_program;
  doneCurrent();
}

void MainOpenGLWidget::setAngle(int angle) { 
  m_pose->setSelectedAngle(angle); 
  setAngleLimit(angle);
  update();
}

void MainOpenGLWidget::setXangle(int angle) {
  int selected = m_pose->selectedAngle;
  m_pose->angles[selected][0] = (angle * 1.0) / 180 * 3.141593;
  m_pose->FK();
  update();
}

void MainOpenGLWidget::setYangle(int angle) {
  int selected = m_pose->selectedAngle;
  m_pose->angles[selected][1] = (angle * 1.0) / 180 * 3.141593;
  m_pose->FK();
  update();
}

void MainOpenGLWidget::setZangle(int angle) {
  int selected = m_pose->selectedAngle;
  m_pose->angles[selected][2] = (angle * 1.0) / 180 * 3.141593;
  m_pose->FK();
  update();
}


void MainOpenGLWidget::initializeGL() {
  // setui
  setAngleLimit(m_pose->selectedAngle);
  resetPose();


  // shader part
  m_core = QOpenGLContext::currentContext()
               ->versionFunctions<QOpenGLFunctions_4_5_Core>();
  m_shader = new Shader("ShaderFiles/vertex_shader_source.vert",
                        "ShaderFiles/fragment_shader_source.frag");

  // prepare triangles
  m_core->glGenVertexArrays(1, &VAOObj);
  m_core->glGenBuffers(1, &VBOObj);
  m_core->glBindVertexArray(VAOObj);
  m_core->glBindBuffer(GL_ARRAY_BUFFER, VBOObj);
  m_core->glBufferData(GL_ARRAY_BUFFER,
                       sizeof(GLfloat) * (m_pose->joints[0].num_faces * 3 * 9 *
                                          m_pose->jointNum),
                       m_pose->poseData, GL_STATIC_DRAW);
  m_core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                (void *)0);
  m_core->glEnableVertexAttribArray(0);
  m_core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                (GLvoid *)(3 * sizeof(GLfloat)));
  m_core->glEnableVertexAttribArray(1);
  m_core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                (GLvoid *)(6 * sizeof(GLfloat)));
  m_core->glEnableVertexAttribArray(2);

  // prepare lines
  m_core->glGenVertexArrays(1, &VAOObj_Line);
  m_core->glGenBuffers(1, &VBOObj_Line);
  m_core->glBindVertexArray(VAOObj_Line);
  m_core->glBindBuffer(GL_ARRAY_BUFFER, VBOObj_Line);
//  m_core->glBufferData(GL_ARRAY_BUFFER,
//                       sizeof(GLfloat) * (2 * 9 * (m_pose->jointNum)),
//                       m_pose->boneData, GL_STATIC_DRAW);
  m_core->glBufferData(GL_ARRAY_BUFFER,
                         sizeof(GLfloat) * (4 * 2 * 9 * (m_pose->jointNum)),
                         m_pose->axisData, GL_STATIC_DRAW);
  m_core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                (void *)0);
  m_core->glEnableVertexAttribArray(0);
  m_core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                (GLvoid *)(3 * sizeof(GLfloat)));
  m_core->glEnableVertexAttribArray(1);
  m_core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                (GLvoid *)(6 * sizeof(GLfloat)));
  m_core->glEnableVertexAttribArray(2);

  // set color
  //m_core->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  m_core->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // set camera
  m_camera.setToIdentity();
  m_camera.translate(0, 0, -80);

  m_is_reload = true;
}

void MainOpenGLWidget::resizeGL(int width, int height) {
  m_proj.setToIdentity();
  m_proj.perspective(45.0f, GLfloat(width) / height, 0.01f, 100.0f);
}

void MainOpenGLWidget::paintGL() {
  m_core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_core->glEnable(GL_DEPTH_TEST);
  m_core->glEnable(GL_CULL_FACE);

  m_world.setToIdentity();
  m_world.rotate(m_xRot / 16.0f, 1, 0, 0);
  m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
  m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

  m_a_light_pos = QVector3D(0.0, 2.0, 3.0);
  m_b_light_pos = QVector3D(0.0, 2.0, -3.0);
  m_view_pos = QVector3D(0.0, 0.0, 2.0);

  m_shader->use();
  m_projMatrixLoc = m_shader->shader_program.uniformLocation("proj_mat");
  m_modelMatrixLoc = m_shader->shader_program.uniformLocation("model_mat");
  m_viewMatrixLoc = m_shader->shader_program.uniformLocation("view_mat");
  m_aLightPosLoc = m_shader->shader_program.uniformLocation("a_light_pos");
  m_bLightPosLoc = m_shader->shader_program.uniformLocation("b_light_pos");
  m_viewPosLoc = m_shader->shader_program.uniformLocation("view_pos");

  m_shader->shader_program.setUniformValue(m_projMatrixLoc, m_proj);
  m_shader->shader_program.setUniformValue(m_modelMatrixLoc, m_world);
  m_shader->shader_program.setUniformValue(m_viewMatrixLoc, m_camera);
  m_shader->shader_program.setUniformValue(m_aLightPosLoc, m_a_light_pos);
  m_shader->shader_program.setUniformValue(m_bLightPosLoc, m_b_light_pos);
  m_shader->shader_program.setUniformValue(m_viewPosLoc, m_view_pos);

  m_core->glBindVertexArray(VAOObj);
  if (m_is_reload) {
    m_core->glGenVertexArrays(1, &VAOObj);
    m_core->glGenBuffers(1, &VBOObj);
    m_core->glBindVertexArray(VAOObj);
    m_core->glBindBuffer(GL_ARRAY_BUFFER, VBOObj);
    m_core->glBufferData(GL_ARRAY_BUFFER,
                         sizeof(GLfloat) * (m_pose->joints[0].num_faces * 3 *
                                            9 * m_pose->jointNum),
                         m_pose->poseData, GL_STATIC_DRAW);
    m_core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                  (void *)0);
    m_core->glEnableVertexAttribArray(0);
    m_core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat)));
    m_core->glEnableVertexAttribArray(1);
    m_core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                  (GLvoid *)(6 * sizeof(GLfloat)));
    m_core->glEnableVertexAttribArray(2);

    //m_is_reload = false;
  }

  m_core->glDrawArrays(GL_TRIANGLES, 0,
                       m_pose->joints[0].num_faces * 3 * m_pose->jointNum);

  // draw lines
  m_core->glBindVertexArray(VAOObj_Line);
  if (m_is_reload) {
    m_core->glGenVertexArrays(1, &VAOObj_Line);
    m_core->glGenBuffers(1, &VBOObj_Line);
    m_core->glBindVertexArray(VAOObj_Line);
    m_core->glBindBuffer(GL_ARRAY_BUFFER, VBOObj_Line);
    m_core->glBufferData(GL_ARRAY_BUFFER,
                         sizeof(GLfloat) * (4 * 2 * 9 * (m_pose->jointNum)),
                         m_pose->axisData, GL_STATIC_DRAW);
    m_core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                  (void *)0);
    m_core->glEnableVertexAttribArray(0);
    m_core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat)));
    m_core->glEnableVertexAttribArray(1);
    m_core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                                  (GLvoid *)(6 * sizeof(GLfloat)));
    m_core->glEnableVertexAttribArray(2);

    //m_is_reload = false;
  }
  m_core->glLineWidth(2);
  m_core->glDrawArrays(GL_LINES, 0, (m_pose->jointNum) * 2 * 3);
  m_core->glLineWidth(10);
  m_core->glDrawArrays(GL_LINES, (m_pose->jointNum) * 2 * 3,
                       (m_pose->jointNum) * 2);
  // m_pose->joints[0].num_faces * 3 * m_pose->jointNum);

  m_shader->shader_program.release();

  QMetaObject::invokeMethod(this, "updateGL", Qt::QueuedConnection);
}

void MainOpenGLWidget::setAngleLimit(int angle) {
  glm::fvec3 bufferAngle = m_pose->angles[angle];
  emit setXSliderMin(m_pose->angleMins[angle][0]); 
  emit setXSliderMax(m_pose->angleMaxs[angle][0]);
  if (bufferAngle != m_pose->angles[angle]) {
    m_pose->angles[angle] = bufferAngle;
  }
  emit setXSliderValue(int(m_pose->angles[angle][0] * 180.0 / 3.141593));
  
  emit setYSliderMin(m_pose->angleMins[angle][1]); 
  emit setYSliderMax(m_pose->angleMaxs[angle][1]); 
  if (bufferAngle != m_pose->angles[angle]) {
    m_pose->angles[angle] = bufferAngle;
  }
  emit setYSliderValue(int(m_pose->angles[angle][1] * 180.0 / 3.141593));
  
  emit setZSliderMin(m_pose->angleMins[angle][2]); 
  emit setZSliderMax(m_pose->angleMaxs[angle][2]); 
  if (bufferAngle != m_pose->angles[angle]) {
    m_pose->angles[angle] = bufferAngle;
  }
  emit setZSliderValue(int(m_pose->angles[angle][2] * 180.0 / 3.141593));
}

void MainOpenGLWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      close();
    case Qt::Key_Space:
      resetPose();
    default:
      break;
  }
}

void MainOpenGLWidget::resetPose() {
  for (auto &i : m_pose->angles) {
    i = glm::fvec3(0, 0, 0);
  }
  emit setXSliderValue(0);
  emit setYSliderValue(0);
  emit setZSliderValue(0);
  m_pose->FK();
  update();
}

void MainOpenGLWidget::mousePressEvent(QMouseEvent *event) {
  m_lastPos = event->pos();
}

void MainOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - m_lastPos.x();
  int dy = event->y() - m_lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(m_xRot + 8 * dy);
    setYRotation(m_yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(m_xRot + 8 * dy);
    setZRotation(m_zRot + 8 * dx);
  } else if (event->buttons() & Qt::MidButton) {
    m_camera.translate((GLfloat)dx / 600, -1 * (GLfloat)dy / 600, 0);
    update();
  }
  m_lastPos = event->pos();
}

void MainOpenGLWidget::wheelEvent(QWheelEvent *event) {
  if (event->delta() > 0) {
    m_camera.translate(0, 0, 0.05f);
    update();
  } else {
    m_camera.translate(0, 0, -0.05f);
    update();
  }
}