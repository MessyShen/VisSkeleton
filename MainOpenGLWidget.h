#pragma once
#include "QtWidgets/qopenglwidget.h"
#include "qopenglbuffer.h"
#include "qopenglfunctions.h"
#include "qopenglfunctions_4_5_core.h"
#include "qopenglvertexarrayobject.h"

#include "qdebug.h"
#include "qevent.h"
#include "qmatrix4x4.h"

#include <Shader.h>

#include "Joint.h"
#include "Pose.h"

class MainOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  MainOpenGLWidget(QWidget *parent = 0);
  ~MainOpenGLWidget();

  static bool isTransparent() { return m_transparent; };
  static void setTransparent(bool t) { m_transparent = t; };

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  Pose *m_pose;
  Joint *m_joint;
  // Objects3D* m_object_3d;

 public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
  void cleanup();
  void setAngle(int angle);
  void setXangle(int angle);
  void setYangle(int angle);
  void setZangle(int angle);
  void setAngleLimit(int angle);
  void resetPose();

 signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);
  
  void setXSliderMin(int angle);
  void setXSliderMax(int angle);
  void setYSliderMin(int angle);
  void setYSliderMax(int angle);
  void setZSliderMin(int angle);
  void setZSliderMax(int angle);

  void setXSliderValue(int angle);
  void setYSliderValue(int angle);
  void setZSliderValue(int angle);

  //int setXsliderValue(int value);

 protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *e);
  //void setTPose();

 private:
  Shader *m_shader;
  QOpenGLFunctions_4_5_Core *m_core;

  int m_xRot;
  int m_yRot;
  int m_zRot;
  QPoint m_lastPos;      // mouse's last position
  int m_projMatrixLoc;   // vertices' projection matrix location
  int m_modelMatrixLoc;  // vertices' world matrix location
  int m_viewMatrixLoc;   // vertices' camera matrix location
  int m_aLightPosLoc;    // frag's A light position location
  int m_bLightPosLoc;    // frag's B light position location
  int m_viewPosLoc;      // frag's view position location
  QMatrix4x4 m_proj;     // project matrix
  QMatrix4x4 m_camera;   // camera transform matrix
  QMatrix4x4 m_world;    // world transform matrix
  QVector3D m_a_light_pos;
  QVector3D m_b_light_pos;
  QVector3D m_view_pos;
  static bool m_transparent;
  

  bool m_is_reload;
};