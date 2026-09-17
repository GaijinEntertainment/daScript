// Emscripten GLFW assumes Pointer Lock exists when restoring a normal cursor.
// Touch-only WebKit exposes neither entry point, but still needs normal input.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>

EM_JS_DEPS(das_glfw_cursor_deps, "$GLFW,$Browser");

EM_JS(void, das_glfw_set_input_mode,
      (GLFWwindow *window, int mode, int value, int cursor_mode, int normal_mode, int disabled_mode), {
    var id = Number(window);
    var win = GLFW.WindowFromId(id);
    if (!win) return;
    var canvas = Browser.getCanvas();
    if (mode == cursor_mode && value == normal_mode && typeof document.exitPointerLock != 'function') {
        win.inputModes[mode] = value;
        canvas.removeEventListener('click', GLFW.onClickRequestPointerLock, true);
        return;
    }
    if (mode == cursor_mode && value == disabled_mode &&
        (typeof canvas.requestPointerLock != 'function' || typeof document.exitPointerLock != 'function')) {
        return;
    }
    GLFW.setInputMode(id, mode, value);
});

void glfwSetInputMode(GLFWwindow *window, int mode, int value) {
    das_glfw_set_input_mode(window, mode, value, GLFW_CURSOR, GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED);
}
