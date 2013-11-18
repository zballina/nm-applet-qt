#include "networkmanager.h"
#include "nmdialog.h"
#include "menusettings.h"
#include "uiutils.h"

#include <QtGui/QMessageBox>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/ModemDevice>

bool networkInterfaceLessThan(const NetworkManager::Device::Ptr &if1, const NetworkManager::Device::Ptr &if2);
bool networkInterfaceSameConnectionStateLessThan(const NetworkManager::Device::Ptr &if1, const NetworkManager::Device::Ptr &if2);

class NetworkManagerApplet::Private
{
public:
    Private(): m_popup(0) { }

    NMDialog * m_popup;
    MenuSettings *m_settings;
    QList<QAction*> actions;
};

NetworkManagerApplet::NetworkManagerApplet(QObject *parent) :
    QSystemTrayIcon(parent),
    m_activatables(0),
    m_totalActiveVpnConnections(0),
    m_activeInterface(0),
    m_activeSystrayInterface(0),
    d(new Private())
{
    setIcon(QIcon(":/icons/nm-applet-qt.svg"));

    m_activeInterfaceState = NetworkManager::Device::UnknownState;
    updateInterfaceList();

    if (!m_interfaces.isEmpty()) {
        qSort(m_interfaces.begin(), m_interfaces.end(), networkInterfaceLessThan);
        setActiveInterface(m_interfaces.first());
        setActiveSystrayInterface(m_activeInterface);
        m_activeSystrayInterfaceState = NetworkManager::Device::UnknownState;
    }

    m_activatables = new RemoteActivatableList(this);
    connect(m_activatables, SIGNAL(activatableAdded(RemoteActivatable*,int)), this, SLOT(activatableAdded(RemoteActivatable*)));
    connect(m_activatables, SIGNAL(activatableRemoved(RemoteActivatable*)), this, SLOT(activatableRemoved(RemoteActivatable*)));
    connect(m_activatables, SIGNAL(disappeared()), this, SLOT(activatablesDisappeared()));

    init();
}

NetworkManagerApplet::~NetworkManagerApplet()
{
}

void NetworkManagerApplet::init()
{
    QObject::connect(NetworkManager::notifier(), SIGNAL(deviceAdded(QString)),
            this, SLOT(deviceAdded(QString)));
    QObject::connect(NetworkManager::notifier(), SIGNAL(deviceRemoved(QString)),
            this, SLOT(deviceRemoved(QString)));

    QObject::connect(NetworkManager::notifier(), SIGNAL(statusChanged(NetworkManager::Status)),
                     this, SLOT(managerStatusChanged(NetworkManager::Status)));

    setupInterfaceSignals();

    d->m_popup = new NMDialog(m_activatables);
    d->m_settings = new MenuSettings();
    setContextMenu(d->m_settings);

    QAction* action = new QAction(tr("CheckBox to enable or disable networking completely", "Enable networking"), this);
    action->setToolTip(tr("@info:tooltip tooltip for the 'Enable networking' checkbox", "Enable or disable the networking system"));
    action->setCheckable(true);
    action->setChecked(NetworkManager::isNetworkingEnabled());
    connect(action, SIGNAL(triggered(bool)), d->m_popup, SLOT(networkingEnabledToggled(bool)));
    connect(NetworkManager::notifier(), SIGNAL(networkingEnabledChanged(bool)),
            action, SLOT(setChecked(bool)));

    d->actions.append(action);
    //setGraphicsWidget(d->m_popup);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("org.kde.kded", "/org/kde/networkmanagement", "org.kde.networkmanagement", "ModuleReady", this, SLOT(finishInitialization()));

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.networkmanagement"))
    {
        QTimer::singleShot(0, this, SLOT(finishInitialization()));
    }
    else
    {
        QDBusInterface kded(QLatin1String("org.kde.kded"), QLatin1String("/kded"),
                            QLatin1String("org.kde.kded"), QDBusConnection::sessionBus());
        kded.asyncCall(QLatin1String("loadModule"), QLatin1String("networkmanagement"));
    }
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void NetworkManagerApplet::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            if(d->m_popup->isVisible())
                d->m_popup->hide();
            else
                d->m_popup->show();
            break;
        case QSystemTrayIcon::DoubleClick:
        break;
        case QSystemTrayIcon::MiddleClick:
            QMessageBox::information(0, tr("Network Settings"),
                                  tr("Sorry, I already gave what help I could.\n"
                                     "Maybe you should try asking a human?"));
        break;
        default:
    ;
    }
}

void NetworkManagerApplet::updateInterfaceList()
{
    m_interfaces = NetworkManager::networkInterfaces();
// TODO Fixme Set icon in system tray
//    setStatus(m_interfaces.isEmpty() ? Plasma::PassiveStatus : Plasma::ActiveStatus);
}

void NetworkManagerApplet::finishInitialization()
{
    // Finishes kded module initialization.
    QDBusInterface networkmanagement(QLatin1String("org.kde.networkmanagement"), QLatin1String("/org/kde/networkmanagement"),
                                     QLatin1String("org.kde.networkmanagement"), QDBusConnection::sessionBus());

    networkmanagement.call(QLatin1String("FinishInitialization"));

    // m_activatables->init() must be called after SLOT(activatableAdded(RemoteActivatable*,int)) has been connected and
    // NMPopup has been allocated.
    m_activatables->init();

    // this needs m_activables initialized so that it can get the connection name to add to the InterfaceItem.
    interfaceConnectionStateChanged();

    // to force InterfaceItems to update their hasDefaultRoute state.
    if (m_activeInterface) {
        QMetaObject::invokeMethod(m_activeInterface.data(), "stateChanged",
                                  Q_ARG(NetworkManager::Device::State, m_activeInterface->state()),
                                  Q_ARG(NetworkManager::Device::State, NetworkManager::Device::UnknownState),
                                  Q_ARG(NetworkManager::Device::StateChangeReason, NetworkManager::Device::NoReason));
    }
}

void NetworkManagerApplet::vpnActivationStateChanged(Knm::InterfaceConnection::ActivationState oldState, Knm::InterfaceConnection::ActivationState newState)
{
    RemoteInterfaceConnection *ic = static_cast<RemoteInterfaceConnection*>(sender());
    QUuid id = ic->connectionUuid();
    switch (newState)
    {
        case Knm::InterfaceConnection::Activated:
            m_totalActiveVpnConnections++;
            // TODO Fixme:
            //setStatusOverlay(generateVpnProgressStatusOverlay(ic));
            //setStatusOverlay("task-complete");
            QTimer::singleShot(4000, this, SLOT(clearActivatedOverlay()));
            if (!m_activeVpnConnections.contains(id))
                m_activeVpnConnections.insert(id, QWeakPointer<RemoteInterfaceConnection>(ic));
            break;
        case Knm::InterfaceConnection::Activating:
            // TODO Fixme:
            //setStatusOverlay(generateVpnProgressStatusOverlay(ic));
            m_activeVpnConnections.insert(id, QWeakPointer<RemoteInterfaceConnection>(ic));
            break;
        case Knm::InterfaceConnection::Unknown:
            // TODO Fixme:
            //setStatusOverlay(QPixmap());
            m_activeVpnConnections.remove(id);
            if (oldState >= Knm::InterfaceConnection::Activated && m_totalActiveVpnConnections > 0) {
                m_totalActiveVpnConnections--;
            }
            break;
    }
    //kDebug() << newState << m_totalActiveVpnConnections;
    //update();
}

void NetworkManagerApplet::updateActiveInterface(bool hasDefaultRoute)
{
    RemoteInterfaceConnection *ic = qobject_cast<RemoteInterfaceConnection*>(sender());
    if (hasDefaultRoute) {
        // TODO: add support for VpnRemoteInterfaceConnection's, which have "any" as ic->deviceUni().
        setActiveInterface(NetworkManager::findNetworkInterface(ic->deviceUni()));
        connect(m_activeInterface.data(), SIGNAL(destroyed(QObject*)), SLOT(_k_destroyed(QObject*)));
        resetActiveSystrayInterface();
    }
}

void NetworkManagerApplet::updateWireless(bool checked)
{
    d->m_popup->updateWireless(checked);
}

void NetworkManagerApplet::activatableAdded(RemoteActivatable *activatable)
{
    RemoteInterfaceConnection *ic = qobject_cast<RemoteInterfaceConnection*>(activatable);
    if (activatable->activatableType() == Knm::Activatable::VpnInterfaceConnection)
    {
        connect(ic, SIGNAL(activationStateChanged(Knm::InterfaceConnection::ActivationState,Knm::InterfaceConnection::ActivationState)),
                this, SLOT(vpnActivationStateChanged(Knm::InterfaceConnection::ActivationState,Knm::InterfaceConnection::ActivationState)));
        Knm::InterfaceConnection::ActivationState state = ic->activationState();
        if (state != Knm::InterfaceConnection::Unknown)
        {
                QMetaObject::invokeMethod(ic, "activationStateChanged", Q_ARG(Knm::InterfaceConnection::ActivationState, ic->oldActivationState()), Q_ARG(Knm::InterfaceConnection::ActivationState, state));
        }
    }
    else if (ic)
    {
        connect(ic, SIGNAL(hasDefaultRouteChanged(bool)), SLOT(updateActiveInterface(bool)));
        QMetaObject::invokeMethod(ic, "hasDefaultRouteChanged", Q_ARG(bool, ic->hasDefaultRoute()));
    }
}

void NetworkManagerApplet::managerWirelessEnabledChanged(bool)
{
    setupInterfaceSignals();
}

void NetworkManagerApplet::managerWirelessHardwareEnabledChanged(bool enabled)
{
    Q_UNUSED( enabled );
    setupInterfaceSignals();
//    updatePixmap();
}

void NetworkManagerApplet::userNetworkingEnabledChanged(bool enabled)
{
    qDebug() << enabled;
    NetworkManager::setNetworkingEnabled(enabled);
    setupInterfaceSignals();
}

void NetworkManagerApplet::userWirelessEnabledChanged(bool enabled)
{
    qDebug() << enabled;
    NetworkManager::setWirelessEnabled(enabled);
    setupInterfaceSignals();
}

void NetworkManagerApplet::managerStatusChanged(NetworkManager::Status status)
{
    //qDebug() << "managerstatuschanged";
    updateInterfaceList();
    if (status == NetworkManager::Unknown) {
        setActiveInterface(NetworkManager::Device::Ptr());
        setActiveSystrayInterface(NetworkManager::Device::Ptr());
    } else {
        if (!m_interfaces.isEmpty()) {
            qSort(m_interfaces.begin(), m_interfaces.end(), networkInterfaceLessThan);
            setActiveInterface(m_interfaces.first());
            setActiveSystrayInterface(m_activeInterface);
        }
    }
    setupInterfaceSignals();
}

void NetworkManagerApplet::setupInterfaceSignals()
{
    foreach (const NetworkManager::Device::Ptr &interface, m_interfaces) {
        // be aware of state changes
        QObject::disconnect(interface.data(), SIGNAL(stateChanged(NetworkManager::Device::State,NetworkManager::Device::State,NetworkManager::Device::StateChangeReason)), this, SLOT(interfaceConnectionStateChanged()));

        connect(interface.data(), SIGNAL(stateChanged(NetworkManager::Device::State,NetworkManager::Device::State,NetworkManager::Device::StateChangeReason)), this, SLOT(interfaceConnectionStateChanged()));


        // Interface type-specific connections
        if (interface->type() == NetworkManager::Device::Ethernet)
        {
            NetworkManager::WiredDevice::Ptr wirediface =
                            interface.objectCast<NetworkManager::WiredDevice>();
            connect(wirediface.data(), SIGNAL(carrierChanged(bool)), this, SLOT(interfaceConnectionStateChanged()));
        } else if (interface->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wirelessiface =
                            interface.objectCast<NetworkManager::WirelessDevice>();
            connect(wirelessiface.data(), SIGNAL(activeAccessPointChanged(QString)), SLOT(setupAccessPointSignals(QString)));
            if (wirelessiface->activeAccessPoint())
            {
                QMetaObject::invokeMethod(wirelessiface.data(), "activeAccessPointChanged",
                                          Q_ARG(QString, wirelessiface->activeAccessPoint()->uni()));
            }
        } else if (interface->type() == NetworkManager::Device::Modem)
        {
            NetworkManager::ModemDevice::Ptr modemiface =
                            interface.objectCast<NetworkManager::ModemDevice>();

            ModemManager::ModemGsmNetworkInterface::Ptr modemNetworkIface;
            modemNetworkIface = modemiface->getModemNetworkIface().objectCast<ModemManager::ModemGsmNetworkInterface>();
            if (modemNetworkIface) {
                connect(modemNetworkIface.data(), SIGNAL(signalQualityChanged(uint)), this, SLOT(interfaceConnectionStateChanged()));
                connect(modemNetworkIface.data(), SIGNAL(accessTechnologyChanged(ModemManager::ModemInterface::AccessTechnology)), this, SLOT(interfaceConnectionStateChanged()));
            }
        }
    }
}

void NetworkManagerApplet::setupAccessPointSignals(const QString & uni)
{
    NetworkManager::WirelessDevice * wirelessiface = qobject_cast<NetworkManager::WirelessDevice *>(sender());
    NetworkManager::AccessPoint::Ptr ap = wirelessiface->findAccessPoint(uni);
    if (ap) {
        connect(ap.data(), SIGNAL(signalStrengthChanged(int)), SLOT(interfaceConnectionStateChanged()));
        connect(ap.data(), SIGNAL(destroyed(QObject*)), SLOT(interfaceConnectionStateChanged()));
    }
}

/* Slots to react to changes from the daemon */
void NetworkManagerApplet::deviceAdded(const QString & uni)
{
    Q_UNUSED(uni);
    // update the tray icon
    updateInterfaceList();

    if (!m_activeInterface) {
        if (m_interfaces.isEmpty()) {
            setActiveInterface(NetworkManager::Device::Ptr());
        } else {
            setActiveInterface(m_interfaces.first());
        }
        setActiveSystrayInterface(m_activeInterface);
    }

    setupInterfaceSignals();
    interfaceConnectionStateChanged();
}

void NetworkManagerApplet::deviceRemoved(const QString & uni)
{
    // update the tray icon
    updateInterfaceList();

    if (uni == m_lastActiveInterfaceUni) {
        if (m_interfaces.isEmpty()) {
            setActiveInterface(NetworkManager::Device::Ptr());
        } else {
            qSort(m_interfaces.begin(), m_interfaces.end(), networkInterfaceLessThan);
            setActiveInterface(m_interfaces.first());
            m_activeInterfaceState = NetworkManager::Device::UnknownState;
        }
    }
    setupInterfaceSignals();
    if (uni == m_lastActiveSystrayInterfaceUni) {
        setActiveSystrayInterface(NetworkManager::Device::Ptr());
        resetActiveSystrayInterface();
    } else {
        interfaceConnectionStateChanged();
    }
    // kill any animations involving this interface
}

void NetworkManagerApplet::interfaceConnectionStateChanged()
{
    //qDebug() << " +++ +++ +++ Connection State Changed +++ +++ +++";
    NetworkManager::Device *interface = qobject_cast<NetworkManager::Device *>(sender());
    if (interface) {
        if (m_activeSystrayInterface && m_activeSystrayInterface->uni() != interface->uni()) {
            NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(interface->uni());
            switch (interface->state()) {
            case NetworkManager::Device::Preparing:
            case NetworkManager::Device::ConfiguringHardware:
            case NetworkManager::Device::ConfiguringIp:
            case NetworkManager::Device::CheckingIp:
            case NetworkManager::Device::WaitingForSecondaries:
                setActiveSystrayInterface(device);
                m_activeSystrayInterfaceState = NetworkManager::Device::UnknownState;
                break;
            default:
                break;
            }
        }
    } else if (!m_activeSystrayInterface) {
        setActiveSystrayInterface(m_activeInterface);
        m_activeSystrayInterfaceState = m_activeInterfaceState;
    }
    if (m_activeSystrayInterface) {
        //qDebug() << "busy ... ?";
        NetworkManager::Device::State state = m_activeSystrayInterface->state();
        switch (state) {
            case NetworkManager::Device::Preparing:
            case NetworkManager::Device::ConfiguringHardware:
            case NetworkManager::Device::ConfiguringIp:
            case NetworkManager::Device::CheckingIp:
            case NetworkManager::Device::WaitingForSecondaries:
            case NetworkManager::Device::Deactivating:
                if (m_activeSystrayInterfaceState != state)
                {
                    // TODO Fixme: Set icon in system tray. Progress connection
                    //setStatusOverlay(generateProgressStatusOverlay());
                }
                //setBusy(true);
                break;
            case NetworkManager::Device::NeedAuth:
                //setBusy(false);
                break;
            case NetworkManager::Device::Activated:
                //setBusy(false);
                if (m_activeSystrayInterfaceState != state)
                {
                    // TODO Fixme: Set icon in system tray. Progress connection
                    // We want to show the full circle a bit
//                    setStatusOverlay(generateProgressStatusOverlay());
//                    setStatusOverlay("task-complete");
                    QTimer::singleShot(4000, this, SLOT(clearActivatedOverlay()));
                }
                if (m_activeSystrayInterface->uni() != m_activeInterface->uni()) {
                    QTimer::singleShot(5000, this, SLOT(resetActiveSystrayInterface()));
                }
                break;
            case NetworkManager::Device::UnknownState:
                qDebug() << "UnknownState! should this happen?";
            case NetworkManager::Device::Unmanaged:
            case NetworkManager::Device::Unavailable:
            case NetworkManager::Device::Failed:
                if (m_activeSystrayInterfaceState != state)
                {
                    // TODO Fixme: Set icon system tray
                    //setStatusOverlay("dialog-error");
                }
                QTimer::singleShot(4000, this, SLOT(resetActiveSystrayInterface()));
                break;
            case NetworkManager::Device::Disconnected:
                if (m_activeSystrayInterfaceState != state)
                {
                    // TODO Fixme: Set icon system tray
                    //setStatusOverlay("dialog-cancel");
                }
                QTimer::singleShot(2000, this, SLOT(resetActiveSystrayInterface()));
                break;
        }
        m_activeSystrayInterfaceState = state;
    }
    updatePixmap();
}

void NetworkManagerApplet::clearActivatedOverlay()
{
    if (m_activeInterface && static_cast<NetworkManager::Device::State>(m_activeInterface->state()) == NetworkManager::Device::Activated) {
        // Clear the overlay, but only if we are still activated

        //setStatusOverlay(QPixmap());
    }
}

/*
 * Update icon
 */
void NetworkManagerApplet::updatePixmap()
{
    QString iconName = UiUtils::iconName(m_activeSystrayInterface);
    if (iconName == m_currentPixmapIconName) {
        return;
    }

    //int s = UiUtils::iconSize(contentsRect().size());
    m_currentPixmapIconName = iconName;
//    m_pixmap = KIcon(m_currentPixmapIconName).pixmap(s, s);
//    update();
}

void NetworkManagerApplet::resetActiveSystrayInterface()
{
    if (m_activeInterface && m_activeSystrayInterface && m_activeSystrayInterface->uni() == m_activeInterface->uni()) {
        return;
    }
    setActiveSystrayInterface(m_activeInterface);
    m_activeSystrayInterfaceState = m_activeInterfaceState;
    if (m_activeSystrayInterfaceState == NetworkManager::Device::Activated)
    {
        // TODO Fixme: Set icon system tray
        //setStatusOverlay(QPixmap());
    }
    interfaceConnectionStateChanged();
}

void NetworkManagerApplet::activatableRemoved(RemoteActivatable *activatable)
{
    if (activatable->activatableType() == Knm::Activatable::VpnInterfaceConnection) {
        RemoteInterfaceConnection *ic = static_cast<RemoteInterfaceConnection*>(activatable);
        m_activeVpnConnections.remove(ic->connectionUuid());
        qDebug() << "activatable removed" << m_activeVpnConnections.count();
    }
    RemoteWirelessNetwork *rwic;
    RemoteWirelessInterfaceConnection *rwic2;
    rwic = qobject_cast<RemoteWirelessNetwork *>(activatable);
    if(rwic) {
        qDebug() << "removed network " + rwic->ssid();
    } else {
        rwic2 = qobject_cast<RemoteWirelessInterfaceConnection *>(activatable);
        if(rwic2) {
            qDebug() << "removed network " + rwic2->ssid();
        } else {
            qDebug() << "removed network any";
        }
    }
}

void NetworkManagerApplet::activatablesDisappeared()
{
    m_totalActiveVpnConnections = 0;
    // TODO Fixme: Update widget
    //update();
}

void NetworkManagerApplet::setActiveInterface(const NetworkManager::Device::Ptr &device)
{
    m_activeInterface = device;

    if (m_activeInterface) {
        m_lastActiveInterfaceUni = m_activeInterface->uni();
    }
}

void NetworkManagerApplet::setActiveSystrayInterface(const NetworkManager::Device::Ptr &device)
{
    m_activeSystrayInterface = device;

    if (m_activeSystrayInterface) {
        m_lastActiveSystrayInterfaceUni = m_activeSystrayInterface->uni();
    }
}

bool networkInterfaceLessThan(const NetworkManager::Device::Ptr &if1, const NetworkManager::Device::Ptr &if2)
{
    /*
     * status merging algorithm
     * In descending order of importance:
     * - Connecting devices
     *   - Cellular devices (because of cost)
     *   - = PPP devices
     *   - Ethernet devices
     *   - Wireless devices
     * - Connected devices
     *   - order as above
     * - Disconnected devices
     *   - order as above
     */
    enum {  Connecting,
            Connected,
            Disconnected,
            Unavailable }
        if2status = Unavailable,
        if1status = Unavailable;

    switch (if1->state()) {
        case NetworkManager::Device::Preparing:
        case NetworkManager::Device::ConfiguringHardware:
        case NetworkManager::Device::NeedAuth:
        case NetworkManager::Device::ConfiguringIp:
        case NetworkManager::Device::CheckingIp:
        case NetworkManager::Device::WaitingForSecondaries:
            if1status = Connecting;
            break;
        case NetworkManager::Device::Activated:
            if1status = Connected;
            break;
        case NetworkManager::Device::Disconnected:
            if1status = Disconnected;
            break;
        default: // all kind of unavailable
            break;
    }
    switch (if2->state()) {
        case NetworkManager::Device::Preparing:
        case NetworkManager::Device::ConfiguringHardware:
        case NetworkManager::Device::NeedAuth:
        case NetworkManager::Device::ConfiguringIp:
        case NetworkManager::Device::CheckingIp:
        case NetworkManager::Device::WaitingForSecondaries:
            if2status = Connecting;
            break;
        case NetworkManager::Device::Activated:
            if2status = Connected;
            break;
        case NetworkManager::Device::Disconnected:
            if2status = Disconnected;
            break;
        default: // all kind of disconnected
            break;
    }

    bool lessThan = false;
    switch (if1status) {
        case Connecting:
            lessThan = (if2status != Connecting || networkInterfaceSameConnectionStateLessThan(if1, if2));
            //return true;//
            break;
        case Connected:
            if ( if2status == Connecting) {
               return false;
            }
            lessThan = ((if2status != Connected) || networkInterfaceSameConnectionStateLessThan(if1, if2));
            break;
        case Disconnected:
            lessThan = false;
            if ( if2status == Disconnected) {
                lessThan = networkInterfaceSameConnectionStateLessThan(if1, if2);
            }
            if (if2status == Unavailable) {
                lessThan = true;
            }
            break;
        case Unavailable:
            lessThan = false;
            if ( if2status == Unavailable) {
                lessThan = networkInterfaceSameConnectionStateLessThan(if1, if2);
            }
            break;
    }

    return lessThan;
}

bool networkInterfaceSameConnectionStateLessThan(const NetworkManager::Device::Ptr &if1, const NetworkManager::Device::Ptr &if2)
{
    bool lessThan = false;
    switch (if1->type() ) {
        case NetworkManager::Device::Ethernet:
            switch (if2->type()) {
                case NetworkManager::Device::Ethernet:
                    lessThan = if1->uni() < if2->uni();
                    break;
                case NetworkManager::Device::Wifi:
                    lessThan = true;
                    break;
                case NetworkManager::Device::Modem:
                default:
                    lessThan = false;
                    break;
            }
            break;
        case NetworkManager::Device::Wifi:
            switch (if2->type()) {
                case NetworkManager::Device::Ethernet:
                    lessThan = false;
                    break;
                case NetworkManager::Device::Wifi:
                    lessThan = if1->uni() < if2->uni();
                    break;
                case NetworkManager::Device::Modem:
                    lessThan = false;
                    break;
                default:
                    lessThan = true;
                    break;
            }
            break;
        case NetworkManager::Device::Modem:
            switch (if2->type()) {
                case NetworkManager::Device::Ethernet:
                case NetworkManager::Device::Wifi:
                    lessThan = true;
                    break;
                case NetworkManager::Device::Modem:
                    lessThan = if1->uni() < if2->uni();
                    break;
                default:
                    lessThan = true;
                    break;
            }
            break;
        default:
            lessThan = false;
        }
    return lessThan;
}

