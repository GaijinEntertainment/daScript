options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/json
require dastest/review_gate
require daslib/clargs   // CommandInfo: the flag names the parser itself registers
require cli_args        // the CLI's [CommandLineArgs] structs

// The mechanical half of utils/dasllama-server/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang utils/dasllama-server/REVIEW.das.
// Fixtures are captured wire bytes, swept by KIND: a text capture must carry no latin-1
// round-trip mojibake (the raw Â pair or its \u00c2 escape spelling — the PowerShell/cp1252
// trap) and no machine-local username (the rail normalizes captures to C:/Users/user), a JSON
// one must parse, the wav must carry the RIFF/WAVE header the route answers with, and a kind
// with no arm here is itself a finding. Every path-shaped route registration appears in the
// README and rst endpoint tables, and no surface documents or consumes a route the server
// does not register. Every long flag main.das and cli_args.das parse has its mention in the
// README and in the program's rst page. Every license the SDK installs at its doc root for the
// runtime, the shipped modules and the engine ships in the bundle's licenses/ under the same name.

let FIXTURES_DIR = "utils/dasllama-server/tests/fixtures"
let PACKAGE_DAS = "utils/dasllama-server/.das_package"
let SERVER_DAS = "utils/dasllama-server/openai_server.das"
let MAIN_DAS = "utils/dasllama-server/main.das"
let CLI_ARGS_DAS = "utils/dasllama-server/cli_args.das"
let README_MD = "utils/dasllama-server/README.md"
let SERVER_RST = "doc/source/reference/utils/dasllama_server.rst"
let CLI_RST = "doc/source/reference/utils/dasllama_cli.rst"
let CONTROL_HTML = "utils/dasllama-server/control.html"

var DOC_EXEMPT_ROUTES <- {
    "/",
    "/favicon.ico"
}

def is_route_byte(b : int) : bool => is_alnum(b) || b == '/' || b == '_' || b == '-'

// every path the server answers: the front's ROUTES rows, and the GET( / POST( / STREAM( / ANY( rows of onInit
def served_routes() : table<string> {
    var routes : table<string>
    for (raw in split(fread(SERVER_DAS), "\n")) {
        let line = strip(raw)
        var open_at = -1
        for (m in ["RouteRow(path = \"", "GET(\"", "POST(\"", "STREAM(\"", "ANY(\""]) {
            if (line |> starts_with(m)) {
                open_at = length(m)
                break
            }
        }
        continue if (open_at < 0)
        let close_at = find(line, "\"", open_at)
        continue if (close_at <= open_at)
        let path = slice(line, open_at, close_at)
        if (path |> starts_with("/")) {
            routes |> insert(path)
        }
    }
    return <- routes
}

// route-shaped inline literals between the given quote marks
def quoted_routes(text : string; quote : string; var out : table<string>) {
    let ql = length(quote)
    var lits : array<string>
    peek_data(text) $(d) {
        var from = 0
        while (true) {
            let open_at = find(d, quote, from)
            break if (open_at < 0)
            let close_at = find(d, quote, open_at + ql)
            break if (close_at < 0)
            from = close_at + ql
            continue if (close_at - (open_at + ql) < 2 || int(d[open_at + ql]) != '/')
            var ok = true
            for (i in range(open_at + ql + 1, close_at)) {
                if (!is_route_byte(int(d[i]))) {
                    ok = false
                }
            }
            if (ok) {
                var lit = ""
                for (i in range(open_at + ql, close_at)) {
                    lit += to_char(int(d[i]))
                }
                lits |> push(lit)
            }
        }
    }
    for (lit in lits) {
        out |> insert(lit)
    }
}

def check_routes_in_surfaces() {
    var inscope served <- served_routes()
    let readme = fread(README_MD)
    let rst = fread(SERVER_RST)
    for (path in keys(served)) {
        continue if (key_exists(DOC_EXEMPT_ROUTES, path))
        if (find(readme, "`{path}`") < 0) {
            gate_finding(README_MD, "served route {path} has no `{path}` mention - the README documents every route the server registers (the endpoint table is its home)")
        }
        if (find(rst, "``{path}``") < 0) {
            gate_finding(SERVER_RST, "served route {path} has no ``{path}`` mention - the shipped page documents every route the server registers (the endpoint table is its home)")
        }
    }
    var inscope in_readme : table<string>
    quoted_routes(readme, "`", in_readme)
    for (path in keys(in_readme)) {
        if (!key_exists(served, path) && !key_exists(DOC_EXEMPT_ROUTES, path)) {
            gate_finding(README_MD, "documents route {path}, which the server does not register - a documented-but-dead route misleads every client")
        }
    }
    var inscope in_rst : table<string>
    quoted_routes(rst, "``", in_rst)
    for (path in keys(in_rst)) {
        if (!key_exists(served, path) && !key_exists(DOC_EXEMPT_ROUTES, path)) {
            gate_finding(SERVER_RST, "documents route {path}, which the server does not register - a documented-but-dead route misleads every client")
        }
    }
    var inscope in_control : table<string>
    let control = fread(CONTROL_HTML)
    quoted_routes(control, "\"", in_control)
    quoted_routes(control, "'", in_control)
    quoted_routes(control, "`", in_control)
    for (path in keys(in_control)) {
        if (!key_exists(served, path) && !key_exists(DOC_EXEMPT_ROUTES, path)) {
            gate_finding(CONTROL_HTML, "the control page consumes route {path}, which the server does not register")
        }
    }
}

// ===== flags: every long flag a program parses is documented in the README and its rst page =====

//! The documented spelling of a registered long flag: `--show-help` is the host-safe spelling of
//! the `--help` the docs write (the daslang host takes `--help` for itself under the JIT).
def documented_flag(flag_name : string) : string {
    let bare = (flag_name |> starts_with("--")) ? slice(flag_name, 2) : flag_name   // clargs' info carries the dashes, main.das's field names do not
    return bare == "show-help" ? "--help" : "--{bare}"
}

//! The long flags of one [CommandLineArgs] struct as clargs registers them, tagged with the
//! struct's name.
def cli_flags(info : CommandInfo; owner : string; var out : array<tuple<flag : string; owner : string>>) {
    out |> reserve(length(out) + length(info.args))
    for (a in info.args) {
        if (empty(a.doc_string)) {
            gate_finding(CLI_ARGS_DAS, "flag {documented_flag(a.flag_name)} of {owner} has no @clarg_doc - the help text is the first copy every other copy states the same as")
        }
        out |> push((flag = documented_flag(a.flag_name), owner = owner))
    }
}

//! main.das's ServerArgs flags off the source: the field lines of the struct block, a preceding
//! `@clarg_name` winning over the field's own underscore-to-dash spelling. Not clargs'
//! `get_command_info` as the CLI structs: main.das requires the engine, which the gate does not load.
def server_flags(var out : array<tuple<flag : string; owner : string>>) {
    var inside = false
    var named = ""
    var documented = false
    for (raw in split(fread(MAIN_DAS), "\n")) {
        let line = strip(raw)
        if (!inside) {
            inside = line |> starts_with("struct ServerArgs")
            continue
        }
        break if (line == "}")
        if (line |> starts_with("@clarg_name")) {
            let q1 = find(line, "\"")
            let q2 = find(line, "\"", q1 + 1)
            named = slice(line, q1 + 1, q2)
            continue
        }
        if (line |> starts_with("@clarg_doc")) {
            documented = true
            continue
        }
        continue if (empty(line) || (line |> starts_with("@")) || (line |> starts_with("//")))
        let colon = find(line, " : ")
        continue if (colon <= 0)
        let field = slice(line, 0, colon)
        let flag = documented_flag(empty(named) ? replace(field, "_", "-") : named)
        if (!documented) {
            gate_finding(MAIN_DAS, "flag {flag} of ServerArgs has no @clarg_doc - the help text is the first copy every other copy states the same as")
        }
        out |> push((flag = flag, owner = "ServerArgs"))
        named = ""
        documented = false
    }
}

def flags_in_surface(flags : array<tuple<flag : string; owner : string>>; source, surface, quote : string) {
    let text = fread(surface)
    for (f in flags) {
        if (find(text, "{quote}{f.flag}{quote}") < 0) {
            gate_finding(surface, "flag {f.flag} of {f.owner} ({source}) has no {quote}{f.flag}{quote} mention - every flag a program parses is documented where its user reads about it (the flag table is its home)")
        }
    }
}

def check_flags_in_surfaces() {
    var inscope server : array<tuple<flag : string; owner : string>>
    server_flags(server)
    if (empty(server)) {
        gate_finding(MAIN_DAS, "the ServerArgs scan found no flags - the struct block moved or changed shape; teach the gate the new shape")
    }
    flags_in_surface(server, MAIN_DAS, README_MD, "`")
    flags_in_surface(server, MAIN_DAS, SERVER_RST, "``")
    var inscope cli : array<tuple<flag : string; owner : string>>
    cli_flags(get_command_info(type<CommonArgs>), "CommonArgs", cli)
    cli_flags(get_command_info(type<SamplerArgs>), "SamplerArgs", cli)
    cli_flags(get_command_info(type<PlayArgs>), "PlayArgs", cli)
    cli_flags(get_command_info(type<CompleteArgs>), "CompleteArgs", cli)
    cli_flags(get_command_info(type<ChatArgs>), "ChatArgs", cli)
    cli_flags(get_command_info(type<TranscribeArgs>), "TranscribeArgs", cli)
    cli_flags(get_command_info(type<SpeakArgs>), "SpeakArgs", cli)
    cli_flags(get_command_info(type<TalkArgs>), "TalkArgs", cli)
    cli_flags(get_command_info(type<EmbedArgs>), "EmbedArgs", cli)
    cli_flags(get_command_info(type<TokenizeArgs>), "TokenizeArgs", cli)
    cli_flags(get_command_info(type<BenchArgs>), "BenchArgs", cli)
    flags_in_surface(cli, CLI_ARGS_DAS, README_MD, "`")
    flags_in_surface(cli, CLI_ARGS_DAS, CLI_RST, "``")
}

// a fixture is read as BYTES: a string read stops at the first NUL, so the captured wav's tail
// never reached the scan at all
def private read_bytes(path : string; size : uint64; var out : array<uint8>) : bool {
    out |> resize(int64(size))
    var got = 0
    fopen(path, "rb") $(f) {
        if (f != null) {
            got = fread(f, out)
        }
    }
    return got == length(out)
}

def private has_mojibake_bytes(d : array<uint8>) : bool {
    let n = length(d)
    for (i in range(n - 1)) {
        if (int(d[i]) == 0xC3 && int(d[i + 1]) == 0x82) return true
    }
    return false
}

// the ascii-lowered copy the escape search runs over, so \u00C2 reads the same as \u00c2
def private lowered(d : array<uint8>; var out : array<uint8>) {
    out |> resize(length(d))
    for (b, o in d, out) {
        o = uint8(int(b) >= 'A' && int(b) <= 'Z' ? int(b) + 32 : int(b))
    }
}

def private foreign_username(d : array<uint8>) : string {
    var bad = ""
    let n = length(d)
    var from = 0
    while (empty(bad)) {
        let idx = find(d, "Users", from)
        break if (idx < 0)
        var i = idx + 5
        while (i < n && (int(d[i]) == '\\' || int(d[i]) == '/')) {
            i++
        }
        let start = i
        while (i < n && (is_alnum(int(d[i])) || int(d[i]) == '_' || int(d[i]) == '.' || int(d[i]) == '-')) {
            i++
        }
        let name = build_string() $(writer) {
            writer |> write_string(d, start, i)
        }
        if (!empty(name) && name != "user") {
            bad = name
        }
        from = idx + 5
    }
    return bad
}

def private scan_text(path : string; d : array<uint8>) {
    var inscope low : array<uint8>
    lowered(d, low)
    if (has_mojibake_bytes(d) || find(low, "\\u00c2") >= 0) {
        gate_finding(path, "latin-1 mojibake (Â) in a captured fixture — re-capture per tests/fixtures/README.md with a UTF-8-safe writer, never hand-edit")
    }
    let uname = foreign_username(d)
    if (!empty(uname)) {
        gate_finding(path, "machine-local username '{uname}' — the capture rail normalizes home paths to C:/Users/user")
    }
}

// the wav the speech route answers with is PCM, so the text scans say nothing about it; its
// header is what a hand-written body would not carry
def private is_riff_wave(d : array<uint8>) : bool {
    return false if (length(d) < 12)
    return find(d, "RIFF") == 0 && find(d, "WAVE") == 8
}

// the programs the bundle bakes: the server, the CLI and the benchmark companion
var BUNDLE_ENTRY_FILES <- [
    "utils/dasllama-server/main.das",
    "utils/dasllama-server/cli.das",
    "modules/dasLLAMA/benchmarks/lcpp_bench.das"
]

// `das_metal`, `dasMetal` and `metal` name one folder
def private module_folder_key(name : string) : string {
    let k = to_lower(replace(name, "_", ""))
    return k |> starts_with("das") ? slice(k, 3) : k
}

def private module_folders() : table<string; string> {
    var out : table<string; string>
    dir("modules") $(name) {
        return if (name |> starts_with("."))
        let st = stat("modules/{name}")
        return if (!st.is_valid || !st.is_dir)
        out[module_folder_key(name)] = "modules/{name}"
    }
    return <- out
}

//! The CMake files whose doc-root notices the bundle answers for: the core, plus every module
//! folder a `require` of a baked program reaches - transitively through the `.das` files the
//! requires name. A C++ module ships as a shared module beside the exe, a daslang-only one is
//! compiled into it; either way its notices' terms ride along. Every platform's branch counts:
//! the set is the union over the guarded requires, so a notice a platform's bundle does not need
//! still ships there.
def private bundle_license_owners() : array<string> {
    var inscope folders <- module_folders()
    var owners : table<string>
    owners |> insert("CMakeLists.txt")
    var seen : table<string>
    var queue := BUNDLE_ENTRY_FILES
    while (!empty(queue)) {
        let f = queue[length(queue) - 1]
        queue |> pop()
        continue if (key_exists(seen, f) || !fexist(f))
        seen |> insert(f)
        let fdir = dir_name(f)
        for (r in das_requires(f)) {
            continue if (r |> starts_with("[") || r |> starts_with("daslib/") || r |> starts_with("dastest/"))
            if (r |> ends_with(".das")) {
                queue |> push(path_join(fdir, r))
                continue
            }
            let slash = find(r, "/")
            let head = slash < 0 ? r : slice(r, 0, slash)
            let folder = folders?[module_folder_key(head)] ?? ""
            if (empty(folder)) {
                queue |> push(path_join(fdir, "{r}.das")) if (slash < 0)
                continue
            }
            owners |> insert("{folder}/CMakeLists.txt")
            queue |> push("{folder}/{r}.das")
        }
    }
    var out <- [for (k in keys(owners)); k]
    sort(out)
    return <- out
}

def private is_notice_file(name : string) : bool =>
    find(name, "LICENSE") >= 0 || find(name, "COPYING") >= 0 || find(name, "NOTICES") >= 0

//! The `install(FILES <src>... DESTINATION $\{DAS_INSTALL_DOCDIR\} [RENAME <name>])` rows of one
//! CMake file as (source relative to the repo root, name at the doc root). A source under
//! examples/ rides with the examples, which the bundle does not carry.
def private doc_root_licenses(owner : string) : array<tuple<source : string; name : string>> {
    var out : array<tuple<source : string; name : string>>
    let module_dir = owner == "CMakeLists.txt" ? "" : dir_name(owner)
    for (blk in cmake_command_blocks(fread(owner), "install")) {
        var inscope args <- cmake_args(blk)
        continue if (length(args) < 3 || args[0] != "FILES")
        var dest = ""
        var rename = ""
        var sources : array<string>
        var i = 1
        while (i < length(args)) {
            let a = args[i]
            if (a == "DESTINATION" && i + 1 < length(args)) {
                dest = args[i + 1]
                i += 2
            } elif (a == "RENAME" && i + 1 < length(args)) {
                rename = args[i + 1]
                i += 2
            } else {
                sources |> push(a)   // a bare all-caps name is a file here, not a keyword: the notice filter below sorts them
                i++
            }
        }
        continue if (dest != "$\{DAS_INSTALL_DOCDIR\}")
        for (src in sources) {
            var rel = ""
            if (src |> starts_with("$\{PROJECT_SOURCE_DIR\}/")) {
                rel = slice(src, length("$\{PROJECT_SOURCE_DIR\}/"))
            } elif (src |> starts_with("$\{DAS_") && !empty(module_dir)) {
                let close_at = find(src, "\}/")
                rel = close_at < 0 ? "" : "{module_dir}/{slice(src, close_at + 2)}"
            } elif (!(src |> starts_with("$"))) {
                rel = empty(module_dir) ? src : "{module_dir}/{src}"
            }
            if (empty(rel)) {
                gate_finding(owner, "doc-root install source `{src}` names a variable this gate cannot resolve - teach doc_root_licenses in REVIEW.das the spelling, so the bundle's license list stays checkable")
                continue
            }
            let name = empty(rename) ? base_name(rel) : rename
            continue if (!is_notice_file(name) || rel |> starts_with("examples/"))
            out |> push((source = rel, name = name))
        }
    }
    return <- out
}

//! Every doc-root notice for what the bundle carries has its `release_include_from("<source>",
//! "licenses/<name>")` row in the package, and every such row is backed by an install rule.
def check_bundle_licenses() {
    var live_rows : table<string>   // the package's uncommented licenses/ rows
    for (raw in split(fread(PACKAGE_DAS), "\n")) {
        let line = strip(raw)
        continue if (!(line |> starts_with("release_include_from(")) || find(line, "\"licenses/") < 0)
        let close_at = find(line, ")")
        continue if (close_at < 0)
        live_rows |> insert(slice(line, 0, close_at + 1))
    }
    var expected : table<string>
    for (owner in bundle_license_owners()) {
        continue if (!fexist(owner))
        for (lic in doc_root_licenses(owner)) {
            let row = "release_include_from(\"{lic.source}\", \"licenses/{lic.name}\")"
            expected |> insert(row)
            if (!key_exists(live_rows, row)) {
                gate_finding(PACKAGE_DAS, "the bundle carries no licenses/{lic.name}, which {owner} installs at the SDK doc root - add {row}")
            }
        }
    }
    for (row in keys(live_rows)) {
        if (!key_exists(expected, row)) {
            gate_finding(PACKAGE_DAS, "{row} ships a notice no doc-root install rule of the runtime or a module the baked programs require backs - drop the row")
        }
    }
}

[export]
def main() : int {
    if (!fexist("utils/dasllama-server/REVIEW.das")) {
        to_log(LOG_ERROR, "utils/dasllama-server/REVIEW.das: run from the repo root\n")
        return 2
    }
    dir(FIXTURES_DIR) $(name) {
        return if (name == "." || name == "..")
        let p = "{FIXTURES_DIR}/{name}"
        let st = stat(p)
        return if (!st.is_valid || st.is_dir)
        var inscope bytes : array<uint8>
        if (!read_bytes(p, st.size, bytes)) {
            gate_finding(p, "could not be read as bytes - the sweep must see every byte of every capture")
            return
        }
        // the captured text bodies, plus the folder's own rail doc and capture script
        if (ends_with(name, ".json") || ends_with(name, ".txt") || ends_with(name, ".md")
            || ends_with(name, ".das") || ends_with(name, ".gitattributes")) {
            scan_text(p, bytes)
        } elif (ends_with(name, ".wav")) {
            if (!is_riff_wave(bytes)) {
                gate_finding(p, "no RIFF/WAVE header - a captured speech body carries one; re-capture per tests/fixtures/README.md")
            }
        } else {
            gate_finding(p, "fixture kind this gate cannot scan - teach REVIEW.das what to check for it in the same change that adds it")
        }
        if (ends_with(name, ".json")) {
            var jerr = ""
            var js = read_json(fread(p), jerr)
            if (js == null) {
                gate_finding(p, "fixture does not parse as JSON: {jerr}")
            } else {
                unsafe {
                    delete js
                }
            }
        }
    }
    check_routes_in_surfaces()
    check_flags_in_surfaces()
    check_test_ports()
    check_bundle_licenses()
    return gate_verdict("dasllama-server")
}

let TESTS_DIR = "utils/dasllama-server"

//! Every `[test]` file's boot ports - a literal `let NAME = 18xxx` and every `NAME + n` beside
//! it - claimed once across the folder: two suites on one port bind-fail, or answer from the
//! other's server.
def check_test_ports() {
    var inscope claims : table<int; string>   // port -> the file that claims it
    dir(TESTS_DIR) $(name) {
        return if (!(name |> starts_with("test_")) || !(name |> ends_with(".das")))
        let path = "{TESTS_DIR}/{name}"
        let text = fread(path)
        var inscope bases : table<string; int>
        for (raw in split(text, "\n")) {
            let line = strip(raw)
            continue if (!(line |> starts_with("let ")))
            let eq = find(line, " = 18")
            continue if (eq < 4)
            let port_txt = slice(line, eq + 3, eq + 8)
            continue if (length(port_txt) != 5)
            let port = to_int(port_txt)
            continue if (port < 18000 || port > 18999)
            bases[slice(line, 4, eq)] = port
            claim_port(claims, port, path)
        }
        peek_data(text) $(d) {
            let n = length(d)
            for (base, port in keys(bases), values(bases)) {
                var at = 0
                while (true) {
                    let hit = find(d, "{base} + ", at)
                    break if (hit < 0)
                    at = hit + length(base) + 3
                    continue if (hit > 0 && (is_alnum(int(d[hit - 1])) || int(d[hit - 1]) == '_'))
                    var off = 0
                    var i = at
                    while (i < n && int(d[i]) >= '0' && int(d[i]) <= '9') {
                        off = off * 10 + int(d[i]) - '0'
                        i++
                    }
                    claim_port(claims, port + off, path) if (off > 0)
                }
            }
        }
    }
}

def private claim_port(var claims : table<int; string>; port : int; path : string) {
    let owner = claims?[port] ?? ""
    if (!empty(owner) && owner != path) {
        gate_finding(path, "boots a server on port {port}, which {owner} claims too - give the file a range no sibling claims")
        return
    }
    claims[port] = path
}
