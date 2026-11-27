options gen2
options _dasllama_internal = true

module _perop_ffn_cells shared public

//! The per-op dense FFN rail's cells per model file, on the per-op
//! forms its block loads with the whole-model route off: the rail-off
//! load the counter's control, the rail-on load's steps against the
//! CPU form's chain; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // create_session, the Model
require dasllama/dasllama_math   // with_dasllama_jobque_, active_kernel_backend
require dasllama/dasllama_gpu_tier   // the dense rail's marks and row counters
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, perop_dense_ffn_decline
require _model_tier
require _model_form   // with_ctx_cut, ModelForm, form_name
require _resident_feed   // resident_prompt, feed_logits, feed_steps_within

let private PROMPT = 40l   //! one window
let private STEPS = 6l
//! the context the cells cut their form's model to: the window and
//! the fed steps
let PEROP_FFN_CTX = 64l

//! one per-op FFN carrier: the file, its family's token, the bar of
//! the step's max |logit|
struct PeropFfnRow {
    file : string
    family : string
    bar : float
}

//! the q8 carrier at the llama resident file's bar
def perop_ffn_llama_1b_q8() : PeropFfnRow => PeropFfnRow(
    file = "Llama-3.2-1B-Instruct-Q8_0.gguf", family = "llama", bar = 0.06)

//! the K-quant carrier (Q8_K feed)
def perop_ffn_llama_1b_q4km() : PeropFfnRow => PeropFfnRow(
    file = "Llama-3.2-1B-Instruct-Q4_K_M.gguf", family = "llama", bar = 0.06)

//! the per-32 (q51) dense FFN
def perop_ffn_llama_1b_q51() : PeropFfnRow => PeropFfnRow(
    file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf", family = "llama", bar = 0.06)

//! a per-32 file whose table and classifier are per-32 too
//! (iq4nl32): its per-op chain - the window's GEMMs on the device's
//! f16 tiles beside the CPU's int8 dots - reads 0.44-0.73 of a
//! 10.7-19.6 max logit (3.7-6.1%), argmax flips only at near-ties,
//! the one-step-off control 7.4 and past
def perop_ffn_qwen25_05b_iq4nl32() : PeropFfnRow => PeropFfnRow(
    file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", family = "qwen2", bar = 0.08)

//! what the rail-on form hands the CPU form: the window's and the
//! fed steps' rows, the codec they ran on
struct PeropFfnKept {
    gpu : array<array<float>>
    kv : KVDtype
    on_ran : bool    //! the rail-on form fed its steps
    cpu_ran : bool   //! the CPU form fed and compared them
}

//! the layers whose FFN triple the load's walk marked on the device
def private ffn_triples_on_device(m : Model) : int64 {
    var n = 0l
    for (l in range64(m.config.n_layers)) {
        continue if (m.w1_offs[l] < 0l)
        let w1 = moe_gpu_dense_on_gpu(int(fmt_at(m.w1_fmt, l)), m.w1_offs[l])
        let w3 = moe_gpu_dense_on_gpu(int(fmt_at(m.w3_fmt, l)), m.w3_offs[l])
        let w2 = moe_gpu_dense_on_gpu(int(fmt_at(m.w2_fmt, l)), m.w2_offs[l])
        n += w1 && w3 && w2 ? 1l : 0l
    }
    return n
}

//! the run arms the row (the Vulkan home, its family, the arm), on
//! a per-op form pinning the dense FFN rail (`dense_ffn`), its
//! K-quant planes repacked; false after the skip or failure that says
//! why not
def private perop_ffn_runs(t : T?; m : Model; form : ModelForm; row : PeropFfnRow) : bool {
    return false if (!vulkan_home_asked(t, "the per-op rails' cells are the Vulkan home's")
                     || !family_on(t, row.family) || !arm_on(t, "vk-perop-ffn"))
    if (!form.perop || form.dense_ffn < 0) {
        t |> failure(("{row.file}: the per-op FFN cells run on a per-op form that pins the " +
            "dense FFN rail, not {form_name(form)}"))
        return false
    }
    //! the rail reads the CPU backend's repacked K-quant planes; a
    //! backend without the repack (portable, the reference policy on
    //! x86) leaves the triple to the CPU
    if (!m.kq_repacked) {
        t |> skip(("the per-op FFN rail asks repacked K-quant planes: the " +
            "'{active_kernel_backend()}' backend repacks none"))
        return false
    }
    return true
}

//! The row's cell on a per-op form, by the dense FFN rail the form
//! pins for its load (`form.dense_ffn`): off, the counter's control
//! (`perop_ffn_off_cell`); on, the rail's placement and rows, kept
//! for `perop_ffn_cpu` (`perop_ffn_on_cell`). The model's marks
//! assert the load took the pin
def perop_ffn_device_cell(t : T?; var m : Model; form : ModelForm; row : PeropFfnRow;
                          var kept : PeropFfnKept) {
    if (form.dense_ffn == 0) {
        perop_ffn_off_cell(t, m, form, row)
    } else {
        perop_ffn_on_cell(t, m, form, row, kept)
    }
}

//! On the row's per-op form loaded with the dense FFN rail pinned off
//! (`dense_ffn = 0`, read at load), its context cut to
//! PEROP_FFN_CTX: no layer's FFN triple on the device, and the
//! window and fed steps run no FFN row there (the counter's control)
def private perop_ffn_off_cell(t : T?; var m : Model; form : ModelForm; row : PeropFfnRow) {
    with_ctx_cut(m, PEROP_FFN_CTX) {
        unsafe {
            let title = ("{row.file} with the whole-model route off and the per-op FFN rail " +
                "off: no FFN row on the device")
            t |> run(title) @capture(& m)(t : T ?) {
                return if (!perop_ffn_runs(t, m, form, row))
                t |> equal(ffn_triples_on_device(m), 0l, ("the form loaded with the per-op FFN " +
                    "rail off: no layer's FFN triple on the device"))
                with_dasllama_jobque_() {
                    var prompt <- resident_prompt(PROMPT)
                    var toks <- resident_feed(STEPS)
                    let off0 = moe_gpu_dense_ffn_rows()
                    var so <- create_session(m, form.kv)
                    var ctl <- feed_logits(m, so, prompt, toks)
                    t |> equal(moe_gpu_dense_ffn_rows() - off0, 0l, ("with the rail off no FFN " +
                        "row runs on the device (the counter's control)"))
                    delete so
                    delete ctl
                    delete prompt
                    delete toks
                }
            }
        }
    }
}

//! the rail-on form's placement asserts on `m`: every layer's FFN
//! triple in the rail's formats, the q/o pairs and the classifier
//! the tier's
def private perop_ffn_placed(t : T?; m : Model) {
    t |> success(!moe_gpu_resident_active(),
        "the whole-model route is off: the per-op rails serve")
    t |> success(ffn_triples_on_device(m) > 0l,
        "the form loaded with the per-op FFN rail on: FFN triples on the device")
    var qo_marked = 0l
    for (l in range64(m.config.n_layers)) {
        t |> equal(perop_dense_ffn_decline(m, l), "",
            "layer {l}'s FFN triple is the per-op rail's formats")
        let on_q = moe_gpu_dense_on_gpu(int(fmt_at(m.wq_fmt, l)), m.wq_offs[l])
        qo_marked += on_q && moe_gpu_dense_on_gpu(int(fmt_at(m.wo_fmt, l)), m.wo_offs[l]) ? 1l : 0l
    }
    t |> equal(qo_marked, m.config.n_layers, ("the dense rail holds every layer's q/o pair " +
        "in the file's format ({qo_marked} of {m.config.n_layers})"))
    let cls_fmt = m.config.shared_weights ? m.emb_fmt : m.wcls_fmt
    t |> success(moe_gpu_cls_on_gpu(m.wcls_off),
        "the classifier plane (format {int(cls_fmt)}) is the tier's")
}

//! On the row's per-op form loaded with the dense FFN rail pinned on
//! (`dense_ffn = 1`, read at load), its context cut to
//! PEROP_FFN_CTX: every layer's q/o and FFN on the device, the
//! window's GEMMs and every fed step's FFN row counted; the rows kept
//! for `perop_ffn_cpu`
def private perop_ffn_on_cell(t : T?; var m : Model; form : ModelForm; row : PeropFfnRow;
                              var kept : PeropFfnKept) {
    with_ctx_cut(m, PEROP_FFN_CTX) {
        unsafe {
            let title = ("{row.file} with the whole-model route off: the per-op FFN rail runs " +
                "every layer's FFN row on the device, the steps against the CPU chain")
            t |> run(title) @capture(& m, & kept)(t : T ?) {
                return if (!perop_ffn_runs(t, m, form, row))
                perop_ffn_placed(t, m)
                with_dasllama_jobque_() {
                    var prompt <- resident_prompt(PROMPT)
                    var toks <- resident_feed(STEPS)
                    let rows0 = moe_gpu_dense_ffn_rows()
                    let gemm0 = moe_gpu_dense_gemm_rows()
                    kept.kv = form.kv   //! no mirror arms on a per-op load: the form names the codec
                    var sg <- create_session(m, kept.kv)
                    delete kept.gpu
                    kept.gpu <- feed_logits(m, sg, prompt, toks)
                    let rows = moe_gpu_dense_ffn_rows() - rows0
                    let gemm = moe_gpu_dense_gemm_rows() - gemm0
                    let owed = 5l * m.config.n_layers * PROMPT
                    t |> success(gemm >= owed, ("the window's q/o and FFN GEMMs ran on the " +
                        "device over every layer's planes ({gemm} rows, {owed} owed)"))
                    t |> equal(rows, STEPS * m.config.n_layers,
                        "every fed step ran each layer's FFN row on the device ({rows} rows)")
                    delete sg
                    delete prompt
                    delete toks
                }
                kept.on_ran = true
            }
        }
    }
}

//! the CPU load the rail's CPU side runs on: the q8 CPU chain the
//! block's device forms answer to
def perop_ffn_cpu_form() : ModelForm => cpu_quant_form(QuantMode.q8)

//! On the first pass of `perop_ffn_cpu_form()`'s load (no device
//! state), where the rail-on form ran: the same window and fed steps
//! on the CPU chain, no dense GEMM row on the device (the GEMM
//! counter's control), the rail-on rows within the row's bar of the
//! CPU chain's; other passes pass by
def perop_ffn_cpu(t : T?; var m : Model; form : ModelForm; row : PeropFfnRow;
                  var kept : PeropFfnKept) {
    return if (!kept.on_ran || kept.cpu_ran || form.on_gpu
               || !same_cpu_load(form, perop_ffn_cpu_form()))
    kept.cpu_ran = true
    with_ctx_cut(m, PEROP_FFN_CTX) {
        unsafe {
            let title = "{row.file}: the per-op FFN rail's steps against the CPU chain"
            t |> run(title) @capture(& m, & kept)(t : T ?) {
                with_dasllama_jobque_() {
                    var prompt <- resident_prompt(PROMPT)
                    var toks <- resident_feed(STEPS)
                    let cpu_gemm0 = moe_gpu_dense_gemm_rows()
                    var cpu <- feed_alone(m, kept.kv, prompt, toks)
                    t |> equal(moe_gpu_dense_gemm_rows() - cpu_gemm0, 0l, ("the CPU chain with " +
                        "the model's device state dropped runs no dense GEMM row on the device " +
                        "(the GEMM counter's control)"))
                    feed_steps_within(t, m, "{row.file} per-op FFN ", PROMPT, kept.gpu, cpu, toks,
                        row.bar, "per-op")
                    delete cpu
                    delete prompt
                    delete toks
                }
            }
        }
    }
    delete kept.gpu
}

//! after the block: a rail-on form whose CPU side never ran fails
//! naming the file; the rows freed
def perop_ffn_verdict(t : T?; row : PeropFfnRow; var kept : PeropFfnKept) {
    if (kept.on_ran && !kept.cpu_ran) {
        t |> failure(("{row.file}: the per-op FFN rail's steps lost their CPU side (no " +
            "`perop_ffn_cpu_form` pass)"))
    }
    delete kept
}
