#ifndef EXORKEYBOARDSETTINGS_H
#define EXORKEYBOARDSETTINGS_H

#include <QVariantMap>
#include <QObject>
#include <EPADProxy.h>

#define SETTINGS_RETRY_INTERVAL 3000

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

    Q_INVOKABLE bool update();
    Q_INVOKABLE bool updateLocale(const QString& newLocale);

    QStringList activeLocales();
    QString locale();
    Q_INVOKABLE bool largeDisplay();

private:
    explicit ExorKeyboardSettings(QObject *parent = nullptr);
    void initEPAD();

    QStringList m_activeLocales;
    QString m_locale;
    int m_displayInches;
};

#endif // EXORKEYBOARDSETTINGS_H
