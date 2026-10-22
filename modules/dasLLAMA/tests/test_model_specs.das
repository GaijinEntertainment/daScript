options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
require dastest/testing_boost public
require ../performance/profile_common.das

// The shape invariants of the model-set table (../performance/model_specs.das, re-exported by
// profile_common — the one source of truth behind llm_catalog / official_catalog /
// models_provenance). Model-free data checks: a malformed row reds every PR before any box
// fetches or measures against it.

[test]
def test_model_specs_invariants(t : T?) {
    t |> run("spec rows are keyed, unique, and facet-coherent") @(t : T?) {
        var inscope specs <- model_specs()
        var seen, displays : table<string>
        for (s in specs) {
            t |> success(!empty(s.file), "every spec is keyed by file")
            t |> success(!key_exists(seen, s.file), "file keys are unique: {s.file}")
            seen |> insert(s.file)
            if (!empty(s.display)) {
                t |> success(!key_exists(displays, s.display), "display names are unique: {s.display}")
                displays |> insert(s.display)
            }
            if (s.official) {
                t |> success(!empty(s.url) || !empty(s.recipe),
                    "an official-board spec pins its provenance: {s.file}")
                // the board pregate refuses evidence-less models, so an official spec without
                // a frozen fixture could never board — catch it here, not mid-sweep
                t |> success(!empty(s.parity.gen),
                    "an official-board spec carries parity evidence: {s.file}")
            }
        }
        delete seen
        delete displays
    }
    t |> run("parity evidence is well-formed") @(t : T?) {
        var inscope specs <- model_specs()
        for (s in specs) {
            if (!empty(s.parity.gen)) {
                t |> success(empty(s.parity.prompt) != empty(s.parity.prompt_text),
                    "evidence carries exactly one prompt form (pinned ids, or text encoded in-model): {s.file}")
                t |> success(!s.parity.prefix_bar || length(s.parity.gen) >= 2,
                    "a prefix_bar fixture holds at least two ids, so its half-span bar is a token or more: {s.file}")
            } else {
                t |> success(!(s.parity.kq_native || s.parity.dn_chunked || s.parity.moe_reference
                        || s.parity.moe_per_expert || s.parity.require_dn_kq || s.parity.prefix_bar),
                    "arm flags without evidence are dead data: {s.file}")
            }
        }
    }
}

[test]
def test_fixture_bar_rule(t : T?) {
    t |> run("the fixture compare: token for token, or a matching prefix of half the span under prefix_bar") @(t : T?) {
        let want <- [for (i in range(12)); 100l + int64(i)]
        var got := want
        t |> equal(prefix_match_len(got, want), 12l, "equal rows match whole")
        t |> success(fixture_ok(false, 12l, 12l) && fixture_ok(true, 12l, 12l), "a whole match holds under both forms")
        got[11] = 7l
        t |> equal(prefix_match_len(got, want), 11l, "a flipped last id stops the prefix there")
        t |> success(!fixture_ok(false, 11l, 12l), "the strict form reds a flipped last id")
        t |> success(fixture_ok(true, 11l, 12l), "the half-span form holds a flipped last id")
        t |> success(fixture_ok(true, 6l, 12l), "the bar sits at half the span: 6 of 12 hold")
        t |> success(!fixture_ok(true, 5l, 12l), "5 of 12 red the half-span form")
        got[11] = want[11]
        got[3] = 7l
        t |> equal(prefix_match_len(got, want), 3l, "a flip inside the first half stops the prefix there")
        t |> success(!fixture_ok(true, 3l, 12l) && !fixture_ok(false, 3l, 12l), "a flip inside the first half reds both forms")
        let short <- [for (i in range(5)); 100l + int64(i)]
        t |> equal(prefix_match_len(short, want), 5l, "a shorter row matches no further than its own length")
        t |> equal(prefix_match_len(want, short), 5l, "nor does a longer one")
    }
}

[test]
def test_model_spec_views(t : T?) {
    t |> run("llm_catalog / official_catalog are exact filters over the table") @(t : T?) {
        var inscope specs <- model_specs()
        var inscope prof <- llm_catalog()
        var inscope off <- official_catalog()
        var want_prof = 0
        var want_off = 0
        for (s in specs) {
            want_prof += empty(s.display) ? 0 : 1
            want_off += s.official ? 1 : 0
        }
        t |> equal(length(prof), want_prof, "the profiled view keeps every displayed row and nothing else")
        t |> equal(length(off), want_off, "the official view keeps every official row and nothing else")
        for (s in prof) {
            t |> success(!empty(s.display), "a profiled row has a display name: {s.file}")
        }
        for (s in off) {
            t |> success(s.official, "an official view row is flagged official: {s.file}")
        }
    }
    t |> run("spec_by_file: a table row comes back whole, an off-table name comes back empty") @(t : T?) {
        var inscope hit <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
        t |> equal(hit.file, "Qwen3-0.6B-Q8_0.gguf")
        t |> success(!empty(hit.parity.gen), "the row's evidence rides along")
        var inscope miss <- spec_by_file("no-such-model.gguf")
        t |> equal(miss.file, "", "a miss is the empty spec")
        t |> success(empty(miss.parity.gen), "a miss carries no evidence")
        let shard = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"
        var inscope by_path <- spec_by_file(shard)
        t |> equal(by_path.file, shard, "a subfolder row comes back by its models-dir path")
        var inscope by_base <- spec_by_file("Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf")
        t |> equal(by_base.file, shard, "and by the base name the pregate holds")
    }
    t |> run("counting_prompt renders 1..N with the trailing comma") @(t : T?) {
        t |> equal(counting_prompt(3), "1, 2, 3,")
        t |> equal(counting_prompt(1), "1,")
    }
    t |> run("mmproj_companion pairs the vision rows and only them") @(t : T?) {
        var inscope g12 <- spec_by_file("gemma-4-12B-it-Q4_K_M.gguf")
        t |> equal(mmproj_companion(g12), "mmproj-gemma-4-12B-it-BF16.gguf")
        var inscope oss <- spec_by_file("gpt-oss-20b-mxfp4.gguf")
        t |> equal(mmproj_companion(oss), "", "a text-only row has no vision pairing")
        var shuffled <- ModelSpec(file = "toy.gguf", companions <- [
            ProvEntry(name = "coco-val2017-000000039769.jpg"),
            ProvEntry(name = "mmproj-toy.gguf")])
        t |> equal(mmproj_companion(shuffled), "mmproj-toy.gguf", "the mmproj is found by name, not by companion position")
        delete shuffled
        // the board's image legs need real coverage: at least two official vision decoders
        var inscope off <- official_catalog()
        var vision = 0
        for (s in off) {
            vision += empty(mmproj_companion(s)) ? 0 : 1
        }
        t |> success(vision >= 2, "the official board carries >= 2 vision rows (got {vision})")
    }
    t |> run("the image fixture is provenanced (some spec row owns its pin)") @(t : T?) {
        var inscope entries <- models_provenance()
        var pinned = false
        for (e in entries) {
            pinned ||= e.name == bench_image_fixture()
        }
        t |> success(pinned, "{bench_image_fixture()} rides a companion list, so every box provisions it")
        var inscope specs <- model_specs()
        var seen : table<string>
        for (s in specs) {
            for (c in s.companions) {
                t |> success(!key_exists(seen, c.name), "a companion rides exactly one row's companions: {c.name}")
                seen |> insert(c.name)
            }
        }
        delete seen
    }
}

[test]
def test_models_provenance_invariants(t : T?) {
    t |> run("the provenance view is well-formed") @(t : T?) {
        var inscope entries <- models_provenance()
        var seen : table<string>
        for (e in entries) {
            t |> success(!empty(e.name), "every entry is named")
            t |> success(!key_exists(seen, e.name), "names are unique")
            seen |> insert(e.name)
            t |> success(e.root == "llm" || e.root == "whisper", "root is a known symbol: {e.name}")
            // a registry-served file pins bytes AND sha; a converted file carries its recipe —
            // one of the two provenance stories must hold for every entry
            let sha_ok = length(e.sha256) == 64
            t |> success(sha_ok || !empty(e.recipe), "sha256 or recipe present: {e.name}")
            if (!empty(e.url)) {
                t |> success(starts_with(e.url, "https://"), "url is https: {e.name}")
                t |> success(e.bytes > 0l && sha_ok, "a fetched entry pins bytes and sha256: {e.name}")
            }
        }
        delete seen
    }
    t |> run("the provenance view is exactly the pinned rows of both halves") @(t : T?) {
        var inscope entries <- models_provenance()
        var names : table<string>
        for (e in entries) {
            names |> insert(e.name)
        }
        var want = 0
        var inscope aspecs <- asr_catalog()
        var asr_pins : table<string>
        for (a in aspecs) {
            if (!empty(a.prov.name)) {
                want++
                asr_pins |> insert(a.prov.name)
                t |> success(key_exists(names, a.prov.name), "an ASR carrier's pin is in the view: {a.prov.name}")
            }
            for (c in a.companions) {
                want++
                t |> success(key_exists(names, c.name), "an ASR companion is in the view: {c.name}")
            }
        }
        var inscope specs <- model_specs()
        for (s in specs) {
            if (!empty(s.url) || !empty(s.recipe)) {
                want++
                t |> success(key_exists(names, s.file), "a pinned text row is in the view: {s.file}")
            } elif (!key_exists(asr_pins, s.file)) {   // an audio-chat carrier's pin rides its asr_catalog spec
                t |> success(!key_exists(names, s.file), "an unpinned text row stays out of the view: {s.file}")
            }
            for (c in s.companions) {
                want++
                t |> success(key_exists(names, c.name), "a text companion is in the view: {c.name}")
            }
        }
        t |> equal(length(entries), want, "the view carries every pin once and nothing else")
        delete asr_pins
        delete names
    }
}

[test]
def test_board_provenance(t : T?) {
    t |> run("a board box's fetch holds every file the board sweeps read, and no test-only carrier") @(t : T?) {
        var names : table<string>
        for (e in board_provenance()) {
            names |> insert(e.name)
        }
        for (s in official_catalog()) {
            t |> success(key_exists(names, s.file), "official row {s.file}")
        }
        t |> success(key_exists(names, bench_image_fixture()), "the image fixture")
        for (a in asr_catalog()) {
            t |> success(key_exists(names, a.das_model), "ASR carrier {a.das_model}")
            t |> success(empty(a.das_mmproj) || key_exists(names, a.das_mmproj), "ASR mmproj {a.das_mmproj}")
        }
        for (s in tts_catalog()) {
            t |> success(key_exists(names, s.file), "TTS model {s.file}")
        }
        t |> success(!key_exists(names, "Mistral-7B-Instruct-v0.3-Q8_0.gguf"), "a row off the board stays out")
        for (n in keys(names)) {
            t |> success(!(n |> starts_with("tts_oracle")), "the on-box parity oracles stay out: {n}")
        }
        delete names
    }
}
