options gen2
options stack = 524288   // the test file is the program root under dastest; the gpt-oss forward (mx4 experts, sinks, biases) overflows the default budget
options persistent_heap   // + explicit deletes below: each arm frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require dasllama/dasllama_transformer
require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_moe
require daslib/f16_cvt   // f16_to_f32 — the tied-cls arm reads the live scale plane
require dasllama/dasllama_math   // pin_kernel_backend / kernel_backend_pin_refusal / active_kernel_backend — the facade wraps none of them
require dasllama/dasllama_env    // g_env_engine.pin_backend
require daslib/defer   // the backend pin is restored on every path out of a pinned cell
require daslib/jobque_boost
require daslib/fio
require strings   // find: the refusals' names
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require _resident_feed
require ../performance/model_specs.das   // the model-set table — parity evidence lives there as DATA
require ../harness/_parity_dump.das

// Frozen parity gates (token for token; a `prefix_bar` carrier as a half-span prefix) for the arches stories15M (test_forward.das) doesn't cover.
// Evidence is DATA in model_specs.das — pinned ids (or a text prompt encoded here) + the arms it was
//   frozen under; regenerate with harness/parity.sh. The board's --parity pregate reads the same rows.
// Models are gitignored multi-GB files: absent = clean skip (a real local regression, a no-op in CI).

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

// cls_q8 invariant: a tied Q8 load of a Q8_0 embedding drops the fp32 table (the classifier
// matmuls the disk quants; rows dequant on demand from the linear transcode at emb_q8_off) — and
// those rows must be bit-identical to the fp32 table a fp32 load decodes from the same disk data.
// SmolLM2-135M is the smallest tied model.
[test(description = "A tied Q8 load of SmolLM2-135M takes the cls_q8 path, and its dequantized embedding rows for four tokens are bit-identical to the fp32 table an fp32 load decodes")]
def test_parity_tied_cls_q8_rows(t : T?) {
    t |> run("tied Q8 load: cls_q8 embedding rows bit-match the fp32 table") @(t : T?) {
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var t32 <- load_gguf(path, QuantMode.fp32)
        var t8 <- load_gguf(path, QuantMode.q8)
        t |> success(!t32.cls_q8, "fp32 load keeps the fblob table")
        t |> success(t8.cls_q8, "tied Q8_0 load takes the cls_q8 path")
        let dim = t8.config.dim
        var bad = 0l
        for (token in [0l, 1l, 1000l, t8.config.vocab_size - 1l]) {
            for (i in range64(dim)) {
                let el = t8.emb_q8_off + token * dim + i
                let want = t32.fblob[t32.tok_emb_off + token * dim + i]
                // read whichever scale plane is live (wscale_f16 frees the f32 one; the widen
                // is the exact disk value, so the bit-match claim is unchanged)
                let sc = t8.wscale_f16 ? f16_to_f32(uint(t8.qscales16[el / 32l])) : t8.qscales[el / 32l]
                if (float(t8.qblob[el]) * sc != want) {
                    bad++
                }
            }
        }
        t |> success(bad == 0l, "embedding rows bit-identical ({bad} mismatches)")
        delete t32
        delete t8
    }
}

// emb_q8 invariant, the untied half: a Q8 load of a Q8_0 embedding with a SEPARATE output.weight drops the
// fp32 table too (rows dequant from the linear transcode at emb_q8_off, bit-identical to the fp32 decode);
// without it an untied model carries vocab x dim floats, 1.5 GB on a 152k-vocab 4B. stories15M is the smallest untied Q8_0 file.
[test(description = "An untied Q8 load of stories15M drops the fp32 embedding table for the emb_q8 plane, whose rows and `embed_row` output are bit-identical to the fp32 load's table")]
def test_parity_untied_emb_q8_rows(t : T?) {
    t |> run("untied Q8 load: emb_q8 embedding rows bit-match the fp32 table, no fp32 table kept") @(t : T?) {
        let path = path_join(models_dir(), "stories15M-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var t32 <- load_gguf(path, QuantMode.fp32)
        var t8 <- load_gguf(path, QuantMode.q8)
        t |> success(!t32.emb_q8 && t32.tok_emb_off >= 0l, "fp32 load keeps the fblob table")
        t |> success(!t8.config.shared_weights, "the fixture is untied (output.weight on disk)")
        t |> success(!t8.cls_q8, "untied: the classifier flag stays off")
        t |> success(t8.emb_q8, "untied Q8_0 load takes the emb_q8 path")
        t |> success(t8.tok_emb_off < 0l, "no fp32 table in fblob")
        t |> success(t8.emb_q8_off != t8.wcls_off, "the embedding copy is not the classifier plane")
        let dim = t8.config.dim
        var bad = 0l
        for (token in [0l, 1l, 1000l, t8.config.vocab_size - 1l]) {
            for (i in range64(dim)) {
                let el = t8.emb_q8_off + token * dim + i
                let want = t32.fblob[t32.tok_emb_off + token * dim + i]
                let sc = t8.wscale_f16 ? f16_to_f32(uint(t8.qscales16[el / 32l])) : t8.qscales[el / 32l]
                if (float(t8.qblob[el]) * sc != want) {
                    bad++
                }
            }
        }
        t |> success(bad == 0l, "embedding rows bit-identical ({bad} mismatches)")
        // the row reader the forward pass uses agrees with the raw plane arithmetic
        var inscope row : array<float>
        row |> resize(dim)
        embed_row(t8, 1000l, unsafe(addr(row[0])))
        var bad_row = 0l
        for (i in range64(dim)) {
            if (row[i] != t32.fblob[t32.tok_emb_off + 1000l * dim + i]) {
                bad_row++
            }
        }
        t |> success(bad_row == 0l, "embed_row reads the same values ({bad_row} mismatches)")
        delete t32
        delete t8
    }
}

// the deltanet-plane engage count for one plane kind — a silently-demoted tag would otherwise
// pass the parity green on the q8 fallback rail
def private dn_kq_engaged(fmts : array<KqFmt>) : int64 {
    var n = 0l
    for (f in fmts) {
        if (f != KqFmt.q8) {
            n++
        }
    }
    return n
}

// The half-span prefix bar on one live stream, with the loosened check's control on the same stream: the fixture
// with an id flipped inside its first half must red the bar.
def private prefix_bar_cell(t : T?; tr : Model; prompt, want : array<int64>; what : string) {
    let n = length(want)
    var got <- parity_gen_ids(tr, prompt, n)
    to_log(LOG_INFO, "eyeball want: {eyeball_text(tr, want)}\n")
    to_log(LOG_INFO, "eyeball got:  {eyeball_text(tr, got)}\n")
    let same = prefix_match_len(got, want)
    t |> success(length(got) == n && fixture_ok(true, same, int64(n)), "{what} ({same} of {n} ids match)")
    var flipped := want
    let k = n / 4
    flipped[k] ^= 1l
    t |> success(!fixture_ok(true, prefix_match_len(got, flipped), int64(n)),
        "{what}: control - the fixture with id {k} of {n} flipped reds the bar")
    delete flipped
    delete got
}

// One spec's arms off ONE model load. load_model_ (not load_gguf) so the tokenizer rides along:
// text-form prompts encode here, and every compare logs its decoded-text eyeball lines.
[cold_path]
def private parity_spec_arms(t : T?; spec : ModelSpec; path : string) {
    let prev_kq = get_kquant_native()
    let prev_grouped = get_moe_grouped_prefill()
    let prev_fused = get_moe_fused_expert_gemms()
    defer() {
        set_moe_grouped_prefill(prev_grouped)
        set_moe_fused_expert_gemms(prev_fused)
    }
    // the main arm runs on the levers' defaults, whatever an earlier cell left
    set_moe_grouped_prefill(true)
    set_moe_fused_expert_gemms(true)
    set_kquant_native(spec.parity.kq_native)
    var tr <- load_model_(path, spec.parity.quant)
    set_kquant_native(prev_kq)
    // Cap the KV context: only ~(prompt + gen) positions are used, and the KV stride doesn't
    // change the computed tokens — Llama-3's native 131072 would just burn ~8GB of KV RAM.
    if (tr.config.seq_len > 2048l) {
        tr.config.seq_len = 2048l
    }
    let be = " [backend {active_kernel_backend()}]"   // read AFTER the load: that is where the pin resolves
    var prompt <- (empty(spec.parity.prompt_text) ? clone_to_move(spec.parity.prompt)
        : encode(tr, spec.parity.prompt_text, true, false))
    if (spec.parity.require_dn_kq) {
        t |> success(dn_kq_engaged(tr.dnqkv_fmt) > 0l && dn_kq_engaged(tr.dngate_fmt) > 0l && dn_kq_engaged(tr.dnout_fmt) > 0l,
            "{spec.file} dn planes kq-native (not demoted)")
    }
    let pb = spec.parity.prefix_bar
    let form = pb ? "half-span prefix" : "token-for-token"
    if (pb) {
        prefix_bar_cell(t, tr, prompt, spec.parity.gen, "{spec.file} {form}{be}")
    } else {
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true]), "{spec.file} {form}{be}")
    }
    if (spec.parity.dn_chunked) {
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [dn_chunked = true, eyeball = true, prefix_bar = pb]),
            "{spec.file} chunked-prefill {form}{be}")
    }
    if (spec.parity.moe_reference) {
        set_moe_grouped_prefill(false)
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true, prefix_bar = pb]),
            "{spec.file} reference-prefill {form}{be}")
        set_moe_grouped_prefill(true)
    }
    if (spec.parity.moe_per_expert) {
        set_moe_fused_expert_gemms(false)
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true, prefix_bar = pb]),
            "{spec.file} per-expert-prefill {form}{be}")
        set_moe_fused_expert_gemms(true)
    }
    delete prompt
    delete tr
}

[test(description = "Every model_specs row with a pinned parity fixture generates its frozen greedy ids token for token (or to the half-span prefix bar, with a flipped-id control), including its chunked, reference and per-expert prefill arms")]
def test_parity_specs(t : T?) {
    var files : array<string>
    var all <- model_specs()
    for (s in all) {
        // gpt-oss runs through its own shared-load arm below (both fixtures on ONE load — the
        // ~13GB native-mx4 resident is the suite's biggest; loading twice OOMs a 64GB box)
        if (!empty(s.parity.gen) && s.file != "gpt-oss-20b-mxfp4.gguf") {
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


// The parity dump round trip on Qwen3.5-0.8B-MTP: the device arm's forced-feed run to a file, the CPU arm's with the device
// arm's prompt, feed and routing tape replayed off that file, and the two files compared under the regions files' bar - the
// in-process parity the stocked suite runs, through the files a model past the box has to use. A poisoned row reds the compare,
// a reference with another feed is refused. Skips: no Qwen3.5-0.8B-MTP file under the models dir, the GPU tier not armed
// (DASLLAMA_GPU=1 on a device that serves the whole-model driver), not `-jit`.

let private MTP_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"
let private BAR_REL = 0.20   //! the regions files' row bar

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

[test(description = "On Qwen3.5-0.8B-MTP the device-arm dump reads back bit-exact, the CPU arm replayed on its prompt, feed and tape matches every row within the 0.20 relative bar, a one-row poison reds only that row, and a changed feed is refused")]
def test_parity_dump_round_trip(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the device arm dumped, the CPU arm dumped on the device arm's tape, the files within the bar; a poisoned row reds, another feed is refused") @(t : T?) {
        let path = path_join(models_dir(), MTP_FILE)
        if (!resident_ready(t, path)) return
        let dev_path = path_join(test_temp_dir(), "parity_dev.pd")
        let cpu_path = path_join(test_temp_dir(), "parity_cpu.pd")
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the carrier")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(40l)
            let none : array<int64>
            var dev <- parity_dump_run(tr, MTP_FILE, mirror_kv_dtype(), prompt, none, 6l, none)
            t |> equal(dev.arm, "device", "the first arm is the device's")
            t |> equal(length(dev.rows), 7, "the prefill's row and six fed steps")
            t |> equal(length(dev.feed), 6, "six fed tokens, the run's own greedy continuation")
            t |> equal(int(moe_pick_tape_mode().mode), int(MoePickTape.off), "the tape is off after the run")
            parity_dump_write(dev_path, dev)
            var back <- parity_dump_read(dev_path)
            t |> success(back.arm == dev.arm && back.vocab == dev.vocab && back.picks_per_select == dev.picks_per_select && ids_equal(back.prompt, dev.prompt) && ids_equal(back.feed, dev.feed) && ids_equal(back.tape, dev.tape), "the dump reads back as written")
            t |> equal(logits_maxdiff(back.rows[3], dev.rows[3]), 0.0, "... its rows bit for bit")
            moe_gpu_drop_model()
            var cpu <- parity_dump_run(tr, MTP_FILE, mirror_kv_dtype(), back.prompt, back.feed, 0l, back.tape)
            t |> equal(cpu.arm, "cpu", "the second arm is the CPU chain's")
            // the carrier is dense: its tape is empty and the replay sees no select; a routed carrier's replay counts every select
            t |> success(dev.picks_per_select == 0l ? cpu.tape_seen == 0l && empty(back.tape) : cpu.tape_seen > 0l, "the CPU arm replayed the device arm's routing ({cpu.tape_seen} selects, {cpu.tape_miss} own picks off the tape, {dev.picks_per_select} picks a select)")
            parity_dump_write(cpu_path, cpu)
            var cpu_back <- parity_dump_read(cpu_path)
            var r <- parity_dump_compare(back, cpu_back, BAR_REL)
            for (row in r.rows) {
                to_log(LOG_INFO, "parity dump row {row.row}: maxdiff {row.diff} (bar {row.bar}) one-step-off {row.ctrl}, argmax device {row.argmax_a} '{decode_(tr, [row.argmax_a])}' against cpu {row.argmax_b} '{decode_(tr, [row.argmax_b])}'\n")
            }
            t |> success(r.ok && empty(r.why), "the device dump is within the bar of the CPU dump on every row, every one-step-off control past it ({r.why})")
            back.rows[2][17] = back.rows[2][17] + 3.0 * r.rows[2].bar
            var poisoned <- parity_dump_compare(back, cpu_back, BAR_REL)
            t |> success(!poisoned.ok && !poisoned.rows[2].ok && poisoned.rows[3].ok, "a poisoned row (one logit three bars off) reds its compare alone")
            cpu_back.feed[0] = cpu_back.feed[0] + 1l
            var refused <- parity_dump_compare(back, cpu_back, BAR_REL)
            t |> success(!refused.ok && !empty(refused.why), "a reference with another feed is refused ({refused.why})")
            delete refused
            delete poisoned
            delete r
            delete cpu_back
            delete cpu
            delete back
            delete dev
            delete prompt
        }
        delete tr
    }
}
