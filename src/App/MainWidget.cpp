#include "App/MainWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
  :QWidget{parent}
{
  layout_ = new QHBoxLayout;

  propertiesPanel_ = new PropertiesPanel{this};
  layout_->addWidget(propertiesPanel_);

  graphicsView_ = new GraphicsViewWidget{this};
  layout_->addWidget(graphicsView_);

  setLayout(layout_);
} 

void MainWidget::loadImage(const QString &filename)
{
  graphicsView_->loadImage(filename);
}
