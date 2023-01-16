#include "App/GraphicsViewWidget.hpp"
#include "implicit/InterpolatingImplicitFunction2d.hpp"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_1_Core>

#include <QtMath>

#include <QMessageBox>

GraphicsViewWidget::GraphicsViewWidget(QWidget *parent)
  : QOpenGLWidget{parent}, renderer_{nullptr}
{
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void GraphicsViewWidget::loadImage(const QString &filename)
{
  if (curImage_.load(filename) && curImage_.allGray()) {
    // TODO: load image

    // TODO: 
    // extract a level-set of the image
    // extract the contours of the level sets
    // compute boundary and internal points
    // compute implicit function 
    curImage_.da

  }
  else {
    QMessageBox::warning(this, tr("Wrong image format"), 
      tr("The image couldn't be loaded or it is a colored image.\n"
         "Currently, the program accepts only grayscale images."), 
         QMessageBox::Ok, QMessageBox::Ok);
  }
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

  float left = width() / 4.0f;
  float right = width() * (3.0f/4.0f);
  float bottom = width() / 4.0f;
  float top = width() * (3.0f/4.0f);

  float vhalf = height() / 2.0f;
  float hhalf = width()  / 2.0f;

  interp.pushBoundaryConstraint(QVector2D{ left,  vhalf});
  interp.pushBoundaryConstraint(QVector2D{ hhalf,  top});
  interp.pushBoundaryConstraint(QVector2D{ right,  vhalf});
  interp.pushBoundaryConstraint(QVector2D{ hhalf, bottom});

  interp.pushInteriorConstraint(QVector2D{ hhalf, vhalf});

  qDebug() << width();

  // ImplicitFunction f = interp.optimize();


  // ===============================================
  // POLYGONIZE IMPLICIT FUNCTION
  // ===============================================
  // Polygonizer polygonizer{f, {-1.5f, 1.5f}, {1.5f, -1.5f}, 0.25, 0.0f};
  // Polygon polygon = polygonizer.polygonize();

  renderer_ = std::make_unique<InterpImplicitFunctionRenderer>(gl, 
    static_cast<float>(width()), static_cast<float>(height()));

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
  renderer_->keyPressedEvent(e);
  update();
}

void GraphicsViewWidget::keyReleaseEvent(QKeyEvent *e)
{}