#pragma once 

#include <QMainWindow>

class MainWidget;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
  MainWindow();

protected:
  void createMenus();

  void openImageAct_onAction();

protected:
  MainWidget *mainWidget_;
};