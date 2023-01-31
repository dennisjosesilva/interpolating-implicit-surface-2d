#include "App/GraphicsViewWidget.hpp"
#include "implicit/InterpolatingImplicitFunction2d.hpp"
#include "Skeleton/SkeletonComputer.hpp"
#include "Contour/ContourComputer.hpp"

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
    // extract the contours of the level sets     
    // compute boundary and internal points       DONE (using medial axis-skeleton)
    // compute implicit function                
    QVector<bool> bimg(curImage_.width() * curImage_.height(), false);
    for (int y = 0; y < curImage_.height(); y++) {
      for (int x = 0; x < curImage_.width(); x++) {
        if (qGray(curImage_.pixel(x, y)) <= 10)
          bimg[y*curImage_.width() + x] = true;
      }
    }

    
    // QVector<bool> skel = computeSkeleton(curImage_.size(), bimg);
    QVector<bool> contours = computeContours(curImage_.size(), bimg);

    QImage cImg { curImage_.size(), QImage::Format_ARGB32 };
    for (int l = 0; l < curImage_.height(); l++) {
      QRgb *line = reinterpret_cast<QRgb*>(cImg.scanLine(l));
      for (int c = 0; c < curImage_.width(); c++) {
        int greylevel = 255;

        if (bimg[curImage_.width() * l + c])
          greylevel = 0;

        if (contours[curImage_.width() * l + c])
          greylevel = 127;
        
        line[c] = qRgba(greylevel, greylevel, greylevel, 255);
      }
    }

    // The commented lines below is not work altough I expect it should.
    // QImage dtImage{curImage_.size(), QImage::Format_Grayscale8};
    // unsigned char *dtImageData = dtImage.bits();
    
    // for (int pidx = 0; pidx < edt.count(); pidx++) {
    //    dtImageData[pidx] = 255 * edt[pidx];
    // }

    // QImage skelImg{ curImage_.size(), QImage::Format_ARGB32};
    // for (int l = 0; l < curImage_.height(); l++) {
    //   QRgb *line = reinterpret_cast<QRgb*>(skelImg.scanLine(l));
    //   for (int c = 0; c < curImage_.width(); c++) {
    //     int greyLevel = 255;

    //     if (bimg[curImage_.width() * l + c])
    //       greyLevel = 0;
        
    //     if (skel[curImage_.width() * l + c])
    //       greyLevel = 127;
        
    //     line[c] = qRgba(greyLevel, greyLevel, greyLevel, 255);
    //   }
    // }

    curImage_.save("curImage.png");
    cImg.save("contours.png");
    // skelImg.save("skel.png");
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