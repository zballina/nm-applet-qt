#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/BluetoothDevice>

#include "widgetinfoconnection.h"

WidgetInfoConnection::WidgetInfoConnection(NetworkManager::Device::Ptr device, QWidget *parent) :
    QGroupBox(parent), verticalLayout(new QVBoxLayout(this))
{
    setTitle(device->activeConnection()->connection()->name());

    // Information general of device
    QLabel *labelHeaderGeneral = new QLabel(this);
    labelHeaderGeneral->setText(tr("General"));
    QVBoxLayout *layoutGeneral = new QVBoxLayout(this);

    // Information IP Version 4
    QLabel *labelHeaderIPV4Config = new QLabel(this);
    labelHeaderIPV4Config->setText(tr("IPv4"));
    QVBoxLayout *layoutIPV4Config = new QVBoxLayout(this);

    NetworkManager::IpConfig ipConfig = device->ipV4Config();

    if(ipConfig.isValid())
    {
        // General
        QHBoxLayout *layoutRowType = new QHBoxLayout(this);
        QLabel *labelTagType = new QLabel(this);
        labelTagType->setText(tr("Type Device: "));
        QLabel *labelType = new QLabel(this);
        labelType->setText(typeAsString(device->type()) + QString(" (%1)").replace("%1", device->interfaceName()));
        layoutRowType->addWidget(labelTagType);
        layoutRowType->addWidget(labelType);

        QHBoxLayout *layoutRowDriver = new QHBoxLayout(this);
        QLabel *labelTagDriver = new QLabel(this);
        labelTagDriver->setText(tr("Driver: "));
        QLabel *labelDriver = new QLabel(this);
        labelDriver->setText(device->driver());
        layoutRowDriver->addWidget(labelTagDriver);
        layoutRowDriver->addWidget(labelDriver);

        QHBoxLayout *layoutRowMAC = new QHBoxLayout(this);
        QLabel *labelTagMAC = new QLabel(this);
        labelTagMAC->setText(tr("Driver: "));
        QLabel *labelMAC = new QLabel(this);
        labelMAC->setText(getHardwareAddress(device->uni(), device->type()));
        layoutRowMAC->addWidget(labelTagMAC);
        layoutRowMAC->addWidget(labelMAC);

        layoutGeneral->addLayout(layoutRowType);
        layoutGeneral->addLayout(layoutRowDriver);
        layoutGeneral->addLayout(layoutRowMAC);

        // IPv4
        QHBoxLayout *layoutRowIP = new QHBoxLayout(this);
        QLabel *labelTagIP = new QLabel(this);
        labelTagIP->setText(tr("IP Address: "));
        QLabel *labelIP = new QLabel(this);
        labelIP->setText(ipConfig.addresses().at(0).ip().toString());

        layoutRowIP->addWidget(labelTagIP);
        layoutRowIP->addWidget(labelIP);

        layoutIPV4Config->addLayout(layoutRowIP);
    }
    verticalLayout->addWidget(labelHeaderGeneral);
    verticalLayout->addLayout(layoutGeneral);
    verticalLayout->addWidget(labelHeaderIPV4Config);
    verticalLayout->addLayout(layoutIPV4Config);
}

QString typeAsString(const int type)
{
    switch (type) {
    case 0x0: return QString("Unknown");
    case 0x1: return QString("Ethernet");
    case 0x2: return QString("Wifi");
    case 0x3: return QString("Unused1");
    case 0x4: return QString("Unused2");
    case 0x5: return QString("Bluetooth");
    case 0x6: return QString("OlpcMesh");
    case 0x7: return QString("Wimax");
    case 0x8: return QString("Modem");
    }
    return QString("Unknown");
}

QString getHardwareAddress(QString uni, NetworkManager::Device::Type type)
{
    switch (type)
    {
        case NetworkManager::Device::Ethernet:
            return NetworkManager::WiredDevice(uni).hardwareAddress();
        case NetworkManager::Device::Wifi:
            return NetworkManager::WirelessDevice(uni).hardwareAddress();
        case NetworkManager::Device::Bluetooth:
            return NetworkManager::BluetoothDevice(uni).hardwareAddress();
        case NetworkManager::Device::UnknownType:
        case NetworkManager::Device::Unused1:
        case NetworkManager::Device::Unused2:
        case NetworkManager::Device::OlpcMesh:
        case NetworkManager::Device::Wimax:
        case NetworkManager::Device::Modem:
        case NetworkManager::Device::InfiniBand:
        case NetworkManager::Device::Bond:
        case NetworkManager::Device::Vlan:
        case NetworkManager::Device::Adsl:
        case NetworkManager::Device::Bridge:
        break;
    }
    return "";
}
