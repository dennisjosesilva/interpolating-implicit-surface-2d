#include "App/GraphicsViewWidget.hpp"
#include "implicit/InterpolatingImplicitFunction2d.hpp"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_1_Core>

#include <QtMath>

GraphicsViewWidget::GraphicsViewWidget(QWidget *parent)
  : QOpenGLWidget{parent}, renderer_{nullptr}
{
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void GraphicsViewWidget::initializeGL()
{
  using ImplicitFunction = Polygonizer::ImplicitFunction;

  initializeOpenGLFunctions();

  QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();

  // ===============================================
  // CIRCLE IMPLICIT FUNCTION
  // =============================================== 
  // float r = 0.5;
  // ImplicitFunction f = [r](const QVector2D &p) {
  //   return p.x()*p.x() + p.y()*p.y() - r;
  // };
  InterpolatingImplicitFunction2D interp;

  interp.pushBoundaryConstraint(QVector2D{-0.5f,  0.0f});
  interp.pushBoundaryConstraint(QVector2D{ 0.0f,  0.5f});
  interp.pushBoundaryConstraint(QVector2D{ 0.5f,  0.0f});
  interp.pushBoundaryConstraint(QVector2D{ 0.0f, -0.5f});

  interp.pushInteriorConstraint(QVector2D{ 0.0f, 0.0f});

  // ImplicitFunction f = interp.optimize();


  // ===============================================
  // POLYGONIZE IMPLICIT FUNCTION
  // ===============================================
  // Polygonizer polygonizer{f, {-1.5f, 1.5f}, {1.5f, -1.5f}, 0.25, 0.0f};
  // Polygon polygon = polygonizer.polygonize();

  renderer_ = std::make_unique<InterpImplicitFunctionRenderer>(gl);

  // interp.optimize();

  renderer_->init(interp);
  setFocus();
}

void GraphicsViewWidget::paintGL()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  renderer_->draw();
}

void GraphicsViewWidget::keyPressEvent(QKeyEvent *e)
{
  QOpenGLWidget::keyPressEvent(e);
  qDebug() << "Pass here";
  renderer_->keyPressedEvent(e);
  update();
}

void GraphicsViewWidget::keyReleaseEvent(QKeyEvent *e)
{}