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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtOpenGL>
#include <QSvgWidget>
#include <QGraphicsScene>
#include "festival/festival.h"
#include <EST_String.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cs = new CharacterSheet();
    ui->setupUi(this);
    ui->frameSoftware->hide();
    ui->graphicsView->hide();
    loadDeckSoftware();
    isConnected = false;
    bmOpen = false;
    ckOpen = false;
    dkOpen = false;
    flOpen = false;
    gnOpen = false;
    gdOpen = false;
    hmOpen = false;
    hhOpen = false;
    hyOpen = false;
    lbOpen = false;
    rpOpen = false;
    shOpen = false;
    wbOpen = false;

    EST_String textToSay = "Good ";
    QTime theTime = QTime::currentTime();
    int theHour = theTime.hour();
    if(theHour >= 5 && theHour < 12)
    {
        textToSay += "Morning ";
    }
    else if(theHour >= 12 && theHour < 17)
    {
        textToSay += "Afternoon ";
    }
    else
    {
        textToSay += "Evening ";
    }
    const char* estUserName = cs->userName.toLatin1();
    textToSay += estUserName;
    festival_say_text(textToSay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_toolButtonConnect_clicked()
{
    if(isConnected == false)
    {
        festival_say_text("Creating routed connection to the internet.");
        introScene = new QGraphicsScene(0, 0, 600, 400);
        setupIntroScene();
        ui->graphicsView->setScene(introScene);
        ui->graphicsView->show();
        isConnected = true;
        ui->toolButtonDisconnect->setEnabled(true);
        ui->toolButtonConnect->setEnabled(false);
    }
    else
    {
        qWarning("introScene already played");
    }
}

void MainWindow::setupIntroScene()
{
    int xPos,yPos;
    srand ( time(NULL) );

    connectionNodes[0][0] = 285.0;
    connectionNodes[0][1] = 150.0;
    for(int i=1; i<10; i++)
    {
        xPos = rand() % 600;
        yPos = rand() % 400;
        //qWarning() << "xPos = " << (qreal)xPos;
        //qWarning() << "yPos = " << (qreal)yPos;
        connectionNodes[i][0] = xPos;
        connectionNodes[i][1] = yPos;
        //qWarning() << i << "0 =" << connectionNodes[i][0];
        //qWarning() << i << "1 =" << connectionNodes[i][1];
    }

    introSceneNumber = 1;
    introSceneTimer = new QTimer(this);
    connect(introSceneTimer, SIGNAL(timeout()), this, SLOT(updateIntroMap()));
    introSceneTimer->start(1000);
}

void MainWindow::updateIntroMap()
{
    if(introSceneNumber==10)
    {
        //introScene->addText("CONNECTED");
        introSceneTimer->stop();
        festival_say_text("You are now connected to the internet.");
    }
    else
    {
        introScene->addEllipse(connectionNodes[introSceneNumber-1][0]-1, connectionNodes[introSceneNumber-1][1]-1, 2, 2, QPen(QColor(255,0,0)));
        introScene->addLine(connectionNodes[introSceneNumber-1][0],connectionNodes[introSceneNumber-1][1],connectionNodes[introSceneNumber][0], connectionNodes[introSceneNumber][1], QPen(QColor(255,0,0)));
        introScene->addEllipse(connectionNodes[introSceneNumber][0]-1, connectionNodes[introSceneNumber][1]-1, 2, 2, QPen(QColor(255,0,0)));
        //qWarning() << introSceneNumber;
    }
    ui->graphicsView->show();
    introSceneNumber++;
}

void MainWindow::on_toolButtonDisconnect_clicked()
{
    if(isConnected == true)
    {
        QGraphicsScene *outroScene = new QGraphicsScene(0, 0, 600, 400);
        ui->graphicsView->setScene(outroScene);
        //outroScene->addText("DISCONNECTED");
        festival_say_text("You are now disconnected from the internet.");
        ui->graphicsView->show();
        isConnected = false;
        if(ui->toolButtonDisconnect->isEnabled()==true)
        {
            ui->toolButtonDisconnect->setEnabled(false);
        }
        if(ui->toolButtonConnect->isEnabled()==false)
        {
            ui->toolButtonConnect->setEnabled(true);
        }
    }
    else
    {
        qWarning("outroScene already played");
    }
}

void MainWindow::on_toolButtonNetwork_toggled(bool checked)
{
    if(ui->toolButtonNetwork->isChecked() == true)
    {
        ui->graphicsView->show();
        if(isConnected == false)
        {
           ui->toolButtonConnect->setEnabled(true);
        }
    }
    else
    {
        ui->graphicsView->hide();
        ui->toolButtonConnect->setEnabled(false);
    }
}

void MainWindow::on_toolButtonSoftware_toggled(bool checked)
{
    if(ui->toolButtonSoftware->isChecked() == true)
    {
        ui->frameSoftware->show();
    }
    else
    {
        ui->frameSoftware->hide();
    }
}

void MainWindow::on_toolButtonShutdown_released()
{
    festival_say_text("Closing... Goodbye!");
    QApplication::quit();
}

void MainWindow::loadDeckSoftware()
{
    if(cs->BlackMask == true)
        ui->toolButtonBlackMask->setEnabled(true);
    else
        ui->toolButtonBlackMask->setEnabled(false);
    if(cs->Cloak ==true)
        ui->toolButtonCloak->setEnabled(true);
    else
        ui->toolButtonCloak->setEnabled(false);
    if (cs->DArcKnight ==true)
        ui->toolButtonDarcKnight->setEnabled(true);
    else
        ui->toolButtonDarcKnight->setEnabled(false);
    if (cs->Flatline == true)
        ui->toolButtonFlatline->setEnabled(true);
    else
        ui->toolButtonFlatline->setEnabled(false);
    if (cs->Genie == true)
        ui->toolButtonGenie->setEnabled(true);
    else
        ui->toolButtonGenie->setEnabled(false);
    if (cs->GuardDog == true)
        ui->toolButtonGuardDog->setEnabled(true);
    else
        ui->toolButtonGuardDog->setEnabled(false);
    if (cs->Hammer == true)
        ui->toolButtonHammer->setEnabled(true);
    else
        ui->toolButtonHammer->setEnabled(false);
    if (cs->HellHound == true)
        ui->toolButtonHellhound->setEnabled(true);
    else
        ui->toolButtonHellhound->setEnabled(false);
    if (cs->Hydra == true)
        ui->toolButtonHydra->setEnabled(true);
    else
        ui->toolButtonHydra->setEnabled(false);
    if (cs->LightningBug == true)
        ui->toolButtonLightningBug->setEnabled(true);
    else
        ui->toolButtonLightningBug->setEnabled(false);
    if (cs->Reaper == true)
        ui->toolButtonReaper->setEnabled(true);
    else
        ui->toolButtonReaper->setEnabled(false);
    if (cs->Shield == true)
        ui->toolButtonShield->setEnabled(true);
    else
        ui->toolButtonShield->setEnabled(false);
    if (cs->WizardsBook == true)
        ui->toolButtonWizardBook->setEnabled(true);
    else
        ui->toolButtonWizardBook->setEnabled(false);
}

void MainWindow::on_toolButtonBlackMask_released()
{
    if (bmOpen == false)
    {
        festival_say_text("Launching Black Mask utility.");
        bm = new BlackMask(*cs);
        connect(bm, SIGNAL(finished(int)), this, SLOT(bmClosed()));
        bm->show();
        bmOpen = true;
    }
    else
    {
        bm->show();
        bm->raise();
        bm->activateWindow();
    }
}

void MainWindow::bmClosed()
{
    bmOpen = false;
}

void MainWindow::on_toolButtonCloak_released()
{
    if (isConnected==true)
    {
        if (ckOpen == false)
        {
            festival_say_text("Launching Cloak utility.");
            ck = new Cloak(*cs);
            connect(ck, SIGNAL(finished(int)), this, SLOT(cloakClosed()));
            ck->show();
            ckOpen = true;
        }
        else
        {
            ck->show();
            ck->raise();
            ck->activateWindow();
        }
    }
    else
    {
        festival_say_text("You must connect to the internet before running this program.");
    }
}

void MainWindow::cloakClosed()
{
    ckOpen = false;
}

void MainWindow::on_toolButtonDarcKnight_released()
{
    if (isConnected==true)
    {
        if (dkOpen == false)
        {
            festival_say_text("Launching Dark Knight utility.");
            dk = new DarcKnight(*cs);
            connect(dk, SIGNAL(finished(int)), this, SLOT(darcKnightClosed()));
            dk->show();
            dkOpen = true;
        }
        else
        {
            dk->show();
            dk->raise();
            dk->activateWindow();
        }
    }
    else
    {
        festival_say_text("You must connect to the internet before running this program.");
    }
}

void MainWindow::darcKnightClosed()
{
    dkOpen = false;
}


void MainWindow::on_toolButtonFlatline_released()
{
    if (flOpen == false)
    {
        festival_say_text("Launching Flat Line.");
        fl = new Flatline(*cs);
        connect(fl, SIGNAL(finished(int)), this, SLOT(flClosed()));
        fl->show();
        flOpen = true;
    }
    else
    {
        fl->show();
        fl->raise();
        fl->activateWindow();
    }
}

void MainWindow::flatlineClosed()
{
    flOpen = false;
}

void MainWindow::on_toolButtonGenie_released()
{
    if (gnOpen == false)
    {
        festival_say_text("Launching Genie.");
        gn = new Genie(*cs);
        connect(gn, SIGNAL(finished(int)), this, SLOT(gnClosed()));
        gn->show();
        gnOpen = true;
    }
    else
    {
        gn->show();
        gn->raise();
        gn->activateWindow();
    }
}

void MainWindow::genieClosed()
{
    gnOpen = false;
}

void MainWindow::on_toolButtonGuardDog_released()
{
    if (gdOpen == false)
    {
        festival_say_text("Launching Guard Dog.");
        gd = new GuardDog(*cs);
        connect(gd, SIGNAL(finished(int)), this, SLOT(guardDogClosed()));
        gd->show();
        gdOpen = true;
    }
    else
    {
        gd->show();
        gd->raise();
        gd->activateWindow();
    }
}

void MainWindow::guardDogClosed()
{
    gdOpen = false;
}

void MainWindow::on_toolButtonHammer_released()
{

}

void MainWindow::hammerClosed()
{
    hmOpen = false;
}


void MainWindow::on_toolButtonHellhound_released()
{

}

void MainWindow::hellHoundClosed()
{
    hmOpen = false;
}

void MainWindow::on_toolButtonHydra_released()
{

}

void MainWindow::hydraClosed()
{
    hyOpen = false;
}

void MainWindow::on_toolButtonLightningBug_released()
{

}

void MainWindow::lightningBugClosed()
{
    lbOpen = false;
}

void MainWindow::on_toolButtonReaper_released()
{

}

void MainWindow::reaperClosed()
{
    rpOpen = false;
}

void MainWindow::on_toolButtonShield_released()
{

}

void MainWindow::shieldClosed()
{
    shOpen = false;
}

void MainWindow::on_toolButtonWizardBook_released()
{

}

void MainWindow::wizardBookClosed()
{
    wbOpen = false;
}
