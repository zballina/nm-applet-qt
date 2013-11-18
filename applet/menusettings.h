#ifndef MENUSETTINGS_H
#define MENUSETTINGS_H

#include <QtGui/QMenu>
#include <NetworkManagerQt/Manager>

class QAction;
class InfoConnection;
class About;

class MenuSettings : public QMenu
{
        Q_OBJECT
    public:
        explicit MenuSettings(QWidget *parent = 0);
        ~MenuSettings();
    protected:
        void closeEvent(QCloseEvent *event);

    signals:

    private slots:
        void showNetworkSettings();
        void showAbout();
        void activateNetwork(bool);
        void activateWireless(bool);
        void activateShowNotifications(bool);
        void statusChange(NetworkManager::Status status);

        void networkingEnabledChange(bool);
        void wirelessEnabledChange(bool);

    private:
        void createActions();
        void addActionsMenu();

        QAction *mShowNetworkSettingsAction;
        QAction *mShowAboutAction;
        QAction *mActivateNetworkAction;
        QAction *mActivateWirelessAction;
        QAction *mActivateShowNotificationsAction;
        QAction *mShowInfoConnectionAction;
        QAction *mQuitAction;
        About *mAbout;
};

#endif // MENUSETTINGS_H
