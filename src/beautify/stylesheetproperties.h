#ifndef STYLESHEETPROPERTIES_H
#define STYLESHEETPROPERTIES_H

#include <QColor>
#include <QFont>
#include <QGradient>

namespace StyleSheetProperties {
struct GlowEffect {
  // player glow effect
  QColor color;
  int radius;
  int xOffset;
  int yOffset;
};

struct Widget {
  // widget general
  int borderRadius;
  int borderThickness;
  QColor borderColor;
  QColor backgroundColor;
};

struct Dialog {
  // widget general
  int borderRadius;
  int borderThickness;
  QColor borderColor;
  QColor backgroundColor;
};

struct VolumePopupWidget {
  int blurPadding;
  int edgeCurve;
  int layoutPadding;
  QColor backgroundColor;
};

struct Margin {
  int top;
  int right;
  int bottom;
  int left;
};

struct LinearGradient2 {
  int x1;
  int y1;
  int x2;
  int y2;
  int startpos;
  int stoppos;
  QColor startColor;
  QColor stopColor;
};

struct LinearGradient3 {
  int x1;
  int y1;
  int x2;
  int y2;
  int startpos;
  double midpos;
  int stoppos;
  QColor startColor;
  QColor midColor;
  QColor stopColor;
};

struct HorizontalSliderWidget {
  struct Groove {
    int borderThickness;
    QColor borderColor;
    int height;
    LinearGradient2 backgroundGradiant;
    Margin margin;
  } groove;
  struct Handle {
    int borderThickness;
    QColor borderColor;
    int width;
    int height;
    LinearGradient2 backgroundGradiant;
    Margin margin;
  } handle;
  struct Subpage {
    int width;
    LinearGradient2 backgroundGradiant;
    Margin margin;
  } subpage;
};

struct ListviewWidget {
  int borderRadius;
  int borderThickness;
  QColor borderColor;
  QColor backgroundColor;
};

struct TreeviewWidget {
  int borderRadius;
  QColor backgroundColor;
};

struct ConsoleWidget {
  int borderRadius;
  QColor backgroundColor;
};

struct VerticalScrollbar {
  int borderRadius;
  int borderThickness;
  QColor borderColor;
  QColor backgroundColor;
  int width;
  Margin margin;
  struct Handle {
    int borderRadius;
    int borderThickness;
    QColor borderColor;
    QColor backgroundColor;
    int minHeight;
  } handle;
  struct Addline {
    LinearGradient3 backgroundGradiant;
    int height;
  } addLine;
  struct Subline {
    int height;
  } subline;
};

struct LabelWidget {
  QColor backgroundColor;
  QColor foregroundColor;
  QFont font;
  int borderRadius;
};

}  // namespace StyleSheetProperties

#endif  // STYLESHEETPROPERTIES_H
