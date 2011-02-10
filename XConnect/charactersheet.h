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

#ifndef CHARACTERSHEET_H
#define CHARACTERSHEET_H

#include <QObject>
#include <QString>
#include <QtSql>
#include <QImage>

using namespace QSql;

class CharacterSheet : public QObject
{
public:
    CharacterSheet();
    ~CharacterSheet();
    QSqlDatabase configdb;
    QString userFileName;
    QString userName;
    QString userEmail;
    QVector<QImage> avatars;
    int intelligence;
    int netrunning;
    bool BlackMask;
    bool Cloak;
    bool DArcKnight;
    bool Flatline;
    bool Genie;
    bool GuardDog;
    bool Hammer;
    bool HellHound;
    bool Hydra;
    bool LightningBug;
    bool Reaper;
    bool Shield;
    bool WizardsBook;
    void loadAvatars();
    void loadUserDetails();
};

#endif // CHARACTERSHEET_H
