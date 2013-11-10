#include <QtCore/QDebug>
#include <QtGui/QLayout>
#include <NetworkManagerQt/Manager>

#include "widgetinfoconnection.h"
#include "infoconnection.h"
#include "ui_infoconnection.h"

InfoConnection::InfoConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoConnection)
{
    ui->setupUi(this);
    for(int i = 0; i < this->ui->verticalLayoutItems->count(); i++)
    {
        QLayoutItem *o = this->ui->verticalLayoutItems->itemAt(i);
        this->ui->verticalLayoutItems->removeItem(o);
    }

    NetworkManager::Device::List devices = NetworkManager::networkInterfaces();

    foreach (NetworkManager::Device::Ptr ptr, devices)
    {
        if(ptr->isValid() && ptr->isActive())
        {
            WidgetInfoConnection *info = new WidgetInfoConnection(ptr);
            this->ui->verticalLayoutItems->addWidget(info);
        }
    }
}

InfoConnection::~InfoConnection()
{
    delete ui;
}
