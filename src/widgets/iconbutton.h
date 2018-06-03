#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QImage>
#include <QPixmap>
#include <QWidget>

class IconButton : public QWidget {
  Q_OBJECT

 public:
  IconButton(QWidget *parent = nullptr);
  virtual QSize sizeHint() const;
  void setIcon(const QImage &img, int steps = 0);

 signals:
  void clicked();

 protected:
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void paintEvent(QPaintEvent *);
  virtual void resizeEvent(QResizeEvent *);
  virtual void timerEvent(QTimerEvent *);

  /**
   Reload the content for the given size
   The iconbutton preserves a square size, so sz.width() == sz.height()
  */
  virtual void reloadContent(const QSize &sz) = 0;

 private:
  void updateIconBuffer();

  bool m_isClick;
  struct {
    int step;
    int steps;
    int timer;
  } m_anim;

  struct {
    QImage image;
    QPixmap pixmap;
  } m_buffer;

  QImage m_icon, m_oldIcon;
};

#endif  // ICONBUTTON_H
