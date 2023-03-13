#pragma once 

#include <QWidget>

class MainWidget;

class QPushButton;
class QDoubleSpinBox;

class PropertiesPanel : public QWidget 
{
Q_OBJECT 

public:
  PropertiesPanel(MainWidget *parent=nullptr);
  
  QDoubleSpinBox *percentageSpinBox() { return percentageSpinBox_; }
  QPushButton *computeBtn() { return computeBtn_; }

  void percentageSpinBox_onValueChanged(double val);
  void computeBtn_onClick();

private:
  MainWidget *mainWidget_;

  QDoubleSpinBox *percentageSpinBox_;
  QPushButton *computeBtn_;
};