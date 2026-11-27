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
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the overrides, inside the das_metal halves
require _metal_blob_twin   // nolint:STYLE030,LINT019 - the census counts and the overrides' seat, inside the das_metal halves
require _compares   // argmax_of
require _cpu_gpu_model   // with_cpu_and_gpu_snapshot, the forms
require _metal_decode_cells   // decode_per32_mlx_3b_4bit: the 3B snapshot's per-32 row
require _mlx_cells   // the prompt, the oracles' gate, the teacher-forced rail, the snapshots' Metal cells
require _metal_format_census_cells   // the 35B snapshot's weight-format census row
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_stats, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_stats, inside the das_metal halves
require math
require _kernel_census   // nolint:STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves

// The stocked safetensors snapshots, one block each
// (`with_cpu_and_gpu_snapshot`): on the snapshot's CPU form the
// per-plane format tags and teacher-forced logits against the
// snapshot's own runtime (the same weights; the gap is the activation
// forms), on its Metal form - the blob twin - the window and greedy
// steps against the CPU rails' (the portable-pinned CPU form), the
// Metal compares after the block. A cell skips where its snapshot is
// not stocked, where the snapshot sits above the large tier without
// DASLLAMA_PARITY_FULL=1, and the Metal forms where the build carries
// no dasMetal, the run is interpreted (`jit_only`) or the arm filter
// reaches none of their cells. The Llama-3.2-1B 8-bit snapshot's
// block runs beside its GGUF twin's, in that twin's test
// (`test_gpu_resident_llama.das`); the Qwen3.8-27B 4-bit snapshot's
// cells ride its trunk's block (`mlx_qwen38_27b_forms`,
// `_mlx_cells.das`).

let private ORACLE_3B <- [(2l, 5.8398, 14924l, 5.7305), (220l, 7.9961, 2768l, 6.9570), (315l, 18.3438, 3363l, 16.9062), (9822l, 13.7266, 8494l, 13.5312), (374l, 18.9844, 11l, 18.2031), (12366l, 17.5625, 7559l, 14.8203), (13l, 19.5469, 11l, 18.9531), (578l, 16.7812, 1102l, 16.3750), (6864l, 16.7969, 3363l, 15.6719), (315l, 19.5156, 374l, 18.7656), (12366l, 17.7969, 22463l, 13.6641), (374l, 18.6406, 11l, 16.4219), (7295l, 16.6719, 1101l, 14.1953), (13l, 20.0625, 627l, 17.6250), (578l, 17.8438, 1628l, 14.9141), (6864l, 17.7500, 7928l, 15.5703), (4409l, 14.6016, 40214l, 13.3125), (22463l, 11.7344, 279l, 11.5000), (13l, 16.5312, 11l, 15.3906), (578l, 16.5781, 358l, 14.1953)]


//! mlx-lm's forward of Qwen3.6-35B-A3B-4bit on the GPU over
//! `PROMPT_27B` (the same tokenizer): the top-2 per position
let private ORACLE_35B <- [(2614l, 13.4375, 1156l, 11.9375), (314l, 17.5000, 3177l, 15.9375), (279l, 14.0000, 9338l, 11.7500), (11l, 19.0000, 369l, 18.6250), (11751l, 17.7500, 264l, 15.7500), (11l, 19.5000, 13l, 19.2500), (198l, 15.4375, 11751l, 15.2500), (6511l, 15.8750, 242476l, 13.7500), (314l, 21.7500, 3177l, 17.3750), (9564l, 16.7500, 17163l, 16.3750), (369l, 22.1250, 11l, 16.5000), (21047l, 19.7500, 43984l, 14.6250), (13l, 21.8750, 11l, 18.6250), (561l, 18.3750, 3437l, 16.1250), (6511l, 20.2500, 89798l, 13.5625), (1881l, 17.7500, 369l, 17.3750), (6105l, 17.5000, 279l, 16.6250), (13l, 20.8750, 11l, 17.5000), (561l, 15.9375, 271l, 15.5625)]

//! the 3B snapshot on its CPU form: mlx4 planes, the teacher-forced
//! logits against the snapshot runtime's, a greedy continuation
def private mlx_3b_cpu_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("Llama-3.2-3B 4-bit snapshot: mlx4 planes, logits against the runtime oracle, a greedy continuation") @capture(& m)(t : T ?) {
            t |> equal(m.emb_fmt, KqFmt.mlx4, "the token table serves its own mlx4 planes")
            t |> equal(fmt_at(m.w2_fmt, 0l), KqFmt.mlx4, "the FFN down plane too")
            var inscope lg <- teacher_forced(m)
            oracle_gate(t, "3B", lg, ORACLE_3B)
            var s <- make_run_state(m.config)
            var tok = 0l
            var ids : array<int64>
            ids |> reserve(6)
            for (id, i in PROMPT_IDS, count()) {
                forward(m, s, id, int64(i))
                tok = argmax_of(s.logits)
            }
            for (i in range(6)) {
                ids |> push(tok)
                forward(m, s, tok, int64(length(PROMPT_IDS) + i))
                tok = argmax_of(s.logits)
            }
            let text = decode(m.tok, ids)
            to_log(LOG_INFO, "3B greedy continuation: '{text}'\n")
            t |> success(!empty(text), "the greedy continuation decodes to text ('{text}')")
            delete s
            delete ids
        }
    }
}

[test(description = "The Llama-3.2-3B MLX 4-bit snapshot serves mlx4 planes on the CPU, its teacher-forced logits within 0.4 of the snapshot runtime's top-1 at every position with no lead miss, and a greedy continuation decodes to text; on the Metal home's blob twin, on a das_metal build, it prefills in one device window and decodes 16 greedy steps on the affine GEMV and embed stamps, emitting the CPU rails' ids exactly, batches two to twelve per-32 streams token for token the single-row step's, and its prefill window, uncrowned and with the `kq_mulmm` tensor twins crowned, lands its last logits row within 4.0 max-abs of the CPU rails' while the one-token-short control lands outside; each form loads once")]
def test_mlx_llama_3b_4bit(t : T?) {
    return if (!family_on(t, "llama"))
    var forms <- [cpu_cells_form()]
    var inscope metal <- mlx_metal_forms(t, "mlx-community/Llama-3.2-3B-Instruct-4bit", ["metal-mlx4", "metal-prefill-mlx4", "batch-per32"])
    forms |> push_from(metal)
    var k : MlxLlamaMetalKept
    with_cpu_and_gpu_snapshot(t, "llm", "mlx-community/Llama-3.2-3B-Instruct-4bit", MLX_SNAPSHOT_CTX, forms) $(var m, form) {
        if (form.on_metal) {
            mlx_llama_metal_cells(t, m, form, KqFmt.mlx4, "metal-mlx4", "metal-prefill-mlx4", decode_per32_mlx_3b_4bit(), k)
        } elif (alone_pass_of(form, cpu_cells_form())) {
            mlx_3b_cpu_cell(t, m)
        } else {
            mlx_llama_metal_cpu(m, form, k)
        }
    }
    mlx_llama_metal_verdict(t, k)
    delete forms
}

//! the worst gap of our row at position i + `shift` to the oracle's
//! top-1 logit at position i
def private oracle_worst(lg : array<array<float>>; oracle : array<tuple<int64; float; int64; float>>; shift : int) : float {
    var worst = 0.0
    for (i in range(length(oracle) - shift)) {
        worst = max(worst, nan_wide_diff(lg[i + shift][oracle[i]._0], oracle[i]._1))
    }
    return worst
}

//! the routed FFN's planes of every layer tagged `f`: the three
//! expert stacks and the shared expert's triple
def private moe_planes_all(m : Model; f : KqFmt) : bool {
    for (l in range64(m.config.n_layers)) {
        for (g in [fmt_at(m.we1_fmt, l), fmt_at(m.we2_fmt, l), fmt_at(m.we3_fmt, l), fmt_at(m.wsh1_fmt, l), fmt_at(m.wsh2_fmt, l), fmt_at(m.wsh3_fmt, l)]) {
            return false if (g != f)
        }
    }
    return true
}

//! the 35B-A3B snapshot on its CPU form: the expert geometry off
//! config.json, every routed and shared plane in its own mlx4b
//! member, the teacher-forced logits against mlx-lm's forward, and a
//! greedy counting walk
def private q36_35b_cpu_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("Qwen3.6-35B-A3B 4-bit snapshot: the expert geometry, mlx4b routed and shared planes, the logits against mlx-lm's") @capture(& m)(t : T ?) {
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
            eval_(m, s, count_ids)
            var inscope walk <- greedy_eval_steps(m, s, 6)
            let text = decode(m.tok, walk)
            to_log(LOG_INFO, "35B-A3B greedy after the count: '{text}'\n")
            t |> equal(text, " 8, 9,", "the greedy walk continues the count")
            delete s
        }
    }
}

//! the routed snapshot's window against the CPU rails: the Metal
//! window's last row within the census bar of the CPU's (the
//! qwen35moe hybrid's deltanet drift under a 4-bit routed stack reads
//! 1.5-2.0 there, the one-step-off rows 10.9 and up)
let private MLX_MOE_BAR = 3.0

//! what the routed cell's Metal side keeps: the prompt, each window
//! leg's name and last row, the device's greedy ids; the CPU rails'
//! window rows and ids beside them
struct private RoutedKept {
    tag : string
    prompt : array<int64>
    nsteps : int
    legs : array<string>
    rows : array<array<float>>
    got : array<int64>
    cpu : array<float>
    cpu_short : array<float>
    want : array<int64>
    metal : bool
    cpu_ran : bool
}

//! one window leg of `routed_metal` over a fresh session: the routed
//! sites on the member's tensor twins (`base` false, the knee's pick)
//! or on its simdgroup base (the knee at 0 and the member's
//! `moe_mulmm_<f>` and `kq_mulmm_<f>` crowns pinned off - a crowned
//! family takes its twins at every shape), one device prefill whose
//! last row the leg keeps
def private routed_window_leg(t : T?; trg : Model; f : KqFmt; var k : RoutedKept; nstacks : int64; prev_crowns : string;
                              base : bool) : Session {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let tag = k.tag
        let leg = base ? "base" : "twins"
        set_metal_moe_kq_twin_avg(base ? 0l : 16l)
        if (base) {
            pin_metal_tensor_crowns(crowns_without(prev_crowns, ["moe_mulmm_{f}", "kq_mulmm_{f}"]))   //! the dense crown admits the routed twins too
            metal_prefill_shutdown()
        }
        var sg = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        let pf0 = metal_prefill_stats().prefills
        kernel_census_reset_metal()
        eval_(trg, sg, k.prompt)
        t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag} {leg}: the window was one prefill the device served")
        let nb = metal_kernel_count("metal_moe_mulmm_{f}")
        let nt = routed_twin_dispatches(f, "metal_moe_mulmm_b32_t", false)
        if (base) {
            t |> success(nb >= nstacks && nt == 0l, "{tag} {leg}: every routed site rode the {f} base tile ({nb} base, {nt} twin dispatches, {nstacks} stacks)")
        } else {
            t |> success(nt >= nstacks, "{tag} {leg}: every routed site rode the {f} tensor twins ({nt} twin, {nb} base dispatches, {nstacks} stacks)")
        }
        k.legs |> push(leg)
        k.rows |> push_clone(sg.logits)
        return <- sg
    } else {
        return <- make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
    }
}

//! The routed snapshot on the Metal form's twin `trg` over `text`'s
//! ids: the prefill window twice - the routed sites on the member's
//! tensor twins (the knee's pick), then on its simdgroup base (the
//! knee pinned to 0 and the member's routed and dense crowns pinned
//! off: a crowned family takes its twins at every shape) - each one
//! device prefill, then `nsteps` greedy device decodes from the base
//! leg's window, every step's routed sites on the member's
//! expert-indexed GEMV; the rows and ids kept for the CPU rails'
//! (`routed_cpu`). The base leg shuts the prefill driver down so its
//! pipelines compile under its crowns
[unused_argument(t, trg, f, text, nsteps, k)]
def private routed_metal(t : T?; var trg : Model; f : KqFmt; text : string; nsteps : int; var k : RoutedKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        k.metal = true
        k.nsteps = nsteps
        k.prompt <- encode(trg, text, true, false)
        let tag = k.tag
        let nstacks = 3l * trg.config.n_layers
        with_dasllama_jobque_() {
            with_metal_rails(MetalRails(decode = false)) {
                let knee = set_metal_moe_kq_twin_avg(16l)
                let prev_crowns = get_metal_tensor_crowns()
                let prev_pinned = metal_tensor_crowns_pinned()
                var sg <- routed_window_leg(t, trg, f, k, nstacks, prev_crowns, false)
                delete sg
                sg <- routed_window_leg(t, trg, f, k, nstacks, prev_crowns, true)
                if (prev_pinned) {
                    pin_metal_tensor_crowns(prev_crowns)
                } else {
                    unpin_metal_tensor_crowns()
                    set_metal_tensor_crowns(prev_crowns)
                }
                set_metal_moe_kq_twin_avg(knee)
                select_decode_override("metal")
                let de0 = metal_decode_stats().decodes
                kernel_census_reset_metal()
                k.got <- greedy_eval_steps(trg, sg, nsteps)
                t |> equal(metal_decode_stats().decodes - de0, int64(nsteps), "{tag}: every step was a device decode")
                let gv = routed_gemv_dispatches(f, kq_stem(f))
                t |> success(gv >= int64(nsteps) * nstacks, "{tag}: every step read every routed stack through the {f} routed stamps ({gv} stacks served, {int64(nsteps) * nstacks} stacks x steps)")
                routed_steps_form_cell(t, f, kq_stem(f), tag, int64(nsteps), trg.config.n_layers)
                delete sg
            }
        }
    }
}

//! the routed cell's CPU side on the CPU rails' model `tr`
//! (`mlx_cpu_form`), once: the window's last row over the kept prompt
//! and the prompt one token short, the greedy ids after it
def private routed_cpu(var tr : Model; var k : RoutedKept) {
    return if (!k.metal || k.cpu_ran)
    k.cpu_ran = true
    var inscope short <- [for (i in range(length(k.prompt) - 1)); k.prompt[i]]
    k.cpu <- window_last_logits(tr, k.prompt)
    k.cpu_short <- window_last_logits(tr, short)
    with_dasllama_jobque_() {
        var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        eval_(tr, sc, k.prompt)
        k.want <- greedy_eval_steps(tr, sc, k.nsteps)
        delete sc
    }
    log_gen_texts(tr, "{k.tag} Metal", k.want, k.got)
}

//! the routed cell's compares after the block: the CPU row of the
//! prompt one token short outside `MLX_MOE_BAR` (the control), each
//! window leg's last row within it of the CPU rails', the Metal
//! greedy ids the CPU rails' token for token; a Metal side whose CPU
//! side never ran a failure
def private routed_verdict(t : T?; var k : RoutedKept) {
    let tag = k.tag
    if (k.metal && !k.cpu_ran) {
        t |> failure("{tag}: the Metal legs ran and the CPU rails' side did not - the block gives no `mlx_cpu_form()`")
    } elif (k.cpu_ran) {
        let ctl = logits_maxdiff(k.cpu_short, k.cpu)
        t |> success(all_finite(k.cpu_short) && all_finite(k.cpu), "{tag}: every logit the control reads is finite")
        t |> success(ctl > MLX_MOE_BAR, "{tag}: the control - the prompt one token short lands outside the bar ({ctl})")
        for (leg, row in k.legs, k.rows) {
            let d = logits_maxdiff(row, k.cpu)
            to_log(LOG_INFO, "{tag} {leg}: window last row vs the CPU rails maxd {d} (bar {MLX_MOE_BAR})\n")
            t |> success(d <= MLX_MOE_BAR, "{tag} {leg}: the window's last row within {MLX_MOE_BAR} of the CPU rails' (maxd {d})")
        }
        t |> equal(k.got, k.want, "{tag}: the Metal greedy ids are the CPU rails' token for token")
    }
    delete k
}

//! the 35B-A3B snapshot on its Metal form, under the arm `metal-35b`:
//! the teacher-forced logits against mlx-lm's, then the routed window
//! and walk kept for the CPU rails'
def private q36_35b_metal_cells(t : T?; var m : Model; var k : RoutedKept) {
    return if (!arm_on(t, "metal-35b"))
    metal_teacher_forced(t, m, "35B-A3B", ORACLE_35B)
    k.tag = "metal-35b"
    routed_metal(t, m, KqFmt.mlx4b, counting_text(24), 8, k)
}

[test(description = "The Qwen3.6-35B-A3B MLX snapshot, large tier, reads its expert geometry off config.json, serves every routed and shared plane as mlx4b, holds 0.4 of mlx-lm's top-1 logits (the one-position-shifted control outside), and greedily continues a count; on the Metal home's blob twin, on a das_metal build, it holds 0.4 of mlx-lm's top-1 logits per device decode, its routed window on tensor twins and on the base tile lands within 3.0 of the CPU row, and 8 greedy steps match the CPU ids; on the blob twin every routed stack loads as mlx4b with every site native, and the Metal window and six steps ride the mlx4b routed stamps within 3.0 of the portable CPU chain's replay; each form loads once")]
def test_mlx_qwen36_35b_a3b_4bit(t : T?) {
    return if (!family_on(t, "qwen35moe") || full_suite_only(t, "the 35B-A3B snapshot"))
    let row = format_census_mlx4b_moe()
    var forms <- [cpu_cells_form()]
    var inscope arms <- ["metal-35b", row.arm]
    var inscope metal <- mlx_metal_forms(t, "mlx-community/Qwen3.6-35B-A3B-4bit", arms)
    forms |> push_from(metal)
    var k : RoutedKept
    var census : CensusKept
    with_cpu_and_gpu_snapshot(t, "llm", "mlx-community/Qwen3.6-35B-A3B-4bit", MLX_SNAPSHOT_CTX, forms) $(var m, form) {
        if (form.on_metal) {
            format_census_metal(t, m, form, row, census)
            q36_35b_metal_cells(t, m, k)
        } elif (alone_pass_of(form, cpu_cells_form())) {
            q36_35b_cpu_cell(t, m)
        } elif (alone_pass_of(form, mlx_cpu_form())) {
            routed_cpu(m, k)
            format_census_cpu(t, m, form, row, census)
        }
    }
    routed_verdict(t, k)
    format_census_verdict(t, row, census)
    delete forms
}
