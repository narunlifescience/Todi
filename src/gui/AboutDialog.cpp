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

   Description : About Dialog
*/

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent), ui_(new Ui_AboutDialog) {
  ui_->setupUi(this);
  ui_->titleLabel->setText("Todi");
  QPixmap logo = QPixmap(":/icons/nocover.svg");
  logo = logo.scaled(48, 48);
  ui_->logoLabel->setPixmap(logo);

}

AboutDialog::~AboutDialog() { delete ui_; }
