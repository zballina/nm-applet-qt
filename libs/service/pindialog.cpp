/*
Copyright 2011 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pindialog.h"

#include <QIntValidator>
#include <QLineEdit>

#include "ui_pinwidget.h"

#include <QDebug>
#include "environment.h"
//#include <kwindowsystem.h>
#include <ModemManagerQt/manager.h>

PinDialog::PinDialog(ModemManager::ModemGsmCardInterface *modem, const Type type, QWidget *parent)
    : QDialog(parent), m_type(type)
{
    if (modem) {
        m_udi = modem->udi();
        m_name = modem->masterDevice();
/*        foreach (const Solid::Device &d, Solid::Device::allDevices()) {
            if (d.udi().contains(m_name, Qt::CaseInsensitive)) {
                m_name = d.product();
                if (!m_name.startsWith(d.vendor())) {
                    m_name = d.vendor() + ' ' + m_name;
                }
                break;
            }
        }
*/
    }

    QWidget *w = new QWidget();
    ui = new Ui::PinWidget();
    ui->setupUi(w);
    ui->pin->setEchoMode(QLineEdit::Password);

    QIntValidator * validator = new QIntValidator(this);
    validator->setRange(1000, 99999999);
    ui->pin->setValidator(validator);
    ui->pin2->setValidator(validator);

    QIntValidator * validator2 = new QIntValidator(this);
    validator2->setRange(10000000, 99999999);
    ui->puk->setValidator(validator2);

    ui->errorMessage->setHidden(true);
//    QRect desktop = KGlobalSettings::desktopGeometry(topLevelWidget());
//    setMinimumWidth(qMin(1000, qMax(sizeHint().width(), desktop.width() / 4)));

//    pixmapLabel = new QLabel( mainWidget() );
//    pixmapLabel->setAlignment( Qt::AlignLeft | Qt::AlignTop );
//    ui->gridLayout->addWidget( pixmapLabel, 0, 0 );
//    pixmapLabel->setPixmap( QIcon("dialog-password"));

//    button(KDialog::Ok)->setText(trc("As in 'Unlock cell phone with this pin code'", "Unlock"));
//    setMainWidget(w);

    if (m_type == PinPuk) {
        setWindowTitle(tr("SIM PUK unlock required"));
        ui->title->setText(tr("SIM PUK Unlock Required"));
        ui->prompt->setText(tr("The mobile broadband device '%1' requires a SIM PUK code before it can be used.").arg(m_name));
        ui->pukLabel->setText(tr("PUK code:"));
        ui->pinLabel->setText(tr("New PIN code:"));
        ui->pin2Label->setText(tr("Re-enter new PIN code:"));
        ui->chkShowPass->setText(tr("Show PIN/PUK code"));

        ui->puk->setFocus();
        ui->pukLabel->show();
        ui->puk->show();
        ui->pin2Label->show();
        ui->pin2->show();
    } else {
        setWindowTitle(tr("SIM PIN unlock required"));
        ui->title->setText(tr("SIM PIN Unlock Required"));
        ui->prompt->setText(tr("The mobile broadband device '%1' requires a SIM PIN code before it can be used.").arg(m_name));
        ui->pinLabel->setText(tr("PIN code:"));
        ui->chkShowPass->setText(tr("Show PIN code"));

        ui->pin->setFocus();
        ui->pukLabel->hide();
        ui->puk->hide();
        ui->pin2Label->hide();
        ui->pin2->hide();
    }

    ui->puk->setText("");
    ui->pin->setText("");
    ui->pin2->setText("");
    ui->puk->setCursorPosition(0);
    ui->pin->setCursorPosition(0);
    ui->pin2->setCursorPosition(0);

//    KWindowSystem::setState( winId(), NET::KeepAbove );
//    KWindowSystem::setOnAllDesktops( winId(), true );
//    KWindowSystem::activateWindow( winId());

//    move((desktop.width() - width()) / 2, (desktop.height() - height()) / 2);
    connect(ui->chkShowPass, SIGNAL(stateChanged(int)), this, SLOT(chkShowPassToggled()));
    connect(ModemManager::notifier(), SIGNAL(modemRemoved(QString)), SLOT(modemRemoved(QString)));
}

PinDialog::~PinDialog()
{
    delete ui;
}

void PinDialog::chkShowPassToggled()
{
    bool on = ui->chkShowPass->isChecked();
    ui->pin->setEchoMode(on ? QLineEdit::Normal : QLineEdit::Password);
    ui->pin2->setEchoMode(on ? QLineEdit::Normal : QLineEdit::Password);
    ui->puk->setEchoMode(on ? QLineEdit::Normal : QLineEdit::Password);

    ui->puk->setCursorPosition(0);
    ui->pin->setCursorPosition(0);
    ui->pin2->setCursorPosition(0);

    if (m_type == PinPuk) {
        ui->puk->setFocus();
    } else {
        ui->pin->setFocus();
    }
}

void PinDialog::modemRemoved(const QString &udi)
{
    if (udi == m_udi) {
        reject();
    }
}

PinDialog::Type PinDialog::type() const
{
    return m_type;
}

QString PinDialog::pin() const
{
    return ui->pin->text();
}

QString PinDialog::pin2() const
{
    return ui->pin2->text();
}

QString PinDialog::puk() const
{
    return ui->puk->text();
}

void PinDialog::showErrorMessage(const PinDialog::ErrorCode error)
{
    QString msg;
    QFont bold = font();
    ui->pinLabel->setFont(bold);
    ui->pin2Label->setFont(bold);
    ui->pukLabel->setFont(bold);
    bold.setBold( true );
    switch(error) {
        case PinCodeTooShort: msg = tr("PIN code too short. It should be at least 4 digits.");
                              ui->pin->setFocus();
                              ui->pinLabel->setFont(bold);
        break;
        case PinCodesDoNotMatch: msg = tr("The two PIN codes do not match");
                              ui->pin2->setFocus();
                              ui->pin2Label->setFont(bold);
        break;
        case PukCodeTooShort: msg = tr("PUK code too short. It should be 8 digits.");
                              ui->puk->setFocus();
                              ui->pukLabel->setFont(bold);
        break;
        default: msg = tr("Unknown Error");
    }
    ui->errorMessage->setText(msg);
    adjustSize();
}


void PinDialog::accept()
{
    if (m_type == PinPuk) {
        if (pin() != pin2()) {
            showErrorMessage(PinCodesDoNotMatch);
            return;
        } else if (puk().length() < 8) {
            showErrorMessage(PukCodeTooShort);
            return;
        }
    }

    if (pin().length() < 4) {
        showErrorMessage(PinCodeTooShort);
        return;
    }

    QDialog::accept();
}
