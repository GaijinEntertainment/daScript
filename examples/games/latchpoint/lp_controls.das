options gen2
require glfw/glfw_boost public
require math public

def menu_hit(position, screen_size : float2) : int {
    let relative = position - screen_size * 0.5
    return -1 if (abs(relative.x) > 240.0)
    return 0 if (relative.y >= -56.0 && relative.y <= 8.0)
    return 1 if (relative.y >= 28.0 && relative.y <= 92.0)
    return -1
}

def movement_axes(state : bool[512]) : float2 {
    let right = state[GLFW_KEY_D] || state[GLFW_KEY_RIGHT]
    let left = state[GLFW_KEY_A] || state[GLFW_KEY_LEFT]
    let forward = state[GLFW_KEY_W] || state[GLFW_KEY_UP]
    let backward = state[GLFW_KEY_S] || state[GLFW_KEY_DOWN]
    let input = float2((right ? 1.0 : 0.0) - (left ? 1.0 : 0.0),
        (forward ? 1.0 : 0.0) - (backward ? 1.0 : 0.0))
    return length(input) > 0.0 ? normalize(input) : input
}
