#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtGui/QMenu>
#include <QtCore/QDebug>
#include <NetworkManagerQt/Manager>

#include "about.h"
#include "infoconnection.h"
#include "networkmanagermenu.h"

NetworkManagerMenu::NetworkManagerMenu(QWidget *parent) :
    QMenu(parent), mAbout(new About(this)), mInfoConnection(new InfoConnection(this))
{
    createActions();
    addActionsMenu();
}

NetworkManagerMenu::~NetworkManagerMenu()
{
    delete mAbout;
    delete mInfoConnection;

}

void NetworkManagerMenu::createActions()
{
    mActivateNetworkAction = new QAction(tr("Activate &Network"), this);
    mActivateNetworkAction->setCheckable(true);
    connect(mActivateNetworkAction, SIGNAL(triggered(bool)), this, SLOT(activateNetwork(bool)));

    mActivateWirelessAction = new QAction(tr("Activate &Wireless"), this);
    mActivateWirelessAction->setCheckable(true);
    connect(mActivateWirelessAction, SIGNAL(triggered(bool)), this, SLOT(activateWireless(bool)));

    mActivateShowNotificationsAction = new QAction(tr("Show noti&fications"), this);
    mActivateShowNotificationsAction->setCheckable(true);
    connect(mActivateShowNotificationsAction, SIGNAL(triggered(bool)), this, SLOT(activateShowNotifications(bool)));

    mShowInfoConnectionAction = new QAction(tr("Show &information connection"), this);
    connect(mShowInfoConnectionAction, SIGNAL(triggered(bool)), this, SLOT(showInfoConnection()));

    mShowNetworkSettingsAction = new QAction(tr("Show network &settings"), this);
    connect(mShowNetworkSettingsAction, SIGNAL(triggered()), this, SLOT(showNetworkSettings()));

    mQuitAction = new QAction(tr("&Quit"), this);
    connect(mQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void NetworkManagerMenu::addActionsMenu()
{
    this->addAction(mActivateNetworkAction);
    this->addAction(mActivateWirelessAction);
    this->addSeparator();
    this->addAction(mActivateShowNotificationsAction);
    this->addSeparator();
    this->addAction(mShowInfoConnectionAction);
    this->addAction(mShowNetworkSettingsAction);
    this->addSeparator();
    this->addAction(mQuitAction);
}

void NetworkManagerMenu::showNetworkSettings()
{
    QMessageBox::information(0, tr("Network Settings"),
                          tr("Sorry, I already gave what help I could.\n"
                             "Maybe you should try asking a human?"));

}

void NetworkManagerMenu::showInfoConnection()
{
    qDebug() << "show info connection";
        mInfoConnection->show();
}

void NetworkManagerMenu::showAbout()
{
    qDebug() << "show About";
    mAbout->show();
}

void NetworkManagerMenu::activateNetwork(bool)
{

}

void NetworkManagerMenu::activateWireless(bool)
{

}

void NetworkManagerMenu::activateShowNotifications(bool)
{

}

void NetworkManagerMenu::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}
