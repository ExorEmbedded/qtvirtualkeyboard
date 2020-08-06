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
