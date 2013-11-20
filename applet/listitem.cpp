#include "listitem.h"
#include "ui_listitem.h"

ListItem::ListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::setConnectionName(const QString &name)
{
    ui->connectionName->setText(name);
}

void ListItem::setSignalStrength(int signal)
{
    ui->signalStrength->setValue(signal);
}

void ListItem::setActivationState(const QString &state)
{
    ui->QIconItem->setText(state);
}

void ListItem::setConnectionIcon(const QString &icon)
{
    ui->QIconItem->setText(icon);
}
