#pragma once 

#include <QWidget>

class MainWidget;

class QPushButton;
class QDoubleSpinBox;
class QComboBox;

enum InteriorConstraint {
  CentralSkelPoint = 0,
  SampledSkelPoint = 1,
  Normal
};

class PropertiesPanel : public QWidget 
{
Q_OBJECT 

public:
  PropertiesPanel(MainWidget *parent=nullptr);
  
  QDoubleSpinBox *percentageSpinBox() { return percentageSpinBox_; }
  QPushButton *computeBtn() { return computeBtn_; }

  QComboBox *interiorConstraintComboBox() { return interiorConstraintComboBox_; }
  InteriorConstraint currentInteriorConstraintComboBox();

  void percentageSpinBox_onValueChanged(double val);
  void computeBtn_onClick();
  void interiorConstraintComboBox_onCurrentIndexChanged(int index);

private:
  MainWidget *mainWidget_;

  QComboBox *interiorConstraintComboBox_;
  QDoubleSpinBox *percentageSpinBox_;
  QPushButton *computeBtn_;
};