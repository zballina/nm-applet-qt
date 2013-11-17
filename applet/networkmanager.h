#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtGui/QSystemTrayIcon>
#include <QtCore/QMap>
#include <QtGui/QAction>
#include <QtCore/QString>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/AccessPoint>
#include "remoteinterfaceconnection.h"
#include "remoteactivatablelist.h"

class NetworkManagerApplet : public QSystemTrayIcon
{
        Q_OBJECT
    public:
        explicit NetworkManagerApplet(QObject *parent = 0);
        ~NetworkManagerApplet();
        void init();

        QList<QAction*> contextualActions();
    signals:

    public slots:
        /**
         * Handle signals from NM if wireless was disabled in software
         */
        void managerWirelessEnabledChanged(bool);
        /**
         * Handle signals from NM if wireless was disabled in hardware
         */
        void managerWirelessHardwareEnabledChanged(bool);
        /**
         * Handle clicks to enable/disable enabled
         */
        void userNetworkingEnabledChanged(bool);
        /**
         * Handle clicks to enable/disable wireless
         */
        void userWirelessEnabledChanged(bool);
        /**
         * React to manager status changes
         */
        void managerStatusChanged(NetworkManager::Status);

    private slots:
        void iconActivated(QSystemTrayIcon::ActivationReason reason);

        void deviceAdded(const QString& = QString());
        void deviceRemoved(const QString&);
        void interfaceConnectionStateChanged();
        void updatePixmap();
        void finishInitialization();
        void activatableAdded(RemoteActivatable*);
        void activatableRemoved(RemoteActivatable*);
        void vpnActivationStateChanged(Knm::InterfaceConnection::ActivationState, Knm::InterfaceConnection::ActivationState);
        void activatablesDisappeared();

        void resetActiveSystrayInterface();
        void updateActiveInterface(bool);
        void setupAccessPointSignals(const QString &);
        void updateWireless(bool checked);

    private:
        inline void setActiveInterface(const NetworkManager::Device::Ptr &device = NetworkManager::Device::Ptr());
        inline void setActiveSystrayInterface(const NetworkManager::Device::Ptr &device = NetworkManager::Device::Ptr());
        void updateInterfaceList();
        void setupInterfaceSignals();

        NetworkManager::Device::List m_interfaces;
        RemoteActivatableList* m_activatables;

        QPixmap m_pixmap;
        QString m_currentPixmapIconName;
        QMap<QUuid, QWeakPointer<RemoteInterfaceConnection> > m_activeVpnConnections;
        int m_totalActiveVpnConnections;

        // For tracking which status we should show.
        // This one is always the interface which has the default route
        // or the first interface selected after sorting the current
        // interface list using networkInterfaceLessThan().
        // WARNING: do not directly assign this pointer, use setActiveInterface instead.
        NetworkManager::Device::Ptr m_activeInterface;
        // Interface used to update system tray icon. If we have only one interface
        // then this one is always equals to m_activeInterfaceState.
        // WARNING: do not directly assign this pointer, use setActiveSystrayInterface instead.
        NetworkManager::Device::Ptr m_activeSystrayInterface;
        NetworkManager::AccessPoint* m_accessPoint;

        // Timeline controlling a connection progress overlay on the main icon
//        QTimeLine m_overlayTimeline;

        NetworkManager::Device::State m_activeInterfaceState;
        NetworkManager::Device::State m_activeSystrayInterfaceState;

        QString m_lastActiveInterfaceUni;
        QString m_lastActiveSystrayInterfaceUni;

        class Private;
        Private * const d;
};

#endif // NETWORKMANAGER_H
