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

Q_LOGGING_CATEGORY(qExorKeyboardWayland, "exor.keyboard.wayland")

/* ************************************************************** */

/*
 * The following text utilities have been imported from Weston client keyboard.
 * They are kept as plain C static routines and they manage the input context.
 */
static const char *
prev_utf8_char(const char *s, const char *p)
{
        for (--p; p >= s; --p) {
                if ((*p & 0xc0) != 0x80)
                        return p;
        }
        return NULL;
}

static char *
insert_text(const char *text, uint32_t offset, const char *insert)
{
    int tlen = strlen(text), ilen = strlen(insert);
    char *new_text = (char*) malloc(tlen + ilen + 1);

    memcpy(new_text, text, offset);
    memcpy(new_text + offset, insert, ilen);
    memcpy(new_text + offset + ilen, text + offset, tlen - offset);
    new_text[tlen + ilen] = '\0';

    return new_text;
}

static char *
append(char *s1, const char *s2)
{
        int len1, len2;
        char *s;

        len1 = strlen(s1);
        len2 = strlen(s2);
        s = (char *) realloc(s1, len1 + len2 + 1);
        memcpy(s + len1, s2, len2);
        s[len1 + len2] = '\0';

        return s;
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
    m_preedit_string = strdup("");
    m_preferred_language = strdup(mk_language.c_str());
}


void ExorKeyboardWayland::globalHandler(struct wl_registry *wl_registry, uint32_t name,
                                     const char *interface, uint32_t version)
{
    qCDebug(qExorKeyboardWayland) << "Wayland: interface [" << name << "] " << interface << " (v" << version << ")";

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
    qCDebug(qExorKeyboardWayland) << "Input method activation request";

    if (m_context) {
        zwp_input_method_context_v1_destroy(m_context);
    }

    if (m_preedit_string)
        free(m_preedit_string);

    m_preedit_string = strdup("");
    m_content_hint = 0;
    m_content_purpose = 0;
    free(m_preferred_language);
    m_preferred_language = NULL;
    free(m_surrounding_text);
    m_surrounding_text = NULL;

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
                                         mk_language.c_str());
    zwp_input_method_context_v1_text_direction(m_context,
                                               m_serial,
                                               mk_text_direction);

    /* Change activation status -will notify with signal- */
    activateContext(true);
}


void ExorKeyboardWayland::inputMethodHandleDeactivate(struct zwp_input_method_v1 *,
                                             struct zwp_input_method_context_v1 *)
{
    qCDebug(qExorKeyboardWayland) << "Input method deactivation request";

    /* Change activation status -will notify with signal- */
    activateContext(false);
}


void ExorKeyboardWayland::inputContextHandlePreferredLanguage(struct zwp_input_method_context_v1 *,
                                               const char *language)
{
    if (m_preferred_language)
        free(m_preferred_language);
    m_preferred_language = NULL;
    if (language) {
        m_preferred_language = strdup(language);
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
                                         mk_language.c_str());
    zwp_input_method_context_v1_text_direction(m_context,
                                               m_serial,
                                               mk_text_direction);
}


void ExorKeyboardWayland::inputContextHandleReset(struct zwp_input_method_context_v1 *)
{
    if (strlen(m_preedit_string)) {
        free(m_preedit_string);
        m_preedit_string = strdup("");
    }
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
                                             const char *text, uint32_t cursor, uint32_t )
{
    free(m_surrounding_text);
    m_surrounding_text = strdup(text);
    m_surrounding_cursor = cursor;
}


void ExorKeyboardWayland::commitPreedit()
{
    char *surrounding_text;

    if (!m_preedit_string ||
            strlen(m_preedit_string) == 0)
        return;

    zwp_input_method_context_v1_cursor_position(m_context,
                                                0, 0);
    zwp_input_method_context_v1_commit_string(m_context,
                                              m_serial,
                                              m_preedit_string);

    if (m_surrounding_text) {
        surrounding_text = insert_text(m_surrounding_text,
                                       m_surrounding_cursor,
                                       m_preedit_string);
        free(m_surrounding_text);
        m_surrounding_text = surrounding_text;
        m_surrounding_cursor += strlen(m_preedit_string);
    } else {
        m_surrounding_text = strdup(m_preedit_string);
        m_surrounding_cursor = strlen(m_preedit_string);
    }

    free(m_preedit_string);
    m_preedit_string = strdup("");
}


void ExorKeyboardWayland::sendPreedit(int32_t cursor)
{
    uint32_t index = strlen(m_preedit_string);

    if (m_preedit_style)
        zwp_input_method_context_v1_preedit_styling(m_context,
                                                    0,
                                                    strlen(m_preedit_string),
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
    /* May need QString */
    const char *start, *end;

    if (!m_surrounding_text) {
        /* nothing to do */
        return;
    }

    start = prev_utf8_char(m_surrounding_text,
                           m_surrounding_text + m_surrounding_cursor);
    if (!start) {
        /* nothing to do */
        return;
    }

    end = m_surrounding_text + m_surrounding_cursor;

    zwp_input_method_context_v1_delete_surrounding_text(m_context,
                                                        (start - m_surrounding_text) - m_surrounding_cursor,
                                                        end - start);
    zwp_input_method_context_v1_commit_string(m_context,
                                              m_serial,
                                              "");

    /* Update surrounding text */
    m_surrounding_cursor = start - m_surrounding_text;
    m_surrounding_text[m_surrounding_cursor] = '\0';
    if (*end)
        memmove(m_surrounding_text + m_surrounding_cursor, end, strlen(end));
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

    qCDebug(qExorKeyboardWayland) << Q_FUNC_INFO
             << "keyEvent " << key << " "
             << text << " " << modifiers;

    const char *label = text.toStdString().c_str();
    uint32_t time_u32 = (uint32_t) time(NULL);

    switch (key) {

    case Qt::Key_Backspace:
        if (strlen(m_preedit_string) == 0) {
            deleteBeforeCursor();
        } else {
            m_preedit_string[strlen(m_preedit_string) - 1] = '\0';
            sendPreedit(-1);
        }
        break;


    case Qt::Key_Enter:
        commitPreedit();
        keyPressRelease(time_u32, XKB_KEY_Return);
        break;

    case Qt::Key_Space:
        m_preedit_string = append(m_preedit_string, " ");
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
        m_preedit_string = append(m_preedit_string, label);
        sendPreedit(-1);
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
        fprintf(stderr, "Can't connect to display\n");
        exit(1);
    }

    qCDebug(qExorKeyboardWayland) << "Connected to display: " << wl_display_get_fd(display);

    /* Register callbacks from global registry */
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_dispatch(display);
    wl_display_roundtrip(display);
}

 void ExorKeyboardWayland::activateContext(bool active)
 {
     qCDebug(qExorKeyboardWayland) << Q_FUNC_INFO
              << "active " << active;

     if (m_contextIsActive == active)
         return;

     m_contextIsActive = active;
     emit activationChanged(m_contextIsActive);
 }
