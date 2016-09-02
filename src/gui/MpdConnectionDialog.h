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

   Description : MPD connection dialog
*/

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include <QDialog>

class Ui_MpdConnectionDialog;

class MpdConnectionDialog : public QDialog {
  Q_OBJECT

 public:
  MpdConnectionDialog(QWidget *parent = nullptr);
  ~MpdConnectionDialog();

 private:
  Ui_MpdConnectionDialog *ui_;

 private slots:
  void connecttompd();
  void rejectConnection();
};

#endif
