#pragma once

#include <QOpenGLWidget>

#include "MarchingSquares/InterpImplicitFunctionRenderer.hpp"

#include <memory>

class GraphicsViewWidget : public QOpenGLWidget, QOpenGLFunctions 
{
Q_OBJECT
public:
  GraphicsViewWidget(QWidget *parent=nullptr);
  ~GraphicsViewWidget() { }

  QSize sizeHint() const override { return QSize{720, 720}; }
  
protected:
  void initializeGL() override;  
  void paintGL() override;

protected:
  std::unique_ptr<InterpImplicitFunctionRenderer> renderer_;
};