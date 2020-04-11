#include "exorkeyboardpanel.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>
#include <QScreen>

#include "exordebug.h"

Q_LOGGING_CATEGORY(qExorKeyboardPanel, "exor.keyboard.panel")

ExorKeyboardPanel::ExorKeyboardPanel()
{
    initGUI();

//    /* Register QML code backed.
//     * It is used to report back keyEvents from InputMethod. */
//    QQmlEngine *const engine(this->engine());
//    engine->rootContext()->setContextProperty("ExorVirtualKeyboard", this);
}

void ExorKeyboardPanel::activate(bool active)
{
    qDebug(qExorKeyboardPanel) << Q_FUNC_INFO << "active:" << active;

    if (active)
        showPanel();
    else
        hidePanel();
}

void ExorKeyboardPanel::showPanel()
{
     qCDebug(qExorKeyboardPanel) << Q_FUNC_INFO;

    /* Update settings configuration */
    if (m_exorImHandler) {
        QMetaObject::invokeMethod(m_exorImHandler, "updateInputPanel");
    }

    /* Display keyboard */
    show();

    /* Unlikely */
    if (!m_inputPanel) {
        qCWarning(qExorKeyboardPanel) << "showKeyboard error: no panel.";
        return;
    }

    /*
     * Retrieve InputPanel area
     * Prevent window area outside InputPanel to get events
     */
    int x = m_inputPanel->property("x").toInt();
    int y = m_inputPanel->property("y").toInt();
    int width = m_inputPanel->property("width").toInt();
    int height = m_inputPanel->property("height").toInt();

    setMask(QRegion(x,y,width,height));
}


void ExorKeyboardPanel::hidePanel()
{
    qCDebug(qExorKeyboardPanel) << Q_FUNC_INFO;
    /* Unlikely */
    if (!m_inputPanel) {
        qCWarning(qExorKeyboardPanel) << "hideKeyboard error: no panel.";
        return;
    }

    /* Reset mask */
    setMask(QRegion());

    /* Hide keyboard */
    hide();
}


void ExorKeyboardPanel::initGUI()
{
    qCDebug(qExorKeyboardPanel) << Q_FUNC_INFO;
    /* QWindow customization */
    setFlags(Qt::Window | Qt::FramelessWindowHint
        | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);

    setWindowState(Qt::WindowNoState);

    /* QWindow - Background */
    QSurfaceFormat format = this->format();
    format.setAlphaBufferSize(8);
    setFormat(format);
    setColor(QColor(Qt::transparent));

    /*
     * QWindow - Occupy entire available surface
     * (Wayland InputPanelSurface will slide bottom-up)
     */
    setGeometry(qGuiApp->primaryScreen()->geometry());
    setResizeMode(QQuickView::SizeRootObjectToView);

    /* QQuickView content */
    const QUrl url(QStringLiteral("qrc:/exorvirtualkeyboard.qml"));
    setSource(url);

    /* Render to initialize panel, then hide */
    show();

    if (!m_inputPanel) {
        m_inputPanel = findChild<QObject*>("inputPanel");
    }

    /* Set Exor InputMethod as default IM */
    m_exorImHandler = findChild<QObject*>("exorIMHandler");

    /* Unlikely */
    if (!m_exorImHandler) {
        qCWarning(qExorKeyboardPanel) << "showKeyboard error: no exor handler.";
    } else {
        QMetaObject::invokeMethod(m_exorImHandler, "setCustomInputMethod");
    }
    hide();
}
