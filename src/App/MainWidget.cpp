#include "App/MainWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDoubleSpinBox>
#include <QPushButton>

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
  if (graphicsView_->loadImage(filename)) {
    propertiesPanel_->percentageSpinBox()->setEnabled(true);
    propertiesPanel_->computeBtn()->setEnabled(true);
  }
}
