options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

//! The mechanical half of modules/dasSpirv/REVIEW.md. Run from the
//! repo root: exit 0 clean, 1 with findings.

let private ROOT = "modules/dasSpirv"
let private BANNED_DESCRIPTORS <- ["modules/dasGlsl/.das_module", "modules/dasOpenGL/.das_module"]
let private BANNED_DIRS <- ["dasGlsl/", "dasOpenGL/"]

//! every `category/name` require path the two banned modules'
//! descriptors register
def private banned_requires : table<string> {
    var out : table<string>
    for (desc in BANNED_DESCRIPTORS) {
        continue if (!fexist(desc))
        let text = strip_line_comments(fread(desc))
        var first = true
        for (call in split(text, "register_native_path(")) {
            if (first) {
                first = false
                continue
            }
            var inscope args <- split(call, "\"")
            if (length(args) >= 4) {
                out |> insert("{args[1]}/{args[3]}")
            }
        }
    }
    return <- out
}

def private walk_sources(root : string; var out : array<string>) {
    dir(root) $(name) {
        return if ((name |> starts_with(".") && name != ".das_module") || name |> starts_with("_")
            || name == "spirv_headers" || name == "REVIEW.das")
        let p = path_join(root, name)
        let st = stat(p)
        return if (!st.is_valid)
        if (st.is_dir) {
            walk_sources(p, out)
        } elif (name |> ends_with(".das") || name == ".das_module" || name == "CMakeLists.txt"
                || name |> ends_with(".cpp") || name |> ends_with(".h")) {
            out |> push(p)
        }
    }
}

//! `text` without its comments: `#` lines for CMake, `//` tails for
//! everything else
def private code_of(path, text : string) : string {
    if (!(path |> ends_with("CMakeLists.txt"))) return strip_line_comments(text)
    return join([for (l in split(text, "\n")); strip(l) |> starts_with("#") ? "" : l], "\n")
}

def private check_no_glsl_dependency {
    var inscope banned <- banned_requires()
    var files : array<string>
    walk_sources(ROOT, files)
    files |> sort
    for (path in files) {
        if (path |> ends_with(".das")) {
            for (req in das_requires(path)) {
                if (key_exists(banned, req) || names_banned_dir(req)) {
                    gate_finding(path, find_line(fread(path), req),
                        "requires `{req}` from dasGlsl/dasOpenGL - "
                        + "write dasSpirv's own implementation under {ROOT} instead")
                }
            }
        }
        var line_no = 0
        for (line in split(code_of(path, fread(path)), "\n")) {
            line_no++
            if (names_banned_dir(line)) {
                gate_finding(path, line_no,
                    "includes or loads a path under modules/dasGlsl or modules/dasOpenGL - "
                    + "write dasSpirv's own implementation under {ROOT} instead")
            }
        }
    }
}

def private names_banned_dir(text : string) : bool {
    for (d in BANNED_DIRS) {
        return true if (find(text, d) >= 0)
    }
    return false
}

[export]
def main() : int {
    if (!fexist("{ROOT}/REVIEW.das")) {
        to_log(LOG_ERROR, "{ROOT}/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_no_glsl_dependency()
    return gate_verdict("dasspirv")
}
