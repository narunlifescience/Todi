#include "theme.h"
#include <QDebug>

using namespace StyleSheetProperties;

Theme::Theme(QObject *parent)
    : QObject(parent),
      playerGlowEffect_(new GlowEffect()),
      volumepopupGlowEffect_(new GlowEffect),
      playerWidget_(new Widget),
      volumepopupWidget_(new VolumePopupWidget),
      fancytabWidget_(new Widget),
      trackSliderWidget_(new HorizontalSliderWidget),
      volumeSliderWidget_(new HorizontalSliderWidget),
      currentSongMetadataWidget_(new LabelWidget),
      timerLabelWidget_(new LabelWidget),
      playlistviewWidget_(new ListviewWidget),
      libraryviewWidget_(new TreeviewWidget),
      folderviewWidget_(new TreeviewWidget),
      consoleWidget_(new ConsoleWidget),
      vScrollbar_(new VerticalScrollbar) {}

void Theme::initializeTodidark() {
  // Player Glow effect
  playerGlowEffect_->color = QColor(181, 185, 190);
  playerGlowEffect_->radius = 5;
  playerGlowEffect_->xOffset = 0;
  playerGlowEffect_->yOffset = 0;
  changePlayerGlowTheme();

  // Volume popup Glow effect
  volumepopupGlowEffect_->color = QColor(181, 185, 190);
  volumepopupGlowEffect_->radius = 5;
  volumepopupGlowEffect_->xOffset = 0;
  volumepopupGlowEffect_->yOffset = 0;
  changeVolumepopupGlowTheme();

  // Player widget
  playerWidget_->borderRadius = 3;
  playerWidget_->backgroundColor = QColor(20, 20, 20, 225);
  playerWidget_->borderThickness = 0;
  playerWidget_->borderColor = QColor(92, 92, 92);
  changePlayerWidgetTheme();

  // Volume Popup widget
  volumepopupWidget_->blurPadding = 4;
  volumepopupWidget_->edgeCurve = 6;
  volumepopupWidget_->layoutPadding = 6;
  volumepopupWidget_->backgroundColor = QColor(20, 20, 20, 200);
  changeVolumepopupWidgetTheme();

  // Track Slider Widget
  trackSliderWidget_->groove.borderThickness = 0;
  trackSliderWidget_->groove.borderColor = QColor(153, 153, 153);
  trackSliderWidget_->groove.height = 3;
  trackSliderWidget_->groove.backgroundGradiant.x1 = 0;
  trackSliderWidget_->groove.backgroundGradiant.y1 = 0;
  trackSliderWidget_->groove.backgroundGradiant.x2 = 0;
  trackSliderWidget_->groove.backgroundGradiant.y2 = 1;
  trackSliderWidget_->groove.backgroundGradiant.startpos = 0;
  trackSliderWidget_->groove.backgroundGradiant.startColor =
      QColor(177, 177, 177);
  trackSliderWidget_->groove.backgroundGradiant.stoppos = 1;
  trackSliderWidget_->groove.backgroundGradiant.stopColor =
      QColor(196, 196, 196);
  trackSliderWidget_->groove.margin.top = 0;
  trackSliderWidget_->groove.margin.right = 0;
  trackSliderWidget_->groove.margin.bottom = 0;
  trackSliderWidget_->groove.margin.left = 0;
  trackSliderWidget_->handle.backgroundGradiant.x1 = 0;
  trackSliderWidget_->handle.backgroundGradiant.y1 = 0;
  trackSliderWidget_->handle.backgroundGradiant.x2 = 1;
  trackSliderWidget_->handle.backgroundGradiant.y2 = 1;
  trackSliderWidget_->handle.backgroundGradiant.startpos = 0;
  trackSliderWidget_->handle.backgroundGradiant.startColor =
      QColor(239, 240, 241);
  trackSliderWidget_->handle.backgroundGradiant.stoppos = 1;
  trackSliderWidget_->handle.backgroundGradiant.stopColor =
      QColor(239, 240, 241);
  trackSliderWidget_->handle.borderThickness = 0;
  trackSliderWidget_->handle.borderColor = QColor(92, 92, 92);
  trackSliderWidget_->handle.width = 2;
  trackSliderWidget_->handle.margin.top = -2;
  trackSliderWidget_->handle.margin.right = 0;
  trackSliderWidget_->handle.margin.bottom = -2;
  trackSliderWidget_->handle.margin.left = 0;
  trackSliderWidget_->subpage.backgroundGradiant.x1 = 0;
  trackSliderWidget_->subpage.backgroundGradiant.y1 = 0;
  trackSliderWidget_->subpage.backgroundGradiant.x2 = 0;
  trackSliderWidget_->subpage.backgroundGradiant.y2 = 1;
  trackSliderWidget_->subpage.backgroundGradiant.startpos = 0;
  trackSliderWidget_->subpage.backgroundGradiant.startColor =
      QColor(61, 174, 233);
  trackSliderWidget_->subpage.backgroundGradiant.stoppos = 1;
  trackSliderWidget_->subpage.backgroundGradiant.stopColor =
      QColor(61, 174, 233);
  trackSliderWidget_->subpage.width = 3;
  trackSliderWidget_->subpage.margin.top = 0;
  trackSliderWidget_->subpage.margin.right = 0;
  trackSliderWidget_->subpage.margin.bottom = 0;
  trackSliderWidget_->subpage.margin.left = 0;
  changeTrackSliderWidgetTheme();

  // Volume Slider Widget
  volumeSliderWidget_->groove.borderThickness = 0;
  volumeSliderWidget_->groove.borderColor = QColor(153, 153, 153);
  volumeSliderWidget_->groove.height = 3;
  volumeSliderWidget_->groove.backgroundGradiant.x1 = 0;
  volumeSliderWidget_->groove.backgroundGradiant.y1 = 0;
  volumeSliderWidget_->groove.backgroundGradiant.x2 = 0;
  volumeSliderWidget_->groove.backgroundGradiant.y2 = 1;
  volumeSliderWidget_->groove.backgroundGradiant.startpos = 0;
  volumeSliderWidget_->groove.backgroundGradiant.startColor =
      QColor(177, 177, 177);
  volumeSliderWidget_->groove.backgroundGradiant.stoppos = 1;
  volumeSliderWidget_->groove.backgroundGradiant.stopColor =
      QColor(196, 196, 196);
  volumeSliderWidget_->groove.margin.top = 0;
  volumeSliderWidget_->groove.margin.right = 0;
  volumeSliderWidget_->groove.margin.bottom = 0;
  volumeSliderWidget_->groove.margin.left = 0;
  volumeSliderWidget_->handle.backgroundGradiant.x1 = 0;
  volumeSliderWidget_->handle.backgroundGradiant.y1 = 0;
  volumeSliderWidget_->handle.backgroundGradiant.x2 = 1;
  volumeSliderWidget_->handle.backgroundGradiant.y2 = 1;
  volumeSliderWidget_->handle.backgroundGradiant.startpos = 0;
  volumeSliderWidget_->handle.backgroundGradiant.startColor =
      QColor(239, 240, 241);
  volumeSliderWidget_->handle.backgroundGradiant.stoppos = 1;
  volumeSliderWidget_->handle.backgroundGradiant.stopColor =
      QColor(239, 240, 241);
  volumeSliderWidget_->handle.borderThickness = 0;
  volumeSliderWidget_->handle.borderColor = QColor(92, 92, 92);
  volumeSliderWidget_->handle.width = 2;
  volumeSliderWidget_->handle.margin.top = -4;
  volumeSliderWidget_->handle.margin.right = 0;
  volumeSliderWidget_->handle.margin.bottom = -4;
  volumeSliderWidget_->handle.margin.left = 0;
  volumeSliderWidget_->subpage.backgroundGradiant.x1 = 0;
  volumeSliderWidget_->subpage.backgroundGradiant.y1 = 0;
  volumeSliderWidget_->subpage.backgroundGradiant.x2 = 0;
  volumeSliderWidget_->subpage.backgroundGradiant.y2 = 1;
  volumeSliderWidget_->subpage.backgroundGradiant.startpos = 0;
  volumeSliderWidget_->subpage.backgroundGradiant.startColor =
      QColor(61, 174, 233);
  volumeSliderWidget_->subpage.backgroundGradiant.stoppos = 1;
  volumeSliderWidget_->subpage.backgroundGradiant.stopColor =
      QColor(61, 174, 233);
  volumeSliderWidget_->subpage.width = 3;
  volumeSliderWidget_->subpage.margin.top = 0;
  volumeSliderWidget_->subpage.margin.right = 0;
  volumeSliderWidget_->subpage.margin.bottom = 0;
  volumeSliderWidget_->subpage.margin.left = 0;
  changeVolumeSliderWidgetTheme();

  // Vertical Scrollbar
  vScrollbar_->borderThickness = 0;
  vScrollbar_->borderRadius = 3;
  vScrollbar_->backgroundColor = QColor(200, 200, 200, 100);
  vScrollbar_->width = 6;
  vScrollbar_->margin.top = 0;
  vScrollbar_->margin.right = 0;
  vScrollbar_->margin.bottom = 0;
  vScrollbar_->margin.left = 0;
  vScrollbar_->handle.borderRadius = 3;
  vScrollbar_->handle.backgroundColor = QColor(245, 245, 245, 200);
  vScrollbar_->handle.minHeight = 20;
  vScrollbar_->addLine.backgroundGradiant.x1 = 0;
  vScrollbar_->addLine.backgroundGradiant.y1 = 0;
  vScrollbar_->addLine.backgroundGradiant.x2 = 1;
  vScrollbar_->addLine.backgroundGradiant.y2 = 0;
  vScrollbar_->addLine.backgroundGradiant.startpos = 0;
  vScrollbar_->addLine.backgroundGradiant.startColor = QColor(32, 47, 130);
  vScrollbar_->addLine.backgroundGradiant.midpos = 0.5;
  vScrollbar_->addLine.backgroundGradiant.midColor = QColor(245, 245, 245);
  vScrollbar_->addLine.backgroundGradiant.stoppos = 1;
  vScrollbar_->addLine.backgroundGradiant.stopColor = QColor(32, 47, 130);
  vScrollbar_->addLine.height = 0;
  vScrollbar_->subline.height = 0;
  changeVscrollbarTheme();

  currentSongMetadataWidget_->backgroundColor = QColor(80, 80, 80, 100);
  currentSongMetadataWidget_->foregroundColor = QColor();
  currentSongMetadataWidget_->borderRadius = 3;
  currentSongMetadataWidget_->font = QFont();
  changeCurrentSongMetadataLabelWidgetTheme();

  timerLabelWidget_->backgroundColor = QColor(20, 20, 20, 0);
  timerLabelWidget_->foregroundColor = QColor();
  timerLabelWidget_->borderRadius = 0;
  timerLabelWidget_->font = QFont();
  changeTimeLabelWidgetTheme();

  playlistviewWidget_->backgroundColor = QColor(80, 80, 80, 100);
  playlistviewWidget_->borderRadius = 3;
  changePlaylistviewWidgetTheme();

  libraryviewWidget_->backgroundColor = QColor(80, 80, 80, 100);
  libraryviewWidget_->borderRadius = 3;
  changeLibraryviewWidgetTheme();

  folderviewWidget_->backgroundColor = QColor(80, 80, 80, 100);
  folderviewWidget_->borderRadius = 3;
  changeFolderviewWidgetTheme();

  consoleWidget_->backgroundColor = QColor(80, 80, 80, 100);
  consoleWidget_->borderRadius = 3;
  changeConsoleWidgetTheme();
}

void Theme::setTheme(Theme::Themes theme) {
  switch (theme) {
    case Theme::Themes::TodiDark:
      initializeTodidark();
      break;
    case Theme::Themes::TodiLight:
      break;
  }
}

void Theme::changePlayerGlowTheme() {
  emit themePlayerGlowChanged(playerGlowEffect_);
}

void Theme::changeVolumepopupGlowTheme() {
  emit themeVolumepopupGlowChanged(volumepopupGlowEffect_);
}

void Theme::changePlayerWidgetTheme() {
  QString stylesheet = QString(
                           ".QWidget{border-radius: %1px; background-color: "
                           "rgba(%2, %3, %4, %5); border: %6px solid %7;}")
                           .arg(playerWidget_->borderRadius)
                           .arg(playerWidget_->backgroundColor.red())
                           .arg(playerWidget_->backgroundColor.green())
                           .arg(playerWidget_->backgroundColor.blue())
                           .arg(playerWidget_->backgroundColor.alpha())
                           .arg(playerWidget_->borderThickness)
                           .arg(playerWidget_->borderColor.name());
  emit themePlayerWidgetChanged(stylesheet);
}

void Theme::changeVolumepopupWidgetTheme() {
  emit themeVolumepopupWidgetChanged(volumepopupWidget_);
}

void Theme::changeTrackSliderWidgetTheme() {
  QString stylesheet =
      QString(
          ".TrackSlider::groove:horizontal { border: %1px solid %2; height: "
          "%3px; background: qlineargradient(x1:%4, y1:%5, x2:%6, y2:%7, "
          "stop:%8 %9, stop:%10 %11); margin: %12px %13px %14px %15px; } "
          ".TrackSlider::handle:horizontal {background: qlineargradient("
          "x1:%16, y1:%17, x2:%18, y2:%19, stop:%20 %21, stop:%22 %23); "
          "border: %24px solid %25; width: %26px; margin: %27px %28px %29px "
          "%30px; }"
          ".TrackSlider::sub-page:horizontal {background: qlineargradient("
          "x1: %31, y1: %32, x2: %33, y2: %34, stop: %35 %36, stop: %37 %38); "
          "width: %39px; margin: %40px %41px %42px %43px;}")
          .arg(trackSliderWidget_->groove.borderThickness)
          .arg(trackSliderWidget_->groove.borderColor.name())
          .arg(trackSliderWidget_->groove.height)
          .arg(trackSliderWidget_->groove.backgroundGradiant.x1)
          .arg(trackSliderWidget_->groove.backgroundGradiant.y1)
          .arg(trackSliderWidget_->groove.backgroundGradiant.x2)
          .arg(trackSliderWidget_->groove.backgroundGradiant.y2)
          .arg(trackSliderWidget_->groove.backgroundGradiant.startpos)
          .arg(trackSliderWidget_->groove.backgroundGradiant.startColor.name())
          .arg(trackSliderWidget_->groove.backgroundGradiant.stoppos)
          .arg(trackSliderWidget_->groove.backgroundGradiant.stopColor.name())
          .arg(trackSliderWidget_->groove.margin.top)
          .arg(trackSliderWidget_->groove.margin.right)
          .arg(trackSliderWidget_->groove.margin.bottom)
          .arg(trackSliderWidget_->groove.margin.left)
          .arg(trackSliderWidget_->handle.backgroundGradiant.x1)
          .arg(trackSliderWidget_->handle.backgroundGradiant.y1)
          .arg(trackSliderWidget_->handle.backgroundGradiant.x2)
          .arg(trackSliderWidget_->handle.backgroundGradiant.y2)
          .arg(trackSliderWidget_->handle.backgroundGradiant.startpos)
          .arg(trackSliderWidget_->handle.backgroundGradiant.startColor.name())
          .arg(trackSliderWidget_->handle.backgroundGradiant.stoppos)
          .arg(trackSliderWidget_->handle.backgroundGradiant.stopColor.name())
          .arg(trackSliderWidget_->handle.borderThickness)
          .arg(trackSliderWidget_->handle.borderColor.name())
          .arg(trackSliderWidget_->handle.width)
          .arg(trackSliderWidget_->handle.margin.top)
          .arg(trackSliderWidget_->handle.margin.right)
          .arg(trackSliderWidget_->handle.margin.bottom)
          .arg(trackSliderWidget_->handle.margin.left)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.x1)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.y1)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.x2)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.y2)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.startpos)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.startColor.name())
          .arg(trackSliderWidget_->subpage.backgroundGradiant.stoppos)
          .arg(trackSliderWidget_->subpage.backgroundGradiant.stopColor.name())
          .arg(trackSliderWidget_->subpage.width)
          .arg(trackSliderWidget_->subpage.margin.top)
          .arg(trackSliderWidget_->subpage.margin.right)
          .arg(trackSliderWidget_->subpage.margin.bottom)
          .arg(trackSliderWidget_->subpage.margin.left);
  emit themeTrackSliderWidgetChanged(stylesheet);
}

void Theme::changeVolumeSliderWidgetTheme() {
  QString stylesheet =
      QString(
          ".VolumeSlider::groove:horizontal { border: %1px solid %2; height: "
          "%3px; background: qlineargradient(x1:%4, y1:%5, x2:%6, y2:%7, "
          "stop:%8 %9, stop:%10 %11); margin: %12px %13px %14px %15px; } "
          ".VolumeSlider::handle:horizontal {background: qlineargradient("
          "x1:%16, y1:%17, x2:%18, y2:%19, stop:%20 %21, stop:%22 %23); "
          "border: %24px solid %25; width: %26px; margin: %27px %28px %29px "
          "%30px; }"
          ".VolumeSlider::sub-page:horizontal {background: qlineargradient("
          "x1: %31, y1: %32, x2: %33, y2: %34, stop: %35 %36, stop: %37 %38); "
          "width: %39px; margin: %40px %41px %42px %43px;}")
          .arg(volumeSliderWidget_->groove.borderThickness)
          .arg(volumeSliderWidget_->groove.borderColor.name())
          .arg(volumeSliderWidget_->groove.height)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.x1)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.y1)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.x2)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.y2)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.startpos)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.startColor.name())
          .arg(volumeSliderWidget_->groove.backgroundGradiant.stoppos)
          .arg(volumeSliderWidget_->groove.backgroundGradiant.stopColor.name())
          .arg(volumeSliderWidget_->groove.margin.top)
          .arg(volumeSliderWidget_->groove.margin.right)
          .arg(volumeSliderWidget_->groove.margin.bottom)
          .arg(volumeSliderWidget_->groove.margin.left)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.x1)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.y1)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.x2)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.y2)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.startpos)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.startColor.name())
          .arg(volumeSliderWidget_->handle.backgroundGradiant.stoppos)
          .arg(volumeSliderWidget_->handle.backgroundGradiant.stopColor.name())
          .arg(volumeSliderWidget_->handle.borderThickness)
          .arg(volumeSliderWidget_->handle.borderColor.name())
          .arg(volumeSliderWidget_->handle.width)
          .arg(volumeSliderWidget_->handle.margin.top)
          .arg(volumeSliderWidget_->handle.margin.right)
          .arg(volumeSliderWidget_->handle.margin.bottom)
          .arg(volumeSliderWidget_->handle.margin.left)
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.x1)
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.y1)
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.x2)
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.y2)
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.startpos)
          .arg(
              volumeSliderWidget_->subpage.backgroundGradiant.startColor.name())
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.stoppos)
          .arg(volumeSliderWidget_->subpage.backgroundGradiant.stopColor.name())
          .arg(volumeSliderWidget_->subpage.width)
          .arg(volumeSliderWidget_->subpage.margin.top)
          .arg(volumeSliderWidget_->subpage.margin.right)
          .arg(volumeSliderWidget_->subpage.margin.bottom)
          .arg(volumeSliderWidget_->subpage.margin.left);
  emit themeVolumeSliderWidgetChanged(stylesheet);
}

void Theme::changeVscrollbarTheme() {
  QString stylesheet =
      QString(
          "QScrollBar:vertical {border: %1px; border-radius: %2px; background:"
          "rgba(%3, %4, %5, %6); width:%7px; margin: %8px %9px %10px %11px;}"
          "QScrollBar::handle:vertical {border-radius: %12px; background: rgb("
          "%13, %14, %15, %16); min-height: %17px;}"
          "QScrollBar::add-line:vertical {background: qlineargradient(x1:%18, "
          "y1:%19, x2:%20, y2:%21, stop: %22 rgb(%23, %24, %25), stop: %26 "
          "rgb(%27, %28, %29),  stop:%30 rgb(%31, %32, %33)); height: %34px; "
          "subcontrol-position: bottom; subcontrol-origin: margin;}"
          "QScrollBar::sub-line:vertical {height: %35 px;}")
          .arg(vScrollbar_->borderThickness)
          .arg(vScrollbar_->borderRadius)
          .arg(vScrollbar_->backgroundColor.red())
          .arg(vScrollbar_->backgroundColor.green())
          .arg(vScrollbar_->backgroundColor.blue())
          .arg(vScrollbar_->backgroundColor.alpha())
          .arg(vScrollbar_->width)
          .arg(vScrollbar_->margin.top)
          .arg(vScrollbar_->margin.right)
          .arg(vScrollbar_->margin.bottom)
          .arg(vScrollbar_->margin.left)
          .arg(vScrollbar_->handle.borderRadius)
          .arg(vScrollbar_->handle.backgroundColor.red())
          .arg(vScrollbar_->handle.backgroundColor.green())
          .arg(vScrollbar_->handle.backgroundColor.blue())
          .arg(vScrollbar_->handle.backgroundColor.alpha())
          .arg(vScrollbar_->handle.minHeight)
          .arg(vScrollbar_->addLine.backgroundGradiant.x1)
          .arg(vScrollbar_->addLine.backgroundGradiant.y1)
          .arg(vScrollbar_->addLine.backgroundGradiant.x2)
          .arg(vScrollbar_->addLine.backgroundGradiant.y2)
          .arg(vScrollbar_->addLine.backgroundGradiant.startpos)
          .arg(vScrollbar_->addLine.backgroundGradiant.startColor.red())
          .arg(vScrollbar_->addLine.backgroundGradiant.startColor.green())
          .arg(vScrollbar_->addLine.backgroundGradiant.startColor.blue())
          .arg(vScrollbar_->addLine.backgroundGradiant.midpos)
          .arg(vScrollbar_->addLine.backgroundGradiant.midColor.red())
          .arg(vScrollbar_->addLine.backgroundGradiant.midColor.green())
          .arg(vScrollbar_->addLine.backgroundGradiant.midColor.blue())
          .arg(vScrollbar_->addLine.backgroundGradiant.stoppos)
          .arg(vScrollbar_->addLine.backgroundGradiant.stopColor.red())
          .arg(vScrollbar_->addLine.backgroundGradiant.stopColor.green())
          .arg(vScrollbar_->addLine.backgroundGradiant.stopColor.blue())
          .arg(vScrollbar_->addLine.height)
          .arg(vScrollbar_->subline.height);
  emit themeVscrollbarChanged(stylesheet);
}

void Theme::changeCurrentSongMetadataLabelWidgetTheme() {
  QString stylesheet =
      QString(
          ".MetadataWidget {background:rgba(%1, %2, %3, %4); "
          "border-radius: %5px;}")
          .arg(currentSongMetadataWidget_->backgroundColor.red())
          .arg(currentSongMetadataWidget_->backgroundColor.green())
          .arg(currentSongMetadataWidget_->backgroundColor.blue())
          .arg(currentSongMetadataWidget_->backgroundColor.alpha())
          .arg(currentSongMetadataWidget_->borderRadius);
  emit themeCurrentSongMetadataLabelWidgetChanged(stylesheet);
}

void Theme::changeTimeLabelWidgetTheme() {
  QString stylesheet = QString(
                           ".QLabel {background:rgba(%1, %2, %3, %4); "
                           "border-radius: %5px;}")
                           .arg(timerLabelWidget_->backgroundColor.red())
                           .arg(timerLabelWidget_->backgroundColor.green())
                           .arg(timerLabelWidget_->backgroundColor.blue())
                           .arg(timerLabelWidget_->backgroundColor.alpha())
                           .arg(timerLabelWidget_->borderRadius);
  emit themeTimeLabelWidgetChanged(stylesheet);
}

void Theme::changePlaylistviewWidgetTheme() {
  QString stylesheet =
      QString(
          "QListView {background:rgba(%1, %2, %3, %4); border-radius: %5px;}")
          .arg(playlistviewWidget_->backgroundColor.red())
          .arg(playlistviewWidget_->backgroundColor.green())
          .arg(playlistviewWidget_->backgroundColor.blue())
          .arg(playlistviewWidget_->backgroundColor.alpha())
          .arg(playlistviewWidget_->borderRadius);
  emit themePlaylistviewWidgetChanged(stylesheet);
}

void Theme::changeLibraryviewWidgetTheme() {
  QString stylesheet =
      QString(
          "QTreeView {background:rgba(%1, %2, %3, %4); border-radius: %5px;}")
          .arg(libraryviewWidget_->backgroundColor.red())
          .arg(libraryviewWidget_->backgroundColor.green())
          .arg(libraryviewWidget_->backgroundColor.blue())
          .arg(libraryviewWidget_->backgroundColor.alpha())
          .arg(libraryviewWidget_->borderRadius);
  emit themeLibraryviewWidgetChanged(stylesheet);
}

void Theme::changeFolderviewWidgetTheme() {
  QString stylesheet =
      QString(
          "QTreeView {background:rgba(%1, %2, %3, %4); border-radius: %5px;}")
          .arg(folderviewWidget_->backgroundColor.red())
          .arg(folderviewWidget_->backgroundColor.green())
          .arg(folderviewWidget_->backgroundColor.blue())
          .arg(folderviewWidget_->backgroundColor.alpha())
          .arg(folderviewWidget_->borderRadius);
  emit themeFolderviewWidgetChanged(stylesheet);
}

void Theme::changeConsoleWidgetTheme() {
  QString stylesheet =
      QString("* {background:rgba(%1, %2, %3, %4); border-radius: %5px;}")
          .arg(consoleWidget_->backgroundColor.red())
          .arg(consoleWidget_->backgroundColor.green())
          .arg(consoleWidget_->backgroundColor.blue())
          .arg(consoleWidget_->backgroundColor.alpha())
          .arg(consoleWidget_->borderRadius);
  emit themeConsoleWidgetChanged(stylesheet);
}
