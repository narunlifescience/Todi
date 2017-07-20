#ifndef APPLICATION_H
#define APPLICATION_H
/* This file is part of Todi.

   Copyright 2017, Arun Narayanankutty <n.arun.lifescience@gmail.com>

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

#include <memory>

#include <QObject>

class ApplicationImpl;
class CurrentArtLoader;
class MPDClient;

class Application : public QObject {
  Q_OBJECT
 public:
  explicit Application(QObject* parent = nullptr);
  ~Application();

  void MoveToNewThread(QObject* object);
  void MoveToThread(QObject* object, QThread* thread);

  CurrentArtLoader* currentArtLoader() const;
  MPDClient* mpdClient() const;

 private:
  std::unique_ptr<ApplicationImpl> appimp_;
  QList<QThread*> threads_;
};

#endif
