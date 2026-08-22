options gen2
options indenting = 4

// _update_results.das — regenerate the INTERP/JIT matrices in results.md from the
// machine-readable benchmark output (`dastest ... --bench --bench-format json`). Not a
// benchmark (underscore prefix); run after a sweep:
//
//   daslang examples/benchmarks/sql/_update_results.das -- --interp <interp.json> --jit <jit.json> --date <YYYY-MM-DD>
//
// It rewrites ONLY the region between the `<!-- BENCH:TABLES BEGIN -->` / `<!-- BENCH:TABLES END -->`
// markers; all prose (header narrative, engine notes, "How to re-run") is human-owned.

require daslib/clargs
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings

// One benchmark sample, parsed from a `BenchmarkRunStats` JSON line (extra keys are ignored by sscan_json).
struct BenchRow {
    name : string
    n : int
    time_ns : int64
    func_type : string
}

[CommandLineArgs]
struct Config {
    @clarg_required
    @clarg_doc = "Path to the INTERP benchmark JSON (one BenchmarkRunStats per line)"
    interp : string

    @clarg_required
    @clarg_doc = "Path to the JIT benchmark JSON (one BenchmarkRunStats per line)"
    jit : string

    @clarg_required
    @clarg_doc = "Date stamp for the generated banner (YYYY-MM-DD)"
    date : string

    @clarg_doc = "Path to results.md (default: examples/benchmarks/sql/results.md)"
    results : string

    @clarg_doc = "Also write the daslang.io data record (site/files/performance_engines.json) to this path"
    site_json : string

    @clarg_doc = "Machine identity string for the site record (required with --site-json)"
    machine : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let LANES = ["sqlite", "duckdb", "postgres", "array"]
let HEADERS = ["SQLite", "DuckDB", "PostgreSQL", "Array"]
let BEGIN_MARKER = "<!-- BENCH:TABLES BEGIN -->"
let END_MARKER = "<!-- BENCH:TABLES END -->"

// 1-decimal fixed rounding via integer math (avoids float-interpolation drift). 31.35 -> 314.
def scaled1(v : double) : int {
    return int(v * 10.0lf + 0.5lf)
}

def fmt1(v : double) : string {
    let scaled = scaled1(v)
    return "{scaled / 10}.{scaled % 10}"
}

def strip_to_first_brace(raw : string) : string {
    let s = strip(raw)
    let br = find(s, "\{")
    if (br < 0) return ""
    return br == 0 ? s : slice(s, br)
}

def write_text_file(path, text : string) : bool {
    var wrote = false
    fopen(path, "wb") $(f) {
        // `return` here only exits the block — the wrote flag carries the failure out.
        if (f == null) {
            to_log(LOG_ERROR, "cannot open {path} for writing\n")
            return
        }
        fwrite(f, text)
        wrote = true
    }
    return wrote
}

// Parse a JSON-lines benchmark file into the flat cell map keyed "family\tlane" -> ns/op, collecting
// the set of family names seen.
def load_cells(path : string; var cells : table<string; double>; var families : table<string>) {
    let text = fread(path)
    for (raw in split(text, "\n")) {
        let line = strip_to_first_brace(raw)
        if (empty(line)) continue
        var row : BenchRow
        if (!sscan_json(line, row) || row.n == 0) continue
        let us = rfind(row.name, "_")
        if (us < 0) continue
        let fam = slice(row.name, 0, us)
        let lane = slice(row.name, us + 1)
        if (find_index(LANES, lane) < 0) continue
        families |> insert(fam)
        cells["{fam}\t{lane}"] = double(row.time_ns) / double(row.n)
    }
}

def cell_str(cells : table<string; double>; fam, lane : string) : string {
    let key = "{fam}\t{lane}"
    if (!key_exists(cells, key)) return "—"
    return fmt1(cells?[key] ?? 0.0lf)
}

def cell_jv(cells : table<string; double>; fam, lane : string) : JsonValue? {
    let key = "{fam}\t{lane}"
    if (!key_exists(cells, key)) return JVNull()
    return JV(double(scaled1(cells?[key] ?? 0.0lf)) / 10.0lf)
}

def write_site_json(path, date, machine : string; interpCells, jitCells : table<string; double>; families : array<string>) : bool {
    var lanesArr <- [for (id, hdr in LANES, HEADERS); JV({ "id" => JV(id), "name" => JV(hdr) })]
    var fams : array<JsonValue?>
    fams |> reserve(length(families))
    for (fam in families) {
        var interp <- [for (lane in LANES); cell_jv(interpCells, fam, lane)]
        var jit <- [for (lane in LANES); cell_jv(jitCells, fam, lane)]
        fams |> push(JV({ "name" => JV(fam), "interp" => JV(interp), "jit" => JV(jit) }))
    }
    let root = JV({
        "generated" => JV(date),
        "machine" => JV(machine),
        "unit" => JV("ns/op"),
        "lanes" => JV(lanesArr),
        "families" => JV(fams)
    })
    if (!write_text_file(path, write_json(root))) return false
    to_log(LOG_INFO, "site record written: {path}\n")
    return true
}

def build_table(cells : table<string; double>; families : array<string>) : string {
    return build_string() $(w) {
        w |> write("| Benchmark |")
        for (h in HEADERS) {
            w |> write(" {h} |")
        }
        w |> write("\n")
        // Generate the alignment row from HEADERS so it stays in sync when a lane is added/dropped.
        w |> write("|---|")
        for (_h in HEADERS) {
            w |> write("---:|")
        }
        w |> write("\n")
        for (fam in families) {
            w |> write("| `{fam}` |")
            for (lane in LANES) {
                w |> write(" {cell_str(cells, fam, lane)} |")
            }
            w |> write("\n")
        }
    }
}

[export]
def main : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "_update_results")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "_update_results")
        return 0
    }
    let resultsPath = empty(cfg.results) ? "examples/benchmarks/sql/results.md" : cfg.results

    var interpCells : table<string; double>
    var jitCells : table<string; double>
    var famSet : table<string>
    load_cells(cfg.interp, interpCells, famSet)
    load_cells(cfg.jit, jitCells, famSet)
    if (empty(famSet)) {
        // A bad path parses as 0 rows; without this gate it would silently overwrite the tables with an empty matrix.
        to_log(LOG_ERROR, "no benchmark rows parsed from '{cfg.interp}' / '{cfg.jit}' — check the paths (expected `dastest --bench --bench-format json` output)\n")
        return 1
    }

    var families <- [for (f in keys(famSet)); f]
    sort(families)

    if (!empty(cfg.site_json) && empty(cfg.machine)) {
        to_log(LOG_ERROR, "--site-json needs --machine — the published record must name its capture box\n")
        return 1
    }

    let body = build_string() $(w) {
        w |> write("{BEGIN_MARKER}\n")
        w |> write("*Generated {cfg.date} by `examples/benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane (e.g. no PostgreSQL server was reachable). Edit the prose around the markers, not the tables.*\n\n")
        w |> write("## INTERP\n\n")
        w |> write(build_table(interpCells, families))
        w |> write("\n## JIT\n\n")
        w |> write(build_table(jitCells, families))
        w |> write(END_MARKER)
    }

    let doc = fread(resultsPath)
    let bi = find(doc, BEGIN_MARKER)
    let ei = find(doc, END_MARKER)
    if (bi < 0 || ei < 0 || ei < bi) {
        to_log(LOG_ERROR, "results.md is missing the BENCH:TABLES markers — add them once before running this tool\n")
        return 1
    }
    // The site record and results.md must move together (site/REVIEW.md): the record writes
    // only after every validation, immediately before the results write.
    if (!empty(cfg.site_json)) {
        if (!write_site_json(cfg.site_json, cfg.date, cfg.machine, interpCells, jitCells, families)) return 1
    }
    let updated = slice(doc, 0, bi) + body + slice(doc, ei + length(END_MARKER))
    if (!write_text_file(resultsPath, updated)) return 1
    to_log(LOG_INFO, "results.md updated: {length(families)} families, INTERP+JIT matrices regenerated\n")
    return 0
}
