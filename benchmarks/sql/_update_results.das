options gen2
options indenting = 4

// _update_results.das — regenerate the INTERP/JIT matrices in benchmarks/sql/results.md from the
// machine-readable benchmark output (`dastest ... --bench --bench-format json`). Not a benchmark
// (underscore prefix); run after a sweep:
//
//   daslang benchmarks/sql/_update_results.das -- --interp <interp.json> --jit <jit.json> --date <YYYY-MM-DD>
//
// It rewrites ONLY the region between the `<!-- BENCH:TABLES BEGIN -->` / `<!-- BENCH:TABLES END -->`
// markers; all prose (header narrative, "Notes on missing lanes", "How to re-run") is human-owned.

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings

// One benchmark sample, parsed from a `BenchmarkRunStats` JSON line (extra keys are ignored by sscan_json).
struct BenchRow {
    name     : string
    n        : int
    time_ns  : int64
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

    @clarg_doc = "Path to results.md (default: benchmarks/sql/results.md)"
    results : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let LANES = ["m1", "m3f", "m4", "m5", "m5f"]
let HEADERS = ["SQL (m1)", "Array (m3f)", "Decs (m4)", "XML (m5)", "XML fold (m5f)"]
let BEGIN_MARKER = "<!-- BENCH:TABLES BEGIN -->"
let END_MARKER = "<!-- BENCH:TABLES END -->"

// 1-decimal fixed format via integer math (avoids float-interpolation drift). 31.35 -> "31.4".
def fmt1(v : double) : string {
    let scaled = int(v * 10.0lf + 0.5lf)
    return "{scaled / 10}.{scaled % 10}"
}

// 2-decimal ratio with the multiply glyph. 1.0 -> "1.00x". absent or sub-resolution inputs -> em-dash
// (m3f below 0.05 ns/op rounds to 0.0 in the table — the ratio would be measurement noise).
def fmt_ratio(m4present, m3fpresent : bool; m4, m3f : double) : string {
    if (!m4present || !m3fpresent || m3f < 0.05lf) return "—"
    let h = int((m4 / m3f) * 100.0lf + 0.5lf)
    let frac = h % 100
    let pad = frac < 10 ? "0" : ""
    return "{h / 100}.{pad}{frac}×"
}

// Parse a JSON-lines benchmark file into the flat cell map keyed "family\tlane" -> ns/op, collecting
// the set of family names seen.
def load_cells(path : string; var cells : table<string; double>; var families : table<string>) {
    let text = fread(path)
    for (raw in split(text, "\n")) {
        let line = strip(raw)
        if (!starts_with(line, "\{")) continue
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

def build_table(cells : table<string; double>; families : array<string>) : string {
    return build_string() $(w) {
        w |> write("| Benchmark |")
        for (h in HEADERS) {
            w |> write(" {h} |")
        }
        w |> write(" Decs vs Array |\n")
        w |> write("|---|---:|---:|---:|---:|---:|---:|\n")
        for (fam in families) {
            w |> write("| `{fam}` |")
            for (lane in LANES) {
                w |> write(" {cell_str(cells, fam, lane)} |")
            }
            let m3fKey = "{fam}\tm3f"
            let m4Key = "{fam}\tm4"
            let hasM3f = key_exists(cells, m3fKey)
            let hasM4 = key_exists(cells, m4Key)
            w |> write(" {fmt_ratio(hasM4, hasM3f, cells?[m4Key] ?? 0.0lf, cells?[m3fKey] ?? 0.0lf)} |\n")
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
    let resultsPath = empty(cfg.results) ? "benchmarks/sql/results.md" : cfg.results

    var interpCells : table<string; double>
    var jitCells : table<string; double>
    var famSet : table<string>
    load_cells(cfg.interp, interpCells, famSet)
    load_cells(cfg.jit, jitCells, famSet)

    var families <- [for (f in keys(famSet)); f]
    sort(families)

    let body = build_string() $(w) {
        w |> write("{BEGIN_MARKER}\n")
        w |> write("*Generated {cfg.date} by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane; Decs vs Array = m4/m3f. Edit the prose around the markers, not the tables.*\n\n")
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
    let updated = slice(doc, 0, bi) + body + slice(doc, ei + length(END_MARKER))
    fopen(resultsPath, "wb") $(f) {
        if (f == null) {
            to_log(LOG_ERROR, "cannot open {resultsPath} for writing\n")
            return
        }
        fwrite(f, updated)
    }
    to_log(LOG_INFO, "results.md updated: {length(families)} families, INTERP+JIT matrices regenerated\n")
    return 0
}
