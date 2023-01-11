#pragma once 

#include <QMainWindow>

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
  MainWindow();

protected:
  void createMenus();

  void openImageAct_onAction();
};