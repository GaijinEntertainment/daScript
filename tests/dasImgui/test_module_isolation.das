options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/module_path
require strings

//! Source-level audit: `widgets/imgui_visual_aids.das` must not directly
//! require any GLFW module. The Vulkan-backed ImGui case must work without
//! `glfw_live` loaded; this test guards against accidentally re-adding such
//! a dependency to the visualization layer.
//!
//! Why source-grep and not RTTI: `imgui_app` (the C++ binding) currently
//! requires the `glfw` C++ module unconditionally, so transitive RTTI walks
//! would always surface glfw — a future PR splits `imgui_app` into
//! `imgui_synth` + `imgui_app_glfw` (see project memory
//! `dasimgui_cpp_glfw_split_deferred`). Until then, the .das layer is the
//! checkable boundary: imgui_visual_aids names no glfw module directly.

def private starts_with_require(line : string) : bool {
    let trimmed = chop(line, 0, length(line))
    return false if (length(trimmed) < 8)
    return slice(trimmed, 0, 8) == "require "
}

def private require_token(line : string) : string {
    // Strip `require ` and any trailing ` public`/`private`, return the module path.
    var tok = slice(line, 8, length(line))
    // Drop trailing modifiers.
    for (suffix in ["public", "private"]) {
        if (length(tok) > length(suffix) + 1
                && slice(tok, length(tok) - length(suffix), length(tok)) == suffix
                && slice(tok, length(tok) - length(suffix) - 1, length(tok) - length(suffix)) == " ") {
            tok = chop(tok, 0, length(tok) - length(suffix) - 1)
        }
    }
    return tok
}

def private file_lines(path : string; var lines : array<string>) : bool {
    var ok = false
    fopen(path, "r") $(f) {
        if (f == null) return
        ok = true
        while (true) {
            let line = f |> fgets()
            break if (empty(line))
            lines |> push(chop(line, 0, length(line)))   // keep trailing newline if present; pattern checks are length-bounded
        }
    }
    return ok
}

[test]
def test_imgui_visual_aids_has_no_direct_glfw_require(t : T?) {
    // Resolve the target off this file's own location, not cwd: get_this_module_dir() returns
    // <root>/tests/integration, so two dir_name hops give the module root (works under both CI's
    // junction layout and a local -load_module). Must be inside a function (-exe top-level let baking).
    let target = "{dir_name(dir_name(get_this_module_dir()))}/widgets/imgui_visual_aids.das"
    var lines : array<string>
    let ok = file_lines(target, lines)
    t |> success(ok, "open {target}")
    return if (!ok)

    var offenders : array<string>
    for (line in lines) {
        if (!starts_with_require(line)) continue
        let mod = require_token(line)
        // Direct glfw deps live under `glfw/...` or `live/glfw_*`.
        if (length(mod) >= 5 && slice(mod, 0, 5) == "glfw/") {
            offenders |> push(line)
        }
        if (length(mod) >= 10 && slice(mod, 0, 10) == "live/glfw_") {
            offenders |> push(line)
        }
    }
    t |> equal(length(offenders), 0,
               "imgui_visual_aids must not directly require any GLFW module — see project_dasimgui_cpp_glfw_split_deferred")
    for (off in offenders) {
        to_log(LOG_ERROR, "test_module_isolation: offending require in {target}: {off}")
    }
}
