#include <pwd.h>
#include <unistd.h>
#include <QtCore/QString>

#include "environment.h"

QString Environment::userName()
{
    register struct passwd *pw;
    register uid_t uid;
    int c;

    uid = geteuid();
    pw = getpwuid(uid);
    if (pw)
    {
        return QString(pw->pw_name);
    }
    return QString();
}

QString i18nc(const char *text)
{
    return QString(text);
}

QString i18nc(const char *context, const char *text)
{
    return QString(text);
}

QString i18nc(const char *context, const char *text, const char *args)
{
    return QString(text).arg(args);
}

QString i18nc(const char *context, const char *text, QString args)
{
    return QString(text).arg(args);
}

QString i18nc(const char *context, const char *text, double args)
{
    return QString(context) + QString(text).arg(args);
}
