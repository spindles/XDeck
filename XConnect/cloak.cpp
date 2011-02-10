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

#include "cloak.h"
#include "ui_cloak.h"
#include "festival/festival.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

Cloak::Cloak(CharacterSheet& cs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cloak)
{
    ui->setupUi(this);
    csLocal = &cs;
    intelligence = csLocal->intelligence;
    netrunning = csLocal->netrunning;
    timerLength = intelligence + netrunning;
    cloakTime = QTime::QTime(0, timerLength, 0, 0);
    ui->lcdNumber->display(cloakTime.toString("mm:ss"));
    counter = 0;
    timerRunning=false;
}

Cloak::~Cloak()
{
    delete ui;
}

void Cloak::changeEvent(QEvent *e)
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

void Cloak::showTime()
{
    if((counter+(timerLength*60))==-1)
    {
        timer->stop();
        ui->pushButtonActivateCloak->setEnabled(true);
        counter = 0;
        festival_say_text("Cloak timer expired. Cloak is now inactive");
        timerRunning=false;
    }
    else if((counter+(timerLength*60))==29)
    {
        festival_say_text("Caution. Cloak timer will expire in 30 seconds.");
        QTime time = cloakTime.addSecs(counter);
        QString text = time.toString("mm:ss");
        if ((time.second() % 2) == 0)
            text[2] = ' ';
        ui->lcdNumber->display(text);
        counter--;
    }
    else
    {
        QTime time = cloakTime.addSecs(counter);
        QString text = time.toString("mm:ss");
        if ((time.second() % 2) == 0)
            text[2] = ' ';
        ui->lcdNumber->display(text);
        counter--;
    }
}

void Cloak::on_pushButtonActivateCloak_released()
{
    if(timerRunning==false)
    {
        EST_String textToSay = "Cloak will be active for ";
        std::stringstream sstr;
        sstr << timerLength;
        std::string str1 = sstr.str();
        const char* estTimerLength = str1.c_str();
        textToSay += estTimerLength;
        textToSay += " minutes.";
        festival_say_text(textToSay);
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
        timer->start(1000);
        showTime();
        ui->pushButtonActivateCloak->setEnabled(false);
        timerRunning=true;
    }
}
