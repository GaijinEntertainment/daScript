// GLFW 3.4+ and input-method entry points missing from emscripten's -sUSE_GLFW=3 JS library
// (src/lib/libglfw.js — a GLFW 3.3-era subset; absence of every function below
// verified against emsdk 5.0.7, the exact pin in .github/workflows/pages.yml).
// The daslang binding registers the full patched surface unconditionally so the
// das-visible module is identical on desktop and web: a host-cross-compiled
// card resolves every referenced extern by mangled name at startup
// (jit_init_extern_function), and a registration missing on the wasm side is a
// startup fatal ("Failed to find @glfw::... in module glfw"). Each stub is the
// most honest web behavior available; one line per stub names the JS-side gap.

#ifndef __EMSCRIPTEN__
#error "glfw_emscripten_stubs.c is web-only: it backfills GLFW 3.4+ and input-method entry points absent from emcc's -sUSE_GLFW=3 JS library. Desktop builds link the patched GLFW 3.5.1."
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include <stdlib.h>
#include <string.h>

// Stock GLFW 3.4 defines no emscripten platform id. Use the value the
// emscripten-glfw (pongasoft) contrib port assigns to GLFW_PLATFORM_EMSCRIPTEN
// so platform checks agree across the two web GLFW implementations.
#define DAS_GLFW_PLATFORM_EMSCRIPTEN 0x00060006

// ===== init (3.4 additions; the JS library has a fixed init path) =====

// libglfw.js reads no init hints; every hint would be ignored anyway.
void glfwInitHint(int hint, int value) {
    (void)hint; (void)value;
}

// libglfw.js allocates on the JS heap; a C-side allocator cannot apply.
void glfwInitAllocator(const GLFWallocator * allocator) {
    (void)allocator;
}

// ===== error / platform (libglfw.js tracks no errors, has no platform enum) =====

// libglfw.js has no error bookkeeping to read back.
int glfwGetError(const char ** description) {
    if (description) *description = NULL;
    return GLFW_NO_ERROR;
}

// The platform is always the web; see DAS_GLFW_PLATFORM_EMSCRIPTEN above.
int glfwGetPlatform(void) {
    return DAS_GLFW_PLATFORM_EMSCRIPTEN;
}

// Only the web platform is compiled in.
int glfwPlatformSupported(int platform) {
    return platform == DAS_GLFW_PLATFORM_EMSCRIPTEN ? GLFW_TRUE : GLFW_FALSE;
}

// ===== monitor user pointer (libglfw.js exposes exactly one monitor) =====

static void * s_monitor_user_pointer = NULL;

void glfwSetMonitorUserPointer(GLFWmonitor * monitor, void * pointer) {
    (void)monitor;  // single-monitor: one static slot is faithful
    s_monitor_user_pointer = pointer;
}

void * glfwGetMonitorUserPointer(GLFWmonitor * monitor) {
    (void)monitor;
    return s_monitor_user_pointer;
}

// ===== gamepad mapping layer (browser Gamepad API has its own "standard"
// mapping; SDL_GameControllerDB tables are inapplicable. Raw input still works
// via glfwGetJoystickAxes/Buttons, which libglfw.js implements) =====

// No SDL mapping database to update; honest failure.
int glfwUpdateGamepadMappings(const char * string) {
    (void)string;
    return GLFW_FALSE;
}

// NULL is the documented "no mapping / not present" return.
const char * glfwGetGamepadName(int jid) {
    (void)jid;
    return NULL;
}

// GLFW_FALSE without touching state = documented "no gamepad mapping" return.
int glfwGetGamepadState(int jid, GLFWgamepadstate * state) {
    (void)jid; (void)state;
    return GLFW_FALSE;
}

// ===== window title read-back (3.4 addition) =====
// libglfw.js implements glfwSetWindowTitle (win.title is write-only there; it
// forwards to emscripten_set_window_title = document.title for the active
// window) but has no glfwGetWindowTitle. Shadow the setter natively — a native
// definition takes precedence over a JS library symbol — to keep a C-side copy,
// and forward to emscripten_set_window_title exactly as the JS version does
// (libglfw.js supports a single window, so no per-window map is needed).

static char * s_window_title = NULL;    // strdup'd copy; GLFW contract: valid until next set

void glfwSetWindowTitle(GLFWwindow * window, const char * title) {
    (void)window;   // single-window build
    if (!title) title = "";
    free(s_window_title);
    s_window_title = strdup(title);
    emscripten_set_window_title(title);
}

// "" before any set: libglfw.js never applies the glfwCreateWindow title to the
// page either, so there is no truer initial value to report.
const char * glfwGetWindowTitle(GLFWwindow * window) {
    (void)window;
    return s_window_title ? s_window_title : "";
}

// ===== input-method API (dasGlfw's patch: modules/dasGlfw/patches) =====
// libglfw.js has no input-method surface; dasImgui's browser bridge composes text
// through a DOM textarea instead, so the web build has no preedit to report.

void glfwGetPreeditCursorRectangle(GLFWwindow * window, int * x, int * y, int * w, int * h) {
    (void)window;
    if (x) *x = 0;
    if (y) *y = 0;
    if (w) *w = 0;
    if (h) *h = 0;
}

void glfwSetPreeditCursorRectangle(GLFWwindow * window, int x, int y, int w, int h) {
    (void)window; (void)x; (void)y; (void)w; (void)h;
}

void glfwResetPreeditText(GLFWwindow * window) {
    (void)window;
}

void glfwSetTextInputFocus(GLFWwindow * window, int focused) {
    (void)window; (void)focused;
}

// NULL with a zero count is the documented "no such candidate" return.
unsigned int * glfwGetPreeditCandidate(GLFWwindow * window, int index, int * textCount) {
    (void)window; (void)index;
    if (textCount) *textCount = 0;
    return NULL;
}
