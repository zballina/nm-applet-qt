// This file is generated by kconfig_compiler from 802-1x.kcfg.
// All changes you do to this file will be lost.

#include "802-1xpersistence.h"

using namespace Knm;

Security8021xPersistence::Security8021xPersistence(Security8021xSetting * setting, KSharedConfig::Ptr config, SettingPersistence::SecretStorageMode mode) : SettingPersistence(setting, config, mode)
{
}

Security8021xPersistence::~Security8021xPersistence()
{
}

void Security8021xPersistence::load()
{
  if (m_config->exists()) {
  Security8021xSetting * setting = static_cast<Security8021xSetting *>(m_setting);
  setting->setEnabled(true);
  setting->setEap(m_config->readEntry("eap", QStringList()));
  setting->setIdentity(m_config->readEntry("identity", ""));
  setting->setAnonymousidentity(m_config->readEntry("anonymousidentity", ""));
  setting->setCacert(QByteArray::fromBase64(m_config->readEntry("cacert", QByteArray())));
  setting->setCapath(m_config->readEntry("capath", ""));
  setting->setClientcert(QByteArray::fromBase64(m_config->readEntry("clientcert", QByteArray())));
  setting->setClientcertpath(m_config->readEntry("clientcertpath", ""));
  {
    QString contents = m_config->readEntry("phase1peapver", "automatic");
    if (contents == "automatic")
      setting->setPhase1peapver(Security8021xSetting::EnumPhase1peapver::automatic);
    if (contents == "zero")
      setting->setPhase1peapver(Security8021xSetting::EnumPhase1peapver::zero);
    else     if (contents == "one")
      setting->setPhase1peapver(Security8021xSetting::EnumPhase1peapver::one);

  }
  setting->setPhase1peaplabel(m_config->readEntry("phase1peaplabel", ""));
  setting->setPhase1fastprovisioning(m_config->readEntry("phase1fastprovisioning", ""));
  {
    QString contents = m_config->readEntry("phase2auth", "none");
    if (contents == "none")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::none);
    else     if (contents == "pap")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::pap);
    else     if (contents == "mschap")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::mschap);
    else     if (contents == "mschapv2")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::mschapv2);
    else     if (contents == "chap")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::chap);
    else     if (contents == "md5")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::md5);
    else     if (contents == "gtc")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::gtc);
    else     if (contents == "otp")
      setting->setPhase2auth(Security8021xSetting::EnumPhase2auth::otp);

  }
  {
    QString contents = m_config->readEntry("phase2autheap", "none");
    if (contents == "none")
      setting->setPhase2autheap(Security8021xSetting::EnumPhase2autheap::none);
    else     if (contents == "md5")
      setting->setPhase2autheap(Security8021xSetting::EnumPhase2autheap::md5);
    else     if (contents == "gtc")
      setting->setPhase2autheap(Security8021xSetting::EnumPhase2autheap::gtc);
    else     if (contents == "otp")
      setting->setPhase2autheap(Security8021xSetting::EnumPhase2autheap::otp);
    else     if (contents == "mschapv2")
      setting->setPhase2autheap(Security8021xSetting::EnumPhase2autheap::mschapv2);
    else     if (contents == "tls")
      setting->setPhase2autheap(Security8021xSetting::EnumPhase2autheap::tls);

  }
  setting->setPhase2cacert(QByteArray::fromBase64(m_config->readEntry("phase2cacert", QByteArray())));
  setting->setPhase2capath(m_config->readEntry("phase2capath", ""));
  setting->setPhase2clientcert(QByteArray::fromBase64(m_config->readEntry("phase2clientcert", QByteArray())));
  setting->setPhase2clientcertpath(m_config->readEntry("phase2clientcertpath", ""));
  // SECRET
  if (m_storageMode == SettingPersistence::PlainText) {
    setting->setPassword(m_config->readEntry("password", ""));
  }
  setting->setPrivatekey(QByteArray::fromBase64(m_config->readEntry("privatekey", QByteArray())));
  setting->setPrivatekeypath(m_config->readEntry("privatekeypath", ""));
  // SECRET
  if (m_storageMode == SettingPersistence::PlainText) {
    setting->setPrivatekeypassword(m_config->readEntry("privatekeypassword", ""));
  }
  setting->setPhase2privatekey(QByteArray::fromBase64(m_config->readEntry("phase2privatekey", QByteArray())));
  setting->setPhase2privatekeypath(m_config->readEntry("phase2privatekeypath", ""));
  // SECRET
  if (m_storageMode == SettingPersistence::PlainText) {
    setting->setPhase2privatekeypassword(m_config->readEntry("phase2privatekeypassword", ""));
    setting->setSecretsAvailable(true);
  }
  setting->setPin(m_config->readEntry("pin", ""));
  //setting->setPsk(m_config->readEntry("psk", ""));
  setting->setUseSystemCaCerts(m_config->readEntry("UseSystemCaCerts", false));

  setting->setPasswordflags(Setting::AgentOwned);
  setting->setPrivatekeypasswordflags(Setting::AgentOwned);
  setting->setPhase2privatekeypasswordflags(Setting::AgentOwned);
  setting->setPinflags(Setting::AgentOwned);
  }
}
