#include <QGuiApplication>
//#include <QDesktopWidget>
#include <QQmlEngine>

#include "exorkeyboardsettings.h"
#include "exorvirtualkeyboard.h"

int main(int argc, char *argv[])
{
    /* Run the main application */
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    qputenv("QT_QPA_PLATFORM", QByteArray("wayland-egl"));
    //qputenv("QT_WAYLAND_SHELL_INTEGRATION", QByteArray("inputpanel-shell"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    /* Keyboard backend */
    qmlRegisterSingletonType<ExorKeyboardSettings>("com.exor.Keyboard", 1, 0, "Settings",
                             ExorKeyboardSettings::exorKeyboardSettingsProvider);

    ExorVirtualKeyboard evk;

    /* Configure keyboard (initial) settings here */

    /* Run the main application */
    return app.exec();
}
