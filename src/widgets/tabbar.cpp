/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#include "tabbar.h"
#include "stylehelper.h"

#include <QDebug>

#include <QAnimationGroup>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QSignalMapper>
#include <QSplitter>
#include <QStackedLayout>
#include <QStyleOptionTabV3>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

const int FancyTabBar::m_rounding = 12;

namespace {
static void selectTab(QPainter* p, const QRect& rect) {
  // background
  p->save();
  p->setRenderHints(QPainter::Antialiasing);
  QLinearGradient grad(rect.topLeft(), rect.topRight());
  grad.setColorAt(0, QColor(45, 45, 45, 240));
  grad.setColorAt(1, QColor(45, 45, 45, 170));
  QPainterPath path;
  path.addRoundRect(rect.adjusted(1, 1, -1, -2), 12, 12);
  p->fillPath(path, grad);
  p->restore();
}
}  // namespace

FancyTab::FancyTab(QWidget* tabbar)
    : QWidget(tabbar), tabbar(tabbar), m_fader(0) {
  animator.setPropertyName("fader");
  animator.setTargetObject(this);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void FancyTab::fadeIn() {
  animator.stop();
  animator.setDuration(80);
  animator.setEndValue(40);
  animator.start();
}

void FancyTab::fadeOut() {
  animator.stop();
  animator.setDuration(160);
  animator.setEndValue(0);
  animator.start();
}

void FancyTab::setFader(float value) {
  m_fader = value;
  tabbar->update();
}

FancyTabBar::FancyTabBar(QWidget* parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  setAttribute(Qt::WA_Hover, true);
  setFocusPolicy(Qt::NoFocus);
  setMouseTracking(true);  // Needed for hover events
  m_triggerTimer.setSingleShot(true);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  // We use a zerotimer to keep the sidebar responsive
  connect(&m_triggerTimer, SIGNAL(timeout()), this, SLOT(emitCurrentIndex()));
}

FancyTabBar::~FancyTabBar() { delete style(); }

QSize FancyTab::sizeHint() const {
  int spacing = 10;
  int width = 22 + spacing;
  int iconHeight = 22;
  QSize ret(width, iconHeight + spacing);
  return ret;
}

QSize FancyTabBar::tabSizeHint(bool minimum) const {
  int spacing = 10;
  int width = 22 + spacing;
  int iconHeight = minimum ? 0 : 22;
  return QSize(width, iconHeight + spacing);
}

void FancyTabBar::paintEvent(QPaintEvent*) {
  QPainter p(this);

  for (int i = 0; i < count(); ++i)
    if (i != currentIndex()) paintTab(&p, i);

  // paint active tab last, since it overlaps the neighbors
  if (currentIndex() != -1) paintTab(&p, currentIndex());
}

bool FancyTab::event(QEvent* event) {
  if (event->type() != QEvent::ToolTip) return QWidget::event(event);

  QHelpEvent* helpevent = static_cast<QHelpEvent*>(event);
  QToolTip::showText(helpevent->globalPos(), text);
  return true;
}

void FancyTab::enterEvent(QEvent*) { fadeIn(); }

void FancyTab::leaveEvent(QEvent*) { fadeOut(); }

QSize FancyTabBar::sizeHint() const {
  QSize sizehint = tabSizeHint();
  return QSize(sizehint.width(), sizehint.height() * m_tabs.count());
}

QSize FancyTabBar::minimumSizeHint() const {
  QSize sizehint = tabSizeHint(true);
  return QSize(sizehint.width(), sizehint.height() * m_tabs.count());
}

QRect FancyTabBar::tabRect(int index) const {
  return m_tabs[index]->geometry();
}

QString FancyTabBar::tabToolTip(int index) const {
  return m_tabs[index]->toolTip();
}

void FancyTabBar::setTabToolTip(int index, const QString& toolTip) {
  m_tabs[index]->setToolTip(toolTip);
}

// This keeps the sidebar responsive since we get a repaint before loading the
// mode itself
void FancyTabBar::emitCurrentIndex() { emit currentChanged(m_currentIndex); }

void FancyTabBar::mousePressEvent(QMouseEvent* e) {
  e->accept();
  for (int index = 0; index < m_tabs.count(); ++index) {
    if (tabRect(index).contains(e->pos())) {
      m_currentIndex = index;
      update();
      m_triggerTimer.start(0);
      break;
    }
  }
}

void FancyTabBar::addTab(const QIcon& icon, const QString& label) {
  FancyTab* tab = new FancyTab(this);
  tab->icon = icon;
  tab->text = label;
  tab->setToolTip(label);
  m_tabs.append(tab);
  qobject_cast<QVBoxLayout*>(layout())->insertWidget(layout()->count() - 1,
                                                     tab);
}

void FancyTabBar::addSpacer(int size) {
  qobject_cast<QVBoxLayout*>(layout())->insertSpacerItem(
      layout()->count() - 1,
      new QSpacerItem(0, size, QSizePolicy::Fixed, QSizePolicy::Maximum));
}

void FancyTabBar::paintTab(QPainter* painter, int tabIndex) const {
  if (!validIndex(tabIndex)) {
    qWarning("invalid index");
    return;
  }
  painter->save();

  QRect rect = tabRect(tabIndex);
  bool selected = (tabIndex == m_currentIndex);

  if (selected) {
    selectTab(painter, rect);
  }

  QRect tabIconRect(rect);
  tabIconRect.adjust(+4, +4, -4, -4);

  if (!selected) {
    painter->save();
    int fader = int(m_tabs[tabIndex]->fader());
    QLinearGradient grad(rect.topLeft(), rect.topRight());
    grad.setColorAt(0, QColor(45, 45, 45, fader));
    grad.setColorAt(0.5, QColor(45, 45, 45, fader));
    grad.setColorAt(1, QColor(45, 45, 45, fader));
    painter->fillRect(rect, grad);
    painter->setPen(QPen(grad, 1.0));
    painter->drawLine(rect.topLeft(), rect.topRight());
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    painter->restore();
  }

  Utils::StyleHelper::drawIconWithShadow(tabIcon(tabIndex), tabIconRect,
                                         painter, QIcon::Normal);

  painter->translate(0, -1);
  painter->restore();
}

void FancyTabBar::setCurrentIndex(int index) {
  m_currentIndex = index;
  update();
  emit currentChanged(m_currentIndex);
}

//////
// FancyColorButton
//////

class FancyColorButton : public QWidget {
 public:
  FancyColorButton(QWidget* parent) : m_parent(parent) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  }

  void mousePressEvent(QMouseEvent* ev) {
    if (ev->modifiers() & Qt::ShiftModifier)
      Utils::StyleHelper::setBaseColor(QColorDialog::getColor(
          Utils::StyleHelper::requestedBaseColor(), m_parent));
  }

 private:
  QWidget* m_parent;
};

//////
// FancyTabWidget
//////

FancyTabWidget::FancyTabWidget(QWidget* parent)
    : QWidget(parent),
      mode_(Mode_None),
      tab_bar_(nullptr),
      stack_(new QStackedLayout),
      side_widget_(new QWidget),
      side_layout_(new QVBoxLayout),
      top_layout_(new QVBoxLayout),
      use_background_(false),
      menu_(nullptr) {
  side_layout_->setSpacing(0);
  side_layout_->setMargin(0);
  side_layout_->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));

  side_widget_->setLayout(side_layout_);
  side_widget_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  top_layout_->setMargin(0);
  top_layout_->setSpacing(0);
  top_layout_->addLayout(stack_);

  QHBoxLayout* main_layout = new QHBoxLayout;
  main_layout->setMargin(0);
  main_layout->setSpacing(1);
  main_layout->addWidget(side_widget_);
  main_layout->addLayout(top_layout_);
  setLayout(main_layout);
}

void FancyTabWidget::AddTab(QWidget* tab, const QIcon& icon,
                            const QString& label) {
  stack_->addWidget(tab);
  items_ << Item(icon, label);
}

void FancyTabWidget::AddSpacer(int size) { items_ << Item(size); }

void FancyTabWidget::SetBackgroundPixmap(const QPixmap& pixmap) {
  background_pixmap_ = pixmap;
  update();
}

void FancyTabWidget::paintEvent(QPaintEvent*) {
  if (!use_background_) return;

  QPainter painter(this);

  QRect rect = side_widget_->rect().adjusted(0, 0, 1, 0);
  rect = style()->visualRect(layoutDirection(), geometry(), rect);
  Utils::StyleHelper::verticalGradient(&painter, rect, rect);

  if (!background_pixmap_.isNull()) {
    QRect pixmap_rect(background_pixmap_.rect());
    pixmap_rect.moveTo(rect.topLeft());

    while (pixmap_rect.top() < rect.bottom()) {
      QRect source_rect(pixmap_rect.intersected(rect));
      source_rect.moveTo(0, 0);
      painter.drawPixmap(pixmap_rect.topLeft(), background_pixmap_,
                         source_rect);
      pixmap_rect.moveTop(pixmap_rect.bottom() - 10);
    }
  }

  painter.setPen(Utils::StyleHelper::borderColor());
  painter.drawLine(rect.topRight(), rect.bottomRight());

  QColor light = Utils::StyleHelper::sidebarHighlight();
  painter.setPen(light);
  painter.drawLine(rect.bottomLeft(), rect.bottomRight());
}

int FancyTabWidget::current_index() const { return stack_->currentIndex(); }

void FancyTabWidget::SetCurrentIndex(int index) {
  if (FancyTabBar* bar = qobject_cast<FancyTabBar*>(tab_bar_)) {
    bar->setCurrentIndex(index);
  } else {
    stack_->setCurrentIndex(index);
  }
}

void FancyTabWidget::SetCurrentWidget(QWidget* widget) {
  SetCurrentIndex(stack_->indexOf(widget));
}

void FancyTabWidget::ShowWidget(int index) {
  stack_->setCurrentIndex(index);
  emit CurrentChanged(index);
}

void FancyTabWidget::AddBottomWidget(QWidget* widget) {
  top_layout_->addWidget(widget);
}

void FancyTabWidget::SetMode(Mode mode) {
  // Remove previous tab bar
  delete tab_bar_;
  tab_bar_ = nullptr;

  use_background_ = false;

  // Create new tab bar
  switch (mode) {
    case Mode_LargeSidebar: {
      FancyTabBar* bar = new FancyTabBar(this);
      setMinimumWidth(32);

      side_layout_->insertWidget(0, bar);
      tab_bar_ = bar;

      for (const Item& item : items_) {
        if (item.type_ == Item::Type_Spacer)
          bar->addSpacer(item.spacer_size_);
        else
          bar->addTab(item.tab_icon_, item.tab_label_);
      }

      bar->setCurrentIndex(stack_->currentIndex());
      connect(bar, SIGNAL(currentChanged(int)), SLOT(ShowWidget(int)));

      use_background_ = false;
    } break;

    case Mode_Tabs:
      break;

    case Mode_IconOnlyTabs:
      break;

    case Mode_SmallSidebar:
      use_background_ = true;
      break;

    case Mode_PlainSidebar:
      break;

    case Mode_None:
    default:
      qWarning() << "Unknown fancy tab mode" << mode;
      // fallthrough
  }

  tab_bar_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  mode_ = mode;
  emit ModeChanged(mode);
  update();
}

void FancyTabWidget::contextMenuEvent(QContextMenuEvent* event) {
  if (!menu_) {
    menu_ = new QMenu(this);

    QSignalMapper* mapper = new QSignalMapper(this);
    QActionGroup* group = new QActionGroup(this);
    AddMenuItem(mapper, group, tr("Large sidebar"), Mode_LargeSidebar);
    AddMenuItem(mapper, group, tr("Small sidebar"), Mode_SmallSidebar);
    AddMenuItem(mapper, group, tr("Plain sidebar"), Mode_PlainSidebar);
    AddMenuItem(mapper, group, tr("Tabs on top"), Mode_Tabs);
    AddMenuItem(mapper, group, tr("Icons on top"), Mode_IconOnlyTabs);
    menu_->addActions(group->actions());

    connect(mapper, SIGNAL(mapped(int)), SLOT(SetMode(int)));
  }

  menu_->popup(event->globalPos());
}

void FancyTabWidget::AddMenuItem(QSignalMapper* mapper, QActionGroup* group,
                                 const QString& text, Mode mode) {
  QAction* action = group->addAction(text);
  action->setCheckable(true);
  mapper->setMapping(action, mode);
  connect(action, SIGNAL(triggered()), mapper, SLOT(map()));

  if (mode == mode_) action->setChecked(true);
}
