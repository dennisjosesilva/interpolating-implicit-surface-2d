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

  void initializeGL() override;  
  void paintGL() override;

protected:
  MainWidget *mainWidget_;
  std::unique_ptr<InterpImplicitFunctionRenderer> renderer_;
  QImage curImage_;
  int margin_;
};