#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QSystemTrayIcon>

class QAction;

class SystemTrayIcon : public QSystemTrayIcon {
  Q_OBJECT
 public:
  explicit SystemTrayIcon(QObject* parent = nullptr);
  ~SystemTrayIcon();

  void SetupMenu(QAction* previous, QAction* play, QAction* stop,
                 QAction* stop_after, QAction* next, QAction* mute,
                 QAction* love, QAction* quit);

  bool setupTrayIcon();

 protected:
  bool eventFilter(QObject *object, QEvent *event);

 signals:
  void ShowHide();
  void PlayPause();
  void SeekForward();
  void SeekBackward();

 public slots:
  void trayIconUpdateProgress(int value, int track_slider_max);

 private slots:
  void Clicked(QSystemTrayIcon::ActivationReason reason);

 private:
  enum class SystemTrayProgress {
    FirstOctave,
    SecondOctave,
    ThirdOctave,
    FourthOctave,
    FifthOctave,
    SixthOctave,
    SeventhOctave,
    EighthOctave,
  };

  void setTrayIconProgress(SystemTrayProgress trayProgress);
  SystemTrayProgress prevSystemTrayProgress_;
  QMenu* menu_;
  QAction* action_play_pause_;
  QAction* action_stop_;
  QAction* action_stop_after_this_track_;
  QAction* action_mute_;
};

#endif  // SYSTEMTRAYICON_H
