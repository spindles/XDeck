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

#include <QtGui/QApplication>
#include "mainwindow.h"
#include <festival.h>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int heap_size = 10000000;
    int load_init_files = 1;
    festival_initialize(load_init_files,heap_size);
    MainWindow w;
    if(w.cs->userFileName=="NULL")
    {
        QMessageBox msgBox;
        msgBox.setText("You must run configwizard before you can use xconnect!");
        msgBox.exec();
        a.quit();
    }
    else
    {
        w.setWindowFlags(Qt::FramelessWindowHint);
        w.move(QPoint(150,0));
        w.show();
        return a.exec();
    }
}
