options gen2
options stack = 524288   // the test file is the program root under dastest; the gpt-oss forward (mx4 experts, sinks, biases) overflows the default budget
options persistent_heap   // + explicit deletes below: each arm frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require dasllama/dasllama_transformer
require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_math   // pin_kernel_backend / kernel_backend_pin_refusal / active_kernel_backend — the facade wraps none of them
require dasllama/dasllama_env    // g_env_engine.pin_backend
require daslib/defer   // the backend pin is restored on every path out of a pinned cell
require daslib/jobque_boost
require daslib/fio
require strings   // find: the refusals' names
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require _resident_feed
require _cpu_gpu_model   // with_cpu_and_gpu_model, cpu_cells_form: the untied stories15M block
require ../performance/model_specs.das   // the model-set table — parity evidence lives there as DATA
require ../harness/_parity_dump.das
require _parity_cells   // parity_prompt, embedding_rows

// Frozen parity gates (token for token; a `prefix_bar` carrier as a half-span prefix) for the arches stories15M (test_forward.das) doesn't cover.
// Evidence is DATA in model_specs.das — pinned ids (or a text prompt encoded here) + the arms it was
//   frozen under; regenerate with harness/parity.sh. The board's --parity pregate reads the same rows.
// Models are gitignored multi-GB files: absent = clean skip (a real local regression, a no-op in CI).
// Every model cell skips off the JIT, without its file, past the
// size tier (`model_available`), or where DASLLAMA_PIN_BACKEND names
// a backend this box cannot pin. The layer cuts' parity streams (`_parity_cells.das`)
// run on the cuts' blocks, and so do the model_specs rows
// `PARITY_BLOCK_ROWS` names, each on its file's block.

def private with_parity_backend(t : T?; blk : block<() : void>) {
    let name = g_env_engine.pin_backend
    if (empty(name)) {
        invoke(blk)
        return
    }
    let why = kernel_backend_pin_refusal(name)
    if (!empty(why)) {
        t |> skip("DASLLAMA_PIN_BACKEND='{name}' cannot be pinned on this box: {why}")
        return
    }
    let seat = kernel_backend_seat()
    defer() {
        restore_kernel_backend(seat)
    }
    pin_kernel_backend(name)
    invoke(blk)
}

// The tied half (cls_q8, SmolLM2-135M) runs on the SmolLM2 block of test_gpu_resident_llama.das (`tied_cls_q8_rows_cell`).
// emb_q8 invariant, the untied half: a Q8 load of a Q8_0 embedding with a SEPARATE output.weight drops the
// fp32 table too (rows dequant from the linear transcode at emb_q8_off, bit-identical to the fp32 decode);
// without it an untied model carries vocab x dim floats, 1.5 GB on a 152k-vocab 4B. stories15M is the smallest untied Q8_0 file.
//! what one stories15M form collects for the untied compare: the load's flags, its `embedding_rows` (the fp32 table's on the
//! fp32 form, the emb_q8 plane's dequant on the q8 form) and, where the load took the emb_q8 plane, `embed_row`'s
//! row of token 1000
struct private UntiedRows {
    ran : bool
    emb_q8 : bool
    table_kept : bool   //! the fp32 table stayed in fblob
    tied : bool
    cls_q8 : bool
    emb_apart : bool   //! the embedding copy is not the classifier plane
    rows : array<float>
    row1000 : array<float>
}

//! the untied compare's collection on the loaded form `m`
def private untied_rows(m : Model) : UntiedRows {
    var r = UntiedRows(ran = true, emb_q8 = m.emb_q8, table_kept = m.tok_emb_off >= 0l, tied = m.config.shared_weights,
        cls_q8 = m.cls_q8, emb_apart = m.emb_q8_off != m.wcls_off, rows <- embedding_rows(m, m.emb_q8))
    let dim = m.config.dim
    if (m.emb_q8) {
        r.row1000 |> resize(dim)
        embed_row(m, 1000l, unsafe(addr(r.row1000[0])))
    }
    return <- r
}

//! the untied fp32 form's collection `f32` against the q8 form's `q8`
def private untied_emb_q8_within(t : T?; f32, q8 : UntiedRows) {
    t |> success(!f32.emb_q8 && f32.table_kept, "fp32 load keeps the fblob table")
    t |> success(!q8.tied, "the fixture is untied (output.weight on disk)")
    t |> success(!q8.cls_q8, "untied: the classifier flag stays off")
    t |> success(q8.emb_q8, "untied Q8_0 load takes the emb_q8 path")
    t |> success(!q8.table_kept, "no fp32 table in fblob")
    t |> success(q8.emb_apart, "the embedding copy is not the classifier plane")
    let bad = mismatch_exact(q8.rows, f32.rows)
    t |> success(bad == 0, "embedding rows bit-identical ({bad} mismatches)")
    let dim = long_length(q8.row1000)
    var inscope want <- [for (i in range64(dim)); i + 2l * dim < long_length(f32.rows) ? f32.rows[i + 2l * dim] : 0.0]
    let bad_row = mismatch_exact(q8.row1000, want) + (dim == 0l ? 1 : 0)
    t |> success(bad_row == 0, "embed_row reads the same values ({bad_row} mismatches)")   //! the row reader the forward pass uses agrees with the raw plane arithmetic
}

[test(description = "An untied Q8 load of stories15M drops the fp32 embedding table for the emb_q8 plane, whose rows and `embed_row` output are bit-identical to the fp32 load's table; each form loads once")]
def test_parity_untied_emb_q8_rows(t : T?) {
    return if (!family_on(t, "llama"))
    var f32, q8 : UntiedRows
    with_cpu_and_gpu_model(t, "llm", "stories15M-Q8_0.gguf", 2048l, [cpu_cells_form(), cpu_cells_form(QuantMode.fp32)]) $(m, form) {
        if (form.quant == QuantMode.fp32) {
            delete f32
            f32 <- untied_rows(m)
        } else {
            delete q8
            q8 <- untied_rows(m)
        }
    }
    if (f32.ran || q8.ran) {
        unsafe {   //! the cell reads the block's collection by reference
            t |> run("untied Q8 load: emb_q8 embedding rows bit-match the fp32 table, no fp32 table kept") @capture(& f32, & q8)(t : T ?) {
                t |> success(f32.ran && q8.ran, "stories15M: both forms collected (fp32 {f32.ran}, q8 {q8.ran})")
                return if (!f32.ran || !q8.ran)
                untied_emb_q8_within(t, f32, q8)
            }
        }
    }
    delete f32
    delete q8
}

//! `path` loaded as the parity row `spec` loads its file: under the
//! row's K-quant native knob, put back after the load, in its load
//! mode, the K/V context cut to 2048 - only about prompt plus
//! generation positions are used, the K/V stride changes no computed
//! token, and Llama-3's native 131072 would burn ~8GB of K/V RAM
def private parity_row_load(path : string; spec : ModelSpec) : Model {
    let prev_kq = get_kquant_native()
    set_kquant_native(spec.parity.kq_native)
    var tr <- load_model_(path, spec.parity.quant)
    set_kquant_native(prev_kq)
    tr.config.seq_len = min(tr.config.seq_len, PARITY_ROW_CTX)
    return <- tr
}

//! the spec rows a test of this file loads for itself: gpt-oss, both
//! fixtures on one load - its ~13 GB native-mx4 resident is the
//! suite's biggest, and two loads run a 64 GB box out of memory
let private PARITY_OWN_ROWS = fixed_array("gpt-oss-20b-mxfp4.gguf")

//! one walked row's arms (`parity_row_arms`) off ONE model load of its file at `path`
def private parity_spec_arms(t : T?; spec : ModelSpec; path : string) {
    var tr <- parity_row_load(path, spec)
    parity_row_arms(t, spec, tr)
    delete tr
}

[test(description = "Every model_specs row with a pinned parity fixture whose file has no block of its own (`PARITY_BLOCK_ROWS` names the rows their files' blocks run) and no test of this file loading it (`PARITY_OWN_ROWS`) generates its frozen greedy ids token for token (or to the half-span prefix bar, with a flipped-id control), including its chunked, reference and per-expert prefill arms")]
def test_parity_specs(t : T?) {
    var files : array<string>
    var all <- model_specs()
    for (s in all) {
        if (!empty(s.parity.gen) && !has_value(PARITY_OWN_ROWS, s.file) && !has_value(PARITY_BLOCK_ROWS, s.file)) {
            files |> push(s.file)
        }
    }
    delete all
    for (file in files) {
        t |> run("pinned-greedy parity: {file}") @(t : T?) {
            return if (!jit_only(t))
            var inscope spec <- spec_by_file(file)
            let path = path_join(models_dir(), spec.file)
            if (!model_available(t, path)) return
            with_parity_backend(t) {
                parity_spec_arms(t, spec, path)
            }
        }
    }
    delete files
}

// The second gpt-oss fixture, window-engaged: "1, 2, …, 150," (449 tokens ≫ the 128-token window,
// so sliding layers genuinely clip — kstart > 0) ENCODED IN-TEST through the gpt-4o pre-tokenizer
// (verified id-for-id against the oracle's tokenization), gating the tokenizer AND the forward,
// same pattern as the gemma4 fixture. Test-local on purpose: the table carries the model's short
// counting fixture (the board pregate reads that one); this is the suite's extra window arm.
let GPTOSS_LONG_GEN <- [220l, 17395l, 11l, 220l, 17336l, 11l, 220l, 17773l, 11l, 220l,
                        19145l, 11l, 220l, 17385l, 11l, 220l, 18369l, 11l, 220l, 19676l,
                        11l, 220l, 19548l, 11l, 220l, 19218l, 11l, 220l, 9444l, 11l,
                        220l, 18881l, 11l, 220l, 18694l, 11l, 220l, 17666l, 11l, 220l]


// gpt-oss-20b (arch "gpt-oss", native-MXFP4 expert planes — attn/cls Q8) — the wave-5 stack:
// per-head attention sinks, YaRN rope (factor 32 / 4096), alternating 128-token SWA,
// softmax-after-top-k routing with router + per-expert + QKV + output-projection biases,
// clamped SwiGLU. Both fixtures share ONE model load.
[test(description = "gpt-oss-20b on one Q8 load reproduces its frozen greedy ids token for token on the short spec fixture and on a 449-token counting prompt that engages the 128-token sliding window")]
def test_parity_gptoss_20b(t : T?) {
    t |> run("gpt-oss forward (gpt-oss-20b native-MXFP4 experts, sinks + YaRN + MoE biases; short + window-engaged)") @(t : T?) {
        return if (!jit_only(t))
        var inscope spec <- spec_by_file("gpt-oss-20b-mxfp4.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        with_parity_backend(t) {
            var tr <- load_model_(path, QuantMode.q8)
            if (tr.config.seq_len > 2048l) {
                tr.config.seq_len = 2048l
            }
            let be = " [backend {active_kernel_backend()}]"
            t |> success(parity_gen_ok(tr, spec.parity.prompt, spec.parity.gen, [eyeball = true]), "gpt-oss-20b token-for-token{be}")
            let long_prompt <- encode(tr, counting_prompt(150), true, false)
            t |> success(parity_gen_ok(tr, long_prompt, GPTOSS_LONG_GEN, [eyeball = true]), "gpt-oss-20b window-engaged token-for-token{be}")
            delete tr
        }
    }
}

// The parity dump's header refusals, model-free; the round trip on
// Qwen3.5-0.8B-MTP - the device arm's forced-feed run to a file, the
// CPU arm's replayed off it, the two compared - rides that file's
// block (`test_gpu_resident_hybrid.das`).

//! a file at `name` under the test's temp dir holding the two header words `magic` and `version` and nothing after
def private header_file(name : string; magic, version : int64) : string {
    let path = path_join(test_temp_dir(), name)
    fopen(path, "wb") $(f) {
        fwrite(f, magic)
        fwrite(f, version)
    }
    return path
}

[test(description = "`parity_dump_refusal` names a foreign magic word, a different version word and a missing file as refusals, and accepts a header carrying this build's magic and version")]
def test_parity_dump_refuses_foreign_files(t0 : T?) {
    t0 |> run("a file with another magic word is no parity dump, one with another version word is another version's, and this build's header words are read: each refused by name, model-free") @(t : T?) {
        let foreign = header_file("foreign.pd", 1313821514l, PARITY_DUMP_VERSION)   // "JSON" as a little-endian word
        let old = header_file("old.pd", PARITY_DUMP_MAGIC, PARITY_DUMP_VERSION + 1l)
        let ours = header_file("ours.pd", PARITY_DUMP_MAGIC, PARITY_DUMP_VERSION)
        let why_foreign = parity_dump_refusal(foreign)
        let why_old = parity_dump_refusal(old)
        t |> success(find(why_foreign, "is not a parity dump") >= 0, "another magic word is refused by name ({why_foreign})")
        t |> success(find(why_old, "is another version's") >= 0, "another version word is refused by name ({why_old})")
        t |> equal(parity_dump_refusal(ours), "", "this build's header words are read")
        t |> success(find(parity_dump_refusal(path_join(test_temp_dir(), "missing.pd")), "cannot read") >= 0, "a missing file is refused by name")
    }
}
