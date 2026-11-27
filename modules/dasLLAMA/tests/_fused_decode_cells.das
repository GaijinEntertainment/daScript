options gen2
options _dasllama_internal = true

module _fused_decode_cells shared public

//! the fused decode chains against the per-op path, on a model its caller loaded; no entry loads one

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math   // the fused knob + the backend pin's name
require daslib/jobque_boost
require math
require _model_tier   // greedy_ids, nan_wide_diff
require _cpu_gpu_model   // with_ctx_cut: the cells' cut contexts

// Fused decode chains (team_parallel_stages: qkv->attn->wo and w1/w3->w2 as ONE dispatch each, the MoE expert chain as one
// rendezvous) against the per-op dispatch path on a loaded model, bit for bit: same backend row dots, same per-head
// norm/rope/store order, same act/quant/add per element, so greedy ids AND final logits match EXACTLY. Each cell forces
// target_chunk_work to 1 (identical dispatch decisions on both sides). The deep-context flash-decode cells hold the sliced
// attention to an in-chain witness instead. Their models' blocks run them.

let FUSED_STEPS = 24   //! the greedy compare's decode steps
let FUSED_CTX = 64l   //! the GGUF sessions' context: FUSED_STEPS positions and room; a CPU f32 K/V at a file's whole context runs to tens of GB (77 on the 262144-position Qwen3-4B)

//! fused and per-op decode over FUSED_STEPS greedy steps on `tr`, each on a fresh f32 session: the greedy ids and the final
//! logits bit for bit
[cold_path]
def fused_vs_std_bit_exact(t : T?; tr : Model) {
    let c = tr.config
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

//! the run `title` of `fused_vs_std_bit_exact` on `tr` with dispatch forced threaded: SmolLM2-135M's head size 64 takes the
//! attention chain onto the fused path too, where stories15M's 48 gates it off
def fused_attn_chain_cell(t : T?; tr : Model; title : string) {
    unsafe {
        t |> run(title) @capture(& tr)(t : T ?) {
            with_threaded_jobque() {
                fused_vs_std_bit_exact(t, tr)
            }
        }
    }
}

//! 300 per-token prefill positions then 4 teacher-forced steps on `kv` sessions, per-op then fused with the in-chain witness
//! armed: the witness counted a layer (`what` names the path in its message), and the fused steps' logits within 4 of the
//! per-op ones
def private flash_deep_ctx_within(t : T?; tr : Model; kv : KVDtype; what : string) {
    let c = tr.config
    let PREFILL = 300  // past the 256 boundary -> the fused side slices (nsl == 2)
    let STEPS = 4
    var lg_std : array<float>   // mode 0's per-step logits, STEPS x vocab (teacher-forced)
    lg_std |> resize(int64(STEPS) * c.vocab_size)
    var worst = 0.0
    for (mode in range(2)) {
        set_fused_decode(mode == 1)
        set_flash_decode_check(mode == 1)   // panics inside the chain past 1e-4
        var s <- make_run_state(c, kv, kv)
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
                    worst = max(worst, nan_wide_diff(lg_std[base + i], s.logits[i]))
                }
            }
        }
        delete s
    }
    t |> success(get_flash_decode_check_count() > 0, "the sliced{what} path ran and every layer passed the witness ({get_flash_decode_check_count()} layers checked)")
    set_flash_decode_check(false)
    set_fused_decode(true)
    t |> success(worst < 4.0, "teacher-forced logits within the structural bound (worst |diff| = {worst})")
    delete lg_std
}

//! the deep-context witness cell `title` on `tr` under a `kv` K/V cache: `tr` loaded and run on its form's portable pin
//! (`cpu_cells_form(backend = "portable")`: the rows core every host shares, the layout the load lays its q8 rows out in), its
//! context cut to 384, dispatch forced threaded
def private flash_deep_ctx_cell(t : T?; var tr : Model; kv : KVDtype; what, title : string) {
    with_ctx_cut(tr, 384l) {
        unsafe {
            t |> run(title) @capture(& tr)(t : T ?) {
                t |> equal(kernel_backend_pin(), "portable", "the cell runs on its form's portable pin")
                with_threaded_jobque() {
                    flash_deep_ctx_within(t, tr, kv, what)
                }
            }
        }
    }
}

//! Past 256 positions on SmolLM2-135M `tr` (loaded under the portable pin) the fused flash-sliced attention passes an in-chain
//! witness recomputing exact single-pass attention within 1e-4 at every sliced layer, and teacher-forced logits stay within a
//! structural bound of 4 of the per-op path
def fused_flash_deep_ctx_cell(t : T?; var tr : Model) {
    flash_deep_ctx_cell(t, tr, KVDtype.f32, "", "flash-sliced fused chain: per-layer witness + structural logit bound (needs SmolLM2-135M)")
}

//! the q8_0-K/V twin of `fused_flash_deep_ctx_cell`: the sliced K pass runs the int8 x int8 quantized-query dot and the witness
//! recomputes through the SAME dot (kv_score), so the 1e-4 in-chain contract holds under the block codec too
def fused_flash_deep_ctx_q8_cell(t : T?; var tr : Model) {
    flash_deep_ctx_cell(t, tr, KVDtype.q8_0, " q8", "flash-sliced fused chain under q8_0 KV: per-layer witness + structural logit bound (needs SmolLM2-135M)")
}

//! On gpt-oss-4L mxfp4 `tr` (the 20b's first four blocks, the mx4 fixture), its context cut to FUSED_CTX, with an mx4 rows
//! core: the fused MoE expert chain (gate+up rows + bias + act + requant -> down rows + bias + weighted reduce as ONE
//! rendezvous) matches the three-dispatch groupn path's greedy ids and final logits bit for bit
def fused_decode_moe_chain_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, FUSED_CTX) {
        unsafe {
            t |> run("fused MoE expert chain bit-exact on gpt-oss (needs the gguf)") @capture(& tr)(t : T ?) {
                with_threaded_jobque() {
                    if (!kernel_backend_has_mx4_rows()) {
                        t |> success(true, "skipped: active backend has no mx4 rows core")
                    } else {
                        fused_vs_std_bit_exact(t, tr)
                    }
                }
            }
        }
    }
}

//! one model file's fused-decode row: the file its messages name,
//! and whether its layers are native K-quant - the auto backend's
//! kq rows cores the cell asks, where a pre/post-norm row asks the
//! portable pin
struct FusedRow {
    file : string
    kquant : bool
}

//! gemma-3-1b: qk-norm, dual-rope sliding (5:1), the attention
//! chain's deferred residual add
def fused_row_gemma3_1b() : FusedRow => FusedRow(file = "gemma-3-1b-it-Q8_0.gguf")

//! gemma-2-2b: softcap, GeGLU, qd != dim, alternating sliding
def fused_row_gemma2_2b() : FusedRow => FusedRow(file = "gemma-2-2b-it-Q8_0.gguf")

//! gemma-4 E2B: the pre/post-norm chain of the gemma-4 generation
def fused_row_gemma4_e2b() : FusedRow => FusedRow(file = "gemma-4-E2B-it-Q8_0.gguf")

//! Qwen3-4B Q5_K_M: k5 planes, k6 ffn_down
def fused_row_qwen3_4b_q5km() : FusedRow => FusedRow(
    file = "Qwen3-4B-Instruct-2507-Q5_K_M.gguf", kquant = true)

//! Qwen3-4B Q4_K_M: k4 planes, k6 ffn_down
def fused_row_qwen3_4b_q4km() : FusedRow => FusedRow(
    file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", kquant = true)

//! the row's bit-exact compare on `tr`: the portable pin held on a
//! pre/post-norm row; on a K-quant row a load the backend did not
//! repack skips (the fused gate would compare std against std)
def private fused_row_within(t : T?; tr : Model; row : FusedRow) {
    if (!row.kquant) {
        t |> equal(kernel_backend_pin(), "portable", "the cell runs on its form's portable pin")
        fused_vs_std_bit_exact(t, tr)
    } elif (!tr.kq_repacked) {
        t |> success(true, "skipped: active backend has no kq rows cores")
    } else {
        fused_vs_std_bit_exact(t, tr)
    }
}

//! the CPU form a row's cell runs on: a pre/post-norm row on
//! `cpu_cells_form(backend = "portable")` (the chains defer the
//! residual add, the post-norm a serial tail after the rendezvous); a
//! K-quant row on the auto backend's q8 form with the native K-quant
//! planes on (Q8_K activations: the layer-input quantize, the
//! w2-input requant at 256-row stage-0 grain, the wo-input requant
//! per head group)
def fused_row_form(row : FusedRow) : ModelForm {
    return cpu_cells_form(kq_native = KqNative.on) if (row.kquant)
    return cpu_cells_form(backend = "portable")
}

//! On `tr`, on the pass of the row's CPU form alone
//! (`fused_row_form`), its context cut to FUSED_CTX, dispatch forced
//! threaded: the fused decode chains match the per-op path's greedy
//! ids and final logits bit for bit; the fused knob put back as
//! found. Other passes pass by
def fused_decode_row_cell(t : T?; var tr : Model; form : ModelForm; row : FusedRow) {
    return if (!alone_pass_of(form, fused_row_form(row)))
    let title = (row.kquant ? "fused chains bit-exact on native K-quant layers (needs the kq ggufs)"
        : "fused chains bit-exact on pre/post-norm arches (needs the gemma ggufs)")
    with_ctx_cut(tr, FUSED_CTX) {
        unsafe {
            t |> run("{row.file}: {title}") @capture(& tr)(t : T ?) {
                return if (!jit_only(t))
                t |> equal(form.backend, row.kquant ? "" : "portable",
                    "{row.file}: the cell runs on the form its row asks")
                let fused0 = get_fused_decode()
                with_threaded_jobque() {
                    fused_row_within(t, tr, row)
                }
                set_fused_decode(fused0)
            }
        }
    }
}
