#pragma once 

#include <QVector>
#include <QPoint>
#include <QVector2D>

class ContourTracer
{
public:
  ContourTracer(const QVector<bool> &contourImg, int width, int height);
  QVector<float> traceFlat();
  QVector<QVector2D> tracePoint();

private:
  // =============================================================
  // AUX PRIVATE METHODS
  // =============================================================
  QVector<float> traceContour(const QPoint &p);
  
  bool imgContains(const QPoint &p) const;
  int index(const QPoint &p) const;
  QPoint point(int idx) const;


  // ==============================================================
  // ATTRIBUTES
  // ==============================================================
  int width_;
  int height_;
  const QVector<bool> &contourImg_;
  QVector<bool> visited_;

  static const QVector<QPoint> Dir;
};

QVector<float> traceContourFlat(const QVector<bool> &bimg, 
  int width, int height);
QVector<QVector2D> traceContourPoint(const QVector<bool> &bimg,
  int width, int height);