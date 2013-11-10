#ifndef WIDGETINFOCONNECTION_H
#define WIDGETINFOCONNECTION_H

#include <QtGui/QGroupBox>
#include <NetworkManagerQt/Device>

class QVBoxLayout;

class WidgetInfoConnection : public QGroupBox
{
    Q_OBJECT
public:
    explicit WidgetInfoConnection(NetworkManager::Device::Ptr device, QWidget *parent = 0);

signals:

public slots:
private:
    QVBoxLayout *verticalLayout;
};

QString typeAsString(const int type);
QString getHardwareAddress(QString uni, NetworkManager::Device::Type type);

#endif // WIDGETINFOCONNECTION_H
