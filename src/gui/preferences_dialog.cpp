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

#include "preferences_dialog.h"
//#include "main_window.h"

PreferencesDialog::PreferencesDialog(const int tab, QWidget *parent,
                                     Qt::WindowFlags f)
    : QDialog(parent, f),
      //xfade(MPDStatus::getInstance()->xfade()),
      lastFmUpdated(false) {
  setupUi(this);
  loadSettings();

  /*crossfading->setValue(MPDStatus::getInstance()->xfade());*/
  tabs->setCurrentIndex(tab);
}

void PreferencesDialog::setCrossfading(int crossfade) {
  xfade = crossfade;
  crossfading->setValue(xfade);
}

void PreferencesDialog::loadSettings() {
  hostLineEdit->setText(
      settings.value("connection/host", "localhost").toString());
  portSpinBox->setValue(settings.value("connection/port", 6600).toInt());
  passwordLineEdit->setText(
      settings.value("connection/password", "").toString());
  systemTrayCheckBox->setChecked(settings.value("systemtray").toBool());
  if (settings.value("systemtray").toBool()) {
    systemTrayPopup->setChecked(settings.value("systemtrayPopup").toBool());
  } else {
    systemTrayPopup->setEnabled(false);
  }
  crossfading->setValue(xfade);
  updateInterval->setValue(settings.value("connection/interval", 1000).toInt());

  bool lastfm = settings.value("lastfm/enabled", "false").toBool();
  lastFmCheckBox->setChecked(lastfm);
  lastFmUserName->setText(settings.value("lastfm/username", "").toString());
  lastFmPassword->setText(settings.value("lastfm/password", "").toString());
  lastFmUserName->setEnabled(lastfm);
  lastFmPassword->setEnabled(lastfm);
}

void PreferencesDialog::writeSettings() {
  settings.setValue("connection/host", hostLineEdit->text());
  settings.setValue("connection/port", portSpinBox->value());
  settings.setValue("connection/password", passwordLineEdit->text());
  settings.setValue("connection/interval", updateInterval->value());
  settings.setValue("systemtray", systemTrayCheckBox->isChecked());

  // Lastfm stuff is updated
  if (settings.value("lastfm/enabled", "false").toBool() !=
          lastFmCheckBox->isChecked() ||
      settings.value("lastfm/username", "") != lastFmUserName->text() ||
      settings.value("lastfm/password", "") != lastFmPassword->text()) {
    lastFmUpdated = true;
  }

  settings.setValue("lastfm/enabled", lastFmCheckBox->isChecked());
  settings.setValue("lastfm/username", lastFmUserName->text());
  settings.setValue("lastfm/password", lastFmPassword->text());

  if (systemTrayCheckBox->isChecked()) {
    settings.setValue("systemtrayPopup", systemTrayPopup->isChecked());
  } else {
    settings.setValue("systemtrayPopup", false);
  }
  if (crossfading->value() != xfade)
    emit crossfadingChanged(crossfading->value());
  emit systemTraySet(systemTrayCheckBox->isChecked());
  emit updateIntervalChanged(updateInterval->value());
}

void PreferencesDialog::emitLastFmReAuth() {
  if (lastFmUpdated) {
    emit lastFmReAuth(settings.value("lastfm/enabled", "false").toBool());
  }
}
