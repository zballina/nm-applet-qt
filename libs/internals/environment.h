#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class QString;

class Environment
{
    public:
        static QString userName();
};

QString i18nc(const char *);
QString i18nc(const char *, const char *);
QString i18nc(const char *, const char *, const char *);
QString i18nc(const char *, const char *, QString);
QString i18nc(const char *, const char *, double);

#endif // ENVIRONMENT_H
