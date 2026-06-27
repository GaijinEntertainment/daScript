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
    // 2. emit extras from opengl_const.das (format: `let GL_X = uint(0xNu)` / `int(N)`)
    let out = "{root}/modules/dasOpenGL/src/dasOpenGL.const.desktop_extras.inc"
    var count = 0
    fopen(out, "wb") $(f) {
        fprint(f, "// this file is generated via gen_const_compat.das\n")
        fprint(f, "// desktop GL constants absent from GLES3, for boost-layer compile compat on web\n")
        for (line in split(fread("{root}/modules/dasOpenGL/opengl/opengl_const.das"), "\n")) {
            if (!starts_with(line, "let GL_")) { continue }
            let toks <- nonempty(split_by_chars(line, " =()"))
            if (length(toks) < 4) { continue }       // let NAME TYPE VALUE
            let name = toks[1]
            let dtype = toks[2]
            let value = toks[3]
            if (defined?[name] ?? false) { continue }
            let ctype = dtype == "int" ? "int32_t" : "uint32_t"
            fprint(f, "addConstant<{ctype}>(*this,\"{name}\",{ctype}({value}));\n")
            count ++
        }
    }
    print("gen_const_compat: {count} desktop const extras -> {out}\n")
}
