// This file is generated by kconfig_compiler from cdma.kcfg.
// All changes you do to this file will be lost.

#include "cdma.h"

using namespace Knm;

CdmaSetting::CdmaSetting() : Setting(Setting::Cdma)
{
}

CdmaSetting::CdmaSetting(CdmaSetting *setting) : Setting(setting), mPasswordflags(Setting::AgentOwned)
{
    setNumber(setting->number());
    setUsername(setting->username());
    setPassword(setting->password());
    setPasswordflags(setting->passwordflags());
}

CdmaSetting::~CdmaSetting()
{
}

QString CdmaSetting::name() const
{
    return QLatin1String("cdma");
}

QMap<QString, QString> CdmaSetting::secretsToMap() const
{
    QMap<QString, QString> map;
    if (passwordflags().testFlag(Setting::AgentOwned))
    {
        map.insert(QLatin1String("password"), password());
    }
    return map;
}

void CdmaSetting::secretsFromMap(QMap<QString, QString> secrets)
{
    setPassword(secrets.value("password"));
}

QStringList CdmaSetting::needSecrets(const bool requestNew) const
{
    QStringList list;
    if ((password().isEmpty() || requestNew) && !passwordflags().testFlag(Setting::NotRequired))
        list.append("password");
    return list;
}

bool CdmaSetting::hasPersistentSecrets() const
{
    if (passwordflags().testFlag(Setting::None) || passwordflags().testFlag(Setting::AgentOwned))
        return true;
    return false;
}
