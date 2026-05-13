options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_live shared public

//! ImGui lifecycle for daslang-live: idempotent init, reload-aware shutdown,
//! pointer roundtripped via the persistent byte store so the C++ ctx (font
//! atlas, window state, active widget, drag-in-progress) survives reload.
//!
//! Mirrors `live/glfw_live` exactly:
//!   - `live_imgui_init(window)` — call from your `init()`. Cold start runs
//!     CreateContext + ImGui_Impl* setup; reload reuses the preserved ctx.
//!   - `live_imgui_shutdown()` — call from your `shutdown()`. Skips during
//!     reload (the [before_reload] handler hands the ctx pointer off
//!     instead); runs only on process exit.

require imgui public
require imgui_app
require glfw/glfw_boost
require daslib/archive
require live_host

let private STORE_KEY = "__imgui_ctx"

var public live_imgui_ctx : ImGuiContext?

def public live_imgui_init(window : GLFWwindow?; glsl_version : string = "#version 330") : ImGuiContext? {
    //! Idempotent. Call from your script's `init()`.
    if (live_imgui_ctx != null) {
        SetCurrentContext(live_imgui_ctx)
        return live_imgui_ctx
    }
    live_imgui_ctx = CreateContext(null)
    StyleColorsDark(null)
    ImGui_ImplGlfw_InitForOpenGL(window, true)
    ImGui_ImplOpenGL3_Init(glsl_version)
    return live_imgui_ctx
}

def public live_imgui_shutdown() {
    //! Reload-aware. Call from your script's `shutdown()`. Skips work
    //! during reload — `is_reload()` tells us the ctx will be reused.
    if (live_imgui_ctx != null && !is_reload()) {
        ImGui_ImplOpenGL3_Shutdown()
        ImGui_ImplGlfw_Shutdown()
        DestroyContext(live_imgui_ctx)
        live_imgui_ctx = null
    }
}

[before_reload]
def private save_imgui_ctx() {
    if (live_imgui_ctx == null) return
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    var ptr_val = unsafe(reinterpret<uint64>(live_imgui_ctx))
    arch |> serialize(ptr_val)
    let data <- ser->extractData()
    live_store_bytes(STORE_KEY, data)
    unsafe { delete ser; }
}

[after_reload]
def private restore_imgui_ctx() {
    var data : array<uint8>
    if (live_load_bytes(STORE_KEY, data) && !empty(data)) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        var ptr_val : uint64
        arch |> serialize(ptr_val)
        unsafe {
            live_imgui_ctx = reinterpret<ImGuiContext?>(ptr_val)
            delete ser
        }
        if (live_imgui_ctx != null) {
            SetCurrentContext(live_imgui_ctx)
        }
    }
}
