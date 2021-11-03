#include <QDebug>
#include "epaddbusclient.h"

Q_LOGGING_CATEGORY(qEPADDBusClient, "exor.keyboard.dbus")

EPADDBusClient* EPADDBusClient::instance = 0;
/******************************************************************************
 *** EPADDBusClient
 ******************************************************************************/
EPADDBusClient::EPADDBusClient():
    m_epad(NULL),
    m_NFCReader(NULL)

{
}

/******************************************************************************
 *** getInstance
 ******************************************************************************/
EPADDBusClient* EPADDBusClient::getInstance()
{
    if (instance == 0)
        instance = new EPADDBusClient();
    return instance;
}

bool EPADDBusClient::initEPAD()
{

    bool initFail=false;

    qCDebug(qEPADDBusClient) << "Init EPAD";

    if(!QFile::exists("/var/run/dbus/system_bus_socket"))
        return false;

    m_mutex.lock();
    if ((m_epad!=NULL) && (m_NFCReader!=NULL))
    {
        m_mutex.unlock();
        return true;
    }


    if (!QDBusConnection::systemBus().isConnected()) {
        qWarning(qEPADDBusClient) << "Failed to get dbus connection : " << QDBusConnection::systemBus().lastError();
        m_mutex.unlock();
        return false;
    }


    if (m_epad!=NULL)
        delete m_epad;
    m_epad = new ComExorEPADInterface("com.exor.EPAD", "/",  QDBusConnection::systemBus());
    if (!m_epad->isValid()) {
        qWarning(qEPADDBusClient) << "Failed to get dbus object / : " << QDBusConnection::systemBus().lastError();
        initFail=true;
    }

    if (m_NFCReader!=NULL)
        delete m_NFCReader;


     m_NFCReader = new ComExorEPADNFCReaderInterface("com.exor.EPAD", "/NFCReader", QDBusConnection::systemBus());
    if (!m_NFCReader->isValid()) {
        qWarning(qEPADDBusClient) << "Failed to get dbus object /NFCReader : " << QDBusConnection::systemBus().lastError();
        initFail=true;
    }

    if (!initFail)
    {
        for (int i=0;i<DBUS_INIT_RETRIES;i++)
        {
            initFail=false;

            //Try to launch both EPAD and NFC Reader dbus commands, to check if everything is really fine
            QDBusPendingReply<QString> sysParmReply=m_epad->getSystemParameter("locale/keyboard/layout");
            sysParmReply.waitForFinished();
            if(!sysParmReply.isFinished() || sysParmReply.isError() || !sysParmReply.isValid()){
                //if something is wrong, retry everything
                qCWarning(qEPADDBusClient) << "EPAD Service test failed: consider EPAD offline";
                initFail=true;
            }

            QDBusPendingReply<int> nfcReply=m_NFCReader->start();
            nfcReply.waitForFinished();
            if(!nfcReply.isFinished() || nfcReply.isError() || !nfcReply.isValid()){
                //if something is wrong, retry everything
                qCWarning(qEPADDBusClient) << "NFC Service test failed: consider EPAD offline";
                initFail=true;
            }

            QThread::msleep(DBUS_INIT_DELAY);
        }
    }

    if (initFail)
    {
        if (m_NFCReader!=NULL)
        {
            delete m_NFCReader;
            m_NFCReader = NULL;
        }

        if (m_epad!=NULL)
        {
            delete m_epad;
            m_epad = NULL;
        }
    }
    else
    {        
        qCDebug(qEPADDBusClient) << "Connect NFCReader notification to EPAD Client ";
        /* For some reason, connect on nfc interface SIGNAL does not work.
         * Probably it is due to Weston starts before EPAD at startup, but even some retries implementation
         * seems to not affect the implementation.
         * Strange behaviour is that EPAD interface seems to work fine.
         * Anyway, we currently use the "base" system bus connect, but this should be investigated
         */
        //connect(m_NFCReader, SIGNAL(notifyJSON(QString)), this, SLOT(nfcReceivedNotifyJSON(QString)));
        QDBusConnection::systemBus().connect("com.exor.EPAD","/NFCReader","com.exor.EPAD.NFCReader","notifyJSON",this,SLOT(nfcReceivedNotifyJSON(QString)));
    }

    m_mutex.unlock();

    return !initFail;
}

/*
 *
 */
QString EPADDBusClient::NFCRead()
{
    QString ret="";
    m_mutex.lock();
    if (m_NFCReader==NULL)
        ret="";
    else
        ret=m_NFCReader->readJSON().argumentAt(1).toString();
    m_mutex.unlock();
    return ret;
}

void EPADDBusClient::NFCStart()
{
    m_mutex.lock();
    if (m_NFCReader!=NULL)
        m_NFCReader->start();
    m_mutex.unlock();
}

void EPADDBusClient::NFCStop()
{
    m_mutex.lock();
    if (m_NFCReader!=NULL)
        m_NFCReader->stop();
    m_mutex.unlock();
}

QString EPADDBusClient::getLayout()
{
    QString ret="";
    m_mutex.lock();
    if (m_epad==NULL)
        ret="";
    else
        ret=m_epad->getSystemParameter("locale/keyboard/layout");
    m_mutex.unlock();
    return ret;
}

QString EPADDBusClient::getActiveLayouts()
{
    QString ret="";
    m_mutex.lock();
    if (m_epad==NULL)
        ret="";
    else
        ret=m_epad->getSystemParameter("locale/keyboard/activeLayouts");
    m_mutex.unlock();
    return ret;
}

int EPADDBusClient::setLayout(const QString& newLayout)
{
    int ret=1;
    m_mutex.lock();
    ret=m_epad->setSystemParameter("locale/keyboard/layout", newLayout);
    m_mutex.unlock();
    return ret;

}
/******************************************************************************
 *** getInfos
 ******************************************************************************/
quint8 EPADDBusClient::getInfos()
{
    return 0;
}

void EPADDBusClient::nfcReceivedNotifyJSON(const QString &json)
{
    emit nfcNewNotification(json);
}

bool EPADDBusClient::getNfcKeyboardEmulation()
{
    QString ret="false";
    m_mutex.lock();
    if (m_epad==NULL)
        ret="false";
    else
        ret=m_epad->getSystemParameter("services/nfc/kbd_emu/autostart");
    m_mutex.unlock();

    return (ret=="true");
}
