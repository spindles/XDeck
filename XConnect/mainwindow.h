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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QToolButton>
#include "EST_String.h"
#include "charactersheet.h"
#include "blackmask.h"
#include "cloak.h"
#include "darcknight.h"
#include "flatline.h"
#include "genie.h"
#include "guarddog.h"
#include "hammer.h"
#include "hellhound.h"
#include "hydra.h"
#include "lightningbug.h"
#include "reaper.h"
#include "shield.h"
#include "wizardbook.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    bool isConnected;
    qreal connectionNodes[10][2];
    QTimer *introSceneTimer;
    void setupIntroScene();
    int introSceneNumber;
    QGraphicsScene *introScene;
    CharacterSheet *cs;
    void loadDeckSoftware();
    bool bmOpen;
    bool ckOpen;
    bool dkOpen;
    bool flOpen;
    bool gnOpen;
    bool gdOpen;
    bool hmOpen;
    bool hhOpen;
    bool hyOpen;
    bool lbOpen;
    bool rpOpen;
    bool shOpen;
    bool wbOpen;
    BlackMask* bm;
    Cloak* ck;
    DarcKnight* dk;
    Flatline* fl;
    Genie* gn;
    GuardDog* gd;
    Hammer* hm;
    HellHound* hh;
    Hydra* hy;
    LightningBug* lb;
    Reaper* rp;
    Shield* sh;
    WizardBook* wb;

    void Speak(EST_String text);

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;

private slots:
    void on_toolButtonWizardBook_released();
    void on_toolButtonShield_released();
    void on_toolButtonReaper_released();
    void on_toolButtonLightningBug_released();
    void on_toolButtonHydra_released();
    void on_toolButtonHellhound_released();
    void on_toolButtonHammer_released();
    void on_toolButtonGuardDog_released();
    void on_toolButtonGenie_released();
    void on_toolButtonFlatline_released();
    void on_toolButtonDarcKnight_released();
    void on_toolButtonCloak_released();
    void on_toolButtonBlackMask_released();
    void on_toolButtonShutdown_released();
    void on_toolButtonSoftware_toggled(bool checked);
    void on_toolButtonNetwork_toggled(bool checked);
    void on_toolButtonDisconnect_clicked();
    void on_toolButtonConnect_clicked();
    void updateIntroMap();
    void bmClosed();
    void cloakClosed();
    void darcKnightClosed();
    void flatlineClosed();
    void genieClosed();
    void guardDogClosed();
    void hammerClosed();
    void hellHoundClosed();
    void hydraClosed();
    void lightningBugClosed();
    void reaperClosed();
    void shieldClosed();
    void wizardBookClosed();
};

#endif // MAINWINDOW_H
