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

#ifndef SPEECHTHREAD_H
#define SPEECHTHREAD_H

#include <QThread>
#include <EST_String.h>

class SpeechThread : public QThread
{
Q_OBJECT
public:
    explicit SpeechThread(EST_String TextToSay, QObject *parent = 0);
    EST_String SpeechText;
    void run();
    bool firstRun;

protected:

private:

signals:

public slots:

};

#endif // SPEECHTHREAD_H
