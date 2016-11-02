/* This file is part of Todi.

   Copyright 2016, Arun Narayanankutty <n.arun.lifescience@gmail.com>

   Todi is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   Todi is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with Todi.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLAYBACKOPTIONSCONTROLLER_H
#define PLAYBACKOPTIONSCONTROLLER_H

#include <QObject>
#include <memory>

class MPDSocket;

class PlaybackOptionsController : public QObject {
  Q_OBJECT
 public:
  explicit PlaybackOptionsController(
      QObject *parent = nullptr,
      std::shared_ptr<MPDSocket> mpdSocket = nullptr);
  ~PlaybackOptionsController();

 signals:

 public slots:
  void consume(quint8 toggle);
  void crossfade(int seconds);
  //void mixrampdb();
  //void mixrampdelay();
  void random(quint8 toggle);
  void repeat(quint8 toggle);
  void setvol(quint8 vol);
  void single(quint8 toggle);
  //void replay_gain_mode();
  //void replay_gain_status();
  void volume(int volChange);

 private:
  std::shared_ptr<MPDSocket> mpdSocket_;
  const static QByteArray consumeCmd;
  const static QByteArray crossfadeCmd;
  const static QByteArray mixrampdbCmd;
  const static QByteArray mixrampdelayCmd;
  const static QByteArray randomCmd;
  const static QByteArray repeatCmd;
  const static QByteArray setvolCmd;
  const static QByteArray singleCmd;
  const static QByteArray replay_gain_modeCmd;
  const static QByteArray replay_gain_statusCmd;
  const static QByteArray volumeCmd;
};

#endif  // PLAYBACKOPTIONSCONTROLLER_H
