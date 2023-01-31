#include "MarchingSquares/InterpImplicitFunctionRenderer.hpp"

InterpImplicitFunctionRenderer::InterpImplicitFunctionRenderer(
  QOpenGLFunctions *gl, float width, float height) 
  : gl_{gl}, width_{width}, height_{height}, showConstraints_{true}
{}


void InterpImplicitFunctionRenderer::setRendererSize(float width, float height)
{
  width_ = width;
  height_ = height;
  updateProjectionMatrix();
}

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
  // Create constraints VAO and VBO
  vaoConstraint_.create();
  vaoConstraint_.bind();
  shaderProgram_.bind();
  
  coordsConstraintVBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  coordsConstraintVBO_.create();
  coordsConstraintVBO_.bind();  
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);

  vaoConstraint_.release();
  shaderProgram_.release();

  // Create curve (polygon) VAO and VBO
  vaoPolygon_.create();
  vaoPolygon_.bind();
  shaderProgram_.bind();

  coordsPolygonVBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  coordsPolygonVBO_.create();
  coordsPolygonVBO_.bind();
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);

  vaoPolygon_.release();

  // load vertices into the buffers
  loadVBOs(interp);
}

void InterpImplicitFunctionRenderer::loadVBOs(
  const InterpolatingImplicitFunction2D &interp)
{
  // 1. Compute interpolating implicit function
  using ImplicitFunction = Polygonizer::ImplicitFunction;

  ImplicitFunction f = interp.optimize();

  // 2. Runs match squares to polygonise "f"
  Polygonizer polygonizer {f, {-10.0f, height_+10.0f}, 
    {width_+10.0f, -10.0f}, 5.0f, 0.0f};  
  Polygon polygon = polygonizer.polygonize();

  numPolygonVertices_ = polygon.vertPos.count();

  // 3. Extract different types of constraints 
  // in different vectors
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

  // 3.2 Register number of constrains for each type.
  numPosConstraints_ = interiorConsraint.count();
  numNegConstraints_ = exteriorConstraint.count();
  numBoundaryConstraints_ = boundaryConstraint.count();

   QVector<QVector2D> allConstraints = boundaryConstraint 
     + interiorConsraint + exteriorConstraint;  

  vaoConstraint_.bind();
  coordsConstraintVBO_.bind();
  coordsConstraintVBO_.allocate(allConstraints.constData(), 
    allConstraints.count() * sizeof(QVector2D));
  coordsConstraintVBO_.release();
  vaoConstraint_.release();

  const QVector<QVector2D> &coords = polygon.vertPos;
  vaoPolygon_.bind();
  coordsPolygonVBO_.bind();
  coordsPolygonVBO_.allocate(coords.constData(), 
    coords.count() * sizeof(QVector2D));
  coordsPolygonVBO_.release();
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
  qDebug() << "Drawing call!";
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