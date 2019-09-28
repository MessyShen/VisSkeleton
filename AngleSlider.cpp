#include "AngleSlider.h"
#include <qslider.h>

AngleSlider::AngleSlider(QWidget *parent) : QSlider(parent) {}
AngleSlider::~AngleSlider() {}

void AngleSlider::setSliderMin(int value) { 
  this->setMinimum(value-spareDeg); 
}

void AngleSlider::setSliderMax(int value) { 
  this->setMaximum(value+spareDeg); 
}

void AngleSlider::setSliderValue(int value) { this->setValue(value); }
