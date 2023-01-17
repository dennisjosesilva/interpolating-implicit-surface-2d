#pragma once 

#include "DistanceTransform/DTComputer.hpp"

QVector<float> computeDistanceTransform(QOpenGLFunctions *gl,
  const QVector<bool> &bimg, int width, int height)
{
  DTComputer comp{gl};
  comp.init();
  return comp.compute(width, height, bimg);
}