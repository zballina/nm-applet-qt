// This file is generated by kconfig_compiler from 802-11-wireless.kcfg.
// All changes you do to this file will be lost.

#include "802-11-wirelessdbus.h"

#include "802-11-wireless.h"

WirelessDbus::WirelessDbus(Knm::WirelessSetting *setting) : SettingDbus(setting)
{
}

WirelessDbus::~WirelessDbus()
{
}

void WirelessDbus::fromMap(const QVariantMap &map)
{
    Knm::WirelessSetting *setting = static_cast<Knm::WirelessSetting *>(m_setting);
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_SSID)))
    {
        setting->setSsid(map.value(QLatin1String(NM_SETTING_WIRELESS_SSID)).value<QByteArray>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_MODE)))
    {
        QString mode = map.value(QLatin1String(NM_SETTING_WIRELESS_MODE)).value<QString>();
        if (mode == QLatin1String(NM_SETTING_WIRELESS_MODE_ADHOC))
            setting->setMode(Knm::WirelessSetting::EnumMode::adhoc);
        else
            if (mode == QLatin1String(NM_SETTING_WIRELESS_MODE_INFRA))
                setting->setMode(Knm::WirelessSetting::EnumMode::infrastructure);
#if NM_CHECK_VERSION(0, 9, 7)
            else
                if (mode == QLatin1String(NM_SETTING_WIRELESS_MODE_AP))
                    setting->setMode(Knm::WirelessSetting::EnumMode::apMode);
#endif
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_BAND)))
    {
        QString band = map.value(QLatin1String(NM_SETTING_WIRELESS_BAND)).value<QString>();
        if (band == QLatin1String("a"))
            setting->setBand(Knm::WirelessSetting::EnumBand::a);
        else
            if (band == QLatin1String("bg"))
                setting->setBand(Knm::WirelessSetting::EnumBand::bg);
        setting->setChannel(map.value(QLatin1String(NM_SETTING_WIRELESS_CHANNEL)).value<uint>());
    }
    else
    {
        setting->setBand(Knm::WirelessSetting::EnumBand::automatic);
    }

    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_BSSID)))
    {
        setting->setBssid(map.value(QLatin1String(NM_SETTING_WIRELESS_BSSID)).value<QByteArray>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_RATE)))
    {
        setting->setRate(map.value(QLatin1String(NM_SETTING_WIRELESS_RATE)).value<uint>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_TX_POWER)))
    {
        setting->setTxpower(map.value(QLatin1String(NM_SETTING_WIRELESS_TX_POWER)).value<uint>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_MAC_ADDRESS)))
    {
        setting->setMacaddress(map.value(QLatin1String(NM_SETTING_WIRELESS_MAC_ADDRESS)).value<QByteArray>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_CLONED_MAC_ADDRESS)))
    {
        setting->setClonedmacaddress(map.value(QLatin1String(NM_SETTING_WIRELESS_CLONED_MAC_ADDRESS)).value<QByteArray>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_MTU)))
    {
        setting->setMtu(map.value(QLatin1String(NM_SETTING_WIRELESS_MTU)).value<uint>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_SEEN_BSSIDS)))
    {
        setting->setSeenbssids(map.value(QLatin1String(NM_SETTING_WIRELESS_SEEN_BSSIDS)).value<QStringList>());
    }
    if (map.contains(QLatin1String(NM_SETTING_WIRELESS_SEC)))
    {
        setting->setSecurity(map.value(QLatin1String(NM_SETTING_WIRELESS_SEC)).value<QString>());
    }
    else
    {
        setting->setSecurity("");
    }
}

QVariantMap WirelessDbus::toMap()
{
    QVariantMap map;
    Knm::WirelessSetting *setting = static_cast<Knm::WirelessSetting *>(m_setting);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SSID), setting->ssid());
    switch (setting->mode())
    {
        case Knm::WirelessSetting::EnumMode::infrastructure:
            map.insert(QLatin1String(NM_SETTING_WIRELESS_MODE), QLatin1String(NM_SETTING_WIRELESS_MODE_INFRA));
            break;
        case Knm::WirelessSetting::EnumMode::adhoc:
            map.insert(QLatin1String(NM_SETTING_WIRELESS_MODE), QLatin1String(NM_SETTING_WIRELESS_MODE_ADHOC));
#if NM_CHECK_VERSION(0, 9, 7)
        case Knm::WirelessSetting::EnumMode::apMode:
            if (setting->mode() == Knm::WirelessSetting::EnumMode::apMode)
            {
                map.insert(QLatin1String(NM_SETTING_WIRELESS_MODE), QLatin1String(NM_SETTING_WIRELESS_MODE_AP));
            }
#endif
            if (setting->band() != Knm::WirelessSetting::EnumBand::automatic)
            {
                switch (setting->band())
                {
                    case Knm::WirelessSetting::EnumBand::a:
                        map.insert(QLatin1String(NM_SETTING_WIRELESS_BAND), "a");
                        break;
                    case Knm::WirelessSetting::EnumBand::bg:
                        map.insert(QLatin1String(NM_SETTING_WIRELESS_BAND), "bg");
                        break;
                }
                map.insert(QLatin1String(NM_SETTING_WIRELESS_CHANNEL), setting->channel());
            }
            break;
    }



    if (!setting->bssid().isEmpty())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_BSSID), setting->bssid());
    }
    if (setting->rate())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_RATE), setting->rate());
    }
    if (setting->txpower())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_TX_POWER), setting->txpower());
    }
    if (!setting->macaddress().isEmpty())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_MAC_ADDRESS), setting->macaddress());
    }
    if (!setting->clonedmacaddress().isEmpty())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_CLONED_MAC_ADDRESS), setting->clonedmacaddress());
    }
    if (setting->mtu())
        map.insert(QLatin1String(NM_SETTING_WIRELESS_MTU), setting->mtu());
    if (!setting->seenbssids().isEmpty())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_SEEN_BSSIDS), setting->seenbssids());
    }
    if (!setting->security().isEmpty())
    {
        map.insert(QLatin1String(NM_SETTING_WIRELESS_SEC), setting->security());
    }
    return map;
}

QVariantMap WirelessDbus::toSecretsMap()
{
    QVariantMap map;
    return map;
}

