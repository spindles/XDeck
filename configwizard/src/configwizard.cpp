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


#include <QtGui>
#include "configwizard.h"

#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QVariant>

QSqlDatabase configdb;
QString databasePath;

//!  Default Constructor 
/*!
  Default Constructor for ConfigWizard class
*/
ConfigWizard::ConfigWizard(QWidget *parent) : QWizard(parent)
{
     setPage(Intro_Page, new IntroPage);
     setPage(Load_Or_New_Page, new LoadOrNewPage);
     setPage(Load_Page, new LoadPage);
     setPage(New_Page, new NewPage);
     setPage(Basic_Stats, new BasicStats);
     setPage(Deck_Stats, new DeckStats);
     setPage(Last_Page, new LastPage);

     setStartId(0);
}

//!  IntroPage Constructor
/*!
  Default Constructor for IntroPage class
*/
IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent)
{
     setTitle(tr("Configuration Wizard"));
//     setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

     label = new QLabel(tr("Welcome to X-Deck. In order for this software to "
                           "function properly it needs to be populated with "
                           "details specific to your character.\n\n"
                           "This wizard will allow you to either generate a "
                           "new user definition, or to load a "
                           "pre-existing user profile into X-Deck. \n\n"
                           "Please complete this wizard and then X-Deck "
                           "will be ready to fulfil all your NetRunning needs."));
     label->setWordWrap(true);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(label);
     setLayout(layout);
}

//!  LoadOrNewPage Constructor
/*!
  Default Constructor for LoadOrNewPage class
*/
LoadOrNewPage::LoadOrNewPage(QWidget *parent) : QWizardPage(parent)
{
     setTitle(tr("Configuration Wizard"));
     setSubTitle(tr("Would you like to load an existing user or create a new user?"));

     groupBox = new QGroupBox(tr("Choose.."));
     radioButtonNew = new QRadioButton(tr("&New User"));
     radioButtonExisting = new QRadioButton(tr("&Load Existing User"));


     QVBoxLayout *groupBoxLayout = new QVBoxLayout;
     groupBoxLayout->addWidget(radioButtonNew);
     groupBoxLayout->addWidget(radioButtonExisting);

     radioButtonNew->setChecked(true);

     setLayout(groupBoxLayout);
}

//!  Overloaded Method nextId
/*!
  Checks the value of radioButtonNew and sets next page id
*/
int LoadOrNewPage::nextId() const
{
     if (radioButtonNew->isChecked()) {
         return ConfigWizard::New_Page;
     } else {
         return ConfigWizard::Load_Page;
     }
}

//!  LoadPage Constructor
/*!
  Default Constructor for LoadPage class
*/
LoadPage::LoadPage(QWidget *parent) : QWizardPage(parent)
{
     setTitle(tr("Configuration Wizard"));
     setSubTitle(tr("Select a user database to load..."));
     fileToLoad = new QLineEdit;
     btnLoadFile = new QPushButton("Load file...");
     QGridLayout *layout = new QGridLayout;
     layout->addWidget(fileToLoad, 0, 0);
     layout->addWidget(btnLoadFile, 0, 1);
     setLayout(layout);
     connect(btnLoadFile, SIGNAL(clicked()), this, SLOT(setOpenFileName()));

}

//!  setOpenFileName method
/*!
  Launches a QFileDialog to select a user database then stores the path and filename in a config file.
*/
void LoadPage::setOpenFileName()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QDir d( QDir::home() );
    QString homeDir = d.absolutePath();
    homeDir.append("/.configwizard");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Select file to open",
                                                    homeDir,
                                                    tr("SQLite Databases (*.db)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty())
    {
        fileToLoad->setText(fileName);
        QString userFileName = homeDir;
        userFileName.append("/userFileName");
        QFile userFile(userFileName);
        userFile.open(QIODevice::WriteOnly);
        userFile.write(fileName.toLatin1());
        userFile.close();
    }
}

//!  nextId method
/*!
  Overloaded method to set correct next page ID
*/
int LoadPage::nextId() const
{
    return ConfigWizard::Last_Page;
}

//!  NewPage Constructor
/*!
  Default Constructor for NewPage class
*/
NewPage::NewPage(QWidget *parent) : QWizardPage(parent)
{
     setTitle(tr("Configuration Wizard"));
     setSubTitle(tr("Please complete the following details..."));

     nameLabel = new QLabel(tr("Character N&ame:"));
     nameLineEdit = new QLineEdit;
     nameLabel->setBuddy(nameLineEdit);

     emailLabel = new QLabel(tr("&Email address:"));
     emailLineEdit = new QLineEdit;
     emailLineEdit->setValidator(new QRegExpValidator(QRegExp(".*@.*"), this));
     emailLabel->setBuddy(emailLineEdit);

     passLabel = new QLabel(tr("&Password:"));
     passLineEdit = new QLineEdit;
     passLineEdit->setEchoMode(QLineEdit::Password);
     passLabel->setBuddy(nameLineEdit);

     registerField("evaluate.name*", nameLineEdit);
     registerField("evaluate.email*", emailLineEdit);
     registerField("evaluate.pass*", passLineEdit);

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(nameLabel, 0, 0);
     layout->addWidget(nameLineEdit, 0, 1);
     layout->addWidget(emailLabel, 1, 0);
     layout->addWidget(emailLineEdit, 1, 1);
     layout->addWidget(passLabel, 2, 0);
     layout->addWidget(passLineEdit, 2, 1);
     setLayout(layout);
}

bool NewPage::validatePage()
{
     CreateDatabase();
     return true;
}

//!  CreateDatabase method
/*!
  Creates a new SQLITE database in the user's home/.configwizard directory,
  named according to the username input in the previous page.
*/
void NewPage::CreateDatabase()
{
     QDir d( QDir::home() );
     d.cd(".configwizard");
     QString dbPath = d.path();
     dbPath.append("/");
     QString userName = nameLineEdit->text();
     userName.replace(" ", "");
     QString userEmail = emailLineEdit->text();
     userEmail.replace(" ", "");
     QString userPass = passLineEdit->text();
     userPass.replace(" ", "");
     dbPath.append(userName);
     dbPath.append(".db");
     databasePath = dbPath;

     configdb = QSqlDatabase::addDatabase("QSQLITE");
     configdb.setDatabaseName(dbPath);
     if( configdb.open() )
     {
        qWarning("Database created");
     }
     else
     {
        qWarning("Database not created");
     }

     qWarning() << "Path to database is " << databasePath;

     QString userFileName = d.absolutePath();
     userFileName.append("/userFileName");
     QFile userFile(userFileName);
     userFile.open(QIODevice::WriteOnly);
     userFile.write(databasePath.toLatin1());
     userFile.close();


     // Create the database structure
     QSqlQuery createTables(configdb);
     QString createTablesSQL = "CREATE TABLE \"tuser\" (\"userid\", \"username\", \"useremail\", \"password\")";
     createTables.exec(createTablesSQL);
     QString insertUserSQL = "INSERT INTO tuser (userid, username, useremail, password) VALUES (1, \'";
     insertUserSQL.append(userName);
     insertUserSQL.append("\',\'");
     insertUserSQL.append(userEmail);
     insertUserSQL.append("\',\'");
     insertUserSQL.append(userPass);
     insertUserSQL.append("\')");
     createTables.exec(insertUserSQL);
     qWarning() << insertUserSQL;
}

//!  BasicStats Constructor
/*!
  Default Constructor for BasicStats class
*/
BasicStats::BasicStats(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Configuration Wizard"));
    setSubTitle(tr("Please complete the following details..."));
    intLabel = new QLabel(tr("INT"));
    netLabel = new QLabel(tr("Netrunning"));
    intComboBox = new QComboBox(parent);
    netComboBox = new QComboBox(parent);
    intComboBox->addItem("1", 1);
    intComboBox->addItem("2", 2);
    intComboBox->addItem("3", 3);
    intComboBox->addItem("4", 4);
    intComboBox->addItem("5", 5);
    netComboBox->addItem("1", 1);
    netComboBox->addItem("2", 2);
    netComboBox->addItem("3", 3);
    netComboBox->addItem("4", 4);
    netComboBox->addItem("5", 5);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(intLabel, 0, 0);
    layout->addWidget(intComboBox, 0, 1);
    layout->addWidget(netLabel, 1, 0);
    layout->addWidget(netComboBox, 1, 1);
    setLayout(layout);
}

bool BasicStats::validatePage()
{
    int intelligence = intComboBox->currentIndex() + 1;
    int netrunning = netComboBox->currentIndex() + 1;
    qWarning() << "intelligence: " << intelligence << ", NetRunning: " << netrunning;

    if( configdb.open() )
    {
        // Create the database structure
        QSqlQuery createStatTables(configdb);
        QString createStatTablesSQL = "CREATE TABLE \"tstats\" (\"userid\", \"intelligence\", \"netrunning\")";
        createStatTables.exec(createStatTablesSQL);
        QString insertStatsSQL = "INSERT INTO tstats (userid, intelligence, netrunning) VALUES (1, ";
        insertStatsSQL.append(QString::number(intelligence));
        insertStatsSQL.append(", ");
        insertStatsSQL.append(QString::number(netrunning));
        insertStatsSQL.append(")");
        createStatTables.exec(insertStatsSQL);
        qWarning() << insertStatsSQL;
        return true;
    }
    else
    {
        qWarning("Database not open");
        return false;
    }
}

DeckStats::DeckStats(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Configuration Wizard"));
    setSubTitle(tr("Please select software for your deck..."));

    lblBlackMask = new QLabel(tr("Black Mask"));
    chkBlackMask = new QCheckBox(parent);
    lblCloak = new QLabel(tr("Cloak"));
    chkCloak = new QCheckBox(parent);
    lblDArcKnight = new QLabel(tr("D'Arc Knight"));
    chkDArcKnight = new QCheckBox(parent);
    lblFlatline = new QLabel(tr("Flatline"));
    chkFlatline = new QCheckBox(parent);
    lblGenie = new QLabel(tr("Genie"));
    chkGenie = new QCheckBox(parent);
    lblGuardDog = new QLabel(tr("Guard Dog"));
    chkGuardDog = new QCheckBox(parent);
    lblHammer = new QLabel(tr("Hammer"));
    chkHammer = new QCheckBox(parent);
    lblHellHound = new QLabel(tr("HellHound"));
    chkHellHound = new QCheckBox(parent);
    lblHydra = new QLabel(tr("Hydra"));
    chkHydra = new QCheckBox(parent);
    lblLightningBug = new QLabel(tr("Lightning Bug"));
    chkLightningBug = new QCheckBox(parent);
    lblReaper = new QLabel(tr("Reaper"));
    chkReaper = new QCheckBox(parent);
    lblShield = new QLabel(tr("Shield"));
    chkShield = new QCheckBox(parent);
    lblWizardsBook = new QLabel(tr("Wizard's Book"));
    chkWizardsBook = new QCheckBox(parent);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lblBlackMask, 0, 0);
    layout->addWidget(chkBlackMask, 0, 1);
    layout->addWidget(lblCloak, 1, 0);
    layout->addWidget(chkCloak, 1, 1);
    layout->addWidget(lblDArcKnight, 2, 0);
    layout->addWidget(chkDArcKnight, 2, 1);
    layout->addWidget(lblFlatline, 3, 0);
    layout->addWidget(chkFlatline, 3, 1);
    layout->addWidget(lblGenie, 4, 0);
    layout->addWidget(chkGenie, 4, 1);
    layout->addWidget(lblGuardDog, 5, 0);
    layout->addWidget(chkGuardDog, 5, 1);
    layout->addWidget(lblHammer, 6, 0);
    layout->addWidget(chkHammer, 6, 1);
    layout->addWidget(lblHellHound, 0, 2);
    layout->addWidget(chkHellHound, 0, 3);
    layout->addWidget(lblHydra, 1, 2);
    layout->addWidget(chkHydra, 1, 3);
    layout->addWidget(lblLightningBug, 2, 2);
    layout->addWidget(chkLightningBug, 2, 3);
    layout->addWidget(lblReaper, 3, 2);
    layout->addWidget(chkReaper, 3, 3);
    layout->addWidget(lblShield, 4, 2);
    layout->addWidget(chkShield, 4, 3);
    layout->addWidget(lblWizardsBook, 5, 2);
    layout->addWidget(chkWizardsBook, 5, 3);
    setLayout(layout);
}

bool DeckStats::validatePage()
{
    if( configdb.open() )
    {
        // Create the database structure
        QSqlQuery createDeckStatTables(configdb);
        QString createDeckStatTablesSQL = "CREATE TABLE \"tdeckstats\" (\"softwaretype\", \"present\")";
        createDeckStatTables.exec(createDeckStatTablesSQL);
        insertSoftware("Black Mask", chkBlackMask->checkState());
        insertSoftware("Cloak", chkCloak->checkState());
        insertSoftware("DArc Knight", chkDArcKnight->checkState());
        insertSoftware("Flatline", chkFlatline->checkState());
        insertSoftware("Genie", chkGenie->checkState());
        insertSoftware("Guard Dog", chkGuardDog->checkState());
        insertSoftware("Hammer", chkHammer->checkState());
        insertSoftware("HellHound", chkHellHound->checkState());
        insertSoftware("Hydra", chkHydra->checkState());
        insertSoftware("Lightning Bug", chkLightningBug->checkState());
        insertSoftware("Reaper", chkReaper->checkState());
        insertSoftware("Shield", chkShield->checkState());
        insertSoftware("Wizards Book", chkWizardsBook->checkState());

        return true;
    }
    else
    {
        qWarning("Database not open");
        return false;
    }
}

void DeckStats::insertSoftware(QString softwareName, bool hasSoftware)
{
    QString insertDeckStatsSQL;
    QString hasSoftwareString;
    QSqlQuery insertSoftwareQuery(configdb);

    if (hasSoftware == true)
        hasSoftwareString = "true";
    else
        hasSoftwareString = "false";

    insertDeckStatsSQL = "INSERT INTO tdeckstats (softwaretype, present) VALUES (\"";
    insertDeckStatsSQL.append(softwareName);
    insertDeckStatsSQL.append("\", \"");
    insertDeckStatsSQL.append(hasSoftwareString);
    insertDeckStatsSQL.append("\")");
    qWarning() << insertDeckStatsSQL;
    insertSoftwareQuery.exec(insertDeckStatsSQL);
}

LastPage::LastPage(QWidget *parent) : QWizardPage(parent)
{
    successTextLabel = new QLabel(tr("Congratulations, your user details have been loaded. \n \n"
                                     "Happy Netrunning"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(successTextLabel);
    setLayout(layout);
}
