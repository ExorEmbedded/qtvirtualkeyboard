#include "qwayland-input-method-unstable-v1.h"

QT_BEGIN_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wmissing-field-initializers")

namespace QtWayland {

static inline void *wlRegistryBind(struct ::wl_registry *registry, uint32_t name, const struct ::wl_interface *interface, uint32_t version)
{
    const uint32_t bindOpCode = 0;
#if (WAYLAND_VERSION_MAJOR == 1 && WAYLAND_VERSION_MINOR > 10) || WAYLAND_VERSION_MAJOR > 1
    return (void *) wl_proxy_marshal_constructor_versioned((struct wl_proxy *) registry,
        bindOpCode, interface, version, name, interface->name, version, nullptr);
#else
    return (void *) wl_proxy_marshal_constructor((struct wl_proxy *) registry,
        bindOpCode, interface, name, interface->name, version, nullptr);
#endif
}

    zwp_input_method_context_v1::zwp_input_method_context_v1(struct ::wl_registry *registry, int id, int version)
    {
        init(registry, id, version);
    }

    zwp_input_method_context_v1::zwp_input_method_context_v1(struct ::zwp_input_method_context_v1 *obj)
        : m_zwp_input_method_context_v1(obj)
    {
        init_listener();
    }

    zwp_input_method_context_v1::zwp_input_method_context_v1()
        : m_zwp_input_method_context_v1(nullptr)
    {
    }

    zwp_input_method_context_v1::~zwp_input_method_context_v1()
    {
    }

    void zwp_input_method_context_v1::init(struct ::wl_registry *registry, int id, int version)
    {
        m_zwp_input_method_context_v1 = static_cast<struct ::zwp_input_method_context_v1 *>(wlRegistryBind(registry, id, &zwp_input_method_context_v1_interface, version));
        init_listener();
    }

    void zwp_input_method_context_v1::init(struct ::zwp_input_method_context_v1 *obj)
    {
        m_zwp_input_method_context_v1 = obj;
        init_listener();
    }

    zwp_input_method_context_v1 *zwp_input_method_context_v1::fromObject(struct ::zwp_input_method_context_v1 *object)
    {
        if (wl_proxy_get_listener((struct ::wl_proxy *)object) != (void *)&m_zwp_input_method_context_v1_listener)
            return nullptr;
        return static_cast<zwp_input_method_context_v1 *>(zwp_input_method_context_v1_get_user_data(object));
    }

    bool zwp_input_method_context_v1::isInitialized() const
    {
        return m_zwp_input_method_context_v1 != nullptr;
    }

    const struct wl_interface *zwp_input_method_context_v1::interface()
    {
        return &::zwp_input_method_context_v1_interface;
    }

    void zwp_input_method_context_v1::destroy()
    {
        zwp_input_method_context_v1_destroy(
            m_zwp_input_method_context_v1);
        m_zwp_input_method_context_v1 = nullptr;
    }

    void zwp_input_method_context_v1::commit_string(uint32_t serial, const QString &text)
    {
        zwp_input_method_context_v1_commit_string(
            m_zwp_input_method_context_v1,
            serial,
            text.toUtf8().constData());
    }

    void zwp_input_method_context_v1::preedit_string(uint32_t serial, const QString &text, const QString &commit)
    {
        zwp_input_method_context_v1_preedit_string(
            m_zwp_input_method_context_v1,
            serial,
            text.toUtf8().constData(),
            commit.toUtf8().constData());
    }

    void zwp_input_method_context_v1::preedit_styling(uint32_t index, uint32_t length, uint32_t style)
    {
        zwp_input_method_context_v1_preedit_styling(
            m_zwp_input_method_context_v1,
            index,
            length,
            style);
    }

    void zwp_input_method_context_v1::preedit_cursor(int32_t index)
    {
        zwp_input_method_context_v1_preedit_cursor(
            m_zwp_input_method_context_v1,
            index);
    }

    void zwp_input_method_context_v1::delete_surrounding_text(int32_t index, uint32_t length)
    {
        zwp_input_method_context_v1_delete_surrounding_text(
            m_zwp_input_method_context_v1,
            index,
            length);
    }

    void zwp_input_method_context_v1::cursor_position(int32_t index, int32_t anchor)
    {
        zwp_input_method_context_v1_cursor_position(
            m_zwp_input_method_context_v1,
            index,
            anchor);
    }

    void zwp_input_method_context_v1::modifiers_map(const QByteArray &map)
    {
        struct wl_array map_data;
        map_data.size = map.size();
        map_data.data = static_cast<void *>(const_cast<char *>(map.constData()));
        map_data.alloc = 0;

        zwp_input_method_context_v1_modifiers_map(
            m_zwp_input_method_context_v1,
            &map_data);
    }

    void zwp_input_method_context_v1::keysym(uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers)
    {
        zwp_input_method_context_v1_keysym(
            m_zwp_input_method_context_v1,
            serial,
            time,
            sym,
            state,
            modifiers);
    }

    struct ::wl_keyboard *zwp_input_method_context_v1::grab_keyboard()
    {
        return zwp_input_method_context_v1_grab_keyboard(
            m_zwp_input_method_context_v1);
    }

    void zwp_input_method_context_v1::key(uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
    {
        zwp_input_method_context_v1_key(
            m_zwp_input_method_context_v1,
            serial,
            time,
            key,
            state);
    }

    void zwp_input_method_context_v1::modifiers(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
    {
        zwp_input_method_context_v1_modifiers(
            m_zwp_input_method_context_v1,
            serial,
            mods_depressed,
            mods_latched,
            mods_locked,
            group);
    }

    void zwp_input_method_context_v1::language(uint32_t serial, const QString &language)
    {
        zwp_input_method_context_v1_language(
            m_zwp_input_method_context_v1,
            serial,
            language.toUtf8().constData());
    }

    void zwp_input_method_context_v1::text_direction(uint32_t serial, uint32_t direction)
    {
        zwp_input_method_context_v1_text_direction(
            m_zwp_input_method_context_v1,
            serial,
            direction);
    }

    void zwp_input_method_context_v1::zwp_input_method_context_v1_surrounding_text(const QString &, uint32_t , uint32_t )
    {
    }

    void zwp_input_method_context_v1::handle_surrounding_text(
        void *data,
        struct ::zwp_input_method_context_v1 *object,
        const char *text,
        uint32_t cursor,
        uint32_t anchor)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_context_v1 *>(data)->zwp_input_method_context_v1_surrounding_text(
            QString::fromUtf8(text),
            cursor,
            anchor);
    }

    void zwp_input_method_context_v1::zwp_input_method_context_v1_reset()
    {
    }

    void zwp_input_method_context_v1::handle_reset(
        void *data,
        struct ::zwp_input_method_context_v1 *object)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_context_v1 *>(data)->zwp_input_method_context_v1_reset();
    }

    void zwp_input_method_context_v1::zwp_input_method_context_v1_content_type(uint32_t , uint32_t )
    {
    }

    void zwp_input_method_context_v1::handle_content_type(
        void *data,
        struct ::zwp_input_method_context_v1 *object,
        uint32_t hint,
        uint32_t purpose)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_context_v1 *>(data)->zwp_input_method_context_v1_content_type(
            hint,
            purpose);
    }

    void zwp_input_method_context_v1::zwp_input_method_context_v1_invoke_action(uint32_t , uint32_t )
    {
    }

    void zwp_input_method_context_v1::handle_invoke_action(
        void *data,
        struct ::zwp_input_method_context_v1 *object,
        uint32_t button,
        uint32_t index)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_context_v1 *>(data)->zwp_input_method_context_v1_invoke_action(
            button,
            index);
    }

    void zwp_input_method_context_v1::zwp_input_method_context_v1_commit_state(uint32_t )
    {
    }

    void zwp_input_method_context_v1::handle_commit_state(
        void *data,
        struct ::zwp_input_method_context_v1 *object,
        uint32_t serial)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_context_v1 *>(data)->zwp_input_method_context_v1_commit_state(
            serial);
    }

    void zwp_input_method_context_v1::zwp_input_method_context_v1_preferred_language(const QString &)
    {
    }

    void zwp_input_method_context_v1::handle_preferred_language(
        void *data,
        struct ::zwp_input_method_context_v1 *object,
        const char *language)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_context_v1 *>(data)->zwp_input_method_context_v1_preferred_language(
            QString::fromUtf8(language));
    }

    const struct zwp_input_method_context_v1_listener zwp_input_method_context_v1::m_zwp_input_method_context_v1_listener = {
        zwp_input_method_context_v1::handle_surrounding_text,
        zwp_input_method_context_v1::handle_reset,
        zwp_input_method_context_v1::handle_content_type,
        zwp_input_method_context_v1::handle_invoke_action,
        zwp_input_method_context_v1::handle_commit_state,
        zwp_input_method_context_v1::handle_preferred_language,
    };

    void zwp_input_method_context_v1::init_listener()
    {
        zwp_input_method_context_v1_add_listener(m_zwp_input_method_context_v1, &m_zwp_input_method_context_v1_listener, this);
    }

    zwp_input_method_v1::zwp_input_method_v1(struct ::wl_registry *registry, int id, int version)
    {
        init(registry, id, version);
    }

    zwp_input_method_v1::zwp_input_method_v1(struct ::zwp_input_method_v1 *obj)
        : m_zwp_input_method_v1(obj)
    {
        init_listener();
    }

    zwp_input_method_v1::zwp_input_method_v1()
        : m_zwp_input_method_v1(nullptr)
    {
    }

    zwp_input_method_v1::~zwp_input_method_v1()
    {
    }

    void zwp_input_method_v1::init(struct ::wl_registry *registry, int id, int version)
    {
        m_zwp_input_method_v1 = static_cast<struct ::zwp_input_method_v1 *>(wlRegistryBind(registry, id, &zwp_input_method_v1_interface, version));
        init_listener();
    }

    void zwp_input_method_v1::init(struct ::zwp_input_method_v1 *obj)
    {
        m_zwp_input_method_v1 = obj;
        init_listener();
    }

    zwp_input_method_v1 *zwp_input_method_v1::fromObject(struct ::zwp_input_method_v1 *object)
    {
        if (wl_proxy_get_listener((struct ::wl_proxy *)object) != (void *)&m_zwp_input_method_v1_listener)
            return nullptr;
        return static_cast<zwp_input_method_v1 *>(zwp_input_method_v1_get_user_data(object));
    }

    bool zwp_input_method_v1::isInitialized() const
    {
        return m_zwp_input_method_v1 != nullptr;
    }

    const struct wl_interface *zwp_input_method_v1::interface()
    {
        return &::zwp_input_method_v1_interface;
    }

    void zwp_input_method_v1::zwp_input_method_v1_activate(struct ::zwp_input_method_context_v1 *)
    {
    }

    void zwp_input_method_v1::handle_activate(
        void *data,
        struct ::zwp_input_method_v1 *object,
        struct ::zwp_input_method_context_v1 *id)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_v1 *>(data)->zwp_input_method_v1_activate(
            id);
    }

    void zwp_input_method_v1::zwp_input_method_v1_deactivate(struct ::zwp_input_method_context_v1 *)
    {
    }

    void zwp_input_method_v1::handle_deactivate(
        void *data,
        struct ::zwp_input_method_v1 *object,
        struct ::zwp_input_method_context_v1 *context)
    {
        Q_UNUSED(object);
        static_cast<zwp_input_method_v1 *>(data)->zwp_input_method_v1_deactivate(
            context);
    }

    const struct zwp_input_method_v1_listener zwp_input_method_v1::m_zwp_input_method_v1_listener = {
        zwp_input_method_v1::handle_activate,
        zwp_input_method_v1::handle_deactivate,
    };

    void zwp_input_method_v1::init_listener()
    {
        zwp_input_method_v1_add_listener(m_zwp_input_method_v1, &m_zwp_input_method_v1_listener, this);
    }

    zwp_input_panel_v1::zwp_input_panel_v1(struct ::wl_registry *registry, int id, int version)
    {
        init(registry, id, version);
    }

    zwp_input_panel_v1::zwp_input_panel_v1(struct ::zwp_input_panel_v1 *obj)
        : m_zwp_input_panel_v1(obj)
    {
    }

    zwp_input_panel_v1::zwp_input_panel_v1()
        : m_zwp_input_panel_v1(nullptr)
    {
    }

    zwp_input_panel_v1::~zwp_input_panel_v1()
    {
    }

    void zwp_input_panel_v1::init(struct ::wl_registry *registry, int id, int version)
    {
        m_zwp_input_panel_v1 = static_cast<struct ::zwp_input_panel_v1 *>(wlRegistryBind(registry, id, &zwp_input_panel_v1_interface, version));
    }

    void zwp_input_panel_v1::init(struct ::zwp_input_panel_v1 *obj)
    {
        m_zwp_input_panel_v1 = obj;
    }

    zwp_input_panel_v1 *zwp_input_panel_v1::fromObject(struct ::zwp_input_panel_v1 *object)
    {
        return static_cast<zwp_input_panel_v1 *>(zwp_input_panel_v1_get_user_data(object));
    }

    bool zwp_input_panel_v1::isInitialized() const
    {
        return m_zwp_input_panel_v1 != nullptr;
    }

    const struct wl_interface *zwp_input_panel_v1::interface()
    {
        return &::zwp_input_panel_v1_interface;
    }

    struct ::zwp_input_panel_surface_v1 *zwp_input_panel_v1::get_input_panel_surface(struct ::wl_surface *surface)
    {
        return zwp_input_panel_v1_get_input_panel_surface(
            m_zwp_input_panel_v1,
            surface);
    }

    zwp_input_panel_surface_v1::zwp_input_panel_surface_v1(struct ::wl_registry *registry, int id, int version)
    {
        init(registry, id, version);
    }

    zwp_input_panel_surface_v1::zwp_input_panel_surface_v1(struct ::zwp_input_panel_surface_v1 *obj)
        : m_zwp_input_panel_surface_v1(obj)
    {
    }

    zwp_input_panel_surface_v1::zwp_input_panel_surface_v1()
        : m_zwp_input_panel_surface_v1(nullptr)
    {
    }

    zwp_input_panel_surface_v1::~zwp_input_panel_surface_v1()
    {
    }

    void zwp_input_panel_surface_v1::init(struct ::wl_registry *registry, int id, int version)
    {
        m_zwp_input_panel_surface_v1 = static_cast<struct ::zwp_input_panel_surface_v1 *>(wlRegistryBind(registry, id, &zwp_input_panel_surface_v1_interface, version));
    }

    void zwp_input_panel_surface_v1::init(struct ::zwp_input_panel_surface_v1 *obj)
    {
        m_zwp_input_panel_surface_v1 = obj;
    }

    zwp_input_panel_surface_v1 *zwp_input_panel_surface_v1::fromObject(struct ::zwp_input_panel_surface_v1 *object)
    {
        return static_cast<zwp_input_panel_surface_v1 *>(zwp_input_panel_surface_v1_get_user_data(object));
    }

    bool zwp_input_panel_surface_v1::isInitialized() const
    {
        return m_zwp_input_panel_surface_v1 != nullptr;
    }

    const struct wl_interface *zwp_input_panel_surface_v1::interface()
    {
        return &::zwp_input_panel_surface_v1_interface;
    }

    void zwp_input_panel_surface_v1::set_toplevel(struct ::wl_output *output, uint32_t position)
    {
        zwp_input_panel_surface_v1_set_toplevel(
            m_zwp_input_panel_surface_v1,
            output,
            position);
    }

    void zwp_input_panel_surface_v1::set_overlay_panel()
    {
        zwp_input_panel_surface_v1_set_overlay_panel(
            m_zwp_input_panel_surface_v1);
    }
}

QT_WARNING_POP
QT_END_NAMESPACE
