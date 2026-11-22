options gen2
options persistent_heap   // + explicit deletes below (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; load_checkpoint resolves "llama" from the registry
require _model_tier
require daslib/jobque_boost
require daslib/fio
require math
require dasllama/dasllama_math  // the fused knob
require _fused_decode_cells   // fused_vs_std_bit_exact, FUSED_STEPS, FUSED_CTX

// Fused decode chains (team_parallel_stages: qkv→attn→wo and w1/w3→w2 as ONE dispatch each) vs
// the per-op dispatch path: bit-exact equivalence. The fused path claims bit-identical arithmetic
// — same backend row dots, same per-head norm/rope/store order, same act/quant/add per element —
// so greedy ids AND final logits must match EXACTLY, not within tolerance. stories15M sits below
// the fused size gate, so both sides run with target_chunk_work forced to 1 (identical
// dispatch decisions per-op path included). Every cell skips off the
// JIT or without its model file. The SmolLM2 cells and the gpt-oss-4L
// mx4 expert chain's (`_fused_decode_cells.das`) run on their models'
// blocks.

let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

[test(description = "On Q8 stories15M with dispatch forced threaded, fused decode stage chains produce the per-op dispatch path's greedy ids and final logits bit for bit")]
def test_fused_decode_bit_exact(t : T?) {
    t |> run("fused stage chains == per-op dispatches, ids + logits bit-exact (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)  // fused chains are Q8-only
        with_threaded_jobque() {   // team mode on — the fused gate requires it
            var ids_std, ids_fused : array<int64>
            var lg_std, lg_fused : array<float>
            set_fused_decode(false)
            var s1 <- make_run_state(c)
            greedy_ids(tr, s1, FUSED_STEPS, ids_std, lg_std)
            delete s1
            set_fused_decode(true)
            var s2 <- make_run_state(c)
            greedy_ids(tr, s2, FUSED_STEPS, ids_fused, lg_fused)
            delete s2
            var id_bad = 0
            for (i in range(FUSED_STEPS)) {
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
        delete tr
    }
}

// stories15M's head_size (48) gates the ATTENTION chain off — this twin runs a real GGUF with
// head_size 64, so both chains (qkv→attn→wo AND w1/w3→w2) take the fused path. The auto-selected
// backend's pass (arm64-gen on an M1 — the interleaved rows core) runs on the SmolLM2 block of
// test_gpu_resident_llama.das; this load is under a "portable" pin — the rows core every host shares, and the EPYC box's profile-pinned
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
// The q8_0-KV twin of the deep-ctx witness: the sliced K pass runs the int8×int8 quantized-query
// dot and the witness recomputes through the SAME dot (kv_score), so the 1e-4 in-chain contract
// holds under the block codec too.
// pre_post_norm arches (Gemma2/3): the chains defer the residual add and run the post-norm as a
// serial tail after the rendezvous. gemma-3-1b covers qk-norm + dual-rope sliding (5:1) + the
// attn deferral; gemma-2-2b covers softcap + GeGLU + qd != dim + alternating sliding.
[test(description = "On the pre/post-norm arches gemma-3-1b, gemma-2-2b and gemma-4 E2B under the portable backend, fused decode chains match the per-op path's greedy ids and final logits bit for bit")]
def test_fused_decode_pre_post_norm(t : T?) {
    t |> run("fused chains bit-exact on pre/post-norm arches (needs the gemma ggufs)") @(t : T?) {
        return if (!jit_only(t))
        let prev_backend = active_kernel_backend()
        with_threaded_jobque() {
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
                tr.config.seq_len = min(tr.config.seq_len, FUSED_CTX)
                fused_vs_std_bit_exact(t, tr)
                delete tr
            }
            clear_kernel_backend_pin()
        }
        select_kernel_backend(prev_backend)
    }
}

// Native K-quant layers (kq v2 regrain): the chains run Q8_K-form activations — the layer-input
// quantize, the w2-input requant at 256-row stage-0 grain, and the wo-input requant per head
// GROUP (Qwen3's head_size 128 = head pairs). Qwen3-4B Q5_K_M covers k5 + k6 (ffn_down);
// Qwen3-4B Q4_K_M covers k4 + k6. Bit-exact vs the per-op kq path, same contract as the q8
// arms. Auto backend only: a kq load repacks (kq_repacked) only when the active backend
// carries the kq rows cores — a pin without them would silently compare std vs std.
[test(description = "On Qwen3-4B Q5_K_M and Q4_K_M repacked for the native K-quant rows cores, fused decode chains with Q8_K activations match the per-op kq path's greedy ids and final logits bit for bit")]
def test_fused_decode_kquant(t : T?) {
    t |> run("fused chains bit-exact on native K-quant layers (needs the kq ggufs)") @(t : T?) {
        return if (!jit_only(t))
        with_threaded_jobque() {
            for (name in ["Qwen3-4B-Instruct-2507-Q5_K_M.gguf", "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"]) {
                let path = path_join(models_dir(), name)
                if (!stat(path).is_valid) {
                    // deliberately NOT `t |> skip` — the loop goes on to the models that ARE present
                    t |> success(true, "skipped: {path} not present")
                    continue
                }
                var tr <- load_gguf(path, QuantMode.q8)
                tr.config.seq_len = min(tr.config.seq_len, FUSED_CTX)
                if (!tr.kq_repacked) {
                    t |> success(true, "skipped: active backend has no kq rows cores")
                } else {
                    fused_vs_std_bit_exact(t, tr)
                }
                delete tr
            }
        }
    }
}
