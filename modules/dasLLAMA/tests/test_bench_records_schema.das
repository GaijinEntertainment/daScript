options gen2
options _dasllama_internal = true
require dastest/testing_boost public
require ../performance/profile_common.das
require dasllama/dasllama_version

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
        noise = engine == "das" ? "ok" : "",
        parity = engine == "das" ? "ok" : "",
        dasllama_version = engine == "das" ? DASLLAMA_VERSION : 0,
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
        t |> equal(back[0].runs[0].noise, "ok")
        t |> equal(back[0].runs[1].noise, "")
        t |> equal(back[0].runs[0].parity, "ok")
        t |> equal(back[0].runs[1].parity, "")
        // the engine's own release counter: stamped on das rows, absent (0) on reference rows
        t |> equal(back[0].runs[0].dasllama_version, DASLLAMA_VERSION)
        t |> success(back[0].runs[0].dasllama_version >= 1, "the counter starts at 1 - 0 means unstamped")
        t |> equal(back[0].runs[1].dasllama_version, 0)
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
        let annos <- [BenchAnnotation(gguf = "ggml-tiny.bin", comment = "board caveat"),
            BenchAnnotation(gguf = "ggml-tiny.bin", engine = "whisper-cli", comment = "reference note"),
            BenchAnnotation(gguf = "absent.gguf", comment = "must not land")]
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

[test]
def test_optional_fields_round_trip(t : T?) {
    t |> run("encode_ms and void_clips survive write/read") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        models[0].runs[0].tests["asr:jfk.wav"].encode_ms = 395.2lf
        models[0].runs[0].void_clips = "jfk.wav"
        models[0].runs[0].hardware.remote_desktop = "off"
        models[0].runs[0].comment = "a hand note"
        write_bench_records(TMP, models)
        var back : array<BenchModel>
        t |> success(read_bench_records(TMP, back), "store re-reads")
        t |> equal(back[0].runs[0].tests?["asr:jfk.wav"]?.encode_ms ?? 0.0lf, 395.2lf, "encode_ms round-trips")
        t |> equal(back[0].runs[0].void_clips, "jfk.wav", "void_clips round-trips")
        t |> equal(back[0].runs[0].hardware.remote_desktop, "off", "remote_desktop stamp round-trips")
        t |> equal(back[0].runs[0].comment, "a hand note", "comment round-trips")
        remove(TMP)
        delete back
        delete models
    }
}

[test]
def test_void_clips_annotation_lands(t : T?) {
    t |> run("an annotation's void_clips reaches only the selected run") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        var annos : array<BenchAnnotation>
        annos |> emplace(BenchAnnotation(gguf = "ggml-tiny.bin", engine = "whisper-cli",
            comment = "ref runaway on jfk", void_clips = "jfk.wav"))
        apply_annotations(models, annos)
        t |> equal(models[0].runs[1].void_clips, "jfk.wav", "selected run voided")
        t |> equal(models[0].runs[0].void_clips, "", "das run untouched")
        delete annos
        delete models
    }
}

[test]
def test_list_record_stores_strictness(t : T?) {
    t |> run("the store lister admits records/\{box\}.json ONLY") @(t : T?) {
        var terr = ""
        let dir = temp_directory(terr)
        t |> success(terr == "", "temp_directory resolves")
        let root = path_join(dir, "dasllama_test_stores")
        mkdir_rec(root)
        for (fn in ["m9.json", "m9.tune.abcdef123456.json", "annotations.json", "_scratch.json", "notes.txt"]) {
            fopen(path_join(root, fn), "wb") $(f) {
                if (f != null) {
                    fwrite(f, "[]")
                }
            }
        }
        var stores <- list_record_stores(root)
        t |> equal(length(stores), 1, "one store admitted")
        t |> equal(base_name(stores[0]), "m9.json", "the box store, nothing else")
        delete stores
    }
}

[test]
def test_das_run_unstamped(t : T?) {
    t |> run("the store gate reads an empty tune stamp") @(t : T?) {
        var r <- make_asr_run("das", "asr")
        t |> success(das_run_unstamped(r), "no stamps = unstamped")
        r.tune = "k4q8_tile_gen=mr8 (manifest)"
        t |> success(das_run_unstamped(r), "tune without tune_sha is still unnameable")
        r.tune_sha = "abc123def456"
        t |> success(!das_run_unstamped(r), "both stamps = named winners")
        var rr <- make_asr_run("whisper-cli", "asr")
        t |> success(!das_run_unstamped(rr), "reference runs carry no stamp obligation")
        delete r
        delete rr
    }
}

[test]
def test_workload_scope(t : T?) {
    t |> run("the -w scope table: llm/asr/image cells per workload, unknown rejected") @(t : T?) {
        var llm = false
        var asr = false
        var image = false
        t |> success(workload_scope("all", llm, asr, image), "all is known")
        t |> success(llm && asr && image, "all selects every cell set")
        t |> success(workload_scope("llm", llm, asr, image), "llm is known")
        t |> success(llm && !asr && image, "llm selects text + image (image rides the llm sweep)")
        t |> success(workload_scope("asr", llm, asr, image), "asr is known")
        t |> success(!llm && asr && !image, "asr selects only the audio cells")
        t |> success(workload_scope("image", llm, asr, image), "image is known")
        t |> success(!llm && !asr && image, "image narrows to the image cells alone")
        t |> success(!workload_scope("bogus", llm, asr, image), "an unknown workload is rejected")
        t |> success(!llm && !asr && !image, "a rejection selects nothing")
        t |> success(!workload_scope("", llm, asr, image), "the empty workload is rejected")
    }
}

[test]
def test_stored_row_leg(t : T?) {
    t |> run("the stored-row -> rig-leg map: three stamps resolve, everything else refuses") @(t : T?) {
        var leg = "poison"
        t |> success(stored_row_leg("metal", "tuned", leg), "metal/tuned is a rig stamp")
        t |> equal(leg, "metal")
        t |> success(stored_row_leg("cpu", "tuned", leg), "cpu/tuned is a rig stamp")
        t |> equal(leg, "", "the plain cpu leg is the empty leg symbol")
        t |> success(stored_row_leg("cpu", "accel", leg), "cpu/accel is a rig stamp")
        t |> equal(leg, "accel")
        t |> success(!stored_row_leg("metal", "accel", leg), "accel is a CPU-tier stamp - no metal leg produces it")
        t |> equal(leg, "", "a refusal leaves no stale leg behind")
        t |> success(!stored_row_leg("vulkan", "tuned", leg), "a foreign backend has no rig leg")
        t |> success(!stored_row_leg("cpu", "debug-jit", leg), "a debug flavor has no rig leg")
    }
}

[test]
def test_image_chat_rows_pin_the_fixture(t : T?) {
    t |> run("every committed image-chat row's receipt names the one pinned picture") @(t : T?) {
        // the oracle re-measures a stored image row with bench_image_fixture(); a BenchRun has
        // no image-identity field, so the cmd receipt is where that assumption is checkable
        var stores <- list_record_stores("modules/dasLLAMA/performance/records")
        var rows = 0
        for (path in stores) {
            var models : array<BenchModel>
            t |> success(read_bench_records(path, models), "{path} parses")
            for (m in models) {
                for (r in m.runs) {
                    if (r.workload != "image-chat") {
                        continue
                    }
                    rows++
                    t |> success(find(r.cmd, bench_image_fixture()) >= 0,
                        "{base_name(path)} {m.gguf} {r.backend}/{r.flavor}: the cmd receipt names {bench_image_fixture()}")
                    t |> success(find(r.cmd, "mmproj") >= 0,
                        "{base_name(path)} {m.gguf} {r.backend}/{r.flavor}: the cmd receipt names its mmproj (either engine's flag spelling)")
                }
            }
            delete models
        }
        t |> success(rows > 0, "the sweep found image-chat rows to gate ({rows})")
        delete stores
    }
}

[test]
def test_parse_mtmd_image(t : T?) {
    t |> run("the image ref parse: encode lines sum, MTMD_TIMING splits, counts carry") @(t : T?) {
        // fixture text from a live patched-mtmd run (m1, E2B metal) - two noise lines around it
        let out = ("0.01.517.685 I mtmd batch encoding done in 116 ms\n"
            + "1.  **Analyze the image:**\n"
            + "MTMD_TIMING encode_prefill_ms 266.8 decode_ms 432.4 total_ms 699.1 n_prefill 159 n_decode 32\n")
        let r = parse_mtmd_image(out)
        t |> success(r.ok, "a complete run parses")
        t |> numericEqual(float(r.enc_ms), 116.0)
        t |> numericEqual(float(r.prefill_ms), 150.8)
        t |> numericEqual(float(r.decode_ms), 432.4)
        t |> equal(r.n_prefill, 159)
        t |> equal(r.n_decode, 32)
    }
    t |> run("multi-chunk encodes sum; incomplete output refuses") @(t : T?) {
        let two = ("I mtmd batch encoding done in 100 ms\nI mtmd batch encoding done in 50 ms\n"
            + "MTMD_TIMING encode_prefill_ms 200.0 decode_ms 80.0 total_ms 280.0 n_prefill 40 n_decode 8\n")
        let r2 = parse_mtmd_image(two)
        t |> success(r2.ok)
        t |> numericEqual(float(r2.enc_ms), 150.0)
        t |> numericEqual(float(r2.prefill_ms), 50.0)
        let noenc = parse_mtmd_image("MTMD_TIMING encode_prefill_ms 200.0 decode_ms 80.0 total_ms 280.0 n_prefill 40 n_decode 8\n")
        t |> success(!noenc.ok, "no encode line = no cell (an unpatched or non-image run must not mint)")
        let noline = parse_mtmd_image("I mtmd batch encoding done in 100 ms\nsome reply\n")
        t |> success(!noline.ok, "no MTMD_TIMING = no cell")
        let nocounts = parse_mtmd_image("I mtmd batch encoding done in 100 ms\n"
            + "MTMD_TIMING encode_prefill_ms 200.0 decode_ms 80.0 total_ms 280.0\n")
        t |> success(!nocounts.ok, "an old-patch line without position counts must not mint a bogus tok/s")
    }
}
