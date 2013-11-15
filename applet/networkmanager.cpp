#include "networkmanager.h"
#include "ui_networkmanagerapplet.h"

NetworkManagerApplet::NetworkManagerApplet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkManagerApplet)
{
    ui->setupUi(this);
}

NetworkManagerApplet::~NetworkManagerApplet()
{
    delete ui;
}
