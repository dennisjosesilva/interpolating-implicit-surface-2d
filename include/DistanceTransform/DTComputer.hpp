#pragma once 

#include <QOpenGLFunctions>

#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QMatrix4x4>

class DTComputer
{
public:
  DTComputer(QOpenGLFunctions *gl);
  QVector<float> compute(int width, int height, 
    const QVector<bool> &bimg);

  void init();

  ~DTComputer();

private:  
  void destroy();

  int pixelIndex(int width, int height, const QVector2D &p) const;
  int pixelIndex(int width, int height, int x, int y) const;

  void setUpShaders();
  QVector<QVector2D> traceBoundaries(int width, int height,
    const QVector<bool> &bimg) const;
  bool isBoundary(int width, int height, const QVector<bool> &bimg,
    const QVector2D &p) const;
  bool isBoundary(int width, int height, const QVector<bool> &bimg,
    int x, int y) const;


  void genSplatTexture(int width, int height);
  void setupFramebuffer(int width, int height);


private:
  QOpenGLFunctions *gl_;

  // QOpenGLVertexArrayObject vao_;
  // QOpenGLBuffer vbo_;

  unsigned int splatTexture_;
  QOpenGLShaderProgram shader_;
  int splatDiameter_;
  int splatRadius_;

  unsigned int framebuffer_;
  unsigned int texFramebuffer_;
  unsigned int rbo_;
};


