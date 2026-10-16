options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require dasllama/dasllama_kqformat   // KqFmt - the PLE table's tag
require dasllama/dasllama_gpu_resident   // resident_plan - the table's seat
require _format_census   // census_cell - the per-layer planes in their own formats
require daslib/fio
require math   // nolint:STYLE030,LINT019 - abs, read in the vulkan half
require strings
require dasllama/dasllama_ple   // ple_cpu_pre_rows - the CPU pre-step's witness
require dasllama/dasllama_common   // Model, embed_row, forward, forward_prefill
require dasllama/dasllama   // create_session
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_ple_side_row, vk_rdec_token_gathered - the device pre-step's witnesses

// gemma-4-E2B Q8_0 (the E-series carrier: per-layer embeddings, the upper layers sharing the K/V of the ones below, two
// dense FFN widths, a sliding window the two-window cell engages): arm `g4e`. Skips off the JIT, without DASLLAMA_GPU=1
// on a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4E_FILE = GemmaFile(file = "gemma-4-E2B-it-Q8_0.gguf", arm = "g4e")

[test]
def test_gpu_resident_gemma4_e2b(t : T?) {
    t |> run("gemma-4-E2B Q8_0: resident prefill + decode - the per-layer-embedding branch, the shared-KV layers, two FFN widths - vs the CPU chain, forty tokens") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 520 ids prefilled - two windows, the sliding window engaged on the last eight - then 80 teacher-forced") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA4E_FILE)
    }
}

[test]
def test_gpu_resident_gemma4_e2b_block_codecs(t : T?) {
    t |> run("gemma-4-E2B Q8_0: a q8_0 K/V mirror - the shared-KV layers store no row of their own - vs the CPU chain, forty tokens") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            gemma_forced_feed(t, 40l, GEMMA4E_FILE)
        }
    }
    t |> run("gemma-4-E2B Q8_0: a tq4 K/V mirror - a shared-KV layer rotates its query and stores nothing - scored as a perplexity against the CPU chain") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
        }
    }
}

//! the per-layer-embedding table in its own tag on the device (arm `g4e-ple`): E2B Q8_0 with the token and PLE tables
//! requantized to Q5_0, Q5_1 and Q4_0 (`../performance/model_specs.das`) and the Q8_0 file as the shape control - the plan
//! seats the table under its tag, and the forced feed's prefill gathers the side rows off it against the CPU chain
[test]
def test_gpu_resident_gemma4_e2b_ple_tables(t : T?) {
    for (spec in [(file = "gemma-4-E2B-it-Q8_0.gguf", fmt = KqFmt.q8),
                  (file = "gemma-4-E2B-it-emb-Q5_0-local.gguf", fmt = KqFmt.q5032),
                  (file = "gemma-4-E2B-it-emb-Q5_1-local.gguf", fmt = KqFmt.q51),
                  (file = "gemma-4-E2B-it-emb-Q4_0-local.gguf", fmt = KqFmt.q40)]) {
        let gf = GemmaFile(file = spec.file, arm = "g4e-ple")
        t |> run("{spec.file}: the {spec.fmt} per-layer-embedding table seats on the device and the prefill gathers off it") @(t : T?) {
            with_gemma_rig(t, gf) $(tr) {
                t |> equal(tr.ple_emb_fmt, spec.fmt, "the PLE table keeps its disk format's tag")
                let plan = resident_plan(tr, 4096l, KVDtype.f16, KVDtype.f16)
                t |> success(plan.ple_tbl_bytes > 0l, "the plan seats the {spec.fmt} table on the device ({plan.ple_tbl_bytes} bytes)")
            }
            gemma_forced_feed(t, 40l, gf)
        }
    }
}

//! the per-layer gate and projection planes in their own formats on the device (arm `g4e-kq`): the E2B Q4_K_M file ships them
//! F32 - the router GEMV, the f32 act and the f32 tile read the file's rows, the feed held against the file's f32 chain - and the Q4_0 file Q4_0, on the q40 GEMV and tiles
//! with the f32 classes undispatched (the control)
[test]
def test_gpu_resident_gemma4_e2b_ple_planes(t : T?) {
    for (spec in [(file = "gemma-4-E2B-it-Q4_K_M.gguf", f32 = true), (file = "gemma-4-E2B-it-Q4_0.gguf", f32 = false)]) {
        let gf = GemmaFile(file = spec.file, arm = "g4e-kq")
        t |> run("{spec.file}: the per-layer gate and projection planes in their file format, the forced feed against the CPU chain") @(t : T?) {
            var armed = false   //! the rig armed the driver on the file (else every cell below skipped, saying why)
            with_gemma_rig(t, gf) $(tr) {
                armed = true
                census_cell(t, tr, path_join(models_dir(), spec.file), "vulkan load", ["per-layer embeddings"])
                t |> equal(tr.ple_gate_f32 && tr.ple_proj_f32, spec.f32, "the planes ship F32 exactly where the file carries F32")
            }
            let act0 = census("ple_act_f32_cls_spv")
            let gemm0 = census("dn_ba_cls_spv")
            if (spec.f32) {   //! the Q4_K_M file's CPU q8 chain sits off its f32 chain at one step: `gemma_forced_feed_exact`
                gemma_forced_feed_exact(t, 40l, gf)
            } else {
                gemma_forced_feed(t, 40l, gf)
            }
            return if (!armed || act0 == NO_CENSUS)
            let act_n = census("ple_act_f32_cls_spv") - act0
            let gemm_n = census("dn_ba_cls_spv") - gemm0
            if (spec.f32) {
                t |> success(act_n > 0l && gemm_n > 0l, "the token command's f32 act ({act_n}) and the window's f32 tile ({gemm_n}) served the f32 rows")
            } else {
                t |> equal(act_n + gemm_n, 0l, "no f32 per-layer plane, no f32 act or tile dispatched")
            }
        }
    }
}

//! the decode step's pre-step on the device (arm `g4e-pre`): the side row the one-row command gathered off the step's token id,
//! projected under the CPU's 1/sqrt(dim) and finished, read back against the CPU pre-step's row of the same token at 1e-4 of its
//! largest - the projection without the 1/sqrt(dim) reads the eps alone apart, 1e-3 - with another token's CPU row the control
[test]
def test_gpu_resident_gemma4_e2b_ple_pre_step(t : T?) {
    t |> run("gemma-4-E2B Q8_0: the step's side row built on the device is the CPU pre-step's row") @(t : T?) {
        with_gemma_rig(t, GemmaFile(file = "gemma-4-E2B-it-Q8_0.gguf", arm = "g4e-pre")) $(tr) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                var prompt <- resident_prompt(40l)
                var sg <- create_session(tr, mirror_kv_dtype())
                forward_prefill(tr, sg, prompt, 40l, 0l)
                let tok = 1234l
                let g0 = vk_rdec_token_gathered()
                let cpu0 = ple_cpu_pre_rows()
                forward(tr, sg, tok, 40l)
                t |> success(vk_rdec_token_gathered() > g0, "the step's embed row gathered off its token id")
                t |> equal(ple_cpu_pre_rows() - cpu0, 0l, "the step built no CPU pre-step row")
                var dev : array<float>
                vk_rdec_ple_side_row(dev)
                var inscope cpu <- cpu_side_row(tr, tok)
                var inscope other <- cpu_side_row(tr, tok + 1l)
                var mx = 0.0
                var off = 0.0
                var big = 0.0
                for (i in range64(long_length(cpu))) {
                    mx = max(mx, abs(dev[i] - cpu[i]))
                    off = max(off, abs(dev[i] - other[i]))
                    big = max(big, abs(cpu[i]))
                }
                let bar = 1e-4 * big
                to_log(LOG_INFO, "e2b pre-step: device side row against the CPU's: maxdiff {mx}, another token's {off} (bar {bar}, largest {big})\n")
                t |> success(mx <= bar, "the device side row within 1e-4 of the CPU's largest ({mx} of {bar})")
                t |> success(off > bar, "another token's CPU row lands outside the bar ({off})")
                delete dev
                delete sg
                delete prompt
            }
        }
    }
}

//! the greedy chain on the E-series (arm `g4e-chain`): a chained step's side row gathers off the pick the device lands in the id
//! plane, so the chained walk is bit for bit the plain walk (`greedy_chain_cell`)
[test]
def test_gpu_resident_gemma4_e2b_greedy_chain(t : T?) {
    t |> run("gemma-4-E2B Q8_0: the greedy chain over the side rows, bit for bit the plain walk") @(t : T?) {
        return if (!arm_on(t, "g4e-chain"))
        greedy_chain_cell(t, path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf"))
    }
}

//! the CPU pre-step's side row of `tok` on a scratch session: the scaled embedding row, then `ple_pre_decode`
def private cpu_side_row(tr : Model; tok : int64) : array<float> {
    var s <- make_run_state(tr.config)
    embed_row(tr, tok, unsafe(addr(s.x[0])))
    if (tr.config.embed_scale != 1.0) {
        for (i in range64(tr.config.dim)) {
            s.x[i] *= tr.config.embed_scale
        }
    }
    ple_pre_decode(tr, s, tok)
    let n = tr.config.n_layers * tr.config.n_embd_per_layer
    var row <- [for (i in range64(n)); s.ple_inp[i]]
    delete s
    return <- row
}

//! the tied F16 token table (arm `g4e-f16`, the `-local` mint of `../performance/model_specs.das`): no device plane carries the
//! classifier, so the whole-model driver declines the model by name and the per-op rails take it - never a q8 plane seated where no
//! bytes sit; the PLE table loads as a q8 transcode (the stated converted copy)
[test]
def test_gpu_resident_gemma4_e2b_tied_f16(t : T?) {
    t |> run("gemma-4-E2B Q8_0 with an F16 token table: the whole-model driver declines the tied classifier by name") @(t : T?) {
        return if (!arm_on(t, "g4e-f16"))
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0-embf16-local.gguf")
        return if (!resident_ready(t, path))
        var tr <- load_resident(path)
        t |> success(tr.config.shared_weights && !tr.cls_q8 && !cls_kq(tr), "the tied table carries no q8 or K-quant plane")
        t |> success(find(resident_decline_reason(tr), "tied classifier") >= 0, "the decline names the tied classifier: {resident_decline_reason(tr)}")
        t |> success(!moe_gpu_resident_active(), "the whole-model driver stays off the model")
        t |> equal(tr.ple_emb_fmt, KqFmt.q8, "the F16 PLE table reads its q8 transcode")
        delete tr
    }
}

//! a short tail on the E-series peels through the token commands, each row's side input finished on the device: after a 40-row
//! opening, tails of 1, 2 and 5 rows (one peel counted each) and four fed steps within the split bar of the same tail stepped a row at
//! a time on a second session, the row one step off the control
[test]
def test_gpu_resident_gemma4_e2b_short_window(t : T?) {
    t |> run("gemma-4-E2B Q8_0: short tails peel through the token commands, the side rows riding them") @(t : T?) {
        with_gemma_rig(t, GEMMA4E_FILE) $(tr) {
            var opening <- resident_prompt(40l)
            var feed <- resident_feed(5l)
            let lever = vulkan_prefill_peel()
            set_vulkan_prefill_peel(true)
            for (rows in [1l, 2l, 5l]) {
                var tail <- [for (i in range64(rows)); feed[i]]
                var steps <- ramp_ids(4l, rows, 2500l, 71l, 4500l)
                var sa <- create_session(tr, mirror_kv_dtype())
                eval(tr, sa, opening)
                let peels0 = moe_gpu_resident_peels()
                forward_prefill(tr, sa, tail, rows, 40l)
                t |> equal(moe_gpu_resident_peels() - peels0, 1l, "a tail of {rows} rows peels through the token commands")
                sa.n_past = 40l + rows
                var got <- steps_after(tr, sa, steps)
                var sb <- create_session(tr, mirror_kv_dtype())
                eval(tr, sb, opening)
                var inscope one : array<int64>
                one |> resize(1)
                for (i in range64(rows)) {
                    one[0] = tail[i]
                    eval_(tr, sb, one)
                }
                var want <- steps_after(tr, sb, steps)
                rows_within_control(t, "a tail of {rows} rows", got, want, SPLIT_BAR_REL, 1l, 1.0, $(r) => r == 0l ? "the tail's last row" : "step {r}")
                served_every_call(t)
                delete sa
                delete sb
                delete got
                delete want
                delete tail
                delete steps
            }
            set_vulkan_prefill_peel(lever)
            delete opening
            delete feed
        }
    }
}

//! the sliding layers' rings against the flat mirror (`ring_vs_flat`): 1300 prose ids past the 512-position window - three windows of
//! the chain - and eight steps, bit for bit, on the f16 mirror and both block codecs (their f16 shadow reads each position's slot)
[test]
def test_gpu_resident_gemma4_e2b_ring(t : T?) {
    for (kv in fixed_array(KVDtype.f16, KVDtype.q8_0, KVDtype.tq4)) {
        t |> run("gemma-4-E2B Q8_0: the sliding layers' rings against every row kept, a {kv} mirror, 1300 ids and eight steps") @(t : T?) {
            ring_vs_flat(t, 1300l, GEMMA4E_FILE, kv)
        }
    }
}
