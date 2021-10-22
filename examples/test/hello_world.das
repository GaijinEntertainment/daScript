require glfw/glfw_boost
require daslib/defer
require daslib/safe_addr

require bgfx

[export]
def main

    let WNDW_WIDTH = 1280
    let WNDW_HEIGHT = 720

    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    var window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "Testing BGFX", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)

    var pd: bgfx_platform_data_s
    pd.nwh = glfwGetWin32Window(window)
    bgfx_set_platform_data(safe_addr(pd))

    var bgfxInit: bgfx_init_s
    bgfx_init_ctor(safe_addr(bgfxInit))

    bgfxInit._type = bgfx_renderer_type BGFX_RENDERER_TYPE_COUNT
    bgfxInit.resolution.width = uint(WNDW_WIDTH)
    bgfxInit.resolution.height = uint(WNDW_HEIGHT)
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC
    bgfx_init(safe_addr(bgfxInit))

    bgfx_set_view_clear(uint16(0), uint16(BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH), 0x443355FF, 1.0f, uint8(0))

	bgfx_set_debug(BGFX_DEBUG_TEXT)

    while glfwWindowShouldClose(window)==0
        glfwPollEvents()
        var display_w, display_h : int
        glfwGetFramebufferSize(window, display_w, display_h)

        bgfx_set_view_rect(uint16(0), uint16(0), uint16(0), uint16(WNDW_WIDTH), uint16(WNDW_HEIGHT))

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		let encoder = bgfx_encoder_begin(true)
		bgfx_encoder_touch(encoder, uint16(0))
		bgfx_encoder_end(encoder)

		bgfx_dbg_text_clear(uint8(0), false)
		bgfx_dbg_text_printf(uint16(1), uint16(1), uint8(0x0f), "Hello BGFX")

        bgfx_frame(true)

        // glfwMakeContextCurrent(window)
        // glfwSwapBuffers(window)// options optimize = false

    bgfx_shutdown()
