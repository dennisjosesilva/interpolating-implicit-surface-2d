#include <iostream>

#include "implicit/InterpolatingImplicitFunction2d.hpp"

#include <QVector2D>

#include <QApplication>

#include "App/MainWindow.hpp"

int main(int argc, char *argv[])
{
  // using ImplFunctionType = InterpolatingImplicitFunction2D::ImplFunctionType;

  // InterpolatingImplicitFunction2D interp;

  // interp.pushBoundaryConstraint(QVector2D(-0.5f, 0.0f));
  // interp.pushBoundaryConstraint(QVector2D( 0.0f, 0.5f));
  // interp.pushBoundaryConstraint(QVector2D( 0.5f, 0.0f));
  // interp.pushBoundaryConstraint(QVector2D( 0.0f,-0.5f));

  // interp.pushInteriorConstraint(QVector2D{ 0.0f, 0.0f});

  // ImplFunctionType f = interp.optimize();

  // std::cout << "\n";
  // std::cout << "f(0.5,  0) = " << f(QVector2D{0.5f, 0.0f}) << "\n";
  // std::cout << "f(0.25, 0) = " <<  f(QVector2D{0.45f, 0.0f}) << "\n";
  // std::cout << "f(0.55, 0) = " <<  f(QVector2D{0.55f, 0.0f}) << "\n";

  QApplication app{argc, argv};

  MainWindow mainWindow;
  mainWindow.resize(720, 680);

  mainWindow.show();

  return app.exec();
}