options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module live_stub shared public

//! Playground shim for daslang-live apps.
//!
//! Lets a daslang-live game/app run under the web playground's
//! init/update/shutdown browser loop with NO live-reload host, NO REST API,
//! and NO jobque. Provides the window + frame-clock API the apps call
//! (live_create_window / live_begin_frame / live_end_frame /
//! live_get_framebuffer_size / live_destroy_window / live_window), the
//! live_host clock/exit helpers (get_dt / get_uptime / is_reload /
//! exit_requested / request_exit / maybe_collect_gc), and a no-op
//! [live_command] so REST command functions still parse (they are dead code
//! on the web). `@live` needs no shim — it is inert free-form metadata when
//! live_vars is not required.
//!
//! Swap an app's `require live/...` + `require live_host` block for a single
//! `require live_stub` (same directory) and it runs in the playground.

require glfw/glfw_boost
require opengl/opengl_cache
require daslib/ast_boost
require math

var public live_window : GLFWwindow?

var private g_exit_requested = false
var private g_start_ticks = ref_time_ticks()
var private g_uptime = 0.0
var private g_dt = 0.0
var private g_last_gc_bytes = 0ul

def public live_create_window(title : string; width, height : int) : GLFWwindow? {
    return live_window if (live_window != null)
    if (glfwInit() == 0) {
        panic("live_stub: can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    live_window = glfwCreateWindow(width, height, title, null, null)
    if (live_window == null) {
        panic("live_stub: can't create window")
    }
    glfwMakeContextCurrent(live_window)
    // No GLFW_SAMPLES hint / glEnable(GL_MULTISAMPLE): GL_MULTISAMPLE is an
    // invalid enum under WebGL2 (MSAA there is per-FBO, not a global toggle).
    // Mirrors the proven gl_* playground window path; opengl_cache the games need.
    init_opengl_cache()
    g_start_ticks = ref_time_ticks()
    g_uptime = 0.0
    g_dt = 0.0
    return live_window
}

def public live_destroy_window() {
    if (live_window != null) {
        glfwDestroyWindow(live_window)
        live_window = null
        glfwTerminate()
    }
}

def public live_begin_frame() : bool {
    return false if (live_window == null)
    glfwPollEvents()
    if (glfwWindowShouldClose(live_window) != 0) {
        g_exit_requested = true
        return false
    }
    let now = float(double(get_time_usec(g_start_ticks)) / 1000000.0lf)
    g_dt = clamp(now - g_uptime, 0.0, 0.1)   // clamp the first frame / tab-stall spikes
    g_uptime = now
    return true
}

def public live_end_frame() {
    if (live_window != null) {
        glfwSwapBuffers(live_window)
    }
    maybe_collect_gc()   // the browser loop does not GC for us; do it per frame
}

def public live_get_framebuffer_size(var width, height : int&) {
    if (live_window != null) {
        glfwGetFramebufferSize(live_window, width, height)
    } else {
        width = 0
        height = 0
    }
}

def public get_dt() : float {
    return g_dt
}

def public get_uptime() : float {
    return g_uptime
}

def public is_reload() : bool {
    return false
}

def public exit_requested() : bool {
    return g_exit_requested
}

def public request_exit() {
    g_exit_requested = true
}

let private GC_GROWTH_BYTES = 4ul * 1024ul * 1024ul   // collect once the live heap grows ~4 MB

def public maybe_collect_gc() {
    // The browser loop never collects for us, and glfw_live's free-ratio heuristic
    // needs heap_total_allocated() (post-64-bit-sweep API). To stay portable we use
    // a growth threshold on the live bytes only: collect when the combined live heap
    // has grown past GC_GROWTH_BYTES since the last collect. Requires options gc +
    // options persistent_heap on the program (heap_collect throws otherwise).
    let used = heap_bytes_allocated() + string_heap_bytes_allocated()
    if (used > g_last_gc_bytes + GC_GROWTH_BYTES) {
        unsafe(heap_collect(true, false))
        g_last_gc_bytes = heap_bytes_allocated() + string_heap_bytes_allocated()
    }
}

// No-op [live_command]: the REST command functions stay defined (harmless dead
// code on the web) but register nothing. Mirrors live_commands' macro shape.
[function_macro(name="live_command")]
class LiveCommandStub : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        return true
    }
}
