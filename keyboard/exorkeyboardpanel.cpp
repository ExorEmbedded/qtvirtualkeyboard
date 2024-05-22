#include "exorkeyboardpanel.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>
#include <QScreen>
#include <QTimer>

#include "exordebug.h"
#include "exorkeyboardsettings.h"

Q_LOGGING_CATEGORY(qExorKeyboardPanel, "exor.keyboard.panel")

ExorKeyboardPanel::ExorKeyboardPanel()
{
    initGUI();

//    /* Register QML code backed.
//     * It is used to report back keyEvents from InputMethod. */
//    QQmlEngine *const engine(this->engine());
//    engine->rootContext()->setContextProperty("ExorVirtualKeyboard", this);

    _update();
}

void ExorKeyboardPanel::_update()
{
    ExorKeyboardSettings *settings = ExorKeyboardSettings::getInstance();

    if (!settings ||
            !settings->update()) {
        qCDebug(qExorKeyboardPanel) << "Rescheduling update";
        QTimer::singleShot(SETTINGS_RETRY_INTERVAL, this, SLOT(_update()));
        return;
    }

    bool large = ExorKeyboardSettings::getInstance()->largeDisplay();

    // only large screens need resizing compared to defaults
    if (!large)
        return;

    int w = width();
    int h = height();
    qCDebug(qExorKeyboardPanel) << "Resizing to " << w << "x" << h;

    // NOTE: keep aligned with exorvirtualkeyboard.qml implementation
    m_inputPanel->setProperty("x", w < h ? 0 : w / 10);
    m_inputPanel->setProperty("width", w < h ? w : (w * 4) / 5);
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

    // Override mask to screen width
    x = 0;
    width = qGuiApp->primaryScreen()->geometry().width();

    QRegion panelArea = QRegion(x,y,width,height);

    if (panelArea != mask()) {
	// Reset mask
	setMask(QRegion());
	setMask(QRegion(x,y,width,height));
    }

    qCDebug(qExorKeyboardPanel) << "set InputMethod visible";
    QGuiApplication::inputMethod()->setVisible(true);
}

void ExorKeyboardPanel::hidePanel()
{
    /* Unlikely */
    if (!m_inputPanel) {
        qCWarning(qExorKeyboardPanel) << "hideKeyboard error: no panel.";
        return;
    }

    /* Hide keyboard */
    hide();
}


void ExorKeyboardPanel::initGUI()
{
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
