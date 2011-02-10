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

#include "flatline.h"
#include "ui_flatline.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

Flatline::Flatline(CharacterSheet& cs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Flatline)
{
    ui->setupUi(this);
    running = false;
    csLocal = &cs;
    intelligence = csLocal->intelligence;
    netrunning = csLocal->netrunning;
    timerLength = 60/netrunning;
}

Flatline::~Flatline()
{
    delete ui;
}

void Flatline::changeEvent(QEvent *e)
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

void Flatline::on_pushButtonActivate_released()
{
    if (running==false)
    {
        running = true;
        ui->plainTextEditOutput->clear();
        ui->pushButtonActivate->setEnabled(false);
        setupProgramOutput();
        running = false;
    }
}

void Flatline::setupProgramOutput()
{
    int ip1, ip2, ip3, ip4;

    /* initialize random seed: */
    srand ( time(NULL) );

    /* generate secret number: */
    ip1 = rand() % 255 + 1;
    ip2 = rand() % 255 + 1;
    ip3 = rand() % 255 + 1;
    ip4 = rand() % 255 + 1;

    std::stringstream sstr;
    sstr << ip1;
    std::string str1 = sstr.str();
    const char* ip1chr = str1.c_str();

    std::stringstream sstr2;
    sstr2 << ip2;
    std::string str2 = sstr2.str();
    const char* ip2chr = str2.c_str();

    std::stringstream sstr3;
    sstr3 << ip3;
    std::string str3 = sstr3.str();
    const char* ip3chr = str3.c_str();

    std::stringstream sstr4;
    sstr4 << ip4;
    std::string str4 = sstr4.str();
    const char* ip4chr = str4.c_str();

    QString targetIP = "";
    targetIP.append(ip1chr);
    targetIP.append(".");
    targetIP.append(ip2chr);
    targetIP.append(".");
    targetIP.append(ip3chr);
    targetIP.append(".");
    targetIP.append(ip4chr);

    textToPrint = "Creating logic bomb... \n\n";

    for (int i=0; i < 990; i++)
    {
        int bit = rand() % 100 + 1;
        if(bit>50)
        {
            textToPrint.append("1");
        }
        else
        {
            textToPrint.append("0");
        }
    }

    textToPrint.append("\n \nLaunching logic bomb at ");
    textToPrint.append(targetIP);
    textToPrint.append("\n\n0%||||||||||25%||||||||||50%||||||||||75%||||||||||100%");

    counter = 0;
    programOutputTimer = new QTimer(this);
    connect(programOutputTimer, SIGNAL(timeout()), this, SLOT(updateText()));
    programOutputTimer->start(100);
}

void Flatline::updateText()
{
    if(counter==textToPrint.length())
    {
        ui->plainTextEditOutput->textCursor().insertText("\n \nDone!");
        programOutputTimer->stop();
        ui->pushButtonActivate->setEnabled(true);
    }
    else
    {
        /* initialize random seed: */
        srand ( time(NULL) );

        /* generate secret number: */
        int chanceToPause = rand() % 100 + 1;

        if(chanceToPause > 20)
        {
            ui->plainTextEditOutput->textCursor().insertText(textToPrint.at(counter));
            counter++;
        }
    }
}
