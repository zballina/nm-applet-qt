#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtGui/QMenu>
#include <QtCore/QDebug>

#include "about.h"
#include "infoconnection.h"
#include "networkmanagermenu.h"

NetworkManagerMenu::NetworkManagerMenu(QWidget *parent) :
    QMenu(parent),
    mAbout(new About(this)),
    mInfoConnection(new InfoConnection(this))
{
    createActions();
    addActionsMenu();
    this->mActivateNetworkAction->setChecked(NetworkManager::isNetworkingEnabled());
    this->mActivateWirelessAction->setChecked(NetworkManager::isWirelessEnabled());

    NetworkManager::Notifier *notifier = NetworkManager::notifier();
    connect(notifier, SIGNAL(networkingEnabledChanged(bool)), this, SLOT(networkingEnabledChange(bool)));
    connect(notifier, SIGNAL(wirelessEnabledChanged(bool)), this, SLOT(wirelessEnabledChange(bool)));
    connect(notifier, SIGNAL(statusChanged(NetworkManager::Status)), this, SLOT(statusChange(NetworkManager::Status)));
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
    mInfoConnection->show();
}

void NetworkManagerMenu::showAbout()
{
    mAbout->show();
}

void NetworkManagerMenu::activateNetwork(bool enable)
{
    NetworkManager::setNetworkingEnabled(enable);
}

void NetworkManagerMenu::networkingEnabledChange(bool enable)
{
    mActivateNetworkAction->setChecked(enable);
    mActivateWirelessAction->setVisible(enable ? true: false);
}

void NetworkManagerMenu::activateWireless(bool enable)
{
    NetworkManager::setWirelessEnabled(enable);
}

void NetworkManagerMenu::wirelessEnabledChange(bool enable)
{
    mActivateWirelessAction->setChecked(enable);
}

void NetworkManagerMenu::activateShowNotifications(bool show)
{

}

void NetworkManagerMenu::statusChange(NetworkManager::Status status)
{
    switch (status)
    {
        case NetworkManager::Asleep:
        case NetworkManager::Disconnected:
        case NetworkManager::Connecting:
        case NetworkManager::Disconnecting:
        case NetworkManager::Unknown:
            mShowInfoConnectionAction->setEnabled(false);
            break;
        case NetworkManager::Connected:
        case NetworkManager::ConnectedLinkLocal:
        case NetworkManager::ConnectedSiteOnly:
            mShowInfoConnectionAction->setEnabled(true);
            break;
    }
}

void NetworkManagerMenu::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}
