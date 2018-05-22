#ifndef STYLESHEETPROPERTIES_H
#define STYLESHEETPROPERTIES_H

#include <QColor>
#include <QGradient>
#include <QRect>

namespace StyleSheetProperties {
struct GlowEffect {
  // player glow effect
  QColor glowColor;
  uint glowRadius;
  int glowXoffset;
  int glowYoffset;
};

struct Widget {
  // widget general
  int widgetBorderRadius;
  int widgetBorderThickness;
  QColor widgetBorderColor;
  QColor widgetBackgroundColor;
};

struct HorizontalSliderWidget {
  struct Groove {
    int grooveBorderThickness;
    QColor grooveBorderColor;
    uint grooveHeight;
    QLinearGradient grooveBackgroundGradiant;
    int grooveMargin;
  } groove;
  struct Handle {
    int handleBorderThickness;
    QColor handleBorderColor;
    int handleWidth;
    int handleHeight;
    QLinearGradient handleBackgroundGradiant;
    int handleMarginX;
    int handleMarginY;
  } handle;
  struct Subpage {
    int subpageWidth;
    QLinearGradient subpageBackgroundGradiant;
    int subpageMarginX;
    int subpageMarginY;
  } subpage;
};

struct ListviewWidget {
  int listviewBorderRadius;
  int listviewBorderThickness;
  QColor listviewBorderColor;
  QColor listviewBackgroundColor;
};

struct VerticalScrollbar {
  int verticalScrollbarBorderRadius;
  int verticalScrollbarBorderThickness;
  QColor verticalScrollbarBorderColor;
  QColor verticalScrollbarBackgroundColor;
  int verticalScrollbarWidth;
  QRect verticalScrollbarMargin;
  struct Handle {
    int handleBorderRadius;
    int handleBorderThickness;
    QColor handleBorderColor;
    QLinearGradient handleBackgroundGradiant;
    int handleMinHeight;
  } handle;
  struct Addline {
    int addlineHeight;
  } addLine;
  struct Subline {
    int sublineHeight;
  } subline;
};

struct LabelWidget {
  QColor labelBackgroundColor;
};

}  // namespace StyleSheetProperties

#endif  // STYLESHEETPROPERTIES_H
