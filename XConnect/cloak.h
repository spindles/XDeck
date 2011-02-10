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

#ifndef CLOAK_H
#define CLOAK_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include "charactersheet.h"

namespace Ui {
    class Cloak;
}

class Cloak : public QDialog {
    Q_OBJECT
public:
    Cloak(CharacterSheet& cs, QWidget *parent = 0);
    ~Cloak();
    CharacterSheet* csLocal;
    int intelligence;
    int netrunning;
    int timerLength;
    int counter;
    bool timerRunning;
    QTimer *timer;
    QTime cloakTime;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Cloak *ui;

private slots:
    void on_pushButtonActivateCloak_released();
    void showTime();
};

#endif // CLOAK_H
