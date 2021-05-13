#ifndef EXORKEYBOARDSETTINGS_H
#define EXORKEYBOARDSETTINGS_H

#include <QObject>
#include <EPADProxy.h>

class QQmlEngine;
class QJSEngine;

/*
 * --- ExorKeyboardSettings ---
 *
 * This class wraps the DBUS calls to "com.exor.EPAD.KeyboardSettings"
 * Maps the remote properties to the values to be passed to
 * Qt VirtualKeyboardSettings properties @m_activeLocales and @locale.
 *
 */
class ExorKeyboardSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList activeLocales READ activeLocales)
    Q_PROPERTY(QString locale READ locale)

public:
    static ExorKeyboardSettings* getInstance();
    static QObject* exorKeyboardSettingsProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE void update();
    Q_INVOKABLE void updateLocale(const QString& newLocale);

    QStringList activeLocales();
    QString locale();

private:
    explicit ExorKeyboardSettings(QObject *parent = nullptr);
    void initEPAD();

    ComExorEPADInterface* m_epad;

    QStringList m_activeLocales;
    QString m_locale;
};

#endif // EXORKEYBOARDSETTINGS_H
