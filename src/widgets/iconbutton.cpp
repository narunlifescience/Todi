#include "iconbutton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QSizePolicy>
#include <QTimerEvent>
#include <QToolBar>

IconButton::IconButton(QWidget *parent) : QWidget(parent), m_isClick(false) {
  m_anim.step = 0;
  m_anim.timer = 0;
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void IconButton::setIcon(const QImage &img, int steps) {
  m_anim.step = 0;
  m_anim.steps = steps;

  m_icon = img;
  m_oldIcon = steps ? m_buffer.image : QImage();

  if (m_anim.timer) killTimer(m_anim.timer);
  if (steps)
    m_anim.timer = startTimer(40);
  else
    updateIconBuffer();
  repaint();
}

void IconButton::mousePressEvent(QMouseEvent *me) {
  me->accept();
  m_isClick = true;
}

void IconButton::mouseReleaseEvent(QMouseEvent *me) {
  me->accept();
  if (m_isClick && rect().contains(me->pos())) {
    m_isClick = false;
    emit clicked();
  }
}

void IconButton::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(0, 0, m_buffer.pixmap);
  p.end();
}

void IconButton::resizeEvent(QResizeEvent *re) {
  if (width() != height())
    resize(height(), height());
  else {
    reloadContent(re->size());
    QWidget::resizeEvent(re);
  }
}

QSize IconButton::sizeHint() const {
  if (QToolBar *toolBar = qobject_cast<QToolBar *>(parentWidget()))
    return toolBar->iconSize();

  return QSize(32, 32);
}

void IconButton::timerEvent(QTimerEvent *te) {
  if (te->timerId() != m_anim.timer) return;

  ++m_anim.step;
  updateIconBuffer();
  if (m_anim.step >= m_anim.steps) {
    killTimer(m_anim.timer);
    m_anim.timer = 0;
    m_oldIcon = QImage();
  }
  repaint();
}

static QImage adjusted(QImage img, const QSize &sz) {
  if (img.size() == sz) return img;
  QImage ret(sz, QImage::Format_ARGB32_Premultiplied);
  ret.fill(Qt::transparent);
  QPainter p(&ret);
  p.drawImage((ret.width() - img.width()) / 2,
              (ret.height() - img.height()) / 2, img);
  p.end();
  return ret;
}

static inline QImage interpolated(const QImage &img1, const QImage &img2,
                                  int a1, int a2) {
  const int a = a1 + a2;
  if (!a) return img1.copy();

  QImage img(img1.size(), img1.format());

  const uchar *src[2] = {img1.bits(), img2.bits()};
  uchar *dst = img.bits();
  const int n = img.width() * img.height() * 4;
  for (int i = 0; i < n; ++i) {
    *dst = ((*src[0] * a1 + *src[1] * a2) / a) & 0xff;
    ++dst;
    ++src[0];
    ++src[1];
  }
  return img;
}

void IconButton::updateIconBuffer() {
  if (m_anim.step >= m_anim.steps)
    m_buffer.image = adjusted(m_icon, size());
  else
    m_buffer.image =
        interpolated(adjusted(m_oldIcon, size()), adjusted(m_icon, size()),
                     m_anim.steps - m_anim.step, m_anim.step);

  m_buffer.pixmap = QPixmap::fromImage(m_buffer.image);
}
