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

#include <QThread>
#include <QDebug>

#include "../lib/mpdclient.h"
#include "../tagger/currentartloader.h"
#include "application.h"
#include "lazy.h"

class ApplicationImpl {
 public:
  ApplicationImpl(Application* app)
      : tagreader_([=]() { return new CurrentArtLoader(app); }),
        mpdclient_([=]() { return new MPDClient(app); }) {}
  Lazy<CurrentArtLoader> tagreader_;
  Lazy<MPDClient> mpdclient_;
};

Application::Application(QObject* parent)
    : QObject(parent), appimp_(new ApplicationImpl(this)) {}

Application::~Application() {
  for (QThread* thread : threads_) {
    thread->quit();
  }

  for (QThread* thread : threads_) {
    thread->wait();
  }
}

void Application::MoveToNewThread(QObject* object) {
  QThread* thread = new QThread(this);

  MoveToThread(object, thread);

  thread->start();
  threads_ << thread;
}

void Application::MoveToThread(QObject* object, QThread* thread) {
  object->setParent(nullptr);
  object->moveToThread(thread);
}

CurrentArtLoader* Application::currentArtLoader() const {
  return appimp_->tagreader_.get();
}

MPDClient* Application::mpdClient() const { return appimp_->mpdclient_.get(); }
