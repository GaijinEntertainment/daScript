options gen2
options stack = 524288
require dastest/testing_boost public
require ../performance/exchange_schema.das
require ../performance/profile_common.das   // the REAL record writer — the anti-drift arm
require dasllama/dasllama_version
require daslib/json_boost   // the %json~ fixtures + write_json
require daslib/fio

// The exchange validator's net: the ENTIRE in-tree corpus must pass shape-level validation
// (schema drift between the writers and the validator fails here first), the live writer's
// stamped output must pass submission-grade, and hostile documents must be rejected.

let RECORDS_DIR = "modules/dasLLAMA/performance/records"
let TMP = "{RECORDS_DIR}/_test_exchange.json"   // _ prefix: excluded from the site merge

def private read_text(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

def private report(t : T?; tag : string; ok : bool; errors : array<string>) {
    t |> success(ok, tag)
    if (!ok) {
        for (e in errors) {
            t |> failure("{tag}: {e}")
        }
    }
}

[test]
def test_corpus_records(t : T?) {
    t |> run("every in-tree record store passes shape validation") @(t : T?) {
        var stores <- list_record_stores(RECORDS_DIR)
        t |> success(length(stores) >= 5, "the corpus is present")
        for (p in stores) {
            var errors : array<string>
            let ok = validate_record_store(read_text(p), errors)
            report(t, base_name(p), ok, errors)
            delete errors
        }
        delete stores
    }
}

[test]
def test_corpus_sidecars(t : T?) {
    t |> run("every current-format in-tree sidecar passes shape validation and indexes") @(t : T?) {
        // parse_sidecar_info is the format discriminator: pre-box / four-field-box sidecars are
        // frozen history the exchange can never serve, so only indexable files carry the claim
        var current = 0
        var legacy = 0
        dir(RECORDS_DIR) $(name) {
            if (find(name, ".tune.") >= 0 && ends_with(name, ".json")) {
                let text = read_text("{RECORDS_DIR}/{name}")
                var info : SidecarInfo
                if (!parse_sidecar_info(text, info)) {
                    legacy++
                } else {
                    current++
                    var errors : array<string>
                    report(t, name, validate_sidecar(text, errors), errors)
                    delete errors
                    t |> success(info.kernel_count > 0, "{name} has kernels")
                    t |> equal(info.dasllama_version, 0, "{name} predates the release counter")
                    t |> success(!empty(info.identity.platform) && !empty(info.identity.cpu), "{name} box identity parses")
                    // pre-counter sidecars are shape-valid history, but the exchange must refuse them
                    var serrors : array<string>
                    t |> success(!validate_sidecar_submission(text, serrors), "{name} is not submission-grade")
                    delete serrors
                }
            }
        }
        t |> success(current >= 4, "the current-format sidecar corpus is present ({current})")
        t |> success(current > legacy, "current format dominates ({current} current, {legacy} legacy)")
    }
}

// a fully stamped das run, as lcpp_bench emits since the release counter landed
def private make_das_run() : BenchRun {
    var r = BenchRun(engine = "das", backend = "cpu", flavor = "tuned", box = "testbox",
        threads = 8, date = "2026-08-10", cmd = "daslang -jit bench.das", env = "DASLLAMA_BOX=test",
        sha = "abc123def", version = "0.6.4", dasllama_version = DASLLAMA_VERSION,
        tune = "k4q8_tile_gen=mr8 (manifest)", tune_sha = "cafe0123beef", exec_fmt = "q8 blob")
    r.hardware.cpu = "Test CPU 9000"
    r.tests |> insert("pp512", BenchTest(tok_s = 100.0lf, stddev = 1.0lf))
    r.tests |> insert("tg128", BenchTest(tok_s = 50.0lf, stddev = 0.5lf))
    return <- r
}

def private make_ref_run() : BenchRun {
    var r = BenchRun(engine = "llama.cpp", backend = "cpu", flavor = "clean-cpu", box = "testbox",
        threads = 8, date = "2026-08-10", cmd = "llama-bench -m m.gguf", sha = "ebd048fc5")
    r.hardware.cpu = "Test CPU 9000"
    r.tests |> insert("pp512", BenchTest(tok_s = 80.0lf, stddev = 1.0lf))
    return <- r
}

def private write_store(var runs : array<BenchRun>) : string {
    var models : array<BenchModel>
    models |> emplace(BenchModel(gguf = "test-model-Q8_0.gguf", runs <- runs))
    write_bench_records(TMP, models)
    delete models
    return read_text(TMP)
}

[test]
def test_writer_round_trip(t : T?) {
    t |> run("the live writer's stamped output is submission-grade") @(t : T?) {
        var runs : array<BenchRun>
        runs |> emplace(make_das_run())
        runs |> emplace(make_ref_run())
        let text = write_store(runs)
        var errors : array<string>
        report(t, "shape", validate_record_store(text, errors), errors)
        delete errors
        var serrors : array<string>
        report(t, "submission", validate_record_submission(text, serrors), serrors)
        delete serrors
        remove(TMP)
    }
    t |> run("a submission claiming official is rejected") @(t : T?) {
        var r <- make_das_run()
        r.source = "official"
        var runs : array<BenchRun>
        runs |> emplace(r)
        let text = write_store(runs)
        var errors : array<string>
        t |> success(validate_record_store(text, errors), "shape still fine")
        delete errors
        var serrors : array<string>
        t |> success(!validate_record_submission(text, serrors), "submission refused")
        delete serrors
        remove(TMP)
    }
    t |> run("an unstamped das run is rejected at submission grade") @(t : T?) {
        var r <- make_das_run()
        r.dasllama_version = 0
        r.tune_sha = ""
        var runs : array<BenchRun>
        runs |> emplace(r)
        let text = write_store(runs)
        var serrors : array<string>
        t |> success(!validate_record_submission(text, serrors), "submission refused")
        delete serrors
        remove(TMP)
    }
    t |> run("a reference-only store is rejected at submission grade") @(t : T?) {
        var runs : array<BenchRun>
        runs |> emplace(make_ref_run())
        let text = write_store(runs)
        var serrors : array<string>
        t |> success(!validate_record_submission(text, serrors), "no das run - refused")
        delete serrors
        remove(TMP)
    }
}

[test]
def test_hostile_records(t : T?) {
    t |> run("malformed documents are rejected with named paths") @(t : T?) {
        var errors : array<string>
        t |> success(!validate_record_store("not json at all", errors), "garbage text")
        errors |> clear()
        t |> success(!validate_record_store("\{}", errors), "root object instead of array")
        errors |> clear()
        t |> success(!validate_record_store("[1, 2]", errors), "models are not objects")
        errors |> clear()
        t |> success(validate_record_store("[]", errors), "an empty store is shape-valid")
        t |> success(!validate_record_submission("[]", errors), "but not submission-grade")
        delete errors
    }
    t |> run("bad field values are rejected through the real writer") @(t : T?) {
        // each case rides the genuine writer so the validator sees real serializer output
        var r1 <- make_das_run()
        r1.threads = 0
        var runs1 : array<BenchRun>
        runs1 |> emplace(r1)
        var e1 : array<string>
        t |> success(!validate_record_store(write_store(runs1), e1), "threads 0")
        delete e1
        var r2 <- make_das_run()
        r2.date = "2026-8-1"
        var runs2 : array<BenchRun>
        runs2 |> emplace(r2)
        var e2 : array<string>
        t |> success(!validate_record_store(write_store(runs2), e2), "malformed date")
        delete e2
        var r3 <- make_das_run()
        r3.tests |> insert("weird", BenchTest(stddev = 1.0lf))
        var runs3 : array<BenchRun>
        runs3 |> emplace(r3)
        var e3 : array<string>
        t |> success(!validate_record_store(write_store(runs3), e3), "test row with neither tok_s nor ms")
        delete e3
        var r4 <- make_das_run()
        r4.tests |> insert("pp512", BenchTest(tok_s = -5.0lf))
        var runs4 : array<BenchRun>
        runs4 |> emplace(r4)
        var e4 : array<string>
        t |> success(!validate_record_store(write_store(runs4), e4), "negative tok/s")
        delete e4
        remove(TMP)
    }
}

[test]
def test_hostile_sidecars(t : T?) {
    t |> run("a minimal versioned sidecar passes both grades") @(t : T?) {
        var good = %json~
        {
            "kernels" : { "dot" : "vec8_u8", "softmax" : "vec8_u2" },
            "runtime" : { "jobque_spin_us" : 30000 },
            "provenance" : {
                "box" : "darwin|arm64|MacBookPro18,2|25F84|Apple M1 Max",
                "engine_sha" : "8a538d795",
                "written" : "2026-08-10T00:00:00Z",
                "noise" : "ok",
                "dasllama_version" : "3"
            }
        }
        %%
        let text = write_json(good)
        var errors : array<string>
        report(t, "shape", validate_sidecar(text, errors), errors)
        delete errors
        var serrors : array<string>
        report(t, "submission", validate_sidecar_submission(text, serrors), serrors)
        delete serrors
        var info : SidecarInfo
        t |> success(parse_sidecar_info(text, info), "info parses")
        t |> equal(info.dasllama_version, 3)
        t |> equal(info.identity.cpu, "Apple M1 Max")
        t |> equal(info.kernel_count, 2)
        t |> equal(info.noise, "ok")
    }
    t |> run("hostile sidecar shapes are rejected") @(t : T?) {   // nolint:STYLE038 - flat one-shape-per-arm rejection list; no arm stands alone
        var errors : array<string>
        t |> success(!validate_sidecar("\{}", errors), "empty object")
        errors |> clear()
        var inj = %json~
        {
            "kernels" : { "dot; DROP TABLE users" : "vec8" },
            "provenance" : { "box" : "a|b|c|d|e", "engine_sha" : "x" }
        }
        %%
        t |> success(!validate_sidecar(write_json(inj), errors), "injection-shaped kernel name")
        errors |> clear()
        var numwin = %json~
        {
            "kernels" : { "dot" : 42 },
            "provenance" : { "box" : "a|b|c|d|e", "engine_sha" : "x" }
        }
        %%
        t |> success(!validate_sidecar(write_json(numwin), errors), "non-string winner")
        errors |> clear()
        var nested = %json~
        {
            "kernels" : { "dot" : "vec8" },
            "runtime" : { "knob" : { "deep" : 1 } },
            "provenance" : { "box" : "a|b|c|d|e", "engine_sha" : "x" }
        }
        %%
        t |> success(!validate_sidecar(write_json(nested), errors), "nested runtime knob")
        errors |> clear()
        var badbox = %json~
        {
            "kernels" : { "dot" : "vec8" },
            "provenance" : { "box" : "darwin|arm64|nocpu", "engine_sha" : "x" }
        }
        %%
        t |> success(!validate_sidecar(write_json(badbox), errors), "box string with three fields")
        errors |> clear()
        // an unknown top-level section would bypass the privacy strip (it only walks provenance)
        var extra = %json~
        {
            "kernels" : { "dot" : "vec8" },
            "provenance" : { "box" : "a|b|c|d|e", "engine_sha" : "x" },
            "notes" : { "binary" : "/Users/victim/bin/daslang" }
        }
        %%
        t |> success(!validate_sidecar(write_json(extra), errors), "unknown top-level section")
        errors |> clear()
        // a provenance value carrying markup: the charset is the injection guard for the page
        var xss = %json~
        {
            "kernels" : { "dot" : "vec8" },
            "provenance" : { "box" : "linux|x86_64|<img src=x onerror=alert(1)>|b|cpu", "engine_sha" : "x" }
        }
        %%
        t |> success(!validate_sidecar(write_json(xss), errors), "markup in a provenance value")
        errors |> clear()
        var badsha = %json~
        {
            "kernels" : { "dot" : "vec8" },
            "provenance" : { "box" : "a|b|c|d|e", "engine_sha" : "<svg onload=alert(1)>" }
        }
        %%
        t |> success(!validate_sidecar(write_json(badsha), errors), "non-identifier engine_sha")
        delete errors
    }
}

[test]
def test_box_parse(t : T?) {
    t |> run("box identity parsing") @(t : T?) {
        var b : BoxIdentity
        t |> success(parse_box_string("darwin|arm64|MacBookPro18,2|25F84|Apple M1 Max", b))
        t |> equal(b.platform, "darwin")
        t |> equal(b.model_id, "MacBookPro18,2")
        t |> equal(b.cpu, "Apple M1 Max")
        t |> success(parse_box_string("linux|x86_64|||AMD Ryzen 7 3700X", b), "blank model_id/os_build allowed")
        t |> success(!parse_box_string("darwin|arm64|m|b", b), "four fields refused")
        t |> success(!parse_box_string("darwin|arm64|m|b|", b), "empty cpu refused")
        t |> success(!parse_box_string("", b), "empty refused")
    }
}
