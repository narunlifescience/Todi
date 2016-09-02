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

#include "MpdConnectionDialog.h"
#include "globals.h"
#include "ui_MpdConnectionDialog.h"

#include <QSettings>

MpdConnectionDialog::MpdConnectionDialog(QWidget *parent)
    : QDialog(parent), ui_(new Ui_MpdConnectionDialog) {
  ui_->setupUi(this);
  ui_->hostnameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  ui_->portLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  ui_->passwdLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  QSettings settings;
  settings.beginGroup("mpd-server-connection");
  ui_->hostnameLineEdit->setText(
      settings.value("host", "localhost").toString());
  ui_->portSpinBox->setValue(settings.value("port", 6600).toInt());
  ui_->passwdLineEdit->setText(settings.value("passwd", "").toString());
  settings.endGroup();

  connect(ui_->buttonBox, SIGNAL(accepted()), SLOT(connecttompd()));
  connect(ui_->buttonBox, SIGNAL(rejected()), SLOT(rejectConnection()));
}

MpdConnectionDialog::~MpdConnectionDialog() { delete ui_; }

void MpdConnectionDialog::connecttompd() {
  Todi::hostname = ui_->hostnameLineEdit->text().trimmed();
  Todi::port = static_cast<quint16>(ui_->portSpinBox->value());
  Todi::passwd = ui_->passwdLineEdit->text();
  QSettings settings;
  settings.beginGroup("mpd-server-connection");
  settings.setValue("host", Todi::hostname);
  settings.setValue("port", Todi::port);
  settings.setValue("passwd", Todi::passwd);
  settings.endGroup();
  QDialog::accept();
}

void MpdConnectionDialog::rejectConnection() { QDialog::reject(); }
