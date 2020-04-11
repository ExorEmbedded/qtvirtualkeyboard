#include <QGuiApplication>
//#include <QDesktopWidget>
#include <QQmlEngine>

#include "exorkeyboardsettings.h"
#include "exorkeyboardwayland.h"
#include "exorkeyboardpanel.h"

#include "exordebug.h"

Q_LOGGING_CATEGORY(qExorKeyboard, "exor.keyboard.keyboard")

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ExorDebug::openFile("/var/log/exorvk.log");

    /* Keyboard backend */
    qmlRegisterSingletonType<ExorKeyboardSettings>("com.exor.Keyboard", 1, 0, "Settings",
                             ExorKeyboardSettings::exorKeyboardSettingsProvider);

    qmlRegisterSingletonType<ExorKeyboardWayland>("com.exor.Keyboard", 1, 0, "Wayland",
                             ExorKeyboardWayland::exorKeyboardWaylandProvider);

    ExorKeyboardPanel ekp;
    ExorKeyboardWayland *ekw = ExorKeyboardWayland::getInstance();

    QObject::connect(ekw, &ExorKeyboardWayland::activationChanged, &ekp, &ExorKeyboardPanel::activate);

    qDebug(qExorKeyboard) << "Started";

    /* Run the main application */
    return app.exec();
}
