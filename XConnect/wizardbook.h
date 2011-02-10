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

#ifndef WIZARDBOOK_H
#define WIZARDBOOK_H

#include <QDialog>
#include "charactersheet.h"

namespace Ui {
    class WizardBook;
}

class WizardBook : public QDialog {
    Q_OBJECT
public:
    WizardBook(CharacterSheet& cs, QWidget *parent = 0);
    ~WizardBook();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WizardBook *ui;
};

#endif // WIZARDBOOK_H
