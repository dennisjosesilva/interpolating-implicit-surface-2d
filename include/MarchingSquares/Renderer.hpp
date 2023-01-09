#pragma once 

#include <QOpenGLFunctions>

#include "MarchingSquares/Polygonizer.hpp"

#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QMatrix4x4>

class Renderer
{
public:
  Renderer(QOpenGLFunctions *gl, const Polygon &polygon,
    float width = 1.0f, float height = 1.0f);

  void setRendererSize(float width, float height);

  void initShaders();
  void initBuffers();

  void draw();

  void destroy();

private:
  void updateProjectionMatrix();

private:
  float width_;
  float height_;

  QOpenGLFunctions *gl_;
  Polygon polygon_;

  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer coordsVBO_;

  QOpenGLShaderProgram shaderProgram_;
};