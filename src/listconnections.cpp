#include "listconnections.h"
#include "ui_listconnections.h"

ListConnections::ListConnections(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListConnections)
{
    ui->setupUi(this);
}

ListConnections::~ListConnections()
{
    delete ui;
}
