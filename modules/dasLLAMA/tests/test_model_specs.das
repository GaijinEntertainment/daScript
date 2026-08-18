options gen2
require dastest/testing_boost public
require ../performance/profile_common.das

// The model-set table's shape invariants (profile_common's model_specs — the one source of
// truth behind llm_catalog / official_catalog / models_provenance). Model-free: pure data
// checks, so a malformed row reds every PR before any box fetches or measures against it.

[test]
def test_model_specs_invariants(t : T?) {
    t |> run("spec rows are keyed, unique, and facet-coherent") @(t : T?) {
        var inscope specs <- model_specs()
        var seen : table<string>
        var displays : table<string>
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
                // exactly one prompt form: pinned ids, or text the test encodes in-model
                t |> success(empty(s.parity.prompt) != empty(s.parity.prompt_text),
                    "evidence carries exactly one prompt form: {s.file}")
            } else {
                t |> success(!(s.parity.kq_native || s.parity.dn_chunked || s.parity.moe_reference
                        || s.parity.moe_per_expert || s.parity.require_dn_kq),
                    "arm flags without evidence are dead data: {s.file}")
            }
        }
    }
}

[test]
def test_models_provenance_invariants(t : T?) {
    t |> run("the provenance view is well-formed") @(t : T?) {
        var inscope entries <- models_provenance()
        t |> success(length(entries) >= 18, "the catalog is complete (18+ entries)")
        var seen : table<string>
        for (e in entries) {
            t |> success(!empty(e.name), "every entry is named")
            t |> success(!key_exists(seen, e.name), "names are unique")
            seen |> insert(e.name)
            t |> success(e.root == "llm" || e.root == "whisper", "root is a known symbol: {e.name}")
            // a registry-served file pins bytes; a converted file carries its recipe — one
            // of the two provenance stories must hold for every entry
            let sha_ok = length(e.sha256) == 64
            t |> success(sha_ok || !empty(e.recipe), "sha256 or recipe present: {e.name}")
            if (!empty(e.url)) {
                t |> success(starts_with(e.url, "https://"), "url is https: {e.name}")
            }
        }
        delete seen
    }
}
