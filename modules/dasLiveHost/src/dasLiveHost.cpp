#include "dasLiveHost.h"

namespace das {

// --- State management ---
// Single state instance, owned by the DLL. Host accesses via exported C functions.
static LiveHostState g_state;

// Clear data entries from the store (caller must hold store_mutex).
// Convention: "__live_vars_*" and "__decs_*" are data entries that may contain
// corrupted state. Everything else (window handles, audio handles, etc.) is
// infrastructure and must survive.
static void clear_store_data_locked() {
    for (auto it = g_state.store.begin(); it != g_state.store.end(); ) {
        const auto & key = it->first;
        if (key.compare(0, 12, "__live_vars_") == 0 ||
            key.compare(0, 7, "__decs_") == 0) {
            it = g_state.store.erase(it);
        } else {
            ++it;
        }
    }
}

// Exported C functions for the host executable to access state.
// These cross the DLL boundary safely because they use only POD types.
extern "C" {
    DAS_EXPORT_DLL bool live_host_exit_requested()      { return g_state.exit_requested; }
    DAS_EXPORT_DLL bool live_host_reload_requested()    { return g_state.reload_requested; }
    DAS_EXPORT_DLL bool live_host_full_reload()         { return g_state.full_reload; }
    DAS_EXPORT_DLL bool live_host_is_paused()           { return g_state.paused; }
    DAS_EXPORT_DLL bool live_host_files_changed()       { return g_state.files_changed; }

    DAS_EXPORT_DLL void live_host_set_live_mode(bool v)  { g_state.live_mode = v; }

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
    DAS_EXPORT_DLL void live_host_clear_live_vars() {
        lock_guard<mutex> lock(g_state.store_mutex);
        // Only clear @live variable entries (prefix "__live_vars_"),
        // preserving infrastructure keys like "__glfw_window", "__decs_*"
        for (auto it = g_state.store.begin(); it != g_state.store.end(); ) {
            if (it->first.compare(0, 12, "__live_vars_") == 0) {
                it = g_state.store.erase(it);
            } else {
                ++it;
            }
        }
    }
    DAS_EXPORT_DLL void live_host_clear_store() {
        lock_guard<mutex> lock(g_state.store_mutex);
        clear_store_data_locked();
    }
    DAS_EXPORT_DLL bool live_host_is_context_dead()    { return g_state.context_dead; }
    DAS_EXPORT_DLL void live_host_clear_context_dead() { g_state.context_dead = false; }
    DAS_EXPORT_DLL void live_host_set_auth_token(const char * token) {
        g_state.auth_token = token ? token : "";
    }
    DAS_EXPORT_DLL const char * live_host_get_auth_token() {
        return g_state.auth_token.empty() ? nullptr : g_state.auth_token.c_str();
    }
    DAS_EXPORT_DLL void live_host_clear_error() {
        g_state.last_error.clear();
    }
    DAS_EXPORT_DLL void live_host_set_last_error(const char * err) {
        g_state.last_error = err ? err : "";
    }

    // Called by host to set watched files (script + dependencies)
    DAS_EXPORT_DLL void live_host_set_watched_files(const char ** files, int count) {
        g_state.watched_files.clear();
        for (int i = 0; i < count; i++) {
            if (files[i]) g_state.watched_files.push_back(files[i]);
        }
    }

    // Called by host after compile to set up command dispatch bridge
    DAS_EXPORT_DLL void live_host_set_dispatch_context(Context * ctx) {
        g_state.dispatch_context = ctx;
        g_state.dispatch_fn = nullptr;
        if (!ctx) return;
        auto fns = ctx->findFunctions("live_dispatch_command");
        if (fns.empty()) {
            // Function not found — script may not require live_commands
            return;
        }
        for (auto fn : fns) {
            g_state.dispatch_fn = fn;
            break;
        }
    }
}

// --- Lifecycle functions exposed to daScript ---

bool live_is_live_mode() {
    return g_state.live_mode;
}

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
    if (!g_state.live_mode) {
        // Standalone mode: compute dt from elapsed time
        auto now = std::chrono::steady_clock::now();
        if (!g_state.time_initialized) {
            g_state.last_time = now;
            g_state.time_initialized = true;
            return 0.0f;
        }
        float dt = std::chrono::duration<float>(now - g_state.last_time).count();
        g_state.last_time = now;
        g_state.dt = dt;
        g_state.uptime += dt;
    }
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

void live_set_paused(bool v) {
    g_state.paused = v;
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

// --- Command dispatch bridge ---
// Called from the live_api agent context. Since the HV handler runs on the main
// thread during tick(), the main context is idle and safe to call into.

const char * live_dispatch_command_via_host(const char * cmd_json, const char * auth_token, Context * callerCtx) {
    if (!g_state.auth_token.empty()) {
        if (!auth_token || g_state.auth_token != auth_token) {
            return callerCtx->allocateString("{\"error\": \"unauthorized\"}", nullptr);
        }
    }
    if (!g_state.dispatch_context || !g_state.dispatch_fn || !cmd_json) {
        return callerCtx->allocateString("{\"error\": \"no dispatch context\"}", nullptr);
    }
    // Allocate the command string in the main context
    auto * mainCtx = g_state.dispatch_context;
    auto * cmdStr = mainCtx->allocateString(cmd_json, nullptr);
    // Call live_dispatch_command(cmd_json) in the main context
    vec4f args[1];
    args[0] = cast<char *>::from(cmdStr);
    auto result = mainCtx->evalWithCatch(g_state.dispatch_fn, args);
    if (auto ex = mainCtx->getException()) {
        string msg = string("EXCEPTION in live_command: ") + ex + " at " + mainCtx->exceptionAt.describe();
        g_state.last_error = msg;
        g_state.paused = true;
        g_state.context_dead = true;
        {
            lock_guard<mutex> lock(g_state.store_mutex);
            clear_store_data_locked();
        }
        g_state.dispatch_context = nullptr;
        g_state.dispatch_fn = nullptr;
        string err = string("{\"error\": \"") + msg + "\"}";
        return callerCtx->allocateString(err, nullptr);
    }
    // Clone result string to caller's context (main ctx string may be GC'd)
    auto * resultStr = cast<char *>::to(result);
    if (!resultStr) return callerCtx->allocateString("{\"ok\": true}", nullptr);
    return callerCtx->allocateString(resultStr, nullptr);
}

// --- Watched files ---

int32_t live_get_watched_file_count() {
    return int32_t(g_state.watched_files.size());
}

const char * live_get_watched_file(int32_t index, Context * ctx) {
    if (index < 0 || index >= int32_t(g_state.watched_files.size())) return nullptr;
    return ctx->allocateString(g_state.watched_files[index], nullptr);
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

struct BeforeUpdateAnnotation : FunctionAnnotation {
    BeforeUpdateAnnotation() : FunctionAnnotation("before_update") {}
    virtual bool apply(const FunctionPtr & func, ModuleGroup &,
                       const AnnotationArgumentList &, string &) override {
        func->exports = true;
        func->name = "__before_update_" + func->name;
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
        addAnnotation(new BeforeReloadAnnotation());
        addAnnotation(new AfterReloadAnnotation());
        addAnnotation(new BeforeUpdateAnnotation());

        // Mode
        addExtern<DAS_BIND_FUN(live_is_live_mode)>(*this, lib, "is_live_mode",
            SideEffects::accessGlobal, "das::live_is_live_mode");

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
        addExtern<DAS_BIND_FUN(live_set_paused)>(*this, lib, "set_paused",
            SideEffects::modifyExternal, "das::live_set_paused")
                ->args({"paused"});
        addExtern<DAS_BIND_FUN(live_get_last_error)>(*this, lib, "get_last_error",
            SideEffects::accessGlobal, "das::live_get_last_error");

        // File watcher
        addExtern<DAS_BIND_FUN(live_signal_files_changed)>(*this, lib, "signal_files_changed",
            SideEffects::modifyExternal, "das::live_signal_files_changed");
        addExtern<DAS_BIND_FUN(live_get_watched_file_count)>(*this, lib, "get_watched_file_count",
            SideEffects::accessGlobal, "das::live_get_watched_file_count");
        addExtern<DAS_BIND_FUN(live_get_watched_file)>(*this, lib, "get_watched_file",
            SideEffects::accessGlobal, "das::live_get_watched_file")
                ->args({"index", "context"});

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

        // Command dispatch bridge (called from live_api agent, dispatches in main context)
        addExtern<DAS_BIND_FUN(live_dispatch_command_via_host)>(*this, lib, "dispatch_command",
            SideEffects::modifyExternal, "das::live_dispatch_command_via_host")
                ->args({"command_json", "auth_token", "context"});
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
