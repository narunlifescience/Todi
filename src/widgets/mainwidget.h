#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class MainWidget : public QWidget
{
  Q_OBJECT
public:
  explicit MainWidget(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event);

public slots:
};

#endif // MAINWIDGET_H
