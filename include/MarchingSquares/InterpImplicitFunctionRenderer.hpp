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
  InterpImplicitFunctionRenderer(QOpenGLFunctions *gl, float width=1.0f, float height=1.0f);

  void setRendererSize(float width, float height);

  
  void init(const InterpolatingImplicitFunction2D &interp);
  void loadVBOs(const InterpolatingImplicitFunction2D &interp);

  void draw();

  void keyPressedEvent(QKeyEvent *e);
  void keyReleasedEvent(QKeyEvent *e);

private:
  void initShaders();
  void initBuffers(const InterpolatingImplicitFunction2D &interp);

  void updateProjectionMatrix();

private:
  float width_;
  float height_;

  QOpenGLFunctions *gl_;

  QOpenGLVertexArrayObject vaoPolygon_;
  QOpenGLBuffer coordsPolygonVBO_;

  QOpenGLVertexArrayObject vaoConstraint_;
  QOpenGLBuffer coordsConstraintVBO_;

  QOpenGLShaderProgram shaderProgram_;

  unsigned int uniColor_;
  unsigned int uniProjection_;

  unsigned int numPolygonVertices_;
  unsigned int numPosConstraints_;
  unsigned int numNegConstraints_;
  unsigned int numBoundaryConstraints_;

  bool showConstraints_;
};