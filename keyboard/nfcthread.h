#ifndef NFCTHREAD_H
#define NFCTHREAD_H

#include <QThread>

class NFCThread : public QThread
{
    Q_OBJECT
public:
    explicit NFCThread();
    void stop();
    void join();
    bool isStopped();

signals:
    void nfcAvailable(QString nfc);

private:
    void run();
    bool readNFC(QString& nfc);

    bool m_stop;
    bool m_running;

public Q_SLOTS:
    void nfcJSONNotification(const QString& json);
};

#endif // NFCTHREAD_H
