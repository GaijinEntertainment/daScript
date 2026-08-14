options gen2
// Generates src/dasOpenGL.const.desktop_extras.inc: the desktop GL_* constants from
// opengl/opengl_const.das that are NOT already in the GLES3-derived src/dasOpenGL.const.inc.
// The shared opengl boost layer references many desktop-only enums (compute, SSBO, debug,
// texture-level queries, ...) that must EXIST for it to compile on web even though those
// code paths aren't reached on WebGL2. Values are harmless integers on WebGL2.
//
// Run after bind_opengl.das:  daslang modules/dasOpenGL/bind/gen_const_compat.das
require daslib/fio
require daslib/strings_boost
require strings

def nonempty(toks : array<string>) : array<string> {
    var r <- [for (t in toks); t; where t != ""]
    return <- r
}

[export]
def main {
    let root = get_das_root()
    // 1. names already defined by the GLES3 native const.inc
    var defined : table<string; bool>
    for (line in split(fread("{root}/modules/dasOpenGL/src/dasOpenGL.const.inc"), "\n")) {
        let q1 = find(line, "\"")
        if (q1 < 0) { continue }
        let rest = slice(line, q1 + 1)
        let q2 = find(rest, "\"")
        if (q2 < 0) { continue }
        let name = slice(rest, 0, q2)
        if (starts_with(name, "GL_")) { defined[name] = true }
    }
    // 2. emit extras from opengl_const.das (format: `let GL_X = 0xNu` / `N` bare literals,
    //    or `TYPE(VALUE)` when the emitter kept a genuine cast)
    let out = "{root}/modules/dasOpenGL/src/dasOpenGL.const.desktop_extras.inc"
    var count = 0
    fopen(out, "wb") $(f) {
        fprint(f, "// this file is generated via gen_const_compat.das\n")
        fprint(f, "// desktop GL constants absent from GLES3, for boost-layer compile compat on web\n")
        for (line in split(fread("{root}/modules/dasOpenGL/opengl/opengl_const.das"), "\n")) {
            if (!starts_with(line, "let GL_")) { continue }
            let toks <- nonempty(split_by_chars(strip(line), " =()"))
            if (length(toks) < 3) { continue }
            // exactly `let NAME VALUE` or `let NAME TYPE VALUE` - anything longer (a trailing
            // comment, an expression) would silently mistype the constant, so fail loudly
            if (length(toks) > 4) {
                panic("gen_const_compat: unexpected token shape in `{line}`")
            }
            let name = toks[1]
            var ctype = ""
            var value = ""
            if (length(toks) == 4) {                 // let NAME TYPE VALUE
                value = toks[3]
                if (toks[2] == "int") {
                    ctype = "int32_t"
                } elif (toks[2] == "uint") {
                    ctype = "uint32_t"
                } elif (toks[2] == "int64") {
                    ctype = "int64_t"
                } elif (toks[2] == "uint64") {
                    ctype = "uint64_t"
                } else {
                    panic("gen_const_compat: unknown cast type `{toks[2]}` in `{line}`")
                }
            } else {                                 // let NAME VALUE (bare literal)
                value = toks[2]
                if (ends_with(value, "ul")) {        // uint64 literal - never truncate to 32 bits
                    ctype = "uint64_t"
                } elif (ends_with(value, "u")) {
                    ctype = "uint32_t"
                } elif (ends_with(value, "l")) {
                    ctype = "int64_t"
                } else {
                    ctype = "int32_t"
                }
            }
            if (defined?[name] ?? false) { continue }
            fprint(f, "addConstant<{ctype}>(*this,\"{name}\",{ctype}({value}));\n")
            count ++
        }
    }
    print("gen_const_compat: {count} desktop const extras -> {out}\n")
}
