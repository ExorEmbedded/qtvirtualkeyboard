#include "exorkeyboardsettings.h"

#include <QFile>
#include <QDBusInterface>
#include <QDebug>

#define DEFAULT_LOCALE "en_US"

#define KEYBOARD_CONFFILE "/etc/weston/keyboard"

#include "exordebug.h"
#include "epaddbusclient.h"

Q_LOGGING_CATEGORY(qExorKeyboardSettings, "exor.keyboard.settings")

/* Provider for Singleton type */
ExorKeyboardSettings* ExorKeyboardSettings::getInstance()
{
    static ExorKeyboardSettings *exorKeyboardSettings = NULL;

    if (!exorKeyboardSettings)
    {
        exorKeyboardSettings = new ExorKeyboardSettings();
    }

    return exorKeyboardSettings;
}

QObject * ExorKeyboardSettings::exorKeyboardSettingsProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return static_cast<QObject *>(ExorKeyboardSettings::getInstance());
}

ExorKeyboardSettings::ExorKeyboardSettings(QObject *parent) :
    QObject(parent),
    m_activeLocales(QStringList(DEFAULT_LOCALE)),
    m_locale(DEFAULT_LOCALE),
    m_displayInches(-1)
{
}

bool ExorKeyboardSettings::update()
{
    EPADDBusClient* client = EPADDBusClient::getInstance();
    if (!client->initEPAD())
        return false;
    m_locale = client->getLayout();
    QString activeLocales = client->getActiveLayouts();
    m_activeLocales = activeLocales.split(",", QString::SkipEmptyParts);

    qCDebug(qExorKeyboardSettings) << "Active locales: " << m_activeLocales;
    qCDebug(qExorKeyboardSettings) << "Locale: " << m_locale;

    if (m_displayInches != -1)
        return true;

    QString jsonInfo = client->getSystemInfo();
    qCDebug(qExorKeyboardSettings) << "Info: " << jsonInfo;
    if (jsonInfo.isEmpty())
        return false;

    QJsonDocument json = QJsonDocument::fromJson(jsonInfo.toUtf8());
    QVariantMap sysInfo = json.toVariant().toMap();
    if (sysInfo.contains("video")) {
        QVariantMap video = sysInfo["video"].toMap();
        if (video.contains("inches"))
            m_displayInches = video["inches"].toInt();
    }
    qCDebug(qExorKeyboardSettings) << "Large? " << largeDisplay();

    return true;
}

bool ExorKeyboardSettings::updateLocale(const QString& newLocale)
{
    EPADDBusClient* client = EPADDBusClient::getInstance();
    if (!client->initEPAD())
        return false;

    qCDebug(qExorKeyboardSettings) << "Writing new default locale" << newLocale;
    if ( client->setLayout(newLocale) ) {
       qCWarning(qExorKeyboardSettings) << "Failed to set layout";
       return false;
    }

    return true;
}

QStringList ExorKeyboardSettings::activeLocales()
{
    return m_activeLocales;
}

QString ExorKeyboardSettings::locale()
{
    return m_locale;
}

// Touch display is considered large for 15 inch upwards
bool ExorKeyboardSettings::largeDisplay()
{
    return (m_displayInches >= 15);
}
