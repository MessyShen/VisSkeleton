#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisSkeleton.h"

class VisSkeleton : public QMainWindow
{
    Q_OBJECT

public:
    VisSkeleton(QWidget *parent = Q_NULLPTR);

private:
    Ui::VisSkeletonClass ui;
};
