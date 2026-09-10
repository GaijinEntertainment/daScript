options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_env
require daslib/fio
require strings

// Shared by the browser examples' smoke tests (test_storyteller_restart.das, test_storywish.das):
// each spawns its example as a child of this binary, and the example opens a GLFW window.

//! this binary, as the child's argv[0]
def public example_daslang() : string {
    let argv <- get_command_line_arguments()
    return find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
}

//! a Linux box reached over ssh has no window server, and that is the box's fact, not a red; macOS and
//! Windows tell only by trying, which skip_without_window reads off the child's output
def public has_window_server() : bool {
    return get_platform_name() != "linux" || !empty(g_env_test.display) || !empty(g_env_test.wayland_display)
}

//! the child could not open its window (glfw_live's own panic in `out`): the cell skips on the box's fact instead of reading a red
def public skip_without_window(t : T?; rc : int; out : string) : bool {
    return false if (rc == 0 || find(out, "glfw_live: can't") < 0)
    t |> skip("the example could not open a window on this box (glfw_live: can't init glfw / create window)")
    return true
}
