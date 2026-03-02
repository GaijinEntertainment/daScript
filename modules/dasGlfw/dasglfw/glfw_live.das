options gen2

options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options threadlock_context = true

module glfw_live shared public

require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_cache
require daslib/defer
require daslib/live
require daslib/safe_addr

require debugapi

var private glfw_live_window : GLFWwindow?

def public get_live_window : GLFWwindow ? {
    if (glfw_live_window == null) {
        unsafe {
            var ppl = addr(glfw_live_window)
            invoke_in_context(get_live_context()) <| @ {
                *ppl = glfw_live_window
            }
        }
    }
    return glfw_live_window
}

def private setup_events(win : GLFWwindow?) {
    win |> glfwSetKeyCallback <| @(ewin, key, scancode, action, mods) {
        live_keypressed(key, scancode, action, mods)
    }
    win |> glfwSetCharCallback <| @(ewin, charcode) {
        live_charpressed(charcode)
    }
    win |> glfwSetCursorPosCallback <| @(ewin, x, y) {
        live_mousemoved(x, y)
    }
    win |> glfwSetCursorEnterCallback <| @(ewin, ent) {
        live_mouseenter(ent)
    }
    win |> glfwSetMouseButtonCallback <| @(ewin, button, action, mods) {
        live_mousebutton(button, action, mods)
    }
    win |> glfwSetScrollCallback <| @(ewin, x, y) {
        live_mousescroll(x, y)
    }
}

def public glfw_live(title, appf, appd : string) {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    defer <| $() {
        glfwTerminate()
    }
    glfwInitOpenGL(3, 3)
    var vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor())
    glfw_live_window = glfwCreateWindow(vidMode.width / 2, vidMode.height / 2, title, null, null)
    if (glfw_live_window == null) {
        panic("can't create window")
    }
    defer <| $() {
        glfwDestroyWindow(glfw_live_window)
        glfw_live_window = null
    }
    glfwSetInputMode(glfw_live_window, int(GLFW_STICKY_KEYS), GLFW_TRUE)  // note sticky keys
    glfwSetWindowPos(glfw_live_window, vidMode.width / 2, vidMode.height / 4)
    glfwMakeContextCurrent(glfw_live_window)
    init_opengl_cache()
    setup_events(glfw_live_window)
    go_live(appf, appd)
    while (glfwWindowShouldClose(glfw_live_window) == 0) {
        glfwPollEvents()
        live_update(glfwGetTime())
        var display_w, display_h : int
        glfwGetFramebufferSize(glfw_live_window, display_w, display_h)
        if (display_w != 0 && display_h != 0) {
            if (is_live()) {
                invoke_live("display", display_w, display_h)
            } else {
                glViewport(0, 0, display_w, display_h)
                glClearColor(1.0, 0.0, 0.0, 1.0)
                glClearDepth(1.0lf)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            }
        }
        glfwMakeContextCurrent(glfw_live_window)
        glfwSwapBuffers(glfw_live_window)
        if (watch_files()) {
            recompile()
        }
    }
}

def public glfw_app(title : string) {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    defer <| $() {
        glfwTerminate()
    }
    glfwInitOpenGL(3, 3)
    var vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor())
    glfw_live_window = glfwCreateWindow(vidMode.width / 2, vidMode.height / 2, title, null, null)
    if (glfw_live_window == null) {
        panic("can't create window")
    }
    defer <| $() {
        glfwDestroyWindow(glfw_live_window)
        glfw_live_window = null
    }
    glfwSetInputMode(glfw_live_window, int(GLFW_STICKY_KEYS), GLFW_TRUE)    // note sticky keys
    glfwSetWindowPos(glfw_live_window, vidMode.width / 4, vidMode.height / 4)
    glfwMakeContextCurrent(glfw_live_window)
    init_opengl_cache()
    setup_events(glfw_live_window)
    go_main()
    let full_restart : bool = false
    invoke_live("initialize", full_restart)
    while (glfwWindowShouldClose(glfw_live_window) == 0) {
        glfwPollEvents()
        live_update(glfwGetTime())
        var display_w, display_h : int
        glfwGetFramebufferSize(glfw_live_window, display_w, display_h)
        if (display_w != 0 && display_h != 0) {
            invoke_live("display", display_w, display_h)
        }
        glfwMakeContextCurrent(glfw_live_window)
        glfwSwapBuffers(glfw_live_window)
    }
    invoke_live("shutdown")
}
