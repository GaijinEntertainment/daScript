options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_env
require daslib/fio
require daslib/env_registry
require daslib/strings_boost
require strings
require math
require _model_tier   // models_dir, model_available: the smoke cells are stocked

// Shared by the browser examples' smoke tests (test_storyteller_restart.das, test_storywish.das, test_parrot.das):
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

//! the window server and every model file in `models`, each absence registering its own loud skip
def public example_ready(t : T?; models : array<string>) : bool {
    if (!has_window_server()) {
        t |> skip("no window server (DISPLAY and WAYLAND_DISPLAY unset) - the example opens a window")
        return false
    }
    for (name in models) {
        return false if (!model_available(t, path_join(models_dir(), name)))
    }
    return true
}

//! the example `examples/dasLLAMA/<main>` spawned in its smoke mode over the stocked models with `args` after, its output into
//! `out` under a 300 s wall clock; DASLLAMA_IMAGE=0 for the child, so it never mints a sidecar beside the stocked models
def public run_example(main : string; args : array<string>; var out : string&) : int {
    let prev_image = env_value_of("DASLLAMA_IMAGE")
    set_env_variable("DASLLAMA_IMAGE", "0")
    let root = get_das_root()
    var argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/examples/dasLLAMA/{main}", "--", "--models", models_dir(), "--smoke"]
    argv |> push_from(args)
    let rc = run_and_capture(argv, out, 300.0)
    set_env_variable("DASLLAMA_IMAGE", prev_image)
    return rc
}

//! the lines of `out` the example tagged `<tag>:` - the witness its smoke run prints
def public example_witness(out, tag : string) : string {
    var inscope lines <- split(out, "\n")
    return join([for (line in lines); line; where find(line, "{tag}:") >= 0], "\n")
}

//! the last 600 bytes of `out`: a red's context
def public example_tail(out : string) : string => slice(out, max(0, length(out) - 600))
