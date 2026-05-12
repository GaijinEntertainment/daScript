options gen2
options no_aot
// no_aot: this module only carries a tiny sort helper + a wire-data struct.
// AOT-compiling it would force every test that requires `live/mouse_events`
// into the AOT registry; keeping it interpreter-only lets the regression
// test under tests/dasglfw/ avoid pulling in glfw_live's full GL chain.

struct MouseEventWire {
    @optional t_ms   : int    = 0
    kind             : string = "move"  // "move" | "button" | "scroll"
    @optional x      : float  = 0.0
    @optional y      : float  = 0.0
    @optional button : int    = 0
    @optional action : string = "press"
}

def public sort_play_events(var events : array<MouseEventWire>) {
    //! Stable-sort a mouse_play timeline by ``t_ms``. mouse_play only walks
    //! the queue forward, so out-of-order input would silently drop earlier
    //! events; sort first.
    events |> sort() $(a, b : MouseEventWire) {
        return a.t_ms < b.t_ms
    }
}
