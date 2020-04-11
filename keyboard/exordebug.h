#ifndef EXORDEBUG_H
#define EXORDEBUG_H

#include <QLoggingCategory>

namespace ExorDebug {
void openFile(const QString& name);
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}

#endif // EXORDEBUG_H
