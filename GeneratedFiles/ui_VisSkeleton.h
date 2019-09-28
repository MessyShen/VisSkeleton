/********************************************************************************
** Form generated from reading UI file 'VisSkeleton.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISSKELETON_H
#define UI_VISSKELETON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "AngleSlider.h"
#include "MainOpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_VisSkeletonClass
{
public:
    QWidget *centralWidget;
    MainOpenGLWidget *openGLWidget;
    AngleSlider *xSlider;
    AngleSlider *ySlider;
    AngleSlider *zSlider;
    QLabel *xlabel;
    QSpinBox *spinBox;
    QLabel *xlabel_2;
    QLabel *xlabel_3;
    QPushButton *pushButton;
    QLabel *xMinLabel;
    QLabel *xMaxLabel;
    QLabel *yMinLabel;
    QLabel *yMaxLabel;
    QLabel *zMinLabel;
    QLabel *zMaxLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VisSkeletonClass)
    {
        if (VisSkeletonClass->objectName().isEmpty())
            VisSkeletonClass->setObjectName(QString::fromUtf8("VisSkeletonClass"));
        VisSkeletonClass->resize(925, 1150);
        centralWidget = new QWidget(VisSkeletonClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        openGLWidget = new MainOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(20, 50, 881, 771));
        xSlider = new AngleSlider(centralWidget);
        xSlider->setObjectName(QString::fromUtf8("xSlider"));
        xSlider->setGeometry(QRect(120, 850, 621, 31));
        xSlider->setMinimum(-180);
        xSlider->setMaximum(180);
        xSlider->setSingleStep(1);
        xSlider->setValue(0);
        xSlider->setSliderPosition(0);
        xSlider->setOrientation(Qt::Horizontal);
        ySlider = new AngleSlider(centralWidget);
        ySlider->setObjectName(QString::fromUtf8("ySlider"));
        ySlider->setGeometry(QRect(120, 890, 621, 31));
        ySlider->setMinimum(-180);
        ySlider->setMaximum(180);
        ySlider->setSingleStep(1);
        ySlider->setValue(0);
        ySlider->setOrientation(Qt::Horizontal);
        zSlider = new AngleSlider(centralWidget);
        zSlider->setObjectName(QString::fromUtf8("zSlider"));
        zSlider->setGeometry(QRect(120, 930, 621, 31));
        zSlider->setMinimum(-180);
        zSlider->setMaximum(180);
        zSlider->setPageStep(10);
        zSlider->setValue(0);
        zSlider->setOrientation(Qt::Horizontal);
        xlabel = new QLabel(centralWidget);
        xlabel->setObjectName(QString::fromUtf8("xlabel"));
        xlabel->setGeometry(QRect(40, 850, 51, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(22);
        xlabel->setFont(font);
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(390, 1020, 101, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Agency FB"));
        font1.setPointSize(24);
        spinBox->setFont(font1);
        spinBox->setMaximum(18);
        xlabel_2 = new QLabel(centralWidget);
        xlabel_2->setObjectName(QString::fromUtf8("xlabel_2"));
        xlabel_2->setGeometry(QRect(40, 890, 51, 31));
        xlabel_2->setFont(font);
        xlabel_3 = new QLabel(centralWidget);
        xlabel_3->setObjectName(QString::fromUtf8("xlabel_3"));
        xlabel_3->setGeometry(QRect(40, 930, 51, 31));
        xlabel_3->setFont(font);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(810, 910, 81, 41));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Agency FB"));
        font2.setPointSize(12);
        pushButton->setFont(font2);
        xMinLabel = new QLabel(centralWidget);
        xMinLabel->setObjectName(QString::fromUtf8("xMinLabel"));
        xMinLabel->setGeometry(QRect(120, 850, 54, 12));
        xMaxLabel = new QLabel(centralWidget);
        xMaxLabel->setObjectName(QString::fromUtf8("xMaxLabel"));
        xMaxLabel->setGeometry(QRect(690, 850, 54, 12));
        xMaxLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yMinLabel = new QLabel(centralWidget);
        yMinLabel->setObjectName(QString::fromUtf8("yMinLabel"));
        yMinLabel->setGeometry(QRect(120, 890, 54, 12));
        yMaxLabel = new QLabel(centralWidget);
        yMaxLabel->setObjectName(QString::fromUtf8("yMaxLabel"));
        yMaxLabel->setGeometry(QRect(690, 890, 54, 12));
        yMaxLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMinLabel = new QLabel(centralWidget);
        zMinLabel->setObjectName(QString::fromUtf8("zMinLabel"));
        zMinLabel->setGeometry(QRect(120, 930, 54, 12));
        zMaxLabel = new QLabel(centralWidget);
        zMaxLabel->setObjectName(QString::fromUtf8("zMaxLabel"));
        zMaxLabel->setGeometry(QRect(690, 930, 54, 12));
        zMaxLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        VisSkeletonClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VisSkeletonClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 925, 22));
        VisSkeletonClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VisSkeletonClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        VisSkeletonClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VisSkeletonClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        VisSkeletonClass->setStatusBar(statusBar);

        retranslateUi(VisSkeletonClass);
        QObject::connect(xSlider, SIGNAL(valueChanged(int)), xlabel, SLOT(setNum(int)));
        QObject::connect(ySlider, SIGNAL(valueChanged(int)), xlabel_2, SLOT(setNum(int)));
        QObject::connect(zSlider, SIGNAL(valueChanged(int)), xlabel_3, SLOT(setNum(int)));
        QObject::connect(pushButton, SIGNAL(clicked()), openGLWidget, SLOT(resetPose()));

        QMetaObject::connectSlotsByName(VisSkeletonClass);
    } // setupUi

    void retranslateUi(QMainWindow *VisSkeletonClass)
    {
        VisSkeletonClass->setWindowTitle(QApplication::translate("VisSkeletonClass", "VisSkeleton", nullptr));
        xlabel->setText(QApplication::translate("VisSkeletonClass", "0", nullptr));
        xlabel_2->setText(QApplication::translate("VisSkeletonClass", "0", nullptr));
        xlabel_3->setText(QApplication::translate("VisSkeletonClass", "0", nullptr));
        pushButton->setText(QApplication::translate("VisSkeletonClass", "Reset", nullptr));
        xMinLabel->setText(QApplication::translate("VisSkeletonClass", "TextLabel", nullptr));
        xMaxLabel->setText(QApplication::translate("VisSkeletonClass", "TextLabel", nullptr));
        yMinLabel->setText(QApplication::translate("VisSkeletonClass", "TextLabel", nullptr));
        yMaxLabel->setText(QApplication::translate("VisSkeletonClass", "TextLabel", nullptr));
        zMinLabel->setText(QApplication::translate("VisSkeletonClass", "TextLabel", nullptr));
        zMaxLabel->setText(QApplication::translate("VisSkeletonClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VisSkeletonClass: public Ui_VisSkeletonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISSKELETON_H
