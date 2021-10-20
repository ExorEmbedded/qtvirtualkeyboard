#ifndef EPADDBUSCLIENT_H
#define EPADDBUSCLIENT_H


#include "EPADProxy.h"
#include <qdbusconnection.h>
#include <qmutex.h>

class EPADDBusClient:public QObject
{
    Q_OBJECT
public:
    static EPADDBusClient *getInstance();
    bool initEPAD();
    quint8 getInfos();
    QString NFCRead();
    void NFCStart();
    void NFCStop();

    QString getLayout();
    QString getActiveLayouts();
    int setLayout(const QString& newLayout);

    bool getNfcKeyboardEmulation();
private:
    EPADDBusClient();  /* singleton class */
    static EPADDBusClient*                  instance;
    ComExorEPADInterface*                   m_epad;
    ComExorEPADNFCReaderInterface*          m_NFCReader;

    //bool m_nfcStarted;
    QMutex m_mutex;

public Q_SLOTS:
    void nfcReceivedNotifyJSON(const QString& json);
Q_SIGNALS:
    void nfcNewNotification(const QString& json);
};

#endif // EPADDBUSCLIENT_H
