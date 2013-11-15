// This file is generated by kconfig_compiler from pppoe.kcfg.
// All changes you do to this file will be lost.
#ifndef PPPOEDBUS_H
#define PPPOEDBUS_H

#include <nm-setting-pppoe.h>

#include <kdebug.h>
#include <kcoreconfigskeleton.h>
#include "settingdbus.h"
#include "nm09dbus_export.h"
namespace Knm
{
    class PppoeSetting;
}

class NM09DBUS_EXPORT PppoeDbus : public SettingDbus
{
    public:
        PppoeDbus(Knm::PppoeSetting *setting);
        ~PppoeDbus();
        void fromMap(const QVariantMap &);
        QVariantMap toMap();
        QVariantMap toSecretsMap();
};
#endif

