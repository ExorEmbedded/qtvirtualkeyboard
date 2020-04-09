#ifndef EXORKEYBOARDWAYLAND_H
#define EXORKEYBOARDWAYLAND_H

#include <QObject>
class QQmlEngine;
class QJSEngine;

#include <string>

#include <wayland-client.h>
#include "input-method-unstable-v1.h"
#include "text-input-unstable-v1.h"

class ExorKeyboardWayland : public QObject
{
    Q_OBJECT

public:
    static ExorKeyboardWayland* getInstance();
    static QObject* exorKeyboardWaylandProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE bool keyEvent(Qt::Key, const QString &, Qt::KeyboardModifiers);

    /* Wayland registry handler */
    void globalHandler(struct wl_registry *wl_registry, uint32_t name,
                        const char *interface, uint32_t version);

    /* Wayland InputMethod callbacks */
    void inputMethodHandleActivate(struct zwp_input_method_v1 *input_method,
                             struct zwp_input_method_context_v1 *context);

    void inputMethodHandleDeactivate(struct zwp_input_method_v1 *input_method,
                               struct zwp_input_method_context_v1 *context);

    /* Wayland InputMethodContext callbacks */
    void inputContextHandlePreferredLanguage(struct zwp_input_method_context_v1 *,
                                 const char *language);
    void inputContextHandleCommitState(struct zwp_input_method_context_v1 *,
                           uint32_t serial);
    void inputContextHandleReset(struct zwp_input_method_context_v1 *);
    void inputContextHandleContentType(struct zwp_input_method_context_v1 *,
                           uint32_t hint, uint32_t purpose);
    void inputContextHandleInvokeAction(struct zwp_input_method_context_v1 *,
                            uint32_t button, uint32_t index);
    void inputContextHandleSurroundingText(struct zwp_input_method_context_v1 *,
                               const char *text, uint32_t cursor, uint32_t anchor);

    /* Wayland InputMethodContext methods */
    void commitPreedit();
    void sendPreedit(int32_t cursor);
    void deleteBeforeCursor();

signals:
    void activationChanged(bool active);

private:
    explicit ExorKeyboardWayland(QObject *parent = nullptr);

    /* Wayland */
    void waylandConnect();

    struct wl_compositor *m_compositor = NULL;
    struct wl_registry *m_registry = NULL;
    struct zwp_input_method_v1 *m_input_method = NULL;
    struct zwp_input_method_context_v1 *m_context = NULL;

    /* InputMethod Context status */
    void activateContext(bool active);

    bool m_contextIsActive;
    uint32_t m_serial;
    uint32_t m_content_hint;
    uint32_t m_content_purpose;
    uint32_t m_preedit_style;
    char *m_preedit_string = NULL;
    char *m_preferred_language = NULL;
    char *m_surrounding_text = NULL;
    uint32_t m_surrounding_cursor;

    /* Keyboard hints */
    std::string mk_language;
    uint32_t mk_text_direction;

    void keyPressRelease(uint32_t time_u32, uint32_t sym);
};

#endif // EXORKEYBOARDWAYLAND_H
