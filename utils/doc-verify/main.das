options gen2
options persistent_heap
options indenting = 4

// doc-verify — compile-checks the das code blocks of authored RST pages.
//
// Model: each page is a literate program. Blocks concatenate in document order into
// one synthetic module — decls at module scope, statements into one function so locals
// flow block to block. daslang bans shadowing, so a narrative re-declaration renames
// (`name__N`) from that point on; a class re-declared with a leading `...` merges into
// the original (elision means "previous members here"). Preamble (require/options)
// comes from the page's companion tutorial .das when one exists.
//
// Markers, written as RST comments directly above a block (invisible when rendered):
//   .. das-doc: skip                 -- not code (wire formats, output dumps)
//   .. das-doc: fresh                -- start a new synthetic program here
//   .. das-doc: given <one-line das> -- inject context the prose assumes
//   .. das-doc: signatures           -- API listing; compiler skips, agents verify
//   .. das-doc: expect error[NNNNN]  -- block must FAIL to compile with this code
//
// Rule 0: before any page verdict, every require the corpus depends on is probed
// against the checker binary. A companion-sourced require that fails to load is a
// binary gap and ABORTS the audit; a page-sourced require that fails only reds
// that page (the page itself may be wrong).

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings
require math

let AREAS = [
    "doc/source/reference/tutorials",
    "doc/source/reference/language",
    "doc/source/reference/utils",
    "doc/source/stdlib/handmade"
]

let ELIDE = "__DOC_ELIDE__"

[CommandLineArgs]
struct Config {
    @clarg_doc = "Repository root (default: current directory)"
    root : string = "."

    @clarg_doc = "Output directory for synthetic modules and reports"
    out : string = "build/doc_verify"

    @clarg_doc = "Path to the daslang binary used for compile checks"
    daslang : string = "bin/daslang"

    @clarg_short = "p"
    @clarg_doc = "Only process pages whose path contains this substring"
    page : string

    @clarg_doc = "Path for the JSON report (default: <out>/report.json)"
    json : string

    @clarg_doc = "Skip rule-0 module probes (NOT for audit runs)"
    no_probes : bool

    @clarg_short = "v"
    @clarg_doc = "Per-page progress"
    verbose : bool
}

// ===== RST extraction =====

struct RstBlock {
    rst_line : int
    marker : string
    marker_arg : string
    body : array<string>
}

struct PageSource {
    path : string
    blocks : array<RstBlock>
    givens : array<string>
}

def leading_spaces(s : string) : int {
    // tabs count as indentation too — a tab-indented block line must not read as
    // column 0, or the whole block silently drops from extraction
    var n = 0
    peek_data(s) $(arr) {
        for (c in arr) {
            break if (int(c) != ' ' && int(c) != '\t')
            n++
        }
    }
    return n
}

def parse_marker(trimmed : string; var marker : string&; var marker_arg : string&) {
    let rest = trim(slice(trimmed, length(".. das-doc:")))
    let sp = find(rest, ' ')
    if (sp < 0) {
        marker = rest
        marker_arg = ""
    } else {
        marker = slice(rest, 0, sp)
        marker_arg = trim(slice(rest, sp + 1))
    }
}

def parse_page(path : string) : PageSource {
    var page = PageSource(path = path)
    let lines <- split(fread(path), "\n")
    var pending_marker = ""
    var pending_arg = ""
    var i = 0
    while (i < length(lines)) {
        let trimmed = trim(lines[i])
        if (trimmed |> starts_with(".. das-doc:")) {
            var m, a : string
            parse_marker(trimmed, m, a)
            if (m == "given") {
                page.givens |> push(a)
            } else {
                pending_marker = m
                pending_arg = a
            }
            i++
            continue
        }
        if (!(trimmed |> starts_with(".. code-block:: das")) || trimmed != ".. code-block:: das") {
            i++
            continue
        }
        let base_indent = leading_spaces(lines[i])
        var j = i + 1
        while (j < length(lines) && (empty(trim(lines[j])) || (trim(lines[j]) |> starts_with(":")))) {
            j++
        }
        var blk = RstBlock(rst_line = j + 1, marker = pending_marker, marker_arg = pending_arg)
        pending_marker = ""
        pending_arg = ""
        while (j < length(lines)) {
            let ln = lines[j]
            if (empty(trim(ln))) {
                blk.body |> push("")
                j++
                continue
            }
            break if (leading_spaces(ln) <= base_indent)
            blk.body |> push(ln)
            j++
        }
        while (!empty(blk.body) && empty(blk.body[length(blk.body) - 1])) {
            blk.body |> pop()
        }
        if (!empty(blk.body)) {
            dedent(blk.body)
            page.blocks |> emplace(blk)
        }
        i = j
    }
    return <- page
}

def dedent(var body : array<string>) {
    var min_ind = 1000
    for (l in body) {
        continue if (empty(trim(l)))
        min_ind = min(min_ind, leading_spaces(l))
    }
    return if (min_ind == 0 || min_ind == 1000)
    for (l in body) {
        l = empty(trim(l)) ? "" : slice(l, min_ind)
    }
}

// ===== chunking =====

let DECL_STARTERS = [
    "def ", "struct ", "class ", "enum ", "variant ", "typedef ", "bitfield ", "tuple "
]

def is_decl_start(line : string) : bool {
    for (s in DECL_STARTERS) {
        return true if (line |> starts_with(s))
    }
    // `private`/`shared` var-decls are module-scope-only grammar — a col-0 plain `var`
    // stays a narrative statement, but these can only be globals
    for (s in ["var private ", "let private ", "var shared ", "let shared ", "var public ", "let public "]) {
        return true if (line |> starts_with(s))
    }
    return line |> starts_with("[") || line |> starts_with("@")
}

def is_hoist(line : string) : bool {
    return (line |> starts_with("require ")) || (line |> starts_with("options ")) || (line |> starts_with("module "))
}

struct Chunk {
    kind : string   // "hoist" | "decl" | "stmt"
    lines : array<string>
}

def flush_chunk(var chunks : array<Chunk>; var cur : array<string>; var kind : string&) {
    if (!empty(cur)) {
        chunks |> emplace(Chunk(kind = kind, lines <- cur))
    }
    kind = ""
}

def last_nonblank(lines : array<string>) : string {
    var i = length(lines) - 1
    while (i >= 0) {
        return lines[i] if (!empty(trim(lines[i])))
        i--
    }
    return ""
}

def chunks_of(body : array<string>) : array<Chunk> {
    var chunks : array<Chunk>
    var cur : array<string>
    var kind = ""
    for (ln in body) {
        if (empty(trim(ln))) {
            if (!empty(cur)) {
                cur |> push(ln)
            }
            continue
        }
        let top = leading_spaces(ln) == 0
        if (top && is_hoist(ln)) {
            flush_chunk(chunks, cur, kind)
            chunks |> emplace(Chunk(kind = "hoist", lines <- [ln]))
            continue
        }
        if (top && is_decl_start(ln)) {
            let glue = kind == "decl" && !empty(cur) && ((trim(last_nonblank(cur)) |> starts_with("[")) || (trim(last_nonblank(cur)) |> starts_with("@")))
            if (glue) {
                cur |> push(ln)
                continue
            }
            flush_chunk(chunks, cur, kind)
            kind = "decl"
            cur |> push(ln)
            continue
        }
        if (top && kind == "decl" && (ln |> starts_with("}"))) {
            cur |> push(ln)
            continue
        }
        if (top && kind != "stmt") {
            flush_chunk(chunks, cur, kind)
            kind = "stmt"
        } elif (empty(kind)) {
            kind = "stmt"
        }
        cur |> push(ln)
    }
    flush_chunk(chunks, cur, kind)
    return <- chunks
}

// ===== rename-on-redeclare =====

struct Renamer {
    counts : table<string; int>
    aliases : table<string; string>
    def_headers : table<string>   // exact-duplicate def headers rename; overloads keep the name
}

def is_ident_char(c : int) : bool {
    return is_alpha(c) || is_number(c) || c == '_'
}

def take_ident(s : string) : string {
    var n = 0
    peek_data(s) $(arr) {
        for (c in arr) {
            break if (!is_ident_char(int(c)))
            n++
        }
    }
    return slice(s, 0, n)
}

let TYPE_KEYWORDS = ["class ", "struct ", "enum ", "variant ", "bitfield ", "tuple ", "typedef "]
let TYPE_MODIFIERS = ["private ", "public ", "sealed ", "abstract ", "shared ", "distinct "]

def type_decl_name(line : string) : string {
    var mods : string
    return type_decl_name_mods(line, mods)
}

def type_decl_name_mods(line : string; var mods : string&) : string {
    mods = ""
    for (kw in TYPE_KEYWORDS) {
        continue if (!(line |> starts_with(kw)))
        var rest = trim(slice(line, length(kw)))
        var again = true
        var mod_list : array<string>
        while (again) {
            again = false
            for (m in TYPE_MODIFIERS) {
                if (rest |> starts_with(m)) {
                    mod_list |> push(m)
                    rest = trim(slice(rest, length(m)))
                    again = true
                }
            }
        }
        mods = join(mod_list, "")
        return take_ident(rest)
    }
    return ""
}

// a def with an untyped parameter segment is a GENERIC — contract annotations then
// participate in overload resolution, so annotated same-header generics are distinct
def def_is_generic(line : string) : bool {
    let open = find(line, '(')
    return false if (open < 0)
    let close = find(line, ')')
    return false if (close <= open + 1)
    for (seg in split(slice(line, open + 1, close), ";")) {
        return true if (find(seg, ':') < 0)
    }
    return false
}

// the def name for duplicate-header renaming; "" for operators and non-def lines
def def_decl_name(line : string) : string {
    return "" if (!(line |> starts_with("def ")))
    var rest = trim(slice(line, length("def ")))
    for (m in ["private ", "public ", "static ", "abstract ", "const "]) {
        if (rest |> starts_with(m)) {
            rest = trim(slice(rest, length(m)))
        }
    }
    let name = take_ident(rest)
    return name == "operator" ? "" : name
}

def def_header_key(line : string) : string {
    var cut = length(line)
    let b = find(line, "\{")
    if (b >= 0) {
        cut = min(cut, b)
    }
    let a = find(line, "=>")
    if (a >= 0) {
        cut = min(cut, a)
    }
    var k = trim(slice(line, 0, cut))
    // `def main {` and `def main() {` are the same header
    if (k |> ends_with("()")) {
        k = trim(slice(k, 0, length(k) - 2))
    }
    return k
}

def var_decl_names(line : string) : array<string> {
    var t = trim(line)
    var names : array<string>
    if ((t |> starts_with("var ")) || (t |> starts_with("let "))) {
        t = trim(slice(t, 4))
        var again = true
        while (again) {
            again = false
            for (m in ["inscope ", "private ", "shared ", "public "]) {
                if (t |> starts_with(m)) {
                    t = trim(slice(t, length(m)))
                    again = true
                }
            }
        }
    } elif ((t |> starts_with("for (")) || (t |> starts_with("for("))) {
        let open = find(t, '(')
        let in_pos = find(t, " in ")
        return <- names if (in_pos < 0)
        t = trim(slice(t, open + 1, in_pos))
    } else {
        return <- names
    }
    // tuple destructure: `let (ok, err) = ...` — names live inside the parens.
    // `_` is the discard (binds nothing, repeats freely) — never register or rename it
    if (t |> starts_with("(")) {
        let close = find(t, ')')
        return <- names if (close < 0)
        for (piece in split(slice(t, 1, close), ",")) {
            let name = take_ident(trim(piece))
            if (!empty(name) && name != "_") {
                names |> push(name)
            }
        }
        return <- names
    }
    // truncate at the first initializer/type delimiter — only the name list splits on commas
    var cut = length(t)
    for (d in [find(t, ':'), find(t, '='), find(t, '<'), find(t, '&')]) {
        if (d >= 0) {
            cut = min(cut, d)
        }
    }
    t = slice(t, 0, cut)
    for (piece in split(t, ",")) {
        let name = take_ident(trim(piece))
        if (!empty(name)) {
            names |> push(name)
        }
    }
    return <- names
}

def declare(var ren : Renamer; names : array<string>) {
    for (n in names) {
        let gen = (ren.counts?[n] ?? 0) + 1
        ren.counts[n] = gen
        if (gen > 1) {
            ren.aliases[n] = "{n}__{gen}"
        }
    }
}

def replace_ident(line : string; base : string; alias_name : string) : string {
    return line if (find(line, base) < 0)
    var out = ""
    let blen = length(base)
    peek_data(line) $(arr) {
        // string-literal TEXT is not code — "\n" must not become "\n__2" when `n`
        // aliases — but an unescaped {…} interpolation region inside a string IS
        // code and must rename with the rest of the line
        var in_str : array<bool>
        in_str |> resize(length(arr))
        var q = false
        var depth = 0
        for (i in range(length(arr))) {
            let c = int(arr[i])
            let esc = i > 0 && int(arr[i - 1]) == '\\'
            if (!q) {
                if (c == '"' && !esc) {
                    q = true
                    in_str[i] = true
                } else {
                    in_str[i] = false
                }
            } elif (depth == 0 && c == '"' && !esc) {
                q = false
                in_str[i] = true
            } elif (c == '{' && !esc) {
                depth++
                in_str[i] = true
            } elif (c == '}' && !esc && depth > 0) {
                depth--
                in_str[i] = true
            } else {
                in_str[i] = depth == 0
            }
        }
        out = build_string() $(var writer) {
            var start = 0
            while (true) {
                let pos = find(line, base, start)
                if (pos < 0) {
                    writer |> write(slice(line, start))
                    break
                }
                let before_ok = pos == 0 || (!is_ident_char(int(arr[pos - 1])) && int(arr[pos - 1]) != '.')
                let after_pos = pos + blen
                let after_ok = after_pos >= length(arr) || !is_ident_char(int(arr[after_pos]))
                writer |> write(slice(line, start, pos))
                writer |> write((before_ok && after_ok && !in_str[pos]) ? alias_name : base)
                start = after_pos
            }
        }
    }
    return out
}

def apply_renames(ren : Renamer; line : string) : string {
    var out = line
    for (base, alias_name in keys(ren.aliases), values(ren.aliases)) {
        out = replace_ident(out, base, alias_name)
    }
    return out
}

def rename_line(var ren : Renamer; line : string) : string {
    return rename_line_ex(ren, line, true)
}

// a line declares either a type name or a list of variable names
def private declare_line_names(var ren : Renamer; line : string) {
    let tname = type_decl_name(line)
    if (!empty(tname)) {
        declare(ren, [tname])
    } else {
        declare(ren, var_decl_names(line))
    }
}

def rename_line_ex(var ren : Renamer; line : string; declare_ok : bool) : string {
    if (declare_ok) {
        declare_line_names(ren, line)
    }
    return apply_renames(ren, line)
}

// ===== page emission =====

struct DeclChunk {
    name : string
    mods : string   // modifier words of the header; a differing set is a new example, not a merge
    lines : array<string>
}

struct Segment {
    decls : array<DeclChunk>
    stmts : array<string>
}

struct EmittedPage {
    files : array<string>
    checked : int
    skipped : int
    fragments : int
    expects : array<RstBlock>
    require_lines : array<string>
    companion_requires : table<string>   // lines that came from the companion preamble
    file_written : table<string>    // sibling modules minted by `file` markers this page
    companion_backed : bool
}

def emit_alt_block(blk : RstBlock; hoists : array<string>; givens : array<string>; out_base : string; var files : array<string>) {
    // `alt` compiles the block as its own program (hoists + givens + this block only) —
    // the way to show an alternative spelling of a member the page program already has
    var aseg = Segment()
    var aren = Renamer()
    seed_renamer(aren, givens)
    var body := blk.body
    mark_elisions(body)
    var ahoists := hoists
    let tag = "// --- alt block at rst:{blk.rst_line}"
    for (chunk in chunks_of(body)) {
        if (chunk.kind == "hoist") {
            for (h in chunk.lines) {
                let t = trim(h)
                continue if (t == ELIDE)
                ahoists |> push(t)
            }
        } elif (chunk.kind == "decl") {
            append_decl(aseg, aren, chunk, tag)
        } else {
            append_stmts(aseg, aren, chunk, tag)
        }
    }
    flush_segment(aseg, ahoists, givens, "{out_base}__alt_{blk.rst_line}.das", files)
}

def strip_comment(line : string) : string {
    let c = find(line, "//")
    return c < 0 ? line : trim(slice(line, 0, c))
}

// a same-dir companion require (`require tutorial_server`) only resolves from the
// companion's own directory — rewrite it file-relative to the synthetic pages dir
def rewrite_sibling_require(line : string; comp_dir : string; pages_dir : string) : string {
    let t = strip_comment(line)
    return line if (!(t |> starts_with("require ")))
    var target = trim(slice(t, length("require ")))
    var suffix = ""
    if (target |> ends_with(" public")) {
        suffix = " public"
        target = trim(trim_suffix(target, " public"))
    }
    return line if (find(target, "/") >= 0 || find(target, ".") >= 0)
    let sibling = path_join(comp_dir, target + ".das")
    return line if (!fexist(sibling))
    var err : string
    let rel = relative(comp_dir, pages_dir, err)
    return line if (!empty(err))
    return "require {to_generic_path(path_join(rel, target))}.das{suffix}"
}

def companion_preamble(rst_path : string; root : string; pages_dir : string) : array<string> {
    var pre : array<string>
    let base = stem(rst_path)
    var cands : array<string>
    let us = find(base, '_')
    if (us > 0 && !is_number(first_character(base))) {
        let fam = slice(base, 0, us)
        let tail = slice(base, us + 1)
        let fam_dir = path_join(root, path_join("tutorials", fam))
        cands |> push(path_join(fam_dir, tail + ".das"))
        cands |> push(path_join(fam_dir, replace(tail, "_", "-") + ".das"))
        // sql-style naming: hyphen after the number only (02-insert_data.das)
        let t2 = find(tail, '_')
        if (t2 > 0) {
            cands |> push(path_join(fam_dir, slice(tail, 0, t2) + "-" + slice(tail, t2 + 1) + ".das"))
        }
        // number-prefix fallback: the slug can differ entirely (sql_01_hello.rst
        // vs tutorials/sql/01-version.das) — any companion sharing the number wins
        var dig = 0
        peek_data(tail) $(arr) {
            for (c in arr) {
                break if (!is_number(int(c)))
                dig++
            }
        }
        if (dig > 0) {
            var globbed : array<string>
            expand_glob(path_join(fam_dir, slice(tail, 0, dig) + "*.das"), globbed)
            cands |> push_from(globbed)
        }
    }
    if (length(base) > 2 && is_number(first_character(base))) {
        cands |> push(path_join(root, path_join("tutorials/language", base + ".das")))
    }
    // some families keep companions under the page's own basename, or in a module tree
    let gpath = to_generic_path(rst_path)
    if (find(gpath, "/tutorials/imgui/") >= 0) {
        cands |> push(path_join(root, path_join("modules/dasImgui/examples/tutorial", base + ".das")))
    }
    if (us > 0 && !is_number(first_character(base))) {
        let fam2 = slice(base, 0, us)
        cands |> push(path_join(root, path_join("tutorials", path_join(fam2, base + ".das"))))
    }
    // subdir families (tutorials/macros/NN_x.rst): the RST's own directory names the
    // companion family — this is what lets usage blocks compile against the REAL
    // sibling macro modules via the companion's rewritten requires
    let parent = base_name(dir_name(rst_path))
    if (parent != "tutorials") {
        cands |> push(path_join(root, path_join("tutorials", path_join(parent, base + ".das"))))
    }
    for (c in cands) {
        continue if (!fexist(c))
        for (ln in split(fread(c), "\n")) {
            let t = trim(ln)
            if (t |> starts_with("require ")) {
                pre |> push(rewrite_sibling_require(t, dir_name(c), pages_dir))
            } elif ((t |> starts_with("options ")) && find(t, "indenting") < 0) {
                pre |> push(t)
            }
            break if (((t |> starts_with("def ")) || (t |> starts_with("["))) && !empty(pre))
        }
        break if (!empty(pre))
    }
    return <- pre
}

def mark_elisions(var body : array<string>) {
    for (l in body) {
        let t = trim(l)
        if (t == "..." || t == "// ...") {
            l = slice(l, 0, leading_spaces(l)) + ELIDE
            continue
        }
        // inline elided bodies: `if (cond) { ... }` — a brace pair whose sole
        // content is `...` compiles as `{ pass }`
        if (find(l, "\{ ... \}") >= 0) {
            l = replace(l, "\{ ... \}", "\{ pass \}")
        }
        if (find(l, "\{...\}") >= 0) {
            l = replace(l, "\{...\}", "\{ pass \}")
        }
    }
}

// duplicate exact def headers rename; overloads keep the name. For GENERICS the
// annotations join the identity (contracts participate in overload resolution),
// so annotated same-header generics are distinct, not duplicates
def register_def_header(var ren : Renamer; chunk : Chunk) {
    var ann_parts : array<string>
    for (l in chunk.lines) {
        let t = trim(l)
        if ((t |> starts_with("[")) || (t |> starts_with("@"))) {
            ann_parts |> push(t)
            continue
        }
        let dname = def_decl_name(l)
        continue if (empty(dname))
        let hdr = (def_is_generic(l) ? join(ann_parts, "") : "") + def_header_key(l)
        if (key_exists(ren.def_headers, hdr)) {
            declare(ren, [dname])
        } else {
            ren.def_headers |> insert(hdr)
            // claim generation 1 so the FIRST duplicate already aliases to __2
            if ((ren.counts?[dname] ?? 0) == 0) {
                declare(ren, [dname])
            }
        }
        break
    }
}

def append_decl(var seg : Segment; var ren : Renamer; chunk : Chunk; tag : string) {
    let header = chunk.lines[0]
    var mods : string
    let tname = type_decl_name_mods(header, mods)
    // exact-duplicate def header = narrative re-declaration -> rename; overloads keep the name
    if (empty(tname)) {
        register_def_header(ren, chunk)
    }
    var merge_idx = -1
    if (!empty(tname) && (ren.counts?[tname] ?? 0) > 0) {
        // merge only when the body opens with an elision — "previous members here" —
        // AND the modifier set matches (a `sealed` re-declaration is a new example)
        for (i in range(1, length(chunk.lines))) {
            let t = trim(chunk.lines[i])
            continue if (empty(t))
            if (t == ELIDE) {
                for (di in range(length(seg.decls))) {
                    if (seg.decls[di].name == tname && seg.decls[di].mods == mods) {
                        merge_idx = di
                    }
                }
            }
            break
        }
    }
    if (merge_idx >= 0) {
        var members : array<string>
        for (i in range(1, length(chunk.lines))) {
            let t = trim(chunk.lines[i])
            continue if (t == ELIDE || (i == length(chunk.lines) - 1 && t == "}"))
            members |> push(chunk.lines[i])
        }
        var target & = unsafe(seg.decls[merge_idx])
        var close = length(target.lines) - 1
        while (close > 0 && trim(target.lines[close]) != "}") {
            close--
        }
        target.lines |> resize(length(target.lines) + length(members))
        var i = length(target.lines) - 1
        while (i >= close + length(members)) {
            target.lines[i] = target.lines[i - length(members)]
            i--
        }
        for (k, m in count(), members) {
            target.lines[close + k] = m
        }
        return
    }
    var dc = DeclChunk(name = tname, mods = mods)
    dc.lines |> push(tag)
    for (l in chunk.lines) {
        continue if (trim(l) == ELIDE)
        // indented lines inside a decl body are function-local — they get alias
        // APPLICATION but must not claim their names page-wide
        dc.lines |> push(rename_line_ex(ren, l, leading_spaces(l) == 0))
    }
    seg.decls |> emplace(dc)
}

def append_stmts(var seg : Segment; var ren : Renamer; chunk : Chunk; tag : string) {
    // statement lines declare page-wide regardless of indentation: daslang bans
    // shadowing even in nested scopes, so an indented re-declaration must rename,
    // and the alias persisting page-wide is the model the corpus is authored
    // against (a page that needs a name back after a nested binding uses `alt`
    // or renames the local — see the plan ledger)
    seg.stmts |> push("    " + tag)
    for (l in chunk.lines) {
        if (trim(l) == ELIDE) {
            seg.stmts |> push(slice(l, 0, leading_spaces(l)) + "    pass")
            continue
        }
        let r = rename_line_ex(ren, l, true)
        seg.stmts |> push(empty(trim(r)) ? "" : "    " + r)
    }
    seg.stmts |> push("")
}

def is_global_decl(line : string) : bool {
    return (line |> starts_with("var ")) || (line |> starts_with("let "))
}

def seed_renamer(var ren : Renamer; givens : array<string>) {
    for (g in givens) {
        declare_line_names(ren, g)
    }
}

def flush_segment(var seg : Segment; hoists : array<string>; givens : array<string>; fname : string; var files : array<string>) {
    return if (empty(seg.decls) && empty(seg.stmts))
    var out : array<string>
    out |> reserve(length(hoists) + length(seg.stmts) + 16)
    // a `module X` line (hoisted from a block or given) must be the first declaration
    var seen : table<string>
    for (g in givens) {
        if ((g |> starts_with("module ")) && !key_exists(seen, g)) {
            seen |> insert(g)
            out |> push(g)
        }
    }
    for (hpass in range(2)) {
        for (h in hoists) {
            continue if ((h |> starts_with("module ")) != (hpass == 0) || key_exists(seen, h))
            seen |> insert(h)
            out |> push(h)
        }
    }
    out |> push("")
    var given_stmts : array<string>
    for (g in givens) {
        continue if ((g |> starts_with("module ")) || key_exists(seen, g))
        if (is_hoist(g)) {
            seen |> insert(g)
        }
        if (is_decl_start(g) || is_hoist(g) || is_global_decl(g)) {
            // var/let givens become globals: page-wide context (a finalizer counter,
            // a db handle) that block parameters may legally shadow. `inscope` is
            // statement-only grammar — drop it on the way to module scope
            out |> push(replace(g, " inscope ", " "))
        } else {
            given_stmts |> push("    " + g)
        }
    }
    for (d in seg.decls) {
        out |> push_from(d.lines)
        out |> push("")
    }
    if (!empty(seg.stmts) || !empty(given_stmts)) {
        out |> push("def _doc_main() \{")
        out |> push_from(given_stmts, seg.stmts)
        out |> push("\}")
    }
    fwrite(fname, join(out, "\n") + "\n")
    files |> push(fname)
    // clear, NOT delete: these arrays hold borrowed string pointers (rename_line
    // returns the original line when nothing renamed), and delete on array<string>
    // frees the strings — leaving every alias in hoists/require tables dangling
    seg.decls |> clear()
    seg.stmts |> clear()
}

// `member <Class>`: the block's content lives inside the named class — chunk-aware:
// hoist chunks route to the page hoists, decl chunks (def override ... ) splice as
// members of a synthesized subclass, statement chunks wrap in a helper method so
// unqualified calls resolve against the class surface (HvWebServer routes, adapters, ...)
def append_member_block(var seg : Segment; var ren : Renamer; blk : RstBlock; var hoists : array<string>) {
    var body := blk.body
    mark_elisions(body)
    var members : array<string>
    var stmts : array<string>
    for (chunk in chunks_of(body)) {
        if (chunk.kind == "hoist") {
            for (h in chunk.lines) {
                let t = trim(h)
                continue if (t == ELIDE)
                hoists |> push(t)
            }
            continue
        }
        // member content is class/method-scoped: aliases apply, names stay local
        for (l in chunk.lines) {
            if (trim(l) == ELIDE) {
                continue if (chunk.kind == "decl")
                stmts |> push("        pass")
                continue
            }
            let r = rename_line_ex(ren, l, false)
            if (chunk.kind == "decl") {
                members |> push(empty(trim(r)) ? "" : "    " + r)
            } else {
                stmts |> push(empty(trim(r)) ? "" : "        " + r)
            }
        }
    }
    var dc = DeclChunk(name = "_doc_member_{blk.rst_line}", lines <- [
        "// --- member block at rst:{blk.rst_line}",
        "class _doc_member_{blk.rst_line} : {blk.marker_arg} \{"
    ])
    dc.lines |> push_from(members)
    if (!empty(stmts)) {
        // `: auto` lets method-TAIL excerpts (`return <expr>` in a member block) compile;
        // a void body still infers void
        dc.lines |> push("    def _doc_body_{blk.rst_line}() : auto \{")
        dc.lines |> push_from(stmts)
        dc.lines |> push("    \}")
    }
    dc.lines |> push("\}")
    seg.decls |> emplace(dc)
}

// true when the marker fully consumed the block (fresh falls through — it needs segment state)
def handle_marker(blk : RstBlock; var em : EmittedPage; hoists : array<string>; givens : array<string>; out_base : string; pages_dir : string) : bool {
    if (blk.marker == "skip" || blk.marker == "signatures" || blk.marker == "fragment") {
        em.skipped++
        if (blk.marker == "fragment") {
            em.fragments++
        }
        return true
    }
    if (blk.marker == "expect") {
        em.expects |> push_clone(blk)
        return true
    }
    if (blk.marker == "alt") {
        em.checked++
        emit_alt_block(blk, hoists, givens, out_base, em.files)
        return true
    }
    if (blk.marker == "file") {
        // multi-file literate page: the block IS a sibling module ("put this in
        // helpers.das") — written next to the synthetic page so the page's own
        // bare `require helpers` resolves same-dir; compiled transitively.
        // Repeated `file` markers with the same name APPEND (a literate tangle),
        // and whole-line `...` elisions are dropped like any decl-body elision
        em.checked++
        let fname = base_name(blk.marker_arg)
        var body := blk.body
        mark_elisions(body)
        let content = build_string() $(var writer) {
            for (l in body) {
                continue if (trim(l) == ELIDE)
                writer |> write(l)
                writer |> write("\n")
            }
        }
        let fpath = path_join(pages_dir, fname)
        if (key_exists(em.file_written, fname)) {
            fwrite(fpath, fread(fpath) + content)
        } else {
            em.file_written |> insert(fname)
            fwrite(fpath, content)
        }
        return true
    }
    return false
}

// a module the page mints via a `file` marker shadows the companion's require of the
// real one — both would declare the same module name; the page's text wins
def add_companion_hoists(var hoists : array<string>; comp : array<string>; page : PageSource) {
    var minted : table<string>
    for (blk in page.blocks) {
        if (blk.marker == "file" && !empty(blk.marker_arg)) {
            let s = stem(base_name(blk.marker_arg))
            if (!key_exists(minted, s)) {
                minted |> insert(s)
            }
        }
    }
    for (c in comp) {
        let t = strip_comment(c)
        if (t |> starts_with("require ")) {
            var target = trim(slice(t, length("require ")))
            if (target |> ends_with(" public")) {
                target = trim(trim_suffix(target, " public"))
            }
            continue if (key_exists(minted, stem(base_name(target))))
        }
        hoists |> push(c)
    }
}

def emit_page(page : PageSource; cfg : Config) : EmittedPage {
    var em = EmittedPage()
    var hoists : array<string>
    let comp <- companion_preamble(page.path, cfg.root, path_join(cfg.out, "pages"))
    em.companion_backed = !empty(comp)
    add_companion_hoists(hoists, comp, page)
    // only requires that genuinely came FROM the companion are binary-gap-fatal;
    // a page-text require on a companion-backed page is the page's own problem
    for (h in hoists) {
        if ((h |> starts_with("require ")) && !key_exists(em.companion_requires, h)) {
            em.companion_requires |> insert(h)
        }
    }
    if (!em.companion_backed && (base_name(page.path) |> starts_with("dasAudio_"))) {
        hoists |> push("require audio/audio_boost")
        hoists |> push("require math")
    }
    var seg = Segment()
    var ren = Renamer()
    seed_renamer(ren, page.givens)
    var seg_no = 0
    let out_base = path_join(path_join(cfg.out, "pages"), stem(page.path) + "_doc")
    for (blk in page.blocks) {
        continue if (handle_marker(blk, em, hoists, page.givens, out_base, path_join(cfg.out, "pages")))
        if (blk.marker == "member") {
            em.checked++
            append_member_block(seg, ren, blk, hoists)
            continue
        }
        if (blk.marker == "fresh") {
            let fname = seg_no == 0 ? out_base + ".das" : "{out_base}__{seg_no + 1}.das"
            flush_segment(seg, hoists, page.givens, fname, em.files)
            seg_no++
            // clear, not delete: key strings may alias page lines (identity slices)
            ren.counts |> clear()
            ren.aliases |> clear()
            ren.def_headers |> clear()
            seed_renamer(ren, page.givens)
        }
        em.checked++
        var body := blk.body
        mark_elisions(body)
        let tag = "// --- {base_name(page.path)}:{blk.rst_line}"
        for (chunk in chunks_of(body)) {
            if (chunk.kind == "hoist") {
                for (h in chunk.lines) {
                    let t = trim(h)
                    continue if (trim(t) == ELIDE)
                    hoists |> push(t)
                    if (t |> starts_with("require ")) {
                        em.require_lines |> push(t)
                    }
                }
            } elif (chunk.kind == "decl") {
                append_decl(seg, ren, chunk, tag)
            } else {
                append_stmts(seg, ren, chunk, tag)
            }
        }
    }
    let last_fname = seg_no == 0 ? out_base + ".das" : "{out_base}__{seg_no + 1}.das"
    flush_segment(seg, hoists, page.givens, last_fname, em.files)
    for (h in hoists) {
        if (h |> starts_with("require ")) {
            em.require_lines |> push(h)
        }
    }
    return <- em
}

// ===== compile driver =====

def compile_file_rc(cfg : Config; path : string; var output : string&) : int {
    let args <- [normalize(cfg.daslang), "-compile-only", normalize(path)]
    return run_and_capture(args, output, 120.0)
}

def first_lines(s : string; n : int) : array<string> {
    var out : array<string>
    for (l in split(s, "\n")) {
        break if (length(out) >= n)
        continue if (empty(trim(l)))
        out |> push(l)
    }
    return <- out
}

// ===== reporting =====

struct PageReport {
    page : string
    status : string
    das_blocks : int
    checked : int
    skipped : int
    fragments : int
    expect_checked : int
    companion_backed : bool
    errors : array<string>
}

struct SweepReport {
    total_pages : int
    green : int
    red : int
    no_blocks : int
    probe_failures : array<string>
    pages : array<PageReport>
}

def check_expects(em : EmittedPage; cfg : Config; base_file : string; var rep : PageReport) {
    for (ex in em.expects) {
        var body := ex.body
        mark_elisions(body)
        let ex_file = replace(base_file, ".das", "__expect_{ex.rst_line}.das")
        var lines <- split(fread(base_file), "\n")
        for (chunk in chunks_of(body)) {
            if (chunk.kind == "decl" || chunk.kind == "hoist") {
                lines |> push_from(chunk.lines)
            } else {
                lines |> reserve(length(lines) + length(chunk.lines) + 2)
                lines |> push("def _doc_expect_{ex.rst_line}() \{")
                for (l in chunk.lines) {
                    lines |> push("    " + l)
                }
                lines |> push("\}")
            }
        }
        fwrite(ex_file, join(lines, "\n") + "\n")
        var out : string
        let rc = compile_file_rc(cfg, ex_file, out)
        if (rc == 0) {
            rep.status = "red"
            rep.errors |> push("expect block at :{ex.rst_line} compiled clean (expected {ex.marker_arg})")
        } elif (!empty(ex.marker_arg) && find(out, ex.marker_arg) < 0) {
            rep.status = "red"
            rep.errors |> push("expect block at :{ex.rst_line} failed with a different error (expected {ex.marker_arg})")
        } else {
            rep.expect_checked++
        }
    }
}

def scan_pages(cfg : Config) : array<string> {
    var pages : array<string>
    for (area in AREAS) {
        let area_path = path_join(cfg.root, area)
        continue if (!fexist(area_path))
        dir_rec(area_path) $(name, isdir) {
            // dir_rec yields paths relative to the walked root
            let full = path_join(area_path, name)
            return if (isdir || extension(full) != ".rst" || (!empty(cfg.page) && find(to_generic_path(full), cfg.page) < 0))
            pages |> push(full)
        }
    }
    pages |> sort()
    return <- pages
}

// module mounts only: file-relative requires (./x.das, ../y.das, rewritten siblings)
// and on-page modules are the page's own problem, not a binary gap
def probe_target(req : string) : string {
    var t = strip_comment(req)
    return "" if (!(t |> starts_with("require ")))
    t = trim(slice(t, length("require ")))
    if (t |> ends_with(" public")) {
        t = trim(trim_suffix(t, " public"))
    }
    return "" if (empty(t) || find(t, ".") >= 0 || (t |> starts_with("daslib/")))
    return t
}

def run_probes(require_lines : table<string; bool>; file_mods : table<string>; cfg : Config) : array<string> {
    var failures : array<string>
    let probe_dir = path_join(cfg.out, "probes")
    mkdir_rec(probe_dir)
    var probed : table<string; bool>   // target -> from_companion (OR-ed)
    for (req, from_companion in keys(require_lines), values(require_lines)) {
        let target = probe_target(req)
        // a module minted by a `file` marker exists only beside its synthetic page
        continue if (empty(target) || key_exists(file_mods, target))
        let prev = probed?[target] ?? false
        probed[target] = prev || from_companion
    }
    // write ALL probe files before the first popen, then compile and rebuild
    // messages from file contents — string handles derived from table keys read
    // back corrupted after run_and_capture, so the filesystem is the stable store
    var origins : array<bool>
    for (target, from_companion in keys(probed), values(probed)) {
        fwrite(path_join(probe_dir, "probe_{length(origins)}.das"), "require {target}\n")
        origins |> push(from_companion)
    }
    for (idx in range(length(origins))) {
        let pfile = path_join(probe_dir, "probe_{idx}.das")
        var out : string
        if (compile_file_rc(cfg, pfile, out) != 0) {
            failures |> push((origins[idx] ? "[companion] " : "[page] ") + trim(fread(pfile)))
        }
    }
    return <- failures
}

def report_page(page_path : string; cfg : Config) : PageReport {
    let page <- parse_page(page_path)
    var rep = PageReport(page = to_generic_path(page_path), das_blocks = length(page.blocks))
    if (empty(page.blocks)) {
        rep.status = "no_blocks"
        return <- rep
    }
    var em <- emit_page(page, cfg)
    rep.checked = em.checked
    rep.skipped = em.skipped
    rep.fragments = em.fragments
    rep.companion_backed = em.companion_backed
    rep.status = "green"
    for (f in em.files) {
        var out : string
        if (compile_file_rc(cfg, f, out) != 0) {
            rep.status = "red"
            rep.errors |> push_from(first_lines(out, 24))
        }
    }
    // expects append to the main page program, not an alt program
    var expect_base = ""
    for (f in em.files) {
        if (find(f, "__alt_") < 0) {
            expect_base = f
            break
        }
    }
    if (rep.status == "green" && !empty(expect_base)) {
        check_expects(em, cfg, expect_base, rep)
    }
    return <- rep
}

[export]
def main() : int {
    var cfg = Config()
    let rc = parse_args_with_help(cfg, "doc-verify")
    return rc if (rc >= 0)
    mkdir_rec(path_join(cfg.out, "pages"))
    let pages <- scan_pages(cfg)
    to_log(LOG_INFO, "doc-verify: {length(pages)} pages\n")

    // pass 1: parse+emit everything, collecting the corpus require set for rule 0
    var require_origin : table<string; bool>   // require line -> seen-from-companion
    var file_mods : table<string>              // modules minted by `file` markers
    for (p in pages) {
        if (cfg.verbose) {
            to_log(LOG_INFO, "  emit {p}\n")
        }
        let page <- parse_page(p)
        continue if (empty(page.blocks))
        var em <- emit_page(page, cfg)
        for (r in em.require_lines) {
            let prev = require_origin?[r] ?? false
            require_origin[r] = prev || key_exists(em.companion_requires, r)
        }
        for (f in keys(em.file_written)) {
            let s = stem(f)
            if (!key_exists(file_mods, s)) {
                file_mods |> insert(s)
            }
        }
    }

    if (!cfg.no_probes) {
        let failures <- run_probes(require_origin, file_mods, cfg)
        var fatal = false
        for (f in failures) {
            to_log(LOG_ERROR, "rule-0 probe failed: {f}\n")
            fatal ||= f |> starts_with("[companion]")
        }
        if (fatal) {
            to_log(LOG_ERROR, "doc-verify: binary gap — companion-backed requires failed to load; build the missing modules and re-run\n")
            return 2
        }
    }

    // pass 2: verdicts
    var sweep = SweepReport(total_pages = length(pages))
    sweep.pages |> reserve(length(pages))
    for (i, p in count(), pages) {
        var rep <- report_page(p, cfg)
        if (rep.status == "green") {
            sweep.green++
        } elif (rep.status == "red") {
            sweep.red++
        } else {
            sweep.no_blocks++
        }
        if (cfg.verbose || rep.status == "red") {
            to_log(LOG_INFO, "  [{rep.status}] {rep.page}\n")
        }
        if ((i + 1) % 25 == 0) {
            to_log(LOG_INFO, "  ... {i + 1}/{length(pages)}\n")
        }
        sweep.pages |> emplace(rep)
    }

    let json_path = empty(cfg.json) ? path_join(cfg.out, "report.json") : cfg.json
    fwrite(json_path, sprint_json(sweep, true))
    to_log(LOG_INFO, "doc-verify: {sweep.green} green, {sweep.red} red, {sweep.no_blocks} without das blocks; report: {json_path}\n")
    return sweep.red > 0 ? 1 : 0
}
