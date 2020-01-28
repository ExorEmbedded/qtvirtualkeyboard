#include "exorkeyboardsettings.h"

#include <QFile>
#include <QDBusInterface>
#include <QDebug>

#define DEFAULT_LOCALE "it_IT"

#define KEYBOARD_CONFFILE "/etc/weston/keyboard"

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
    m_locale(DEFAULT_LOCALE)
{
#if HAVE_DBUS_SETTINGS
    QDBusInterface *iface;
    QVariant reply;

    /* Query DBUS service */
    iface = new QDBusInterface("com.exor.EPAD", "/", "com.exor.EPAD.KeyboardSettings",
                               QDBusConnection::sessionBus(), this);
    if (!iface->isValid())
    {
        qWarning() << "Unable to connect to com.exor.EPAD.KeyboardSettings ("
                   << QDBusConnection::sessionBus().lastError().message() << ")";
        return;
    }

    /*
     * Query DBUS properties:
     *
     * [com.exor.EPAD.KeyboardSettings.availableLayouts -> m_activeLocales]
     * Comma-separated list of layouts to support.
     *
     * [com.exor.EPAD.KeyboardSettings.currentLayout -> m_layout]
     * Layout to set as current.
     *
     */
    reply = iface->property("availableLayouts");
    if (!reply.isNull())
    {
        QString availableLayoutsString = reply.toString();
        qDebug() << "Received available layouts: " << availableLayoutsString;
        m_activeLocales = availableLayoutsString.split(",");
    }
    else
    {
        qWarning() << "Unable to parse available layouts list "
                   << "from com.exor.EPAD.KeyboardSettings. Keep default.";
    }

    reply = iface->property("currentLayout");
    if (!reply.isNull())
    {
        QString currentLayoutString = reply.toString();
        qDebug() << "Received current layout: " << currentLayoutString;
        m_locale = currentLayoutString;
    }
    else
    {
        qWarning() << "Unable to parse locale from "
                   << "from com.exor.EPAD.KeyboardSettings. Keep default.";
    }

    /*
     * Unlikely. VirtualKeyboardSettings should handle the case.
     * Still, if selected locale is not in active locales list, append it.
     */
    if (-1 == m_activeLocales.indexOf(m_locale))
    {
        qWarning() <<
                      "Locale: " << m_locale <<
                      " not found in active locales: " << m_activeLocales << ". Add.";
        m_activeLocales << m_locale;
    }
#else

    QStringList confLayouts;

    QFile confFile(KEYBOARD_CONFFILE);
    if (confFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&confFile);
        QString line = stream.readLine();
        if (!line.isNull() && line.split(":").size() == 2)
        {
            confLayouts = line.split(":");
            m_locale = confLayouts.at(0);
            m_activeLocales = confLayouts.at(1).split(",");
        }
        else
        {
            qWarning() << "Bad line in file: " << KEYBOARD_CONFFILE;
        }
    }
    else
    {
        qWarning() << "Bad file: " << KEYBOARD_CONFFILE;
    }

#endif

    qDebug() << "Active locales: " << m_activeLocales;
    qDebug() << "Locale: " << m_locale;
}

QStringList ExorKeyboardSettings::activeLocales()
{
    return m_activeLocales;
}

QString ExorKeyboardSettings::locale()
{
    return m_locale;
}
