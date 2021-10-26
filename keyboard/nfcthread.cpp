#include "nfcthread.h"
#include "epaddbusclient.h"

#include "exordebug.h"

#define NFC_SLEEP_TIME         200
#define NFC_CHECK_TIME         10000
#define NFC_CHECK_COUNTER      (NFC_CHECK_TIME / NFC_SLEEP_TIME)

Q_LOGGING_CATEGORY(qExorKeyboardWaylandNFC, "exor.keyboard.wayland.nfc")

NFCThread::NFCThread():
    m_stop(false),
    m_running(false)
{
}

void NFCThread::run()
{
    m_running = true;
    m_stop =  false;

    qCDebug(qExorKeyboardWaylandNFC) << " nfc thread Started";

    EPADDBusClient* client = EPADDBusClient::getInstance();
    //client->NFCStart();
    connect(client, SIGNAL(nfcNewNotification(QString)), this, SLOT(nfcJSONNotification(QString)));

    int counter =  0;
    while (1)
    {
        if (m_stop)
            break;

        counter ++;
        if (counter >= NFC_CHECK_COUNTER)
        {
            //Simply check if EPAD interface has been started
            //As we manage only UID, we simply wait for notifyJSON event
            if (client->initEPAD())
            {
                client->NFCStart();
            }

            counter = 0;
        }

        msleep(NFC_SLEEP_TIME);
    }


    qCDebug(qExorKeyboardWaylandNFC) << "Terminated";
    m_running = false;
}


void NFCThread::stop()
{
    qCDebug(qExorKeyboardWaylandNFC) << "Stopping...";
    m_stop = true;
}

void NFCThread::join()
{
    while (m_running)
        msleep(NFC_SLEEP_TIME / 2);

    qCDebug(qExorKeyboardWaylandNFC) << "Stopped";
}

bool NFCThread::isStopped()
{
    return !m_running;
}

bool NFCThread::readNFC(QString& nfc)
{
    EPADDBusClient* client = EPADDBusClient::getInstance();
    QString data = client->NFCRead();
    if (data.length() == 0)
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonValue tag = doc.object().value("tag");
    if (tag.isUndefined())
        return false;

    QJsonValue uid = tag.toObject().value("uid");
    if (uid.isUndefined())
        return false;

    nfc = uid.toString();
    return true;
}

void NFCThread::nfcJSONNotification(const QString& json)
{
    qDebug()<<"NFC ARRIVED! "<< json;

    if (json.length() == 0)
        return;

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonValue tag = doc.object().value("tag");
    if (tag.isUndefined())
        return;

    QJsonValue jVal = tag.toObject().value("event");
    if (jVal.isUndefined())
        return;

    if (jVal.toInt()!=1)
        //we get the data once the tag is reached
        return;

    jVal = tag.toObject().value("uid");
    if (jVal.isUndefined())
        return;

    //before emit the SIGNAL, check if it is required by keyboard emulation
    if (EPADDBusClient::getInstance()->getNfcKeyboardEmulation())
        emit nfcAvailable(jVal.toString());
}


