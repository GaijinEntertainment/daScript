// Shared model-presence + size-tier gate (+ the parity eyeball rail) for the dasLLAMA
// model-loading tests.
//
// The parity/audio fixtures load multi-GB GGUFs; loading every one on every iteration is the
// suite's main time sink. This gate keeps the fast iteration loop to each arch's small carrier
// and defers the large models (Mistral-7B, gemma-4-12B/E4B, gpt-oss-20b, the MoEs) to a pre-PR
// run. The large tier runs only when DASLLAMA_PARITY_FULL is set. CI has no models on disk, so
// every case skips there regardless of tier — the gate only ever bites on a local box.

options gen2

module _model_tier shared public

require dastest/testing_boost public
require daslib/fio public
require strings public
require daslib/strings_boost   // split — the DASLLAMA_TEST_ARMS comma list
require dasllama/dasllama_gguf   // gguf_shard_paths — split models tier on their TOTAL size
require dasllama/dasllama_common   // decode_ — the eyeball rail detokenizes through the model
require dasllama/dasllama_env

// Models larger than this are large-tier. 6 GiB cleanly splits the ~5GB-and-under carriers
// (incl. gemma-4-E2B at 4.97GB) from the 7.7GB+ heavies.
let LARGE_TIER_BYTES = 6ul * 1024ul * 1024ul * 1024ul

// Machine-local model root; override with DASLLAMA_MODELS_DIR (e.g. the x64 handoff box).
def models_dir() : string {
    let env = g_env_harness.models_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}

// True when the large tier is enabled for this run (the pre-PR full sweep).
def parity_full_mode() : bool {
    return g_env_test.parity_full
}

// Arm filter for the metal suites: DASLLAMA_TEST_ARMS = comma-separated substrings; when set,
// only arm blocks whose name matches a token run. The skip is LOUD (t |> skip) so a filtered
// run can never masquerade as full coverage. Unset/empty = every arm runs. Drive through
// modules/dasLLAMA/tests/run.das — see modules/dasLLAMA/tests/CLAUDE.md.
def arm_on(t : T?; name : string) : bool {
    let filter = g_env_test.test_arms
    if (empty(filter)) {
        return true
    }
    for (tok in split(filter, ",")) {
        let tk = strip(tok)   // "arm12, batch" — the comma-adjacent space must not defeat the match
        if (!empty(tk) && find(name, tk) >= 0) {
            return true
        }
    }
    t |> skip("arm '{name}' filtered out (DASLLAMA_TEST_ARMS={filter})")
    return false
}

// Family filter for the model-loading suites: DASLLAMA_TEST_FAMILY = comma-separated family
// tokens (llama, qwen2, qwen3, phi3, gemma, ultravox, whisper, voxtral) matched EXACTLY — not
// substrings, so "llama" can never accidentally select a non-llama block. When set, only blocks tagged with a
// listed family run; model-free blocks (kernel units, the image mechanics arm) carry no tag and
// always run. Loud-skip contract identical to arm_on. Drive through run.das --family — the
// profiling cadence gate: a llama-format round re-verifies the llama family only.
def family_on(t : T?; name : string) : bool {
    let filter = g_env_test.test_family
    if (empty(filter)) {
        return true
    }
    for (tok in split(filter, ",")) {
        if (strip(tok) == name) {
            return true
        }
    }
    t |> skip("family '{name}' filtered out (DASLLAMA_TEST_FAMILY={filter})")
    return false
}

// EYEBALL RAIL (CODEREVIEW rule 18): every token-for-token generate cell logs both decoded
// streams — a red (or a stale-cache green) should read as TEXT in the log, not as an id diff.
// Decoded model text may carry raw newlines that would split a one-line log record, so every
// eyeball string renders through the \n escape.

def eyeball_str(s : string) : string => replace(s, "\n", "\\n")

def eyeball_text(m : Model; ids : array<int64>) : string => eyeball_str(decode_(m, ids))

def log_gen_texts(m : Model; tag : string; cpu_ids, gpu_ids : array<int64>) {
    to_log(LOG_INFO, "{tag} CPU text: {eyeball_text(m, cpu_ids)}\n")
    to_log(LOG_INFO, "{tag} GPU text: {eyeball_text(m, gpu_ids)}\n")
}

// Presence + tier gate for a model-loading test. Returns true if the caller should proceed to
// load `path`; otherwise registers a LOUD skip on `t` (model absent, or large-tier without
// DASLLAMA_PARITY_FULL) and returns false — the arm lands in the suite's skipped count instead
// of masquerading as a pass (two real x64 failures hid behind the passing form for months).
// `skip` does NOT unwind: the caller's `if (!model_available(t, path)) return` exits the arm.
def model_available(t : T?; path : string) : bool {
    let st = stat(path)
    if (!st.is_valid) {
        t |> skip("{base_name(path)} not present")
        return false
    }
    var total = st.size
    // split models tier on their TOTAL size; an incomplete split is "not present" (skip), not a panic
    let shards <- gguf_shard_paths(path, [missing_ok = true])
    if (empty(shards)) {
        t |> skip("{base_name(path)} split is incomplete (missing sibling shard)")
        return false
    }
    if (length(shards) > 1) {
        total = 0ul
        for (s in shards) {
            total += stat(s).size
        }
    }
    if (!parity_full_mode() && total > LARGE_TIER_BYTES) {
        t |> skip("{base_name(path)} is large-tier ({int64(total / (1024ul * 1024ul * 1024ul))} GB) — set DASLLAMA_PARITY_FULL=1 to run")
        return false
    }
    return true
}
