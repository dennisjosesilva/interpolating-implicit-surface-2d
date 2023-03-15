#pragma once

#include <QOpenGLWidget>
#include "MarchingSquares/InterpImplicitFunctionRenderer.hpp"

#include <memory>

#include <QKeyEvent>
#include <QImage>


class MainWidget;

class GraphicsViewWidget : public QOpenGLWidget, QOpenGLFunctions 
{
Q_OBJECT
public:
  GraphicsViewWidget(MainWidget *parent=nullptr);
  ~GraphicsViewWidget() { }

  QSize sizeHint() const override { return QSize{720, 720}; }
  
  void updateImplicitSurface();


  bool loadImage(const QString &filename);

  void keyPressEvent(QKeyEvent *e) override;
  void keyReleaseEvent(QKeyEvent *e) override;

protected:
  void computeInteriorConstraintSampledSkeletonPoints(
    const QVector<bool> &bimg,
    InterpolatingImplicitFunction2D &interp);
  
  void computeInteriorConstraintCentralSkeletonPoint(
    const QVector<bool> &bimg,
    const QVector2D &centroid,
    InterpolatingImplicitFunction2D &interp);

  void computeInteriorConstraintUsingNormals(
    const QVector<QVector2D> &contour,
    InterpolatingImplicitFunction2D &interp);

  float determinant(const QVector2D &v0, 
    const QVector2D &v1) const;
  QVector2D rotate(const QVector2D &v, float angle) const;
  QVector2D offsetNormal(const QVector2D &p, const QVector2D &n,
    float offset=1.0f);

  QVector2D computeNormal(int pidx, 
    const QVector<QVector2D> &contour);
  QVector2D computeNormalStartPoint(
    const QVector<QVector2D> &contour);
  QVector2D computeNormalEndPoint(
    const QVector<QVector2D> &contour);
  QVector2D computeNormalFromPoints(
    const QVector2D &p0, const QVector2D &p1, const QVector2D &p2);


  void initializeGL() override;  
  void paintGL() override;

protected:
  MainWidget *mainWidget_;
  std::unique_ptr<InterpImplicitFunctionRenderer> renderer_;
  QImage curImage_;
  int margin_;
};