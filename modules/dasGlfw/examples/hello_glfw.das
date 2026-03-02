options gen2
require glfw/glfw_boost
require daslib/defer
require daslib/math_boost
require daslib/safe_addr

[export]
def main {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    defer <| $() {
        glfwTerminate()
    }

    var window = glfwCreateWindow(1270, 720, "Hello GLFW", null, null)
    if (window == null) {
        panic("can't create window")
    }
    defer <| $() {
        glfwDestroyWindow(window)
    }
    glfwMakeContextCurrent(window)
    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents()
    }
}
