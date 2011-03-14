/***************************************************************************
 *   Copyright (C) 2009,2010 by Spindles                                   *
 *   spindles@wonkyspanner.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef CONFIGWIZARD_H
#define CONFIGWIZARD_H

#include <QWizard>
#include <QtSql>
#include <QFileDialog>
#include <QPushButton>
#include <QComboBox>

class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;

using namespace QSql;

//!  ConfigWizard. 
/*!
  Main QWizard class.
*/
class ConfigWizard:public QWizard
{
      Q_OBJECT

public:
      //! enum of wizard pages
      /*! Enumeration of pages and the order in which they appear. */
      enum { Intro_Page, Load_Or_New_Page, Load_Page, New_Page, Basic_Stats, Deck_Stats, Last_Page };
      ConfigWizard(QWidget *parent = 0);

protected:

private slots:

private:

};

//!  IntroPage. 
/*!
  Introduction QWizardPage.
*/
class IntroPage : public QWizardPage
{
     Q_OBJECT

public:
     IntroPage(QWidget *parent = 0);

private:
     QLabel *label;
};

//!  LoadOrNewPage.
/*!
  Load Existing or Create New User QWizardPage.
*/
class LoadOrNewPage : public QWizardPage
{
     Q_OBJECT

public:
     LoadOrNewPage(QWidget *parent = 0);
     int nextId() const;

private:
     QLabel *label;
     QGroupBox *groupBox;
     QRadioButton *radioButtonNew;
     QRadioButton *radioButtonExisting;
};

//!  LoadPage.
/*!
  Load Existing User QWizardPage.
*/
class LoadPage : public QWizardPage
{
     Q_OBJECT

public:
     LoadPage(QWidget *parent = 0);
     int nextId() const;

private slots:
     void setOpenFileName();

private:
     QLineEdit *fileToLoad;
     QPushButton *btnLoadFile;
};

//!  NewPage.
/*!
  Create New User QWizardPage.
*/
class NewPage : public QWizardPage
{
     Q_OBJECT

public:
     NewPage(QWidget *parent = 0);
     void CreateDatabase();
     bool validatePage();

private:
     QLabel *nameLabel;
     QLabel *emailLabel;
     QLabel *passLabel;
     QLineEdit *nameLineEdit;
     QLineEdit *emailLineEdit;
     QLineEdit *passLineEdit;
};

//!  BasicStats.
/*!
  Create basic stats for new user QWizardPage.
*/
class BasicStats : public QWizardPage
{
     Q_OBJECT

public:
     BasicStats(QWidget *parent = 0);
     bool validatePage();

private:
     QLabel *intLabel;
     QLabel *netLabel;
     QComboBox *intComboBox;
     QComboBox *netComboBox;
};

//!  DeckStats.
/*!
  Create stats for cyberdeck QWizardPage.
*/
class DeckStats : public QWizardPage
{
     Q_OBJECT

public:
     DeckStats(QWidget *parent = 0);
     bool validatePage();
     void insertSoftware(QString softwareName, bool hasSoftware);

private:
     QLabel *lblBlackMask;
     QCheckBox *chkBlackMask;
     QLabel *lblCloak;
     QCheckBox *chkCloak;
     QLabel *lblDArcKnight;
     QCheckBox *chkDArcKnight;
     QLabel *lblFlatline;
     QCheckBox *chkFlatline;
     QLabel *lblGenie;
     QCheckBox *chkGenie;
     QLabel *lblGuardDog;
     QCheckBox *chkGuardDog;
     QLabel *lblHammer;
     QCheckBox *chkHammer;
     QLabel *lblHellHound;
     QCheckBox *chkHellHound;
     QLabel *lblHydra;
     QCheckBox *chkHydra;
     QLabel *lblLightningBug;
     QCheckBox *chkLightningBug;
     QLabel *lblReaper;
     QCheckBox *chkReaper;
     QLabel *lblShield;
     QCheckBox *chkShield;
     QLabel *lblWizardsBook;
     QCheckBox *chkWizardsBook;
};


//!  LastPage.
/*!
  Success.
*/
class LastPage : public QWizardPage
{
     Q_OBJECT

public:
     LastPage(QWidget *parent = 0);

private:
     QLabel *successTextLabel;
};

#endif
