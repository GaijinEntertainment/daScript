options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio
require ../performance/gen_site_records.das   // merge_site_records — file-relative require, explicit .das (pays the engine compile, like test_tok_seed)
require ../performance/profile_common.das   // BenchModel — the merge's row type
require dasllama/dasllama_version             // DASLLAMA_RELEASE — the release the cards show
require dasllama/dasllama_exchange_schema     // RETIRED_BOXES — the boards the cards drop

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
        var run = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", box = "m5", threads = 8,
            date = "2026-08-02", cmd = "bin/dasllama-bench -- -m ~/models/x.gguf", workload = "asr",
            source = "official", sha = "7605e8476", version = "0.6.4", versions = "pins",
            dasllama_version = DASLLAMA_RELEASE,
            tune = "k4q8=mr8", tune_sha = "abc", exec_fmt = "q8 native", env = "DASLLAMA_BOX=m5",
            comment = "a note", void_clips = "jfk.wav",
            hardware = HardwareInfo(cpu = "Apple M5 Max", arch = "arm64", os = "macOS", ram_gb = 64, gpu = "M5"))
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
                tt |> equal(r.box, "m5")
                tt |> equal(r.threads, 8)
                tt |> equal(r.date, "2026-08-02")
                tt |> equal(r.workload, "asr")
                tt |> equal(r.source, "official")
                tt |> equal(r.sha, "7605e8476")
                tt |> equal(r.version, "0.6.4")
                tt |> equal(r.comment, "a note")
                tt |> equal(r.void_clips, "jfk.wav")
                tt |> equal(r.hardware.cpu, "Apple M5 Max")
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
        for (leak in ["~/models/x.gguf", "DASLLAMA_BOX=m5", "k4q8=mr8", "\"abc\"", "q8 native", "pins",
                      "the transcript", "deadbeef", "\"bytes\"", "\"stddev\"", "\"encode_ms\"",
                      "arm64", "macOS", "\"ram_gb\"", "Q8_0", "\"params_b\"", "\"dasllama_version\""]) {
            tt |> success(find(text, leak) < 0, "receipt-only value {leak} leaked into the projection")
        }
        delete cells
        delete models
    }
}

def private board_run(engine, box : string; version : int; workload : string = ""; held : string = "") : BenchRun {
    var r = BenchRun(engine = engine, backend = "cpu", flavor = engine == "das" ? "tuned" : "stock",
        box = box, threads = 8, date = "2026-10-21", cmd = "test", workload = workload,
        dasllama_version = engine == "das" ? version : 0, held = held)
    r.tests |> insert("pp512", BenchTest(tok_s = 100.0lf))
    return <- r
}

// one projected row per (box, engine, workload) of a model, keyed for the asserts below
def private projected(board : array<BenchModel>; gguf : string) : table<string; bool> {
    var out : table<string; bool>
    for (m in board) {
        continue if (m.gguf != gguf)
        for (r in m.runs) {
            out["{r.box}/{r.engine}/{r.workload}"] = true
        }
    }
    return <- out
}

def private audio_run(engine, backend : string; ms, lengths : array<double>; void_clips : string = "") : BenchRun {
    var r = BenchRun(engine = engine, backend = backend, flavor = engine == "das" ? "tuned" : "stock",
        box = "m5", threads = 8, date = "2026-10-21", cmd = "test", workload = "audio-chat",
        dasllama_version = engine == "das" ? DASLLAMA_RELEASE : 0, void_clips = void_clips)
    let clips <- ["jfk.wav", "jfk3.wav", "gb1.wav"]
    for (i in range(3)) {
        r.tests |> insert("asr:{clips[i]}", BenchTest(ms = ms[i], audio_s = lengths[i]))
    }
    return <- r
}

def private void_clips_on(board : array<BenchModel>; engine, backend : string) : string {
    for (m in board) {
        for (r in m.runs) {
            return r.void_clips if (r.engine == engine && r.backend == backend)
        }
    }
    return "<absent>"
}

// The board's projection rules: the cards show the current release's rows of the current boards
// alone, a held row never, and every audio run with the outlier rule's verdict on it.
[test]
def test_board_projection(t : T?) {
    t |> run("a stale or retired row never reaches the projection; the current row is the control") @(tt : T?) {
        var models <- [
            <- BenchModel(gguf = "x.gguf", runs <- [
                <- board_run("das", "m5", DASLLAMA_RELEASE),                 // the control
                <- board_run("das", "m5", DASLLAMA_RELEASE - 1),             // an older release on a current board
                <- board_run("das", "m1", DASLLAMA_RELEASE),                 // a retired board at the release
                <- board_run("llama.cpp", "m5", 0),                          // the current row's reference
                <- board_run("llama.cpp", "m1", 0),                          // a retired board's reference
                <- board_run("llama.cpp", "m5", 0, "image-chat")]),          // a reference with no das row to pair
            <- BenchModel(gguf = "old.gguf", runs <- [
                <- board_run("das", "m5", DASLLAMA_RELEASE - 1),
                <- board_run("llama.cpp", "m5", 0)])]
        tt |> success(key_exists(RETIRED_BOXES, "m1"), "the fixture's retired board is on the list")

        var inscope board <- board_models(models)
        tt |> equal(length(board), 1, "a model with no current row is off the board")
        let rows <- projected(board, "x.gguf")
        tt |> equal(length(rows), 2, "the current das row and its reference alone project")
        tt |> success(key_exists(rows, "m5/das/"), "the current-release row on a current board projects")
        tt |> success(key_exists(rows, "m5/llama.cpp/"), "its reference projects beside it")
        for (gone in ["m1/das/", "m1/llama.cpp/", "m5/llama.cpp/image-chat"]) {
            tt |> success(!key_exists(rows, gone), "{gone} must not project")
        }
        for (r in board[0].runs) {
            tt |> success(r.engine != "das" || r.dasllama_version == DASLLAMA_RELEASE, "an older release's das row must not project")
        }
        let text = site_cells_json(models)
        tt |> success(find(text, "\"m1\"") < 0, "no retired box in the cells text")
        tt |> success(find(text, "old.gguf") < 0, "no stale model in the cells text")
        delete models
    }
    t |> run("a held row stays in the records and off the board, its reference with it") @(tt : T?) {
        var models <- [
            <- BenchModel(gguf = "kitten-nano.gguf", runs <- [
                <- board_run("das", "m5", DASLLAMA_RELEASE, "tts", "followup_general.md 219"),
                <- board_run("kittentts", "m5", 0, "tts")]),
            <- BenchModel(gguf = "kokoro-82m.gguf", runs <- [
                <- board_run("das", "m5", DASLLAMA_RELEASE, "tts"),
                <- board_run("kokoro", "m5", 0, "tts")])]
        var inscope board <- board_models(models)
        tt |> equal(length(board), 1)
        tt |> equal(board[0].gguf, "kokoro-82m.gguf", "the unheld sibling is the control")
        tt |> equal(length(board[0].runs), 2)
        tt |> equal(length(models[0].runs), 2, "the held rows stay in the merge")
        delete models
    }
    t |> run("the stored outlier is voided on the projected row without a re-measure") @(tt : T?) {
        let lengths <- [11.0lf, 33.0lf, 198.734375lf]
        let none <- [0.0lf, 0.0lf, 0.0lf]
        var models : array<BenchModel>
        models |> emplace(BenchModel(gguf = "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf", runs <- [
            <- audio_run("das", "metal", [357.6lf, 464.8lf, 4729.2lf], lengths),
            // the reference records no clip length: jfk reads 3178 ms a second against a median of 36
            <- audio_run("llama-mtmd-cli", "metal", [34960.4lf, 553.2lf, 7075.4lf], none),
            // an even run: 851, 787 and 797 ms a second
            <- audio_run("llama-mtmd-cli", "cpu", [9365.4lf, 25961.6lf, 158455.9lf], none),
            // the annotations' spelling unions with the rule's
            <- audio_run("llama-mtmd-cli", "vulkan", [34960.4lf, 553.2lf, 7075.4lf], none, "gb1.wav")]))
        var inscope board <- board_models(models)
        tt |> equal(void_clips_on(board, "llama-mtmd-cli", "metal"), "jfk.wav", "the outlier is voided off the das row's clip lengths")
        tt |> equal(void_clips_on(board, "llama-mtmd-cli", "cpu"), "", "an even run keeps every clip")
        tt |> equal(void_clips_on(board, "das", "metal"), "", "the das run is even")
        tt |> equal(void_clips_on(board, "llama-mtmd-cli", "vulkan"), "gb1.wav,jfk.wav", "the annotation's clip and the rule's, sorted")
        tt |> equal(models[0].runs[1].void_clips, "", "the merge itself is untouched - the verdict is the projection's")
        delete models
    }
}
