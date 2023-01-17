#pragma once 

#include "DistanceTransform/DTComputer.hpp"
#include <QOpenGLFunctions_4_3_Compatibility>

QVector<float> computeDistanceTransform(QOpenGLFunctions_4_3_Compatibility *gl,
  const QVector<bool> &bimg, int width, int height)
{
  DTComputer comp{gl};
  comp.init();
  return comp.compute(width, height, bimg);
}