#include "App/PropertiesPanel.hpp"
#include "App/GraphicsViewWidget.hpp"
#include "App/MainWidget.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>

#include <QDoubleSpinBox>

PropertiesPanel::PropertiesPanel(MainWidget *parent)
  : QWidget{parent},
    mainWidget_{parent}
{
  QLabel *label = new QLabel{"Properties", this};
  QVBoxLayout *layout = new QVBoxLayout;

  layout->setAlignment(Qt::AlignTop);

  label->setStyleSheet("font-weight: bold; text-decoration: underline;");
  layout->addWidget(label);

  percentageSpinBox_ = new QDoubleSpinBox{this};
  percentageSpinBox_->setRange(0.0f, 1.0f);
  percentageSpinBox_->setSingleStep(0.05f);
  percentageSpinBox_->setValue(0.1f);
  percentageSpinBox_->setEnabled(false);
  connect(percentageSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
    &PropertiesPanel::percentageSpinBox_onValueChanged);
  
  QHBoxLayout *percentageLayout = new QHBoxLayout;
  percentageLayout->addWidget(new QLabel{"Portion of contour: ", this});
  percentageLayout->addWidget(percentageSpinBox_);
  layout->addItem(percentageLayout);

  computeBtn_ = new QPushButton{"compute", this};
  computeBtn_->setEnabled(false);
  connect(computeBtn_, &QPushButton::clicked, this, 
    &PropertiesPanel::computeBtn_onClick);
  layout->addWidget(computeBtn_);

  setLayout(layout);
}

void PropertiesPanel::percentageSpinBox_onValueChanged(double val)
{ }

void PropertiesPanel::computeBtn_onClick()
{ 
  mainWidget_->graphicsView()->updateImplicitSurface();
}