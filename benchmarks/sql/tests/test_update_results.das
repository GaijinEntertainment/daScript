options gen2

require dastest/testing_boost public
require daslib/json_boost
require daslib/fio
require daslib/strings_boost
require strings

// Settles the _update_results.das contract: cells parse (including a JSON line carrying a
// leaked ANSI-escape prefix), absent lanes stay em-dashes / nulls, the site record and
// results.md agree, and a failed run leaves BOTH outputs untouched (site/REVIEW.md).

def private tool_path() : string {
    return "{get_das_root()}/benchmarks/sql/_update_results.das"
}

def private tmp_path(name : string) : string {
    return "{get_das_root()}/.bench_update_test_{name}"
}

def private write_file(t : T?; path, text : string) {
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, text)
            ok = true
        }
    }
    t |> success(ok, "fixture written: {path}")
}

def private read_file(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

def private run_tool(args : string; var output : string&) : int {
    let exe = get_command_line_arguments()[0]
    let cmd = "{exe} {tool_path()} -- {args}"
    var code : int
    output = ""
    unsafe {
        code = popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    output += fgets(f)
                }
            }
        }
    }
    return code
}

def private bench_line(name : string; n : int; time_ns : int64) : string {
    return "\{\"name\":\"{name}\",\"n\":{n},\"time_ns\":{time_ns},\"func_type\":\"INTERP\"}\n"
}

let RESULTS_SKELETON = "# scratch\n\n<!-- BENCH:TABLES BEGIN -->\nold\n<!-- BENCH:TABLES END -->\n"

[test]
def test_update_results_tool(tt : T?) {
    let esc = "{to_char(27)}"
    let interpPath = tmp_path("interp.json")
    let jitPath = tmp_path("jit.json")
    let resultsPath = tmp_path("results.md")
    let recordPath = tmp_path("record.json")

    tt |> run("cells land, an ANSI-prefixed line parses, absent lanes stay dashes, record matches") <| @(t : T?) {
        // alpha_m1 arrives behind a leaked reset escape; alpha has no m4 lane; beta is m3f-only
        let interp = (bench_line("alpha_m3f", 100, 1234l)
            + "{esc}[0m" + bench_line("alpha_m1", 100, 5000l)
            + "not json at all\n"
            + bench_line("beta_m3f", 200, 5000l))
        let jit = bench_line("alpha_m3f", 100, 250l)
        t |> write_file(interpPath, interp)
        t |> write_file(jitPath, jit)
        t |> write_file(resultsPath, RESULTS_SKELETON)
        var output : string
        let code = run_tool("--interp {interpPath} --jit {jitPath} --date 2026-01-01 --results {resultsPath} --site-json {recordPath} --machine \"TEST BOX\"", output)
        t |> equal(code, 0, "tool exit code; output:\n{output}")
        let md = read_file(resultsPath)
        t |> success(md |> contains("| `alpha` | 50.0 | 12.3 | — |"), "alpha row: escaped m1 cell parsed, m4 dash; got:\n{md}")
        t |> success(md |> contains("| `beta` | — | 25.0 | — |"), "beta row present")
        var err = ""
        let rec = read_json(read_file(recordPath), err)
        t |> success(rec != null, "record parses: {err}")
        t |> equal(rec?["machine"] ?? "", "TEST BOX", "record names its capture box")
        let alpha = rec?["families"]?[0]
        t |> equal(alpha?["name"] ?? "", "alpha", "families sorted, alpha first")
        t |> equal(alpha?["interp"]?[0] ?? -1.0lf, 50.0lf, "record m1 cell equals the md cell")
        t |> success((alpha?["interp"]?[2]) == null || (alpha?["interp"]?[2] is _null), "absent lane is null in the record")
        t |> equal(alpha?["jit"]?[1] ?? -1.0lf, 2.5lf, "jit m3f cell rounded to 1 decimal")
    }

    tt |> run("missing BENCH:TABLES markers fail BEFORE either output is written") <| @(t : T?) {
        t |> write_file(interpPath, bench_line("alpha_m3f", 100, 1234l))
        t |> write_file(jitPath, bench_line("alpha_m3f", 100, 250l))
        t |> write_file(resultsPath, "# scratch with no markers\n")
        remove(recordPath)
        var output : string
        let code = run_tool("--interp {interpPath} --jit {jitPath} --date 2026-01-01 --results {resultsPath} --site-json {recordPath} --machine \"TEST BOX\"", output)
        t |> success(code != 0, "missing markers must fail")
        t |> equal(read_file(resultsPath), "# scratch with no markers\n", "results.md untouched on the failed run")
        t |> equal(read_file(recordPath), "", "no record written on the failed run")
    }

    tt |> run("--site-json without --machine fails BEFORE touching either output") <| @(t : T?) {
        t |> write_file(interpPath, bench_line("alpha_m3f", 100, 1234l))
        t |> write_file(jitPath, bench_line("alpha_m3f", 100, 250l))
        t |> write_file(resultsPath, RESULTS_SKELETON)
        remove(recordPath)
        var output : string
        let code = run_tool("--interp {interpPath} --jit {jitPath} --date 2026-01-01 --results {resultsPath} --site-json {recordPath}", output)
        t |> success(code != 0, "missing --machine must fail")
        t |> equal(read_file(resultsPath), RESULTS_SKELETON, "results.md untouched on the failed run")
        t |> equal(read_file(recordPath), "", "no record written on the failed run")
    }

    remove(interpPath)
    remove(jitPath)
    remove(resultsPath)
    remove(recordPath)
}
