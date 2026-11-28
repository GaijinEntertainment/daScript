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
require _format_census
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require _compares   // check, the bars: the per-32 tables' CPU cells
require _kquant_cells   // read_prefix: the per-32 tables' file truth; native_site_rows, kq_dense_sites_cell
require _metal_format_census_cells   // the Metal arm of the census on the carriers' blob twins
require _perop_ffn_cells   // the per-op FFN rail's cells on the iq4nl32 file's per-op forms
require _fused_decode_cells   // fused_per32_cell: the per-32 carriers' decode chains
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
// A carrier's Metal form is one skipped cell on a build without das_metal; a low-bit carrier's
// crowned window skips where the box compiles no Metal-4 tensor twin, and a per-32 carrier's
// fused decode chains' cell where the kernel backend has no rows core for its format.
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
    fused : bool       //!< layer 0's attention planes are `fmt`: the fused chains' cell runs
    fused_ffn : bool   //!< ... and every FFN plane of it too, so the FFN chain takes them
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
            native_site_rows(t, m, path, "classifier", "token_embd.weight", cls_fmt_of(m),
                m.wcls_off, dim, vocab)
            if (spec.hybrid) {
                let eh = "blk.{m.config.n_layers}.nextn.eh_proj.weight"
                native_site_rows(t, m, path, "eh_proj", eh, m.mtp_ehproj_fmt, m.mtp_ehproj_off,
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
        if (spec.fused) {
            fused_per32_cell(t, m, spec.file, spec.fmt, spec.fused_ffn)
        }
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

[test(description = "Qwen2.5-0.5B iq4nl32, q4032 and q5032 carriers and a Q5_1 Qwen3.5-MTP: on the Vulkan home every site read in its own per-32 format, the tied table gathered on the device each window and step, and forced-feed steps within 6 to 8 percent of the CPU chain; with the whole-model route off the per-op dense FFN rail on the iq4nl32 carrier runs every layer's q/o and FFN on the device, every step's FFN row counted, within 8 percent of the CPU chain, with rail-off as control; on the Metal home's blob twin, one load on a das_metal build, every site read in its own per-32 format and the Metal window and six steps within the census bar of the portable CPU chain's replay; on the K-quant native CPU form, on every box, every table site loads native, `embed_row` rows match the file dequant bit for bit (job lanes too), and classifier and eh_proj rows match the file rows' dot within f32 reassociation, and on the three qwen2 carriers, where the backend has the format's rows core, the fused decode chains give the per-op dispatches' greedy ids and logits bit for bit - layer 0's attention chain taken, its FFN chain taken on q5032 and declined on the two carriers whose down plane is a superblock format; each form loads once")]
def test_gpu_resident_per32_tables(t : T?) {
    t |> run(("Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf: the per-32 token table and " +
            "classifier, census, forced feed and the per-op FFN rail")) @(t : T?) {
        return if (!family_on(t, "qwen2"))
        let spec = Per32Spec(file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf",
            vk_arm = "vk-per32-iq4nl32", bar = IQ4NL32_TABLE_BAR_REL,
            census = format_census_iq4nl32_table(), fmt = KqFmt.iq4nl32, perop = true, fused = true)
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
            fmt = KqFmt.q4032, fused = true)
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
            fmt = KqFmt.q5032, fused = true, fused_ffn = true)
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

//! a low-bit carrier's forms: on the Vulkan home, where `arm` reaches
//! it, the gather's one-region form; the K-quant native CPU form on
//! every box
def private lowbit_forms(t : T?; file, arm : string) : array<ModelForm> {
    var forms : array<ModelForm>
    let why = vulkan_home_why("the table gather is the Vulkan home's")
    if (!empty(why)) {
        skip_cell(t, "{file}, the gather's form: {why}")
    } elif (empty(arm_why(arm))) {
        forms |> push(device_form(regions = 1l))
    }
    forms |> push(per32_cpu_form())
    return <- forms
}

//! a low-bit carrier on its K-quant native CPU form: where `tied`, the
//! table sites served off their own bytes (`per32_tables_cpu_cell`);
//! else layer 0's attention q and FFN down rows against the file
//! rows' dot (`kq_dense_sites_cell`)
def private lowbit_cpu_cell(t : T?; var m : Model; file : string; fmt : KqFmt; tied : bool) {
    if (tied) {
        per32_tables_cpu_cell(t, m, Per32Spec(file = file, fmt = fmt))
        return
    }
    let path = path_join(models_dir(), file)
    unsafe {   //! the cell reads the block's model by reference
        t |> run(("{file}: the attention and FFN planes serve their own {fmt} bytes on the " +
                "CPU")) @capture(& m)(t : T ?) {
            kq_dense_sites_cell(t, m, path, fmt)
        }
    }
    fused_per32_cell(t, m, file, fmt, true)
}

//! one form's share of a low-bit carrier's block: on the device the `fmt` matrices (and table, where `tied`), census and gather; on
//! the K-quant native CPU form the sites served off their own bytes (`lowbit_cpu_cell`); on the q8 CPU form the feed's other side
def private lowbit_rows(t : T?; var g : EmbGather; var m : Model; form : ModelForm; file : string; fmt : KqFmt; tied : bool) {
    if (form.on_gpu) {
        t |> equal(fmt_at(m.wq_fmt, 0l), fmt, "{file}: attention q loads as {fmt}")
        t |> equal(fmt_at(m.w2_fmt, 0l), fmt, "{file}: FFN down loads as {fmt}")
        if (tied) {
            t |> equal(m.emb_fmt, fmt, "{file}: the tied token table loads as {fmt}")
        }
        census_cell(t, m, path_join(models_dir(), file), "vulkan load", tied ? ["attention q/k/v/o", "token table", "classifier"] : ["attention q/k/v/o"])
        emb_gather_rows(g, m, form, file)
    } elif (alone_pass_of(form, per32_cpu_form())) {
        lowbit_cpu_cell(t, m, file, fmt, tied)
    } elif (g.ran && empty(g.cpu.rows) && same_cpu_load(form, cpu_quant_form(QuantMode.q8))) {
        g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
    }
}

//! a low-bit carrier's compare after its block: the gather's rows
//! against the CPU chain's at the per-32 tables' bar
def private lowbit_within(t : T?; ran : bool; g : EmbGather; file : string) {
    return if (!ran)
    with_dasllama_jobque_() {
        emb_gather_within(t, g, file, PER32_TABLE_BAR_REL)
    }
}

[test(description = "Ternary-Bonsai-1.7B Q2_0 and the Llama-3.2-1B Q2_0 mint: on the Vulkan home every site read in its own format on the whole-model driver - Bonsai's tied q20 table gathered on the device each window and step - and forced-feed steps within 8 percent of the CPU chain, no call taking a CPU pass; on the Metal home's blob twin (arm `census-q20`) Bonsai's every site is read in its own format, its window and six steps ride the q20 stamps and the per-32 table gather, and both land within 4 percent of the portable CPU chain's rows; on the K-quant native CPU form, on every box, Bonsai's table sites load native, `embed_row` rows match the file dequant bit for bit (job lanes too) and its classifier rows match the file rows' dot within f32 reassociation, and the mint's every attention site loads native, its attention q and FFN down rows matching the file rows' dot within f32 reassociation, and where the backend has the format's rows core the fused decode chains take its planes and give the per-op dispatches' greedy ids and logits bit for bit; each form loads once")]
def test_gpu_resident_q20(t : T?) {
    t |> run("Ternary-Bonsai-1.7B-Q2_0_g64.gguf: the q20 matrices and tied table on the device, census and forced feed; the table sites on the CPU") @(t : T?) {
        return if (!family_on(t, "qwen3"))
        let file = "Ternary-Bonsai-1.7B-Q2_0_g64.gguf"
        var g : EmbGather
        var census : CensusKept
        var forms <- lowbit_forms(t, file, "vk-per32-q20")
        if (empty(arm_why(format_census_q20().arm))) {
            format_census_forms(forms, format_census_q20())
        }
        let ran = with_cpu_and_gpu_model(t, "llm", "Ternary-Bonsai-1.7B-Q2_0_g64.gguf", GATHER_CTX, forms) $(var m, form) {
            if (!format_census_pass(t, m, form, format_census_q20(), census)) {
                lowbit_rows(t, g, m, form, file, KqFmt.q20, true)
            }
        }
        lowbit_within(t, ran, g, file)
        format_census_verdict(t, format_census_q20(), census)
        delete g
        delete forms
    }
    t |> run("Llama-3.2-1B-Instruct-Q2_0-local.gguf: the q20 matrices on the device, census and forced feed; the attention and FFN sites on the CPU") @(t : T?) {
        return if (!family_on(t, "llama"))
        let file = "Llama-3.2-1B-Instruct-Q2_0-local.gguf"
        var g : EmbGather
        var forms <- lowbit_forms(t, file, "vk-per32-q20")
        let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q2_0-local.gguf", GATHER_CTX, forms) $(var m, form) {
            lowbit_rows(t, g, m, form, file, KqFmt.q20, false)
        }
        lowbit_within(t, ran, g, file)
        delete g
        delete forms
    }
}

[test(description = "Bonsai-1.7B Q1_0 and the Llama-3.2-1B Q1_0 mint: on the Vulkan home every site read in its own format on the whole-model driver - Bonsai's tied q10 table gathered on the device each window and step - and forced-feed steps within 8 percent of the CPU chain, no call taking a CPU pass; on the Metal home's blob twin (arm `census-q10`) Bonsai's every site is read in its own format, its window and six steps ride the q10 stamps and the per-32 table gather, and both land within 4 percent of the portable CPU chain's rows; on the K-quant native CPU form, on every box, Bonsai's table sites load native, `embed_row` rows match the file dequant bit for bit (job lanes too) and its classifier rows match the file rows' dot within f32 reassociation, and the mint's every attention site loads native, its attention q and FFN down rows matching the file rows' dot within f32 reassociation, and where the backend has the format's rows core the fused decode chains take its planes and give the per-op dispatches' greedy ids and logits bit for bit; each form loads once")]
def test_gpu_resident_q10(t : T?) {
    t |> run("Bonsai-1.7B-Q1_0.gguf: the q10 matrices and tied table on the device, census and forced feed; the table sites on the CPU") @(t : T?) {
        return if (!family_on(t, "qwen3"))
        let file = "Bonsai-1.7B-Q1_0.gguf"
        var g : EmbGather
        var census : CensusKept
        var forms <- lowbit_forms(t, file, "vk-per32-q10")
        if (empty(arm_why(format_census_q10().arm))) {
            format_census_forms(forms, format_census_q10())
        }
        let ran = with_cpu_and_gpu_model(t, "llm", "Bonsai-1.7B-Q1_0.gguf", GATHER_CTX, forms) $(var m, form) {
            if (!format_census_pass(t, m, form, format_census_q10(), census)) {
                lowbit_rows(t, g, m, form, file, KqFmt.q10, true)
            }
        }
        lowbit_within(t, ran, g, file)
        format_census_verdict(t, format_census_q10(), census)
        delete g
        delete forms
    }
    t |> run("Llama-3.2-1B-Instruct-Q1_0-local.gguf: the q10 matrices on the device, census and forced feed; the attention and FFN sites on the CPU") @(t : T?) {
        return if (!family_on(t, "llama"))
        let file = "Llama-3.2-1B-Instruct-Q1_0-local.gguf"
        var g : EmbGather
        var forms <- lowbit_forms(t, file, "vk-per32-q10")
        let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q1_0-local.gguf", GATHER_CTX, forms) $(var m, form) {
            lowbit_rows(t, g, m, form, file, KqFmt.q10, false)
        }
        lowbit_within(t, ran, g, file)
        delete g
        delete forms
    }
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
    format_census_forms(forms, row)
    return <- forms
}

[test(description = "On the every-plane NVFP4 Qwen3.5-0.8B-MTP mint, on the Metal home's blob twin, one load on a das_metal build, every site - the deltanet planes, the NextN eh_proj, the table and the classifier - read as nv4, the Metal window and six steps within the census bar of the portable CPU chain's replay; the form loads once")]
def test_metal_census_nv4_hybrid(t : T?) {
    return if (!family_on(t, "qwen35"))
    let row = format_census_nv4_hybrid()
    var kept : CensusKept
    var forms <- census_row_forms(t, row)
    with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-MTP-NVFP4-pure-local.gguf", CENSUS_CELLS_CTX,
            forms) $(var m, form) {
        format_census_pass(t, m, form, row, kept)
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
        format_census_pass(t, m, form, row, kept)
    }
    format_census_verdict(t, row, kept)
    delete forms
}
