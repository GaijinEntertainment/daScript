options gen2
options persistent_heap = true

require imgui_app
require glfw/glfw_boost
require imgui/imgui_boost
require opengl/opengl_boost
require imgui_demo

var window : GLFWwindow?

def imgui_app(title : string; blk : block) {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3, false, false)
    window = glfwCreateWindow(1280, 720, title, null, null)
    if (window == null) {
          panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    glfwSwapInterval(1)
    CreateContext(null)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 2.0   // BBATKIN: note - my monitor is HUGE
    StyleColorsDark(null)
    ImGui_ImplGlfw_InitForOpenGL(window, true)
    ImGui_ImplOpenGL3_Init("#version 330")
    let clear_color = float4(0.45f, 0.55f, 0.60f, 1.00f)
    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents()
        ImGui_ImplOpenGL3_NewFrame()
        ImGui_ImplGlfw_NewFrame()
        invoke(blk)
        var display_w, display_h : int
        glfwGetFramebufferSize(window, display_w, display_h)
        glViewport(0, 0, display_w, display_h)
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w)
        glClear(GL_COLOR_BUFFER_BIT)
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)
    }
    ImGui_ImplOpenGL3_Shutdown()
    ImGui_ImplGlfw_Shutdown()
    DestroyContext(null)
    glfwDestroyWindow(window)
    glfwTerminate()
}

[export]
def main {
    imgui_app("Testing IMGUI") {
        NewFrame()
        DasShowDemoWindow(null)
        Render()
    }
}

