#include "exordebug.h"

#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QScopedPointer>

namespace ExorDebug {

static QScopedPointer<QFile> m_logFile;

void openFile(const QString &name)
{
        // Set the logging file
        // check which a path to file you use
        m_logFile.reset(new QFile(name));
        // Open the file logging
        m_logFile.data()->open(QFile::Append | QFile::Text);
        // Set handler
        qInstallMessageHandler(messageHandler);
}

// The implementation of the handler
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    // Open stream file writes
    QTextStream out(m_logFile.data());
    // Write the date of recording
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // By type determine to what level belongs message
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Write to the output category of the message and the message itself
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Clear the buffered data
}

}
