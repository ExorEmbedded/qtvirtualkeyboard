#include "EPADProxy.h"

/*
 * Implementation of interface class ComExorEPADInterface
 */

ComExorEPADInterface::ComExorEPADInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
	: QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

ComExorEPADInterface::~ComExorEPADInterface()
{
}


/*
 *
 */
ComExorEPADNFCReaderInterface::ComExorEPADNFCReaderInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

ComExorEPADNFCReaderInterface::~ComExorEPADNFCReaderInterface()
{
}


QDBusPendingReply<int, QString> ComExorEPADNFCReaderInterface::readJSON()
{
    return asyncCall(QLatin1String("readJSON"));
}

QDBusPendingReply<int>ComExorEPADNFCReaderInterface::start()
{
    return asyncCall(QLatin1String("start"));
}

QDBusPendingReply<int>ComExorEPADNFCReaderInterface::stop()
{
    return asyncCall(QLatin1String("stop"));
}
