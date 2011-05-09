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


#include "speechthread.h"
#include "festival.h"
#include <QDebug>

SpeechThread::SpeechThread(EST_String TextToSay, QObject *parent) :
    QThread(parent)
{
    this->SpeechText = TextToSay;
}

void SpeechThread::run()
{
    int heap_size = 10000000;
    int load_init_files = 1;
    festival_initialize(load_init_files,heap_size);

    festival_say_text(SpeechText);
    this->quit();
}
