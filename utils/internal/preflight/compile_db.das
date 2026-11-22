options gen2
options indenting = 4

// C++ build-flag reader for preflight's cpp-syntax gate:
// compile_commands.json, or the CMake file API's code model. Lives
// beside main.das as a module so it can be tested directly - its
// failure mode (a mangled path, a dropped flag) is silent, and it
// only bites on a platform the author may not be running.

module compile_db shared

require daslib/fio
require strings


struct CcEntry {
    directory : string
    command : string
    file : string
}


// The database spells every path the way CMake was configured - on a case-insensitive
// filesystem that can differ from the checkout's spelling in case alone (a lowercase `work`
// against the tree's `Work`), and a lookup that honors case then misses the whole database
// and falls back silently. Windows and macOS filesystems fold case, so the key does too.
def public cc_key(path : string) : string => cc_key_on(get_platform_name(), path)


//! the key of source `file` of the tree at `root`
def public cc_file_key(root, file : string) : string => cc_key(path_join(root, file))


//! `cc_key` as `platform` spells it
def public cc_key_on(platform, path : string) : string {
    return to_lower(to_generic_path(path)) if (platform == "windows")
    return platform == "darwin" ? to_lower(path) : path
}


// compile_commands.json writes each command shell-quoted: a define can carry escaped quotes
// (-DX=\"y\") and a path can carry spaces, so splitting on whitespace mangles both.
def public cc_tokenize(cmd : string) : array<string> {
    var out : array<string>
    peek_data(cmd) $(data) {
        let n = length(data)
        var cur : array<uint8>
        var in_quote = false
        var started = false
        var i = 0
        while (i < n) {
            let c = int(data[i])
            // a backslash escapes ONLY a quote or another backslash. Everywhere else it is a
            // literal - on Windows every path in the database is full of them, and consuming
            // them as escapes turns C:\src\x.cpp into C:srcx.cpp. A token with spaces arrives
            // quoted, so nothing else needs escaping here
            if (c == '\\' && i + 1 < n && (int(data[i + 1]) == '"' || int(data[i + 1]) == '\\')) {
                cur |> push(data[i + 1])
                started = true
                i += 2
            } elif (c == '"') {
                in_quote = !in_quote
                started = true
                i++
            } elif (!in_quote && (c == ' ' || c == '\t')) {
                if (started) {
                    out |> push(string(cur))
                    cur |> clear()
                    started = false
                }
                i++
            } else {
                cur |> push(data[i])
                started = true
                i++
            }
        }
        if (started) {
            out |> push(string(cur))
        }
    }
    return <- out
}


// "compile this one file to that one object" - the switches a syntax-only sweep over MANY
// files must not inherit. Both spellings: clang-cl writes the output path JOINED (/Fo<obj>,
// /Fd<pdb>, /Fp<pch>), and a joined switch reaching a multi-file argv makes clang-cl reject
// the whole sweep for naming one output for many inputs - which reads like a source error.
def public cc_is_compile_switch(a : string; is_cl : bool) : bool {
    // cl spells its runtime library -MD and -MT
    let gnu_deps = !is_cl && (a == "-MD" || a == "-MMD")
    if (gnu_deps || a == "-c" || a == "/c" || a == "/showIncludes" || a == "-showIncludes") {
        return true
    }
    for (p in ["/Fo", "-Fo", "/Fd", "-Fd", "/Fp", "-Fp", "/Yc", "-Yc", "/Yu", "-Yu"]) {
        return true if (a |> starts_with(p))
    }
    return false
}


// The flags a syntax-only sweep inherits from one compile
// command: all but the compiler, the file, the per-file output
// switches and a precompiled header's binary. Only the compiler
// build that wrote a .pch reads it, and the gate's compiler need
// not be the build's; the header it stands for stays on the line
// (`-Xclang -include -Xclang <hdr>`).
def public cc_sweep_flags(argv : array<string>; file, file_abs : string;
                          is_cl : bool) : array<string> =>
    cc_keep_flags(argv, 1, file, file_abs, is_cl)


//! `cc_sweep_flags` over `argv` from index `first` on
def private cc_keep_flags(argv : array<string>; first : int; file, file_abs : string;
                          is_cl : bool) : array<string> {
    var flags : array<string>
    flags |> reserve(length(argv))
    var i = first
    while (i < length(argv)) {
        let a = argv[i]
        if (a == "-o" || (!is_cl && (a == "-MT" || a == "-MF"))) {
            i += 2
        } elif (a == "-Xclang" && i + 3 < length(argv) && argv[i + 1] == "-include-pch") {
            i += 4
        } elif (a == file || a == file_abs || cc_is_compile_switch(a, is_cl)) {
            i++
        } else {
            flags |> push(a)
            i++
        }
    }
    return <- flags
}


//! a compiler that takes MSVC's flag spelling: cl or clang-cl
def public cc_is_cl(compiler : string) : bool {
    let name = to_lower(base_name(to_generic_path(compiler)))
    return name == "cl" || name == "cl.exe" || name == "clang-cl" || name == "clang-cl.exe"
}


// The exact flags CMake compiles each TU with, keyed by `cc_key` of the absolute source path
// - the caller looks up through the same key. The gate's own
// include list is a hand-rolled approximation: it models neither build-generated headers
// (tests-cpp/big/standalone_ctx/_generated) nor per-module include dirs, and false-fails any
// TU that needs one. This reflects the LOCAL configure's module ON/OFF set rather than CI's
// all-modules-on lane - still strictly closer than the approximation, which models no module
// include dirs at all. Empty table = fall back to the hand list.
[arch(at="README.md#cpp-flags")]
def public load_compile_commands(build_dir : string; want_cl : bool) : table<string; array<string>> {
    var out : table<string; array<string>>
    return <- out if (empty(build_dir))
    let raw = fread(path_join(build_dir, "compile_commands.json"))
    return <- out if (empty(raw))
    var db : array<CcEntry>
    return <- out if (!sscan_json(raw, db))
    for (e in db) {
        var inscope argv <- cc_tokenize(e.command)
        continue if (length(argv) < 2)
        // the format allows `file` relative to `directory`; the caller looks up by absolute
        // path, and a miss falls back silently
        let file_abs = is_absolute(e.file) ? e.file : path_join(e.directory, e.file)
        continue if (cc_is_cl(argv[0]) != want_cl)
        out[cc_key(file_abs)] <- cc_sweep_flags(argv, e.file, file_abs, want_cl)
    }
    return <- out
}


struct CmObject {
    kind : string
    jsonFile : string
}

struct CmIndex {
    objects : array<CmObject>
}

struct CmCompiler {
    path : string
}

struct CmToolchain {
    language : string
    compiler : CmCompiler
}

struct CmToolchains {
    toolchains : array<CmToolchain>
}

struct CmTargetRef {
    jsonFile : string
}

struct CmConfiguration {
    name : string
    targets : array<CmTargetRef>
}

struct CmPaths {
    source : string
}

struct CmCodeModel {
    paths : CmPaths
    configurations : array<CmConfiguration>
}

struct CmFragment {
    fragment : string
}

struct CmDefine {
    define : string
}

struct CmInclude {
    path : string
}

struct CmStandard {
    standard : string
}

struct CmCompileGroup {
    language : string
    compileCommandFragments : array<CmFragment>
    defines : array<CmDefine>
    includes : array<CmInclude>
    languageStandard : CmStandard
    sourceIndexes : array<int>
}

struct CmSource {
    path : string
}

struct CmTarget {
    sources : array<CmSource>
    compileGroups : array<CmCompileGroup>
}

struct CmReply {
    dir : string
    codemodel_file : string
    toolchains_file : string
}

struct BuildFlags {
    flags : table<string; array<string>>
    origin : string
    configure_error : string
}


def private cm_api_dir(build_dir, leaf : string) : string =>
    path_join(build_dir, ".cmake/api/v1/{leaf}")


//! the newest reply's code model and toolchains files, "" if absent
[arch(at="README.md#cpp-flags")]
def public code_model_reply(build_dir : string) : CmReply {
    var reply = CmReply(dir = cm_api_dir(build_dir, "reply"))
    var newest = ""
    dir(reply.dir) $(name) {
        if (name |> starts_with("index-") && name > newest) {
            newest = name
        }
    }
    return reply if (empty(newest))
    var index : CmIndex
    return reply if (!sscan_json(fread(path_join(reply.dir, newest)), index))
    for (o in index.objects) {
        if (o.kind == "codemodel") {
            reply.codemodel_file = o.jsonFile
        } elif (o.kind == "toolchains") {
            reply.toolchains_file = o.jsonFile
        }
    }
    return reply
}


//! asks the next configure for a code model and the toolchains
[arch(at="README.md#cpp-flags")]
def public write_code_model_query(build_dir : string) : bool {
    let query = cm_api_dir(build_dir, "query/client-preflight")
    return false if (!mkdir_rec(query))
    return (fwrite(path_join(query, "codemodel-v2"), "")
        && fwrite(path_join(query, "toolchains-v1"), ""))
}


//! one compile group's flags, in `want_cl`'s spelling
def public cm_group_flags(group : CmCompileGroup; want_cl : bool) : array<string> {
    var argv : array<string>
    var has_standard = false
    for (f in group.compileCommandFragments) {
        var inscope tokens <- cc_tokenize(f.fragment)
        for (tok in tokens) {
            has_standard ||= tok |> starts_with("/std:") || tok |> starts_with("-std=")
        }
        argv |> push_from(tokens)
    }
    if (!has_standard && !empty(group.languageStandard.standard)) {
        let std = group.languageStandard.standard
        argv |> push(want_cl ? "/std:c++{std}" : "-std=c++{std}")
    }
    let no_file = ""
    var flags <- cc_keep_flags(argv, 0, no_file, no_file, want_cl)
    flags |> reserve(length(flags) + length(group.defines) + length(group.includes))
    for (d in group.defines) {
        flags |> push("-D{d.define}")
    }
    for (inc in group.includes) {
        flags |> push("-I{inc.path}")
    }
    return <- flags
}


//! `load_compile_commands`, read from the CMake file API's reply
[arch(at="README.md#cpp-flags")]
def public load_code_model(build_dir, config : string;
                           want_cl : bool) : table<string; array<string>> {
    var out : table<string; array<string>>
    let reply = code_model_reply(build_dir)
    return <- out if (empty(reply.codemodel_file) || empty(reply.toolchains_file))
    var tools : CmToolchains
    return <- out if (!sscan_json(fread(path_join(reply.dir, reply.toolchains_file)), tools))
    var cxx = ""
    for (tc in tools.toolchains) {
        if (tc.language == "CXX") {
            cxx = tc.compiler.path
        }
    }
    return <- out if (empty(cxx) || cc_is_cl(cxx) != want_cl)
    var model : CmCodeModel
    let model_text = fread(path_join(reply.dir, reply.codemodel_file))
    return <- out if (!sscan_json(model_text, model) || empty(model.configurations))
    var chosen = 0
    for (i, c in count(), model.configurations) {
        if (c.name == config) {
            chosen = i
        }
    }
    for (target_ref in model.configurations[chosen].targets) {
        var target : CmTarget
        continue if (!sscan_json(fread(path_join(reply.dir, target_ref.jsonFile)), target))
        for (group in target.compileGroups) {
            continue if (group.language != "CXX")
            var inscope flags <- cm_group_flags(group, want_cl)
            for (si in group.sourceIndexes) {
                let key = cc_file_key(model.paths.source, target.sources[si].path)
                if (!(out |> key_exists(key))) {
                    out[key] := flags
                }
            }
        }
    }
    return <- out
}


//! every source's flags under `build_dir`, and where they came from
[arch(at="README.md#cpp-flags")]
def public load_build_flags(build_dir, config : string; want_cl : bool;
                            configure : block<() : int>) : BuildFlags {
    var res : BuildFlags
    return <- res if (empty(build_dir))
    if (fexist(path_join(build_dir, "compile_commands.json"))) {
        res.flags <- load_compile_commands(build_dir, want_cl)
        res.origin = "compile_commands"
        return <- res
    }
    res.origin = "the CMake code model"
    let reply = code_model_reply(build_dir)
    if (empty(reply.codemodel_file) || empty(reply.toolchains_file)) {
        if (!write_code_model_query(build_dir)) {
            res.configure_error = "cannot write the code model query under {build_dir}"
            return <- res
        }
        let rc = invoke(configure)
        if (rc != 0) {
            res.configure_error = "configuring {build_dir} for its code model failed (rc={rc})"
        }
    }
    res.flags <- load_code_model(build_dir, config, want_cl)
    return <- res
}
