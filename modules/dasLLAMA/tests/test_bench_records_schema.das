options gen2
require dastest/testing_boost public
require ../performance/profile_common.das

// The record store is the site's single source of truth; this is the net under its schema:
// ASR-shaped runs (workload, ms/audio_s tests, files) must round-trip the JSON store, the
// upsert identity must treat workload as part of the key, and annotations must land on the
// rows they select — and only those.

let TMP = "modules/dasLLAMA/performance/records/_test_schema.json"   // _ prefix: excluded from the site merge

def private make_asr_run(engine, workload : string) : BenchRun {
    var r = BenchRun(
        engine = engine,
        backend = "cpu",
        flavor = engine == "das" ? "tuned" : "stock",
        workload = workload,
        box = "testbox",
        threads = 8,
        date = "2026-07-28",
        cmd = "test-cmd",
        versions = engine == "das" ? "" : "whisper.cpp @ abcdef123")
    r.tests |> insert("asr:jfk.wav", BenchTest(ms = 197.1lf, audio_s = 11.0lf))
    r.tests |> insert("asr:gb1.wav", BenchTest(ms = 6585.0lf, audio_s = 198.7lf))
    r.files |> emplace(InputFile(role = "weights", name = "ggml-tiny.bin",
        sha256 = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef", bytes = 77704715l))
    return <- r
}

def private make_model() : BenchModel {
    return <- BenchModel(gguf = "ggml-tiny.bin", arch = "Whisper tiny", size_bytes = 77704715l,
        runs <- [ <- make_asr_run("das", "asr"), <- make_asr_run("whisper-cli", "asr")])
}

[test]
def test_asr_round_trip(t : T?) {
    t |> run("ASR-shaped runs survive write/read") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        write_bench_records(TMP, models)
        var back : array<BenchModel>
        t |> success(read_bench_records(TMP, back), "store re-reads")
        t |> equal(length(back), 1)
        t |> equal(length(back[0].runs), 2)
        t |> equal(back[0].runs[0].workload, "asr")
        t |> success(key_exists(back[0].runs[0].tests, "asr:jfk.wav"), "clip test key survives")
        t |> equal(back[0].runs[0].tests?["asr:jfk.wav"]?.ms ?? 0.0lf, 197.1lf)
        t |> equal(back[0].runs[0].tests?["asr:jfk.wav"]?.audio_s ?? 0.0lf, 11.0lf)
        t |> equal(length(back[0].runs[0].files), 1)
        t |> equal(back[0].runs[0].files[0].role, "weights")
        t |> equal(back[0].runs[0].files[0].bytes, 77704715l)
        t |> equal(back[0].runs[1].versions, "whisper.cpp @ abcdef123")
        remove(TMP)
    }
}

[test]
def test_upsert_workload_key(t : T?) {
    t |> run("workload joins the upsert identity") @(t : T?) {
        var m = BenchModel(gguf = "gemma-4-E4B-it-Q8_0.gguf")
        // an LLM run and an audio-chat run share (box, engine, backend, flavor) — the workload
        // field is the only thing keeping the audio measurement from clobbering the LLM row
        var llm = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", workload = "",
            box = "m1", threads = 8, date = "2026-07-26", cmd = "llm")
        llm.tests |> insert("pp512", BenchTest(tok_s = 259.6lf))
        var chat = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", workload = "audio-chat",
            box = "m1", threads = 8, date = "2026-07-28", cmd = "audio")
        chat.tests |> insert("asr:jfk3.wav", BenchTest(ms = 5881.0lf, audio_s = 33.0lf))
        upsert_bench_run(m.runs, llm)
        upsert_bench_run(m.runs, chat)
        t |> equal(length(m.runs), 2)
        // a re-measure of the SAME workload replaces in place
        var chat2 = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", workload = "audio-chat",
            box = "m1", threads = 8, date = "2026-07-29", cmd = "audio2")
        upsert_bench_run(m.runs, chat2)
        t |> equal(length(m.runs), 2)
        t |> equal(m.runs[1].date, "2026-07-29")
    }
}

[test]
def test_annotations(t : T?) {
    t |> run("model-level note + run-level comment land where selected") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        var annos : array<BenchAnnotation>
        annos |> push(BenchAnnotation(gguf = "ggml-tiny.bin", comment = "board caveat"))
        annos |> push(BenchAnnotation(gguf = "ggml-tiny.bin", engine = "whisper-cli",
            comment = "reference note"))
        annos |> push(BenchAnnotation(gguf = "absent.gguf", comment = "must not land"))
        apply_annotations(models, annos)
        t |> equal(models[0].note, "board caveat")
        t |> equal(models[0].runs[0].comment, "")            // das run: not selected
        t |> equal(models[0].runs[1].comment, "reference note")
    }
}

def private make_oracle_run(pp, tg : double) : BenchRun {
    var r = BenchRun(engine = "das", backend = "metal", flavor = "tuned", box = "m1",
        threads = 8, date = "2026-07-30", cmd = "test")
    if (pp > 0.0lf) {
        r.tests |> insert("pp512", BenchTest(tok_s = pp, stddev = 1.0lf))
    }
    if (tg > 0.0lf) {
        r.tests |> insert("tg128", BenchTest(tok_s = tg, stddev = 0.5lf))
    }
    return <- r
}

[test]
def test_oracle_compare(t : T?) {
    t |> run("the oracle gate is one-sided with a warn band") @(t : T?) {
        let stored <- make_oracle_run(100.0lf, 50.0lf)
        // clean pass: -1% pp, +2% tg
        var fresh <- make_oracle_run(99.0lf, 51.0lf)
        var lines : array<string>
        t |> success(oracle_compare(fresh, stored, 3.0lf, 5.0lf, lines) == OracleVerdict.ok, "small drift is OK")
        t |> equal(length(lines), 2)
        // -4% pp lands in the warn band; -6% tg fails and wins as worst
        lines |> clear()
        fresh.tests |> insert("pp512", BenchTest(tok_s = 96.0lf))
        t |> success(oracle_compare(fresh, stored, 3.0lf, 5.0lf, lines) == OracleVerdict.warn, "-4% warns")
        lines |> clear()
        fresh.tests |> insert("tg128", BenchTest(tok_s = 47.0lf))
        t |> success(oracle_compare(fresh, stored, 3.0lf, 5.0lf, lines) == OracleVerdict.fail, "-6% fails")
        // gains never fail, but a gain past the fail bar is flagged for verification
        lines |> clear()
        fresh.tests |> insert("pp512", BenchTest(tok_s = 110.0lf))
        fresh.tests |> insert("tg128", BenchTest(tok_s = 50.0lf))
        t |> success(oracle_compare(fresh, stored, 3.0lf, 5.0lf, lines) == OracleVerdict.ok, "+10% stays OK")
        var flagged = false
        for (l in lines) {
            flagged = flagged || find(l, "suspicious gain") >= 0
        }
        t |> success(flagged, "the +10% line carries the suspicion flag")
        // a stored key missing from the fresh run is a coverage fail
        lines |> clear()
        let partial <- make_oracle_run(99.0lf, 0.0lf)
        t |> success(oracle_compare(partial, stored, 3.0lf, 5.0lf, lines) == OracleVerdict.fail, "missing tg row fails")
        // a stored row with nothing comparable (e.g. ms-shaped tests) fails too
        lines |> clear()
        var msonly = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", box = "m1",
            threads = 8, date = "2026-07-30", cmd = "test")
        msonly.tests |> insert("asr:jfk.wav", BenchTest(ms = 197.1lf))
        t |> success(oracle_compare(fresh, msonly, 3.0lf, 5.0lf, lines) == OracleVerdict.fail, "no tok/s tests fails")
    }
}

[test]
def test_asr_catalog_invariants(t : T?) {
    t |> run("catalog stays reproducible") @(t : T?) {
        var cat <- asr_catalog()
        var libriCount = 0
        for (spec in cat) {
            if (!empty(spec.libri_short)) {
                libriCount++
            }
            for (wav in spec.buckets) {
                t |> success(wav != "gemma4a_test2.wav", "no untraceable clips in the catalog")
            }
            let wl = asr_workload(spec)
            t |> success(wl == "asr" || wl == "audio-chat", "workload classifies")
        }
        t |> equal(libriCount, 1)   // LibriSpeech scope: Parakeet v3 only
    }
}
