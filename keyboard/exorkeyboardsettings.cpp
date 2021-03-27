#include "exorkeyboardsettings.h"

#include <QFile>
#include <QDBusInterface>
#include <QDebug>

#define DEFAULT_LOCALE "en_US"

#define KEYBOARD_CONFFILE "/etc/weston/keyboard"

#include "exordebug.h"

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
    m_epad(NULL),
    m_connection(NULL)
{
    update();
}

void ExorKeyboardSettings::initEPAD() {
	m_connection = new QDBusConnection(QDBusConnection::systemBus());

	m_epad = new ComExorEPADInterface("com.exor.EPAD", "/",  *m_connection);
	if (!m_epad->isValid()) {
		qWarning() << "Failed to get dbus object / : " << m_connection->lastError();
		delete m_connection;
		delete m_epad;
		m_connection = NULL;
		m_epad = NULL;
		QDBusConnection::disconnectFromBus(QDBusConnection::systemBus().name());
	}
}

void ExorKeyboardSettings::update()
{
    if ( m_epad == NULL ) {
        initEPAD();
        if ( m_epad == NULL )
            return;
    }

    m_locale = m_epad->getSystemParameter("locale/keyboard/layout");

    QString activeLocales = m_epad->getSystemParameter("locale/keyboard/activeLayouts");
    m_activeLocales = activeLocales.split(",", QString::SkipEmptyParts);

    qCDebug(qExorKeyboardSettings) << "Active locales: " << m_activeLocales;
    qCDebug(qExorKeyboardSettings) << "Locale: " << m_locale;
}

void ExorKeyboardSettings::updateLocale(const QString& newLocale)
{
    if ( m_epad == NULL ) {
        initEPAD();
        if ( m_epad == NULL )
            return;
    }

   qCDebug(qExorKeyboardSettings) << "Writing new default locale" << newLocale;

   if ( m_epad->setSystemParameter("locale/keyboard/layout", newLocale) )
      qCWarning(qExorKeyboardSettings) << "Failed to set layout";

}

QStringList ExorKeyboardSettings::activeLocales()
{
    return m_activeLocales;
}

QString ExorKeyboardSettings::locale()
{
    return m_locale;
}
