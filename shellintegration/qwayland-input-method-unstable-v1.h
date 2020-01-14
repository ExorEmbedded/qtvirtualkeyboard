#ifndef QT_WAYLAND_INPUT_METHOD_UNSTABLE_V1
#define QT_WAYLAND_INPUT_METHOD_UNSTABLE_V1

#include "wayland-input-method-unstable-v1-client-protocol.h"
#include <QByteArray>
#include <QString>

struct wl_registry;

QT_BEGIN_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wmissing-field-initializers")

namespace QtWayland {
    class  zwp_input_method_context_v1
    {
    public:
        zwp_input_method_context_v1(struct ::wl_registry *registry, int id, int version);
        zwp_input_method_context_v1(struct ::zwp_input_method_context_v1 *object);
        zwp_input_method_context_v1();

        virtual ~zwp_input_method_context_v1();

        void init(struct ::wl_registry *registry, int id, int version);
        void init(struct ::zwp_input_method_context_v1 *object);

        struct ::zwp_input_method_context_v1 *object() { return m_zwp_input_method_context_v1; }
        const struct ::zwp_input_method_context_v1 *object() const { return m_zwp_input_method_context_v1; }
        static zwp_input_method_context_v1 *fromObject(struct ::zwp_input_method_context_v1 *object);

        bool isInitialized() const;

        static const struct ::wl_interface *interface();

        void destroy();
        void commit_string(uint32_t serial, const QString &text);
        void preedit_string(uint32_t serial, const QString &text, const QString &commit);
        void preedit_styling(uint32_t index, uint32_t length, uint32_t style);
        void preedit_cursor(int32_t index);
        void delete_surrounding_text(int32_t index, uint32_t length);
        void cursor_position(int32_t index, int32_t anchor);
        void modifiers_map(const QByteArray &map);
        void keysym(uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers);
        struct ::wl_keyboard *grab_keyboard();
        void key(uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
        void modifiers(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
        void language(uint32_t serial, const QString &language);
        void text_direction(uint32_t serial, uint32_t direction);

    protected:
        virtual void zwp_input_method_context_v1_surrounding_text(const QString &text, uint32_t cursor, uint32_t anchor);
        virtual void zwp_input_method_context_v1_reset();
        virtual void zwp_input_method_context_v1_content_type(uint32_t hint, uint32_t purpose);
        virtual void zwp_input_method_context_v1_invoke_action(uint32_t button, uint32_t index);
        virtual void zwp_input_method_context_v1_commit_state(uint32_t serial);
        virtual void zwp_input_method_context_v1_preferred_language(const QString &language);

    private:
        void init_listener();
        static const struct zwp_input_method_context_v1_listener m_zwp_input_method_context_v1_listener;
        static void handle_surrounding_text(
            void *data,
            struct ::zwp_input_method_context_v1 *object,
            const char *text,
            uint32_t cursor,
            uint32_t anchor);
        static void handle_reset(
            void *data,
            struct ::zwp_input_method_context_v1 *object);
        static void handle_content_type(
            void *data,
            struct ::zwp_input_method_context_v1 *object,
            uint32_t hint,
            uint32_t purpose);
        static void handle_invoke_action(
            void *data,
            struct ::zwp_input_method_context_v1 *object,
            uint32_t button,
            uint32_t index);
        static void handle_commit_state(
            void *data,
            struct ::zwp_input_method_context_v1 *object,
            uint32_t serial);
        static void handle_preferred_language(
            void *data,
            struct ::zwp_input_method_context_v1 *object,
            const char *language);
        struct ::zwp_input_method_context_v1 *m_zwp_input_method_context_v1;
    };

    class  zwp_input_method_v1
    {
    public:
        zwp_input_method_v1(struct ::wl_registry *registry, int id, int version);
        zwp_input_method_v1(struct ::zwp_input_method_v1 *object);
        zwp_input_method_v1();

        virtual ~zwp_input_method_v1();

        void init(struct ::wl_registry *registry, int id, int version);
        void init(struct ::zwp_input_method_v1 *object);

        struct ::zwp_input_method_v1 *object() { return m_zwp_input_method_v1; }
        const struct ::zwp_input_method_v1 *object() const { return m_zwp_input_method_v1; }
        static zwp_input_method_v1 *fromObject(struct ::zwp_input_method_v1 *object);

        bool isInitialized() const;

        static const struct ::wl_interface *interface();

    protected:
        virtual void zwp_input_method_v1_activate(struct ::zwp_input_method_context_v1 *id);
        virtual void zwp_input_method_v1_deactivate(struct ::zwp_input_method_context_v1 *context);

    private:
        void init_listener();
        static const struct zwp_input_method_v1_listener m_zwp_input_method_v1_listener;
        static void handle_activate(
            void *data,
            struct ::zwp_input_method_v1 *object,
            struct ::zwp_input_method_context_v1 *id);
        static void handle_deactivate(
            void *data,
            struct ::zwp_input_method_v1 *object,
            struct ::zwp_input_method_context_v1 *context);
        struct ::zwp_input_method_v1 *m_zwp_input_method_v1;
    };

    class  zwp_input_panel_v1
    {
    public:
        zwp_input_panel_v1(struct ::wl_registry *registry, int id, int version);
        zwp_input_panel_v1(struct ::zwp_input_panel_v1 *object);
        zwp_input_panel_v1();

        virtual ~zwp_input_panel_v1();

        void init(struct ::wl_registry *registry, int id, int version);
        void init(struct ::zwp_input_panel_v1 *object);

        struct ::zwp_input_panel_v1 *object() { return m_zwp_input_panel_v1; }
        const struct ::zwp_input_panel_v1 *object() const { return m_zwp_input_panel_v1; }
        static zwp_input_panel_v1 *fromObject(struct ::zwp_input_panel_v1 *object);

        bool isInitialized() const;

        static const struct ::wl_interface *interface();

        struct ::zwp_input_panel_surface_v1 *get_input_panel_surface(struct ::wl_surface *surface);

    private:
        struct ::zwp_input_panel_v1 *m_zwp_input_panel_v1;
    };

    class  zwp_input_panel_surface_v1
    {
    public:
        zwp_input_panel_surface_v1(struct ::wl_registry *registry, int id, int version);
        zwp_input_panel_surface_v1(struct ::zwp_input_panel_surface_v1 *object);
        zwp_input_panel_surface_v1();

        virtual ~zwp_input_panel_surface_v1();

        void init(struct ::wl_registry *registry, int id, int version);
        void init(struct ::zwp_input_panel_surface_v1 *object);

        struct ::zwp_input_panel_surface_v1 *object() { return m_zwp_input_panel_surface_v1; }
        const struct ::zwp_input_panel_surface_v1 *object() const { return m_zwp_input_panel_surface_v1; }
        static zwp_input_panel_surface_v1 *fromObject(struct ::zwp_input_panel_surface_v1 *object);

        bool isInitialized() const;

        static const struct ::wl_interface *interface();

        enum position {
            position_center_bottom = 0,
        };

        void set_toplevel(struct ::wl_output *output, uint32_t position);
        void set_overlay_panel();

    private:
        struct ::zwp_input_panel_surface_v1 *m_zwp_input_panel_surface_v1;
    };
}

QT_WARNING_POP
QT_END_NAMESPACE

#endif
