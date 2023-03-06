#pragma once

#include "Contour/ContourTracer.hpp"

QVector<float> extractSimplifiedContourFlat(const QVector<bool> &bimg, 
  int width, int height, int npoints);

QVector<float> extractSimplifiedContourFlat(const QSize &size,
  const QVector<bool> &bimg, int npoints);

QVector<QVector2D> extractSimplifiedContourPoint(
  const QVector<bool> &bimg, int width, int height, int npoints);

QVector<QVector2D> extractSimplifiedContourPoint(const QSize &size,
  const QVector<bool> &bimg,  int npoints);

QVector<float> simplifyContourFlat(const QVector<float> &contour, 
  int npoints);

QVector<float> simplifyContourFlat(const QVector<QVector2D> &contour, 
  int npoints);

QVector<QVector2D> simplifyContourPoint(const QVector<float> &contour, 
  int npoints);

QVector<QVector2D> simplifyContourPoint(const QVector<QVector2D> &contour, 
  int npoints);