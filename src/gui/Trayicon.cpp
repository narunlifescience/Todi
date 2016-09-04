/*
 * Copyright (c) 2008 Sander Knopper (sander AT knopper DOT tk) and
 *                    Roeland Douma (roeland AT rullzer DOT com)
 *
 * This file is part of QtMPC.
 *
 * QtMPC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * QtMPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtMPC.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QHeaderView>
#include <QIcon>
#include <QKeyEvent>
#include <QPixmap>
#include <QString>

#include "Player.h"

bool Player::setupTrayIcon() {
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    trayIcon = NULL;
    return false;
  }

  trayIcon = new QSystemTrayIcon(this);
  /*trayIcon->installEventFilter(volumeSliderEventHandler);
  trayIconMenu = new QMenu(this);

  // Setup Actions
  playPauseAction = new QAction(tr("&Play"), trayIconMenu);
  playPauseAction->setIcon(playbackPlay);
  connect(playPauseAction, SIGNAL(triggered()), this, SLOT(playPauseTrack()));

  stopAction = new QAction(tr("&Stop"), trayIconMenu);
  stopAction->setIcon(playbackStop);
  connect(stopAction, SIGNAL(triggered()), this, SLOT(stopTrack()));

  prevAction = new QAction(tr("P&rev"), trayIconMenu);
  prevAction->setIcon(playbackPrev);
  connect(prevAction, SIGNAL(triggered()), this, SLOT(previousTrack()));

  nextAction = new QAction(tr("&Next"), trayIconMenu);
  nextAction->setIcon(playbackNext);
  connect(nextAction, SIGNAL(triggered()), this, SLOT(nextTrack()));


  quitAction = new QAction(tr("&Quit"), trayIconMenu);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

  // Setup Menu
  trayIconMenu->addAction(prevAction);
  trayIconMenu->addAction(nextAction);
  trayIconMenu->addAction(stopAction);
  trayIconMenu->addAction(playPauseAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);*/

  // trayIcon->setContextMenu(trayIconMenu);
  trayIcon->setIcon(QIcon(":icons/todi.svg"));
  trayIcon->setToolTip("Todi");

  return true;
}

void Player::trayIconClicked(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::Unknown:
    case QSystemTrayIcon::Context:
    case QSystemTrayIcon::DoubleClick:
      break;
    case QSystemTrayIcon::Trigger:
      if (isHidden()) {
        showNormal();
      } else {
        hide();
      }
      break;
    case QSystemTrayIcon::MiddleClick:
      break;
  }
}

void Player::setIconProgress(int progress) {
  if (progress < 13) {
    trayIcon->setIcon(QIcon(":icons/tray/onebyeight.svg"));
    return;
  }
  if (progress < 26) {
    trayIcon->setIcon(QIcon(":icons/tray/twobyeight.svg"));
    return;
  }
  if (progress < 39) {
    trayIcon->setIcon(QIcon(":icons/tray/threebyeight.svg"));
    return;
  }
  if (progress < 52) {
    trayIcon->setIcon(QIcon(":icons/tray/fourbyeight.svg"));
    return;
  }
  if (progress < 65) {
    trayIcon->setIcon(QIcon(":icons/tray/fivebyeight.svg"));
    return;
  }
  if (progress < 78) {
    trayIcon->setIcon(QIcon(":icons/tray/sixbyeight.svg"));
    return;
  }
  if (progress < 91) {
    trayIcon->setIcon(QIcon(":icons/tray/sevenbyeight.svg"));
    return;
  }
  trayIcon->setIcon(QIcon(":icons/tray/eightbyeight.svg"));
}
