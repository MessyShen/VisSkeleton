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
#include <qslider.h>
#include "Joint.h"
#include "Pose.h"

class AngleSlider : public QSlider {
  Q_OBJECT

 public:
  AngleSlider(QWidget *parent = 0);
  ~AngleSlider();

 public slots:
  void setSliderMin(int minV);
  void setSliderMax(int maxV);
  void setSliderValue(int value);

signals:

 private:
  int spareDeg = 0;
};