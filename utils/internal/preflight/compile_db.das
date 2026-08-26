options gen2
options indenting = 4

// compile_commands.json reader for preflight's cpp-syntax gate. Lives beside main.das as a
// module so the tokenizer can be tested directly - its failure mode (a mangled path) is
// silent, and it only bites on a platform the author may not be running.

module compile_db shared

require daslib/fio
require strings


struct CcEntry {
    directory : string
    command : string
    file : string
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
def public cc_is_compile_switch(a : string) : bool {
    if (a == "-c" || a == "/c" || a == "-MD" || a == "-MMD"
        || a == "/showIncludes" || a == "-showIncludes") {
        return true
    }
    for (p in ["/Fo", "-Fo", "/Fd", "-Fd", "/Fp", "-Fp"]) {
        return true if (a |> starts_with(p))
    }
    return false
}


// The exact flags CMake compiles each TU with, keyed by absolute source path. The gate's own
// include list is a hand-rolled approximation: it models neither build-generated headers
// (tests-cpp/big/standalone_ctx/_generated) nor per-module include dirs, and false-fails any
// TU that needs one. This reflects the LOCAL configure's module ON/OFF set rather than CI's
// all-modules-on lane - still strictly closer than the approximation, which models no module
// include dirs at all. Empty table = fall back to the hand list.
def public load_compile_commands(build_dir : string; want_cl : bool) : table<string; array<string>> {
    var out : table<string; array<string>>
    return <- out if (empty(build_dir))
    let raw = fread(path_join(build_dir, "compile_commands.json"))
    return <- out if (empty(raw))
    var db : array<CcEntry>
    return <- out if (!sscan_json(raw, db))
    for (e in db) {
        let argv <- cc_tokenize(e.command)
        continue if (length(argv) < 2)
        // the format allows `file` relative to `directory`; the caller looks up by absolute
        // path, and a miss falls back silently
        let file_abs = is_absolute(e.file) ? e.file : path_join(e.directory, e.file)
        // the entry's compiler flavor has to match the binary the gate runs, or its flag
        // spelling is wrong for it
        let entry_is_cl = argv[0] |> ends_with("cl") || argv[0] |> ends_with("cl.exe")
        continue if (entry_is_cl != want_cl)
        var flags : array<string>
        flags |> reserve(length(argv))
        var i = 1
        while (i < length(argv)) {
            let a = argv[i]
            if (a == "-o" || a == "-MT" || a == "-MF") {
                i += 2
            } elif (a == e.file || a == file_abs || cc_is_compile_switch(a)) {
                i++
            } else {
                flags |> push(a)
                i++
            }
        }
        out[file_abs] <- flags
    }
    return <- out
}
