#include "VisSkeleton.h"
#include "Pose.h"
#include "ui_VisSkeleton.h"
#include <QtWidgets>
#include <qmainwindow.h>

VisSkeleton::VisSkeleton(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.spinBox, SIGNAL(valueChanged(int)), ui.openGLWidget,
            SLOT(setAngle(int)));

    connect(ui.xSlider, SIGNAL(valueChanged(int)), ui.openGLWidget,
            SLOT(setXangle(int)));

    connect(ui.ySlider, SIGNAL(valueChanged(int)), ui.openGLWidget,
            SLOT(setYangle(int)));

    connect(ui.zSlider, SIGNAL(valueChanged(int)), ui.openGLWidget,
            SLOT(setZangle(int)));

    connect(ui.openGLWidget, SIGNAL(setXSliderMin(int)), ui.xSlider,
           SLOT(setSliderMin(int)));
    connect(ui.openGLWidget, SIGNAL(setXSliderMin(int)), ui.xMinLabel,
            SLOT(setNum(int)));
    connect(ui.openGLWidget, SIGNAL(setXSliderMax(int)), ui.xSlider,
            SLOT(setSliderMax(int)));
    connect(ui.openGLWidget, SIGNAL(setXSliderMax(int)), ui.xMaxLabel,
            SLOT(setNum(int)));

    connect(ui.openGLWidget, SIGNAL(setYSliderMin(int)), ui.ySlider,
            SLOT(setSliderMin(int)));
    connect(ui.openGLWidget, SIGNAL(setYSliderMin(int)), ui.yMinLabel,
            SLOT(setNum(int)));
    connect(ui.openGLWidget, SIGNAL(setYSliderMax(int)), ui.ySlider,
            SLOT(setSliderMax(int)));
    connect(ui.openGLWidget, SIGNAL(setYSliderMax(int)), ui.yMaxLabel,
            SLOT(setNum(int)));

    connect(ui.openGLWidget, SIGNAL(setZSliderMin(int)), ui.zSlider,
            SLOT(setSliderMin(int)));
    connect(ui.openGLWidget, SIGNAL(setZSliderMin(int)), ui.zMinLabel,
            SLOT(setNum(int)));
    connect(ui.openGLWidget, SIGNAL(setZSliderMax(int)), ui.zSlider,
            SLOT(setSliderMax(int)));
    connect(ui.openGLWidget, SIGNAL(setZSliderMax(int)), ui.zMaxLabel,
            SLOT(setNum(int)));

    //setXSliderValue
    connect(ui.openGLWidget, SIGNAL(setXSliderValue(int)), ui.xSlider,
            SLOT(setSliderValue(int)));
    connect(ui.openGLWidget, SIGNAL(setYSliderValue(int)), ui.ySlider,
            SLOT(setSliderValue(int)));
    connect(ui.openGLWidget, SIGNAL(setZSliderValue(int)), ui.zSlider,
            SLOT(setSliderValue(int)));
    //connect(ui.spinBox, SIGNAL(valueChanged(int)), ui.openGLWidget, SLOT(set));
}
