#pragma once

#include <QWidget>
#include <QLayout>

#include "App/PropertiesPanel.hpp"
#include "App/GraphicsViewWidget.hpp"


class MainWidget : public QWidget
{
Q_OBJECT

public:
  MainWidget(QWidget *parent = nullptr);

  GraphicsViewWidget *graphicsView() { return graphicsView_; }
  PropertiesPanel *propertiesPanel() { return propertiesPanel_; }

  void loadImage(const QString &filename);

private:
  QLayout *layout_;
  GraphicsViewWidget *graphicsView_;
  PropertiesPanel *propertiesPanel_;
};