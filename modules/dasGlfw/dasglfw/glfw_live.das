options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module glfw_live shared public

//! GLFW window lifecycle for daslang-live.
//!
//! Provides window creation, frame management, and automatic window
//! preservation across live reloads. The GLFW window pointer is stored
//! in the persistent byte store so it survives recompilation.
//!
//! Usage in your script:
//!   require live/glfw_live
//!   [export] def init() { live_create_window("My App", 1280, 720) }
//!   [export] def update() { if (live_begin_frame()) { /* render */ ; live_end_frame() } }
//!   [export] def shutdown() { live_destroy_window() }

require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_cache
require daslib/archive
require live_host

var public live_window : GLFWwindow?

def public live_create_window(title : string; width, height : int) : GLFWwindow? {
    //! Creates a GLFW window (or reuses the preserved one on reload).
    //! Call this from init().
    if (live_window != null) {
        // Already have a window (restored from reload)
        return live_window
    }
    if (glfwInit() == 0) {
        panic("glfw_live: can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    glfwWindowHint(int(GLFW_SAMPLES), 4)
    live_window = glfwCreateWindow(width, height, title, null, null)
    if (live_window == null) {
        panic("glfw_live: can't create window")
    }
    glfwMakeContextCurrent(live_window)
    glEnable(GL_MULTISAMPLE)
    init_opengl_cache()
    return live_window
}

def public live_destroy_window() {
    //! Destroys the GLFW window. Call this from shutdown() (but NOT during reload —
    //! the [before_reload] handler preserves the window automatically).
    if (live_window != null && !is_reload()) {
        glfwDestroyWindow(live_window)
        live_window = null
        glfwTerminate()
    }
}

def public live_begin_frame() : bool {
    //! Polls GLFW events and checks if the window should close.
    //! Returns false if the window was closed (signals exit).
    if (live_window == null) {
        return false
    }
    glfwPollEvents()
    if (glfwWindowShouldClose(live_window) != 0) {
        request_exit()
        return false
    }
    return true
}

def public live_end_frame() {
    //! Swaps buffers. Call after rendering.
    if (live_window != null) {
        glfwSwapBuffers(live_window)
    }
}

def public live_get_framebuffer_size(var width, height : int&) {
    //! Gets the current framebuffer size for glViewport.
    if (live_window != null) {
        glfwGetFramebufferSize(live_window, width, height)
    } else {
        width = 0
        height = 0
    }
}

// --- Window preservation across reloads ---

[before_reload]
def save_glfw_window() {
    if (live_window != null) {
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var ptr_val = unsafe(reinterpret<uint64>(live_window))
        arch |> serialize(ptr_val)
        let data <- ser->extractData()
        live_store_bytes("__glfw_window", data)
        unsafe { delete ser; }
    }
}

[after_reload]
def restore_glfw_window() {
    var data : array<uint8>
    if (live_load_bytes("__glfw_window", data) && length(data) > 0) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        var ptr_val : uint64
        arch |> serialize(ptr_val)
        unsafe {
            live_window = reinterpret<GLFWwindow?>(ptr_val)
            delete ser
        }
        if (live_window != null) {
            glfwMakeContextCurrent(live_window)
        }
    }
}
