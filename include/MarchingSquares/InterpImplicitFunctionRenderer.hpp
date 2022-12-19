#pragma once 

#include <QOpenGLFunctions>

#include "MarchingSquares/Polygonizer.hpp"
#include "implicit/InterpolatingImplicitFunction2d.hpp"

#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QKeyEvent>

class InterpImplicitFunctionRenderer
{
public:
  InterpImplicitFunctionRenderer(QOpenGLFunctions *gl);

  void init(const InterpolatingImplicitFunction2D &interp);

  void draw();

  void keyPressedEvent(QKeyEvent *e);
  void keyReleasedEvent(QKeyEvent *e);

private:
  void initShaders();
  void initBuffers(const InterpolatingImplicitFunction2D &interp);

private:
  QOpenGLFunctions *gl_;

  QOpenGLVertexArrayObject vaoPolygon_;
  QOpenGLBuffer coordsPolygonVBO_;

  QOpenGLVertexArrayObject vaoConstraint_;
  QOpenGLBuffer coordsConstraintVBO_;

  QOpenGLShaderProgram shaderProgram_;

  unsigned int uniColor_;

  unsigned int numPolygonVertices_;
  unsigned int numPosConstraints_;
  unsigned int numNegConstraints_;
  unsigned int numBoundaryConstraints_;

  bool showConstraints_;
};