    var camera_distance = 24.
    var camera_rotation : float
    var camera_position_y : float
    var cursor_position_xy = glfwGetCursorPos(window)
    var cursor_enabled = false
    unsafe
        glfwSetScrollCallback(window) <| @ [[&camera_distance]] ( w, x, y )
            if y>0.0lf
                camera_distance *= 1.1
            else
                camera_distance /= 1.1
        glfwSetCursorPosCallback(window) <| @ [[&cursor_position_xy,&cursor_enabled,&camera_position_y,&camera_rotation,&camera_distance]] ( w, x, y )
            if cursor_enabled
                let cxy = glfwGetCursorPos(window)
                let dxy = cxy - cursor_position_xy
                var display_w, display_h : int
                glfwGetFramebufferSize(window, display_w, display_h)
                camera_position_y += dxy.y * 4. * camera_distance/float(display_h)
                camera_rotation += dxy.x * 720. / float(display_w)
                cursor_position_xy = float2(display_w/2, display_h/2)
                glfwSetCursorPos(window, double(cursor_position_xy.x), double(cursor_position_xy.y))
        glfwSetMouseButtonCallback(window) <| @ [[&cursor_enabled,&cursor_position_xy]] ( w, b, a, m )
            if b==int(GLFW_MOUSE_BUTTON_1)
                cursor_enabled = a==int(GLFW_PRESS)
                if cursor_enabled
                    glfwSetInputMode(window, int(GLFW_CURSOR), int(GLFW_CURSOR_HIDDEN))
                    glfwSetInputMode(window, int(GLFW_RAW_MOUSE_MOTION), int(GLFW_TRUE))
                    var display_w, display_h : int
                    glfwGetFramebufferSize(window, display_w, display_h)
                    cursor_position_xy = float2(display_w/2, display_h/2)
                    glfwSetCursorPos(window, double(cursor_position_xy.x), double(cursor_position_xy.y))
                else
                    glfwSetInputMode(window, int(GLFW_CURSOR), int(GLFW_CURSOR_NORMAL))
                    glfwSetInputMode(window, int(GLFW_RAW_MOUSE_MOTION), int(GLFW_FALSE))
