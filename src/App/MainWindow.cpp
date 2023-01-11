#include "App/MainWindow.hpp"
#include "App/GraphicsViewWidget.hpp"

#include <QMenuBar>
#include <QAction>

#include <QApplication>
#include <QDebug>


MainWindow::MainWindow()
{
  setWindowTitle("marching squares");

  createMenus();

  GraphicsViewWidget *g = new GraphicsViewWidget{this};
  setCentralWidget(g);
}

void MainWindow::createMenus()
{
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

  QAction *openAct = fileMenu->addAction(tr("&Open Image"), this, &MainWindow::openImageAct_onAction);
  openAct->setToolTip(tr("Choose an image file from disk to be opened"));
  openAct->setShortcut(QKeySequence::Open);
}

void MainWindow::openImageAct_onAction()
{
  qDebug() << "open Image";
}