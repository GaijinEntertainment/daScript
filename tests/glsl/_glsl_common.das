options gen2
options indenting = 4

// Shared gates for the dasGlsl test suite. validate_glsl shells the emitted shader text out to
// glslang (the reference GLSL front-end — the real syntax/extension oracle the substring pins
// can't provide); it soft-skips when the tool is absent locally and CI enforces presence.
// Mirrors tests/spirv/_spirv_common.das validate_spirv (discovery, temp file, soft-skip shape).

module _glsl_common shared public

require daslib/fio
require strings
require dastest/check_cache

def public has(hay, needle : string) : bool => find(hay, needle) >= 0

struct GlslValResult {
    ran : bool      // false => tool not found (soft-skip locally; CI provides it)
    ok  : bool
    msg : string
}

// stage_ext: "frag" | "vert" | "comp" — glslang reads the stage off the file extension.
// Desktop-GL mode (no -V): validates against the #version/#extension lines in the text.
def public validate_glsl(text : string; stage_ext : string) : GlslValResult {
    let tool = vulkan_sdk_tool("glslangValidator")
    if (empty(tool.exe)) {
        return GlslValResult(ran = false, ok = false, msg = "glslangValidator not found")
    }
    let key = hash("{tool.stamp}|{stage_ext}|{hash(text)}")
    if (check_passed("glslang", key)) return GlslValResult(ran = true, ok = true)
    let tf = create_temp_file_result("das_glsl", ".{stage_ext}")
    if (tf is error) return GlslValResult(ran = true, ok = false, msg = "temp file: {tf as error}")
    let path = tf as value
    var wrote = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, text)
            wrote = true
        }
    }
    if (!wrote) {
        remove_result(path)
        return GlslValResult(ran = true, ok = false, msg = "write failed: {path}")
    }
    var out : string
    let rc = run_and_capture([tool.exe, path], out, 30.0)
    remove_result(path)
    if (rc == 0) {
        record_check_passed("glslang", key)
    }
    return GlslValResult(ran = true, ok = rc == 0, msg = out)
}
