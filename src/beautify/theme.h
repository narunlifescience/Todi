#ifndef THEME_H
#define THEME_H

#include <QObject>
#include "stylesheetproperties.h"

class Theme : public QObject {
  Q_OBJECT
 public:
  enum class Themes { TodiDark, TodiLight };
  explicit Theme(QObject *parent = nullptr);

 signals:
  void themePlayerGlowChanged(
      StyleSheetProperties::GlowEffect *playerGlowEffect);
  void themeVolumepopupGlowChanged(
      StyleSheetProperties::GlowEffect *volumepopupGlowEffect);
  void themePlayerWidgetChanged(QString stylesheet);
  void themeVolumepopupWidgetChanged(
      StyleSheetProperties::VolumePopupWidget *volumepopupWidget);
  void themeTrackSliderWidgetChanged(QString Stylesheet);
  void themeVolumeSliderWidgetChanged(QString Stylesheet);
  void themeVscrollbarChanged(QString stylesheet);
  void themeCurrentSongMetadataLabelWidgetChanged(QString stylesheet);
  void themeTimeLabelWidgetChanged(QString stylesheet);
  void themePlaylistviewWidgetChanged(QString stylesheet);
  void themeLibraryviewWidgetChanged(QString stylesheet);
  void themeFolderviewWidgetChanged(QString stylesheet);
  void themeConsoleWidgetChanged(QString stylesheet);

 public slots:
  void setTheme(Themes theme);

 private slots:
  void changePlayerGlowTheme();
  void changeVolumepopupGlowTheme();
  void changePlayerWidgetTheme();
  void changeVolumepopupWidgetTheme();
  void changeTrackSliderWidgetTheme();
  void changeVolumeSliderWidgetTheme();
  void changeVscrollbarTheme();
  void changeCurrentSongMetadataLabelWidgetTheme();
  void changeTimeLabelWidgetTheme();
  void changePlaylistviewWidgetTheme();
  void changeLibraryviewWidgetTheme();
  void changeFolderviewWidgetTheme();
  void changeConsoleWidgetTheme();

 private:
  StyleSheetProperties::GlowEffect *playerGlowEffect_;
  StyleSheetProperties::GlowEffect *volumepopupGlowEffect_;
  StyleSheetProperties::Widget *playerWidget_;
  StyleSheetProperties::VolumePopupWidget *volumepopupWidget_;
  StyleSheetProperties::Widget *fancytabWidget_;
  StyleSheetProperties::HorizontalSliderWidget *trackSliderWidget_;
  StyleSheetProperties::HorizontalSliderWidget *volumeSliderWidget_;
  StyleSheetProperties::LabelWidget *currentSongMetadataWidget_;
  StyleSheetProperties::LabelWidget *timerLabelWidget_;
  StyleSheetProperties::ListviewWidget *playlistviewWidget_;
  StyleSheetProperties::TreeviewWidget *libraryviewWidget_;
  StyleSheetProperties::TreeviewWidget *folderviewWidget_;
  StyleSheetProperties::ConsoleWidget *consoleWidget_;
  StyleSheetProperties::VerticalScrollbar *vScrollbar_;

  void initializeTodidark();
};

#endif  // THEME_H
