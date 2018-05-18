/* This file is part of AlphaPlot.
   Copyright 2016, Arun Narayanankutty <n.arun.lifescience@gmail.com>

   AlphaPlot is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   AlphaPlot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with AlphaPlot.  If not, see <http://www.gnu.org/licenses/>.

   Description : AlphaPlot Console dock widget
*/
#include "consolewidget.h"
#include "console.h"
#include "ui_consolewidget.h"

#include <QDebug>
#include <QPainter>

ConsoleWidget::ConsoleWidget(QWidget *parent)
    : QWidget(parent), ui_(new Ui_ConsoleWidget), sentcommand(QString("")) {
  ui_->setupUi(this);
  setWindowTitle(tr("Scripting Console"));
  setWindowIcon(QIcon());
  ui_->gridLayout->setSpacing(0);
  ui_->gridLayout->setContentsMargins(0, 0, 0, 0);
  ui_->console->setFrameShape(QFrame::NoFrame);

  connect(ui_->console, SIGNAL(command(QString)), this,
          SLOT(evaluate(QString)));
}

ConsoleWidget::~ConsoleWidget() { delete ui_; }

void ConsoleWidget::printError(QString err) {
  ui_->console->result("muParser error: " + err, Console::Error);
}

void ConsoleWidget::evaluate(const QString line) {
  sentcommand = line;
  emit sendCommand(sentcommand.toLatin1());
}

void ConsoleWidget::commandwithresults(QString command, QByteArray result) {
  // dont print periodic status command
  if (command != QString("status")) {
    if (command != sentcommand) {
      ui_->console->append(command);
      sentcommand = "";
    }
    QList<QByteArray> lines = result.split('\n');
    foreach (const QByteArray &line, lines) {
      ui_->console->partialResult();
      ui_->console->append(line);
    }

    ui_->console->insertPlainText("\n");
    ui_->console->promptWithoutResult();
  }
}
