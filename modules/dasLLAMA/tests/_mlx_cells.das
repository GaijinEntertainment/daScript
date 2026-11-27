options gen2
options _dasllama_internal = true

module _mlx_cells shared public

//! the MLX snapshots' oracles and teacher-forced rail, the
//! Llama-3.2-1B 8-bit snapshot's halves against its GGUF twin, and
//! the Metal cells a snapshot block's forms run; no entry loads a
//! model

require dastest/testing_boost public
require _model_tier   // hf_snapshot_dir, models_dir, log_gen_texts
require _compares   // argmax_of: a logits row's top-1 id
require _model_form   // with_ctx_cut, ModelForm, alone_pass_of
require _metal_blob_twin   // with_metal_rails, the census counts, counting_text
require _metal_decode_cells   // decode_per32_metal: the per-32 carriers' batched streams
require _kernel_census   // nolint:STYLE029,STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the overrides, inside the das_metal halves
require dasllama/dasllama_core
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_math
require dasllama/dasllama_env   // parity_full_mode: the full suite's flag
require dasllama/dasllama_gguf   // gguf_file_int, gguf_file_arch: the twin file's own context, read off its header
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration for the snapshot's load
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_stats, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_stats, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - get_metal_b32_wide, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the crowns, inside the das_metal halves
require daslib/fio   // path_join, fread
require daslib/json_boost   // read_json: the snapshot's config.json
require math

//! the teacher-forced prompt both oracles ran: "The capital of France
//! is Paris. The capital of Germany is Berlin. The sky is blue."
let PROMPT_IDS = fixed_array(128000l, 791l, 6864l, 315l, 9822l, 374l, 12366l, 13l, 578l, 3363l, 315l, 9635l, 374l, 7295l, 13l, 578l, 10846l, 374l, 6437l, 13l)

//! the snapshot runtime's top-2 per prompt position (CPU device, the
//! snapshot's own weights): id, logit, runner-up id, logit
let private ORACLE_1B <- [(16309l, 7.0703, 2l, 7.0664), (2768l, 10.1875, 220l, 10.1016), (315l, 20.4062, 3363l, 18.0156), (9822l, 16.5781, 279l, 16.2656), (374l, 21.3594, 198l, 19.5938), (12366l, 19.0000, 264l, 16.3594), (13l, 22.7969, 627l, 22.3594), (578l, 19.3594, 12366l, 18.9688), (469l, 17.4844, 6864l, 16.9062), (374l, 22.3438, 315l, 21.7812), (12366l, 20.0312, 3021l, 16.4531), (374l, 23.2812, 11l, 21.0625), (7295l, 18.6250, 3967l, 18.4688), (13l, 25.6406, 627l, 22.8750), (578l, 21.1406, 1628l, 18.3125), (6864l, 21.2969, 3363l, 18.2344), (14298l, 13.4766, 6498l, 13.3203), (645l, 15.5625, 264l, 14.1875), (11l, 19.3125, 323l, 19.0938), (578l, 19.0156, 3639l, 16.3281)]

//! the oracle's logit at its top-1 id against ours: the gap the
//! activation forms leave (Q8_0 rows against the runtime's halves)
let ORACLE_LOGIT_BAR = 0.4
//! a position whose oracle top-2 sit closer than this is a near tie:
//! either id may lead
let private ORACLE_TIE_GAP = 0.5

//! a snapshot block's context: every cell's sessions fit it
let MLX_SNAPSHOT_CTX = 512l

//! the prefill cell's bar against the CPU rails: the device window
//! reads half tiles where the CPU reads Q8_0 rows (the dense K-quant
//! bar of the format census)
let private PREFILL_BAR = 4.0

//! `ids` fed one a position from 0, each position's logits row
def teacher_forced(var m : Model; ids : array<int64>) : array<array<float>> {
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

def teacher_forced(var m : Model) : array<array<float>> {
    var inscope ids <- [for (id in PROMPT_IDS); id]
    return <- teacher_forced(m, ids)
}

//! our teacher-forced logits against the snapshot runtime's top-2
//! rows
def oracle_gate(t : T?; tag : string; lg : array<array<float>>; oracle : array<tuple<int64; float; int64; float>>) {
    var worst = 0.0
    var lead_miss = 0
    for (row, o in lg, oracle) {
        worst = max(worst, nan_wide_diff(row[o._0], o._1))
        if (o._1 - o._3 >= ORACLE_TIE_GAP && argmax_of(row) != o._0) {
            lead_miss++
        }
    }
    to_log(LOG_INFO, "{tag} vs the snapshot runtime: worst top-1 logit gap {worst}, {lead_miss} lead misses\n")
    t |> success(worst < ORACLE_LOGIT_BAR, "{tag}: the oracle's top-1 logit within {ORACLE_LOGIT_BAR} at every position (worst {worst})")
    t |> equal(lead_miss, 0, "{tag}: every position the oracle leads by {ORACLE_TIE_GAP}+ keeps its top-1")
}

//! the context the GGUF file at `path` declares
//! (`{arch}.context_length`), read off its header: a block's form
//! caps its model's context, the file's stays the twin's
def private file_seq_len(path : string) : int64 => gguf_file_int(path, "{gguf_file_arch(path)}.context_length", 0l)

//! the context the snapshot `repo` declares
//! (`max_position_embeddings`), read off its `config.json`: the
//! snapshot's twin of `file_seq_len`
def private snapshot_seq_len(repo : string) : int64 {
    var err = ""
    var doc = read_json(fread(path_join(hf_snapshot_dir(repo), "config.json")), err)
    let n = doc?["max_position_embeddings"] ?? 0l
    delete_json(doc)
    return n
}

//! one side of the 8-bit snapshot's compare against its Q8_0 GGUF
//! twin: whether its form ran, its rope pairing, its config as JSON
//! with the pairing cleared (the one field the two loaders tell
//! apart), its teacher-forced rows
struct MlxPairSide {
    ran : bool
    neox : bool
    config : string
    rows : array<array<float>>
}

//! a config as the pair's compare reads it: the rope pairing cleared
def private pair_config(c : Config) : string {
    var cc = c
    cc.rope_neox = false
    return sprint_json(cc, false)
}

//! the Q8_0 GGUF twin's side on its CPU form `b`: the file's own
//! context in the config, the rows at 512 positions
def mlx_llama_1b_twin_side(var b : Model) : MlxPairSide {
    var cb = b.config
    cb.seq_len = file_seq_len(path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"))
    var side = MlxPairSide(ran = true, neox = b.config.rope_neox, config = pair_config(cb))
    with_ctx_cut(b, 512l) {
        side.rows <- teacher_forced(b)
    }
    return <- side
}

//! the 8-bit snapshot's side on its CPU form `a`, the snapshot's own
//! context in the config, with what it
//! asserts on its load alone: mlx8 planes at every dense site and the
//! token table, no q8 copy of any plane, the teacher-forced rows
//! against the snapshot runtime's
def mlx_llama_1b_8bit_side(t : T?; var a : Model) : MlxPairSide {
    t |> equal(a.emb_fmt, KqFmt.mlx8, "the token table (tied classifier) serves its own mlx8 planes")
    for (l in range64(a.config.n_layers)) {
        for (f in [fmt_at(a.wq_fmt, l), fmt_at(a.wk_fmt, l), fmt_at(a.wv_fmt, l), fmt_at(a.wo_fmt, l), fmt_at(a.w1_fmt, l), fmt_at(a.w2_fmt, l), fmt_at(a.w3_fmt, l)]) {
            if (f != KqFmt.mlx8) {
                t |> failure("layer {l}: a dense plane tagged {f}, not its disk type's mlx8")
            }
        }
    }
    t |> equal(long_length(a.qblob), 0l, "no q8 copy of any plane")
    var ca = a.config
    ca.seq_len = snapshot_seq_len(decode_per32_mlx_1b_8bit().repo)
    var side = MlxPairSide(ran = true, neox = a.config.rope_neox, config = pair_config(ca))
    side.rows <- teacher_forced(a)
    oracle_gate(t, "1B", side.rows, ORACLE_1B)
    return <- side
}

//! after both blocks: the snapshot's config the twin's but the rope
//! pairing, the checkpoint's half-split q/k rows roped as NEOX pairs
//! against the converter's permuted adjacent ones, the teacher-forced
//! logits within 8% of the twin's range and its top-1 at 17 of 20
//! positions; a skip where the twin's form did not run
def mlx_llama_1b_8bit_within(t : T?; twin, snap : MlxPairSide) {
    return if (!snap.ran)
    if (!twin.ran) {
        skip_cell(t, "Llama-3.2-1B 8-bit snapshot against its Q8_0 twin: the twin's form did not run")
        return
    }
    unsafe {
        t |> run("Llama-3.2-1B 8-bit snapshot: config = the Q8_0 twin's, logits against the twin") @capture(& twin, & snap)(t : T ?) {
            t |> success(snap.neox && !twin.neox, "the checkpoint's half-split q/k rows rope as NEOX pairs, the converter's permuted rows as adjacent ones")
            t |> equal(snap.config, twin.config, "every other config field equals the twin's")
            var worst = 0.0
            var agree = 0
            for (ra, rb in snap.rows, twin.rows) {
                let rel = logits_maxdiff(ra, rb) / logits_maxabs(rb)   //! inf / inf where a side holds a NaN: a NaN, read as the worst
                worst = max(worst, is_nan(rel) ? INFINITE_DIFF : rel)
                agree += argmax_of(ra) == argmax_of(rb) ? 1 : 0
            }
            to_log(LOG_INFO, "1B vs the Q8_0 twin: worst max|d|/max|logit| {worst}, top-1 agree {agree}/20\n")
            t |> success(worst < 0.08, "the twin's teacher-forced logits within 8% of their range (worst {worst}): two quantizers of one checkpoint")
            t |> success(agree >= 17, "top-1 equal to the twin's at {agree} of 20 positions")
        }
    }
}

//! a snapshot block's Metal form: the blob twin on the default
//! weight scale
def mlx_metal_form() : ModelForm => metal_form(ctx = MLX_SNAPSHOT_CTX)

//! whether the pass `form` is the snapshot's own Metal form
//! (`mlx_metal_form`), not a block's other Metal forms - a served
//! image, a required twin
def mlx_metal_pass(form : ModelForm) : bool => (form.on_metal && !form.metal_image
    && !form.metal_required && form.ctx == MLX_SNAPSHOT_CTX)

//! the CPU form a snapshot's Metal cells read the CPU rails on: the
//! portable pin
def mlx_cpu_form() : ModelForm => cpu_cells_form(backend = "portable")

//! the snapshot block's Metal forms where the build serves the Metal
//! home and the arm filter reaches one of `arms`, the CPU form their
//! cells read beside them; else none, the reason a skipped cell of
//! `t` naming `repo`
def mlx_metal_forms(t : T?; repo : string; arms : array<string>) : array<ModelForm> {
    var forms : array<ModelForm>
    let lacks = metal_build_why()
    var reached = false
    for (arm in arms) {
        reached ||= empty(arm_why(arm))
    }
    if (!empty(lacks)) {
        skip_cell(t, "{repo}: the Metal forms: {lacks}")
    } elif (!reached) {
        skip_cell(t, "{repo}: the Metal forms: the arm filter reaches none of {arms}")
    } else {
        forms |> push(mlx_metal_form())
        forms |> push(mlx_cpu_form())
    }
    return <- forms
}

//! member `f`'s dispatches since the census reset on the forms a
//! window takes: the base tile, its split stamp and its tensor arms
//! (every `MetalKqMulMm<stem>*` stamp), the small-batch forms, the
//! GEMV
def private mlx_window_dispatches(f : KqFmt) : int64 {
    return (metal_kernel_count_prefix("MetalKqMulMm{kq_stem(f)}") + kq_mvb_dispatches(f)
        + kq_gemv_dispatches(f))
}

//! what the served cell's Metal side keeps for its CPU side and
//! verdict: the prompt, the ids the device's greedy steps emitted,
//! the CPU rails' over the same prompt
struct MlxServedKept {
    tag : string
    prompt : array<int64>
    nsteps : int
    got : array<int64>
    want : array<int64>
    metal : bool
    cpu : bool
}

//! The snapshot served on the Metal home, on the Metal form's blob
//! twin `trg`: the prefill window and `nsteps` greedy steps over
//! `prompt` - the window one device prefill and every step a device
//! decode, the window's affine sites on their stamps (the GEMV, the
//! small-batch form or the base tile), every step's on the GEMV, the
//! token table gathered on the device by the member's stamp - the ids
//! kept for the CPU rails' (`mlx_served_cpu`); under the arm `tag`
[unused_argument(t, trg, f, tag, prompt, nsteps, kept)]
def mlx_served_metal(t : T?; var trg : Model; f : KqFmt; tag : string; prompt : array<int64>; nsteps : int;
                     var kept : MlxServedKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!arm_on(t, tag))
        kept.metal = true
        kept.tag = tag
        kept.prompt := prompt
        kept.nsteps = nsteps
        let np = sites_count(trg, true, false, $(g : KqFmt) : bool => g == f)
        t |> success(np > 0l, "{tag}: the dense planes load as {f} ({np} planes)")
        let emb = "MetalEmbedB32{kq_stem(f)}_metal_embed_b32"
        with_dasllama_jobque_() {
            var sg = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            var de0 = 0l
            with_metal_rails(MetalRails(decode = false)) {
                let pf0 = metal_prefill_stats().prefills
                kernel_census_reset_metal()
                eval_(trg, sg, prompt)
                t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag}: the window was one prefill the device served")
                let w = mlx_window_dispatches(f)
                t |> success(w >= np, "{tag}: the window's {f} sites rode the affine stamps ({w} dispatches, {np} planes)")
                t |> success(metal_kernel_count(emb) >= 1l, "{tag}: the window gathered its token rows on the device ({emb})")
                select_decode_override("metal")
                de0 = metal_decode_stats().decodes
                kernel_census_reset_metal()
                kept.got <- greedy_eval_steps(trg, sg, nsteps)
            }
            let served = metal_decode_stats().decodes - de0
            t |> equal(served, int64(nsteps), "{tag}: every step was a device decode")
            let gv = kq_gemv_dispatches(f)
            t |> success(gv >= int64(nsteps) * np, "{tag}: the steps read every {f} plane through its GEMV ({gv} dispatches, {int64(nsteps) * np} planes x steps)")
            let gw = metal_kernel_count("MetalKqGemv{kq_stem(f)}W_metal_b32_gemv_w")
            t |> success(gw > 0l || !get_metal_b32_wide(),
                "{tag}: the planes of 512 rows or more rode the wide one-row form ({gw} dispatches; DASLLAMA_METAL_B32_WIDE={get_metal_b32_wide()})")
            let gl = metal_kernel_count("MetalKqGemv{kq_stem(f)}Glu_metal_b32_gemv_w")
            t |> success(gl >= int64(nsteps) * trg.config.n_layers || !get_metal_b32_glu() || !get_metal_b32_wide(),
                "{tag}: every step's FFN read its gate and up planes in the fused dispatch ({gl} dispatches; DASLLAMA_METAL_B32_GLU={get_metal_b32_glu()}, DASLLAMA_METAL_B32_WIDE={get_metal_b32_wide()})")
            t |> success(metal_kernel_count(emb) >= int64(nsteps), "{tag}: every step gathered its token row on the device ({emb})")
            delete sg
        }
    }
}

//! the served cell's CPU side on the CPU rails' model `tr`, on the
//! pass of `mlx_cpu_form()` alone: the greedy ids over the kept
//! prompt; other passes pass by
def mlx_served_cpu(var tr : Model; form : ModelForm; var kept : MlxServedKept) {
    return if (!kept.metal || kept.cpu || !alone_pass_of(form, mlx_cpu_form()))
    kept.cpu = true
    with_dasllama_jobque_() {
        var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        eval_(tr, sc, kept.prompt)
        kept.want <- greedy_eval_steps(tr, sc, kept.nsteps)
        delete sc
    }
    log_gen_texts(tr, "{kept.tag} Metal", kept.want, kept.got)
}

//! the served cell's compare after the block: the Metal greedy ids
//! the CPU rails' token for token; a Metal side whose CPU side never
//! ran a failure
def mlx_served_verdict(t : T?; var kept : MlxServedKept) {
    if (kept.metal && !kept.cpu) {
        t |> failure("{kept.tag}: the Metal side ran and the CPU rails' side did not - the block gives no `mlx_cpu_form()`")
    } elif (kept.cpu) {
        t |> equal(kept.got, kept.want, "{kept.tag}: the Metal greedy ids are the CPU rails' token for token")
    }
    delete kept
}

//! what the prefill cell's Metal side keeps: the prompt, each leg's
//! name and device window's last row, the CPU rails' row of the
//! prompt and of the prompt one token short, the next pieces both
//! sides pick
struct MlxPrefillKept {
    tag : string
    prompt : array<int64>
    legs : array<string>
    rows : array<array<float>>
    gpu_pieces : array<string>
    cpu : array<float>
    cpu_short : array<float>
    cpu_piece : string
    metal : bool
    cpu_ran : bool
}

//! The snapshot's prefill window on the Metal form's blob twin `trg`:
//! the base leg (no crown: the base tile, the dev-W chain and the
//! small-batch remainder), then the member's `kq_mulmm_<f>` crown
//! pinned (the tensor twins) - each leg one device prefill, its sites
//! on the member's stamps by the census, its last row kept for the
//! CPU rails' (`mlx_prefill_cpu`); under the arm `tag`. The crowned
//! leg shuts the prefill driver down so its pipelines compile under
//! the crown
[unused_argument(t, trg, f, tag, kept)]
def mlx_prefill_metal(t : T?; var trg : Model; f : KqFmt; tag : string; var kept : MlxPrefillKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!arm_on(t, tag))
        kept.metal = true
        kept.tag = tag
        kept.prompt <- encode(trg, counting_text(40), true, false)
        let prev_crowns = get_metal_tensor_crowns()
        let prev_pinned = metal_tensor_crowns_pinned()
        kept.gpu_pieces |> reserve(2)
        kept.legs |> reserve(2)
        kept.rows |> reserve(2)
        with_metal_rails(MetalRails(decode = false)) {
            for (crowned in [false, true]) {
                let leg = crowned ? "kq_mulmm_{f} crowned" : "no crown"
                if (crowned) {
                    pin_metal_tensor_crowns("kq_mulmm_{f}")
                    metal_prefill_shutdown()
                }
                let pf0 = metal_prefill_stats().prefills
                kernel_census_reset_metal()
                var got <- window_last_logits(trg, kept.prompt)
                t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag} {leg}: the window was one prefill the device served")
                let base = metal_kernel_count("MetalKqMulMm{kq_stem(f)}_metal_kq_mulmm") + metal_kernel_count_compiled("MetalKqMulMm{kq_stem(f)}Sk_metal_kq_mulmm")
                let twins = kq_twin_dispatches(f, true)
                if (crowned) {
                    t |> success(twins > 0l, "{tag} {leg}: the window's {f} sites rode the member's tensor twins or dev-W chain ({twins} dispatches)")
                } else {
                    t |> success(base + twins > 0l, "{tag} {leg}: the window's {f} sites rode the base tile or the dev-W chain ({base} + {twins} dispatches)")
                }
                var pick = 0l
                with_dasllama_jobque_() {
                    pick = argmax_of(got, trg.config.vocab_size)
                }
                kept.gpu_pieces |> push(piece_(trg, pick))
                kept.legs |> push(leg)
                kept.rows |> emplace(got)
            }
        }
        if (prev_pinned) {
            pin_metal_tensor_crowns(prev_crowns)
        } else {
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
        }
    }
}

//! the prefill cell's CPU side on the CPU rails' model `tr`, on the
//! pass of `mlx_cpu_form()` alone: the window's last row over the
//! kept prompt and over the prompt one token short, the row's next
//! piece; other passes pass by
def mlx_prefill_cpu(var tr : Model; form : ModelForm; var kept : MlxPrefillKept) {
    return if (!kept.metal || kept.cpu_ran || !alone_pass_of(form, mlx_cpu_form()))
    kept.cpu_ran = true
    var inscope short <- [for (i in range(length(kept.prompt) - 1)); kept.prompt[i]]
    kept.cpu <- window_last_logits(tr, kept.prompt)
    kept.cpu_short <- window_last_logits(tr, short)
    var pick = 0l
    with_dasllama_jobque_() {
        pick = argmax_of(kept.cpu, tr.config.vocab_size)
    }
    kept.cpu_piece = piece_(tr, pick)
}

//! the prefill cell's compares after the block: each leg's last row
//! within `PREFILL_BAR` of the CPU rails', the CPU row of the prompt
//! one token short outside it (the control); a Metal side whose CPU
//! side never ran a failure
def mlx_prefill_verdict(t : T?; var kept : MlxPrefillKept) {
    if (kept.metal && !kept.cpu_ran) {
        t |> failure("{kept.tag}: the Metal legs ran and the CPU rails' side did not - the block gives no `mlx_cpu_form()`")
    } elif (kept.cpu_ran) {
        let tag = kept.tag
        for (leg, got, piece in kept.legs, kept.rows, kept.gpu_pieces) {
            let d = logits_maxdiff(got, kept.cpu)
            to_log(LOG_INFO, "{tag} {leg}: window last row vs the CPU rails maxd {d} (bar {PREFILL_BAR}); next '{eyeball_str(piece)}' vs '{eyeball_str(kept.cpu_piece)}'\n")
            t |> success(d <= PREFILL_BAR, "{tag} {leg}: the window's last row within {PREFILL_BAR} of the CPU rails' (maxd {d})")
        }
        let ctl = logits_maxdiff(kept.cpu_short, kept.cpu)
        t |> success(all_finite(kept.cpu_short) && all_finite(kept.cpu), "{tag}: every logit the control reads is finite")
        t |> success(ctl > PREFILL_BAR, "{tag}: the control - the prompt one token short lands outside the bar ({ctl})")
    }
    delete kept
}

//! one Llama snapshot's Metal cells: the served cell's, the prefill
//! cell's
struct MlxLlamaMetalKept {
    served : MlxServedKept
    prefill : MlxPrefillKept
}

//! a Llama MLX snapshot (member `f`) on its Metal form `trg`: the
//! served window and 16 greedy steps (arm `served_arm`), the per-32
//! carrier's batched streams (`per32`), the prefill window uncrowned
//! and crowned (arm `prefill_arm`) - last: its crowned leg shuts the
//! prefill driver down
def mlx_llama_metal_cells(t : T?; var trg : Model; form : ModelForm; f : KqFmt; served_arm, prefill_arm : string;
                          per32 : DecodePer32Row; var kept : MlxLlamaMetalKept) {
    var inscope prompt <- [for (id in PROMPT_IDS); id]
    mlx_served_metal(t, trg, f, served_arm, prompt, 16, kept.served)
    decode_per32_metal(t, trg, form, per32)
    mlx_prefill_metal(t, trg, f, prefill_arm, kept.prefill)
}

//! the Llama snapshot's CPU sides of its Metal cells, on the pass of
//! `mlx_cpu_form()` alone
def mlx_llama_metal_cpu(var tr : Model; form : ModelForm; var kept : MlxLlamaMetalKept) {
    mlx_served_cpu(tr, form, kept.served)
    mlx_prefill_cpu(tr, form, kept.prefill)
}

//! the Llama snapshot's Metal cells' compares after the block
def mlx_llama_metal_verdict(t : T?; var kept : MlxLlamaMetalKept) {
    mlx_served_verdict(t, kept.served)
    mlx_prefill_verdict(t, kept.prefill)
    delete kept
}

//! the 27B oracle's text, which its own tokenizer encodes to
//! `PROMPT_27B` (no BOS)
let private TEXT_27B = "The capital of France is Paris. The capital of Italy is Rome. The ocean is blue."
let PROMPT_27B = fixed_array(760l, 6511l, 314l, 9338l, 369l, 11751l, 13l, 561l, 6511l, 314l, 14898l, 369l, 21047l, 13l, 561l, 17415l, 369l, 6105l, 13l)

//! mlx-lm's forward of Qwen3.8-27B-4bit on the GPU over `PROMPT_27B`:
//! the top-2 per position (id, logit, runner-up id, logit)
let private ORACLE_27B <- [(2614l, 9.5000, 220l, 9.2500), (314l, 16.2500, 3177l, 14.1875), (279l, 13.1875, 264l, 10.5000), (369l, 18.2500, 11l, 17.5000), (11751l, 17.3750, 264l, 14.6250), (13l, 19.3750, 11l, 17.6250), (198l, 14.1250, 11751l, 13.6875), (6511l, 14.3125, 6823l, 11.8125), (314l, 21.0000, 3177l, 15.1250), (9564l, 16.1250, 14898l, 15.5625), (369l, 22.0000, 11l, 15.7500), (21047l, 18.7500, 31586l, 13.1875), (13l, 18.7500, 11l, 14.5000), (561l, 17.3750, 198l, 14.8125), (6511l, 17.8750, 89798l, 13.1875), (421l, 16.8750, 369l, 16.3750), (6105l, 17.2500, 264l, 16.3750), (13l, 20.7500, 11l, 17.1250), (561l, 15.5625, 198l, 13.8125)]

//! the teacher-forced logits over `PROMPT_27B` on the Metal form's
//! twin `trg`, every position a device decode, against mlx-lm's
//! forward `oracle`
def metal_teacher_forced(t : T?; var trg : Model; tag : string; oracle : array<tuple<int64; float; int64; float>>) {
    var inscope want <- [for (id in PROMPT_27B); id]
    with_dasllama_jobque_() {
        with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
            let de0 = metal_decode_count()
            var inscope lg <- teacher_forced(trg, want)
            let served = metal_decode_count() - de0
            t |> equal(served, long_length(want), "{tag} Metal: every teacher-forced position was a device decode")
            oracle_gate(t, "{tag} Metal", lg, oracle)
        }
    }
}

//! the 27B snapshot on its CPU form: mlx4b planes, its
//! tokenizer.json's ids, the teacher-forced logits against mlx-lm's
def private q38_27b_cpu_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("Qwen3.8-27B 4-bit snapshot: mlx4b planes, the tokenizer's ids and the teacher-forced logits against mlx-lm's") @capture(& m)(t : T ?) {
            t |> equal(fmt_at(m.w2_fmt, 0l), KqFmt.mlx4b, "the FFN down plane serves its own mlx4b planes")
            var inscope want <- [for (id in PROMPT_27B); id]
            var inscope ids <- encode(m, TEXT_27B, false, false)
            t |> equal(ids, want, "the snapshot's tokenizer.json encodes the oracle's text to the oracle's ids")
            var inscope lg <- teacher_forced(m, want)
            oracle_gate(t, "27B", lg, ORACLE_27B)
        }
    }
}

//! the 27B snapshot on its Metal form, under the arm `metal-27b`: the
//! teacher-forced logits against mlx-lm's, then the window and 8
//! greedy steps kept for the CPU rails'
def private q38_27b_metal_cells(t : T?; var m : Model; var served : MlxServedKept) {
    return if (!arm_on(t, "metal-27b"))
    metal_teacher_forced(t, m, "27B", ORACLE_27B)
    var inscope want <- [for (id in PROMPT_27B); id]
    mlx_served_metal(t, m, KqFmt.mlx4b, "metal-27b", want, 8, served)
}

//! the Qwen3.8-27B 4-bit snapshot's forms beside its block's own
//! (`test_dflash_drafter.das`'s trunk): its CPU form, and the Metal
//! twin under the arm `metal-27b` with its CPU rails' form
//! (`mlx_metal_forms`); none below the large tier without
//! DASLLAMA_PARITY_FULL=1, a skipped cell of `t`
def mlx_qwen38_27b_forms(t : T?) : array<ModelForm> {
    var forms : array<ModelForm>
    if (!parity_full_mode()) {
        skip_cell(t, "the 27B snapshot is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
        return <- forms
    }
    forms |> push(cpu_cells_form())
    var inscope metal <- mlx_metal_forms(t, "mlx-community/Qwen3.8-27B-4bit", ["metal-27b"])
    forms |> push_from(metal)
    return <- forms
}

//! one form's cells of the 27B snapshot (`mlx_qwen38_27b_forms`): on
//! its Metal twin (`mlx_metal_pass`) the teacher-forced logits
//! against mlx-lm's and the served window and 8 greedy steps; on its
//! CPU form the mlx4b planes, the tokenizer's ids and the
//! teacher-forced logits; on the CPU rails' form the served cell's
//! CPU side; other forms pass by
def mlx_qwen38_27b_cells(t : T?; var m : Model; form : ModelForm; var served : MlxServedKept) {
    if (mlx_metal_pass(form)) {
        q38_27b_metal_cells(t, m, served)
    } elif (alone_pass_of(form, cpu_cells_form())) {
        q38_27b_cpu_cell(t, m)
    } else {
        mlx_served_cpu(m, form, served)
    }
}
