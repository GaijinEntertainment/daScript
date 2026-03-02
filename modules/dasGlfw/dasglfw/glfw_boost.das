options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module glfw_boost shared

require glfw public
require daslib/safe_addr
require math
require rtti

def glfwInitOpenGL(major : int = 3; minor : int = 3; dctx : bool = false; compat : bool = false) {
    this_context().category |= bitfield(1u << 3u)   // setup OpenGL flag
    glfwWindowHint(int(GLFW_CONTEXT_VERSION_MAJOR), major)
    glfwWindowHint(int(GLFW_CONTEXT_VERSION_MINOR), minor)
    if (compat) {
        glfwWindowHint(int(GLFW_OPENGL_PROFILE), int(GLFW_OPENGL_COMPAT_PROFILE))
    } else {
        glfwWindowHint(int(GLFW_OPENGL_PROFILE), int(GLFW_OPENGL_CORE_PROFILE))
    }
    glfwWindowHint(int(GLFW_OPENGL_FORWARD_COMPAT), 1)
    if (dctx) {
        glfwWindowHint(int(GLFW_OPENGL_DEBUG_CONTEXT), 1)
    }
}

def glfwGetFramebufferSize(window : GLFWwindow?; var display_w, display_h : int&) {
    glfwGetFramebufferSize(window, unsafe(addr(display_w)), unsafe(addr(display_h)))
}

def gl_array_ptr(foo : array<auto>) : void? {
    unsafe {
        return reinterpret<void?> addr(foo[0])
    }
}

def gl_array_ptr(foo : auto[]) : void? {
    unsafe {
        return reinterpret<void?> addr(foo[0])
    }
}

def gl_array_ptr(foo : auto[][]) : void? {
    unsafe {
        return reinterpret<void?> addr(foo[0][0])
    }
}

def glfwGetCursorPos(window : GLFWwindow?) : float2 {
    var xpos, ypos : double
    glfwGetCursorPos(window, safe_addr(xpos), safe_addr(ypos))
    return float2(xpos, ypos)
}
