options gen2
options persistent_heap   // + explicit deletes below (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; load_checkpoint resolves "llama" from the registry
require daslib/jobque_boost
require daslib/fio
require math
require dasllama/dasllama_math  // setup_dasllama_jobque_ + target_chunk_work + fused knob

// Fused decode chains (team_parallel_stages: qkv→attn→wo and w1/w3→w2 as ONE dispatch each) vs
// the per-op dispatch path: bit-exact equivalence. The fused path claims bit-identical arithmetic
// — same backend row dots, same per-head norm/rope/store order, same act/quant/add per element —
// so greedy ids AND final logits must match EXACTLY, not within tolerance. stories15M sits below
// the fused size gate, so both sides run with target_chunk_work forced to 1 (identical
// dispatch decisions per-op path included).

def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")
// override the machine-local model root with DASLLAMA_MODELS_DIR (same rail as test_parity)
def private models_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}
let N_STEPS = 24

def private argmax_of(s : Session; vocab : int64) : int64 {
    var best = 0l
    for (i in range64(1l, vocab)) {
        if (s.logits[i] > s.logits[best]) {
            best = i
        }
    }
    return best
}

def private greedy_ids(tr : Model; var s : Session; var ids : array<int64>; var logits : array<float>) {
    ids |> reserve(N_STEPS)
    var token = 1l   // BOS
    var pos = 0l
    for (_i in range(N_STEPS)) {
        forward(tr, s, token, pos)
        pos++
        token = argmax_of(s, tr.config.vocab_size)
        ids |> push(token)
    }
    logits := s.logits
}

[test]
def test_fused_decode_bit_exact(t : T?) {
    t |> run("fused stage chains == per-op dispatches, ids + logits bit-exact (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)  // fused chains are Q8-only
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()   // team mode on — the fused gate requires it
            set_target_chunk_work(1l)
            var ids_std : array<int64>
            var ids_fused : array<int64>
            var lg_std : array<float>
            var lg_fused : array<float>
            set_fused_decode(false)
            var s1 <- make_run_state(c)
            greedy_ids(tr, s1, ids_std, lg_std)
            delete s1
            set_fused_decode(true)
            var s2 <- make_run_state(c)
            greedy_ids(tr, s2, ids_fused, lg_fused)
            delete s2
            var id_bad = 0
            for (i in range(N_STEPS)) {
                if (ids_std[i] != ids_fused[i]) {
                    id_bad++
                }
            }
            t |> equal(0, id_bad)
            var lg_bad = 0
            for (i in range64(c.vocab_size)) {
                if (lg_std[i] != lg_fused[i]) {
                    lg_bad++
                }
            }
            t |> equal(0, lg_bad)
            delete ids_std
            delete ids_fused
            delete lg_std
            delete lg_fused
        }
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}

def private fused_vs_std_bit_exact(t : T?; tr : Model) {
    let c = tr.config
    var ids_std : array<int64>
    var ids_fused : array<int64>
    var lg_std : array<float>
    var lg_fused : array<float>
    set_fused_decode(false)
    var s1 <- make_run_state(c)
    greedy_ids(tr, s1, ids_std, lg_std)
    delete s1
    set_fused_decode(true)
    var s2 <- make_run_state(c)
    greedy_ids(tr, s2, ids_fused, lg_fused)
    delete s2
    var id_bad = 0
    for (i in range(N_STEPS)) {
        if (ids_std[i] != ids_fused[i]) {
            id_bad++
        }
    }
    t |> equal(0, id_bad)
    var lg_bad = 0
    for (i in range64(c.vocab_size)) {
        if (lg_std[i] != lg_fused[i]) {
            lg_bad++
        }
    }
    t |> equal(0, lg_bad)
    delete ids_std
    delete ids_fused
    delete lg_std
    delete lg_fused
}

// stories15M's head_size (48) gates the ATTENTION chain off — this twin runs a real GGUF with
// head_size 64, so both chains (qkv→attn→wo AND w1/w3→w2) take the fused path. Two passes: the
// auto-selected backend (arm64-gen on an M1 — the interleaved rows core), then a fresh load
// under a "portable" pin — the rows core every host shares, and the EPYC box's profile-pinned
// decode config; without the pin an x64 auto-select picks an intrinsic tier with no rows core
// and the fused gate silently falls back (the A/B would compare std vs std).
// Deep-context flash-decode slicing: past FLASH_DECODE_SLICE (256) positions the fused attn
// chain splits each head's KV pass into ctx-derived slices and combines per head. The slicing
// reassociates the softmax/AV sums; a 1-ulp attention difference can then cross an int8 requant
// boundary and amplify through the remaining Q8 layers, so logit-level A/Bs cannot gate this
// (measured: ~3e-6 at every attention output, ~0.5 at the final logits). The real contract is
// checked INSIDE the chain: set_flash_decode_check makes every sliced layer recompute exact
// single-pass attention from the raw scores and panic past fp32 tolerance (1e-4). The logit
// bound here is a loose structural backstop (a dropped/double-counted slice is O(10)).
[test]
def test_fused_flash_deep_ctx(t : T?) {
    t |> run("flash-sliced fused chain: per-layer witness + structural logit bound (needs SmolLM2-135M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        let prev_backend = active_kernel_backend()
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            pin_kernel_backend("portable")
            var tr <- load_gguf(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 384l)
            let c = tr.config
            let PREFILL = 300  // past the 256 boundary -> the fused side slices (nsl == 2)
            let STEPS = 4
            var lg_std : array<float>   // mode 0's per-step logits, STEPS x vocab (teacher-forced)
            lg_std |> resize(int(int64(STEPS) * c.vocab_size))
            var worst = 0.0
            for (mode in range(2)) {
                set_fused_decode(mode == 1)
                set_flash_decode_check(mode == 1)   // panics inside the chain past 1e-4
                var s <- make_run_state(c)
                var pos = 0l
                for (i in range(PREFILL)) {   // fixed pseudo-prompt, per-token (decode path both sides)
                    forward(tr, s, int64(i % 100 + 2), pos)
                    pos++
                }
                for (st in range(STEPS)) {
                    forward(tr, s, int64(st * 37 + 11), pos)
                    pos++
                    let base = int64(st) * c.vocab_size
                    if (mode == 0) {
                        for (i in range64(c.vocab_size)) {
                            lg_std[base + i] = s.logits[i]
                        }
                    } else {
                        for (i in range64(c.vocab_size)) {
                            worst = max(worst, abs(lg_std[base + i] - s.logits[i]))
                        }
                    }
                }
                delete s
            }
            t |> success(get_flash_decode_check_count() > 0, "the sliced path ran and every layer passed the witness ({get_flash_decode_check_count()} layers checked)")
            set_flash_decode_check(false)
            set_fused_decode(true)
            t |> success(worst < 4.0, "teacher-forced logits within the structural bound (worst |diff| = {worst})")
            delete lg_std
            delete tr
            clear_kernel_backend_pin()
        }
        set_target_chunk_work(prev_thresh)
        select_kernel_backend(prev_backend)
    }
}

// The q8_0-KV twin of the deep-ctx witness: the sliced K pass runs the int8×int8 quantized-query
// dot and the witness recomputes through the SAME dot (kv_score), so the 1e-4 in-chain contract
// holds under the block codec too.
[test]
def test_fused_flash_deep_ctx_q8(t : T?) {
    t |> run("flash-sliced fused chain under q8_0 KV: per-layer witness + structural logit bound (needs SmolLM2-135M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        let prev_backend = active_kernel_backend()
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            pin_kernel_backend("portable")
            var tr <- load_gguf(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 384l)
            let c = tr.config
            let PREFILL = 300  // past the 256 boundary -> the fused side slices (nsl == 2)
            let STEPS = 4
            var lg_std : array<float>
            lg_std |> resize(int(int64(STEPS) * c.vocab_size))
            var worst = 0.0
            for (mode in range(2)) {
                set_fused_decode(mode == 1)
                set_flash_decode_check(mode == 1)   // panics inside the chain past 1e-4
                var s <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
                var pos = 0l
                for (i in range(PREFILL)) {
                    forward(tr, s, int64(i % 100 + 2), pos)
                    pos++
                }
                for (st in range(STEPS)) {
                    forward(tr, s, int64(st * 37 + 11), pos)
                    pos++
                    let base = int64(st) * c.vocab_size
                    if (mode == 0) {
                        for (i in range64(c.vocab_size)) {
                            lg_std[base + i] = s.logits[i]
                        }
                    } else {
                        for (i in range64(c.vocab_size)) {
                            worst = max(worst, abs(lg_std[base + i] - s.logits[i]))
                        }
                    }
                }
                delete s
            }
            t |> success(get_flash_decode_check_count() > 0, "the sliced q8 path ran and every layer passed the witness ({get_flash_decode_check_count()} layers checked)")
            set_flash_decode_check(false)
            set_fused_decode(true)
            t |> success(worst < 4.0, "teacher-forced logits within the structural bound (worst |diff| = {worst})")
            delete lg_std
            delete tr
            clear_kernel_backend_pin()
        }
        set_target_chunk_work(prev_thresh)
        select_kernel_backend(prev_backend)
    }
}

// pre_post_norm arches (Gemma2/3): the chains defer the residual add and run the post-norm as a
// serial tail after the rendezvous. gemma-3-1b covers qk-norm + dual-rope sliding (5:1) + the
// attn deferral; gemma-2-2b covers softcap + GeGLU + qd != dim + alternating sliding.
[test]
def test_fused_decode_pre_post_norm(t : T?) {
    t |> run("fused chains bit-exact on pre/post-norm arches (needs the gemma ggufs)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let prev_backend = active_kernel_backend()
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            pin_kernel_backend("portable")   // the rows core every host shares (see the attn-chain twin)
            for (name in ["gemma-3-1b-it-Q8_0.gguf", "gemma-2-2b-it-Q8_0.gguf", "gemma-4-E2B-it-Q8_0.gguf"]) {
                let path = path_join(models_dir(), name)
                if (!stat(path).is_valid) {
                    // deliberately NOT `t |> skip` — it would mark the WHOLE arm skipped even
                    // when the loop goes on to exercise the models that ARE present
                    t |> success(true, "skipped: {path} not present")
                    continue
                }
                var tr <- load_gguf(path, QuantMode.q8)
                fused_vs_std_bit_exact(t, tr)
                delete tr
            }
            clear_kernel_backend_pin()
        }
        set_target_chunk_work(prev_thresh)
        select_kernel_backend(prev_backend)
    }
}

// Native K-quant layers (kq v2 regrain): the chains run Q8_K-form activations — the layer-input
// quantize, the w2-input requant at 256-row stage-0 grain, and the wo-input requant per head
// GROUP (Qwen3's head_size 128 = head pairs). Qwen3-4B Q5_K_M covers k5 + k6 (ffn_down);
// Qwen3-4B Q4_K_M covers k4 + k6. Bit-exact vs the per-op kq path, same contract as the q8
// arms. Auto backend only: a kq load repacks (kq_repacked) only when the active backend
// carries the kq rows cores — a pin without them would silently compare std vs std.
[test]
def test_fused_decode_kquant(t : T?) {
    t |> run("fused chains bit-exact on native K-quant layers (needs the kq ggufs)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            for (name in ["Qwen3-4B-Instruct-2507-Q5_K_M.gguf", "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"]) {
                let path = path_join(models_dir(), name)
                if (!stat(path).is_valid) {
                    // deliberately NOT `t |> skip` — the loop goes on to the models that ARE present
                    t |> success(true, "skipped: {path} not present")
                    continue
                }
                var tr <- load_gguf(path, QuantMode.q8)
                if (!tr.kq_repacked) {
                    t |> success(true, "skipped: active backend has no kq rows cores")
                } else {
                    fused_vs_std_bit_exact(t, tr)
                }
                delete tr
            }
        }
        set_target_chunk_work(prev_thresh)
    }
}

// The fused MoE expert chain (mx4 experts: gate+up rows + bias + act + requant -> down rows +
// bias + weighted reduce as ONE rendezvous) vs the 3-dispatch groupn sequence. gpt-oss-20b is
// the mx4 fixture (12GB — the large-tier PARITY_FULL gate, same rule as the fam cells).
[test]
def test_fused_decode_moe_chain(t : T?) {
    t |> run("fused MoE expert chain bit-exact on gpt-oss (needs the gguf + DASLLAMA_PARITY_FULL)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (empty(get_env_variable("DASLLAMA_PARITY_FULL"))) {
            t |> skip("large-tier fixture (set DASLLAMA_PARITY_FULL=1)")
            return
        }
        let path = path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            var tr <- load_gguf(path, QuantMode.q8)
            if (!kernel_backend_has_mx4_rows()) {
                t |> success(true, "skipped: active backend has no mx4 rows core")
            } else {
                fused_vs_std_bit_exact(t, tr)
            }
            delete tr
        }
        set_target_chunk_work(prev_thresh)
    }
}

[test]
def test_fused_decode_attn_chain(t : T?) {
    t |> run("fused attention chain bit-exact on SmolLM2-135M (needs the gguf)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        let prev_backend = active_kernel_backend()
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            if (kernel_backend_has_rows()) {   // auto backend pass (skipped where auto has no rows core)
                var tr <- load_gguf(path, QuantMode.q8)
                fused_vs_std_bit_exact(t, tr)
                delete tr
            }
            pin_kernel_backend("portable")     // fresh row-major load under the shared rows core
            var tr2 <- load_gguf(path, QuantMode.q8)
            fused_vs_std_bit_exact(t, tr2)
            delete tr2
            clear_kernel_backend_pin()
        }
        set_target_chunk_work(prev_thresh)
        select_kernel_backend(prev_backend)
    }
}
