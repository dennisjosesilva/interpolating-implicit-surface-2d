#pragma once 

#include <QOpenGLFunctions>

#include "MarchingSquares/Polygonizer.hpp"

#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Renderer
{
public:
  Renderer(QOpenGLFunctions *gl, const Polygon &polygon);

  void initShaders();
  void initBuffers();

  void draw();

  void destroy();

private:
  QOpenGLFunctions *gl_;
  Polygon polygon_;

  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer coordsVBO_;

  QOpenGLShaderProgram shaderProgram_;
};