#pragma once

#include <QOpenGLWidget>

#include "MarchingSquares/InterpImplicitFunctionRenderer.hpp"

#include <memory>

#include <QKeyEvent>
#include <QImage>


class GraphicsViewWidget : public QOpenGLWidget, QOpenGLFunctions 
{
Q_OBJECT
public:
  GraphicsViewWidget(QWidget *parent=nullptr);
  ~GraphicsViewWidget() { }

  QSize sizeHint() const override { return QSize{720, 720}; }

  void loadImage(const QString &filename);

  void keyPressEvent(QKeyEvent *e) override;
  void keyReleaseEvent(QKeyEvent *e) override;

protected:
  void initializeGL() override;  
  void paintGL() override;

protected:
  std::unique_ptr<InterpImplicitFunctionRenderer> renderer_;
  QImage curImage_;
};