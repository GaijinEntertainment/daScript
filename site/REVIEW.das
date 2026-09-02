options gen2

require strings
require math
require daslib/fio
require daslib/strings_boost
require daslib/json_boost
require daslib/defer
require dastest/review_gate

// The mechanical half of site/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang site/REVIEW.das - exit 0 clean, 1 with findings, 2 when
// not run from the repo root.

let private TABLE_CSS = "site/files/dasllama-table.css"
let private PAGE_HTML = "site/dasllama.html"
let private RECORDS_DIR = "site/files/dasllama"
let private BENCH_RECORDS = "site/files/dasllama/bench_records.json"

// Hosts a page under this folder may reach without a click. daslang.io is the site itself;
// the rest are the reviewed third parties - jsDelivr serves the DocSearch bundle, gc.zgo.at
// and the goatcounter endpoint are the cookieless hit counter, and the github hosts serve the
// star count, the package index and the source links the pages build.
let private ALLOWED_HOSTS <- {
    "daslang.io",
    "cdn.jsdelivr.net",
    "gc.zgo.at",
    "borisbat.goatcounter.com",
    "api.github.com",
    "raw.githubusercontent.com",
    "github.com"
}

// tests/ is the Playwright harness and doc/ is generated Sphinx output - neither is a page
// this folder authors, and neither ships from this tree
let private SKIP_DIRS <- { "tests", "doc", "node_modules" }

//! one top-level selector -> its normalized body. An at-rule (`@media ...`) block is skipped
//! whole: its nested rules are responsive OVERRIDES of the same selectors by design
def private css_blocks(text : string; var out : table<string; string>) {
    peek_data(text) $(d) {
        var i = 0
        let n = length(d)
        while (i < n) {
            let ob = d |> find("\{", i)
            break if (ob < 0)
            let sel = strip(slice(d, i, ob))
            if (sel |> starts_with("@")) {
                var depth = 1
                var j = ob + 1
                while (j < n && depth > 0) {
                    let c = int(d[j])
                    if (c == '{') {
                        depth++
                    } elif (c == '}') {
                        depth--
                    }
                    j++
                }
                i = j
                continue
            }
            let cb = d |> find("\}", ob)
            break if (cb < 0)
            let body = slice(d, ob + 1, cb)
            // normalize: collapse whitespace runs so formatting differences do not read as drift
            let parts <- [for (p in split_by_chars(body, " \t\r\n")); p; where !empty(p)]
            if (!empty(sel) && (sel |> find(".dl-") >= 0)) {
                out[sel] = join(parts, " ")
            }
            i = cb + 1
        }
    }
}

// dasllama.html carries an inline copy of some dl-* selectors beside the loaded css file;
// a body that drifts between the two copies styles the page and the injected table
// differently depending on load order
def private check_dl_selector_parity {
    let css = fread(TABLE_CSS)
    if (empty(css)) {
        gate_finding(TABLE_CSS, "missing or unreadable")
        return
    }
    let html = fread(PAGE_HTML)
    if (empty(html)) {
        gate_finding(PAGE_HTML, "missing or unreadable")
        return
    }
    let s0 = html |> find("<style>")
    let s1 = html |> find("</style>")
    return if (s0 < 0 || s1 < 0)
    var css_map : table<string; string>
    var html_map : table<string; string>
    css_blocks(css, css_map)
    css_blocks(slice(html, s0 + length("<style>"), s1), html_map)
    for (sel, body in keys(html_map), values(html_map)) {
        let other = css_map?[sel] ?? ""
        if (!empty(other) && other != body) {
            gate_finding(PAGE_HTML, "`{sel}` differs from the {TABLE_CSS} copy - give both copies the same body")
        }
    }
}

def private check_published_records_no_home_paths(folder : string) {
    dir(folder) $(name) {
        return if (name == "." || name == "..")
        let full = "{folder}/{name}"
        if (stat(full).is_dir) {
            check_published_records_no_home_paths(full)
        } elif (name |> ends_with(".json")) {
            gate_home_paths(full)
        }
    }
}

def private check_record_receipts {
    let text = fread(BENCH_RECORDS)
    if (empty(text)) {
        gate_finding(BENCH_RECORDS, "missing or unreadable")
        return
    }
    var jerr = ""
    var doc = read_json(text, jerr)
    defer() {
        unsafe {
            delete doc
        }
    }
    if (doc == null) {
        gate_finding(BENCH_RECORDS, "does not parse as JSON: {jerr}")
        return
    }
    if (!(doc.value is _array)) {
        gate_finding(BENCH_RECORDS, "is not a top-level array of models - the receipt check read no run; regenerate the record")
        return
    }
    for (model in doc.value as _array) {
        let gguf = "{model?["gguf"] ?? "?"}"
        let runs = model?["runs"]
        continue if (runs == null || !(runs.value is _array))
        for (r in runs.value as _array) {
            for (field in ["cmd", "date"]) {
                if (empty("{r?[field] ?? ""}")) {
                    gate_finding(BENCH_RECORDS, "{gguf}: a run carries no {field} - every run object holds its cmd and date, the receipt its rows render")
                }
            }
            let tests = r?["tests"]
            continue if (tests == null || !(tests.value is _object))
            for (tname, entry in keys(tests.value as _object), values(tests.value as _object)) {
                for (field in ["cmd", "date"]) {
                    if (!empty("{entry?[field] ?? ""}")) {
                        gate_finding(BENCH_RECORDS, "{gguf}/{tname}: a tests entry carries {field} - the pair belongs on the run object that produced the rows")
                    }
                }
            }
        }
    }
}

//! a lane's column in a results.md table: the bench table heads it "<name> (<id>)", the engines table "<name>"
def private lane_column(header : array<string>; id, name : string) : int {
    for (k in range(length(header))) {
        return k if (header[k] |> find("({id})") >= 0 || header[k] == name)
    }
    return -1
}

// the header and every body row of one results.md table, cloned out of the nested array
// once: a double index into array<array<string>> leaks an array lock in some interpreter
// permutations, and the scoped delete then fails (issue 3923). False when the section
// holds no table.
def private load_results_table(md, section : string; var header : array<string>; var by_family : table<string; array<string>>) : bool {
    var inscope rows <- markdown_table_rows(md, section)
    return false if (length(rows) < 2)
    var k = 0
    for (row in rows) {
        if (k == 0) {
            header := row
        } else {
            by_family[row[0]] := row
        }
        k++
    }
    return true
}

def private check_bench_cell_parity(record_path, results_path : string) {
    let text = fread(record_path)
    let md = fread(results_path)
    if (empty(text) || empty(md)) {
        gate_finding(empty(text) ? record_path : results_path, "missing or unreadable")
        return
    }
    var jerr = ""
    var doc = read_json(text, jerr)
    defer() {
        unsafe {
            delete doc
        }
    }
    if (doc == null) {
        gate_finding(record_path, "does not parse as JSON: {jerr}")
        return
    }
    let lanes = doc?["lanes"]
    let families = doc?["families"]
    if (lanes == null || families == null || !(lanes.value is _array) || !(families.value is _array)) {
        gate_finding(record_path, "has no lanes or families array - the parity check compared no cell; regenerate the record")
        return
    }
    for (section, arm in ["INTERP", "JIT"], ["interp", "jit"]) {
        var header : array<string>
        var by_family : table<string; array<string>>
        if (!load_results_table(md, section, header, by_family)) {
            gate_finding(results_path, "has no `## {section}` table for {record_path} to agree with")
            continue
        }
        for (fam in families.value as _array) {
            let name = "{fam?["name"] ?? ""}"
            if (!key_exists(by_family, name)) {
                gate_finding(record_path, "family {name} has no `## {section}` row in {results_path}")
                continue
            }
            let md_row := by_family[name]
            let cells = fam?[arm]
            if (cells == null || !(cells.value is _array)) {
                gate_finding(record_path, "{name} / {arm}: no cell array - the parity check compared nothing for this family; regenerate the pair")
                continue
            }
            var li = 0
            for (lane in lanes.value as _array) {
                let id = "{lane?["id"] ?? ""}"
                let col = lane_column(header, id, "{lane?["name"] ?? ""}")
                if (li >= length(cells.value as _array)) {
                    gate_finding(record_path, "{name} / {id} / {arm}: no cell for this lane - the record has fewer cells than lanes; regenerate the pair")
                    li++
                    continue
                }
                let cell = (cells.value as _array)[li]
                li++
                if (col < 0 || col >= length(md_row)) {
                    gate_finding(results_path, "`## {section}` has no column for lane {id}")
                    continue
                }
                let md_cell = md_row[col]
                //! ?? reads both number arms - _longint for an integer literal, _number for a decimal; null is the missing cell
                let json_value = cell ?? -1.0lf
                let recorded = json_value < 0.0lf ? "-" : "{json_value}"
                let md_value = md_cell == "-" ? -1.0lf : to_double(md_cell)
                if ((md_cell == "-") != (recorded == "-") || abs(md_value - json_value) > 0.051lf) {
                    gate_finding(record_path, "{name} / {id} / {arm}: {recorded} but {results_path} shows {md_cell} - both come from one sweep; regenerate the pair")
                }
            }
        }
    }
}

// the dl-* names dasllama.html and its script use are defined in the page's inline <style>
// or in the shared sheet; one defined in neither renders unstyled
def private check_dl_names_are_defined {
    let html = fread(PAGE_HTML)
    var defined <- css_selector_tokens(fread(TABLE_CSS), "dl-")
    let s0 = html |> find("<style>")
    let s1 = html |> find("</style>")
    if (s0 >= 0 && s1 > s0) {
        var inscope inline_defined <- css_selector_tokens(slice(html, s0 + length("<style>"), s1), "dl-")
        for (name in keys(inline_defined)) {
            defined |> insert(name)
        }
    }
    gate_prefixed_names_defined([PAGE_HTML, "site/files/dasllama.js"], "dl-", defined, "{PAGE_HTML}'s inline style or {TABLE_CSS}")
}

// every sample a page shows is a full program the current toolchain compiles, with a way
// into the playground beside it; the gate compiles each block with the binary running it.
// A blog post is prose, not a page here; a block marked data-playground="none" is compiled
// but owes no link (the ledgered case: a sample of an engine the wasm build does not carry)
def private check_embedded_samples {
    let argv <- get_command_line_arguments()
    let self_binary = argv[0]
    var pages : array<string>
    dir("site") $(name) {
        if (name |> ends_with(".html")) {
            pages |> push("site/{name}")
        }
    }
    dir("site/_news") $(name) {
        if (name |> ends_with(".md")) {
            pages |> push("site/_news/{name}")
        }
    }
    for (page in pages) {
        let text = fread(page)
        if (empty(text) && stat(page).size > 0ul) {
            gate_finding(page, "unreadable - the sample check compiled nothing from it, which is not a pass")
            continue
        }
        let is_post = page |> ends_with(".md")
        var inscope blocks <- is_post ? markdown_fences(text, "daslang") : html_code_blocks(text, "language-daslang")
        var k = 0
        for (blk in blocks) {
            k++
            let line = find_line(text, slice(blk.source, 0, min(40, length(blk.source))))
            if (find(blk.source, "[export]") < 0 || find(blk.source, "def main") < 0) {
                gate_finding(page, line, "sample {k} is not a full program - no exported main; show every embedded sample as a program that compiles and runs")
            }
            let tmp_r = create_temp_file_result("site_sample", ".das")
            if (!(tmp_r is value)) {
                gate_finding(page, line, "sample {k}: could not create a temp file to compile it")
                continue
            }
            let tmp = unsafe(tmp_r.value)
            fwrite(tmp, blk.source)
            var output = ""
            let rc = unsafe(popen_argv([self_binary, "-compile-only", tmp], 0.0) $(f) {
                if (f != null) {
                    output = fread(f)
                }
            })
            remove(tmp)
            if (rc != 0) {
                let first_error = find_first_error_line(output)
                gate_finding(page, line, "sample {k} does not compile with the current toolchain: {first_error}")
            }
            // the link belongs beside the sample: in the page section that holds the block, or
            // anywhere in a post, which has no sections
            var linked = false
            peek_data(text) $(d) {
                var lo = 0
                var hi = length(d)
                if (!is_post) {
                    let sec = rfind(d, "<section", blk.at)
                    let sec_end = find(d, "</section>", blk.after)
                    lo = max(sec, 0)
                    hi = sec_end < 0 ? length(d) : sec_end
                }
                linked = find(slice(d, lo, hi), "playground") >= 0
            }
            let opted_out = !is_post && find(blk.tag, "data-playground=\"none\"") >= 0
            if (!linked && !opted_out) {
                gate_finding(page, line, "sample {k} has no playground link beside it - every embedded sample links to the playground")
            }
        }
    }
}

def private find_first_error_line(output : string) : string {
    for (raw in split(output, "\n")) {
        return strip(raw) if (find(raw, "error") >= 0)
    }
    return "(no error line captured)"
}

[export]
def main() : int {
    if (!fexist(TABLE_CSS)) {
        to_log(LOG_ERROR, "site/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_dl_selector_parity()
    check_dl_names_are_defined()
    check_embedded_samples()
    check_published_records_no_home_paths(RECORDS_DIR)
    check_record_receipts()
    check_bench_cell_parity("site/files/performance_bench.json", "benchmarks/sql/results.md")
    check_bench_cell_parity("site/files/performance_engines.json", "examples/benchmarks/sql/results.md")
    gate_web_third_party(["site"], SKIP_DIRS, ALLOWED_HOSTS)
    return gate_verdict("site")
}
