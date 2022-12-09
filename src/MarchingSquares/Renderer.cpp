#include "MarchingSquares/Renderer.hpp"

Renderer::Renderer(QOpenGLFunctions *gl, const Polygon &polygon)
  :gl_{gl}, polygon_{polygon} 
{}

void Renderer::initShaders()
{
  shaderProgram_.create();
  shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex,
    ":/shaders/simple.vs");
  shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment,
    ":/shaders/simple.fs");
  shaderProgram_.link();
}

void Renderer::initBuffers()
{
  const QVector<QVector2D> &coords = polygon_.vertPos;

  shaderProgram_.bind();
  vao_.create();
  QOpenGLVertexArrayObject::Binder{&vao_};

  coordsVBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  coordsVBO_.create();
  coordsVBO_.bind();
  coordsVBO_.allocate(coords.constData(), coords.count() * sizeof(QVector2D));
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);

  shaderProgram_.release();
}

void Renderer::draw()
{
  shaderProgram_.bind();
  QOpenGLVertexArrayObject::Binder {&vao_};

  gl_->glDrawArrays(GL_LINES, 0, polygon_.vertPos.count());
}

void Renderer::destroy()
{
  coordsVBO_.destroy();
  vao_.destroy();
}