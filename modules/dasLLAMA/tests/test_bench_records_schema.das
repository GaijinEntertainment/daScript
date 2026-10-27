options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
require dastest/testing_boost public
require ../performance/profile_common.das
require dasllama/dasllama_version
require dasllama/dasllama_exchange_schema   // the outlier rule the recorder's pass applies

// The record store is the site's single source of truth; this is the net under its schema:
// ASR-shaped runs (workload, ms/audio_s tests, files) must round-trip the JSON store, the
// upsert identity must treat workload as part of the key, and annotations must land on the
// rows they select — and only those. The remote-desktop probe's process read rides here too; its
// live cell skips where the running executable's name is longer than Linux keeps of one.

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
        dasllama_version = engine == "das" ? DASLLAMA_RELEASE : 0,
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
def test_write_reports_failure(t : T?) {
    t |> run("a record file that cannot be opened is a false return, never a silent partial write") @(t : T?) {
        // a directory is never openable for writing, on any platform
        t |> success(!write_records_text("modules/dasLLAMA/performance/records", "[]"), "unopenable path is false")
        let models : array<BenchModel>
        t |> success(!write_bench_records("modules/dasLLAMA/performance/records", models), "the typed writer carries the status")
    }
}

[test]
def test_asr_round_trip(t : T?) {
    t |> run("ASR-shaped runs survive write/read") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        t |> success(write_bench_records(TMP, models), "the store writes")
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
        t |> equal(back[0].runs[0].dasllama_version, DASLLAMA_RELEASE)
        t |> success(back[0].runs[0].dasllama_version >= 1, "the counter starts at 1 - 0 means unstamped")
        t |> equal(back[0].runs[1].dasllama_version, 0)
        remove(TMP)
    }
}

[test]
def test_tts_round_trip(t : T?) {
    t |> run("a TTS cell's das and reference rows survive write/read under one test key") @(t : T?) {
        let key = tts_test_key(20, "af_heart")
        var das = BenchRun(engine = "das", backend = "metal", flavor = "tuned", workload = "tts", box = "testbox",
            threads = 8, date = "2026-10-21", cmd = "lcpp_bench --tts", dasllama_version = DASLLAMA_RELEASE)
        das.tests |> insert(key, BenchTest(ms = 2471.5lf, audio_s = 187.5lf))
        das.files |> emplace(InputFile(role = "corpus", name = "g2p_corpus.json", bytes = 1000l))
        var ref = BenchRun(engine = "mlx-audio", backend = "metal", flavor = "stock", workload = "tts", box = "testbox",
            threads = 8, date = "2026-10-21", cmd = "tts_ref_bench.py", versions = "mlx 0.32.4 (requirements-mlx.txt)")
        ref.tests |> insert(key, BenchTest(ms = 3875.2lf, audio_s = 187.0lf))
        var models : array<BenchModel>
        models |> emplace(BenchModel(gguf = "kokoro-82m.gguf", arch = "Kokoro-82M", runs <- [ <- das, <- ref]))
        t |> success(write_bench_records(TMP, models), "the store writes")
        var back : array<BenchModel>
        t |> success(read_bench_records(TMP, back), "store re-reads")
        t |> equal(length(back[0].runs), 2)
        t |> equal(back[0].runs[0].workload, "tts")
        t |> equal(back[0].runs[0].tests?[key]?.ms ?? 0.0lf, 2471.5lf)
        t |> equal(back[0].runs[0].tests?[key]?.audio_s ?? 0.0lf, 187.5lf)
        t |> equal(back[0].runs[1].tests?[key]?.ms ?? 0.0lf, 3875.2lf)
        t |> equal(back[0].runs[0].files[0].role, "corpus")
        t |> equal(back[0].runs[1].engine, "mlx-audio")
        remove(TMP)
    }
    t |> run("a TTS row and an LLM row of one model and leg are two rows") @(t : T?) {
        var m = BenchModel(gguf = "kokoro-82m.gguf")
        var llm = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", box = "m1", cmd = "llm")
        var tts = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", workload = "tts", box = "m1", cmd = "tts")
        upsert_bench_run(m.runs, llm)
        upsert_bench_run(m.runs, tts)
        t |> equal(length(m.runs), 2)
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
        // the Metal-rail roster: metal_served decides which rows get GPU board cells and
        // which reference legs pair GPU-vs-GPU, so a silent flip re-lanes the public board
        let metal_families <- {"Parakeet-TDT v2", "Parakeet-TDT v3", "Whisper tiny", "Whisper large-v3-turbo", "Canary-Qwen 2.5B",
                               "Qwen3-ASR-0.6B", "Gemma-4 E2B audio", "Qwen3-Omni-30B audio", "GigaAM v3 e2e-RNNT"}
        for (spec in cat) {
            if (!empty(spec.libri_short)) {
                libriCount++
            }
            for (wav in spec.buckets) {
                t |> success(wav != "gemma4a_test2.wav", "no untraceable clips in the catalog")
            }
            let wl = asr_workload(spec)
            var inscope chats <- {"Gemma-4 E2B audio", "Qwen3-Omni-30B audio"}
            t |> equal(wl, key_exists(chats, spec.display) ? "audio-chat" : "asr",
                       "{spec.display}: the chat models are audio-chat, every speech-to-text model asr")
            t |> equal(spec.metal_served, key_exists(metal_families, spec.display),
                       "{spec.display}: metal_served matches the Metal-rail roster")
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

let LINUX_COMM_WIDTH = 15   // the characters of an executable's name a Linux process list keeps

[test]
def test_process_listed(t : T?) {
    t |> run("a listing names an executable by its file name, and the excepted path does not count") @(t : T?) {
        let system_daemon = "/System/Library/PrivateFrameworks/CoreParsec.framework/parsecd"
        let app_daemon = "/Applications/Parsec.app/Contents/MacOS/parsecd"
        t |> success(executable_in("/usr/sbin/cfprefsd\n{app_daemon}\n", "parsecd", "CoreParsec.framework"), "the app's daemon, by path")
        t |> success(executable_in("systemd\nparsecd\n", "parsecd", ""), "a bare name, as Linux lists it")
        t |> success(!executable_in("{system_daemon}\n/usr/bin/grep\n", "parsecd", "CoreParsec.framework"), "the system daemon sits under the excepted path")
        t |> success(executable_in("{system_daemon}\n", "parsecd", ""), "with no exception the same line counts")
        t |> success(!executable_in("sh -c pgrep -fl parsecd\n/usr/libexec/parsecd-helper\n", "parsecd", ""), "a line that only mentions the name is no executable of that name")
        t |> success(!executable_in("", "parsecd", ""), "an empty listing names nothing")
    }
    t |> run("the live probe reads executables, not command lines") @(t : T?) {
        t |> equal(process_listed("no_such_daemon_qq"), false, "a name only the probe's own command line spells is not listed")
        var me = base_name(get_command_line_arguments()[0])
        if (me |> ends_with(".exe")) {
            me = slice(me, 0, length(me) - 4)
        }
        if (length(me) > LINUX_COMM_WIDTH) {
            t |> skip("own executable `{me}` is longer than the {LINUX_COMM_WIDTH} characters a Linux process list keeps")
            return
        }
        t |> success(process_listed(me), "the running executable `{me}` is listed")
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
    t |> run("the -w scope table: llm/asr/image/tts cells per workload, unknown rejected") @(t : T?) {
        let all = workload_scope("all")
        t |> success(all.known && all.llm && all.asr && all.image && all.tts, "all selects every cell set")
        let llm = workload_scope("llm")
        t |> success(llm.known && llm.llm && !llm.asr && llm.image && !llm.tts, "llm selects text + image (image rides the llm sweep)")
        let asr = workload_scope("asr")
        t |> success(asr.known && !asr.llm && asr.asr && !asr.image && !asr.tts, "asr selects only the audio cells")
        let image = workload_scope("image")
        t |> success(image.known && !image.llm && !image.asr && image.image && !image.tts, "image narrows to the image cells alone")
        let tts = workload_scope("tts")
        t |> success(tts.known && !tts.llm && !tts.asr && !tts.image && tts.tts, "tts selects only the synthesis cells")
        let bogus = workload_scope("bogus")
        t |> success(!bogus.known && !bogus.llm && !bogus.asr && !bogus.image && !bogus.tts, "an unknown workload is rejected and selects nothing")
        t |> success(!workload_scope("").known, "the empty workload is rejected")
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
            var inscope models : array<BenchModel>
            t |> success(read_bench_records(path, models), "{path} parses")
            for (m in models) {
                for (r in m.runs) {
                    if (r.workload != "image-chat") {
                        continue
                    }
                    rows++
                    t |> success(find(r.cmd, bench_image_fixture()) >= 0,
                        "{base_name(path)} {m.gguf} {r.backend}/{r.flavor}: the cmd receipt names {bench_image_fixture()}")
                    t |> success(find(r.cmd, "--image-mmproj ") >= 0 || find(r.cmd, "--mmproj ") >= 0,
                        "{base_name(path)} {m.gguf} {r.backend}/{r.flavor}: the cmd receipt passes an mmproj (either engine's flag spelling)")
                }
            }
            delete models
        }
        t |> success(rows > 0, "the sweep found image-chat rows to gate ({rows})")
        delete stores
    }
}

[test]
def test_batched_rows_carry_the_leg_shape(t : T?) {
    t |> run("every committed batched row's receipt carries its leg's shape, both engines alike") @(t : T?) {
        // the cpu legs' 128 x 3 shape and the stock cpu arm's --no-op-offload are what make a
        // batched row comparable to the rest of the store (METHODOLOGY.md); a stored row shows
        // them nowhere but its cmd receipt
        var stores <- list_record_stores("modules/dasLLAMA/performance/records")
        var rows = 0
        for (path in stores) {
            var inscope models : array<BenchModel>
            t |> success(read_bench_records(path, models), "{path} parses")
            for (m in models) {
                for (r in m.runs) {
                    let das_batched = r.engine == "das" && find(r.cmd, " --npl ") >= 0
                    let ref_batched = find(r.cmd, "llama-batched-bench") >= 0
                    if (!das_batched && !ref_batched) {
                        continue
                    }
                    rows++
                    let at = "{base_name(path)} {m.gguf} {r.engine} {r.backend}/{r.flavor}"
                    if (r.backend != "cpu") {
                        t |> success(find(r.cmd, "--npl-plen") < 0 && find(r.cmd, "--no-op-offload") < 0,
                            "{at}: a GPU leg's batched row rides the flat rows' shape and carries no cpu-only flag")
                        t |> success(!ref_batched || find(r.cmd, "-npp 512 ") >= 0,
                            "{at}: a GPU reference's streams start from the flat rows' 512-token prompt")
                    } else {
                        t |> success(!das_batched || find(r.cmd, "--npl-plen 128 --npl-reps 3") >= 0,
                            "{at}: the cpu leg's das child takes the 128 x 3 shape")
                        t |> success(!ref_batched || find(r.cmd, "-npp 128 ") >= 0,
                            "{at}: the cpu leg's reference streams start from 128-token prompts")
                        t |> equal(find(r.cmd, "--no-op-offload") >= 0, r.flavor == "stock",
                            "{at}: --no-op-offload rides the stock cpu arm alone")
                    }
                }
            }
            delete models
        }
        t |> success(rows > 0, "the sweep found batched rows to gate ({rows})")
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

[test]
def test_parse_llama_batched_bench(t : T?) {
    t |> run("the batched reference parse: the S_TG cell of the row at npl, the other rows and noise ignored") @(t : T?) {
        // fixture text from a live llama-batched-bench run (M5, the 1B) - the header, a separator, two rows
        let out = ("main: n_kv_max = 4096, n_batch = 2048, n_ubatch = 512, flash_attn = 1\n"
            + "|    PP |     TG |    B |   N_KV |   T_PP s | S_PP t/s |   T_TG s | S_TG t/s |      T s |    S t/s |\n"
            + "|-------|--------|------|--------|----------|----------|----------|----------|----------|----------|\n"
            + "|   512 |    128 |    1 |    640 |    0.052 |  9846.15 |    0.404 |   316.83 |    0.456 |  1403.51 |\n"
            + "|   512 |    128 |    4 |   2560 |    0.190 | 10778.95 |    0.497 |  1030.18 |    0.687 |  3726.35 |\n"
            + "\nllama_perf_context_print: total time = 1234.5 ms\n")
        t |> numericEqual(float(llama_batched_bench_tg(out, 4l)), 1030.18)
        t |> numericEqual(float(llama_batched_bench_tg(out, 1l)), 316.83)
        t |> numericEqual(float(llama_batched_bench_tg(out, 8l)), 0.0, "no row at that npl = no rate, never a neighbour's")
        t |> numericEqual(float(llama_batched_bench_tg("main: n_kv_max = 4096\nerror: failed to load model\n", 4l)), 0.0, "a run that printed no table reads 0")
        let garbage = "|   512 |    128 |    4 |   2560 |    0.190 | 10778.95 |    0.497 |     n/a |    0.687 |  3726.35 |\n"
        t |> numericEqual(float(llama_batched_bench_tg(garbage, 4l)), 0.0, "a cell that is not a number reads 0")
        t |> equal(llama_batched_bench_sibling("/no/such/dir/llama-bench"), "", "no sibling beside a missing reference = no batched exe")
    }
    t |> run("the batched reference row carries the leg's arms after the row's shape, and a dead exe reads no row") @(t : T?) {
        let br = llama_batched_ref_row("/no/such/dir/llama-batched-bench", "m.gguf", 0, 6, 512l, 128l, 4l, 1,
            ["--no-op-offload", "--cpu-strict", "1"])
        t |> success(ends_with(br.cmd, "-npl 4 -fa on --no-op-offload --cpu-strict 1"), "the CPU leg's arms follow the shape: {br.cmd}")
        t |> success(!empty(br.why), "a run that printed no row names why")
        t |> numericEqual(float(br.row.tok_s), 0.0, "and its row reads 0")
    }
}

[test]
def test_mtmd_versions_bin_param(t : T?) {
    t |> run("the version stamp follows the bin argument - the stock build stamps its own checkout") @(tt : T?) {
        // a non-git dir has no commit, so only the patch tag survives - the stamp's shape is
        // the claim (commit resolution itself belongs to git, not to this seam)
        let foreign = mtmd_versions("/nonexistent/build/bin/llama-mtmd-cli")
        tt |> success(find(foreign, "mtmd-timing.patch") >= 0, "the patch tag is always stamped: {foreign}")
        tt |> success(find(foreign, "llama.cpp") == 0, "the stamp names the engine: {foreign}")
        let dflt = mtmd_versions()
        tt |> success(find(dflt, "mtmd-timing.patch") >= 0, "the default arm keeps the CPU ref build's stamp: {dflt}")
    }
}

[test]
def test_image_ref_flag_invariants(t : T?) {
    t |> run("every committed llama.cpp image row's flags match its backend/flavor stamp") @(t : T?) {
        // the exact defect this gate pins: a stock -ngl 0 run without --no-mmproj-offload
        // minted a Metal-speed encode under a cpu label
        var stores <- list_record_stores("modules/dasLLAMA/performance/records")
        var rows = 0
        for (path in stores) {
            var inscope models : array<BenchModel>
            t |> success(read_bench_records(path, models), "{path} parses")
            for (m in models) {
                for (r in m.runs) {
                    if (r.workload != "image-chat" || r.engine != "llama.cpp") {
                        continue
                    }
                    rows++
                    let cpu = r.backend == "cpu"
                    t |> equal(find(r.cmd, "-ngl 0") >= 0, cpu, "{m.gguf} {r.backend}/{r.flavor}: -ngl 0 iff cpu")
                    t |> equal(find(r.cmd, "--no-mmproj-offload") >= 0, cpu, "{m.gguf} {r.backend}/{r.flavor}: the tower stays off Metal iff cpu")
                    t |> equal(find(r.cmd, "--no-op-offload") >= 0, cpu && r.flavor == "stock", "{m.gguf} {r.backend}/{r.flavor}: -nopo on the stock cpu (amx) arm only")
                }
            }
            delete models
        }
        t |> success(rows > 0, "the sweep found llama.cpp image rows to gate ({rows})")
        delete stores
    }
}

[test]
def test_das_rows_cite_archived_generations(t : T?) {
    t |> run("every das row's tune_sha resolves to a committed generation archive") @(t : T?) {
        // a row citing a generation with no archive is unverifiable by the oracle's crossgen
        // gate and by any reader - the hand-minted row that once did has been re-minted
        var archives : table<string>
        dir("modules/dasLLAMA/performance/records") $(name) {
            let at = find(name, ".tune.")
            if (at >= 0 && name |> ends_with(".json")) {
                archives |> insert(slice(name, at + 6, at + 18))
            }
        }
        var stores <- list_record_stores("modules/dasLLAMA/performance/records")
        var rows = 0
        for (path in stores) {
            var inscope models : array<BenchModel>
            t |> success(read_bench_records(path, models), "{path} parses")
            for (m in models) {
                for (r in m.runs) {
                    if (r.engine != "das" || empty(r.tune_sha)) {
                        continue
                    }
                    rows++
                    t |> success(key_exists(archives, slice(r.tune_sha, 0, 12)),
                        "{base_name(path)} {m.gguf} {r.backend}/{r.flavor}/{r.workload}: tune_sha {slice(r.tune_sha, 0, 12)} has its archive")
                }
            }
            delete models
        }
        t |> success(rows > 0, "the sweep found stamped das rows ({rows})")
        delete archives
        delete stores
    }
}

[test]
def test_parse_mtmd_image_refusals(t : T?) {
    t |> run("the refusal arms nobody hit live: equality, counts-only, bad tail, noise lines") @(t : T?) {
        // encode == encode_prefill would divide a prefill rate by zero - refuse, never inf
        let eq = parse_mtmd_image("I mtmd batch encoding done in 100 ms\n"
            + "MTMD_TIMING encode_prefill_ms 100.0 decode_ms 80.0 total_ms 180.0 n_prefill 40 n_decode 8\n")
        t |> success(!eq.ok, "encode == encode+prefill = zero prefill wall - no cell")
        let countsOnly = parse_mtmd_image("I mtmd batch encoding done in 100 ms\n"
            + "MTMD_TIMING n_prefill 40 n_decode 8\n")
        t |> success(!countsOnly.ok, "counts without the ms split - no cell")
        let badTail = parse_mtmd_image("I mtmd batch encoding done in \n"
            + "MTMD_TIMING encode_prefill_ms 200.0 decode_ms 80.0 total_ms 280.0 n_prefill 40 n_decode 8\n")
        t |> success(!badTail.ok, "an encode line with no number never counts as seen")
        let noise = parse_mtmd_image("I mtmd batch encoding done in 100 ms\n"
            + "a reply line that mentions decode_ms 999 in prose\n"
            + "MTMD_TIMING encode_prefill_ms 200.0 decode_ms 80.0 total_ms 280.0 n_prefill 40 n_decode 8\n")
        t |> success(noise.ok, "a noise line without the MTMD_TIMING tag is skipped")
        t |> numericEqual(float(noise.decode_ms), 80.0, "the tagged line wins over prose")
    }
}

[test]
def test_sibling_mtmd(t : T?) {
    t |> run("the stock llama-mtmd-cli resolves as llama-bench's sibling, or not at all") @(t : T?) {
        t |> equal(sibling_mtmd(""), "", "no stock ref = no sibling")
        t |> equal(sibling_mtmd("/nonexistent/bin/llama-bench"), "", "a missing sibling refuses instead of naming a dead path")
    }
}

[test]
def test_held_annotation_lands(t : T?) {
    t |> run("an annotation's held reaches only the selected run, and rides the store") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        var annos : array<BenchAnnotation>
        annos |> emplace(BenchAnnotation(gguf = "ggml-tiny.bin", engine = "das",
            comment = "held off the board", held = "followup_general.md 219"))
        apply_annotations(models, annos)
        t |> equal(models[0].runs[0].held, "followup_general.md 219", "selected run held")
        t |> equal(models[0].runs[1].held, "", "reference run untouched")
        t |> success(write_bench_records(TMP, models), "the store writes")
        var back : array<BenchModel>
        t |> success(read_bench_records(TMP, back), "store re-reads")
        t |> equal(back[0].runs[0].held, "followup_general.md 219", "held round-trips")
        remove(TMP)
        delete back
        delete annos
        delete models
    }
}

// the audio-chat shape the rule was written on: a short clip read at a cold start against two
// even ones, and the das row that carries every clip's length
def private omni_rates(jfk_ms : double) : table<string; double> {
    return <- { "jfk.wav" => jfk_ms / 11.0lf, "jfk3.wav" => 553.2lf / 33.0lf, "gb1.wav" => 7075.4lf / 198.734375lf }
}

[test]
def test_audio_outlier_rule(t : T?) {
    t |> run("a clip past the multiple of the run's median rate is the outlier; an even run has none") @(t : T?) {
        t |> equal(audio_outlier_clips(omni_rates(34960.4lf)), [ "jfk.wav" ], "3178 ms a second against a median of 36")
        t |> equal(length(audio_outlier_clips(omni_rates(357.6lf))), 0, "33 ms a second sits inside the run")
        t |> equal(length(audio_outlier_clips({ "jfk.wav" => 3178.0lf, "jfk3.wav" => 16.8lf })), 0, "two clips rate nothing")
        t |> equal(length(audio_outlier_clips({ "a" => 100.0lf, "b" => 100.0lf, "c" => 100.0lf, "d" => 401.0lf })), 1, "an even count's median is the middle pair's mean")
        t |> equal(void_clips_of(" jfk.wav , gb1.wav,"), [ "jfk.wav", "gb1.wav" ], "the comma list reads trimmed, no empty name")
    }
    t |> run("the stored run is rated over the das row's clip lengths where it recorded none") @(t : T?) {
        var models : array<BenchModel>
        models |> emplace(make_model())
        let lengths <- clip_lengths(models, "ggml-tiny.bin", "asr")
        t |> equal(lengths?["jfk.wav"] ?? 0.0lf, 11.0lf)
        t |> equal(lengths?["gb1.wav"] ?? 0.0lf, 198.7lf)
        t |> equal(length(lengths), 2)
        var ref = BenchRun(engine = "llama-mtmd-cli", backend = "metal", flavor = "stock", workload = "audio-chat",
            box = "testbox", threads = 8, date = "2026-10-21", cmd = "test")
        ref.tests |> insert("asr:jfk.wav", BenchTest(ms = 34960.4lf))
        ref.tests |> insert("asr:jfk3.wav", BenchTest(ms = 553.2lf))
        ref.tests |> insert("asr:gb1.wav", BenchTest(ms = 7075.4lf))
        let borrowed <- { "jfk.wav" => 11.0lf, "jfk3.wav" => 33.0lf, "gb1.wav" => 198.734375lf }
        t |> equal(run_outlier_clips(ref, borrowed), [ "jfk.wav" ], "rated over the borrowed lengths")
        let unknown : table<string; double>
        t |> equal(length(run_outlier_clips(ref, unknown)), 0, "a clip with no length is not rated")
        delete models
    }
}

[test]
def test_void_outlier_pass(t : T?) {
    t |> run("the recorder re-measures the flagged clip once and voids what stays off") @(t : T?) {
        let lengths <- { "jfk.wav" => 11.0lf, "jfk3.wav" => 33.0lf, "gb1.wav" => 198.734375lf }
        var ref = BenchRun(engine = "llama-mtmd-cli", backend = "metal", flavor = "stock", workload = "audio-chat",
            box = "testbox", threads = 8, date = "2026-10-21", cmd = "test")
        ref.tests |> insert("asr:jfk.wav", BenchTest(ms = 34960.4lf))
        ref.tests |> insert("asr:jfk3.wav", BenchTest(ms = 553.2lf))
        ref.tests |> insert("asr:gb1.wav", BenchTest(ms = 7075.4lf))
        var asked : array<string>
        var voided <- void_outlier_clips(ref, lengths, "test ref") $(clip) {
            asked |> push(clip)
            return BenchTest(ms = 34000.0lf)   // the re-measure reads the same
        }
        t |> equal(asked, [ "jfk.wav" ], "the flagged clip alone is re-measured, once")
        t |> equal(voided, [ "jfk.wav" ])
        t |> equal(ref.void_clips, "jfk.wav", "the row carries the verdict")
        t |> equal(ref.tests?["asr:jfk.wav"]?.ms ?? 0.0lf, 34000.0lf, "the re-measure replaced the test")
        delete voided
        delete asked
    }
    t |> run("a re-measure that lands inside the run clears the clip") @(t : T?) {
        let lengths <- { "jfk.wav" => 11.0lf, "jfk3.wav" => 33.0lf, "gb1.wav" => 198.734375lf }
        var ref = BenchRun(engine = "llama-mtmd-cli", backend = "metal", flavor = "stock", workload = "audio-chat",
            box = "testbox", threads = 8, date = "2026-10-21", cmd = "test")
        ref.tests |> insert("asr:jfk.wav", BenchTest(ms = 34960.4lf))
        ref.tests |> insert("asr:jfk3.wav", BenchTest(ms = 553.2lf))
        ref.tests |> insert("asr:gb1.wav", BenchTest(ms = 7075.4lf))
        var voided <- void_outlier_clips(ref, lengths, "test ref") $(_clip) {
            return BenchTest(ms = 400.0lf)   // the cold start gone
        }
        t |> equal(length(voided), 0, "nothing voided")
        t |> equal(ref.void_clips, "")
        t |> equal(ref.tests?["asr:jfk.wav"]?.ms ?? 0.0lf, 400.0lf, "the fresh timing stands")
        delete voided
    }
    t |> run("an even run re-measures nothing") @(t : T?) {
        let lengths <- { "jfk.wav" => 11.0lf, "jfk3.wav" => 33.0lf, "gb1.wav" => 198.734375lf }
        var ref = BenchRun(engine = "llama-mtmd-cli", backend = "cpu", flavor = "stock", workload = "audio-chat",
            box = "testbox", threads = 8, date = "2026-10-21", cmd = "test")
        ref.tests |> insert("asr:jfk.wav", BenchTest(ms = 9365.4lf))
        ref.tests |> insert("asr:jfk3.wav", BenchTest(ms = 25961.6lf))
        ref.tests |> insert("asr:gb1.wav", BenchTest(ms = 158455.9lf))
        var asked = 0
        var voided <- void_outlier_clips(ref, lengths, "test ref") $(_clip) {
            asked++
            return BenchTest(ms = 1.0lf)
        }
        t |> equal(asked, 0, "the even run is never re-measured")
        t |> equal(length(voided), 0)
        t |> equal(ref.tests?["asr:jfk.wav"]?.ms ?? 0.0lf, 9365.4lf, "every timing stands")
        delete voided
    }
}

[test]
def test_board_policy_helpers(t : T?) {
    t |> run("a clip's length is the smallest positive one any run wrote, under its prefix alone") @(t : T?) {
        var lengths : table<string; double>
        borrow_clip_length(lengths, "asr:", "asr:jfk.wav", 11.0lf)
        borrow_clip_length(lengths, "asr:", "asr:jfk.wav", 12.0lf)
        borrow_clip_length(lengths, "asr:", "asr:jfk.wav", 0.0lf)
        borrow_clip_length(lengths, "asr:", "tts:g2p20@alba", 9.0lf)
        t |> equal(lengths?["jfk.wav"] ?? 0.0lf, 11.0lf, "the smaller length wins, a zero borrows nothing")
        borrow_clip_length(lengths, "asr:", "asr:jfk.wav", 10.5lf)
        t |> equal(lengths?["jfk.wav"] ?? 0.0lf, 10.5lf, "a smaller one later replaces it")
        t |> equal(length(lengths), 1, "a key under another prefix borrows nothing")
    }
    t |> run("a clip rates over its own length, else the borrowed one, else not at all") @(t : T?) {
        let lengths <- { "jfk.wav" => 11.0lf }
        var rates : table<string; double>
        rate_clip(rates, lengths, "gb1.wav", 990.0lf, 9.9lf)
        rate_clip(rates, lengths, "jfk.wav", 110.0lf, 0.0lf)
        rate_clip(rates, lengths, "hp0.wav", 500.0lf, 0.0lf)
        rate_clip(rates, lengths, "jfk3.wav", 0.0lf, 33.0lf)
        t |> equal(rates?["gb1.wav"] ?? 0.0lf, 100.0lf, "its own length")
        t |> equal(rates?["jfk.wav"] ?? 0.0lf, 10.0lf, "the borrowed length")
        t |> equal(length(rates), 2, "no length, or no wall, rates nothing")
    }
    t |> run("the voided clips are the rule's verdict and the named ones, sorted, each once") @(t : T?) {
        let rates <- { "a" => 100.0lf, "b" => 100.0lf, "c" => 100.0lf, "d" => 401.0lf }
        t |> equal(voided_clips(rates, "c, d"), [ "c", "d" ], "a named outlier counts once")
        t |> equal(voided_clips(rates, ""), [ "d" ], "the rule alone")
        let even <- { "a" => 100.0lf, "b" => 100.0lf, "c" => 100.0lf }
        t |> equal(length(voided_clips(even, "")), 0, "an even run with no names voids nothing")
    }
    t |> run("a selector takes a run where each of its fields is empty or equal") @(t : T?) {
        let run = RunKey(box = "m5max", engine = "das", backend = "metal", flavor = "tuned", workload = "asr")
        t |> success(key_selects(RunKey(), run), "an empty selector takes every run")
        t |> success(key_selects(RunKey(box = "m5max", workload = "asr"), run), "the fields it names match")
        t |> success(!key_selects(RunKey(engine = "llama.cpp"), run), "another engine")
        t |> success(!key_selects(RunKey(flavor = "accel"), run), "another flavor")
        t |> success(!key_selects(RunKey(backend = "cpu"), run), "another backend")
        t |> success(!key_selects(RunKey(box = "gnr"), run), "another box")
        t |> success(!key_selects(RunKey(workload = "tts"), run), "another workload")
    }
}
