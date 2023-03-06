#include "App/GraphicsViewWidget.hpp"
#include "implicit/InterpolatingImplicitFunction2d.hpp"
#include "Skeleton/SkeletonComputer.hpp"
#include "Contour/ContourComputer.hpp"

#include <memory>

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Compatibility>

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
    // extract a level-set of the image           DONE
    // extract the contours of the level sets     DONE  
    // compute boundary and internal points       DONE (using medial axis-skeleton)
    // compute implicit function      
    
    int N = 0; // number of foreground pixels      
    QVector2D centroid{0, 0};
        
    QVector<bool> bimg(curImage_.width() * curImage_.height(), false);
    for (int y = 0; y < curImage_.height(); y++) {
      for (int x = 0; x < curImage_.width(); x++) {
        if (qGray(curImage_.pixel(x, y)) <= 10) {
          bimg[y*curImage_.width() + x] = true;
          N++;
          centroid.setX(centroid.x() + x);
          centroid.setY(centroid.y() + y);
        }
      }
    }
    centroid /= N;

    QVector<QVector2D> skel = extractSkeletonPoints(curImage_.size(), bimg);
    QVector<QVector2D> contours = extractContourPoints(curImage_.size(), bimg);

    float HW = width() / 2.0f;
    float HH = height() / 2.0f;

    float hw = curImage_.width() / 2.0f;
    float hh = curImage_.height() / 2.0f;


    InterpolatingImplicitFunction2D interp;
    // for (int i = 0; i < skel.count(); i += 25) {
    //   const QVector2D &s = skel[i];
    //   QVector2D p{ (s.x() - hw) + HW, HH - (s.y() - hh) };
    //   interp.pushInteriorConstraint(p);
    // }

    // Compute a unique interior constraint.
    QVector2D ic{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    float icdist = std::numeric_limits<float>::max();
    for (const QVector2D& s : skel) {
      float sdist = centroid.distanceToPoint(s);
      if (sdist < icdist) {
        ic = s;
        icdist = sdist;
      }
    }
    QVector2D convic{ (ic.x() - hw) + HW, HH - (ic.y() - hh) };
    interp.pushInteriorConstraint(convic);

    for (int i = 0; i < contours.size(); i += 25) {
      QVector2D p{ (contours[i].x() - hw) + HW, HH - (contours[i].y() - hh) };
      interp.pushBoundaryConstraint(p);
    }

    // for (const QVector2D &c : contours) {
    //   interp.pushBoundaryConstraint(c);
    // }

    // float left = width() / 4.0f;
    // float right = width() * (3.0f/4.0f);
    // float bottom = height() / 4.0f;
    // float top = height() * (3.0f/4.0f);

    // float vhalf = height() / 2.0f;
    // float hhalf = width()  / 2.0f;

    // InterpolatingImplicitFunction2D interp;
    // interp.pushBoundaryConstraint({left, top});
    // interp.pushBoundaryConstraint({right, top});
    // interp.pushBoundaryConstraint({left, bottom});
    // interp.pushBoundaryConstraint({right, bottom});

    // interp.pushInteriorConstraint({hhalf, vhalf});

    renderer_->setRendererSize(width(), height());
    renderer_->loadVBOs(interp);
    update();
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

  QSurfaceFormat format;
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
  QOpenGLContext::currentContext()->setFormat(format);

  initializeOpenGLFunctions();

  // QOpenGLFunctions_4_3_Compatibility *gl = 
  //   QOpenGLContext::currentContext()
  //     ->versionFunctions<QOpenGLFunctions_4_3_Compatibility>();

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
  glViewport(0, 0, width(), height());
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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