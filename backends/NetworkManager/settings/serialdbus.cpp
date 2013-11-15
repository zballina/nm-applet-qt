// This file is generated by kconfig_compiler from serial.kcfg.
// All changes you do to this file will be lost.

#include "serialdbus.h"

#include "serial.h"

SerialDbus::SerialDbus(Knm::SerialSetting *setting) : SettingDbus(setting)
{
}

SerialDbus::~SerialDbus()
{
}

void SerialDbus::fromMap(const QVariantMap &map)
{
    Knm::SerialSetting *setting = static_cast<Knm::SerialSetting *>(m_setting);
    if (map.contains(QLatin1String(NM_SETTING_SERIAL_BAUD)))
    {
        setting->setBaud(map.value(QLatin1String(NM_SETTING_SERIAL_BAUD)).value<uint>());
    }
    if (map.contains(QLatin1String(NM_SETTING_SERIAL_BITS)))
    {
        setting->setBits(map.value(QLatin1String(NM_SETTING_SERIAL_BITS)).value<uint>());
    }
    if (map.contains(QLatin1String(NM_SETTING_SERIAL_PARITY)))
    {
        QString value = map.value(QLatin1String(NM_SETTING_SERIAL_PARITY)).value<QString>();
        if (value == "n")
            setting->setParity(Knm::SerialSetting::EnumParity::None);
        else
            if (value == "E")
                setting->setParity(Knm::SerialSetting::EnumParity::Even);
            else
                if (value == "o")
                    setting->setParity(Knm::SerialSetting::EnumParity::Odd);
    }
    if (map.contains(QLatin1String(NM_SETTING_SERIAL_STOPBITS)))
    {
        setting->setStopbits(map.value(QLatin1String(NM_SETTING_SERIAL_STOPBITS)).value<uint>());
    }
    if (map.contains(QLatin1String(NM_SETTING_SERIAL_SEND_DELAY)))
    {
        setting->setSenddelay(map.value(QLatin1String(NM_SETTING_SERIAL_SEND_DELAY)).value<quint64>());
    }
}

QVariantMap SerialDbus::toMap()
{
    QVariantMap map;
    Knm::SerialSetting *setting = static_cast<Knm::SerialSetting *>(m_setting);
    insertIfNonZero(map, NM_SETTING_SERIAL_BAUD, setting->baud());
    insertIfNonZero(map, NM_SETTING_SERIAL_BITS, setting->bits());
    switch (setting->parity())
    {
        case Knm::SerialSetting::EnumParity::None:
            map.insert(QLatin1String(NM_SETTING_SERIAL_PARITY), "n");
            break;
        case Knm::SerialSetting::EnumParity::Even:
            map.insert(QLatin1String(NM_SETTING_SERIAL_PARITY), "E");
            break;
        case Knm::SerialSetting::EnumParity::Odd:
            map.insert(QLatin1String(NM_SETTING_SERIAL_PARITY), "o");
            break;
    }
    insertIfNonZero(map, NM_SETTING_SERIAL_STOPBITS, setting->stopbits());
    insertIfNonZero(map, NM_SETTING_SERIAL_SEND_DELAY, setting->senddelay());
    return map;
}

QVariantMap SerialDbus::toSecretsMap()
{
    QVariantMap map;
    return map;
}

