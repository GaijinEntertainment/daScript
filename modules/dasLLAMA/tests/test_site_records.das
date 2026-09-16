options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/fio
require ../performance/gen_site_records.das   // merge_site_records — file-relative require, explicit .das (pays the engine compile, like test_tok_seed)
require ../performance/profile_common.das   // BenchModel — the merge's row type

// The records-vs-site drift gate: site/files/dasllama/bench_records.json is a COMMITTED merge
// of performance/records/*.json and bench_cells.json its first-paint projection
// (daslang.io/dasllama.html renders from both), and forgetting to regenerate them after a
// records commit ships stale numbers to the live page. This cell IS the tool's --verify mode:
// regenerate in memory, byte-compare against the committed files.

[test]
def test_site_records_current(t : T?) {
    t |> run("the committed site merge and its projection match the record stores byte for byte") @(tt : T?) {
        if (!stat("modules/dasLLAMA/performance/records").is_valid) {
            tt |> skip("not at the repo root (the record stores not visible) - the gate runs from the tree")
            return
        }
        for (path in [SITE_PATH, SITE_CELLS_PATH]) {
            if (!stat(path).is_valid) {
                // the committed files are tracked - an absence is a broken or renamed rail, never a skip
                tt |> failure("{path} is missing - the site merge rail moved or was deleted; update gen_site_records + this gate together")
                return
            }
        }
        var merged : array<BenchModel>
        if (!merge_site_records(merged)) {
            tt |> failure("the record stores under performance/records/ would not merge - see the log")
            return
        }
        let outputs <- site_outputs(merged)
        tt |> equal([for (o in outputs); o.path], [SITE_PATH, SITE_CELLS_PATH], "the merge writes exactly the two site files")
        for (o in outputs) {
            if (fread(o.path) != o.text) {
                tt |> failure("{o.path} drifted from the record stores - a records commit skipped the site merge; run gen_site_records and commit all three")
            }
        }
        delete merged
    }
}

// The projection contract the page's first paint relies on: every field the tables, bars and
// hero read survives with its value, and every receipt-only field - what the page fetches from
// the full store on the first receipt opened - is gone.
[test]
def test_site_cells_projection(t : T?) {
    t |> run("bench_cells.json keeps the render fields and drops the receipt fields") @(tt : T?) {
        var run = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", box = "m1", threads = 8,
            date = "2026-08-02", cmd = "bin/dasllama-bench -- -m ~/models/x.gguf", workload = "asr",
            source = "official", sha = "7605e8476", version = "0.6.4", versions = "pins",
            tune = "k4q8=mr8", tune_sha = "abc", exec_fmt = "q8 native", env = "DASLLAMA_BOX=m1",
            comment = "a note", void_clips = "jfk.wav",
            hardware = HardwareInfo(cpu = "Apple M1 Max", arch = "arm64", os = "macOS", ram_gb = 64, gpu = "M1"))
        run.tests["asr:jfk.wav"] = BenchTest(ms = 812.5lf, audio_s = 11.0lf, encode_ms = 100.0lf, text = "the transcript")
        run.tests["pp512"] = BenchTest(tok_s = 217.5lf, stddev = 1.9lf)
        run.files |> push(InputFile(role = "weights", name = "x.gguf", sha256 = "deadbeef", bytes = 42l))
        var models : array<BenchModel>
        var m = BenchModel(gguf = "x.gguf", arch = "qwen3", quant = "Q8_0", params_b = 35.0lf, size_bytes = 1000l, note = "a caveat")
        m.runs |> emplace(run)
        models |> emplace(m)

        let text = site_cells_json(models)
        var cells : array<SiteModel>
        tt |> success(sscan_json(text, cells), "the projection re-reads as SiteModel")
        tt |> equal(length(cells), 1)
        for (c in cells) {
            tt |> equal(c.gguf, "x.gguf")
            tt |> equal(c.arch, "qwen3")
            tt |> equal(c.size_bytes, 1000l)
            tt |> equal(c.note, "a caveat")
            tt |> equal(length(c.runs), 1)
            for (r in c.runs) {
                tt |> equal(r.engine, "das")
                tt |> equal(r.backend, "cpu")
                tt |> equal(r.flavor, "tuned")
                tt |> equal(r.box, "m1")
                tt |> equal(r.threads, 8)
                tt |> equal(r.date, "2026-08-02")
                tt |> equal(r.workload, "asr")
                tt |> equal(r.source, "official")
                tt |> equal(r.sha, "7605e8476")
                tt |> equal(r.version, "0.6.4")
                tt |> equal(r.comment, "a note")
                tt |> equal(r.void_clips, "jfk.wav")
                tt |> equal(r.hardware.cpu, "Apple M1 Max")
                tt |> equal(r.tests?["pp512"]?.tok_s ?? 0.0lf, 217.5lf)
                tt |> equal(r.tests?["asr:jfk.wav"]?.ms ?? 0.0lf, 812.5lf)
                tt |> equal(r.tests?["asr:jfk.wav"]?.audio_s ?? 0.0lf, 11.0lf)
                tt |> equal(length(r.files), 1)
                for (f in r.files) {
                    tt |> equal(f.role, "weights")
                    tt |> equal(f.name, "x.gguf")
                }
            }
        }
        // the receipt-only values are absent from the TEXT - a key that survived as a zero would
        // still pass the struct reads above
        for (leak in ["~/models/x.gguf", "DASLLAMA_BOX=m1", "k4q8=mr8", "\"abc\"", "q8 native", "pins",
                      "the transcript", "deadbeef", "\"bytes\"", "\"stddev\"", "\"encode_ms\"",
                      "arm64", "macOS", "\"ram_gb\"", "Q8_0", "\"params_b\""]) {
            tt |> success(find(text, leak) < 0, "receipt-only value {leak} leaked into the projection")
        }
        delete cells
        delete models
    }
}
