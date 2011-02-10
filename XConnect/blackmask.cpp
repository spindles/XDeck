//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// (c) 2009, 2010 by Spindles <spindles@wonkyspanner.com>                   //
//                                                                          //
// This file is part of XConnect.                                           //
//                                                                          //
// XConnect is free software: you can redistribute it and/or modify         //
// it under the terms of the GNU General Public License as published by     //
// the Free Software Foundation, either version 3 of the License, or        //
// (at your option) any later version.                                      //
//                                                                          //
// XConnect is distributed in the hope that it will be useful,              //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU General Public License for more details.                             //
//                                                                          //
// You should have received a copy of the GNU General Public License        //
// along with XConnect.  If not, see <http://www.gnu.org/licenses/>.        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include "blackmask.h"
#include "ui_blackmask.h"
#include "time.h"
#include <QGraphicsItem>

BlackMask::BlackMask(CharacterSheet& cs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackMask)
{
    csLocal = &cs;
    ui->setupUi(this);
    chooseInitialAvatar();
}

void BlackMask::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int BlackMask::getRandomNumber()
{
    int number;

    srand(time(NULL));
    number = rand() % csLocal->avatars.size() + 1;

    return number;
}

void BlackMask::chooseInitialAvatar()
{
    ui->avatarLabel->setText("Please wait while a new avatar is being selected...");
    loopCount = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAvatar()));
    timer->start(1000);
    updateAvatar();
}

void BlackMask::updateAvatar()
{
    if (loopCount == 10)
    {
        timer->stop();
        ui->avatarLabel->setText("New avatar chosen. Please use the button below to select a new one.");
    }
    else
    {
        QGraphicsScene* avatarImage = new QGraphicsScene(0, 0, 100, 100);
        QPixmap pm = QPixmap::fromImage(csLocal->avatars[getRandomNumber()].scaled(100,100, Qt::IgnoreAspectRatio, Qt::FastTransformation));
        avatarImage->addPixmap(pm);
        ui->graphicsView->setScene(avatarImage);
        ui->graphicsView->show();
        loopCount++;
    }
}

void BlackMask::chooseNewAvatar()
{
    ui->avatarLabel->setText("Please wait while a new avatar is being selected...");
    loopCount = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAvatar()));
    timer->start(1000);
}

BlackMask::~BlackMask()
{
    timer->stop();
    delete this->ui;
}

void BlackMask::on_pushButtonchooseNewAvatar_released()
{
    chooseNewAvatar();
}
