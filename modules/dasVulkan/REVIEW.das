options gen2

require strings
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasVulkan/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang modules/dasVulkan/REVIEW.das - exit 0
// clean, 1 with findings.

//! The frame-shape rules' mechanical half: every call to a render-pass or legacy-flag barrier helper
//! under `roots` is a finding, except under `ignore_prefix` (the ARCHITECTURE.md exception ledger's
//! tutorials/10_deferred).
def gate_legacy_frame_shape(roots : array<string>; ignore_prefix : string) {
    let legacy <- ["record_render_pass", "transition_image", "transition_depth_image", "transition_image_aspect"]
    for (root in roots) {
        glob(root, "**/*.das") $(rel; is_dir) {
            let path = "{root}/{rel}"
            return if (is_dir || starts_with(path, ignore_prefix))
            let text = strip_line_comments(fread(path))
            for (name in legacy) {
                if (contains_word(text, name)) {
                    gate_finding(path, find_line(text, name),
                        "calls {name} - a tutorial or example draws inside record_rendering and moves layouts with the transition_image2 family; the render-pass object and the 32-bit flags are the pre-1.3 frame shape")
                }
            }
        }
    }
}

//! The viewer-naming rule's mechanical half: every `tutorials/NN_<name>/` holds a `window/show_<name>.das`
//! or `window/show_<name>_<method>.das`, and every `window/show_*.das` is one of those two forms.
def gate_tutorial_viewers(tutorials_root : string) {
    dir(tutorials_root) $(folder) {
        let us = find(folder, "_")
        let first = first_character(folder)
        return if (us <= 0 || first < '0' || first > '9')
        let name = slice(folder, us + 1)
        let window = "{tutorials_root}/{folder}/window"
        var viewers = 0
        dir(window) $(file) {
            return if (!ends_with(file, ".das") || !starts_with(file, "show_"))
            let stem = slice(file, 0, length(file) - 4)
            if (stem == "show_{name}" || starts_with(stem, "show_{name}_")) {
                viewers++
            } else {
                gate_finding("{window}/{file}", "a viewer is show_{name}.das, or show_{name}_<method>.das when the tutorial presents more than one way")
            }
        }
        if (viewers == 0) {
            gate_finding("{tutorials_root}/{folder}", "no window/show_{name}*.das viewer - every tutorial ships a GLFW window that presents its output live")
        }
    }
}

[export]
def main() : int {
    if (!fexist("modules/dasVulkan/REVIEW.das")) {
        to_log(LOG_ERROR, "modules/dasVulkan/REVIEW.das: run from the repo root\n")
        return 2
    }
    let non_module_tokens <- { "vulkan", "Module_dasVULKAN" }
    gate_descriptor_census("modules/dasVulkan/.das_module", ["modules/dasVulkan/daslib"], non_module_tokens)
    gate_legacy_frame_shape(["modules/dasVulkan/tutorials", "modules/dasVulkan/examples"], "modules/dasVulkan/tutorials/10_deferred")
    gate_tutorial_viewers("modules/dasVulkan/tutorials")
    return gate_verdict("dasvulkan")
}
