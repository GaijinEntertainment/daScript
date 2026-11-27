options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the facade: Model
require daslib/fio   // path_join
require daslib/jobque_boost   // parallel_for: the per-32 table's rows on job lanes
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: the gather's driver
require dasllama/dasllama_convert   // quantize_q8_0_into: the per-32 sites' activation row
require _format_census
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require _compares   // check, the bars: the per-32 tables' CPU cells
require _kquant_cells   // read_prefix: the per-32 tables' file truth
require _metal_format_census_cells   // the Metal arm of the census on the carriers' blob twins
require _perop_ffn_cells   // the per-op FFN rail's cells on the iq4nl32 file's per-op forms
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_gathered: the token command's rows gathered off their ids, where the module exists

// The weight-format census on the dense carriers: the per-op FFN rail's iq4nl32 file with the
// whole-model route off (its llama files' rows ride their blocks in
// test_gpu_resident_llama.das), the per-32 tables' mints and the tied tables' gather on the
// Vulkan home, each site's tag against the file's own tensor type
// (`_format_census.das`), the device's rows against the CPU chain; on the Metal home each
// carrier's blob twin read in its own format, its window and steps against the portable CPU
// chain's replay (`_metal_format_census_cells.das`); on the per-32 tables' K-quant native CPU
// forms, on every box, the table sites served off their own bytes. The 35B IQ4_XS census, its
// verify-rows, NextN and per-op cells and the F32 beta/alpha cell ride the qwen35moe cuts'
// blocks (`test_gpu_resident_qwen_moe.das`). DASLLAMA_GPU=1, -jit; skips without the file or
// the armed tier. Under DASLLAMA_CUDA=1 the per-op FFN rail's cells and the per-32 and tied
// tables' gather cells skip: the per-op tier and the gather witness are the Vulkan home's.
// The tied K-quant table's gather rides a smaller file (Llama-3.2-3B-Instruct Q4_K_M;
// Qwen3-4B-Instruct-2507 Q4_K_M's is its block in test_gpu_resident_qwen3.das), the window's
// and the steps' rows off the classifier's plane against the CPU chain at the split bar.

let private GATHER_CTX = 4096l   //! a gather block's context: the cells' prompts are one window

//! the per-32 table carriers' bar: the 0.5B qwen2 file's forced-feed reading (test_gpu_resident_qwen2.das' LOGIT_BAR_REL) on
//! the f16 feed; on the sdot4 feed (a wave64 device) the q4032 steps read 0.061 of an 11.9 max logit on the Radeon 780M, the
//! one-step-off control 7.3+, the iq4nl32 class
let private PER32_TABLE_BAR_REL = 0.08
//! the qwen2 file's IQ4NL32_BAR_REL: the window's f16 FFN feed puts the RTX PRO 4500's steps at 0.060 of the max logit, one-step-off 7.4+
let private IQ4NL32_TABLE_BAR_REL = 0.08
let private Q51_HYBRID_BAR_REL = 0.06   //! the hybrid's q51 table: the deltanet bar

//! one per-32 table carrier: its file, the Vulkan gather's arm and bar,
//! the census row its Metal forms read, its table's per-32 format, and
//! whether its NextN eh_proj and deltanet planes ride the table's
//! format (the hybrid), whether its per-op FFN rows ride its block
struct Per32Spec {
    file : string
    vk_arm : string
    bar : float
    census : CensusRow
    fmt : KqFmt
    hybrid : bool
    perop : bool
}

//! what a per-32 carrier's forms collect: the gather's rows, the
//! census's Metal side, the per-op FFN rail's device rows
struct Per32Kept {
    g : EmbGather
    census : CensusKept
    perop : PeropFfnKept
}

//! a per-32 table carrier's K-quant native CPU form: the native K-quant
//! and Q4_0 planes on, every table site native
def private per32_cpu_form() : ModelForm => cpu_cells_form(kq_native = KqNative.on_q40)

//! the Vulkan home's forms of a per-32 carrier, where its arm reaches
//! them: the gather's one-region form, and the per-op FFN rail's
//! rail-off and rail-on forms where the carrier rides them
def private per32_vulkan_forms(t : T?; var forms : array<ModelForm>; spec : Per32Spec) {
    let why = vulkan_home_why("the table gather and the per-op rails are the Vulkan home's")
    if (!empty(why)) {
        skip_cell(t, "{spec.file}, the Vulkan home's forms: {why}")
        return
    }
    if (empty(arm_why(spec.vk_arm))) {
        forms |> push(device_form(regions = 1l))
    }
    if (spec.perop && empty(arm_why("vk-perop-ffn"))) {
        forms |> push(device_form(perop = true, dense_ffn = 0))
        forms |> push(device_form(perop = true, dense_ffn = 1))
    }
}

//! a per-32 carrier's forms: the Vulkan home's, the census's blob twin
//! and on a das_metal build its portable CPU form where the census arm
//! reaches them, and the K-quant native CPU form on every box
def private per32_forms(t : T?; spec : Per32Spec) : array<ModelForm> {
    var forms : array<ModelForm>
    per32_vulkan_forms(t, forms, spec)
    if (empty(arm_why(spec.census.arm))) {
        forms |> push(metal_form(ctx = CENSUS_CELLS_CTX))
        if (empty(metal_build_why())) {
            forms |> push(census_cpu_form(spec.census))
        }
    }
    forms |> push(per32_cpu_form())
    return <- forms
}

//! a per-32 carrier's device form: every site the file carries read in
//! its own format (the census) and the tied per-32 table gathered on
//! the device by the window and every step; on the hybrid the deltanet
//! planes and the NextN head's eh_proj among the sites, the head on the
//! driver
def private per32_device_rows(t : T?; var g : EmbGather; var m : Model; form : ModelForm;
                              spec : Per32Spec) {
    let path = path_join(models_dir(), spec.file)
    if (spec.hybrid) {
        census_cell(t, m, path, "vulkan load", ["deltanet qkv/gate/out", "deltanet beta/alpha",
            "token table", "classifier", "NextN eh_proj"])
        t |> success(rdec_verify_rows() >= 2l, ("the NextN head - its q51 eh_proj among its " +
            "planes - rides the driver (the load log names any decline)"))
    } else {
        t |> success(kq_block32(cls_fmt_of(m)) && emb_kq(m),
            "{spec.file}: the tied table loads in its per-32 form ({m.emb_fmt})")
        census_cell(t, m, path, "vulkan load", ["attention q/k/v/o", "token table", "classifier"])
    }
    emb_gather_rows(g, m, form, spec.file)
}

//! a per-32 table's rows embedded on job lanes, the interleave read on
//! the calling thread (`embed_b32_mr`), bit for bit the rows the
//! calling thread embeds; where the table sits in a grp<mr>
//! interleave, rows read at disk order miss (the control)
def private per32_lane_embed(t : T?; tr : Model) {
    let dim = tr.config.dim
    let n = 64
    let mr = embed_b32_mr(tr)
    var want, got, flat : array<float>
    want |> resize(int64(n) * dim)
    got |> resize(int64(n) * dim)
    flat |> resize(int64(n) * dim)
    unsafe {
        for (p in range(n)) {
            embed_row(tr, int64(p * 37 + 5), addr(want[int64(p) * dim]))
            embed_row(tr, int64(p * 37 + 5), addr(flat[int64(p) * dim]), 1l)
        }
    }
    let tp = unsafe(addr(tr))
    var gp = unsafe(addr(got[0]))
    with_dasllama_jobque_() {
        parallel_for(0, n, 8) $(rb, re, wg) {
            for (p in range(rb, re)) {
                embed_row(*tp, int64(p * 37 + 5), unsafe(gp + int64(p) * dim), mr)
            }
            notify_and_release(wg)
        }
    }
    check(t, "{n} rows embedded on job lanes against the calling thread's (grp<{mr}>)", got,
        want, BAR_EXACT, FLAT_SHAPE)
    if (mr > 1l) {
        t |> success(check_count(flat, want, BAR_EXACT) > 0,
            "the rows read at disk order miss the grp<{mr}> table (the control)")
    }
    delete want
    delete got
    delete flat
}

//! a per-32 site's rows through `mm_f` (one Q8_0 activation row)
//! against the double dot of the file's dequantized rows with the
//! activation's dequant, at sampled rows; the bar is f32
//! reassociation, the control a row's neighbour
def private per32_site_rows(t : T?; tr : Model; path, site, name : string; fmt : KqFmt;
                            woff, n, d : int64) {
    t |> success(kq_block32(fmt), "{site}: the plane serves its per-32 form ({fmt})")
    var x, y, xs : array<float>
    var xq : array<int8>
    var xbs : array<int>
    x |> resize(n)
    for (i in range64(n)) {
        x[i] = float((i * 37l) % 101l - 50l) * 0.013
    }
    xq |> resize(n)
    xs |> resize(n / 32l)
    xbs |> resize(n / 16l)
    y |> resize(d)
    mm_f(y, tr, fmt, woff, x, xq, xs, xbs, n, d)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    for (r in [0l, 7l, d / 2l + 3l, d - 1l]) {
        var gt = 0
        var fdq <- read_prefix(path, name, n, gt, r * n)
        var ref = 0.0lf
        var mag = 0.0lf
        for (k in range64(n)) {
            let p = double(fdq[k]) * double(float(xq[k]) * xs[k / 32l])
            ref += p
            mag += abs(p)
        }
        let bar = 1.0e-5lf * mag + 1.0e-6lf
        t |> success(abs(double(y[r]) - ref) <= bar,
            "{site} row {r}: {y[r]} within {bar} of the file rows' dot {ref}")
        if (r + 1l < d) {
            t |> success(abs(double(y[r + 1l]) - ref) > bar || abs(ref) < bar,
                "{site} row {r}: the next row's output lands outside the bar (the control)")
        }
        delete fdq
    }
    delete x
    delete y
    delete xs
    delete xq
    delete xbs
}

//! the per-32 carrier on its K-quant native CPU form: every table site
//! native (the census), the token table's rows through `embed_row` bit
//! for bit the file's own dequant, the classifier rows - and the NextN
//! eh_proj's on the hybrid - through the format router against the
//! file rows' dot with the Q8_0 activation's dequant
def private per32_tables_cpu_cell(t : T?; var m : Model; spec : Per32Spec) {
    let path = path_join(models_dir(), spec.file)
    unsafe {   //! the cell reads the block's model by reference
        let title = ("{spec.file}: the per-32 token table, classifier and eh_proj serve their " +
            "own bytes on the CPU")
        t |> run(title) @capture(& m)(t : T ?) {
            var want <- ["token table", "classifier"]
            if (spec.hybrid) {
                want |> push("NextN eh_proj")
            }
            census_cell(t, m, path, "cpu load", want)
            t |> equal(m.emb_fmt, spec.fmt, "the token table loads in its per-32 form")
            let dim = m.config.dim
            let vocab = m.config.vocab_size
            var row : array<float>
            row |> resize(dim)
            for (tok in [0l, 1l, 4097l, vocab - 1l]) {
                embed_row(m, tok, unsafe(addr(row[0])))
                var gt = 0
                var fdq <- read_prefix(path, "token_embd.weight", dim, gt, tok * dim)
                check(t, "token {tok}: embed_row against the file dequant", row, fdq, BAR_EXACT,
                    FLAT_SHAPE)
                delete fdq
            }
            per32_lane_embed(t, m)
            per32_site_rows(t, m, path, "classifier", "token_embd.weight", cls_fmt_of(m),
                m.wcls_off, dim, vocab)
            if (spec.hybrid) {
                let eh = "blk.{m.config.n_layers}.nextn.eh_proj.weight"
                per32_site_rows(t, m, path, "eh_proj", eh, m.mtp_ehproj_fmt, m.mtp_ehproj_off,
                    2l * dim, dim)
            }
            delete row
            delete want
        }
    }
}

//! one form's share of a per-32 carrier's block: the census on the blob
//! twin and its portable CPU form, the table cells on the K-quant
//! native form, the per-op FFN rail on its per-op forms, the gather on
//! the device form, and on the q8 CPU form answering the device forms
//! the gather's and the rail's CPU sides
def private per32_rows(t : T?; var k : Per32Kept; var m : Model; form : ModelForm;
                       spec : Per32Spec) {
    if (form.on_metal) {
        format_census_metal(t, m, form, spec.census, k.census)
    } elif (alone_pass_of(form, census_cpu_form(spec.census))) {
        format_census_cpu(t, m, form, spec.census, k.census)
    } elif (alone_pass_of(form, per32_cpu_form())) {
        per32_tables_cpu_cell(t, m, spec)
    } elif (form.perop) {
        perop_ffn_device_cell(t, m, form, perop_ffn_qwen25_05b_iq4nl32(), k.perop)
    } elif (form.on_gpu) {
        per32_device_rows(t, k.g, m, form, spec)
    } elif (same_cpu_load(form, cpu_quant_form(QuantMode.q8))) {
        if (k.g.ran && empty(k.g.cpu.rows)) {
            k.g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
        }
        if (spec.perop) {
            perop_ffn_cpu(t, m, form, perop_ffn_qwen25_05b_iq4nl32(), k.perop)
        }
    }
}

//! a per-32 carrier's compares after its block: the gather where its
//! device form ran, the census's window and steps, the per-op rail's
def private per32_within(t : T?; ran : bool; var k : Per32Kept; spec : Per32Spec) {
    if (ran && k.g.ran) {
        with_dasllama_jobque_() {
            emb_gather_within(t, k.g, spec.file, spec.bar)
        }
    }
    format_census_verdict(t, spec.census, k.census)
    if (spec.perop) {
        perop_ffn_verdict(t, perop_ffn_qwen25_05b_iq4nl32(), k.perop)
    }
    delete k
}

[test(description = "Qwen2.5-0.5B iq4nl32, q4032 and q5032 carriers and a Q5_1 Qwen3.5-MTP: on the Vulkan home every site read in its own per-32 format, the tied table gathered on the device each window and step, and forced-feed steps within 6 to 8 percent of the CPU chain; with the whole-model route off the per-op dense FFN rail on the iq4nl32 carrier runs every layer's q/o and FFN on the device, every step's FFN row counted, within 8 percent of the CPU chain, with rail-off as control; on the Metal home's blob twin, one load on a das_metal build, every site read in its own per-32 format and the Metal window and six steps within the census bar of the portable CPU chain's replay; on the K-quant native CPU form, on every box, every table site loads native, `embed_row` rows match the file dequant bit for bit (job lanes too), and classifier and eh_proj rows match the file rows' dot within f32 reassociation; each form loads once")]
def test_gpu_resident_per32_tables(t : T?) {
    t |> run(("Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf: the per-32 token table and " +
            "classifier, census, forced feed and the per-op FFN rail")) @(t : T?) {
        return if (!family_on(t, "qwen2"))
        let spec = Per32Spec(file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf",
            vk_arm = "vk-per32-iq4nl32", bar = IQ4NL32_TABLE_BAR_REL,
            census = format_census_iq4nl32_table(), fmt = KqFmt.iq4nl32, perop = true)
        var k : Per32Kept
        var forms <- per32_forms(t, spec)
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf",
                GATHER_CTX, forms) $(var m, form) {
            per32_rows(t, k, m, form, spec)
        }
        per32_within(t, ran, k, spec)
        delete forms
    }
    t |> run(("Qwen2.5-0.5B-Instruct-Q4_0-local.gguf: the per-32 token table and classifier, " +
            "census and forced feed")) @(t : T?) {
        return if (!family_on(t, "qwen2"))
        let spec = Per32Spec(file = "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf",
            vk_arm = "vk-per32-q4032", bar = PER32_TABLE_BAR_REL, census = format_census_q4032(),
            fmt = KqFmt.q4032)
        var k : Per32Kept
        var forms <- per32_forms(t, spec)
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf",
                GATHER_CTX, forms) $(var m, form) {
            per32_rows(t, k, m, form, spec)
        }
        per32_within(t, ran, k, spec)
        delete forms
    }
    t |> run(("Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf: the per-32 token table and " +
            "classifier, census and forced feed")) @(t : T?) {
        return if (!family_on(t, "qwen2"))
        let spec = Per32Spec(file = "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf",
            vk_arm = "vk-per32-q5032", bar = PER32_TABLE_BAR_REL, census = format_census_q5032(),
            fmt = KqFmt.q5032)
        var k : Per32Kept
        var forms <- per32_forms(t, spec)
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf",
                GATHER_CTX, forms) $(var m, form) {
            per32_rows(t, k, m, form, spec)
        }
        per32_within(t, ran, k, spec)
        delete forms
    }
    t |> run(("Qwen3.5-0.8B-MTP-Q5_1-local.gguf: the q51 table, classifier, deltanet planes " +
            "and eh_proj")) @(t : T?) {
        return if (!family_on(t, "qwen35"))
        let spec = Per32Spec(file = "Qwen3.5-0.8B-MTP-Q5_1-local.gguf", vk_arm = "vk-per32-q51",
            bar = Q51_HYBRID_BAR_REL, census = format_census_q51_table(), fmt = KqFmt.q51,
            hybrid = true)
        var k : Per32Kept
        var forms <- per32_forms(t, spec)
        let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-MTP-Q5_1-local.gguf", GATHER_CTX,
                forms) $(var m, form) {
            per32_rows(t, k, m, form, spec)
        }
        per32_within(t, ran, k, spec)
        delete forms
    }
}

//! what the tied Q6_K carrier's forms collect: the gather's rows, the
//! census's Metal side
struct TiedKept {
    g : EmbGather
    census : CensusKept
}

let private TIED_LLAMA = "Llama-3.2-3B-Instruct-Q4_K_M.gguf"

//! the tied Q6_K carrier's forms: on the Vulkan home, where its arm
//! reaches it, the gather's one-region form with the planes on the
//! host; the census's blob twin and on a das_metal build its portable
//! CPU form where the census arm reaches them
def private tied_forms(t : T?) : array<ModelForm> {
    var forms : array<ModelForm>
    let why = vulkan_home_why("the table gather is the Vulkan home's")
    if (!empty(why)) {
        skip_cell(t, "{TIED_LLAMA}, the gather's form: {why}")
    } elif (empty(arm_why("vk-gather"))) {
        forms |> push(device_form(regions = 1l, host = true))
    }
    if (empty(arm_why(format_census_tied_kq().arm))) {
        forms |> push(metal_form(ctx = CENSUS_CELLS_CTX))
        if (empty(metal_build_why())) {
            forms |> push(census_cpu_form(format_census_tied_kq()))
        }
    }
    return <- forms
}

[test(description = "On Llama-3.2-3B Q4_K_M: on the whole-model driver its tied Q6_K table is gathered on the device by the window and every fed step off its token id, with forced-feed steps within the 6 percent split bar of the CPU chain; on the Metal home's blob twin, one load on a das_metal build, every site read in its own format with the gather on the tied Q6_K table, the Metal window and six steps within the census bar of the portable CPU chain's replay; each form loads once")]
def test_gpu_resident_gather_tied_llama(t : T?) {
    return if (!family_on(t, "llama"))
    let row = format_census_tied_kq()
    var k : TiedKept
    var forms <- tied_forms(t)
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-3B-Instruct-Q4_K_M.gguf", GATHER_CTX,
            forms) $(var m, form) {
        if (form.on_metal) {
            format_census_metal(t, m, form, row, k.census)
        } elif (alone_pass_of(form, census_cpu_form(row))) {
            format_census_cpu(t, m, form, row, k.census)
        } elif (form.on_gpu) {
            t |> success(moe_gpu_resident_active(),
                "{TIED_LLAMA}: the whole-model driver armed (the load log names any decline)")
            emb_gather_rows(k.g, m, form, TIED_LLAMA)
        } elif (k.g.ran && empty(k.g.cpu.rows)) {
            k.g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
        }
    }
    if (ran && k.g.ran) {
        with_dasllama_jobque_() {
            emb_gather_within(t, k.g, TIED_LLAMA, SPLIT_BAR_REL)
        }
    }
    format_census_verdict(t, row, k.census)
    delete k
    delete forms
}

//! a census row's own block's forms: its blob twin and, on a
//! das_metal build, its CPU form, where the arm filter reaches it
def private census_row_forms(t : T?; row : CensusRow) : array<ModelForm> {
    var forms : array<ModelForm>
    if (!empty(arm_why(row.arm))) {
        skip_cell(t, "{row.file}: the census arm is filtered out")
        return <- forms
    }
    forms |> push(metal_form(ctx = CENSUS_CELLS_CTX))
    if (empty(metal_build_why())) {
        forms |> push(census_cpu_form(row))
    }
    return <- forms
}

//! a census row's own block's pass: the row's Metal cells on the
//! twin, its CPU side on its CPU form
def private census_row_pass(t : T?; var m : Model; form : ModelForm; row : CensusRow;
                            var kept : CensusKept) {
    if (form.on_metal) {
        format_census_metal(t, m, form, row, kept)
    } else {
        format_census_cpu(t, m, form, row, kept)
    }
}

[test(description = "On the every-plane NVFP4 Qwen3.5-0.8B-MTP mint, on the Metal home's blob twin, one load on a das_metal build, every site - the deltanet planes, the NextN eh_proj, the table and the classifier - read as nv4, the Metal window and six steps within the census bar of the portable CPU chain's replay; the form loads once")]
def test_metal_census_nv4_hybrid(t : T?) {
    return if (!family_on(t, "qwen35"))
    let row = format_census_nv4_hybrid()
    var kept : CensusKept
    var forms <- census_row_forms(t, row)
    with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-MTP-NVFP4-pure-local.gguf", CENSUS_CELLS_CTX,
            forms) $(var m, form) {
        census_row_pass(t, m, form, row, kept)
    }
    format_census_verdict(t, row, kept)
    delete forms
}

[test(description = "On the tied all-F16 canary decoder, on the Metal home's blob twin, one load on a das_metal build, the load keeps a q8 classifier copy, the census lists the table and the classifier as converted copies, and the Metal window and six steps hold the portable CPU chain's replay within 1.0 on fp32 rows; the form loads once")]
def test_metal_census_tied_f16(t : T?) {
    return if (!family_on(t, "qwen3"))
    let row = format_census_tied_f16()
    var kept : CensusKept
    var forms <- census_row_forms(t, row)
    with_cpu_and_gpu_model(t, "llm", "canary-qwen-2.5b-decoder-f16.gguf", CENSUS_CELLS_CTX,
            forms) $(var m, form) {
        census_row_pass(t, m, form, row, kept)
    }
    format_census_verdict(t, row, kept)
    delete forms
}
