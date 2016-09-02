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

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include <QDialog>
#include "ui_preferences_dialog.h"

#include <QSettings>

class PreferencesDialog : public QDialog,
                          private Ui::PreferencesDialog
{
  Q_OBJECT

 public:
  PreferencesDialog(const int tab, QWidget *parent = 0, Qt::WindowFlags f = 0);
  void setCrossfading(int crossfade);

 private:
  int xfade;
  QSettings settings;
  bool lastFmUpdated;

  void writeSettings();
  void emitLastFmReAuth();

 private slots:
  void loadSettings();

 signals:
  void systemTraySet(bool visible);
  void crossfadingChanged(const int seconds);
  void updateIntervalChanged(const int mseconds);
  void lastFmReAuth(bool enabled);
};

#endif
