#include "nmdialog.h"
#include "ui_nmdialog.h"

#define ENABLE_WIRELESS     "Enable Wireless"
#define ENABLE_MOBILE       "Enable Mobile"
#define SETTINGS            "Settings"
#define NO_DEVICE_SELECTED  "No device selected"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/ModemDevice>

NMDialog::NMDialog(RemoteActivatableList * activatableList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NMDialog),
    m_activatables(activatableList)
{
    ui->setupUi(this);

    listModel = new ConnectionsListModel(m_activatables, parent);
    interfaceListModel = new InterfacesListModel(parent);

    connect(listModel, SIGNAL(showInterfaceDetails(QString)), SLOT(showInterfaceDetails(QString)));
    connect(interfaceListModel, SIGNAL(updateTraffic(DeclarativeInterfaceItem*)), this, SLOT(manageUpdateTraffic(DeclarativeInterfaceItem*)));
    connect(this, SIGNAL(finished()), this, SLOT(createActions()));
    connect(NetworkManager::notifier(), SIGNAL(wirelessEnabledChanged(bool)),
            this, SLOT(managerWirelessEnabledChanged(bool)));
    connect(NetworkManager::notifier(), SIGNAL(wirelessHardwareEnabledChanged(bool)),
            this, SLOT(managerWirelessHardwareEnabledChanged(bool)));
    connect(NetworkManager::notifier(), SIGNAL(wwanEnabledChanged(bool)),
            this, SLOT(managerWwanEnabledChanged(bool)));
    connect(NetworkManager::notifier(), SIGNAL(wwanHardwareEnabledChanged(bool)),
            this, SLOT(managerWwanHardwareEnabledChanged(bool)));

    foreach(const NetworkManager::Device::Ptr &iface, NetworkManager::networkInterfaces()) {
        addInterfaceInternal(iface);
        qDebug() << "Network Interface:" << iface->interfaceName() << iface->driver() << iface->designSpeed();
    }

    connect(NetworkManager::notifier(), SIGNAL(deviceAdded(QString)),
            SLOT(interfaceAdded(QString)));
    connect(NetworkManager::notifier(), SIGNAL(deviceRemoved(QString)),
            SLOT(interfaceRemoved(QString)));
    connect(NetworkManager::notifier(), SIGNAL(networkingEnabledChanged(bool)),
            SLOT(managerNetworkingEnabledChanged(bool)));

    ui->listView->setModel(listModel);
//    QDBusConnection dbus = QDBusConnection::sessionBus();
//    dbus.connect("org.kde.Solid.PowerManagement", "/org/kde/Solid/PowerManagement", "org.kde.Solid.PowerManagement", "resumingFromSuspend", this, SLOT(readConfig()));
//    dbus.connect("org.kde.kded", "/org/kde/networkmanagement", "org.kde.networkmanagement", "ReloadConfig", this, SLOT(readConfig()));
}

NMDialog::~NMDialog()
{
    delete ui;
}

void NMDialog::createActions()
{
    QAction *enableWireless = new QAction(ENABLE_WIRELESS, this);
    connect(enableWireless, SIGNAL(toggled(bool)), this, SLOT(updateWireless(bool)));
    m_actions[ENABLE_WIRELESS] = enableWireless;

    QAction *enableMobile = new QAction(ENABLE_MOBILE, this);
    connect(enableMobile, SIGNAL(toggled(bool)), this, SLOT(updateMobile(bool)));
    m_actions[ENABLE_MOBILE] = enableMobile;

    QAction *settings = new QAction(SETTINGS, this);
    connect(settings, SIGNAL(triggered()), this, SLOT(manageConnections()));
    m_actions[SETTINGS] = settings;

    QAction *noDeviceSelected = new QAction(NO_DEVICE_SELECTED, this);
    connect(noDeviceSelected, SIGNAL(triggered()), this, SLOT(manageSelection()));
    m_actions[NO_DEVICE_SELECTED] = noDeviceSelected;

    readConfig();
    addVpnInterface();
}

void NMDialog::addVpnInterface()
{
    m_vpnItem = new DeclarativeInterfaceItem(NetworkManager::Device::Ptr(), m_activatables, DeclarativeInterfaceItem::InterfaceName, this);
    interfaceListModel->appendRow(m_vpnItem);
}

void NMDialog::readConfig()
{
    QSettings config;
    config.beginGroup(QLatin1String("General"));

    if (config.value(QLatin1String("DetailKeys"), QStringList()).toStringList().isEmpty())
    {
        QStringList keys;
        keys << "interface:type"
             << "interface:status"
             << "ipv4:address"
             << "interface:bitrate"
             << "interface:name"
             << "interface:hardwareaddress"
             << "interface:driver"
             << "wireless:ssid"
             << "wireless:accesspoint"
             << "wireless:band"
             << "wireless:channel"
             << "mobile:operator"
             << "mobile:quality"
             << "mobile:technology";

        config.setValue(QLatin1String("DetailKeys"), keys);
        config.sync();
    }

    QAction *enableWireless = m_actions[ENABLE_WIRELESS];
    enableWireless->setChecked(NetworkManager::isWirelessEnabled());
    enableWireless->setEnabled(NetworkManager::isWirelessHardwareEnabled());

    QAction *enableMobile = m_actions[ENABLE_MOBILE];
    enableMobile->setChecked(NetworkManager::isWwanEnabled());
    enableMobile->setEnabled(NetworkManager::isWwanHardwareEnabled());

    foreach(DeclarativeInterfaceItem * i, m_interfaces) {
        i->setNameDisplayMode(DeclarativeInterfaceItem::InterfaceName);
    }

    QString version = NetworkManager::version();
    if (version.isEmpty())
    {
        QMessageBox::warning(this, QApplication::applicationName(), tr("Warning about wrong NetworkManager version", "NetworkManager is not running. Please start it."));
    }
    else if (NetworkManager::compareVersion(QString(MINIMUM_NM_VERSION_REQUIRED)) < 0)
    {
        QMessageBox::warning(this, QApplication::applicationName(), tr("Warning about wrong NetworkManager version", "We need at least NetworkManager-%1 to work properly, found '%2'. Please upgrade to a newer version.").arg(QString(MINIMUM_NM_VERSION_REQUIRED), version));
    }
}

void NMDialog::managerWwanEnabledChanged(bool enabled)
{
    qDebug() << "NM daemon changed wwan enable state" << enabled;
    m_actions[ENABLE_MOBILE]->setChecked(enabled ? Qt::Checked : Qt::Unchecked);
    if (enabled)
    {
        m_actions[ENABLE_MOBILE]->setEnabled(enabled);
    }
}

void NMDialog::addInterfaceInternal(const NetworkManager::Device::Ptr &iface)
{
    if (!iface) {
        // the interface might be gone in the meantime...
        return;
    }

    if (!m_interfaces.contains(iface->uni())) {
        DeclarativeInterfaceItem * ifaceItem = 0;
        if (iface->type() == NetworkManager::Device::Wifi) {
            // Create the wireless interface item
            ifaceItem = new DeclarativeInterfaceItem(iface, m_activatables, DeclarativeInterfaceItem::InterfaceName, this);
            qDebug() << "WiFi added";
        } else {
            // Create the interfaceitem
            qDebug() << "Interface Item added";
            ifaceItem = new DeclarativeInterfaceItem(iface, m_activatables, DeclarativeInterfaceItem::InterfaceName, this);
        }
        m_interfaces.insert(iface->uni(), ifaceItem);
        interfaceListModel->insertRow(interfaceListModel->rowCount()-1, ifaceItem);
    }
    updateHasWireless();
    updateHasWwan();
}

void NMDialog::updateWireless(bool checked)
{
    if (NetworkManager::isWirelessEnabled() != checked) {
        NetworkManager::setWirelessEnabled(checked);
    }
}

void NMDialog::updateMobile(bool checked)
{
    if (NetworkManager::isWwanEnabled() != checked) {
        NetworkManager::setWwanEnabled(checked);
    }
}

void NMDialog::updateHasWireless(bool checked)
{
    //kDebug() << "UPDATE!!!!!!!!!!!!";
    bool hasWireless = true;
    if (!NetworkManager::isWirelessHardwareEnabled() ||
            !NetworkManager::isNetworkingEnabled() ||
            !NetworkManager::isWirelessEnabled() || !checked) {

        //kDebug () << "networking enabled?" << NetworkManager::isNetworkingEnabled();
        //kDebug () << "wireless hardware enabled?" << NetworkManager::isWirelessHardwareEnabled();
        //kDebug () << "wireless enabled?" << NetworkManager::isWirelessEnabled();

        // either networking is disabled, or wireless is disabled
        hasWireless = false;
    }
    //solid is too slow, we need to see if the checkbox was checked by the user
    if (checked)
        hasWireless = true;

    m_hasWirelessInterface = hasWireless;

    if (!m_hasWirelessInterface) {
        hasWireless = false;
    }

    // TODO Fixme:
    //m_rootContext->setContextPrope|rty("wirelessVisible", hasWireless);
    listModel->setHasWireless(hasWireless);
}

void NMDialog::updateHasWwan()
{
    bool hasWwan = false;
    foreach(DeclarativeInterfaceItem * ifaceitem, m_interfaces) {
        NetworkManager::ModemDevice::Ptr iface = ifaceitem->interface().objectCast<NetworkManager::ModemDevice>();
        if (iface) {
            hasWwan = true;
            break;
        }
    }

    // TODO Fixme:
    //m_rootContext->setContextProperty("mobileVisible", hasWwan);
}

void NMDialog::manageConnections()
{
    //kDebug() << "opening connection management dialog";
    QStringList args;
    args << "--icon" << "networkmanager" << "kcm_networkmanagement" << "kcm_networkmanagement_tray";
    //KToolInvocation::kdeinitExec("kcmshell4", args);
}

void NMDialog::showInterfaceDetails(const QString & uni)
{
    DeclarativeInterfaceItem * ifaceItem = m_interfaces.value(uni, 0);
    if (!ifaceItem) {
        return;
    }
    manageUpdateTraffic(ifaceItem);
}

void NMDialog::manageUpdateTraffic(DeclarativeInterfaceItem *device)
{
    qDebug() << "handle traffic plotter changes";
//    m_interfaceDetails->setInterface(device->interface());
//    m_interfaceDetails->setUpdateEnabled(true);
//    QMetaObject::invokeMethod(rootObject(), "showDetailsWidget");
    listModel->setDeviceToFilter(device->interface(), device->type() == "vpn");
}

void NMDialog::manageSelection()
{
    listModel->setDeviceToFilter();
}

void NMDialog::managerNetworkingEnabledChanged(bool enabled)
{
    qDebug() << "NM daemon changed networking enable state" << enabled;
    networkingEnabledToggled(enabled);
}

void NMDialog::networkingEnabledToggled(bool checked)
{
//    if (checked)
//        m_rootContext->setContextProperty("warningLabel", QVariant(QString()));

    // Switch networking on / off
    if (NetworkManager::isNetworkingEnabled() != checked) {
        NetworkManager::setNetworkingEnabled(checked);
    }
}

void NMDialog::managerWirelessEnabledChanged(bool enabled)
{
    qDebug() << "NM daemon changed wireless enable state" << enabled;
    // it might have changed because we toggled the switch,
    // but it might have been changed externally, so set it anyway
//    m_rootContext->setContextProperty("wirelessChecked", enabled);
    if (enabled) {
//        m_rootContext->setContextProperty("wirelessEnabled", enabled);
    } else {
        listModel->removeHiddenItem();
    }
}

void NMDialog::managerWirelessHardwareEnabledChanged(bool enabled)
{
    qDebug() << "Hardware wireless enable switch state changed" << enabled;
    //m_rootContext->setContextProperty("wirelessEnabled", enabled);
}

void NMDialog::managerWwanHardwareEnabledChanged(bool enabled)
{
    qDebug() << "Hardware wwan enable switch state changed" << enabled;
    //m_rootContext->setContextProperty("mobileVisible", enabled);
}

void NMDialog::interfaceRemoved(const QString& uni)
{
    if (m_interfaces.contains(uni))
    {
        // To prevent crashes when the interface removed is the one in interfaceDetailsWidget.
        // the m_iface pointer in interfaceDetailsWidget become invalid in this case.
//        if (m_interfaceDetails && uni == m_interfaceDetails->getLastIfaceUni())
//        {
//            m_interfaceDetails->setInterface(NetworkManager::Device::Ptr(), false);
//        }
        DeclarativeInterfaceItem* item = m_interfaces.take(uni);
        interfaceListModel->removeItem(item);
        updateHasWireless();
        updateHasWwan();
    }
}

void NMDialog::interfaceAdded(const QString& uni)
{
    if (m_interfaces.contains(uni)) {
        return;
    }
    NetworkManager::Device::Ptr iface = NetworkManager::findNetworkInterface(uni);
    if (iface) {
        qDebug() << "Interface Added:" << iface->interfaceName() << iface->driver() << iface->designSpeed();
        addInterfaceInternal(iface);
    }
}
