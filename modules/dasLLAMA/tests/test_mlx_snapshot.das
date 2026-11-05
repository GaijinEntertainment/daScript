options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require _model_tier
require dasllama/dasllama_core
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_math
require dasllama/dasllama_tokenizer
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration for the load
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the overrides and the Metal mode, inside the das_metal halves
require _metal_blob_twin   // nolint:STYLE030,LINT019 - blob_twin and the kernel census, inside the das_metal halves
require _compares   // nolint:STYLE030,LINT019 - argmax_of, inside the das_metal halves
require daslib/strings_boost
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the kernel census, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_stats and shutdown, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_stats and shutdown, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - get_metal_b32_wide, inside the das_metal halves
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - metal_live_object_count, inside the das_metal halves
require math

// The stocked safetensors snapshots served on the CPU from their own affine planes: the config against the GGUF twin's, the
// per-plane format tags, and teacher-forced logits against two oracles - the GGUF twin (other quantized weights of the same
// checkpoint, a loose bar) and the snapshot's own runtime on the CPU (the same weights; the gap is the activation forms).
// A cell skips where its snapshot or its GGUF twin is not stocked, where the snapshot sits above the large tier without
// DASLLAMA_PARITY_FULL=1, and on the Metal arms where the build carries no dasMetal or the run is interpreted (`jit_only`).

//! the teacher-forced prompt both oracles ran: "The capital of France is Paris. The capital of Germany is Berlin. The sky is blue."
let private PROMPT_IDS = fixed_array(128000l, 791l, 6864l, 315l, 9822l, 374l, 12366l, 13l, 578l, 3363l, 315l, 9635l, 374l, 7295l, 13l, 578l, 10846l, 374l, 6437l, 13l)

//! the snapshot runtime's top-2 per prompt position (CPU device, the snapshot's own weights): id, logit, runner-up id, logit
let private ORACLE_1B <- [(16309l, 7.0703, 2l, 7.0664), (2768l, 10.1875, 220l, 10.1016), (315l, 20.4062, 3363l, 18.0156), (9822l, 16.5781, 279l, 16.2656), (374l, 21.3594, 198l, 19.5938), (12366l, 19.0000, 264l, 16.3594), (13l, 22.7969, 627l, 22.3594), (578l, 19.3594, 12366l, 18.9688), (469l, 17.4844, 6864l, 16.9062), (374l, 22.3438, 315l, 21.7812), (12366l, 20.0312, 3021l, 16.4531), (374l, 23.2812, 11l, 21.0625), (7295l, 18.6250, 3967l, 18.4688), (13l, 25.6406, 627l, 22.8750), (578l, 21.1406, 1628l, 18.3125), (6864l, 21.2969, 3363l, 18.2344), (14298l, 13.4766, 6498l, 13.3203), (645l, 15.5625, 264l, 14.1875), (11l, 19.3125, 323l, 19.0938), (578l, 19.0156, 3639l, 16.3281)]
let private ORACLE_3B <- [(2l, 5.8398, 14924l, 5.7305), (220l, 7.9961, 2768l, 6.9570), (315l, 18.3438, 3363l, 16.9062), (9822l, 13.7266, 8494l, 13.5312), (374l, 18.9844, 11l, 18.2031), (12366l, 17.5625, 7559l, 14.8203), (13l, 19.5469, 11l, 18.9531), (578l, 16.7812, 1102l, 16.3750), (6864l, 16.7969, 3363l, 15.6719), (315l, 19.5156, 374l, 18.7656), (12366l, 17.7969, 22463l, 13.6641), (374l, 18.6406, 11l, 16.4219), (7295l, 16.6719, 1101l, 14.1953), (13l, 20.0625, 627l, 17.6250), (578l, 17.8438, 1628l, 14.9141), (6864l, 17.7500, 7928l, 15.5703), (4409l, 14.6016, 40214l, 13.3125), (22463l, 11.7344, 279l, 11.5000), (13l, 16.5312, 11l, 15.3906), (578l, 16.5781, 358l, 14.1953)]

//! the 27B oracle's text, which its own tokenizer encodes to `PROMPT_27B` (no BOS)
let private TEXT_27B = "The capital of France is Paris. The capital of Italy is Rome. The ocean is blue."
let private PROMPT_27B = fixed_array(760l, 6511l, 314l, 9338l, 369l, 11751l, 13l, 561l, 6511l, 314l, 14898l, 369l, 21047l, 13l, 561l, 17415l, 369l, 6105l, 13l)

//! mlx-lm's forward of Qwen3.8-27B-4bit on the GPU over `PROMPT_27B`: the top-2 per position (id, logit, runner-up id, logit)
let private ORACLE_27B <- [(2614l, 9.5000, 220l, 9.2500), (314l, 16.2500, 3177l, 14.1875), (279l, 13.1875, 264l, 10.5000), (369l, 18.2500, 11l, 17.5000), (11751l, 17.3750, 264l, 14.6250), (13l, 19.3750, 11l, 17.6250), (198l, 14.1250, 11751l, 13.6875), (6511l, 14.3125, 6823l, 11.8125), (314l, 21.0000, 3177l, 15.1250), (9564l, 16.1250, 14898l, 15.5625), (369l, 22.0000, 11l, 15.7500), (21047l, 18.7500, 31586l, 13.1875), (13l, 18.7500, 11l, 14.5000), (561l, 17.3750, 198l, 14.8125), (6511l, 17.8750, 89798l, 13.1875), (421l, 16.8750, 369l, 16.3750), (6105l, 17.2500, 264l, 16.3750), (13l, 20.7500, 11l, 17.1250), (561l, 15.5625, 198l, 13.8125)]

//! mlx-lm's forward of Qwen3.6-35B-A3B-4bit on the GPU over `PROMPT_27B` (the same tokenizer): the top-2 per position
let private ORACLE_35B <- [(2614l, 13.4375, 1156l, 11.9375), (314l, 17.5000, 3177l, 15.9375), (279l, 14.0000, 9338l, 11.7500), (11l, 19.0000, 369l, 18.6250), (11751l, 17.7500, 264l, 15.7500), (11l, 19.5000, 13l, 19.2500), (198l, 15.4375, 11751l, 15.2500), (6511l, 15.8750, 242476l, 13.7500), (314l, 21.7500, 3177l, 17.3750), (9564l, 16.7500, 17163l, 16.3750), (369l, 22.1250, 11l, 16.5000), (21047l, 19.7500, 43984l, 14.6250), (13l, 21.8750, 11l, 18.6250), (561l, 18.3750, 3437l, 16.1250), (6511l, 20.2500, 89798l, 13.5625), (1881l, 17.7500, 369l, 17.3750), (6105l, 17.5000, 279l, 16.6250), (13l, 20.8750, 11l, 17.5000), (561l, 15.9375, 271l, 15.5625)]

//! the oracle's logit at its top-1 id against ours: the gap the activation forms leave (Q8_0 rows against the runtime's halves)
let private ORACLE_LOGIT_BAR = 0.4
//! a position whose oracle top-2 sit closer than this is a near tie: either id may lead
let private ORACLE_TIE_GAP = 0.5

//! `ids` fed one a position from 0, each position's logits row
def private teacher_forced(var m : Model; ids : array<int64>) : array<array<float>> {
    m.config.seq_len = min(m.config.seq_len, 512l)
    var s <- make_run_state(m.config)
    var out : array<array<float>>
    for (id, i in ids, count()) {
        forward(m, s, id, int64(i))
        out |> emplace(clone_to_move(s.logits))
    }
    delete s
    return <- out
}

def private teacher_forced(var m : Model) : array<array<float>> {
    var inscope ids <- [for (id in PROMPT_IDS); id]
    return <- teacher_forced(m, ids)
}

def private argmax(v : array<float>) : int64 => parallel_argmax(v, long_length(v))

//! our teacher-forced logits against the snapshot runtime's top-2 rows
def private oracle_gate(t : T?; tag : string; lg : array<array<float>>; oracle : array<tuple<int64; float; int64; float>>) {
    var worst = 0.0
    var lead_miss = 0
    for (row, o in lg, oracle) {
        worst = max(worst, nan_wide_diff(row[o._0], o._1))
        if (o._1 - o._3 >= ORACLE_TIE_GAP && argmax(row) != o._0) {
            lead_miss++
        }
    }
    to_log(LOG_INFO, "{tag} vs the snapshot runtime: worst top-1 logit gap {worst}, {lead_miss} lead misses\n")
    t |> success(worst < ORACLE_LOGIT_BAR, "{tag}: the oracle's top-1 logit within {ORACLE_LOGIT_BAR} at every position (worst {worst})")
    t |> equal(lead_miss, 0, "{tag}: every position the oracle leads by {ORACLE_TIE_GAP}+ keeps its top-1")
}

[test]
def test_mlx_llama_1b_8bit(t : T?) {
    t |> run("Llama-3.2-1B 8-bit snapshot: config = the Q8_0 twin's, mlx8 planes at every dense site, logits against both oracles") @(t : T?) {
        let snap = hf_snapshot_dir("mlx-community/Llama-3.2-1B-Instruct-8bit")
        let twin = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
        if (empty(snap)) {
            t |> skip("the 1B snapshot not present")
            return
        }
        return if (!model_available(t, twin))
        with_dasllama_jobque_() {
            var a <- load_model_(snap, QuantMode.q8)
            var b <- load_model_(twin, QuantMode.q8)
            t |> success(a.config.rope_neox && !b.config.rope_neox, "the checkpoint's half-split q/k rows rope as NEOX pairs, the converter's permuted rows as adjacent ones")
            var ca = a.config
            ca.rope_neox = b.config.rope_neox
            t |> equal(sprint_json(ca, false), sprint_json(b.config, false), "every other config field equals the twin's")
            t |> equal(a.emb_fmt, KqFmt.mlx8, "the token table (tied classifier) serves its own mlx8 planes")
            for (l in range64(a.config.n_layers)) {
                for (f in [fmt_at(a.wq_fmt, l), fmt_at(a.wk_fmt, l), fmt_at(a.wv_fmt, l), fmt_at(a.wo_fmt, l), fmt_at(a.w1_fmt, l), fmt_at(a.w2_fmt, l), fmt_at(a.w3_fmt, l)]) {
                    if (f != KqFmt.mlx8) {
                        t |> failure("layer {l}: a dense plane tagged {f}, not its disk type's mlx8")
                    }
                }
            }
            t |> equal(long_length(a.qblob), 0l, "no q8 copy of any plane")
            var inscope la <- teacher_forced(a)
            var inscope lb <- teacher_forced(b)
            var worst = 0.0
            var agree = 0
            for (ra, rb in la, lb) {
                let rel = logits_maxdiff(ra, rb) / logits_maxabs(rb)   //! inf / inf where a side holds a NaN: a NaN, read as the worst
                worst = max(worst, is_nan(rel) ? INFINITE_DIFF : rel)
                agree += argmax(ra) == argmax(rb) ? 1 : 0
            }
            to_log(LOG_INFO, "1B vs the Q8_0 twin: worst max|d|/max|logit| {worst}, top-1 agree {agree}/20\n")
            t |> success(worst < 0.08, "the twin's teacher-forced logits within 8% of their range (worst {worst}): two quantizers of one checkpoint")
            t |> success(agree >= 17, "top-1 equal to the twin's at {agree} of 20 positions")
            oracle_gate(t, "1B", la, ORACLE_1B)
            delete a
            delete b
        }
    }
}

[test]
def test_mlx_llama_3b_4bit(t : T?) {
    t |> run("Llama-3.2-3B 4-bit snapshot: mlx4 planes, logits against the runtime oracle, a greedy continuation") @(t : T?) {
        let snap = hf_snapshot_dir("mlx-community/Llama-3.2-3B-Instruct-4bit")
        if (empty(snap)) {
            t |> skip("the 3B snapshot not present")
            return
        }
        with_dasllama_jobque_() {
            var m <- load_model_(snap, QuantMode.q8)
            t |> equal(m.emb_fmt, KqFmt.mlx4, "the token table serves its own mlx4 planes")
            t |> equal(fmt_at(m.w2_fmt, 0l), KqFmt.mlx4, "the FFN down plane too")
            var inscope lg <- teacher_forced(m)
            oracle_gate(t, "3B", lg, ORACLE_3B)
            var s <- make_run_state(m.config)
            var tok = 0l
            var ids : array<int64>
            for (id, i in PROMPT_IDS, count()) {
                forward(m, s, id, int64(i))
                tok = argmax(s.logits)
            }
            for (i in range(6)) {
                ids |> push(tok)
                forward(m, s, tok, int64(length(PROMPT_IDS) + i))
                tok = argmax(s.logits)
            }
            let text = decode(m.tok, ids)
            to_log(LOG_INFO, "3B greedy continuation: '{text}'\n")
            t |> success(!empty(text), "the greedy continuation decodes to text ('{text}')")
            delete s
            delete m
        }
    }
}

//! the class stem an affine member's Metal stamps carry
def private mlx_stem(f : KqFmt) : string => capitalize("{f}")

//! member `f`'s one-row GEMV planes read since the census reset: the two-row stamps, the K-split ones, the wide ones, and twice
//! each fused gate|up dispatch - it reads two planes
def private mlx_gemv_dispatches(f : KqFmt) : int64 {
    return (metal_kernel_count("metal_kq_gemv_{f}") + metal_kernel_count("metal_kq_gemv_{f}_ks")
        + metal_kernel_count("MetalKqGemv{mlx_stem(f)}W_metal_b32_gemv_w") + 2l * metal_kernel_count("MetalKqGemv{mlx_stem(f)}Glu_metal_b32_gemv_w"))
}

//! member `f`'s dispatches since the census reset on the forms a window takes: the base tile, its split stamp and its tensor
//! arms (every `MetalKqMulMm<stem>*` stamp), the small-batch forms, the GEMV
def private mlx_window_dispatches(f : KqFmt) : int64 {
    let s = mlx_stem(f)
    var n = metal_kernel_count_prefix("MetalKqMulMm{s}")
    for (ks in ["", "Ks"]) {
        for (w in [2, 4, 8]) {
            n += metal_kernel_count("MetalB32MvB{w}{s}{ks}_metal_b32_mvb")
        }
    }
    return n + mlx_gemv_dispatches(f)
}

//! a dense model's planes tagged `f`
def private planes_of(m : Model; f : KqFmt) : int64 {
    var n = 0l
    for (l in range64(m.config.n_layers)) {
        for (g in [fmt_at(m.wq_fmt, l), fmt_at(m.wk_fmt, l), fmt_at(m.wv_fmt, l), fmt_at(m.wo_fmt, l), fmt_at(m.w1_fmt, l), fmt_at(m.w2_fmt, l), fmt_at(m.w3_fmt, l)]) {
            n += g == f ? 1l : 0l
        }
    }
    return n
}

//! `nsteps` greedy ids after `prompt`'s window through `s`: each step fed the argmax of the row the step before left
def private greedy_after(m : Model; var s : Session; prompt : array<int64>; nsteps : int) : array<int64> {
    eval_(m, s, prompt)
    var ids : array<int64>
    ids |> reserve(nsteps)
    var one <- [0l]
    for (_i in range(nsteps)) {
        one[0] = argmax_of(s.logits, m.config.vocab_size)
        ids |> push(one[0])
        eval_(m, s, one)
    }
    delete one
    return <- ids
}

//! The snapshot served on the Metal home: the blob twin's prefill window and `nsteps` greedy steps against the CPU rails' over
//! the same prompt - the ids token for token, the window one device prefill and every step a device decode, the window's affine
//! sites on their stamps (the GEMV, the small-batch form or the base tile), every step's on the GEMV, and the token table
//! gathered on the device by the member's stamp
[unused_argument(t, snap, f, tag, prompt, nsteps)]
def private metal_served_cell(t : T?; snap : string; f : KqFmt; tag : string; prompt : array<int64>; nsteps : int) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_wscale = get_wscale_f16()
        let prior_backend = active_kernel_backend()
        let prior_mode = get_metal_mode()
        set_wscale_f16(false)
        pin_kernel_backend("portable")
        set_metal_mode(MetalMode.when_available)
        var tr <- load_model_(snap, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        var trg <- blob_twin(t, snap, 512l)
        let np = planes_of(trg, f)
        t |> success(np > 0l, "{tag}: the dense planes load as {f} ({np} planes)")
        let emb = "MetalEmbedB32{mlx_stem(f)}_metal_embed_b32"
        with_dasllama_jobque_() {
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var inscope want <- greedy_after(tr, sc, prompt, nsteps)
            delete sc
            var sg = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let seat = metal_overrides_seat()
            let pf0 = metal_prefill_stats().prefills
            metal_kernel_coverage_reset()
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            eval_(trg, sg, prompt)
            t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag}: the window was one prefill the device served")
            let w = mlx_window_dispatches(f)
            t |> success(w >= np, "{tag}: the window's {f} sites rode the affine stamps ({w} dispatches, {np} planes)")
            t |> success(metal_kernel_count(emb) >= 1l, "{tag}: the window gathered its token rows on the device ({emb})")
            select_decode_override("metal")
            let de0 = metal_decode_stats().decodes
            metal_kernel_coverage_reset()
            var inscope got : array<int64>
            var one <- [0l]
            for (_i in range(nsteps)) {
                one[0] = argmax_of(sg.logits, trg.config.vocab_size)
                got |> push(one[0])
                eval_(trg, sg, one)
            }
            delete one
            restore_metal_overrides(seat)
            let served = metal_decode_stats().decodes - de0
            t |> equal(served, int64(nsteps), "{tag}: every step was a device decode")
            let gv = mlx_gemv_dispatches(f)
            t |> success(gv >= int64(nsteps) * np, "{tag}: the steps read every {f} plane through its GEMV ({gv} dispatches, {int64(nsteps) * np} planes x steps)")
            let gw = metal_kernel_count("MetalKqGemv{mlx_stem(f)}W_metal_b32_gemv_w")
            t |> success(gw > 0l || !get_metal_b32_wide(),
                "{tag}: the planes of 512 rows or more rode the wide one-row form ({gw} dispatches; DASLLAMA_METAL_B32_WIDE={get_metal_b32_wide()})")
            let gl = metal_kernel_count("MetalKqGemv{mlx_stem(f)}Glu_metal_b32_gemv_w")
            t |> success(gl >= int64(nsteps) * trg.config.n_layers || !get_metal_b32_glu() || !get_metal_b32_wide(),
                "{tag}: every step's FFN read its gate and up planes in the fused dispatch ({gl} dispatches; DASLLAMA_METAL_B32_GLU={get_metal_b32_glu()}, DASLLAMA_METAL_B32_WIDE={get_metal_b32_wide()})")
            t |> success(metal_kernel_count(emb) >= int64(nsteps), "{tag}: every step gathered its token row on the device ({emb})")
            log_gen_texts(tr, "{tag} Metal", want, got)
            t |> equal(got, want, "{tag}: the Metal greedy ids are the CPU rails' token for token")
            delete sg
        }
        metal_decode_shutdown()
        metal_prefill_shutdown()
        delete trg
        delete tr
        t |> equal(metal_live_object_count(), 0l, "{tag}: no leaked Metal objects after shutdown")
        set_metal_mode(prior_mode)
        clear_kernel_backend_pin()
        select_kernel_backend(prior_backend)
        set_wscale_f16(prior_wscale)
    }
}

[test]
def test_mlx_metal_served(t : T?) {
    for (spec in [(repo = "mlx-community/Llama-3.2-1B-Instruct-8bit", f = KqFmt.mlx8, arm = "metal-mlx8"),
                  (repo = "mlx-community/Llama-3.2-3B-Instruct-4bit", f = KqFmt.mlx4, arm = "metal-mlx4")]) {
        t |> run("{spec.repo} served on Metal: window and greedy steps == the CPU rails, on the {spec.f} stamps") @(t : T?) {
            if (!family_on(t, "llama") || !arm_on(t, spec.arm)) return
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!jit_only(t))
                let snap = hf_snapshot_dir(spec.repo)
                if (empty(snap)) {
                    t |> skip("{spec.repo} not stocked")
                    return
                }
                var inscope prompt <- [for (id in PROMPT_IDS); id]
                metal_served_cell(t, snap, spec.f, spec.arm, prompt, 16)
            } else {
                t |> skip("dasMetal not built")
            }
        }
    }
}

//! the prefill arm's bar against the CPU rails: the device window reads half tiles where the CPU reads Q8_0 rows (the dense
//! K-quant bar of the format census)
let private PREFILL_BAR = 4.0

//! one Metal prefill window over `prompt` on a fresh f16 session: the last row's logits
def private metal_window_logits(trg : Model; prompt : array<int64>) : array<float> {
    var out : array<float>
    with_dasllama_jobque_() {
        var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        eval_(trg, s, prompt)
        out := s.logits
        delete s
    }
    return <- out
}

//! member `f`'s dense twin and dev-W dispatches since the census reset (`kq_mulmm_<f>`'s forms, the dequant pass)
def private mlx_twin_dispatches(f : KqFmt) : int64 {
    let s = mlx_stem(f)
    var n = metal_kernel_count("MetalKqDequant{s}_metal_kq_dequant")
    for (v in ["T", "TH", "TH128"]) {
        n += metal_kernel_count_compiled("MetalKqMulMm{s}{v}_metal_kq_mulmm_b32_t")
    }
    for (v in ["THDb", "THDb128", "THSk", "TSk", "THSk16", "TSk16"]) {
        n += metal_kernel_count_compiled("MetalKqMulMm{s}{v}_metal_kq_mulmm_db")
    }
    return n
}

//! The snapshot's prefill window on the Metal home against the CPU rails': the base leg (no crown: the base tile, the dev-W
//! chain and the small-batch remainder), then the member's `kq_mulmm_<f>` crown pinned (the tensor twins) - each leg one device
//! prefill whose last row sits within `PREFILL_BAR` of the CPU's, its sites on the member's stamps by the census; the CPU
//! row of the prompt one token short lands outside the bar (the control)
[unused_argument(t, snap, f, tag)]
def private metal_prefill_cell(t : T?; snap : string; f : KqFmt; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_wscale = get_wscale_f16()
        let prior_backend = active_kernel_backend()
        let prev_crowns = get_metal_tensor_crowns()
        set_wscale_f16(false)
        pin_kernel_backend("portable")
        var tr <- load_model_(snap, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        var trg <- blob_twin(t, snap, 512l)
        var inscope prompt <- encode(tr, counting_text(40), true, false)
        var inscope short <- [for (i in range(length(prompt) - 1)); prompt[i]]
        var cpu, cpu_short : array<float>
        with_dasllama_jobque_() {
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sc, prompt)
            cpu := sc.logits
            delete sc
            var ss = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, ss, short)
            cpu_short := ss.logits
            delete ss
        }
        let seat = metal_overrides_seat()
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
        for (crowned in [false, true]) {
            let leg = crowned ? "kq_mulmm_{f} crowned" : "no crown"
            if (crowned) {
                pin_metal_tensor_crowns("kq_mulmm_{f}")
                metal_prefill_shutdown()
            }
            let pf0 = metal_prefill_stats().prefills
            metal_kernel_coverage_reset()
            var inscope got <- metal_window_logits(trg, prompt)
            t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag} {leg}: the window was one prefill the device served")
            let base = metal_kernel_count("MetalKqMulMm{mlx_stem(f)}_metal_kq_mulmm") + metal_kernel_count_compiled("MetalKqMulMm{mlx_stem(f)}Sk_metal_kq_mulmm")
            let twins = mlx_twin_dispatches(f)
            if (crowned) {
                t |> success(twins > 0l, "{tag} {leg}: the window's {f} sites rode the member's tensor twins or dev-W chain ({twins} dispatches)")
            } else {
                t |> success(base + twins > 0l, "{tag} {leg}: the window's {f} sites rode the base tile or the dev-W chain ({base} + {twins} dispatches)")
            }
            let d = logits_maxdiff(got, cpu)
            var pick_gpu = 0l
            var pick_cpu = 0l
            with_dasllama_jobque_() {
                pick_gpu = argmax_of(got, tr.config.vocab_size)
                pick_cpu = argmax_of(cpu, tr.config.vocab_size)
            }
            to_log(LOG_INFO, "{tag} {leg}: window last row vs the CPU rails maxd {d} (bar {PREFILL_BAR}); next '{eyeball_str(piece_(tr, pick_gpu))}' vs '{eyeball_str(piece_(tr, pick_cpu))}'\n")
            t |> success(d <= PREFILL_BAR, "{tag} {leg}: the window's last row within {PREFILL_BAR} of the CPU rails' (maxd {d})")
        }
        let ctl = logits_maxdiff(cpu_short, cpu)
        t |> success(all_finite(cpu_short) && all_finite(cpu), "{tag}: every logit the control reads is finite")
        t |> success(ctl > PREFILL_BAR, "{tag}: the control - the prompt one token short lands outside the bar ({ctl})")
        restore_metal_overrides(seat)
        unpin_metal_tensor_crowns()
        set_metal_tensor_crowns(prev_crowns)
        metal_decode_shutdown()
        metal_prefill_shutdown()
        delete cpu
        delete cpu_short
        delete trg
        delete tr
        t |> equal(metal_live_object_count(), 0l, "{tag}: no leaked Metal objects after shutdown")
        clear_kernel_backend_pin()
        select_kernel_backend(prior_backend)
        set_wscale_f16(prior_wscale)
    }
}

[test]
def test_mlx_metal_prefill(t : T?) {
    for (spec in [(repo = "mlx-community/Llama-3.2-1B-Instruct-8bit", f = KqFmt.mlx8, arm = "metal-prefill-mlx8"),
                  (repo = "mlx-community/Llama-3.2-3B-Instruct-4bit", f = KqFmt.mlx4, arm = "metal-prefill-mlx4")]) {
        t |> run("{spec.repo} prefill window on Metal == the CPU rails, base tile and crowned twins") @(t : T?) {
            if (!family_on(t, "llama") || !arm_on(t, spec.arm)) return
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!jit_only(t))
                let snap = hf_snapshot_dir(spec.repo)
                if (empty(snap)) {
                    t |> skip("{spec.repo} not stocked")
                    return
                }
                metal_prefill_cell(t, snap, spec.f, spec.arm)
            } else {
                t |> skip("dasMetal not built")
            }
        }
    }
}

//! the 27B snapshot (BF16 pairs, the deltanet planes), large tier: its tokenizer.json gives the oracle's ids, and its
//! teacher-forced logits on the CPU rails sit on mlx-lm's forward of the same weights
[test]
def test_mlx_qwen38_27b_4bit(t : T?) {
    t |> run("Qwen3.8-27B 4-bit snapshot: mlx4b planes, the tokenizer's ids and the teacher-forced logits against mlx-lm's") @(t : T?) {
        if (!family_on(t, "qwen35")) return
        let snap = hf_snapshot_dir("mlx-community/Qwen3.8-27B-4bit")
        if (empty(snap)) {
            t |> skip("the 27B snapshot not present")
            return
        }
        if (!parity_full_mode()) {
            t |> skip("the 27B snapshot is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
            return
        }
        with_dasllama_jobque_() {
            var m <- load_model_(snap, QuantMode.q8)
            t |> equal(fmt_at(m.w2_fmt, 0l), KqFmt.mlx4b, "the FFN down plane serves its own mlx4b planes")
            var inscope want <- [for (id in PROMPT_27B); id]
            var inscope ids <- encode(m, TEXT_27B, false, false)
            t |> equal(ids, want, "the snapshot's tokenizer.json encodes the oracle's text to the oracle's ids")
            var inscope lg <- teacher_forced(m, want)
            oracle_gate(t, "27B", lg, ORACLE_27B)
            delete m
        }
    }
}

//! the worst gap of our row at position i + `shift` to the oracle's top-1 logit at position i
def private oracle_worst(lg : array<array<float>>; oracle : array<tuple<int64; float; int64; float>>; shift : int) : float {
    var worst = 0.0
    for (i in range(length(oracle) - shift)) {
        worst = max(worst, nan_wide_diff(lg[i + shift][oracle[i]._0], oracle[i]._1))
    }
    return worst
}

//! the routed FFN's planes of every layer tagged `f`: the three expert stacks and the shared expert's triple
def private moe_planes_all(m : Model; f : KqFmt) : bool {
    for (l in range64(m.config.n_layers)) {
        for (g in [fmt_at(m.we1_fmt, l), fmt_at(m.we2_fmt, l), fmt_at(m.we3_fmt, l), fmt_at(m.wsh1_fmt, l), fmt_at(m.wsh2_fmt, l), fmt_at(m.wsh3_fmt, l)]) {
            return false if (g != f)
        }
    }
    return true
}

//! the 35B-A3B snapshot (routed experts, the shared expert, the 8-bit router), large tier: the expert geometry off config.json,
//! every routed and shared plane in its own mlx4b member, the teacher-forced logits against mlx-lm's forward, and a greedy
//! counting walk
[test]
def test_mlx_qwen36_35b_a3b_4bit(t : T?) {
    t |> run("Qwen3.6-35B-A3B 4-bit snapshot: the expert geometry, mlx4b routed and shared planes, the logits against mlx-lm's") @(t : T?) {
        if (!family_on(t, "qwen35moe")) return
        let snap = hf_snapshot_dir("mlx-community/Qwen3.6-35B-A3B-4bit")
        if (empty(snap)) {
            t |> skip("the 35B-A3B snapshot not present")
            return
        }
        if (!parity_full_mode()) {
            t |> skip("the 35B-A3B snapshot is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
            return
        }
        with_dasllama_jobque_() {
            var m <- load_model_(snap, QuantMode.q8)
            let c = m.config
            t |> success(c.n_expert == 256l && c.n_expert_used == 8l && c.n_ff_exp == 512l && c.n_ff_shexp == 512l,
                "the expert geometry off config.json: 256 experts, top-8, 512-wide experts and shared expert")
            t |> success(c.moe_shexp_gated && c.norm_topk_prob, "the shared expert gated, the top-k weights renormed")
            t |> success(moe_planes_all(m, KqFmt.mlx4b), "every routed stack and shared-expert plane serves its own mlx4b planes")
            t |> equal(long_length(m.qblob), 0l, "no q8 copy of any plane")
            var inscope want <- [for (id in PROMPT_27B); id]
            var inscope lg <- teacher_forced(m, want)
            oracle_gate(t, "35B-A3B", lg, ORACLE_35B)
            let ctl = oracle_worst(lg, ORACLE_35B, 1)
            to_log(LOG_INFO, "35B-A3B control: each row against the next position's oracle, worst gap {ctl}\n")
            t |> success(is_finite(ctl), "the control's every logit and oracle value is finite (a NaN reads as an infinite gap)")
            t |> success(ctl > ORACLE_LOGIT_BAR, "the control - each row against the next position's oracle - lands outside the bar ({ctl})")
            var inscope count_ids <- encode(m, "1, 2, 3, 4, 5, 6, 7,", false, false)
            var s <- make_run_state(m.config)
            var inscope walk <- greedy_after(m, s, count_ids, 6)
            let text = decode(m.tok, walk)
            to_log(LOG_INFO, "35B-A3B greedy after the count: '{text}'\n")
            t |> equal(text, " 8, 9,", "the greedy walk continues the count")
            delete s
            delete m
        }
    }
}

//! the 27B snapshot on the Metal rails, large tier: the teacher-forced logits, every position a device decode, against
//! mlx-lm's forward; then the window and a short greedy walk against the CPU rails' (`metal_served_cell`)
[test]
def test_mlx_qwen38_27b_metal(t : T?) {
    t |> run("Qwen3.8-27B 4-bit snapshot on Metal: the teacher-forced logits against mlx-lm's, the greedy walk == the CPU rails") @(t : T?) {
        if (!family_on(t, "qwen35") || !arm_on(t, "metal-27b")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!jit_only(t))
            let snap = hf_snapshot_dir("mlx-community/Qwen3.8-27B-4bit")
            if (empty(snap)) {
                t |> skip("the 27B snapshot not present")
                return
            }
            if (!parity_full_mode()) {
                t |> skip("the 27B snapshot is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
                return
            }
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            set_wscale_f16(false)
            pin_kernel_backend("portable")
            var inscope want <- [for (id in PROMPT_27B); id]
            with_dasllama_jobque_() {
                var trg <- blob_twin(t, snap, 512l)
                let seat = metal_overrides_seat()
                select_decode_override("metal")
                let de0 = metal_decode_stats().decodes
                var inscope lg <- teacher_forced(trg, want)
                let served = metal_decode_stats().decodes - de0
                restore_metal_overrides(seat)
                t |> equal(served, long_length(want), "27B Metal: every teacher-forced position was a device decode")
                oracle_gate(t, "27B Metal", lg, ORACLE_27B)
                metal_decode_shutdown()
                delete trg
            }
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
            metal_served_cell(t, snap, KqFmt.mlx4b, "metal-27b", want, 8)
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! member `f`'s routed tensor twins dispatched since the census reset
def private mlx_routed_twins(f : KqFmt) : int64 {
    var n = 0l
    for (v in ["T", "TH", "TH128", "THR"]) {
        n += metal_kernel_count("MetalMoeMulMm{mlx_stem(f)}{v}_metal_moe_mulmm_b32_t")
    }
    return n
}

//! the routed snapshot's window against the CPU rails: the Metal window's last row within the census bar of the CPU's (the
//! qwen35moe hybrid's deltanet drift under a 4-bit routed stack reads 1.5-2.0 there, the one-step-off rows 10.9 and up)
let private MLX_MOE_BAR = 3.0

//! one window leg of `metal_routed_cell` over a fresh session: the routed sites on the member's tensor twins (`base` false, the
//! knee's pick) or on its simdgroup base (the knee at 0 and the member's `moe_mulmm_<f>` and `kq_mulmm_<f>` crowns pinned off -
//! a crowned family takes its twins at every shape), one device prefill whose last row holds `MLX_MOE_BAR` of the CPU rails'
def private routed_window_leg(t : T?; trg : Model; f : KqFmt; tag : string; prompt : array<int64>; cpu : array<float>;
                              nstacks : int64; prev_crowns : string; base : bool) : Session {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let leg = base ? "base" : "twins"
        set_metal_moe_kq_twin_avg(base ? 0l : 16l)
        if (base) {
            pin_metal_tensor_crowns(crowns_without(prev_crowns, ["moe_mulmm_{f}", "kq_mulmm_{f}"]))   //! the dense crown admits the routed twins too
            metal_prefill_shutdown()
        }
        var sg = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        let pf0 = metal_prefill_stats().prefills
        metal_kernel_coverage_reset()
        eval_(trg, sg, prompt)
        t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag} {leg}: the window was one prefill the device served")
        let nb = metal_kernel_count("metal_moe_mulmm_{f}")
        let nt = mlx_routed_twins(f)
        if (base) {
            t |> success(nb >= nstacks && nt == 0l, "{tag} {leg}: every routed site rode the {f} base tile ({nb} base, {nt} twin dispatches, {nstacks} stacks)")
        } else {
            t |> success(nt >= nstacks, "{tag} {leg}: every routed site rode the {f} tensor twins ({nt} twin, {nb} base dispatches, {nstacks} stacks)")
        }
        let d = logits_maxdiff(sg.logits, cpu)
        to_log(LOG_INFO, "{tag} {leg}: window last row vs the CPU rails maxd {d} (bar {MLX_MOE_BAR})\n")
        t |> success(d <= MLX_MOE_BAR, "{tag} {leg}: the window's last row within {MLX_MOE_BAR} of the CPU rails' (maxd {d})")
        return <- sg
    } else {
        return <- make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
    }
}

//! The routed snapshot on the Metal home over `text`'s ids: the prefill window twice - the routed sites on the member's tensor
//! twins (the knee's pick), then on its simdgroup base (the knee pinned to 0 and the member's routed and dense crowns pinned off:
//! a crowned family takes its twins at every shape) - each one device prefill whose last row holds
//! `MLX_MOE_BAR` of the CPU rails' (the CPU row of the prompt one token short outside it), then `nsteps` greedy device decodes
//! from the base leg's window, the ids the CPU rails' token for token and every step's routed sites on the member's
//! expert-indexed GEMV
[unused_argument(t, snap, f, tag, text, nsteps)]
def private metal_routed_cell(t : T?; snap : string; f : KqFmt; tag, text : string; nsteps : int) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_wscale = get_wscale_f16()
        let prior_backend = active_kernel_backend()
        let prior_mode = get_metal_mode()
        set_wscale_f16(false)
        pin_kernel_backend("portable")
        set_metal_mode(MetalMode.when_available)
        var tr <- load_model_(snap, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        var inscope prompt <- encode(tr, text, true, false)
        var trg <- blob_twin(t, snap, 512l)
        let nstacks = 3l * trg.config.n_layers
        var inscope short <- [for (i in range(length(prompt) - 1)); prompt[i]]
        var inscope cpu <- metal_window_logits(tr, prompt)
        var inscope cpu_short <- metal_window_logits(tr, short)
        let ctl = logits_maxdiff(cpu_short, cpu)
        t |> success(all_finite(cpu_short) && all_finite(cpu), "{tag}: every logit the control reads is finite")
        t |> success(ctl > MLX_MOE_BAR, "{tag}: the control - the prompt one token short lands outside the bar ({ctl})")
        with_dasllama_jobque_() {
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var inscope want <- greedy_after(tr, sc, prompt, nsteps)
            delete sc
            let seat = metal_overrides_seat()
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            let knee = set_metal_moe_kq_twin_avg(16l)
            let prev_crowns = get_metal_tensor_crowns()
            var sg <- routed_window_leg(t, trg, f, tag, prompt, cpu, nstacks, prev_crowns, false)
            delete sg
            sg <- routed_window_leg(t, trg, f, tag, prompt, cpu, nstacks, prev_crowns, true)
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
            set_metal_moe_kq_twin_avg(knee)
            select_decode_override("metal")
            let de0 = metal_decode_stats().decodes
            metal_kernel_coverage_reset()
            var inscope got : array<int64>
            var one <- [0l]
            for (_i in range(nsteps)) {
                one[0] = argmax_of(sg.logits, trg.config.vocab_size)
                got |> push(one[0])
                eval_(trg, sg, one)
            }
            delete one
            restore_metal_overrides(seat)
            t |> equal(metal_decode_stats().decodes - de0, int64(nsteps), "{tag}: every step was a device decode")
            let gv = routed_gemv_dispatches(f, mlx_stem(f))
            t |> success(gv >= int64(nsteps) * nstacks, "{tag}: every step read every routed stack through the {f} routed stamps ({gv} stacks served, {int64(nsteps) * nstacks} stacks x steps)")
            routed_steps_form_cell(t, f, mlx_stem(f), tag, int64(nsteps), trg.config.n_layers)
            log_gen_texts(tr, "{tag} Metal", want, got)
            t |> equal(got, want, "{tag}: the Metal greedy ids are the CPU rails' token for token")
            delete sg
        }
        metal_decode_shutdown()
        metal_prefill_shutdown()
        delete trg
        delete tr
        t |> equal(metal_live_object_count(), 0l, "{tag}: no leaked Metal objects after shutdown")
        set_metal_mode(prior_mode)
        clear_kernel_backend_pin()
        select_kernel_backend(prior_backend)
        set_wscale_f16(prior_wscale)
    }
}

//! the 35B-A3B snapshot on the Metal rails, large tier: the teacher-forced logits, every position a device decode, against
//! mlx-lm's forward; then the routed window on the twins and on the base tile and a greedy walk against the CPU rails'
[test]
def test_mlx_qwen36_35b_metal(t : T?) {
    t |> run("Qwen3.6-35B-A3B 4-bit snapshot on Metal: the logits against mlx-lm's, the routed window and walk == the CPU rails") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "metal-35b")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!jit_only(t))
            let snap = hf_snapshot_dir("mlx-community/Qwen3.6-35B-A3B-4bit")
            if (empty(snap)) {
                t |> skip("the 35B-A3B snapshot not present")
                return
            }
            if (!parity_full_mode()) {
                t |> skip("the 35B-A3B snapshot is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
                return
            }
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            set_wscale_f16(false)
            pin_kernel_backend("portable")
            var inscope want <- [for (id in PROMPT_27B); id]
            with_dasllama_jobque_() {
                var trg <- blob_twin(t, snap, 512l)
                let seat = metal_overrides_seat()
                select_decode_override("metal")
                let de0 = metal_decode_stats().decodes
                var inscope lg <- teacher_forced(trg, want)
                let served = metal_decode_stats().decodes - de0
                restore_metal_overrides(seat)
                t |> equal(served, long_length(want), "35B-A3B Metal: every teacher-forced position was a device decode")
                oracle_gate(t, "35B-A3B Metal", lg, ORACLE_35B)
                metal_decode_shutdown()
                delete trg
            }
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
            metal_routed_cell(t, snap, KqFmt.mlx4b, "metal-35b", counting_text(24), 8)
        } else {
            t |> skip("dasMetal not built")
        }
    }
}
