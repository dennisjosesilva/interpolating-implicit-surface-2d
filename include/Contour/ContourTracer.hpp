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
  // ============================================================
  // AUX PRIVATE STRUCT
  // ============================================================
  struct PointDir 
  {
    QPoint p;
    int dir;
  };

  // =============================================================
  // AUX PRIVATE METHODS
  // =============================================================
  QVector<float> traceContour(const QPoint &p);
  PointDir findNextPoint(const QPoint &curPoint, int dir) const;

  bool background(const QPoint &p) const { return !foreground(p); }
  bool foreground(const QPoint &p) const;

  bool imgContains(const QPoint &p) const;
  int index(const QPoint &p) const;
  QPoint point(int idx) const;


  // ==============================================================
  // ATTRIBUTES
  // ==============================================================
  int width_;
  int height_;
  const QVector<bool> &contourImg_;

  static const QVector<QPoint> Dir;
};

QVector<float> traceContourFlat(const QVector<bool> &bimg, 
  int width, int height);

QVector<float> traceContourFlat(const QSize &size,
  const QVector<bool> &bimg);

QVector<QVector2D> traceContourPoint(const QVector<bool> &bimg,
  int width, int height);

QVector<QVector2D> traceContourPoint(const QSize &size,
  const QVector<bool> &bimg);