

#include "DistanceTransform/DTComputer.hpp"
#include <cmath>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Compatibility>

DTComputer::DTComputer(QOpenGLFunctions_4_3_Compatibility *gl)
  :gl_{gl}
{  
}

DTComputer::~DTComputer()
{
  destroy();
}

void DTComputer::destroy()
{
  shader_.release();
}

void DTComputer::init()
{
  setUpShaders();
}

int DTComputer::pixelIndex(int width, int height,
  const QVector2D &p) const 
{
  return (width * int(p.y())) + int(p.x());
}

int DTComputer::pixelIndex(int width, int height, 
  int x, int y) const
{
  return (width*y) + x;
}

void DTComputer::setUpShaders()
{
  shader_.create();
  shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, 
    ":/shaders/DistanceTransform/dt.vs");
  shader_.addShaderFromSourceFile(QOpenGLShader::Geometry,
    ":/shaders/DistanceTransform/dt.gs");
  shader_.addShaderFromSourceFile(QOpenGLShader::Fragment,
    ":/shaders/DistanceTransform/dt.fs");
  shader_.link();
}

QVector<QVector2D> DTComputer::traceBoundaries(int width, int height, 
  const QVector<bool> &bimg) const 
{
  QVector<QVector2D> b;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if(bimg[pixelIndex(width, height, x, y)] 
        && isBoundary(width, height, bimg, x, y))
        b.append(QVector2D{float(x), float(y)});
    }
  }
  return b;
}

bool DTComputer::isBoundary(int width, int height, 
  const QVector<bool> &bimg, int x, int y) const 
{
  QVector<QVector2D> neighborhood = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
  QVector2D p{float(x), float(y)};

  for (const QVector2D &offset : neighborhood) {
    QVector2D q = p + offset;
    if ((q.x() < 0 || width <= q.x() || q.y() < 0 || height <= q.y()) ||
      !bimg[pixelIndex(width, height, q)])
      return true;
  }
  return false;
}

bool DTComputer::isBoundary(int width, int height, 
  const QVector<bool> &bimg, const QVector2D &p) const 
{
  QVector<QVector2D> neighborhood = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

  for (const QVector2D &offset : neighborhood) {
    QVector2D q = p + offset;
    if ((0 <= q.x() && q.x() < width && 0 < q.y() && q.y() < height) ||
      !bimg[pixelIndex(width, height, q)])
      return true;
  }
  return false; 
}

void DTComputer::genSplatTexture(int width, int height)
{
  int R = splatRadius_;
  int S = splatDiameter_;
  float dt[S][S];

  for (int i = 0; i < S; i++) {
    for (int j = 0; j < S; j++) {
      float dst = sqrtf32((i-R)*(i-R) + (j-R)*(j-R));
      if (dst > R)
        dst = R;
      dt[i][j] = dst / static_cast<float>(R);
    }
  }

  gl_->glGenTextures(1, &splatTexture_);
  gl_->glBindTexture(GL_TEXTURE_2D, splatTexture_);
  gl_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  gl_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gl_->glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, S, S, 0, GL_RED, GL_FLOAT,
    dt);
  gl_->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void DTComputer::setupFramebuffer(int width, int height)
{
  gl_->glGenFramebuffers(1, &framebuffer_);
  gl_->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

  gl_->glGenTextures(1, &texFramebuffer_);
  gl_->glBindTexture(GL_TEXTURE_2D, texFramebuffer_);
  gl_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
    GL_RGB, GL_FLOAT, nullptr);
  gl_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gl_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
    GL_TEXTURE_2D, texFramebuffer_, 0);

  gl_->glGenRenderbuffers(1, &rbo_);
  gl_->glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  gl_->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
    width, height);
  gl_->glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
    GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_); 
}

QVector<float> DTComputer::compute(int width, int height, 
  const QVector<bool> &bimg)
{
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo;

  vao.create();
  QOpenGLVertexArrayObject::Binder vaoBinder{&vao};
  
  splatDiameter_ = std::min(width, height);
  splatRadius_ = splatDiameter_ / 2;

  QVector<QVector2D> b = traceBoundaries(width, height, bimg);

  shader_.bind();
  genSplatTexture(width, height);
  setupFramebuffer(width, height);

  if (gl_->glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    qDebug() << "Framebuffer completed.";

  gl_->glEnable(GL_BLEND);
  gl_->glBlendEquation(GL_MIN);
  gl_->glBlendFunc(GL_ONE, GL_ONE);
  
  vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo.create();
  vbo.bind();
  vbo.allocate(b.constData(), b.count() * sizeof(QVector2D));
  shader_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shader_.enableAttributeArray(0);

  shader_.setUniformValue("splatRadius", splatRadius_);

  QMatrix4x4 projection;
  projection.ortho(0, width, 0, height, -1.0f, 1.0f);
  shader_.setUniformValue("viewProjection", projection);

  gl_->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  gl_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  gl_->glBindTexture(GL_TEXTURE_2D, splatTexture_);
  shader_.setUniformValue("splatTexture", 0);
  gl_->glActiveTexture(GL_TEXTURE0);

  gl_->glViewport(0, 0, width, height);
  gl_->glDrawArrays(GL_POINTS, 0, b.size());

  QVector<float> dt(width*height, 0.0f);
  gl_->glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, 
    &dt[0]);
  
  gl_->glBindFramebuffer(GL_FRAMEBUFFER, 0);  // set default framebuffer back.
  gl_->glDeleteFramebuffers(1, &framebuffer_);
  gl_->glDeleteTextures(1, &texFramebuffer_);
  gl_->glDeleteTextures(1, &splatTexture_);
  gl_->glDeleteRenderbuffers(1, &rbo_);
  vbo.destroy();

  return dt;
}