#include "dasLiveHost.h"

namespace das {

// --- State management ---
// Single state instance, owned by the DLL. Host accesses via exported C functions.
static LiveHostState g_state;

// Exported C functions for the host executable to access state.
// These cross the DLL boundary safely because they use only POD types.
extern "C" {
    DAS_EXPORT_DLL bool live_host_exit_requested()      { return g_state.exit_requested; }
    DAS_EXPORT_DLL bool live_host_reload_requested()    { return g_state.reload_requested; }
    DAS_EXPORT_DLL bool live_host_full_reload()         { return g_state.full_reload; }
    DAS_EXPORT_DLL bool live_host_is_paused()           { return g_state.paused; }
    DAS_EXPORT_DLL bool live_host_files_changed()       { return g_state.files_changed; }

    DAS_EXPORT_DLL void live_host_set_dt(float v)       { g_state.dt = v; }
    DAS_EXPORT_DLL void live_host_set_uptime(float v)   { g_state.uptime = v; }
    DAS_EXPORT_DLL void live_host_set_fps(float v)      { g_state.fps = v; }
    DAS_EXPORT_DLL void live_host_set_is_reload(bool v) { g_state.is_reload = v; }
    DAS_EXPORT_DLL void live_host_set_paused(bool v)    { g_state.paused = v; }

    DAS_EXPORT_DLL void live_host_clear_reload_flags() {
        g_state.reload_requested = false;
        g_state.full_reload = false;
        g_state.files_changed = false;
    }
    DAS_EXPORT_DLL void live_host_clear_error() {
        g_state.last_error.clear();
    }
}

// --- Lifecycle functions exposed to daScript ---

void live_request_exit() {
    g_state.exit_requested = true;
}

bool live_exit_requested() {
    return g_state.exit_requested;
}

void live_request_reload(bool full) {
    g_state.reload_requested = true;
    if (full) g_state.full_reload = true;
}

bool live_is_reload() {
    return g_state.is_reload;
}

float live_get_dt() {
    return g_state.dt;
}

float live_get_uptime() {
    return g_state.uptime;
}

float live_get_fps() {
    return g_state.fps;
}

bool live_is_paused() {
    return g_state.paused;
}

const char * live_get_last_error(Context * ctx) {
    if (g_state.last_error.empty()) return nullptr;
    return ctx->allocateString(g_state.last_error, nullptr);
}

// --- File watcher support ---

void live_signal_files_changed() {
    g_state.files_changed = true;
}

// --- Persistent store (bytes only) ---

void live_store_bytes(const char * key, const TArray<uint8_t> & data) {
    if (!key) return;
    lock_guard<mutex> lock(g_state.store_mutex);
    auto & entry = g_state.store[key];
    entry.assign(data.data, data.data + data.size);
}

bool live_load_bytes(const char * key, TArray<uint8_t> & data, Context * ctx) {
    if (!key) return false;
    lock_guard<mutex> lock(g_state.store_mutex);
    auto it = g_state.store.find(key);
    if (it == g_state.store.end()) return false;
    auto & src = it->second;
    uint32_t sz = uint32_t(src.size());
    array_resize(*ctx, data, sz, sizeof(uint8_t), false, nullptr);
    if (sz > 0) {
        memcpy(data.data, src.data(), sz);
    }
    return true;
}

// --- GC ---

void live_collect_gc(Context * ctx) {
    ctx->collectHeap(nullptr, /*stringHeap*/true, /*validate*/false);
}

void live_collect_string_gc(Context * ctx) {
    ctx->collectHeap(nullptr, /*stringHeap*/true, /*validate*/false);
}

// --- Annotations ---

struct BeforeReloadAnnotation : FunctionAnnotation {
    BeforeReloadAnnotation() : FunctionAnnotation("before_reload") {}
    virtual bool apply(const FunctionPtr & func, ModuleGroup &,
                       const AnnotationArgumentList &, string &) override {
        func->exports = true;
        func->name = "__before_reload_" + func->name;
        return true;
    }
    virtual bool apply(ExprBlock *, ModuleGroup &,
                       const AnnotationArgumentList &, string & err) override {
        err = "not supported for blocks";
        return false;
    }
    virtual bool finalize(const FunctionPtr &, ModuleGroup &,
                         const AnnotationArgumentList &,
                         const AnnotationArgumentList &, string &) override {
        return true;
    }
    virtual bool finalize(ExprBlock *, ModuleGroup &,
                         const AnnotationArgumentList &,
                         const AnnotationArgumentList &, string &) override {
        return true;
    }
};

struct AfterReloadAnnotation : FunctionAnnotation {
    AfterReloadAnnotation() : FunctionAnnotation("after_reload") {}
    virtual bool apply(const FunctionPtr & func, ModuleGroup &,
                       const AnnotationArgumentList &, string &) override {
        func->exports = true;
        func->name = "__after_reload_" + func->name;
        return true;
    }
    virtual bool apply(ExprBlock *, ModuleGroup &,
                       const AnnotationArgumentList &, string & err) override {
        err = "not supported for blocks";
        return false;
    }
    virtual bool finalize(const FunctionPtr &, ModuleGroup &,
                         const AnnotationArgumentList &,
                         const AnnotationArgumentList &, string &) override {
        return true;
    }
    virtual bool finalize(ExprBlock *, ModuleGroup &,
                         const AnnotationArgumentList &,
                         const AnnotationArgumentList &, string &) override {
        return true;
    }
};

// --- Module ---

class Module_LiveHost : public Module {
public:
    Module_LiveHost() : Module("live_host") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();

        // Annotations
        addAnnotation(make_smart<BeforeReloadAnnotation>());
        addAnnotation(make_smart<AfterReloadAnnotation>());

        // Lifecycle
        addExtern<DAS_BIND_FUN(live_request_exit)>(*this, lib, "request_exit",
            SideEffects::modifyExternal, "das::live_request_exit");
        addExtern<DAS_BIND_FUN(live_exit_requested)>(*this, lib, "exit_requested",
            SideEffects::accessGlobal, "das::live_exit_requested");
        addExtern<DAS_BIND_FUN(live_request_reload)>(*this, lib, "request_reload",
            SideEffects::modifyExternal, "das::live_request_reload")
                ->args({"full"});
        addExtern<DAS_BIND_FUN(live_is_reload)>(*this, lib, "is_reload",
            SideEffects::accessGlobal, "das::live_is_reload");

        // Timing
        addExtern<DAS_BIND_FUN(live_get_dt)>(*this, lib, "get_dt",
            SideEffects::accessGlobal, "das::live_get_dt");
        addExtern<DAS_BIND_FUN(live_get_uptime)>(*this, lib, "get_uptime",
            SideEffects::accessGlobal, "das::live_get_uptime");
        addExtern<DAS_BIND_FUN(live_get_fps)>(*this, lib, "get_fps",
            SideEffects::accessGlobal, "das::live_get_fps");

        // Error state
        addExtern<DAS_BIND_FUN(live_is_paused)>(*this, lib, "is_paused",
            SideEffects::accessGlobal, "das::live_is_paused");
        addExtern<DAS_BIND_FUN(live_get_last_error)>(*this, lib, "get_last_error",
            SideEffects::accessGlobal, "das::live_get_last_error");

        // File watcher
        addExtern<DAS_BIND_FUN(live_signal_files_changed)>(*this, lib, "signal_files_changed",
            SideEffects::modifyExternal, "das::live_signal_files_changed");

        // Persistent store
        addExtern<DAS_BIND_FUN(live_store_bytes)>(*this, lib, "live_store_bytes",
            SideEffects::modifyExternal, "das::live_store_bytes")
                ->args({"key", "data"});
        addExtern<DAS_BIND_FUN(live_load_bytes)>(*this, lib, "live_load_bytes",
            SideEffects::modifyArgumentAndExternal, "das::live_load_bytes")
                ->args({"key", "data", "context"});

        // GC
        addExtern<DAS_BIND_FUN(live_collect_gc)>(*this, lib, "live_collect_gc",
            SideEffects::modifyExternal, "das::live_collect_gc");
        addExtern<DAS_BIND_FUN(live_collect_string_gc)>(*this, lib, "live_collect_string_gc",
            SideEffects::modifyExternal, "das::live_collect_string_gc");
    }

    virtual ModuleAotType aotRequire(TextWriter & tw) const override {
        tw << "#include \"dasLiveHost.h\"\n";
        return ModuleAotType::cpp;
    }
};

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_LiveHost, das);

// REGISTER_DYN_MODULE must use unqualified name (macro creates variable from it)
namespace das {
    REGISTER_DYN_MODULE(Module_LiveHost, Module_LiveHost);
}
