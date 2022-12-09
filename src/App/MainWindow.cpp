#include "App/MainWindow.hpp"

#include "App/GraphicsViewWidget.hpp"

MainWindow::MainWindow()
{
  setWindowTitle("marching squares");

  GraphicsViewWidget *g = new GraphicsViewWidget{this};
  setCentralWidget(g);
}