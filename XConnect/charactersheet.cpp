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

#include "charactersheet.h"
#include <QDir>
#include <QFile>

CharacterSheet::CharacterSheet()
{
    loadUserDetails();
    loadAvatars();
}

void CharacterSheet::loadUserDetails()
{
    QDir d( QDir::home() );
    d.cd(".configwizard");
    QString dbPath = d.path();
    dbPath.append("/");
    QString configFilePath = dbPath;
    configFilePath.append("userFileName");
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qWarning("File not found");
    }
    else
    {
        userFileName = file.readAll();
        //qWarning(userFileName.toLatin1());
    }

    dbPath = userFileName;
    configdb = QSqlDatabase::addDatabase("QSQLITE","userDB");
    configdb.setDatabaseName(dbPath);
    if( configdb.open() )
    {
        //qWarning("Database open");
    }
    else
    {
        //qWarning() << "Database not open! Filepath:" << dbPath;
    }

    QSqlQuery getPlayerStats(configdb);

    // Get base data
    QString getPlayerBaseData = "SELECT * FROM tuser";
    getPlayerStats.exec(getPlayerBaseData);
    getPlayerStats.next();
    userName = getPlayerStats.value(1).toString();
    userEmail = getPlayerStats.value(2).toString();

    // qWarning() << "Name = " << userName << " email = " << userEmail;

    // Get Stats
    QString getPlayerStatsSQL = "SELECT * FROM tstats";
    getPlayerStats.exec(getPlayerStatsSQL);
    getPlayerStats.next();
    intelligence = getPlayerStats.value(1).toInt();
    netrunning = getPlayerStats.value(2).toInt();

    QString getDeckStatsSQL = "SELECT * FROM tdeckstats";
    getPlayerStats.exec(getDeckStatsSQL);
    while (getPlayerStats.next())
    {
       if(getPlayerStats.value(0).toString() == "Black Mask")
       {
           if(getPlayerStats.value(1).toString() == "true")
               BlackMask = true;
           else
               BlackMask = false;
       }
       if(getPlayerStats.value(0).toString() == "Cloak")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Cloak = true;
           else
               Cloak = false;
       }
       if(getPlayerStats.value(0).toString() == "DArc Knight")
       {
           if(getPlayerStats.value(1).toString() == "true")
               DArcKnight = true;
           else
               DArcKnight = false;
       }
       if(getPlayerStats.value(0).toString() == "Flatline")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Flatline = true;
           else
               Flatline = false;
       }
       if(getPlayerStats.value(0).toString() == "Genie")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Genie = true;
           else
               Genie = false;
       }
       if(getPlayerStats.value(0).toString() == "Guard Dog")
       {
           if(getPlayerStats.value(1).toString() == "true")
               GuardDog = true;
           else
               GuardDog = false;
       }
       if(getPlayerStats.value(0).toString() == "Hammer")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Hammer = true;
           else
               Hammer = false;
       }
       if(getPlayerStats.value(0).toString() == "HellHound")
       {
           if(getPlayerStats.value(1).toString() == "true")
               HellHound = true;
           else
               HellHound = false;
       }
       if(getPlayerStats.value(0).toString() == "Hydra")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Hydra = true;
           else
               Hydra = false;
       }
       if(getPlayerStats.value(0).toString() == "Lightning Bug")
       {
           if(getPlayerStats.value(1).toString() == "true")
               LightningBug = true;
           else
               LightningBug = false;
       }
       if(getPlayerStats.value(0).toString() == "Reaper")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Reaper = true;
           else
               Reaper = false;
       }
       if(getPlayerStats.value(0).toString() == "Shield")
       {
           if(getPlayerStats.value(1).toString() == "true")
               Shield = true;
           else
               Shield = false;
       }
       if(getPlayerStats.value(0).toString() == "Wizards Book")
       {
           if(getPlayerStats.value(1).toString() == "true")
               WizardsBook = true;
           else
               WizardsBook = false;
       }
    }
    configdb.close();
    configdb.removeDatabase("userDB");
}

void CharacterSheet::loadAvatars()
{
    QDir resourceDir = QDir(":/avatars/images/avatars");
    int numberOfAvatars = resourceDir.count();
    avatars = QVector<QImage>::QVector(numberOfAvatars);
    QStringList fileNames = resourceDir.entryList();
    for (int i = 0; i < fileNames.size(); i++)
    {
        QString avatarFileName = resourceDir.absolutePath();
        avatarFileName.append("/");
        avatarFileName.append(fileNames.at(i));
        QImage avatar = QImage::QImage(avatarFileName, 0);
        avatars[i] = avatar;
    }
}

CharacterSheet::~CharacterSheet()
{
    //default destructor
}
