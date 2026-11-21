options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // load_model_, create_session, moe_gpu_drop_model
require daslib/fio   // path_join
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // set_gpu_resident_route, set_gpu_want_dense_ffn: the per-op rail's levers
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, perop_dense_ffn_decline
require _format_census
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_gathered: the token command's rows gathered off their ids, where the module exists

// The Vulkan arm of the weight-format census on the dense carriers: the per-op FFN rail's q8, K-quant and per-32 files
// with the whole-model route off, the per-32 tables' mints and the tied tables' gather, each site's tag against the file's
// own tensor type (`_format_census.das`), the device's rows against the CPU chain. The 35B IQ4_XS census, its verify-rows,
// NextN and per-op cells and the F32 beta/alpha cell ride the qwen35moe cuts' blocks (`test_gpu_resident_qwen_moe.das`).
// DASLLAMA_GPU=1, -jit; skips without the file or the armed tier.
// Under DASLLAMA_CUDA=1 the per-op FFN rail's cells and the per-32
// and tied tables' gather cells skip: the per-op tier and the gather
// witness are the Vulkan home's. The tied K-quant table's gather rides a smaller
// file (Llama-3.2-3B-Instruct Q4_K_M; Qwen3-4B-Instruct-2507 Q4_K_M's is its block in test_gpu_resident_qwen3.das),
// the window's and the steps' rows off the classifier's plane against the CPU chain at the split bar.

let private TIED_LLAMA = "Llama-3.2-3B-Instruct-Q4_K_M.gguf"
let private PROMPT = 40l   //! one window
let private STEPS = 6l
let private GATHER_CTX = 4096l   //! a gather block's context: the cells' prompts are one window

//! the per-op FFN rail's carriers: a q8, a K-quant (Q8_K feed) and a per-32 (q51) dense FFN at the llama resident file's bar (of the
//! step's max |logit|), and a per-32 file whose table and classifier are per-32 too (iq4nl32) at 0.08: its per-op chain - the window's
//! GEMMs on the device's f16 tiles beside the CPU's int8 dots - reads 0.44-0.73 of a 10.7-19.6 max logit (3.7-6.1%), argmax flips only
//! at near-ties, the one-step-off control 7.4 and past
let private PEROP_FFN = fixed_array((file = "Llama-3.2-1B-Instruct-Q8_0.gguf", family = "llama", bar = 0.06),
    (file = "Llama-3.2-1B-Instruct-Q4_K_M.gguf", family = "llama", bar = 0.06),
    (file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf", family = "llama", bar = 0.06),
    (file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", family = "qwen2", bar = 0.08))

//! `path` loaded with the whole-model route off and the per-op FFN rail pinned to `ffn` (0 or 1), both levers put back
def private load_perop(path : string; ffn : int) : Model {
    let route = gpu_resident_route_lever()
    let pin = gpu_dense_ffn_pin()
    set_gpu_resident_route(false)
    set_gpu_want_dense_ffn(ffn)
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_want_dense_ffn(pin)
    set_gpu_resident_route(route)
    return <- tr
}

[test(description = "With the whole-model route off, the per-op dense FFN rail on q8, K-quant, Q5_1 and iq4nl32 carriers runs every layer's q/o and FFN on the device, every step's FFN row counted, within 6 or 8 percent of the CPU chain, with rail-off as control")]
def test_gpu_perop_dense_ffn(t : T?) {
    return if (!vulkan_home_asked(t, "the per-op rails' cells are the Vulkan home's"))
    for (spec in PEROP_FFN) {
        let file = spec.file
        t |> run("{file} with the whole-model route off: the per-op FFN rail runs every layer's FFN row on the device, the steps against the CPU chain") @(t : T?) {
            if (!family_on(t, spec.family) || !arm_on(t, "vk-perop-ffn")) return
            let path = path_join(models_dir(), file)
            if (!resident_ready(t, path)) return
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(STEPS)
                var off <- load_perop(path, 0)
                if (!off.kq_repacked) {   //! the rail reads the CPU backend's repacked K-quant planes; a backend without the repack (portable, the reference policy on x86) leaves the triple to the CPU
                    t |> skip("the per-op FFN rail asks repacked K-quant planes: the '{active_kernel_backend()}' backend repacks none")
                    delete off
                    delete prompt
                    delete toks
                    return
                }
                let off0 = moe_gpu_dense_ffn_rows()
                var so <- create_session(off, mirror_kv_dtype())
                var ctl <- feed_logits(off, so, prompt, toks)
                t |> equal(moe_gpu_dense_ffn_rows() - off0, 0l, "with the rail off no FFN row runs on the device (the counter's control)")
                delete so
                delete ctl
                delete off
                var tr <- load_perop(path, 1)
                t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the per-op rails serve")
                var qo_marked = 0l
                for (l in range64(tr.config.n_layers)) {
                    t |> equal(perop_dense_ffn_decline(tr, l), "", "layer {l}'s FFN triple is the per-op rail's formats")
                    let on_q = moe_gpu_dense_on_gpu(int(fmt_at(tr.wq_fmt, l)), tr.wq_offs[l])
                    qo_marked += on_q && moe_gpu_dense_on_gpu(int(fmt_at(tr.wo_fmt, l)), tr.wo_offs[l]) ? 1l : 0l
                }
                t |> equal(qo_marked, tr.config.n_layers, "the dense rail holds every layer's q/o pair in the file's format ({qo_marked} of {tr.config.n_layers})")
                let cls_fmt = tr.config.shared_weights ? tr.emb_fmt : tr.wcls_fmt
                t |> success(moe_gpu_cls_on_gpu(tr.wcls_off), "the classifier plane (format {int(cls_fmt)}) is the tier's")
                let rows0 = moe_gpu_dense_ffn_rows()
                let gemm0 = moe_gpu_dense_gemm_rows()
                var sg <- create_session(tr, mirror_kv_dtype())
                var gpu <- feed_logits(tr, sg, prompt, toks)
                let rows = moe_gpu_dense_ffn_rows() - rows0
                let gemm = moe_gpu_dense_gemm_rows() - gemm0
                t |> success(gemm >= 5l * tr.config.n_layers * PROMPT,
                    "the window's q/o and FFN GEMMs ran on the device over every layer's planes ({gemm} rows, {5l * tr.config.n_layers * PROMPT} owed)")
                delete sg
                moe_gpu_drop_model()
                let cpu_gemm0 = moe_gpu_dense_gemm_rows()
                var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
                t |> equal(moe_gpu_dense_gemm_rows() - cpu_gemm0, 0l, "the CPU chain with the model's device state dropped runs no dense GEMM row on the device (the GEMM counter's control)")
                feed_steps_within(t, tr, "{file} per-op FFN ", PROMPT, gpu, cpu, toks, spec.bar, "per-op")
                t |> equal(rows, STEPS * tr.config.n_layers, "every fed step ran each layer's FFN row on the device ({rows} rows)")
                delete gpu
                delete cpu
                delete tr
                delete prompt
                delete toks
            }
        }
    }
}

//! `file` on the whole-model driver: the window's token rows and every fed step's row gathered on the device off the table's
//! plane (the tied classifier's where the table is tied), the forced feed against the CPU chain at the split bar
def private emb_gather_cell(t : T?; file : string; bar_rel : float) {
    let path = path_join(models_dir(), file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident_host(path, 1l)
    t |> success(moe_gpu_resident_active(), "{file}: the whole-model driver armed (the load log names any decline)")
    if (moe_gpu_resident_active()) {
        with_dasllama_jobque_() {
            var g : EmbGather
            var form = device_form(regions = 1l, host = true)
            form.kv = mirror_kv_dtype()
            emb_gather_rows(g, tr, form, file)
            moe_gpu_drop_model()
            g.cpu <- resident_feed_rows(tr, form, GATHER_PROMPT, GATHER_STEPS)
            emb_gather_within(t, g, file, bar_rel)
            delete g
        }
    }
    delete tr
}

//! the per-32 table carriers' bar: the 0.5B qwen2 file's forced-feed reading (test_gpu_resident_qwen2.das' LOGIT_BAR_REL) on
//! the f16 feed; on the sdot4 feed (a wave64 device) the q4032 steps read 0.061 of an 11.9 max logit on the Radeon 780M, the
//! one-step-off control 7.3+, the iq4nl32 class
let private PER32_TABLE_BAR_REL = 0.08
//! the qwen2 file's IQ4NL32_BAR_REL: the window's f16 FFN feed puts the RTX PRO 4500's steps at 0.060 of the max logit, one-step-off 7.4+
let private IQ4NL32_TABLE_BAR_REL = 0.08

//! one form's share of a per-32 carrier's block on the whole-model driver: on the device form every site the file carries read
//! in its own format (the census) and the tied per-32 table gathered on the device by the window and every step, on the CPU
//! form the forced feed's other side
def private per32_table_rows(t : T?; var g : EmbGather; var m : Model; form : ModelForm; file : string) {
    if (form.on_gpu) {
        t |> success(kq_block32(cls_fmt_of(m)) && emb_kq(m), "{file}: the tied table loads in its per-32 form ({m.emb_fmt})")
        census_cell(t, m, path_join(models_dir(), file), "vulkan load", ["attention q/k/v/o", "token table", "classifier"])
        emb_gather_rows(g, m, form, file)
    } elif (g.ran) {
        g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
    }
}

//! a per-32 block's compares after it
def private per32_table_within(t : T?; ran : bool; g : EmbGather; file : string; bar_rel : float) {
    return if (!ran)
    with_dasllama_jobque_() {
        emb_gather_within(t, g, file, bar_rel)
    }
}

[test(description = "Qwen2.5-0.5B iq4nl32, q4032 and q5032 carriers and a Q5_1 Qwen3.5-MTP read every site in its own per-32 format, gather the tied table on the device each window and step, and hold forced-feed steps within 6 to 8 percent of the CPU chain; each form loads once")]
def test_gpu_resident_per32_tables(t : T?) {
    return if (!vulkan_home_asked(t, "the table gather is the Vulkan home's"))
    t |> run("Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf: the per-32 token table and classifier on the device, census and forced feed") @(t : T?) {
        if (!family_on(t, "qwen2") || !arm_on(t, "vk-per32-iq4nl32")) return
        let file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf"
        var g : EmbGather
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", GATHER_CTX, [device_form(regions = 1l)]) $(m, form) {
            per32_table_rows(t, g, m, form, file)
        }
        per32_table_within(t, ran, g, file, IQ4NL32_TABLE_BAR_REL)
        delete g
    }
    t |> run("Qwen2.5-0.5B-Instruct-Q4_0-local.gguf: the per-32 token table and classifier on the device, census and forced feed") @(t : T?) {
        if (!family_on(t, "qwen2") || !arm_on(t, "vk-per32-q4032")) return
        let file = "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf"
        var g : EmbGather
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf", GATHER_CTX, [device_form(regions = 1l)]) $(m, form) {
            per32_table_rows(t, g, m, form, file)
        }
        per32_table_within(t, ran, g, file, PER32_TABLE_BAR_REL)
        delete g
    }
    t |> run("Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf: the per-32 token table and classifier on the device, census and forced feed") @(t : T?) {
        if (!family_on(t, "qwen2") || !arm_on(t, "vk-per32-q5032")) return
        let file = "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf"
        var g : EmbGather
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf", GATHER_CTX, [device_form(regions = 1l)]) $(m, form) {
            per32_table_rows(t, g, m, form, file)
        }
        per32_table_within(t, ran, g, file, PER32_TABLE_BAR_REL)
        delete g
    }
    t |> run("Qwen3.5-0.8B-MTP-Q5_1-local.gguf: the q51 table, classifier, deltanet planes and eh_proj on the device") @(t : T?) {
        if (!family_on(t, "qwen35") || !arm_on(t, "vk-per32-q51")) return
        let file = "Qwen3.5-0.8B-MTP-Q5_1-local.gguf"
        var g : EmbGather
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-MTP-Q5_1-local.gguf", GATHER_CTX, [device_form(regions = 1l)]) $(m, form) {
            if (form.on_gpu) {
                census_cell(t, m, path_join(models_dir(), file), "vulkan load", ["deltanet qkv/gate/out", "deltanet beta/alpha", "token table", "classifier", "NextN eh_proj"])
                t |> success(rdec_verify_rows() >= 2l, "the NextN head - its q51 eh_proj among its planes - rides the driver (the load log names any decline)")
                emb_gather_rows(g, m, form, file)
            } else {
                g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
            }
        }
        if (ran) {
            with_dasllama_jobque_() {
                emb_gather_within(t, g, file, 0.06)
            }
        }
        delete g
    }
}

[test(description = "On the whole-model driver Llama-3.2-3B Q4_K_M's tied Q6_K table is gathered on the device by the window and every fed step off its token id, with forced-feed steps within the 6 percent split bar of the CPU chain")]
def test_gpu_resident_gather_tied_llama(t : T?) {
    t |> run("{TIED_LLAMA}: the tied Q6_K table gathered on the device by the window and by every decode step") @(t : T?) {
        if (!family_on(t, "llama") || !arm_on(t, "vk-gather") || !vulkan_home_asked(t, "the table gather is the Vulkan home's")) return
        emb_gather_cell(t, TIED_LLAMA, SPLIT_BAR_REL)
    }
}
