#include "App/MainWindow.hpp"
#include "App/GraphicsViewWidget.hpp"

#include <QMenuBar>
#include <QAction>

#include <QFileDialog>
#include <QStandardPaths>
#include <QImage>
#include <QImageWriter>
#include <QImageReader>
#include <QByteArrayList>
#include <QMessageBox>

#include <QApplication>
#include <QDebug>


MainWindow::MainWindow()
{
  setWindowTitle("marching squares");

  createMenus();

  GraphicsViewWidget *g = new GraphicsViewWidget{this};
  setCentralWidget(g);
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
  static bool firstDialog = true;

  if (firstDialog) {
    firstDialog = false;
    const QStringList pictureLocations = 
      QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    dialog.setDirectory(pictureLocations.isEmpty() ? QDir::currentPath() : pictureLocations.last());
  }

  QStringList mimeTypeFilters;
  const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen ? 
    QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
  
  for (const QByteArray &mimeTypeName : supportedMimeTypes)
    mimeTypeFilters.append(mimeTypeName);

  dialog.setMimeTypeFilters(mimeTypeFilters);
  dialog.selectMimeTypeFilter("image/x-portable-greymap"); // PGM
  dialog.setAcceptMode(acceptMode);
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
  QFileDialog dialog(this, tr("Open File"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

  bool accepted = dialog.exec() == QDialog::Accepted;

  if (accepted == dialog.selectedFiles().count() > 0) {
    const QString filename = dialog.selectedFiles().constFirst();    
    qDebug() << filename <<  "chosen to be loaded";
  }
}