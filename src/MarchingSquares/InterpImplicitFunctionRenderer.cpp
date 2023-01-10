#include "MarchingSquares/InterpImplicitFunctionRenderer.hpp"

InterpImplicitFunctionRenderer::InterpImplicitFunctionRenderer(
  QOpenGLFunctions *gl, float width, float height) 
  : gl_{gl}, width_{width}, height_{height}, showConstraints_{true}
{}

void InterpImplicitFunctionRenderer::initShaders()
{
  shaderProgram_.create();
  shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, 
    ":shaders/simple.vs");
  shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment,
    ":shaders/simple.fs");
  shaderProgram_.link();

  uniColor_ = shaderProgram_.uniformLocation("color");
  uniProjection_ = shaderProgram_.uniformLocation("projection");
  updateProjectionMatrix();
}

void InterpImplicitFunctionRenderer::initBuffers(
  const InterpolatingImplicitFunction2D &interp)
{
  using ImplicitFunction = Polygonizer::ImplicitFunction;

  ImplicitFunction f = interp.optimize();
  
  Polygonizer polygonizer {f, {-10.0f, height_+10.0f}, 
    {width_+10.0f, -10.0f}, 20.0f, 0.0f};  
  Polygon polygon = polygonizer.polygonize();

  numPolygonVertices_ = polygon.vertPos.count();

  QVector<QVector2D> interiorConsraint;
  QVector<QVector2D> exteriorConstraint;
  QVector<QVector2D> boundaryConstraint;

  for (int i = 0; i < interp.numConstraints(); i++) {
    if (interp.isInteriorConstraint(i)) 
      interiorConsraint.append(interp.constraint(i));
    if (interp.isExteriorConstraint(i)) 
      exteriorConstraint.append(interp.constraint(i));
    else  
      boundaryConstraint.append(interp.constraint(i));
  }

  numPosConstraints_ = interiorConsraint.count();
  numNegConstraints_ = exteriorConstraint.count();
  numBoundaryConstraints_ = boundaryConstraint.count();

   QVector<QVector2D> allConstraints = boundaryConstraint 
     + interiorConsraint + exteriorConstraint;  

  vaoConstraint_.create();
  vaoConstraint_.bind();
  shaderProgram_.bind();
  
  coordsConstraintVBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  coordsConstraintVBO_.create();
  coordsConstraintVBO_.bind();
  coordsConstraintVBO_.allocate(allConstraints.constData(), 
    allConstraints.count() * sizeof(QVector2D));
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);

  vaoConstraint_.release();
  shaderProgram_.release();

  vaoPolygon_.create();
  vaoPolygon_.bind();
  shaderProgram_.bind();

  const QVector<QVector2D> &coords = polygon.vertPos;
  coordsPolygonVBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  coordsPolygonVBO_.create();
  coordsPolygonVBO_.bind();
  coordsPolygonVBO_.allocate(coords.constData(), 
    coords.count() * sizeof(QVector2D));
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);

  vaoPolygon_.release();
}

void InterpImplicitFunctionRenderer::updateProjectionMatrix()
{
  QMatrix4x4 proj;
  proj.ortho(0.0f, width_, 0.0f, height_, -1.0f, 1.0f);
  shaderProgram_.bind();
  shaderProgram_.setUniformValue(uniProjection_, proj);
  shaderProgram_.release();
}

void InterpImplicitFunctionRenderer::init(
  const InterpolatingImplicitFunction2D &interp)
{
  gl_->glEnable(GL_PROGRAM_POINT_SIZE);
  initShaders();
  initBuffers(interp);
}

void InterpImplicitFunctionRenderer::draw()
{  
  vaoPolygon_.bind();
  shaderProgram_.bind();
  shaderProgram_.setUniformValue(uniColor_, QVector3D{1.0f, 1.0f, 1.0f});
  gl_->glDrawArrays(GL_LINES, 0, numPolygonVertices_);
  vaoPolygon_.release();

  vaoConstraint_.bind();
  shaderProgram_.bind();
  
  if (showConstraints_) {
    if (numBoundaryConstraints_ > 0) {
      shaderProgram_.setUniformValue(uniColor_, QVector3D{1.0f, 1.0f, 1.0f});
      gl_->glDrawArrays(GL_POINTS, 0, numBoundaryConstraints_);
    }

    if (numPosConstraints_ > 0) {
      shaderProgram_.setUniformValue(uniColor_, QVector3D{0.0f, 0.0f, 1.0f});
      gl_->glDrawArrays(GL_POINTS, numBoundaryConstraints_, numPosConstraints_);
    }

    if (numNegConstraints_ > 0) {
      shaderProgram_.setUniformValue(uniColor_, QVector3D{1.0f, 0.0f, 0.0f});
      gl_->glDrawArrays(GL_POINTS, 
        numBoundaryConstraints_+numPosConstraints_, numNegConstraints_);
    }
  }

  shaderProgram_.release();
  vaoConstraint_.release();
}

void InterpImplicitFunctionRenderer::keyPressedEvent(QKeyEvent *e)
{
  switch (e->key())
  {
  case Qt::Key_C:
    showConstraints_ = !showConstraints_;
    break;
  }
}

void InterpImplicitFunctionRenderer::keyReleasedEvent(QKeyEvent *e)
{}