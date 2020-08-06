#ifndef EPADPROXY_H_1437460255
#define EPADPROXY_H_1437460255

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.exor.EPAD
 */
class ComExorEPADInterface: public QDBusAbstractInterface
{
	Q_OBJECT
public:
	static inline const char *staticInterfaceName()
	{ return "com.exor.EPAD"; }

public:
	ComExorEPADInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

	~ComExorEPADInterface();

public Q_SLOTS: // METHODS

	inline QDBusPendingReply<int> setSystemParameter(const QString &key, const QString &value)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(key) << QVariant::fromValue(value);
		return asyncCallWithArgumentList(QLatin1String("setSystemParameter"), argumentList);
	}

	inline QDBusPendingReply<QString> getSystemParameter(const QString &key)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(key);
		return asyncCallWithArgumentList(QLatin1String("getSystemParameter"), argumentList);
	}

	inline QDBusPendingReply<int> setVolatileParameter(const QString &key, const QString &value)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(key) << QVariant::fromValue(value);
		return asyncCallWithArgumentList(QLatin1String("setVolatileParameter"), argumentList);
	}

	inline QDBusPendingReply<QString> getVolatileParameter(const QString &key)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(key);
		return asyncCallWithArgumentList(QLatin1String("getVolatileParameter"), argumentList);
	}

	inline QDBusPendingReply<QDBusObjectPath> checkImage(const QString &imageType)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(imageType);
		return asyncCallWithArgumentList(QLatin1String("checkImage"), argumentList);
	}

	inline QDBusPendingReply<int> clearTimeCounters(int which)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(which);
		return asyncCallWithArgumentList(QLatin1String("clearTimeCounters"), argumentList);
	}

	inline QDBusPendingReply<QDBusObjectPath> downloadImage(const QString &imageType, const QString &source, const QString &sourceMd5, bool checkMd5)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(imageType) << QVariant::fromValue(source) << QVariant::fromValue(sourceMd5) << QVariant::fromValue(checkMd5);
		return asyncCallWithArgumentList(QLatin1String("downloadImage"), argumentList);
	}

	inline QDBusPendingReply<QString> dualBootMode()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("dualBootMode"), argumentList);
	}

	inline QDBusPendingReply<QDBusObjectPath> formatImage(const QString &imageType)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(imageType);
		return asyncCallWithArgumentList(QLatin1String("formatImage"), argumentList);
	}

	inline QDBusPendingReply<int> getBlightCounter()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("getBlightCounter"), argumentList);
	}

	inline QDBusPendingReply<int> getScreenOrientation()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("getScreenOrientation"), argumentList);
	}

	inline QDBusPendingReply<int> getSysCounter()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("getSysCounter"), argumentList);
	}

	inline QDBusPendingReply<int> lockScreen(const QString &display)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(display);
		return asyncCallWithArgumentList(QLatin1String("lockScreen"), argumentList);
	}

	inline QDBusPendingReply<int> lockScreenSupported()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("lockScreenSupported"), argumentList);
	}

	inline QDBusPendingReply<> mouseNotification(int bPlugged)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(bPlugged);
		return asyncCallWithArgumentList(QLatin1String("mouseNotification"), argumentList);
	}

	inline QDBusPendingReply<int> mouseStatus(const QString &display)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(display);
		return asyncCallWithArgumentList(QLatin1String("mouseStatus"), argumentList);
	}

	inline QDBusPendingReply<int> restartSystem()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("restartSystem"), argumentList);
	}

	inline QDBusPendingReply<int> restartSystemWithImage(int imageType)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(imageType);
		return asyncCallWithArgumentList(QLatin1String("restartSystemWithImage"), argumentList);
	}

	inline QDBusPendingReply<int> setScreenOrientation(int angle)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(angle);
		return asyncCallWithArgumentList(QLatin1String("setScreenOrientation"), argumentList);
	}

	inline QDBusPendingReply<int> unlockScreen(const QString &display)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(display);
		return asyncCallWithArgumentList(QLatin1String("unlockScreen"), argumentList);
	}

	inline QDBusPendingReply<QDBusObjectPath> uploadImage(const QString &imageType, const QString &destination, bool checkMd5)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(imageType) << QVariant::fromValue(destination) << QVariant::fromValue(checkMd5);
		return asyncCallWithArgumentList(QLatin1String("uploadImage"), argumentList);
	}

	inline QDBusPendingReply<QDBusObjectPath> uploadRawImage(const QString &imageType, const QString &destination, bool checkMd5)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(imageType) << QVariant::fromValue(destination) << QVariant::fromValue(checkMd5);
		return asyncCallWithArgumentList(QLatin1String("uploadRawImage"), argumentList);
	}

	inline QDBusPendingReply<int> applyDefaultSettings()
	{
		QList<QVariant> argumentList;
		return asyncCallWithArgumentList(QLatin1String("applyDefaultSettings"), argumentList);
	}

	inline QDBusPendingReply<int> loadMachineFile(const QByteArray& data)
	{
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(data);
		return asyncCallWithArgumentList(QLatin1String("loadMachineFile"), argumentList);
	}

Q_SIGNALS:
	void error(const QString &error);
	void mousePlugEvent(int bPlugged);
	void systemParameterChanged(const QString &key, const QString &value);
	void volatileParameterChanged(const QString &key, const QString &value);
};

#endif
