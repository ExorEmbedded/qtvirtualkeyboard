#include "exorkeyboardwayland.h"

#include <QObject>
#include <QGuiApplication>
#include <QDebug>
#include <xkbcommon/xkbcommon.h>
#include <time.h>

#include <qpa/qplatformnativeinterface.h>

#include "input-method-unstable-v1.h"
#include "text-input-unstable-v1.h"

#include "exordebug.h"

#include "nfcthread.h"

Q_LOGGING_CATEGORY(qExorKeyboardWayland, "exor.keyboard.wayland")

/* ************************************************************** */

int prevUtf8CharIndex(const QByteArray &array, uint32_t current)
{
    int idx = current;
    while (--idx >=0)
    {
        if ((array.at(idx) & 0xc0) != 0x80)
        {
            return idx;
        }
    }
    return -1;
}

/* ************************************************************** */

/*
 * Callbacks for Wayland InputMethod Context.
 *
 * Each will redirect to correspoding method in ExorKeyboardWayland.
 * (Static cast from @data to ExorKeyboardWayland).
 */
static void
input_context_handle_surrounding_text(void *data,
                        struct zwp_input_method_context_v1 *context,
                        const char *text,
                        uint32_t cursor,
                        uint32_t anchor)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputContextHandleSurroundingText(context, text, cursor, anchor);
}

static void
input_context_handle_reset(void *data,
             struct zwp_input_method_context_v1 *context)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputContextHandleReset(context);
}

static void
input_context_handle_content_type(void *data,
                    struct zwp_input_method_context_v1 *context,
                    uint32_t hint,
                    uint32_t purpose)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputContextHandleContentType(context, hint, purpose);
}

static void
input_context_handle_invoke_action(void *data,
                     struct zwp_input_method_context_v1 *context,
                     uint32_t button,
                     uint32_t index)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputContextHandleInvokeAction(context, button, index);
}

static void
input_context_handle_commit_state(void *data,
                    struct zwp_input_method_context_v1 *context,
                    uint32_t serial)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputContextHandleCommitState(context, serial);
}

static void
input_context_handle_preferred_language(void *data,
                          struct zwp_input_method_context_v1 *context,
                          const char *language)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputContextHandlePreferredLanguage(context, language);
}

/*
 * Input Method Context listener.
 * Registered when input method is activated.
 */
static const struct zwp_input_method_context_v1_listener input_method_context_listener = {
    input_context_handle_surrounding_text,
    input_context_handle_reset,
    input_context_handle_content_type,
    input_context_handle_invoke_action,
    input_context_handle_commit_state,
    input_context_handle_preferred_language
};


/* ************************************************************** */

/* Input Method listener callbacks.
 * Each will redirect to correspoding method in ExorKeyboardWayland.
 */
static void
input_method_handle_activate(void *data,
                      struct zwp_input_method_v1 *input_method,
                      struct zwp_input_method_context_v1 *context)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputMethodHandleActivate(input_method, context);
}

static void
input_method_handle_deactivate(void *data,
                        struct zwp_input_method_v1 *input_method,
                        struct zwp_input_method_context_v1 *context)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->inputMethodHandleDeactivate(input_method, context);
}

static const struct zwp_input_method_v1_listener input_method_listener = {
    input_method_handle_activate,
    input_method_handle_deactivate
};


/* ************************************************************** */

/* Global registry listener callbacks. */
static void global_handler(void *data,
                           struct wl_registry *wl_registry, uint32_t name,
                           const char *interface, uint32_t version)
{
    ExorKeyboardWayland *ekw = static_cast<ExorKeyboardWayland*>(data);
    ekw->globalHandler(wl_registry, name, interface, version);
}

static void global_remove_handler(void *,
                                  struct wl_registry *, uint32_t )
{

}

static struct wl_registry_listener registry_listener = {
    &global_handler,
    &global_remove_handler
};


/* Provider for Singleton type */
ExorKeyboardWayland* ExorKeyboardWayland::getInstance()
{
    static ExorKeyboardWayland *exorKeyboardSettings = NULL;

    if (!exorKeyboardSettings)
    {
        exorKeyboardSettings = new ExorKeyboardWayland();
    }

    return exorKeyboardSettings;
}

QObject * ExorKeyboardWayland::exorKeyboardWaylandProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return static_cast<QObject *>(ExorKeyboardWayland::getInstance());
}


ExorKeyboardWayland::ExorKeyboardWayland(QObject *parent) :
    QObject(parent),
    m_input_method(NULL),
    m_context(NULL),
    m_contextIsActive(false),
    mk_language("en"),
    mk_text_direction(ZWP_TEXT_INPUT_V1_TEXT_DIRECTION_LTR)
{
    /* Init */
    waylandConnect();

    /* InputMethod context status  */
    m_preedit_string = QByteArray();
    m_preferred_language = QByteArray(mk_language);


    qCDebug(qExorKeyboardWayland) <<  "start NFC Thread -1!!! ";
    m_nfcThread=new NFCThread();
    connect(m_nfcThread, SIGNAL(nfcAvailable(QString)), this, SLOT(nfcReceived(QString)));

    qCDebug(qExorKeyboardWayland) <<  "start NFC Thread!!! ";
    m_nfcThread->start();
}

void ExorKeyboardWayland::nfcReceived(QString nfc)
{
    qCDebug(qExorKeyboardWayland) <<  "RECEIVED!!! "<<nfc;

    m_preedit_string = nfc.toLocal8Bit();
    //sendPreedit(-1);
    commitPreedit(); //REMOVE ?

}

void ExorKeyboardWayland::globalHandler(struct wl_registry *wl_registry, uint32_t name,
                                     const char *interface, uint32_t version)
{
    if (strcmp(interface, "wl_compositor") == 0) {
        qCDebug(qExorKeyboardWayland) << "Wayland: bind wl_compositor";
        m_compositor = (struct wl_compositor *) wl_registry_bind(wl_registry, name,
                                        &wl_compositor_interface, 3);
    }
    else if (!strcmp(interface, "zwp_input_method_v1")) {
        qCDebug(qExorKeyboardWayland) << "Wayland: bind zwp_input_method_v1 and add listener";
        m_input_method = (struct zwp_input_method_v1 *)
                wl_registry_bind(wl_registry,
                                 name, &zwp_input_method_v1_interface, 1);

        zwp_input_method_v1_add_listener(m_input_method,
                                         &input_method_listener,
                                         this);
    }
}

void ExorKeyboardWayland::inputMethodHandleActivate(struct zwp_input_method_v1 *,
                                           struct zwp_input_method_context_v1 *context)
{
    if (m_context) {
        zwp_input_method_context_v1_destroy(m_context);
    }

    m_preedit_string = QByteArray();
    m_content_hint = 0;
    m_content_purpose = 0;
    m_preferred_language = QByteArray();
    m_surrounding_text = QByteArray();

    m_serial = 0;
    m_context = context;

    zwp_input_method_context_v1_add_listener(context,
                                             &input_method_context_listener,
                                             this);

    /* Update information from QtVirtualKeyboard - get_current_layout()
     * This will updated mk_language and mk_text_direction.
     */
    zwp_input_method_context_v1_language(m_context,
                                         m_serial,
                                         mk_language);
    zwp_input_method_context_v1_text_direction(m_context,
                                               m_serial,
                                               mk_text_direction);

    /* Change activation status -will notify with signal- */
    activateContext(true);
}


void ExorKeyboardWayland::inputMethodHandleDeactivate(struct zwp_input_method_v1 *,
                                             struct zwp_input_method_context_v1 *)
{
    /* Change activation status -will notify with signal- */
    activateContext(false);
}


void ExorKeyboardWayland::inputContextHandlePreferredLanguage(struct zwp_input_method_context_v1 *,
                                               const char *language)
{
    if (language)
    {
        m_preferred_language = QByteArray(language);
    }
    else
    {
        m_preferred_language = QByteArray();
    }

    /* Will report to QtVirtualKeyboard configuration */
}


void ExorKeyboardWayland::inputContextHandleCommitState(struct zwp_input_method_context_v1 *,
                                         uint32_t serial)
{
    m_serial = serial;

    /*
     * Update information from QtVirtualKeyboard - get_current_layout()
     * This will updated mk_language and mk_text_direction.
     */
    zwp_input_method_context_v1_language(m_context,
                                         m_serial,
                                         mk_language);
    zwp_input_method_context_v1_text_direction(m_context,
                                               m_serial,
                                               mk_text_direction);
}


void ExorKeyboardWayland::inputContextHandleReset(struct zwp_input_method_context_v1 *)
{
    m_preedit_string = QByteArray();
}


void ExorKeyboardWayland::inputContextHandleContentType(struct zwp_input_method_context_v1 *,
                                         uint32_t hint, uint32_t purpose)
{
    m_content_hint = hint;
    m_content_purpose = purpose;

    /* Will report to QtVirtualKeyboard configuration */
}


void ExorKeyboardWayland::inputContextHandleInvokeAction(struct zwp_input_method_context_v1 *,
                                          uint32_t , uint32_t index)
{
    sendPreedit(index);
}


void ExorKeyboardWayland::inputContextHandleSurroundingText(struct zwp_input_method_context_v1 *,
                                             const char *text, uint32_t cursor, uint32_t anchor)
{
    m_surrounding_text = QByteArray(text);
    m_surrounding_cursor = anchor;
}


void ExorKeyboardWayland::commitPreedit()
{
    QByteArray surrounding_text;

    if (m_preedit_string.isEmpty())
        return;

    zwp_input_method_context_v1_cursor_position(m_context,
                                                0, 0);
    zwp_input_method_context_v1_commit_string(m_context,
                                              m_serial,
                                              m_preedit_string);

    if (m_surrounding_text.isEmpty()) {
        m_surrounding_text = m_preedit_string;
        m_surrounding_cursor = m_preedit_string.length();
    } else {
        surrounding_text = m_surrounding_text.insert(m_surrounding_cursor,
                                       m_preedit_string);
        m_surrounding_text = surrounding_text;
        m_surrounding_cursor += m_preedit_string.length();
    }

    m_preedit_string = QByteArray();
}


void ExorKeyboardWayland::sendPreedit(int32_t cursor)
{
    uint32_t index = m_preedit_string.length();

    if (m_preedit_style)
        zwp_input_method_context_v1_preedit_styling(m_context,
                                                    0,
                                                    m_preedit_string.length(),
                                                    m_preedit_style);
    if (cursor > 0)
        index = cursor;

    zwp_input_method_context_v1_preedit_cursor(m_context,
                                               index);
    zwp_input_method_context_v1_preedit_string(m_context,
                                               m_serial,
                                               m_preedit_string,
                                               m_preedit_string);
}


void ExorKeyboardWayland::deleteBeforeCursor()
{
    int start, length;

    if (m_surrounding_text.isEmpty()) {
        /* nothing to do */
        return;
    }

    start = prevUtf8CharIndex(m_surrounding_text, m_surrounding_cursor);
    if (start < 0)
    {
        return;
    }

    length = m_surrounding_cursor - start;

    zwp_input_method_context_v1_delete_surrounding_text(m_context,
                                                        -length,
                                                        length);
    zwp_input_method_context_v1_commit_string(m_context,
                                              m_serial,
                                              "");

    /* Update surrounding text */
    m_surrounding_cursor = (uint32_t) start;
    m_surrounding_text.remove(start, length);
}


void ExorKeyboardWayland::keyPressRelease(uint32_t time_u32, uint32_t sym)
{
    xkb_mod_mask_t mod_mask = 0;
    zwp_input_method_context_v1_keysym(m_context,
                                       m_serial, /* */
                                       time_u32,
                                       sym, WL_KEYBOARD_KEY_STATE_PRESSED, mod_mask);
    time_u32 = (uint32_t) time(NULL);
    zwp_input_method_context_v1_keysym(m_context,
                                       m_serial, /* */
                                       time_u32,
                                       sym, WL_KEYBOARD_KEY_STATE_RELEASED, mod_mask);
}

/*
 * This method will report back to compositor key event information.
 * Changes:
 * - time() is evaluated at key event processing
 * - keyState release event is already filtered
 * - mod_mask will be applied from shift state from Qt VirtualKeyboard.
 *
 */
bool ExorKeyboardWayland::keyEvent(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers)

    uint32_t time_u32 = (uint32_t) time(NULL);

    switch (key) {

    case Qt::Key_Backspace:
        if (m_preedit_string.isEmpty()) {
            deleteBeforeCursor();
        } else {
            m_preedit_string.chop(1);
            //sendPreedit(-1);
        }
        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Return);
        break;

    case Qt::Key_Space:
        m_preedit_string.append(" ");
        commitPreedit();
        break;

    case Qt::Key_Tab:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Tab);
        break;

    case Qt::Key_Up:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Up);
        break;

    case Qt::Key_Left:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Left);
        break;

    case Qt::Key_Right:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Right);
        break;

    case Qt::Key_Down:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Down);
        break;

    case Qt::Key_Shift:
        /* Nothing to do - shift logic is implemented in
         * QtVirtualKeyboard */
        break;

    default:
        m_preedit_string.append(text.toUtf8());
        //sendPreedit(-1);
        commitPreedit(); //REMOVE ?
        break;
    }

    return true;
}

/* Wayland connection */
void ExorKeyboardWayland::waylandConnect()
{
    QPlatformNativeInterface *native = NULL;
    struct wl_display *display = NULL;
    struct wl_registry *registry = NULL;

    /*
     * Native interface obtained from application.
     * Should export from QtWayland client plugin.
     */
    native = QGuiApplication::platformNativeInterface();

    /* Wayland display (from platform) */
    display = (struct wl_display *)
        native->nativeResourceForWindow("display", NULL);

    /* Fail on error */
    if (display == NULL) {
        qCWarning(qExorKeyboardWayland) << "Can't connect to display";
        exit(1);
    }

    /* Register callbacks from global registry */
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_dispatch(display);
    wl_display_roundtrip(display);
}

void ExorKeyboardWayland::activateContext(bool active)
{
    if (m_contextIsActive == active)
        return;

    m_contextIsActive = active;
    emit activationChanged(m_contextIsActive);
}
