#ifndef EPADDBUSCLIENT_H
#define EPADDBUSCLIENT_H


#include "EPADProxy.h"
#include <qdbusconnection.h>
#include <qmutex.h>

#define DBUS_INIT_RETRIES   10
#define DBUS_INIT_DELAY     1000

class EPADDBusClient:public QObject
{
    Q_OBJECT
public:
    static EPADDBusClient *getInstance();
    bool initEPAD();
    QString NFCRead();
    void NFCStart();
    void NFCStop();

    QString getSystemInfo();
    QString getLayout();
    QString getActiveLayouts();
    int setLayout(const QString& newLayout);

    bool getNfcKeyboardEmulation();
private:
    EPADDBusClient();  /* singleton class */
    static EPADDBusClient*                  instance;
    ComExorEPADInterface*                   m_epad;
    ComExorEPADSystemInfoInterface*         m_sysInfo;
    ComExorEPADNFCReaderInterface*          m_NFCReader;

    //bool m_nfcStarted;
    QMutex m_mutex;

public Q_SLOTS:
    void nfcReceivedNotifyJSON(const QString& json);
Q_SIGNALS:
    void nfcNewNotification(const QString& json);
};

#endif // EPADDBUSCLIENT_H
