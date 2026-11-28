options gen2
options _dasllama_internal = true

module _metal_matrix_cells shared public

//! The Metal support matrix's cells per model file: each path's
//! engage cells by weights and KV codec, the family carriers' token
//! and logits parity against the CPU, the knobs' legs. `_metal` runs
//! on the blob twin a block's `metal_form()` loads, `_cpu` on its
//! `matrix_cpu_form()` (the f32 scale's where the Metal form loaded
//! on it), `_verdict` after the block; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 - the public facade
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - eval_, argmax
require dasllama/dasllama_parity   // nolint:STYLE030,LINT019 - cached_ids, cached_vals
require dasllama/dasllama_ple   // nolint:STYLE030,LINT019 - ple_cpu_pre_rows
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - decode rails
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - prefill rails
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the census
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - residual lanes
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the das_metal halves
require math   // nolint:STYLE030,LINT019 - max, min, inside the das_metal halves
require strings   // nolint:STYLE030,LINT019 - starts_with, ends_with
require daslib/strings_boost   // nolint:STYLE030,LINT019 - split, the stamp lists
require _model_tier
require _model_form   // with_ctx_cut, own_load, ModelForm
require _metal_blob_twin   // nolint:STYLE030,LINT019 - the census readers, the seats, blob_twin
require _compares   // nolint:STYLE030,LINT019 - logits_maxdiff, span_maxdiff, argmax_of
require _format_census   // nolint:STYLE030,LINT019 - census_cell
require ../performance/model_specs.das   // nolint:STYLE030,LINT019 - counting_prompt
require _kernel_census   // nolint:STYLE029,STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves

//! the context every cell cuts its form's model to; each model file's
//! largest but the three below
let MATRIX_CTX = 512l
//! Llama-3.3-70B-Instruct-Q4_K_M.gguf's Metal form: its cells'
//! largest
let MATRIX_LLAMA_70B_CTX = 128l
//! gemma-3-1b-it-Q8_0.gguf's and gemma-4-E2B-it-Q8_0.gguf's Metal
//! forms: the ring row's 400-number prompt and its steps
let MATRIX_RING_CTX = 2048l

let private CONT_TEXT = "The old lighthouse keeper watched the storm roll in from the"
let private CHUNKED_STAMP = "MetalSqAttnPartF16_metal_sq_attn_part"

//! the form's weight-scale plane as a cell's tag names it
def private scale_tag(m : Model) : string => m.wscale_f16 ? "s16" : "s32"

//! whether a family row runs: its family and its `fam-` arm on
def private family_row_on(t : T?; family : string) : bool => (family_on(t, family)
    && arm_on(t, "fam-{family}"))

//! the cell family a form guard's failure names
let private MATRIX_CELLS = "support-matrix Metal"

//! a row's Metal side: where `on` and the form is the blob twin, the
//! side marked run and its scale kept, `body` on the form cut to
//! `MATRIX_CTX`
def private matrix_metal_side(t : T?; var m : Model; form : ModelForm; on : bool; file : string;
                              var kept : auto(K); body : block<void>) {
    return if (!on || !metal_form_held(t, form, MetalWant.blob_twin, file, MATRIX_CELLS))
    kept.metal = true
    kept.s32 = form.wscale_f32
    with_ctx_cut(m, MATRIX_CTX) {
        invoke(body)
    }
}

//! a row's CPU side: where its Metal side ran and this is the pass of
//! its `matrix_cpu_form` alone, the side marked run, `body` on the
//! form cut to `MATRIX_CTX`
def private matrix_cpu_side(var m : Model; form : ModelForm; var kept : auto(K);
                            body : block<void>) {
    return if (!kept.metal || !alone_pass_of(form, matrix_cpu_form(kept.s32)))
    kept.cpu = true
    with_ctx_cut(m, MATRIX_CTX) {
        invoke(body)
    }
}

//! the CPU form a Metal row's CPU side runs on: the portable pin, the
//! f32 weight scale where the row's Metal form loaded on it; a CPU
//! side acts on that form's pass alone, other passes pass by
def matrix_cpu_form(wscale_f32 : bool = false) : ModelForm =>
    cpu_cells_form(backend = "portable", wscale_f32 = wscale_f32)

//! a Metal row's forms added to `forms` where `on`: the blob twin of
//! `ctx` positions, `cpu` on a das_metal build
def metal_row_forms(var forms : array<ModelForm>; on : bool; ctx : int64; cpu : ModelForm) {
    return if (!on)
    forms |> push(metal_form(ctx = ctx))
    if (empty(metal_build_why())) {
        forms |> push(cpu)
    }
}

//! whether a verdict has both sides: false where the Metal side never
//! ran, a failure where it ran and the CPU side did not
def private both_sides(t : T?; tag : string; metal, cpu : bool) : bool {
    return false if (!metal)
    if (!cpu) {
        t |> failure(("{tag}: the Metal side ran and the CPU form's side never did (no " +
            "`matrix_cpu_form` pass)"))
        return false
    }
    return true
}

//! a generate pair's verdict: where both sides ran, the Metal tokens
//! the CPU truth's (`what` the assert)
def private gen_pair_verdict(t : T?; tag, what : string; var kept : auto(K)) {
    if (both_sides(t, tag, kept.metal, kept.cpu)) {
        t |> success(ids_equal(kept.gen.cpu, kept.gen.gpu), what)
    }
    delete kept
}

//! a generate of `n` positions on the Metal paths, prefill and decode
def private metal_generate(m : Model; prompt : array<int64>; n : int;
                           kv : KVDtype = KVDtype.f16) : array<int64> {
    var ids : array<int64>
    with_dasllama_jobque() {
        with_metal_rails(MetalRails()) {
            var s = make_run_state(m.config, kv, kv)
            ids <- generate(m, s, prompt, n)   // nolint:PERF030 - a fresh empty target, one move
            delete s
        }
    }
    return <- ids
}

//! a generate of `n` positions on the CPU form
def private cpu_generate(m : Model; prompt : array<int64>; n : int;
                         kv : KVDtype = KVDtype.f16) : array<int64> {
    var ids : array<int64>
    with_dasllama_jobque() {
        var s = make_run_state(m.config, kv, kv)
        ids <- generate(m, s, prompt, n)   // nolint:PERF030 - a fresh empty target, one move
        delete s
    }
    return <- ids
}

//! a pair of token streams: the CPU's and the Metal paths'
struct MatrixGenPair {
    cpu : array<int64>
    gpu : array<int64>
}

//! a path served `served` steps: it engaged on the GPU
def private engage_cell(t : T?; cell : string; served : int64) {
    t |> success(served > 0l, "{cell}: ENGAGES on the GPU ({served} served)")
}

//! one prefill, two decode steps and one B=2 batch step under the
//! Metal overrides, each path engaging (served > 0)
def private matrix_cell(t : T?; tr : Model; kdt : KVDtype;
                        tag : string) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        let pf0 = metal_prefill_count()
        let de0 = metal_decode_count()
        let ba0 = metal_batch_step_count()
        with_dasllama_jobque() {
            var sa = make_run_state(c, kdt, kdt)
            var sb = make_run_state(c, kdt, kdt)
            let pa <- encode(tr, counting_prompt(1, 40), true, false)
            let pb <- encode(tr, counting_prompt(5, 44), true, false)
            with_metal_rails(t, tag, MetalRails()) {
                eval_(tr, sa, pa)
                eval_(tr, sb, pb)
                var cura = argmax_of(sa.logits, vocab)
                let curb = argmax_of(sb.logits, vocab)
                for (_i in range(2)) {
                    var one <- [cura]
                    eval_(tr, sa, one)
                    delete one
                    cura = argmax_of(sa.logits, vocab)
                }
                var ws <- create_batch_workspace_(tr)
                var sess : array<Session?>
                unsafe {
                    sess |> push(addr(sa))
                    sess |> push(addr(sb))
                }
                var cur <- [cura, curb]
                eval_batch_(tr, ws, sess, cur)
                sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
                unsafe {
                    delete ws
                    delete sess
                }
                delete cur
            }
            delete sa
            delete sb
        }
        engage_cell(t, "{tag} prefill", metal_prefill_count() - pf0)
        engage_cell(t, "{tag} decode", metal_decode_count() - de0)
        engage_cell(t, "{tag} batch", metal_batch_step_count() - ba0)
    }
}

//! one B=nb batched step under the Metal decode override, the
//! sessions filled by the Metal prefill
def private dim_ladder_batch_cell(t : T?; tr : Model; nb : int;
                                  tag : string) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var served = 0l
        with_dasllama_jobque() {
            var ss : array<Session>
            ss |> reserve(nb)
            var sess : array<Session?>
            var cur : array<int64>
            with_metal_rails(MetalRails()) {
                for (i in range(nb)) {
                    ss |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                    var inscope p <- encode(tr, counting_prompt(1 + i, 14 + i), true, false)
                    eval_(tr, ss[i], p)
                    sess |> push(unsafe(addr(ss[i])))
                    cur |> push(argmax_of(ss[i].logits, vocab))
                }
                var ws <- create_batch_workspace_(tr)
                let ba0 = metal_batch_step_count()
                eval_batch_(tr, ws, sess, cur)
                served = metal_batch_step_count() - ba0
                delete ws
            }
            sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete sess
            }
            delete cur
            delete ss
        }
        t |> success(served > 0l, "{tag}: B={nb} batched step ENGAGES on the GPU ({served} served)")
    }
}

//! the derivation bits a family's carriers pin, as `uint` (MetalNeed
//! exists on das_metal builds alone)
def private family_needs(family : string) : uint {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let qwen3 = MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.qd_neq_dim
        let gemma = MetalNeed.neox | MetalNeed.sliding | MetalNeed.pre_post_norm | MetalNeed.geglu
        if (family == "qwen3" || family == "qwen3moe") {
            return uint(qwen3)
        } elif (family == "qwen2") {
            return uint(MetalNeed.neox | MetalNeed.qkv_bias)
        } elif (family == "phi3") {
            return uint(MetalNeed.neox)
        } elif (family == "gemma2") {
            return uint(gemma | MetalNeed.softcap)
        } elif (family == "gemma3") {
            return uint(gemma | MetalNeed.qk_norm | MetalNeed.qd_neq_dim)
        } elif (family == "qwen35" || family == "qwen35moe") {
            return uint(qwen3 | MetalNeed.partial_rope | MetalNeed.q_gated)
        }
    }
    return 0u
}

//! `metal_needs` on `m` derives at least the bits `want`
def private needs_cell(t : T?; m : Model; tag : string;
                       want : uint) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let derived = metal_needs(m)
        let expected = MetalNeed(want)   //! the uint relabelled so the message prints bit NAMES
        t |> success((uint(derived) & want) == want,
            "{tag}: metal_needs derives {expected} (got {derived})")
    }
}

//! what the continuation cell keeps: the Metal tail's served count
//! and last row, the CPU's whole-prompt row, the eyeball text
struct MatrixContKept {
    vocab : int64
    served : int64
    gpu : array<float>
    cpu : array<float>
    eye : string
}

//! the head and the two-token tail of the continuation prompt
def private cont_split(m : Model) : tuple<head : array<int64>; tail : array<int64>> {
    var inscope full <- encode(m, CONT_TEXT, true, false)
    let n = long_length(full)
    var r : tuple<head : array<int64>; tail : array<int64>>
    r.head <- [for (i in range64(n - 2l)); full[i]]
    r.tail <- [full[n - 2l], full[n - 1l]]
    return <- r
}

//! The Metal side: the head prefilled on the Metal paths, then the
//! tail as a continuation chunk (start_pos > 0), its last row kept
def private cont_metal(m : Model; var k : MatrixContKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope parts <- cont_split(m)
        k.vocab = m.config.vocab_size
        with_dasllama_jobque() {
            with_metal_rails(MetalRails()) {
                var sb = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                eval_(m, sb, parts.head)
                let pf0 = metal_prefill_count()
                eval_(m, sb, parts.tail)
                k.served = metal_prefill_count() - pf0
                k.gpu := sb.logits
                delete sb
            }
        }
    }
}

//! the CPU side: the whole prompt prefilled, its last row kept
def private cont_cpu(m : Model; var k : MatrixContKept) {
    var inscope parts <- cont_split(m)
    var inscope full <- clone_to_move(parts.head)
    full |> push_from(parts.tail)
    with_dasllama_jobque() {
        var sc = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, sc, full)
        k.cpu := sc.logits
        delete sc
    }
    let cpk = argmax_of(k.cpu, k.vocab)
    let gpk = argmax_of(k.gpu, k.vocab)
    k.eye = ("CPU next '{eyeball_str(piece_(m, cpk))}' vs GPU next " +
        "'{eyeball_str(piece_(m, gpk))}'")
}

//! continuation-prefill numeric parity within `tol`: a tolerance,
//! not tokens - near-tie picks flip under reduction-order noise
def private cont_verdict(t : T?; tag : string; tol : float; var k : MatrixContKept) {
    let d = logits_maxdiff(k.gpu, k.cpu)
    to_log(LOG_INFO, "{tag} continuation eyeball: {k.eye} (maxd {d})\n")
    t |> success(k.served > 0l, "{tag}: continuation prefill ENGAGED ({k.served} served)")
    t |> success(d < tol, "{tag}: continuation-prefill logits within {tol} of CPU (measured {d})")
    delete k.gpu
    delete k.cpu
}

//! what the depth cell keeps: the Metal side's picks fed to both
//! sides, the three measured steps' rows a side, the census text
struct MatrixDepthKept {
    picks : array<int64>
    gpu : array<array<float>>
    cpu : array<array<float>>
    census : string
    chunked : int64
}

struct private DepthLeg {
    fused : int64
    chunked : int64
}

typedef private DepthLegs = tuple<edge : DepthLeg; short : DepthLeg>

//! one Metal step on the session's own pick, the pick kept
def private depth_step(m : Model; var s : Session; var k : MatrixDepthKept) {
    let pick = argmax_of(s.logits, m.config.vocab_size)
    k.picks |> push(pick)
    var one <- [pick]
    eval_(m, s, one)
    delete one
}

//! the summed dispatches of `stamps` since the census reset
def private stamps_sum(stamps : array<string>) : int64 {
    var n = 0l
    for (stamp in stamps) {
        n += metal_kernel_count(stamp)
    }
    return n
}

//! the reach leg's three steps: the mirror left standing, the
//! tables' reach at the slice's very end, then a unit short of it
def private depth_reach_legs(t : T?; m : Model; var sg : Session; tag : string;
                             stamps : array<string>;
                             var k : MatrixDepthKept) : DepthLegs {   // nolint:LINT012,LINT019
    var r : DepthLegs
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let reach = get_metal_mirror_reach_probe_()
        t |> equal(metal_mirror_slice_units_(0xfffffffffffffffful), 0ul,
            "{tag} depth: a session id the driver never served has no mirror slice")
        for (leg in range(3)) {
            kernel_census_reset_metal()
            let units = metal_mirror_slice_units_(sg.uid)
            set_metal_mirror_reach_probe_(leg == 0 ? 0ul : leg == 1 ? units : units - 1ul)
            depth_step(m, sg, k)
            let here = DepthLeg(fused = stamps_sum(stamps),
                chunked = metal_kernel_count(CHUNKED_STAMP))
            if (leg == 1) {
                r.edge = here
            } elif (leg == 2) {
                r.short = here
            }
        }
        set_metal_mirror_reach_probe_(reach)
    }
    return r
}

//! the census of the three measured steps: every row on `stamps`,
//! none on the chunked pair; the reach legs' routes where run
def private depth_census(t : T?; tag : string; stamps : array<string>; reach_leg : bool;
                         legs : DepthLegs; var k : MatrixDepthKept) {
    var every = true
    k.census = build_string() $(var w) {
        for (stamp in stamps) {
            let n = metal_kernel_count(stamp)
            every = every && n > 0l
            w |> write(" {stamp}: {n}")
        }
    }
    k.chunked = metal_kernel_count(CHUNKED_STAMP)
    t |> success(every && k.chunked == 0l, ("{tag} depth: every attention row took the fused " +
        "stamps under DASLLAMA_METAL_ATTN_D pinned on ({k.census}; chunked: {k.chunked})"))
    if (reach_leg) {
        t |> success(legs.edge.fused > 0l && legs.edge.chunked == 0l, ("{tag} depth: a route " +
            "table that reaches the slice's very end keeps the single row on the fused stamps " +
            "({legs.edge.fused} fused, {legs.edge.chunked} chunked)"))
        t |> success(legs.short.chunked > 0l && legs.short.fused == 0l, ("{tag} depth: a route " +
            "table one unit short of the slice's end sends the single row to the chunked pair " +
            "({legs.short.chunked} chunked, {legs.short.fused} fused)"))
    }
}

//! The Metal side of decode past the single-dispatch ceiling: a
//! 120-number prompt, then three steps on the driver, their rows
//! kept, the census holding every row on the fused `stamps` (a model
//! of two head classes names both; a head off the 64 lattice the
//! narrow one)
def private depth_metal(t : T?; m : Model; tag : string; stamps : array<string>;
                        reach_leg : bool; var k : MatrixDepthKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompt <- encode(m, counting_prompt(1, 120), true, false)
        t |> success(long_length(prompt) > 224l, ("{tag} depth: the prompt " +
            "({long_length(prompt)} tokens) sits past the single-dispatch ceiling"))
        var legs : DepthLegs
        with_dasllama_jobque() {
            with_metal_rails(MetalRails()) {
                var sg = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                with_metal_attn_d(true) $ {
                    //! the census reads the fused stamps: the tile
                    //! form pinned off
                    with_metal_attn_tile_far(-1l) {
                        eval_(m, sg, prompt)
                        if (reach_leg) {
                            legs = depth_reach_legs(t, m, sg, tag, stamps, k)
                        }
                        kernel_census_reset_metal()
                        for (_i in range(3)) {
                            depth_step(m, sg, k)
                            k.gpu |> push_clone(sg.logits)
                        }
                    }
                }
                delete sg
            }
        }
        depth_census(t, tag, stamps, reach_leg, legs, k)
    }
}

//! the CPU side: the same prompt, the Metal side's picks fed in turn,
//! the last three steps' rows kept
def private depth_cpu(m : Model; var k : MatrixDepthKept) {
    var inscope prompt <- encode(m, counting_prompt(1, 120), true, false)
    let first = length(k.picks) - 3
    with_dasllama_jobque() {
        var sc = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, sc, prompt)
        for (pk, i in k.picks, count()) {
            var one <- [pk]
            eval_(m, sc, one)
            delete one
            if (i >= first) {
                k.cpu |> push_clone(sc.logits)
            }
        }
        delete sc
    }
}

//! the three forced steps' logits within `bar` of the CPU's
def private depth_verdict(t : T?; tag : string; bar : float; var k : MatrixDepthKept) {
    var maxd = length(k.cpu) == length(k.gpu) ? 0.0 : INFINITE_DIFF
    let picks = build_string() $(var w) {
        for (c, g in k.cpu, k.gpu) {
            maxd = max(maxd, logits_maxdiff(c, g))   //! a NaN on either side reads as infinite
            w |> write(" {argmax_of(c)}/{argmax_of(g)}")
        }
    }
    t |> success(maxd <= bar, ("{tag} depth: three forced steps' logits within {bar} of the " +
        "CPU's (maxd {maxd}; argmax cpu/gpu per step{picks})"))
    to_log(LOG_INFO, ("{tag} depth: maxd {maxd} (bar {bar}); argmax cpu/gpu per step{picks}; " +
        "census{k.census}, chunked {k.chunked}\n"))
    delete k
}

//! what the batched-step compare keeps: the Metal side's picks fed to
//! both stacks, both rows a side, the device's step and side rows
struct MatrixBatchKept {
    vocab : int64
    served : int64
    side_rows : int64   //!< the per-layer side rows projected on the device over the step
    cpu_pre_rows : int64   //!< the CPU pre-step rows built over the step
    cur : array<int64>
    ga : array<float>
    gb : array<float>
    ca : array<float>
    cb : array<float>
    eye : string
}

//! the per-layer side rows' projections the device ran: the bf16
//! word form's two stamps and the f32 router's
def private ple_proj_count : int64 => kernels_sum(["metal_ple_proj_bf16", "metal_ple_proj_bf16_one",
    "metal_moe_router"])

//! the summed census counts of `names`: a format's stamps at a site
def private kernels_sum(names : array<string>) : int64 {
    var n = 0l
    for (k in names) {
        n += metal_kernel_count_compiled(k)
    }
    return n
}

//! The Metal side: the two prompts prefilled on the Metal paths, the
//! picks off their rows, one batched step through the driver
def private batch_pair_metal(m : Model; pa, pb : array<int64>; var k : MatrixBatchKept) {
    let c = m.config
    k.vocab = c.vocab_size
    with_dasllama_jobque() {
        with_metal_rails(MetalRails()) {
            var sg = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sh = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(m, sg, pa)
            eval_(m, sh, pb)
            k.cur <- [argmax_of(sg.logits, k.vocab), argmax_of(sh.logits, k.vocab)]
            var wsg <- create_batch_workspace_(m)
            var sessg : array<Session?>
            unsafe {
                sessg |> push(addr(sg))
                sessg |> push(addr(sh))
            }
            let ba1 = metal_batch_step_count()
            let bpj0 = ple_proj_count()
            let bcpu0 = ple_cpu_pre_rows()
            eval_batch_(m, wsg, sessg, k.cur)
            k.served = metal_batch_step_count() - ba1
            k.side_rows = ple_proj_count() - bpj0
            k.cpu_pre_rows = ple_cpu_pre_rows() - bcpu0
            k.ga := sg.logits
            k.gb := sh.logits
            sessg |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete wsg
                delete sessg
            }
            delete sg
            delete sh
        }
    }
}

//! the CPU side: the same prompts on the CPU batched stack, the Metal
//! side's picks fed through one batched step
def private batch_pair_cpu(m : Model; pa, pb : array<int64>; var k : MatrixBatchKept) {
    let c = m.config
    with_dasllama_jobque() {
        var sc = make_run_state(c, KVDtype.f16, KVDtype.f16)
        var sd = make_run_state(c, KVDtype.f16, KVDtype.f16)
        eval_(m, sc, pa)
        eval_(m, sd, pb)
        var ws <- create_batch_workspace_(m)
        var sess : array<Session?>
        unsafe {
            sess |> push(addr(sc))
            sess |> push(addr(sd))
        }
        var cur := k.cur
        eval_batch_(m, ws, sess, cur)
        k.ca := sc.logits
        k.cb := sd.logits
        sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
        unsafe {
            delete ws
            delete sess
        }
        delete cur
        delete sc
        delete sd
    }
    let v = k.vocab
    k.eye = ("CPU next '{eyeball_str(piece_(m, argmax_of(k.ca, v)))}' / " +
        "'{eyeball_str(piece_(m, argmax_of(k.cb, v)))}' vs GPU next " +
        "'{eyeball_str(piece_(m, argmax_of(k.ga, v)))}' / " +
        "'{eyeball_str(piece_(m, argmax_of(k.gb, v)))}'")
}

//! the batched step's rows within `bar` of the CPU batched stack's,
//! and the cross-row control outside it; `shown` the bar as worded
def private batch_pair_verdict(t : T?; tag : string; bar : float; shown : string;
                               var k : MatrixBatchKept) {
    let v = int(k.vocab)
    let bmaxd = max(span_maxdiff(k.ga, k.ca, 0, 0, v).diff, span_maxdiff(k.gb, k.cb, 0, 0, v).diff)
    to_log(LOG_INFO, "{tag} batched step: {k.eye} (maxd {bmaxd})\n")
    t |> success(bmaxd <= bar, ("{tag}: batched-step logits within {shown} of the CPU batched " +
        "stack, both rows (maxd {bmaxd})"))
    let xmaxd = span_maxdiff(k.ga, k.cb, 0, 0, v).diff   //! the control: row A against row B
    t |> success(xmaxd > bar, "{tag}: the cross-row compare lands outside the bar (maxd {xmaxd})")
    delete k
}

//! what the Llama-3.2-1B Q8_0 rows keep: the window cell's counts a
//! side
struct MatrixLlama1bKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    blob_prefills : int64   //!< the blob short prompt's and continuation chunk's prefills
    start_pos : int64      //!< the retired start_pos clause's records, both sides
    npos_window : int64    //!< the planar short prompt's npos_window records
}

//! the weights' cells on the form's scale: on the f32 plane every KV
//! codec (`cells-q8`), on the f16 plane the f16 codec (`cells-s16`)
def private llama_scale_cells(t : T?; m : Model) {
    let s = scale_tag(m)
    if (!m.wscale_f16 && arm_on(t, "cells-q8")) {
        for (kdt in [KVDtype.f16, KVDtype.f32, KVDtype.q8_0, KVDtype.tq4]) {
            matrix_cell(t, m, kdt, "q8/{s}/{kdt}")
        }
    } elif (m.wscale_f16 && arm_on(t, "cells-s16")) {
        matrix_cell(t, m, KVDtype.f16, "q8/{s}/f16")
    }
}

//! The window's blob half: below the 64-row floor a blob short prompt
//! serves (no CPU fallback, the policy window collapsed) and a
//! continuation chunk (start_pos > 0) serves
def private window_blob(m : Model; var kept : MatrixLlama1bKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let sp0 = metal_prefill_decline_count("start_pos")
        let pf0 = metal_prefill_count()
        with_dasllama_jobque() {
            with_metal_rails(MetalRails(min_npos = 64l)) {
                var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                let short_p <- encode(m, counting_prompt(1, 12), true, false)
                let cont_p <- encode(m, counting_prompt(13, 26), false, false)
                eval_(m, s, short_p)
                set_metal_prefill_min_npos(1l)
                eval_(m, s, cont_p)
                delete s
            }
        }
        kept.blob_prefills = metal_prefill_count() - pf0
        kept.start_pos += metal_prefill_decline_count("start_pos") - sp0
    }
}

//! the non-causal media span on the blob twin: it engages with the
//! knob on, and with the knob off the declined span panics loudly
def private span_cells(t : T?; m : Model) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let ncs1 = metal_prefill_decline_count("non_causal_span")
        let pf1 = metal_prefill_count()
        span_eval(m)
        let ncs2 = metal_prefill_decline_count("non_causal_span")
        t |> success(ncs2 == ncs1, "a span no longer declines (the knob is on)")
        t |> success(metal_prefill_count() - pf1 == 1l, "the span call ENGAGES the metal prefill")
        let span0 = get_metal_prefill_span()
        set_metal_prefill_span(false)
        var span_off_tripped = false
        try {
            span_eval(m)
        } recover {
            span_off_tripped = true
        }
        set_metal_prefill_span(span0)
        t |> success(span_off_tripped, "knob off: the declined blob span panics loudly")
        t |> success(metal_prefill_decline_count("non_causal_span") > ncs2,
            "knob off: the non_causal_span decline is recorded")
    }
}

//! one 12-row non-causal span prefilled on the Metal paths
def private span_eval(m : Model) {
    with_dasllama_jobque() {
        with_metal_rails(MetalRails()) {
            var sp = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            var rows : array<float>
            rows |> reserve(12l * m.config.dim)
            rows |> resize(12l * m.config.dim)
            eval_embd(m, sp, rows, 12l, true)
            delete rows
            delete sp
        }
    }
}

//! a 12-number prompt and one step on `m` under the mode now set
def private mode_walk(m : Model) {
    with_dasllama_jobque() {
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        let pr <- encode(m, counting_prompt(1, 12), true, false)
        eval_(m, s, pr)
        var one <- [argmax_of(s.logits, m.config.vocab_size)]
        eval_(m, s, one)
        delete one
        delete s
    }
}

//! The MetalMode knob's blob legs: when_available selects the
//! overrides and serves; required serves everything, the short
//! prompt included; the mode found put back
def private mode_blob_cells(t : T?; m : Model) {
    let seat = metal_home_seat()
    set_metal_mode(MetalMode.when_available)
    t |> success(active_decode_override() == "metal",
        "mode when_available: metal overrides selected")
    let de_avail = metal_decode_count()
    mode_walk(m)
    t |> success(metal_decode_count() > de_avail, "mode when_available: decode serves on the GPU")
    set_metal_mode(MetalMode.required)
    let de_req = metal_decode_count()
    let pf_req = metal_prefill_count()
    mode_walk(m)
    t |> success(metal_decode_count() > de_req, "mode required: decode serves on the GPU")
    t |> success(metal_prefill_count() > pf_req,
        "mode required: the short prefill serves too (the npos window collapsed on blob)")
    restore_metal_home(seat)
}

//! the Llama-3.2-1B-Instruct-Q8_0.gguf rows on the blob twin: the
//! weights' cells of its scale; on the default scale the window's
//! blob half and the span, the mode's blob legs
[cold_path]
def matrix_llama_1b_metal(t : T?; var m : Model; form : ModelForm;
                          var kept : MatrixLlama1bKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!family_on(t, "llama") || !metal_form_held(t, form, MetalWant.blob_twin,
                   "Llama-3.2-1B-Instruct-Q8_0.gguf", MATRIX_CELLS))
        with_ctx_cut(m, MATRIX_CTX) {
            llama_scale_cells(t, m)
            if (!form.wscale_f32) {
                kept.metal = true
                if (arm_on(t, "window")) {
                    window_blob(m, kept)
                    span_cells(t, m)
                }
                if (arm_on(t, "mode")) {
                    mode_blob_cells(t, m)
                }
            }
            metal_decode_stage_report()
        }
    }
}

//! The CPU form's legs of the Llama-3.2-1B Q8_0 rows: the planar
//! short prompt under the Metal prefill records npos_window;
//! MetalMode off deselects the overrides and serves nothing on the
//! GPU
[cold_path]
def matrix_llama_1b_cpu(t : T?; var m : Model; form : ModelForm; var kept : MatrixLlama1bKept) {
    matrix_cpu_side(m, form, kept) {
        if (arm_on(t, "window")) {
            let nw0 = metal_prefill_decline_count("npos_window")
            let sp0 = metal_prefill_decline_count("start_pos")
            with_dasllama_jobque() {
                with_metal_rails(MetalRails(min_npos = 64l)) {
                    var sp = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                    let short_p <- encode(m, counting_prompt(1, 12), true, false)
                    eval_(m, sp, short_p)
                    delete sp
                }
            }
            kept.npos_window = metal_prefill_decline_count("npos_window") - nw0
            kept.start_pos += metal_prefill_decline_count("start_pos") - sp0
        }
        if (arm_on(t, "mode")) {
            let seat = metal_home_seat()
            let de_off = metal_decode_count()
            set_metal_mode(MetalMode.off)
            t |> success(active_decode_override() == "", "mode off: overrides deselected")
            mode_walk(m)
            t |> success(metal_decode_count() == de_off, "mode off: nothing served on the GPU")
            restore_metal_home(seat)
        }
    }
}

//! the window cell's compares across the two sides
def matrix_llama_1b_verdict(t : T?; var kept : MatrixLlama1bKept) {
    if (both_sides(t, "Llama-3.2-1B Q8_0", kept.metal, kept.cpu) && arm_on(t, "window")) {
        t |> success(kept.npos_window > 0l,
            "planar short prompt below min_npos declines npos_window")
        t |> success(kept.blob_prefills == 2l,
            "blob short prompt + continuation chunk both ENGAGE (the npos window is planar-only)")
        t |> success(kept.start_pos == 0l, "the retired start_pos clause records nothing")
    }
    delete kept
}

//! what the K-quant row keeps: both sides' generates
struct MatrixLlamaKqKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    gen : MatrixGenPair
}

//! MetalMode.required on the kq twin: the prefill, single decode and
//! batched step serve, a continuation chunk serves, and the
//! legacy-GEMM pin (a `planar` capability decline) panics
def private kq_required_cells(t : T?; m : Model;
                              prompt : array<int64>) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let seat = metal_home_seat()
        let vocab = m.config.vocab_size
        with_dasllama_jobque() {
            var sa = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            set_metal_mode(MetalMode.required)
            set_metal_prefill_min_npos(1l)
            let pf0 = metal_prefill_count()
            eval_(m, sa, prompt)
            eval_(m, sb, prompt)
            t |> success(metal_prefill_count() - pf0 > 0l,
                "required: kq prefill serves on the GPU (M2.3 mul_mm twins)")
            let de0 = metal_decode_count()
            var one <- [argmax_of(sa.logits, vocab)]
            eval_(m, sa, one)
            t |> success(metal_decode_count() - de0 > 0l,
                "required: kq single-stream decode serves on the GPU")
            var ws <- create_batch_workspace_(m)
            var sess : array<Session?>
            unsafe {
                sess |> push(addr(sa))
                sess |> push(addr(sb))
            }
            var cur <- [argmax_of(sa.logits, vocab), argmax_of(sb.logits, vocab)]
            let ba0 = metal_batch_step_count()
            eval_batch_(m, ws, sess, cur)
            t |> success(metal_batch_step_count() - ba0 > 0l,
                "required: kq batched step serves on the GPU (M2.1 per-stream form)")
            kq_required_chunks(t, m, prompt)
            sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete one
            delete sa
            delete sb
        }
        restore_metal_home(seat)
    }
}

//! under required: a continuation chunk serves (M2.5), and the
//! legacy-GEMM pin's capability decline panics instead of falling
//! back
def private kq_required_chunks(t : T?; m : Model;
                               prompt : array<int64>) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var sc = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        let pfc0 = metal_prefill_count()
        eval_(m, sc, prompt)   //! chunk 1: fresh-start
        eval_(m, sc, prompt)   //! chunk 2: start_pos = n_past > 0
        t |> success(metal_prefill_count() - pfc0 == 2l,
            "required: continuation prefill (start_pos > 0) serves on the GPU (M2.5)")
        let prior_legacy = get_metal_prefill_mulmm_legacy()
        set_metal_prefill_mulmm_legacy(true)
        var sd = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        var pf2_panicked = false
        try {
            eval_(m, sd, prompt)
        } recover {
            pf2_panicked = true
        }
        set_metal_prefill_mulmm_legacy(prior_legacy)
        t |> success(pf2_panicked, ("required: the legacy-GEMM pin (a `planar` capability " +
            "decline) panics instead of CPU fallback"))
        delete sd
        delete sc
    }
}

//! the Llama-3.2-1B-Instruct-Q4_K_M.gguf row on the blob twin: native
//! K-quant planes, the cells, the generate, MetalMode.required
[cold_path]
def matrix_llama_1b_kq_metal(t : T?; var m : Model; form : ModelForm;
                             var kept : MatrixLlamaKqKept) {
    matrix_metal_side(t, m, form, family_on(t, "llama") && arm_on(t, "kq"),
            "Llama-3.2-1B-Instruct-Q4_K_M.gguf", kept) {
        t |> success(m.kquant_native, "Q4_K_M under a q8-mode load keeps native K-quant planes")
        let s = scale_tag(m)
        matrix_cell(t, m, KVDtype.f16, "k4/{s}/f16")
        matrix_cell(t, m, KVDtype.q8_0, "k4/{s}/q8_0")
        let kprompt <- encode(m, counting_prompt(1, 30), true, false)
        kept.gen.gpu <- metal_generate(m, kprompt, length(kprompt) + 8)
        kq_required_cells(t, m, kprompt)
    }
}

//! the K-quant row's CPU truth: the generate on the CPU form
[cold_path, unused_argument(t)]
def matrix_llama_1b_kq_cpu(t : T?; var m : Model; form : ModelForm; var kept : MatrixLlamaKqKept) {
    matrix_cpu_side(m, form, kept) {
        let kprompt <- encode(m, counting_prompt(1, 30), true, false)
        kept.gen.cpu <- cpu_generate(m, kprompt, length(kprompt) + 8)
    }
}

//! the GPU kq decode tokens against the CPU truth
def matrix_llama_1b_kq_verdict(t : T?; var kept : MatrixLlamaKqKept) {
    gen_pair_verdict(t, "Llama-3.2-1B Q4_K_M",
        "K-quant continuation with metal overrides ON matches the CPU truth (guard verified)", kept)
}

//! the parameters of one dim-ladder row
struct MatrixDimRow {
    file : string
    arm : string
    tag : string         //!< the matrix cell's weights stem
    nb : int             //!< the batched step's width
    ladder_tag : string
}

//! a dim-ladder row on the blob twin: the matrix cell and a B=`nb`
//! batched step past the batch dim caps
[cold_path]
def matrix_dim_metal(t : T?; var m : Model; form : ModelForm; p : MatrixDimRow) {
    return if (!family_on(t, "llama") || !arm_on(t, p.arm)
               || !metal_form_held(t, form, MetalWant.blob_twin, p.file, MATRIX_CELLS))
    with_ctx_cut(m, MATRIX_CTX) {
        matrix_cell(t, m, KVDtype.f16, "{p.tag}/{scale_tag(m)}/f16")
        dim_ladder_batch_cell(t, m, p.nb, p.ladder_tag)
    }
}

//! dim 4096 at q8: the GEMM tier past B=4
def matrix_dim_8b() : MatrixDimRow => MatrixDimRow(file = "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf",
    arm = "dim8b", tag = "8B-q8", nb = 8, ladder_tag = "8B dim4096")

//! dim 8192 at K-quant: the kq mul_mm twin; its form
//! `MATRIX_LLAMA_70B_CTX`
def matrix_dim_70b() : MatrixDimRow => MatrixDimRow(file = "Llama-3.3-70B-Instruct-Q4_K_M.gguf",
    arm = "dim70b", tag = "70B-kq", nb = 12, ladder_tag = "70B dim8192")

//! the parameters of one family carrier's row
struct MatrixFamilyRow {
    file : string
    family : string
    tag : string
    kv2 : KVDtype = KVDtype.f16   //!< past f16: a second cell row on that KV codec
    cont_tol : float               //!< past 0: the continuation-prefill cell's bar
    s16_tag : string   //!< the tag on an f16-scale form where it differs; that row has no kv2
}

//! what a family row keeps: its tag on the form, the generates, the
//! continuation and depth cells' rows
struct MatrixFamilyKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    tag : string
    gen : MatrixGenPair
    cont : MatrixContKept
    depth : MatrixDepthKept
}

//! the head-96 depth cell's narrow fused stamp and its bar: twice the
//! K-quant row's reading
let private HEAD96_STAMP = "MetalSqAttnDF16_metal_sq_attn_d"
let private HEAD96_BAR = 4.0

//! the metal-overrides generate with its witnesses: the greedy chain
//! on the fp32 token table, the device pair's tail stamp past 64
def private family_gen_metal(t : T?; m : Model; tag : string;
                             var gpu : array<int64>) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        kernel_census_reset_metal()
        let mirror_before = metal_prefill_mirror_served()
        let spec_before = metal_decode_spec_stats().hits
        delete gpu
        gpu <- metal_generate(m, prompt, length(prompt) + 8)
        if (!m.emb_q8 && !cls_kq(m) && !m.planes_trimmed) {
            let hits = metal_decode_spec_stats().hits - spec_before
            t |> success(hits > 0l,
                "{tag}: the greedy chain ran on the fp32 token table ({hits} hits)")
        }
        if (m.config.head_size % 64l != 0l) {
            if (metal_prefill_mirror_served() > mirror_before) {
                let tails = (metal_kernel_count("MetalAttnAVDevTail_metal_attn_av_dev")
                    + metal_kernel_count("MetalAttnAVDev128Tail_metal_attn_av_dev"))
                t |> success(tails > 0l, ("{tag}: the device pair's tail stamp served the head's " +
                    "columns past its 64-wide tiles ({tails} dispatches)"))
            } else {
                t |> skip(
                    ("{tag}: the mirror-fed window did not serve (no attn_dev crown on this " +
                    "box) - the tail stamp's witness stands down"))
            }
        }
    }
}

//! the row's tag and second KV codec on the form's scale
def private family_shape(p : MatrixFamilyRow; m : Model) : tuple<tag : string; kv2 : KVDtype> {
    return (tag = p.s16_tag, kv2 = KVDtype.f16) if (m.wscale_f16 && !empty(p.s16_tag))
    return (tag = p.tag, kv2 = p.kv2)
}

//! the family row's cells on the blob twin
def private family_metal_cells(t : T?; m : Model; p : MatrixFamilyRow;
                               var kept : MatrixFamilyKept) {
    let shape = family_shape(p, m)
    let tag = shape.tag
    kept.tag = tag
    needs_cell(t, m, tag, family_needs(p.family))
    matrix_cell(t, m, KVDtype.f16, tag)
    if (shape.kv2 != KVDtype.f16) {
        matrix_cell(t, m, shape.kv2, "{tag}+{shape.kv2}")
    }
    family_gen_metal(t, m, tag, kept.gen.gpu)
    if (p.cont_tol > 0.0) {
        cont_metal(m, kept.cont)
    }
    if (m.config.head_size == 96l) {
        //! a head off the 64 lattice: the single row past the ceiling
        //! takes the narrow fused stamp
        depth_metal(t, m, tag, [HEAD96_STAMP], false, kept.depth)
    }
}

//! A family carrier's row on the blob twin: the needs bits, the path
//! cells (a second row on `kv2`), the metal-overrides generate, the
//! continuation and depth cells' Metal sides
[cold_path]
def matrix_family_metal(t : T?; var m : Model; form : ModelForm; p : MatrixFamilyRow;
                        var kept : MatrixFamilyKept) {
    matrix_metal_side(t, m, form, family_row_on(t, p.family), p.file, kept) {
        family_metal_cells(t, m, p, kept)
    }
}

//! the family row's CPU side: the generate truth, the continuation
//! and depth cells' rows
[cold_path, unused_argument(t)]
def matrix_family_cpu(t : T?; var m : Model; form : ModelForm; p : MatrixFamilyRow;
                      var kept : MatrixFamilyKept) {
    matrix_cpu_side(m, form, kept) {
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        kept.gen.cpu <- cpu_generate(m, prompt, length(prompt) + 8)
        log_gen_texts(m, kept.tag, kept.gen.cpu, kept.gen.gpu)
        if (p.cont_tol > 0.0) {
            cont_cpu(m, kept.cont)
        }
        if (!empty(kept.depth.picks)) {
            depth_cpu(m, kept.depth)
        }
    }
}

//! the family row's compares: the generate token for token, the
//! continuation within `cont_tol`, the depth steps within the bar
def matrix_family_verdict(t : T?; p : MatrixFamilyRow; var kept : MatrixFamilyKept) {
    if (both_sides(t, kept.tag, kept.metal, kept.cpu)) {
        t |> success(ids_equal(kept.gen.cpu, kept.gen.gpu),
            "{kept.tag}: metal-overrides generate matches the CPU truth token-for-token")
        if (p.cont_tol > 0.0) {
            cont_verdict(t, kept.tag, p.cont_tol, kept.cont)
        }
        if (!empty(kept.depth.picks)) {
            depth_verdict(t, kept.tag, HEAD96_BAR, kept.depth)
        }
    }
    delete kept
}

//! qwen3 = NEOX + QK-norm + qd != dim; the tq4 row proves the tq4
//! rope-store's NEOX partner
def matrix_row_qwen3_0_6b() : MatrixFamilyRow => MatrixFamilyRow(file = "Qwen3-0.6B-Q8_0.gguf",
    family = "qwen3", tag = "qwen3-0.6b/q8/f16", kv2 = KVDtype.tq4, cont_tol = 4.0)

//! k4/k6 mixed sites x qk_norm x qd != dim at 4B scale
def matrix_row_qwen3_4b_k4() : MatrixFamilyRow => MatrixFamilyRow(
    file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", family = "qwen3", tag = "qwen3-4b/k4/f16")

//! qwen2 = NEOX + QKV bias at hs 64, q8_0-KV bias; the f16-scale form
//! drives the fused qkv_rs NEOX+bias epilogue
def matrix_row_qwen25_0_5b() : MatrixFamilyRow => MatrixFamilyRow(
    file = "Qwen2.5-0.5B-Instruct-Q8_0.gguf", family = "qwen2", tag = "qwen2.5-0.5b/q8/f16",
    kv2 = KVDtype.q8_0, s16_tag = "qwen2.5-0.5b/q8/s16")

//! the hs-128 bias geometry
def matrix_row_qwen25_1_5b() : MatrixFamilyRow => MatrixFamilyRow(
    file = "Qwen2.5-1.5B-Instruct-Q8_0.gguf", family = "qwen2", tag = "qwen2.5-1.5b/q8/f16")

//! phi3 = NEOX only, head 96; the q8_0-KV rope-store's NEOX block
//! loop
def matrix_row_phi35_k4() : MatrixFamilyRow => MatrixFamilyRow(
    file = "Phi-3.5-mini-instruct-Q4_K_M.gguf", family = "phi3", tag = "phi3.5-mini/k4/f16",
    kv2 = KVDtype.q8_0)

//! the fused qkv_rs NEOX mapping on the f16 scale plane
def matrix_row_phi35_q8() : MatrixFamilyRow => MatrixFamilyRow(
    file = "Phi-3.5-mini-instruct-Q8_0.gguf", family = "phi3", tag = "phi3.5-mini/q8/s16")

//! gemma2: K-quant sites x wide heads x q8_0-KV block attention
def matrix_row_gemma2_2b_k4() : MatrixFamilyRow => MatrixFamilyRow(
    file = "gemma-2-2b-it-Q4_K_M.gguf", family = "gemma2", tag = "gemma2-2b/k4/f16",
    kv2 = KVDtype.q8_0, cont_tol = 4.0)

//! the s16 GEMVs with the swiglu-fused w13 form standing down to the
//! geglu ew
def matrix_row_gemma2_2b_q8() : MatrixFamilyRow => MatrixFamilyRow(
    file = "gemma-2-2b-it-Q8_0.gguf", family = "gemma2", tag = "gemma2-2b/q8/s16")

//! gemma3's dual-theta per-layer rope; the q8_0-KV row drives the
//! hs-256 QK-norm prepass. Its form `MATRIX_RING_CTX`
def matrix_row_gemma3_1b() : MatrixFamilyRow => MatrixFamilyRow(file = "gemma-3-1b-it-Q8_0.gguf",
    family = "gemma3", tag = "gemma3-1b/q8/f16", kv2 = KVDtype.q8_0, cont_tol = 4.0)

def matrix_row_gemma3_4b() : MatrixFamilyRow => MatrixFamilyRow(file = "gemma-3-4b-it-Q8_0.gguf",
    family = "gemma3", tag = "gemma3-4b/q8/f16")

//! the K-quant carrier: k4 / k6 planes, the Q6_K token table
def matrix_row_gemma3_12b_k4() : MatrixFamilyRow => MatrixFamilyRow(
    file = "gemma-3-12b-it-Q4_K_M.gguf", family = "gemma3", tag = "gemma3-12b/k4/f16")

//! qwen3 attention + GPU MoE routing, the whole file: its generate
//! holds token parity
def matrix_row_qwen3moe_30b() : MatrixFamilyRow => MatrixFamilyRow(
    file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", family = "qwen3moe",
    tag = "qwen3moe-30b/k4/f16", cont_tol = 6.0)

let private GEMMA2_WINDOW = 160l   //! >= 128, the gate's single-form floor

//! what the window row keeps: both sides' window-crossing generates
struct MatrixWindowKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    gen : MatrixGenPair
}

//! `body` with `m`'s sliding window shrunk to 160, put back after
def private with_window(var m : Model; body : block<void>) {
    let w0 = m.config.sliding_window
    m.config.sliding_window = GEMMA2_WINDOW
    invoke(body)
    m.config.sliding_window = w0
}

//! The window row's Metal side: the window shrunk, a generate past
//! it on the fused attention; the prompt depth crosses chunk 224 so
//! whole below-window chunks are skipped
def private window_metal_cells(t : T?; var m : Model;
                               var kept : MatrixWindowKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prompt <- encode(m, counting_prompt(1, 120), true, false)
        t |> success(long_length(prompt) > 224l,
            "gemma2-2b/window: prompt depth {long_length(prompt)} crosses chunk 224")
        let de0 = metal_decode_count()
        let pf0 = metal_prefill_count()
        with_window(m) {
            with_metal_attn_d(true) $ {
                kernel_census_reset_metal()
                kept.gen.gpu <- metal_generate(m, prompt, length(prompt) + 16)
            }
        }
        let served_de = metal_decode_count() - de0
        let served_pf = metal_prefill_count() - pf0
        t |> success(served_de > 0l && served_pf > 0l,
            "gemma2-2b/window: both GPU paths engaged (decode {served_de}, prefill {served_pf})")
        let wide = metal_kernel_count("MetalSqAttnDWF16_metal_sq_attn_d")
        let chunked = metal_kernel_count(CHUNKED_STAMP)
        t |> success(wide > 0l && chunked == 0l, ("gemma2-2b/window: the capped rows past the " +
            "single-dispatch ceiling took the fused attention under DASLLAMA_METAL_ATTN_D pinned " +
            "on (wide stamp {wide}, chunked {chunked})"))
    }
}

//! the gemma-2-2b-it-Q4_K_M.gguf window row on the blob twin
[cold_path]
def matrix_gemma2_window_metal(t : T?; var m : Model; form : ModelForm;
                               var kept : MatrixWindowKept) {
    matrix_metal_side(t, m, form, family_row_on(t, "gemma2"), "gemma-2-2b-it-Q4_K_M.gguf",
            kept) {
        window_metal_cells(t, m, kept)
    }
}

//! the window row's CPU truth: the same shrunk window, the generate
[cold_path, unused_argument(t)]
def matrix_gemma2_window_cpu(t : T?; var m : Model; form : ModelForm; var kept : MatrixWindowKept) {
    matrix_cpu_side(m, form, kept) {
        let prompt <- encode(m, counting_prompt(1, 120), true, false)
        with_window(m) {
            kept.gen.cpu <- cpu_generate(m, prompt, length(prompt) + 16)
        }
        log_gen_texts(m, "gemma2-2b/window", kept.gen.cpu, kept.gen.gpu)
    }
}

//! the window-crossing generates token for token
def matrix_gemma2_window_verdict(t : T?; var kept : MatrixWindowKept) {
    gen_pair_verdict(t, "gemma2-2b/window",
        "gemma2-2b/window: window-crossing generate matches the CPU truth token-for-token", kept)
}

//! the parameters of one ring row
struct MatrixRingRow {
    file : string
    tag : string
    family : string
    arm : string
}

//! `prompt` evaluated `chunk` rows a call, then `steps` greedy picks
//! each fed back: the prompt's ids and the picks
def private ring_row_walk(t : Model; var s : Session; prompt : array<int64>; chunk : int64;
                          steps : int) : array<int64> {
    var out := prompt
    eval_chunked(t, s, prompt, long_length(prompt), chunk)
    out |> reserve(length(out) + steps)
    for (_i in range(steps)) {
        let pick = argmax_of(s.logits, t.config.vocab_size)
        out |> push(pick)
        var one <- [pick]
        eval_(t, s, one)
        delete one
    }
    return <- out
}

//! what the ring row reads a leg: the walk, its rings, its windows
struct private RingLegs {
    ids : array<array<int64>>
    rings : array<int64>
    fed : array<int64>   //!< each leg's windows on the mirror-fed form
}

//! one leg of the ring row under the ring pinned to `ringed`
def private ring_leg(t : T?; m : Model; tag : string; prompt : array<int64>; ringed : int;
                     var legs : RingLegs) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_kv_ring(ringed) {
            let de0 = metal_decode_count()
            let pf0 = metal_prefill_count()
            let mf0 = metal_prefill_mirror_served()
            var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            var got <- ring_row_walk(m, s, prompt, 256l, 24)
            legs.fed |> push(metal_prefill_mirror_served() - mf0)
            legs.rings |> push(mirror_rings(m))
            if (ringed == 1) {
                let ring = mirror_ring(m, 0l)
                t |> success(ring > 0l && ring < long_length(prompt),
                    "{tag}/ring: the sliding layers' {ring}-row ring wraps under the prompt")
            }
            t |> success(metal_decode_count() - de0 >= 23l
                         && metal_prefill_count() - pf0 >= long_length(prompt) / 256l,
                "{tag}/ring {ringed}: the prompt and the steps served on the device")
            legs.ids |> emplace(got)
            delete s
        }
    }
}

//! The sliding layers' rings against every row kept on the Metal
//! mirror (`../ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`): a
//! prompt past the window in 256-row windows and 24 greedy steps,
//! ring pinned on, then off, token for token; the ringed layout holds
//! rings, the flat one none, and feeds every window the flat one
//! feeds
def private ring_cells(t : T?; m : Model; tag : string) {
    let prompt <- encode(m, counting_prompt(1, 400), true, false)
    t |> success(long_length(prompt) > m.config.sliding_window + 128l, ("{tag}/ring: the " +
        "{long_length(prompt)}-token prompt runs past the {m.config.sliding_window}-position " +
        "window"))
    var inscope legs : RingLegs
    with_dasllama_jobque() {
        with_metal_rails(MetalRails(min_npos = 0l)) {
            for (ringed in fixed_array(1, 0)) {
                ring_leg(t, m, tag, prompt, ringed, legs)
            }
        }
    }
    t |> success(legs.rings[0] > 0l && legs.rings[1] == 0l,
        ("{tag}/ring: the ringed layout holds " +
        "rings ({legs.rings[0]} elements), the flat one none ({legs.rings[1]})"))
    t |> success(legs.fed[1] > 0l && legs.fed[0] == legs.fed[1], ("{tag}/ring: every window the " +
        "flat mirror feeds, the ringed one feeds too ({legs.fed[0]} ringed, {legs.fed[1]} flat)"))
    log_gen_texts(m, "{tag}/ring", legs.ids[1], legs.ids[0])
    t |> success(ids_equal(legs.ids[0], legs.ids[1]),
        "{tag}/ring: the ringed mirror generates the flat mirror's tokens")
}

//! a ring row on the blob twin of `MATRIX_RING_CTX` positions
[cold_path]
def matrix_ring_metal(t : T?; var m : Model; form : ModelForm; p : MatrixRingRow) {
    return if (!family_on(t, p.family) || !arm_on(t, p.arm)
               || !metal_form_held(t, form, MetalWant.blob_twin, p.file, MATRIX_CELLS))
    if (m.config.seq_len < MATRIX_RING_CTX) {
        t |> failure(("{p.file}: the ring row needs a form of {MATRIX_RING_CTX} positions " +
            "({m.config.seq_len})"))
        return
    }
    with_ctx_cut(m, MATRIX_RING_CTX) {
        ring_cells(t, m, p.tag)
    }
}

def matrix_ring_gemma3_1b() : MatrixRingRow => MatrixRingRow(file = "gemma-3-1b-it-Q8_0.gguf",
    tag = "gemma3-1b", family = "gemma3", arm = "fam-gemma3")

def matrix_ring_gemma4_e2b() : MatrixRingRow => MatrixRingRow(file = "gemma-4-E2B-it-Q8_0.gguf",
    tag = "gemma4e-e2b", family = "gemma4e", arm = "fam-gemma4e")

//! the 26B cuts' bar, a share of the CPU truth row's peak: the six
//! cuts' rows read 0.012 to 0.029 of a 270-1140 peak after the
//! prefill and on the forced step, the rows one step off 0.89 and
//! past
let private CUT_ROW_BAR_REL = 0.06
let private G4_STEP_PF = "metal_moe_comb_g4_pf"   //! the MoE step's window and decode stamps
let private G4_STEP_DEC = "metal_moe_comb_g4"

//! how a MoE row reads `tol_pf` and `tol_step`: as whole logits, or
//! as shares of the CPU truth row's largest live logit
enum MoeRowBar {
    logits
    peak_share
}

//! the parameters of one gemma4moe row; `gemvs` the comma list of the
//! routed stamps the file's expert stacks name
struct MatrixMoeRow {
    file : string
    tag : string
    tol_pf : float = 8.0
    tol_step : float = 16.0
    gemvs : string
    mx4 : bool
    bars : MoeRowBar = MoeRowBar.logits
}

//! one leg's Metal reads: the post-prefill and forced-step rows, the
//! served count, the picks, the MoE step counts
struct MatrixMoeLeg {
    pf : array<float>
    step : array<float>
    served : int64
    gpk0 : int64
    gpk1 : int64
    npf : int64
    ndec : int64
    eye : string
}

//! what a MoE row keeps: the forced token, the legs' Metal rows, the
//! CPU truths on f16 and q8_0 sessions
struct MatrixMoeKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    vocab : int64
    nl : int64
    t0 : int64   //!< the forced step's token: the first leg's Metal pick
    t1 : int64   //!< the CPU truth's pick after the forced step
    legs : array<MatrixMoeLeg>
    lg0 : array<float>
    lg1 : array<float>
    lq0 : array<float>
    lq1 : array<float>
}

//! the routed GEMVs' census counts that rose, logged as one line
def private routed_gemv_counts : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope cov <- metal_kernel_coverage()
        return build_string() $(var w) {
            for (name, n in keys(cov), values(cov)) {
                if (n > 0l && starts_with(name, "metal_moe_gemv")) {
                    w |> write(" {name}: {n}")
                }
            }
        }
    } else {
        return ""
    }
}

//! the row's routed stamps as a list
def private gemv_list(p : MatrixMoeRow) : array<string> =>
    <- [for (g in split(p.gemvs, ",")); g; where !empty(g)]

//! one leg's walk on the blob twin: prefill, the forced step on the
//! first leg's pick; the MoE tail as one step or, `unfused`, its
//! chain
def private moe_leg_metal(t : T?; m : Model; p : MatrixMoeRow; leg_i : int; prompt : array<int64>;
                          var kept : MatrixMoeKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let unfused = leg_i == 1
        let kdt = leg_i == 2 ? KVDtype.q8_0 : KVDtype.f16
        var leg : MatrixMoeLeg
        with_dasllama_jobque() {
            let prior_unfused = set_metal_moe_g4_unfused(unfused)
            with_metal_rails(MetalRails()) {
                kernel_census_reset_metal()
                let pf0 = metal_prefill_count()
                let d0 = metal_decode_count()
                var s = make_run_state(m.config, kdt, kdt)
                eval_(m, s, prompt)
                leg.pf := s.logits
                leg.gpk0 = argmax_of(s.logits, kept.vocab)
                kept.t0 = leg_i == 0 ? leg.gpk0 : kept.t0
                leg.npf = metal_kernel_count_compiled(G4_STEP_PF)
                var one <- [kept.t0]
                eval_(m, s, one)
                delete one
                leg.step := s.logits
                leg.gpk1 = argmax_of(s.logits, kept.vocab)
                leg.ndec = metal_kernel_count_compiled(G4_STEP_DEC)
                if (leg_i == 0) {
                    //! the routed sites' own formats: each stamp the
                    //! stacks name dispatched
                    to_log(LOG_INFO, "{p.tag}: routed GEMVs dispatched:{routed_gemv_counts()}\n")
                    var inscope gemvs <- gemv_list(p)
                    for (g in gemvs) {
                        t |> success(metal_kernel_count_compiled(g) > 0l,
                            "{p.tag}: the routed sites' {g} dispatched")
                    }
                }
                leg.served = (metal_prefill_count() - pf0) + (metal_decode_count() - d0)
                delete s
            }
            set_metal_moe_g4_unfused(prior_unfused)
        }
        kept.legs |> emplace(leg)
    }
}

//! A gemma4moe row on the blob twin: the MoE geometry and needs, the
//! matrix cell, the three legs' rows (fused, unfused tail, q8_0
//! mirror). Token parity is no instrument here: the router's double
//! norm amplifies any float-class difference, so the bars are logits
[cold_path]
def matrix_moe_metal(t : T?; var m : Model; form : ModelForm; p : MatrixMoeRow;
                     var kept : MatrixMoeKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        matrix_metal_side(t, m, form, family_row_on(t, "gemma4moe"), p.file, kept) {
            let c = m.config
            t |> success(c.moe && c.moe_dense_shexp && c.n_expert == 128l && c.n_expert_used == 8l,
                ("{p.tag}: MoE geometry loads (ne={c.n_expert} k={c.n_expert_used} " +
                "nfe={c.n_ff_exp})"))
            let want = MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.sliding | MetalNeed.v_norm
            needs_cell(t, m, p.tag, uint(want))
            kept.vocab = c.vocab_size
            kept.nl = c.n_layers
            matrix_cell(t, m, KVDtype.f16, "{p.tag}/q8/f16")
            var inscope prompt <- encode(m, counting_prompt(1, 20), true, false)
            for (leg_i in range(3)) {
                moe_leg_metal(t, m, p, leg_i, prompt, kept)
            }
        }
    }
}

//! The MoE row's CPU truths on f16 and q8_0 sessions, the Metal
//! overrides selected: they decline the planar model by name, so none
//! of the routed stamps dispatches - their control; the plane census
[cold_path]
def matrix_moe_cpu(t : T?; var m : Model; form : ModelForm; p : MatrixMoeRow;
                   var kept : MatrixMoeKept) {
    matrix_cpu_side(m, form, kept) {
        var inscope gemvs <- gemv_list(p)
        var inscope prompt <- encode(m, counting_prompt(1, 20), true, false)
        let cpu_gemv0 = kernels_sum(gemvs)
        with_dasllama_jobque() {
            with_metal_rails(MetalRails(min_npos = 0l)) {
                var one <- [kept.t0]
                var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                eval_(m, s, prompt)
                kept.lg0 := s.logits
                eval_(m, s, one)
                kept.lg1 := s.logits
                kept.t1 = argmax_of(s.logits, kept.vocab)
                delete s
                var sq = make_run_state(m.config, KVDtype.q8_0, KVDtype.q8_0)
                eval_(m, sq, prompt)
                kept.lq0 := sq.logits
                eval_(m, sq, one)
                kept.lq1 := sq.logits
                delete sq
                delete one
            }
        }
        t |> equal(kernels_sum(gemvs) - cpu_gemv0, 0l,
            ("{p.tag}: the planar truths dispatch none of the routed stamps ({gemvs}) - their " +
            "control"))
        census_cell(t, m, path_join(models_dir(), p.file), p.tag,
            ["attention q/k/v/o", "routed experts", "token table", "classifier"])
        t |> equal(m.experts_mx4, p.mx4,
            ("{p.tag}: the expert stacks load as the native-MXFP4 planes exactly where the file " +
            "carries them"))
        let cpu_eye = "'{eyeball_str(piece_(m, kept.t0))}'/'{eyeball_str(piece_(m, kept.t1))}'"
        for (leg in kept.legs) {
            leg.eye = ("CPU {cpu_eye} vs GPU '{eyeball_str(piece_(m, leg.gpk0))}'/" +
                "'{eyeball_str(piece_(m, leg.gpk1))}'")
        }
    }
}

//! one leg's rows within the CPU truth's bars, each against the truth
//! one step off outside them; the MoE step counted a layer
def private moe_leg_verdict(t : T?; p : MatrixMoeRow; leg_i : int; leg : MatrixMoeLeg;
                            truth_pf, truth_1 : array<float>; nl, vocab : int64) {
    let unfused = leg_i == 1
    let name = unfused ? "{p.tag} unfused tail" : (leg_i == 2 ? "{p.tag} q8_0 mirror" : p.tag)
    let v = int(vocab)
    let mpf = span_maxdiff(truth_pf, leg.pf, 0, 0, v).diff
    let mpf_off = span_maxdiff(truth_1, leg.pf, 0, 0, v).diff   //! the bars' controls
    let m1 = span_maxdiff(truth_1, leg.step, 0, 0, v).diff
    let m1_off = span_maxdiff(truth_pf, leg.step, 0, 0, v).diff
    let pk_pf = live_maxabs(truth_pf)
    let pk1 = live_maxabs(truth_1)
    let share = p.bars == MoeRowBar.peak_share
    let bar_pf = share ? row_bar(truth_pf, p.tol_pf) : p.tol_pf
    let bar_step = share ? row_bar(truth_1, p.tol_step) : p.tol_step
    t |> success(leg.served == 2l, "{name}: GPU prefill + forced step both served ({leg.served}/2)")
    t |> success(mpf <= bar_pf, ("{name}: post-prefill logits within {bar_pf} of the CPU truth " +
        "(measured {mpf}, peak {pk_pf})"))
    t |> success(m1 <= bar_step, ("{name}: one forced step within {bar_step} of the CPU truth " +
        "(measured {m1}, peak {pk1})"))
    t |> success(is_finite(mpf_off) && mpf_off > bar_pf,
        "{name}: the post-prefill row against the CPU truth one step on lands outside the " +
        "bar, finite ({mpf_off}) - the control")
    t |> success(is_finite(m1_off) && m1_off > bar_step,
        "{name}: the forced step against the CPU truth one step back lands outside the " +
        "bar, finite ({m1_off}) - the control")
    if (unfused) {
        t |> success(leg.npf == 0l && leg.ndec == 0l,
            "{name}: no MoE step dispatched (window {leg.npf}, step {leg.ndec})")
    } else {
        t |> success(leg.npf == nl,
            "{name}: the window's MoE tail one step a layer ({leg.npf} of {nl})")
        //! the greedy chain encodes the next step beside the forced
        //! one
        t |> success(leg.ndec > 0l && leg.ndec % (nl - 1l) == 0l,
            ("{name}: the forced step's MoE " +
            "tail one step a layer past the last ({leg.ndec}, steps of {nl - 1l})"))
    }
    to_log(LOG_INFO, ("{name} tolerance cells: post-prefill maxd={mpf} (bar {bar_pf}, peak " +
        "{pk_pf}, control {mpf_off}), forced step maxd={m1} (bar {bar_step}, peak {pk1}, " +
        "control {m1_off}); MoE steps window {leg.npf} step {leg.ndec}; eyeball {leg.eye}\n"))
}

//! the MoE row's three legs against the CPU truths
def matrix_moe_verdict(t : T?; p : MatrixMoeRow; var kept : MatrixMoeKept) {
    if (both_sides(t, p.tag, kept.metal, kept.cpu)) {
        for (leg, leg_i in kept.legs, count()) {
            let q8 = leg_i == 2
            moe_leg_verdict(t, p, leg_i, leg, q8 ? kept.lq0 : kept.lg0, q8 ? kept.lq1 : kept.lg1,
                kept.nl, kept.vocab)
        }
    }
    delete kept
}

//! the Q8_0 file's q8 stacks on the first six blocks, the cap lifted
def matrix_moe_q8_cut() : MatrixMoeRow => MatrixMoeRow(file = "gemma-4-26B-A4B-6L-Q8_0-local.gguf",
    tag = "gemma4moe-26b", tol_pf = CUT_ROW_BAR_REL, tol_step = CUT_ROW_BAR_REL,
    gemvs = "metal_moe_gemv_q8", bars = MoeRowBar.peak_share)

//! k4 gate_up + native q51 down stacks; the down stacks' experts are
//! 2816 rows, so their routed GEMV is the wide q51 stamp
def matrix_moe_k4_cut() : MatrixMoeRow => MatrixMoeRow(
    file = "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf",
    tag = "gemma4moe-26b-k", tol_pf = CUT_ROW_BAR_REL, tol_step = CUT_ROW_BAR_REL,
    gemvs = "metal_moe_gemv_k4,MetalMoeGemvQ51W_metal_moe_gemv_w", bars = MoeRowBar.peak_share)

//! q40 gate_up stacks beside the 704-wide down stacks in their per-32
//! form (q4032)
def matrix_moe_q40_cut() : MatrixMoeRow => MatrixMoeRow(file = "gemma-4-26B-A4B-6L-Q4_0-local.gguf",
    tag = "gemma4moe-26b-q40", tol_pf = CUT_ROW_BAR_REL, tol_step = CUT_ROW_BAR_REL,
    gemvs = "metal_moe_gemv_q40,metal_moe_gemv_q4032", bars = MoeRowBar.peak_share)

//! grid-format IQ2_S gate_up beside the IQ4_NL down stacks (iq4nl32)
//! and a Q6_K token table
def matrix_moe_iq3xxs_cut() : MatrixMoeRow => MatrixMoeRow(
    file = "gemma-4-26B-A4B-6L-UD-IQ3_XXS-local.gguf", tag = "gemma4moe-26b-iq3xxs",
    tol_pf = CUT_ROW_BAR_REL, tol_step = CUT_ROW_BAR_REL,
    gemvs = "metal_moe_gemv_iq2s,metal_moe_gemv_iq4nl32", bars = MoeRowBar.peak_share)

//! the whole UD-IQ4_XS file: its IQ4_XS gate_up stack is the last
//! block's alone, which no cut keeps
def matrix_moe_iq4xs() : MatrixMoeRow => MatrixMoeRow(file = "gemma-4-26B-A4B-it-UD-IQ4_XS.gguf",
    tag = "gemma4moe-26b-iq4xs", tol_pf = 8.0, tol_step = 24.0,
    gemvs = "metal_moe_gemv_iq4xs,metal_moe_gemv_iq4nl32")

//! the fused gate_up and the down stacks MXFP4 on disk, the native
//! MXFP4 planes under GeGLU and the per-expert down scale
def matrix_moe_mx4_cut() : MatrixMoeRow => MatrixMoeRow(
    file = "gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf",
    tag = "gemma4moe-26b-mx4", tol_pf = CUT_ROW_BAR_REL, tol_step = CUT_ROW_BAR_REL,
    gemvs = "metal_moe_gemv_mx4", mx4 = true, bars = MoeRowBar.peak_share)

//! what the gpt-oss row keeps: the forced token, both sides' rows
struct MatrixGptossKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    vocab : int64
    served : int64
    t0 : int64
    gpk0 : int64
    gpk1 : int64
    gpf : array<float>
    gstep : array<float>
    lg0 : array<float>
    lg1 : array<float>
    eye : string
}

//! The gpt-oss row on the blob twin: MXFP4 expert stacks, attention
//! sinks, softmax_weight routing, biases, swiglu_oai; the geometry,
//! the needs, the matrix cell, the prefill and forced step's rows
[cold_path]
def matrix_gptoss_metal(t : T?; var m : Model; form : ModelForm;
                        var kept : MatrixGptossKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        matrix_metal_side(t, m, form, family_row_on(t, "gptoss"), "gpt-oss-4L-mxfp4-local.gguf",
                kept) {
            let c = m.config
            t |> success(c.moe && m.experts_mx4 && c.attn_sinks && c.n_expert == 32l
                         && c.n_expert_used == 4l,
                ("gptoss-20b: MoE geometry loads (mx4={m.experts_mx4} sinks={c.attn_sinks} " +
                "ne={c.n_expert} k={c.n_expert_used})"))
            let want = MetalNeed.neox | MetalNeed.sliding | MetalNeed.sinks | MetalNeed.out_bias
            needs_cell(t, m, "gptoss-20b", uint(want))
            kept.vocab = c.vocab_size
            matrix_cell(t, m, KVDtype.f16, "gptoss-20b/mx4/f16")
            var inscope prompt <- encode(m, counting_prompt(1, 20), true, false)
            with_dasllama_jobque() {
                with_metal_rails(MetalRails()) {
                    let pf0 = metal_prefill_count()
                    let d0 = metal_decode_count()
                    var s = make_run_state(c, KVDtype.f16, KVDtype.f16)
                    eval_(m, s, prompt)
                    kept.gpf := s.logits
                    kept.gpk0 = argmax_of(s.logits, kept.vocab)
                    kept.t0 = kept.gpk0
                    var one <- [kept.t0]
                    eval_(m, s, one)
                    delete one
                    kept.gstep := s.logits
                    kept.gpk1 = argmax_of(s.logits, kept.vocab)
                    kept.served = (metal_prefill_count() - pf0) + (metal_decode_count() - d0)
                    delete s
                }
            }
        }
    }
}

//! the gpt-oss row's CPU truths: the prefill row and the forced
//! step's
[cold_path, unused_argument(t)]
def matrix_gptoss_cpu(t : T?; var m : Model; form : ModelForm; var kept : MatrixGptossKept) {
    matrix_cpu_side(m, form, kept) {
        var inscope prompt <- encode(m, counting_prompt(1, 20), true, false)
        var t1 = 0l
        with_dasllama_jobque() {
            var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            eval_(m, s, prompt)
            kept.lg0 := s.logits
            var one <- [kept.t0]
            eval_(m, s, one)
            delete one
            kept.lg1 := s.logits
            t1 = argmax_of(s.logits, kept.vocab)
            delete s
        }
        kept.eye = ("CPU '{eyeball_str(piece_(m, kept.t0))}'/'{eyeball_str(piece_(m, t1))}' " +
            "vs GPU " +
            "'{eyeball_str(piece_(m, kept.gpk0))}'/'{eyeball_str(piece_(m, kept.gpk1))}'")
    }
}

//! the gpt-oss tolerance cells: the tightest MoE envelope, bars ~8x
//! the measured noise
def matrix_gptoss_verdict(t : T?; var kept : MatrixGptossKept) {
    if (both_sides(t, "gptoss-20b", kept.metal, kept.cpu)) {
        let mpf = logits_maxdiff(kept.lg0, kept.gpf)
        let m1 = logits_maxdiff(kept.lg1, kept.gstep)
        t |> success(kept.served == 2l,
            "gptoss-20b: GPU prefill + forced step both served ({kept.served}/2)")
        t |> success(mpf <= 2.0,
            "gptoss-20b: post-prefill logits within 2.0 of the CPU truth (measured {mpf})")
        t |> success(m1 <= 4.0,
            "gptoss-20b: one forced step within 4.0 of the CPU truth (measured {m1})")
        to_log(LOG_INFO,
            ("gptoss-20b tolerance cells: post-prefill maxd={mpf} (tol 2), forced step " +
            "maxd={m1} (tol 4); eyeball {kept.eye}\n"))
    }
    delete kept
}

//! the parameters of one hybrid row
struct MatrixHybridRow {
    file : string
    family : string
    tag : string
    cont_tol : float
}

//! what a hybrid row keeps: a generate pair a KV codec, the
//! continuation cell's rows
struct MatrixHybridKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    gpu : array<array<int64>>
    cpu_ids : array<array<int64>>
    cont : MatrixContKept
}

//! the hybrid rows' KV codecs: the f16 mirror and both block codecs
def private hybrid_codecs() : array<KVDtype> => <- [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4]

//! The batched step (the recurrent rows form, each row's scan against
//! its own state) on `kdt` against two single-row GPU steps on twin
//! sessions, row for row; row A against row B outside the bar
def private hybrid_batch_cell(t : T?; m : Model; tag : string;
                              kdt : KVDtype) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = m.config
        let vocab = c.vocab_size
        with_dasllama_jobque() {
            var sa = make_run_state(c, kdt, kdt)
            var sb = make_run_state(c, kdt, kdt)
            var inscope pa <- encode(m, counting_prompt(1, 40), true, false)
            var inscope pb <- encode(m, counting_prompt(5, 44), true, false)
            with_metal_rails(t, tag, MetalRails()) {
                eval_(m, sa, pa)
                eval_(m, sb, pb)
                var sa2 = make_run_state(c, kdt, kdt)
                var sb2 = make_run_state(c, kdt, kdt)
                eval_(m, sa2, pa)
                eval_(m, sb2, pb)
                var cur <- [argmax_of(sa.logits, vocab), argmax_of(sb.logits, vocab)]
                var onea <- [cur[0]]
                var oneb <- [cur[1]]
                eval_(m, sa2, onea)
                eval_(m, sb2, oneb)
                delete onea
                delete oneb
                var ws <- create_batch_workspace_(m)
                var sess : array<Session?>
                unsafe {
                    sess |> push(addr(sa))
                    sess |> push(addr(sb))
                }
                let ba1 = metal_batch_step_count()
                eval_batch_(m, ws, sess, cur)
                t |> equal(metal_batch_step_count() - ba1, 1l,
                    "{tag} batch kv={kdt}: the driver served the step")
                hybrid_batch_asserts(t, m, tag, kdt, sa, sb, sa2, sb2)
                sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
                unsafe {
                    delete ws
                    delete sess
                }
                delete cur
                delete sa2
                delete sb2
            }
            delete sa
            delete sb
        }
    }
}

//! the batched rows against the single-row steps within 2.0, and the
//! cross-row control outside it
def private hybrid_batch_asserts(t : T?; m : Model; tag : string; kdt : KVDtype;
                                 sa, sb, sa2, sb2 : Session) {
    let vocab = m.config.vocab_size
    let bmaxd = max(logits_maxdiff(sa.logits, sa2.logits), logits_maxdiff(sb.logits, sb2.logits))
    //! the control: a step that bound one row's slice to the other
    //! would red
    let xmaxd = logits_maxdiff(sa.logits, sb2.logits)
    to_log(LOG_INFO, ("{tag} batched step kv={kdt}: single-row next " +
        "'{eyeball_str(piece_(m, argmax_of(sa2.logits, vocab)))}' / " +
        "'{eyeball_str(piece_(m, argmax_of(sb2.logits, vocab)))}' vs batched next " +
        "'{eyeball_str(piece_(m, argmax_of(sa.logits, vocab)))}' / " +
        "'{eyeball_str(piece_(m, argmax_of(sb.logits, vocab)))}' (maxd {bmaxd})\n"))
    t |> success(bmaxd <= 2.0,
        ("{tag} kv={kdt}: batched-step logits within 2.0 of the single-row GPU step, both rows " +
        "(maxd {bmaxd})"))
    t |> success(xmaxd > 2.0,
        "{tag} kv={kdt}: the cross-row compare lands outside the bar (maxd {xmaxd})")
}

//! the hybrid row's cells on the blob twin
def private hybrid_metal_cells(t : T?; m : Model; p : MatrixHybridRow;
                               var kept : MatrixHybridKept) {
    needs_cell(t, m, p.tag, family_needs(p.family))
    var inscope any_sites : array<string>
    //! every site the file carries read in its own format on the twin
    census_cell(t, m, path_join(models_dir(), p.file), "{p.tag} blob twin", any_sites)
    let pf0 = metal_prefill_count()
    let de0 = metal_decode_count()
    let ba0 = metal_batch_step_count()
    var inscope codecs <- hybrid_codecs()
    kept.gpu |> reserve(length(codecs))
    for (kdt in codecs) {
        hybrid_batch_cell(t, m, p.tag, kdt)
    }
    t |> success(metal_prefill_count() - pf0 > 0l, "{p.tag} prefill: ENGAGES on the GPU")
    t |> success(metal_decode_count() - de0 > 0l, "{p.tag} decode: ENGAGES on the GPU")
    t |> success(metal_batch_step_count() - ba0 > 0l, "{p.tag} batch: ENGAGES on the GPU")
    let prompt <- encode(m, counting_prompt(1, 30), true, false)
    for (kdt in codecs) {   //! each codec's single-row store on the partial-rope carrier
        kept.gpu |> emplace <| metal_generate(m, prompt, length(prompt) + 8, kdt)
    }
    if (p.cont_tol > 0.0) {
        cont_metal(m, kept.cont)
    }
}

//! A deltanet hybrid row on the blob twin (3-in-4 Gated-DeltaNet
//! layers + gated attention): needs, census, the batched step against
//! single-row steps a codec, the paths' engage, a generate a codec
[cold_path]
def matrix_hybrid_metal(t : T?; var m : Model; form : ModelForm; p : MatrixHybridRow;
                        var kept : MatrixHybridKept) {
    matrix_metal_side(t, m, form, family_row_on(t, p.family), p.file, kept) {
        hybrid_metal_cells(t, m, p, kept)
    }
}

//! the hybrid row's CPU truths: a generate a codec, the continuation
[cold_path, unused_argument(t)]
def matrix_hybrid_cpu(t : T?; var m : Model; form : ModelForm; p : MatrixHybridRow;
                      var kept : MatrixHybridKept) {
    matrix_cpu_side(m, form, kept) {
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        var inscope codecs <- hybrid_codecs()
        for (kdt, i in codecs, count()) {
            kept.cpu_ids |> emplace <| cpu_generate(m, prompt, length(prompt) + 8, kdt)
            log_gen_texts(m, "{p.tag} kv={kdt}", kept.cpu_ids[i], kept.gpu[i])
        }
        if (p.cont_tol > 0.0) {
            cont_cpu(m, kept.cont)
        }
    }
}

//! the hybrid row's generates token for token a codec, the
//! continuation within `cont_tol`
def matrix_hybrid_verdict(t : T?; p : MatrixHybridRow; var kept : MatrixHybridKept) {
    if (both_sides(t, p.tag, kept.metal, kept.cpu)) {
        var inscope codecs <- hybrid_codecs()
        for (kdt, i in codecs, count()) {
            t |> success(i < length(kept.cpu_ids) && ids_equal(kept.cpu_ids[i], kept.gpu[i]),
                "{p.tag} kv={kdt}: metal-overrides generate matches the CPU truth token-for-token")
        }
        if (p.cont_tol > 0.0) {
            cont_verdict(t, p.tag, p.cont_tol, kept.cont)
        }
    }
    delete kept
}

def matrix_hybrid_qwen35_0_8b() : MatrixHybridRow => MatrixHybridRow(
    file = "Qwen3.5-0.8B-Q8_0.gguf", family = "qwen35", tag = "qwen35-0.8b/q8/f16", cont_tol = 4.0)

//! deltanet + grouped MoE + the sigmoid-gated shared expert, the
//! whole file: its generate matches the CPU truth token for token
def matrix_hybrid_qwen36_35b() : MatrixHybridRow => MatrixHybridRow(
    file = "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", family = "qwen35moe", tag = "qwen35moe-35b/k4/f16",
    cont_tol = 6.0)

let private QWEN2MOE_FILE = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"
let private QWEN2MOE_BAR = 8.0

//! what the qwen2moe row keeps: the batched-step rows, the generates,
//! the continuation cell's rows
struct MatrixQwen2moeKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    batch : MatrixBatchKept
    gen : MatrixGenPair
    cont : MatrixContKept
}

//! the two batch prompts: row B counts through another decade, so the
//! rows' next digits differ and the cross-row control can hold
def private qwen2moe_prompts(m : Model) : tuple<a : array<int64>; b : array<int64>> {
    var r : tuple<a : array<int64>; b : array<int64>>
    r.a <- encode(m, counting_prompt(1, 40), true, false)
    r.b <- encode(m, counting_prompt(23, 78), true, false)
    return <- r
}

//! the qwen2moe row's prefill and decode engage on the twin
def private qwen2moe_paths(t : T?; m : Model; pa : array<int64>) {
    let vocab = m.config.vocab_size
    with_dasllama_jobque() {
        with_metal_rails(t, "qwen2moe", MetalRails()) {
            var sa = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            eval_(m, sa, pa)
            var cura = argmax_of(sa.logits, vocab)
            for (_i in range(2)) {
                var one <- [cura]
                eval_(m, sa, one)
                delete one
                cura = argmax_of(sa.logits, vocab)
            }
            delete sa
        }
    }
}

//! The qwen2moe row on the blob twin (softmax-no-renorm select and
//! the sigmoid-gated shared expert): needs, the paths, the batched
//! step's rows, the generate, the continuation's Metal side
[cold_path]
def matrix_qwen2moe_metal(t : T?; var m : Model; form : ModelForm;
                          var kept : MatrixQwen2moeKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        matrix_metal_side(t, m, form, family_row_on(t, "qwen2moe"), QWEN2MOE_FILE, kept) {
            let derived = metal_needs(m)
            let want = uint(MetalNeed.neox | MetalNeed.qkv_bias)
            t |> success((uint(derived) & want) == want,
                "qwen2moe: metal_needs derives neox|qkv_bias (got {derived})")
            let pf0 = metal_prefill_count()
            let de0 = metal_decode_count()
            let ba0 = metal_batch_step_count()
            let graph0 = metal_batch_decline_count("graph")
            var inscope ps <- qwen2moe_prompts(m)
            qwen2moe_paths(t, m, ps.a)
            batch_pair_metal(m, ps.a, ps.b, kept.batch)
            t |> equal(kept.batch.served, 1l, "qwen2moe batch: the driver served the step")
            t |> success(metal_prefill_count() - pf0 > 0l,
                "qwen2moe prefill: ENGAGES on the GPU")
            t |> success(metal_decode_count() - de0 > 0l, "qwen2moe decode: ENGAGES on the GPU")
            let graph = metal_batch_decline_count("graph")
            t |> success(metal_batch_step_count() - ba0 > 0l,
                "qwen2moe batch: ENGAGES on the GPU")
            t |> equal(graph - graph0, 0l,
                "qwen2moe batch: no graph decline (the shared expert rides the rows form)")
            let prompt <- encode(m, counting_prompt(1, 30), true, false)
            kept.gen.gpu <- metal_generate(m, prompt, length(prompt) + 8)
            cont_metal(m, kept.cont)
        }
    }
}

//! the qwen2moe row's CPU side: the CPU batched stack on the Metal
//! side's picks, the generate, the continuation
[cold_path, unused_argument(t)]
def matrix_qwen2moe_cpu(t : T?; var m : Model; form : ModelForm; var kept : MatrixQwen2moeKept) {
    matrix_cpu_side(m, form, kept) {
        var inscope ps <- qwen2moe_prompts(m)
        batch_pair_cpu(m, ps.a, ps.b, kept.batch)
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        kept.gen.cpu <- cpu_generate(m, prompt, length(prompt) + 8)
        log_gen_texts(m, "qwen2moe", kept.gen.cpu, kept.gen.gpu)
        cont_cpu(m, kept.cont)
    }
}

//! the batched step within the bar of the CPU batched stack, the
//! generate token for token, the continuation within 4.0
def matrix_qwen2moe_verdict(t : T?; var kept : MatrixQwen2moeKept) {
    if (both_sides(t, "qwen2moe", kept.metal, kept.cpu)) {
        batch_pair_verdict(t, "qwen2moe", QWEN2MOE_BAR, "8.0", kept.batch)
        t |> success(ids_equal(kept.gen.cpu, kept.gen.gpu),
            "qwen2moe: metal-overrides generate matches the CPU truth token-for-token")
        cont_verdict(t, "qwen2moe-a2.7b/q8/f16", 4.0, kept.cont)
    }
    delete kept
}

//! the parameters of one E-series row; `kernels` the comma list of
//! the stamps only the file's format at its site dispatches
struct MatrixEseriesRow {
    file : string
    tag : string
    bar : float = 8.0
    arm : string = "fam-gemma4e"
    kernels : string
}

//! what an E-series row keeps: the forced steps' picks and the Metal
//! row after them, the batched-step rows, the Metal generate, the
//! depth and continuation cells' rows
struct MatrixEseriesKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    vocab : int64
    picks : array<int64>
    single : array<float>
    cpu_single : array<float>
    single_eye : string
    batch : MatrixBatchKept
    gpu_ids : array<int64>
    depth : MatrixDepthKept
    cont : MatrixContKept
}

//! the row's site stamps as a list
def private kernel_list(p : MatrixEseriesRow) : array<string> =>
    <- [for (k in split(p.kernels, ",")); k; where !empty(k)]

//! the E-series derivation bits, the shared KV layers, the two FFN
//! widths: the config the twin carries
def private eseries_shape(t : T?; m : Model; tag : string) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let want_bits = (MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.v_norm
            | MetalNeed.attn_scale | MetalNeed.sliding | MetalNeed.pre_post_norm
            | MetalNeed.out_scale | MetalNeed.geglu | MetalNeed.ple | MetalNeed.qd_neq_dim)
        needs_cell(t, m, tag, uint(want_bits))
        var nshared = 0l
        for (l in range64(m.config.n_layers)) {
            if (m.kv_src[l] != l) {
                nshared++
            }
        }
        t |> success(nshared > 0l,
            "{tag}: fixture carries cross-layer KV sharing ({nshared} shared layers)")
        //! the MatFormer width split: the per-layer u_hidden bind's
        //! row
        t |> success(ffn_second_hidden(m) > 0l,
            "{tag}: fixture carries two FFN widths (second width {ffn_second_hidden(m)})")
    }
}

//! the decode steps' seams one dispatch each, no plain seam on the
//! sandwich; `sm` the seam counts' rise over `nst` steps
def private eseries_seams(t : T?; m : Model; tag : string; sm : int4; nst : int64) {
    let nl = int(m.config.n_layers)
    //! the encoded stacks: the steps and the chain's pre-encoded one
    let nenc = sm.x / (2 * nl - 1)
    to_log(LOG_INFO, ("{tag} decode: {nst} steps, {nenc} encoded stacks' seams - pre_add_rms " +
        "{sm.x}, pre_add {sm.y}, add_rms {sm.z}, rms {sm.w}\n"))
    t |> success(nst > 0l && int64(nenc) >= nst,
        "{tag} decode: the steps ran on the device ({nst} steps, {nenc} stacks)")
    t |> equal(sm.x, nenc * (2 * nl - 1),
        "{tag} decode: the sandwich seams with a next norm, one dispatch each")
    t |> equal(sm.y, nenc * (nl + 1),
        "{tag} decode: the seams with no norm stored, one dispatch each")
    t |> equal(sm.z, 0, "{tag} decode: no plain seam on the sandwich")
}

//! the window's fused q/k norm and rope pass, both stamps
def private qkn_rope_count : int64 => kernels_sum(["metal_pf_qkn_rope", "metal_pf_qkn_rope_hx"])

//! the residual step's seam stamps the device ran, and the split
//! chain's norm pass
def private seam_counts : int4 {
    let pre_add_rms = int(kernels_sum(["metal_pre_add_rms", "metal_pre_add_rms_hx",
        "metal_pre_add_rms_hxt"]))
    let add_rms = int(kernels_sum(["metal_add_rms", "metal_add_rms_hx", "metal_add_rms_hxt"]))
    let rms = int(kernels_sum(["MetalRmsNorm_metal_rmsnorm", "MetalRmsNormHX_metal_rmsnorm",
        "MetalRmsNormHXT_metal_rmsnorm"]))
    return int4(pre_add_rms, int(kernels_sum(["metal_pre_add"])), add_rms, rms)
}

//! The prefill and two steps on the twin: the PLE pre-step finished
//! on the device, the v-norm on the fused q/k pass, every step's side
//! row projected, no CPU pre-step row; the picks and last row kept
def private eseries_steps(t : T?; m : Model; tag : string; pa : array<int64>;
                          var kept : MatrixEseriesKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vocab = kept.vocab
        var sa = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        let fin0 = metal_kernel_count_compiled("metal_ple_finish")
        let cpu0 = ple_cpu_pre_rows()
        let qn0 = metal_kernel_count_compiled("metal_qk_norm")
        let pq0 = qkn_rope_count()
        eval_(m, sa, pa)
        let fin_pf = metal_kernel_count_compiled("metal_ple_finish") - fin0
        t |> success(fin_pf > 0l,
            "{tag} prefill: the window's PLE pre-step finished on the device ({fin_pf})")
        let qn_pf = metal_kernel_count_compiled("metal_qk_norm") - qn0
        let pq_pf = qkn_rope_count() - pq0
        t |> success(pq_pf > 0l && qn_pf == 0l,
            ("{tag} prefill: the v-norm rows on the fused q/k pass ({pq_pf}), no norm pass " +
            "({qn_pf})"))
        let pj0 = ple_proj_count()
        let sm0 = seam_counts()
        let st0 = metal_decode_count()
        var cura = argmax_of(sa.logits, vocab)
        kept.picks |> reserve(2)
        for (_i in range(2)) {
            kept.picks |> push(cura)
            var one <- [cura]
            eval_(m, sa, one)
            delete one
            cura = argmax_of(sa.logits, vocab)
        }
        eseries_seams(t, m, tag, seam_counts() - sm0, metal_decode_count() - st0)
        let pj_d = ple_proj_count() - pj0
        t |> success(pj_d >= 2l,
            "{tag} decode: every step projected its side row on the device ({pj_d})")
        t |> equal(ple_cpu_pre_rows() - cpu0, 0l,
            "{tag}: the prefill and the steps built no CPU pre-step row")
        kept.single := sa.logits
        delete sa
    }
}

//! the prefill, steps and batched step under the fuse rail pinned on:
//! the seam census counts the fused residual steps
def private eseries_forced(t : T?; m : Model; p : MatrixEseriesRow;
                           var kept : MatrixEseriesKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let pa <- encode(m, counting_prompt(1, 40), true, false)
        let pb <- encode(m, counting_prompt(5, 44), true, false)
        let fuse = get_metal_decode_fuse()
        set_metal_decode_fuse(1)
        with_dasllama_jobque() {
            with_metal_rails(t, p.tag, MetalRails()) {
                eseries_steps(t, m, p.tag, pa, kept)
            }
        }
        batch_pair_metal(m, pa, pb, kept.batch)
        set_metal_decode_fuse(fuse)
        t |> equal(kept.batch.served, 1l, "{p.tag} batch: the driver served the step")
        t |> success(kept.batch.side_rows > 0l,
            "{p.tag} batch: the rows' side input projected on the device")
        t |> equal(kept.batch.cpu_pre_rows, 0l,
            "{p.tag} batch: no CPU pre-step row (the host transpose and gather are gone)")
    }
}

//! the router stamps on an E-series model run the f32 per-layer
//! planes alone: no routed expert; where no plane is f32, none
//! dispatches
def private eseries_router(t : T?; m : Model; tag : string; rt0, rtb0 : int64) {
    let rt_n = metal_kernel_count_compiled("metal_moe_router") - rt0
    let rtb_n = kernels_sum(["metal_moe_router_b", "metal_f32_exact_mulmm"]) - rtb0
    let nl = m.config.n_layers
    if (m.ple_gate_f32 || m.ple_proj_f32) {
        t |> success(rt_n >= 2l * nl,
            ("{tag}: the decode steps read the f32 gate and projection rows on the router GEMV " +
            "({rt_n})"))
        t |> success(rtb_n >= 2l * nl,
            "{tag}: the window reads them on the f32 tile or the slab GEMV ({rtb_n})")
    } else {
        t |> equal(rt_n + rtb_n, 0l, "{tag}: no f32 per-layer plane, no router stamp dispatched")
    }
}

//! The greedy chain pinned on: the E-series chains its steps, each
//! building its side row off the argmax; pinned off, the control, no
//! step chains and the walk's tokens are the chained walk's
def private eseries_chain(t : T?; m : Model; tag : string;
                          var kept : MatrixEseriesKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        let spec_mode = metal_decode_spec_mode()
        set_metal_decode_speculate(1)
        let sp0 = metal_decode_spec_stats()
        kept.gpu_ids <- metal_generate(m, prompt, length(prompt) + 8)
        let sp1 = metal_decode_spec_stats()
        set_metal_decode_speculate(0)
        var inscope ids4 <- metal_generate(m, prompt, length(prompt) + 8)
        let sp2 = metal_decode_spec_stats()
        set_metal_decode_speculate(spec_mode)
        t |> success(sp1.hits - sp0.hits > 0l,
            ("{tag}: the greedy chain engaged on the E-series ({sp1.hits - sp0.hits} hits, " +
            "{sp1.misses - sp0.misses} misses)"))
        t |> equal(sp2.hits - sp1.hits, 0l,
            "{tag}: the chain pinned off takes no chained step - the hits' control")
        log_gen_texts(m, "{tag} chain on, chain off", kept.gpu_ids, ids4)
        t |> success(ids_equal(kept.gpu_ids, ids4),
            "{tag}: the chained walk emits the unchained walk's tokens")
        //! the 2B near-ties on the counting continuation: the
        //! forced-feed cells are the parity instruments, this
        //! generate the eyeball
        t |> success(length(kept.gpu_ids) >= length(prompt),
            ("{tag}: metal-overrides generate emits a continuation (parity = the forced-feed " +
            "cell; EOS may stop it early)"))
    }
}

//! the E-series row's cells on the blob twin
def private eseries_metal_cells(t : T?; m : Model; p : MatrixEseriesRow;
                                var kept : MatrixEseriesKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        eseries_shape(t, m, p.tag)
        kept.vocab = m.config.vocab_size
        var inscope kernels <- kernel_list(p)
        let rt0 = metal_kernel_count_compiled("metal_moe_router")
        let rtb0 = kernels_sum(["metal_moe_router_b", "metal_f32_exact_mulmm"])
        let fk0 = kernels_sum(kernels)
        let pf0 = metal_prefill_count()
        let de0 = metal_decode_count()
        let ba0 = metal_batch_step_count()
        let ple0 = metal_batch_decline_count("need_ple")
        eseries_forced(t, m, p, kept)
        t |> success(metal_prefill_count() - pf0 > 0l, "{p.tag} prefill: ENGAGES on the GPU")
        t |> success(metal_decode_count() - de0 > 0l, "{p.tag} decode: ENGAGES on the GPU")
        eseries_router(t, m, p.tag, rt0, rtb0)
        let ple = metal_batch_decline_count("need_ple")
        t |> success(metal_batch_step_count() - ba0 > 0l, "{p.tag} batch: ENGAGES on the GPU")
        t |> equal(ple - ple0, 0l, "{p.tag} batch: no need_ple decline (the PLE rows form serves)")
        eseries_chain(t, m, p.tag, kept)
        depth_metal(t, m, p.tag, ["MetalSqAttnDXF16_metal_sq_attn_d",
            "MetalSqAttnDWF16_metal_sq_attn_d"], true, kept.depth)
        cont_metal(m, kept.cont)
        if (!empty(kernels)) {
            let fk = kernels_sum(kernels) - fk0
            to_log(LOG_INFO, "{p.tag}: the site's format stamps dispatched ({kernels}: {fk})\n")
            t |> success(fk > 0l, "{p.tag}: the site's format stamps dispatched ({kernels}: {fk})")
        }
    }
}

//! An E-series row on the blob twin: shape and census-free cells, the
//! device pre-step and seams, the batched step's rows, the router
//! stamps, the greedy chain, the depth and continuation Metal sides
[cold_path]
def matrix_eseries_metal(t : T?; var m : Model; form : ModelForm; p : MatrixEseriesRow;
                         var kept : MatrixEseriesKept) {
    matrix_metal_side(t, m, form, family_on(t, "gemma4e") && arm_on(t, p.arm), p.file, kept) {
        eseries_metal_cells(t, m, p, kept)
    }
}

//! The planar control: under the same overrides the CPU form's model
//! declines the drivers by name, so its prefill and the Metal side's
//! forced steps finish, project and dispatch nothing on the device
def private eseries_control(t : T?; m : Model; p : MatrixEseriesRow;
                            var kept : MatrixEseriesKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope kernels <- kernel_list(p)
        let pa <- encode(m, counting_prompt(1, 40), true, false)
        with_dasllama_jobque() {
            with_metal_rails(MetalRails()) {
                let cfin0 = metal_kernel_count_compiled("metal_ple_finish")
                let cpj0 = ple_proj_count()
                let cfk0 = kernels_sum(kernels)
                var sc = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                eval_(m, sc, pa)
                for (pk in kept.picks) {
                    var one <- [pk]
                    eval_(m, sc, one)
                    delete one
                }
                t |> equal(metal_kernel_count_compiled("metal_ple_finish") - cfin0, 0l,
                    ("{p.tag}: the planar model's prefill finishes no PLE pre-step on the " +
                    "device - the finish counter's control"))
                t |> equal(ple_proj_count() - cpj0, 0l,
                    ("{p.tag}: the planar model's steps project no side row on the device - the " +
                    "projection counter's control"))
                t |> equal(kernels_sum(kernels) - cfk0, 0l,
                    ("{p.tag}: the planar model dispatches none of the site's format stamps " +
                    "({kernels}) - their control"))
                kept.cpu_single := sc.logits
                delete sc
            }
        }
        let v = kept.vocab
        let cpk = argmax_of(kept.cpu_single, v)
        let gpk = argmax_of(kept.single, v)
        kept.single_eye = ("CPU next '{eyeball_str(piece_(m, cpk))}' vs " +
            "GPU next '{eyeball_str(piece_(m, gpk))}'")
    }
}

//! The E-series row's CPU side: the planar census of the per-layer
//! planes, the device counters' control and forced steps, the CPU
//! batched stack, the generate eyeball, the depth and continuation
[cold_path]
def matrix_eseries_cpu(t : T?; var m : Model; form : ModelForm; p : MatrixEseriesRow;
                       var kept : MatrixEseriesKept) {
    matrix_cpu_side(m, form, kept) {
        //! the per-layer gate and projection planes in their own
        //! formats
        census_cell(t, m, path_join(models_dir(), p.file), "{p.tag} planar",
            ["per-layer embeddings"])
        eseries_control(t, m, p, kept)
        let pa <- encode(m, counting_prompt(1, 40), true, false)
        let pb <- encode(m, counting_prompt(5, 44), true, false)
        batch_pair_cpu(m, pa, pb, kept.batch)
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        var inscope cpu_ids <- cpu_generate(m, prompt, length(prompt) + 8)
        log_gen_texts(m, "{p.tag}", cpu_ids, kept.gpu_ids)
        depth_cpu(m, kept.depth)
        cont_cpu(m, kept.cont)
    }
}

//! the forced steps and the batched step within the row's bar, the
//! depth steps within it, the continuation within it
def matrix_eseries_verdict(t : T?; p : MatrixEseriesRow; var kept : MatrixEseriesKept) {
    if (both_sides(t, p.tag, kept.metal, kept.cpu)) {
        let ssmaxd = span_maxdiff(kept.cpu_single, kept.single, 0, 0, int(kept.vocab)).diff
        to_log(LOG_INFO, "{p.tag} single-stream forced-step: {kept.single_eye} (maxd {ssmaxd})\n")
        t |> success(ssmaxd <= p.bar,
            "{p.tag}: single-stream forced-step logits within {p.bar} (maxd {ssmaxd})")
        batch_pair_verdict(t, p.tag, p.bar, "{p.bar}", kept.batch)
        depth_verdict(t, p.tag, p.bar, kept.depth)
        cont_verdict(t, "{p.tag}/q8/f16", p.bar, kept.cont)
    }
    delete kept
}

//! E2B: the MatFormer sub-model's two FFN widths. Its form
//! `MATRIX_RING_CTX`, shared with the ring row
def matrix_eseries_e2b() : MatrixEseriesRow => MatrixEseriesRow(file = "gemma-4-E2B-it-Q8_0.gguf",
    tag = "gemma4e-e2b")

//! the kq carrier: the k5-plane PLE table's gather over the compacted
//! scale strips
def matrix_eseries_e2b_kq() : MatrixEseriesRow => MatrixEseriesRow(
    file = "gemma-4-E2B-it-Q4_K_M.gguf", tag = "gemma4e-e2b-kq")

//! the per-layer gate and projection planes Q4_0 on disk; the rows
//! sit closer together (the batch cell's two rows 6.6 apart), so 4.0
def matrix_eseries_e2b_q40() : MatrixEseriesRow => MatrixEseriesRow(
    file = "gemma-4-E2B-it-Q4_0.gguf", tag = "gemma4e-e2b-q40", bar = 4.0, arm = "fam-gemma4e-q40",
    kernels = ("MetalKqGemvQ40_metal_kq_gemv_iq4,MetalKqMvB2Q40_metal_kq_mv_iq4," +
        "MetalKqMvB2Q40H_metal_kq_mv_iq4"))

//! the parameters of one hetero row: `stream` the forced-feed
//! stream's ids, a row held ahead of each - the CPU's greedy
//! continuation of the freeform prompt, frozen, so no home picks it
struct MatrixHeteroRow {
    file : string
    tag : string = "gemma4-12b/k4"
    stream : array<int64>
}

//! the forced-feed stream's reads: the Metal side's held rows, the
//! CPU side's reductions over them
struct MatrixFreeKept {
    served : int64
    prompt_len : int64
    rows : array<array<float>>
    fmaxd : float
    fmax_at : int64
    fmax_peak : float
    foff_min : float = INFINITE_DIFF
    foff_at : int64
    cpu_text : string
    fed_text : string
}

//! what a hetero row keeps: the forced-feed stream, the counting and
//! B=2 pairs, the served counts, the continuation cell's rows
struct MatrixHeteroKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    free : MatrixFreeKept = MatrixFreeKept()
    count : MatrixGenPair
    bat : MatrixGenPair
    served_pf : int64
    served_bat : int64
    cont : MatrixContKept
}

//! The forced-feed stream's Metal side: the freeform prompt
//! prefilled, the stream's ids fed in turn, the row ahead of each kept
def private free_metal(m : Model; stream : array<int64>; var k : MatrixFreeKept) {
    var inscope prompt <- encode(m, CONT_TEXT, true, false)
    k.prompt_len = long_length(prompt)
    with_dasllama_jobque() {
        with_metal_rails(MetalRails()) {
            var sb = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            eval_(m, sb, prompt)
            let df0 = metal_decode_count()
            for (tok in stream) {
                k.rows |> push_clone(sb.logits)
                var one <- [tok]
                eval_(m, sb, one)
                delete one
            }
            k.served = metal_decode_count() - df0
            delete sb
        }
    }
}

//! The forced-feed stream's CPU side: the prompt prefilled, the same
//! ids fed in turn; each held row's distance, and each held Metal row
//! against the CPU row one step back - the bar's control
def private free_cpu(m : Model; stream : array<int64>; var k : MatrixFreeKept) {
    var inscope prompt <- encode(m, CONT_TEXT, true, false)
    var inscope would : array<int64>
    with_dasllama_jobque() {
        var sa = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, sa, prompt)
        var prev : array<float>
        for (tok, r in stream, count()) {
            let q = k.prompt_len - 1l + int64(r)
            would |> push(argmax_of(sa.logits))
            let qd = logits_maxdiff(sa.logits, k.rows[r])
            if (qd > k.fmaxd) {
                k.fmaxd = qd
                k.fmax_at = q
                k.fmax_peak = live_maxabs(sa.logits)
            }
            if (!empty(prev)) {
                let qoff = logits_maxdiff(prev, k.rows[r])
                if (qoff < k.foff_min) {
                    k.foff_min = qoff
                    k.foff_at = q
                }
            }
            prev := sa.logits
            var one <- [tok]
            eval_(m, sa, one)
            delete one
        }
        delete prev
        delete sa
    }
    k.cpu_text = eyeball_text(m, would)
    k.fed_text = eyeball_text(m, stream)
    delete k.rows
}

//! the hetero row's counting generate, B=2 batched decode and the
//! slab-seam cell past dim 4096, on the twin
def private hetero_rest_metal(t : T?; m : Model; tag : string; var kept : MatrixHeteroKept) {
    let count_prompt <- encode(m, counting_prompt(1, 30), true, false)
    let count_prompt2 <- encode(m, counting_prompt(7, 36), true, false)
    let pf0 = metal_prefill_count()
    kept.count.gpu <- metal_generate(m, count_prompt, length(count_prompt) + 8)
    kept.served_pf = metal_prefill_count() - pf0
    let ba0 = metal_batch_step_count()
    with_dasllama_jobque() {
        with_metal_rails(MetalRails()) {
            kept.bat.gpu <- batch_pair_steps(m, count_prompt, count_prompt2, 4)
        }
    }
    kept.served_bat = metal_batch_step_count() - ba0
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (m.config.dim > ADD_RMS_MAX_DIM) {
            past_slab_seam_cell(t, m, tag)
        }
    }
}

//! A stage-3 heterogeneous row on the blob twin (two attention
//! classes, V-from-K, v_norm, out_scale, suppress): the needs, the
//! cells, the forced-feed stream, the counting generate, the B=2 pair
[cold_path]
def matrix_hetero_metal(t : T?; var m : Model; form : ModelForm; p : MatrixHeteroRow;
                        var kept : MatrixHeteroKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        matrix_metal_side(t, m, form, family_row_on(t, "gemma4"), p.file, kept) {
            let want_bits = (MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.sliding
                | MetalNeed.pre_post_norm | MetalNeed.geglu | MetalNeed.v_norm
                | MetalNeed.attn_scale | MetalNeed.out_scale | MetalNeed.qd_neq_dim)
            needs_cell(t, m, p.tag, uint(want_bits))
            matrix_cell(t, m, KVDtype.f16, "{p.tag}/f16")
            free_metal(m, p.stream, kept.free)
            hetero_rest_metal(t, m, p.tag, kept)
            cont_metal(m, kept.cont)
        }
    }
}

//! The hetero row's CPU side: the forced-feed stream replayed, the
//! counting and B=2 truths (the path-keyed cache), the continuation
[cold_path, unused_argument(t)]
def matrix_hetero_cpu(t : T?; var m : Model; form : ModelForm; p : MatrixHeteroRow;
                      var kept : MatrixHeteroKept) {
    matrix_cpu_side(m, form, kept) {
        let path = path_join(models_dir(), p.file)
        free_cpu(m, p.stream, kept.free)
        let count_prompt <- encode(m, counting_prompt(1, 30), true, false)
        let count_prompt2 <- encode(m, counting_prompt(7, 36), true, false)
        with_dasllama_jobque() {
            kept.count.cpu <- cached_ids(path, "gen_c1_30_n8") $ {
                var sc = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                var idsc <- generate(m, sc, count_prompt, length(count_prompt) + 8)
                delete sc
                return <- idsc
            }
            kept.bat.cpu <- cached_ids(path, "b2pair_c1c7_n4") $ {
                return <- batch_pair_steps(m, count_prompt, count_prompt2, 4)
            }
        }
        log_gen_texts(m, "{p.tag} both-paths", kept.count.cpu, kept.count.gpu)
        log_pair_texts(m, "{p.tag} B=2", kept.bat.cpu, kept.bat.gpu)
        cont_cpu(m, kept.cont)
    }
}

//! the forced-feed stream within 8 and its control outside it, the
//! paths engaged, the counting and B=2 pairs token for token
def matrix_hetero_verdict(t : T?; p : MatrixHeteroRow; var kept : MatrixHeteroKept) {
    if (both_sides(t, p.tag, kept.metal, kept.cpu)) {
        let f & = unsafe(kept.free)
        to_log(LOG_INFO, "{p.tag} freeform CPU picks on the fed stream: {f.cpu_text}\n")
        to_log(LOG_INFO,
            ("{p.tag} freeform fed stream (fmaxd {f.fmaxd} at row {f.fmax_at}, CPU max " +
            "|logit| there {f.fmax_peak}): {f.fed_text}\n"))
        t |> success(f.fmaxd < 8.0,
            ("{p.tag}: forced-feed freeform decode logits within 8 of CPU " +
            "over the stream's {length(p.stream)} rows (measured {f.fmaxd})"))
        t |> success(f.foff_min >= 8.0,
            ("{p.tag}: every held GPU row against the CPU row one step " +
            "back lands outside the bar (nearest {f.foff_min} at row {f.foff_at}) - the control"))
        t |> success(f.served > 0l,
            "{p.tag}: the forced-feed freeform decode ENGAGED ({f.served} served)")
        t |> success(kept.served_pf > 0l,
            "{p.tag}: the hetero prefill ENGAGED ({kept.served_pf} served)")
        t |> success(ids_equal(kept.count.cpu, kept.count.gpu),
            "{p.tag}: metal prefill+decode generate matches the CPU truth token-for-token")
        t |> success(kept.served_bat > 0l,
            "{p.tag}: the hetero batch ENGAGED ({kept.served_bat} served)")
        t |> success(ids_equal(kept.bat.cpu, kept.bat.gpu),
            "{p.tag}: B=2 batched decode matches the CPU truth token-for-token")
        cont_verdict(t, p.tag, 8.0, kept.cont)
    }
    delete kept
}

def matrix_hetero_gemma4_12b() : MatrixHeteroRow {
    return <- MatrixHeteroRow(file = "gemma-4-12B-it-Q4_K_M.gguf", stream <- [
        8339l, 236761l, 107l, 100l, 45518l, 107l, 101l, 6372l, 563l, 496l, 9760l, 2591l,
        529l, 5213l, 712l, 63155l, 1018l, 237028l, 1437l, 51881l, 529l, 506l, 1638l, 4068l,
        159050l, 5057l, 6791l, 236754l, 1373l, 528l, 496l, 3605l, 529l, 4171l, 236761l,
        108l, 902l, 672l, 13315l, 236764l, 506l, 623l, 236754l, 236775l, 5057l, 14004l,
        496l, 81964l, 236764l, 4180l, 73887l, 1763l, 236761l, 5715l, 563l, 496l, 25890l,
        529l, 506l, 784l, 2598l, 1280l, 4171l, 236787l, 108l, 236770l, 236761l, 138l, 1018l,
        236754l, 1018l, 1107l, 568l, 19079l, 1178l, 684l, 506l, 4403l, 529l, 496l, 623l,
        46815l, 1373l, 107l, 236778l, 236761l, 138l, 1018l, 236754l, 1018l, 1107l, 2677l,
        568l, 74860l, 5143l, 607l, 1288l, 7846l, 236768l, 107l, 236800l, 236761l, 138l,
        1018l, 236754l, 1018l, 1107l, 568l, 1437l, 623l, 236754l, 236775l, 528l, 623l,
        11480l, 236775l, 563l, 3187l, 1456l, 528l, 1239l, 4458l, 529l, 28166l, 236772l,
        15088l, 10481l, 236768l])
}

//! the largest dense gemma-4; its greedy stream falls into a
//! one-token loop at the 30th row, so the stream is the 29 ids ahead
//! of it
def matrix_hetero_gemma4_31b() : MatrixHeteroRow {
    return <- MatrixHeteroRow(file = "gemma-4-31B-it-Q4_K_M.gguf", tag = "gemma4-31b/k4",
        stream <- [
        11895l, 236761l, 1293l, 1053l, 11742l, 528l, 506l, 1638l, 18515l, 573l, 28959l,
        1518l, 236764l, 532l, 668l, 1053l, 3472l, 1551l, 48312l, 236764l, 840l, 672l, 886l,
        6345l, 1607l, 236761l, 1174l, 886l, 6345l])
}

//! two streams prefilled on `m`, `nsteps` greedy eval_batch steps
//! (whatever decode override is selected): the picks interleaved,
//! each prompt's own first
def private batch_pair_steps(m : Model; pa, pb : array<int64>; nsteps : int) : array<int64> {
    var inscope ps : array<array<int64>>
    ps |> push_clone(pa)
    ps |> push_clone(pb)
    var inscope rows <- batch_greedy_rows(m, ps, nsteps, with_first = true)
    return <- [for (i in range(2 * (nsteps + 1))); rows[i % 2][i / 2]]
}

//! the B=2 cells' eyeball rail: the pair streams de-interleaved
def private log_pair_texts(m : Model; tag : string; cpu, gpu : array<int64>) {
    for (ph in range(2)) {
        var inscope c <- [for (i in range(length(cpu))); cpu[i]; where i % 2 == ph]
        var inscope g <- [for (i in range(length(gpu))); gpu[i]; where i % 2 == ph]
        to_log(LOG_INFO, "{tag} stream {ph} CPU text: {eyeball_text(m, c)}\n")
        to_log(LOG_INFO, "{tag} stream {ph} GPU text: {eyeball_text(m, g)}\n")
    }
}

//! B=4 batched steps under teacher-forced tokens (a synthetic feed,
//! the same on both passes): the four final-step rows concatenated
def private batch_quad_forced_logits(trp, trd : Model; pa, pb, pc, pd : array<int64>;
                                     nsteps : int) : array<float> {
    let c = trp.config
    let vocab = c.vocab_size
    var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sc = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sd = make_run_state(c, KVDtype.f16, KVDtype.f16)
    eval_(trp, sa, pa)
    eval_(trp, sb, pb)
    eval_(trp, sc, pc)
    eval_(trp, sd, pd)
    var ws <- create_batch_workspace_(trp)
    var sess : array<Session?>
    unsafe {
        sess |> push(addr(sa))
        sess |> push(addr(sb))
        sess |> push(addr(sc))
        sess |> push(addr(sd))
    }
    var cur <- [0l, 0l, 0l, 0l]
    for (st in range(nsteps)) {
        for (i in range(4)) {
            cur[i] = 1000l + int64(st) * 7l + int64(i)
        }
        eval_batch_(trd, ws, sess, cur)
    }
    var out : array<float>
    out |> reserve(vocab * 4l)
    out |> push_from(sa.logits, sb.logits, sc.logits, sd.logits)
    sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
    delete cur
    delete sa
    delete sb
    delete sc
    delete sd
    return <- out
}

let private HETERO_Q8_FILE = "gemma-4-12B-it-Q8_0.gguf"

//! what the hetero q8 row keeps: the generates, the B=2 pairs, the
//! B=4 forced rows a side, the served batch count, the continuation
struct MatrixHeteroQ8Kept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    vocab : int64
    gen : MatrixGenPair
    b2 : MatrixGenPair
    b4g : array<float>
    b4c : array<float>
    b4_eye : string
    served_bat : int64
    cont : MatrixContKept
}

//! the hetero q8 row's four counting prompts
def private q8_prompts(m : Model) : array<array<int64>> {
    var ps : array<array<int64>>
    ps |> reserve(4)
    for (r in [int2(1, 30), int2(7, 36), int2(3, 41), int2(11, 29)]) {
        ps |> emplace <| encode(m, counting_prompt(r.x, r.y), true, false)
    }
    return <- ps
}

//! The q8-format twin of the hetero row on the blob twin: batched q8
//! sites ride the fixed-B mul_mv rail (MvB2/B4); the generate, the
//! B=2 pair, the B=4 teacher-forced rows (counting continuations hit
//! a step-3 near-tie, so B=4 is a logits compare), the continuation
[cold_path]
def matrix_hetero_q8_metal(t : T?; var m : Model; form : ModelForm; var kept : MatrixHeteroQ8Kept) {
    matrix_metal_side(t, m, form, family_row_on(t, "gemma4"), HETERO_Q8_FILE, kept) {
        kept.vocab = m.config.vocab_size
        var inscope ps <- q8_prompts(m)
        kept.gen.gpu <- metal_generate(m, ps[0], length(ps[0]) + 8)
        let ba0 = metal_batch_step_count()
        with_dasllama_jobque() {
            with_metal_rails(MetalRails()) {
                kept.b2.gpu <- batch_pair_steps(m, ps[0], ps[1], 4)
                kept.b4g <- batch_quad_forced_logits(m, m, ps[0], ps[1], ps[2], ps[3], 4)
            }
        }
        kept.served_bat = metal_batch_step_count() - ba0
        cont_metal(m, kept.cont)
    }
}

//! the hetero q8 row's CPU truths through the path-keyed cache, the
//! continuation
[cold_path, unused_argument(t)]
def matrix_hetero_q8_cpu(t : T?; var m : Model; form : ModelForm; var kept : MatrixHeteroQ8Kept) {
    matrix_cpu_side(m, form, kept) {
        let path = path_join(models_dir(), HETERO_Q8_FILE)
        var inscope ps <- q8_prompts(m)
        with_dasllama_jobque() {
            kept.gen.cpu <- cached_ids(path, "gen_c1_30_n8") $ {
                var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                var ids <- generate(m, s, ps[0], length(ps[0]) + 8)
                delete s
                return <- ids
            }
            kept.b2.cpu <- cached_ids(path, "b2pair_c1c7_n4") $ {
                return <- batch_pair_steps(m, ps[0], ps[1], 4)
            }
            kept.b4c <- cached_vals(path, "b4forced_n4") $ {
                return <- batch_quad_forced_logits(m, m, ps[0], ps[1], ps[2], ps[3], 4)
            }
        }
        log_gen_texts(m, "gemma4-12b/q8", kept.gen.cpu, kept.gen.gpu)
        log_pair_texts(m, "gemma4-12b/q8 B=2", kept.b2.cpu, kept.b2.gpu)
        var b4c, b4g : array<int64>
        for (s4 in range64(4l)) {
            b4c |> push(argmax_of(kept.b4c, s4 * kept.vocab, kept.vocab))
            b4g |> push(argmax_of(kept.b4g, s4 * kept.vocab, kept.vocab))
        }
        kept.b4_eye = ("CPU forced-final picks '{eyeball_text(m, b4c)}' vs GPU " +
            "'{eyeball_text(m, b4g)}'")
        delete b4c
        delete b4g
        cont_cpu(m, kept.cont)
    }
}

//! the hetero q8 row's compares: the generate and B=2 token for
//! token, the B=4 forced rows within 8
def matrix_hetero_q8_verdict(t : T?; var kept : MatrixHeteroQ8Kept) {
    if (both_sides(t, "gemma4-12b/q8", kept.metal, kept.cpu)) {
        let d4 = logits_maxdiff(kept.b4c, kept.b4g)
        to_log(LOG_INFO, "gemma4-12b/q8 B=4 eyeball: {kept.b4_eye} (maxd {d4})\n")
        t |> success(ids_equal(kept.gen.cpu, kept.gen.gpu),
            "gemma4-12b/q8: metal-decode generate matches the CPU truth token-for-token")
        t |> success(kept.served_bat > 0l,
            "gemma4-12b/q8: the hetero q8 batch ENGAGED ({kept.served_bat} served)")
        t |> success(ids_equal(kept.b2.cpu, kept.b2.gpu),
            "gemma4-12b/q8: B=2 batched decode (MvB2) matches the CPU truth token-for-token")
        t |> success(d4 < 8.0,
            "gemma4-12b/q8: B=4 forced-feed batched logits (MvB4) within 8 of CPU (measured {d4})")
        cont_verdict(t, "gemma4-12b/q8", 8.0, kept.cont)
    }
    delete kept
}

//! past the 4096-float slab (`ADD_RMS_MAX_DIM`) the residual seams of
//! the twin's decode, two single steps and a batched step over
//! device-prefilled prompts: the fuse rail pinned on, every seam one
//! dispatch on the 1024-lane builders; pinned off, the control, every
//! seam's split chain norm passes; the rail and the overrides put
//! back
def private past_slab_seam_cell(t : T?; trg : Model; tag : string) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = trg.config
        t |> success(c.dim > ADD_RMS_MAX_DIM, "{tag}: the row is past the slab (dim {c.dim})")
        let pa <- encode(trg, counting_prompt(1, 30), true, false)
        let pb <- encode(trg, counting_prompt(7, 36), true, false)
        let fuse = get_metal_decode_fuse()
        with_metal_rails(t, tag, MetalRails()) {
            var fused = int2(0, 0)   //! the fused run's stacks and norm passes
            for (on in fixed_array(true, false)) {
                set_metal_decode_fuse(on ? 1 : 0)
                fused = seam_leg(t, trg, tag, on, pa, pb, fused)
            }
            set_metal_decode_fuse(fuse)
        }
    }
}

//! one leg of the slab-seam cell: the steps and the batched step, the
//! seam census read against the fused run's (`fused`), back its own
def private seam_leg(t : T?; trg : Model; tag : string; on : bool; pa, pb : array<int64>;
                     fused : int2) : int2 {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let nl = int(trg.config.n_layers)
        let vocab = trg.config.vocab_size
        var sa = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        var sb = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        eval_(trg, sa, pa)
        eval_(trg, sb, pb)
        let sm0 = seam_counts()
        let ln0 = metal_residual_lanes()
        let st0 = metal_decode_count()
        let ba0 = metal_batch_step_count()
        for (_i in range(2)) {
            var one <- [argmax_of(sa.logits, vocab)]
            eval_(trg, sa, one)
            delete one
        }
        var ws <- create_batch_workspace_(trg)
        var sess : array<Session?>
        unsafe {
            sess |> push(addr(sa))
            sess |> push(addr(sb))
        }
        var cur <- [argmax_of(sa.logits, vocab), argmax_of(sb.logits, vocab)]
        eval_batch_(trg, ws, sess, cur)
        let sm = seam_counts() - sm0
        let ln = metal_residual_lanes()
        let lanes = (narrow = ln.narrow - ln0.narrow, wide = ln.wide - ln0.wide)
        let nst = metal_decode_count() - st0
        let nba = metal_batch_step_count() - ba0
        let name = on ? "fused" : "split"
        to_log(LOG_INFO, ("{tag} past the slab, {name}: {nst} steps, {nba} batched - pre_add_rms " +
            "{sm.x}, pre_add {sm.y}, add_rms {sm.z}, rms {sm.w}; one dispatches on 256 lanes " +
            "{lanes.narrow}, on 1024 lanes {lanes.wide}\n"))
        t |> success(nst >= 2l && nba == 1l,
            "{tag} {name}: the steps and the batched step ran on the device ({nst}, {nba})")
        let out = seam_leg_asserts(t, tag, on, nl, nst + nba, sm, lanes, fused)
        sess |> clear()   //! borrowed handles: delete of array<T?> cascades to the pointees
        unsafe {
            delete ws
            delete sess
        }
        delete cur
        delete sa
        delete sb
        return out
    } else {
        return fused
    }
}

//! the slab-seam leg's census: fused, every seam one dispatch on the
//! 1024-lane builders; split, no one-dispatch seam and every seam's
//! split chain norm passes past the fused run's
def private seam_leg_asserts(t : T?; tag : string; on : bool; nl : int; ran : int64; sm : int4;
                             lanes : tuple<narrow : int64; wide : int64>; fused : int2) : int2 {
    if (on) {
        let nenc = (sm.x + sm.y) / (2 * nl)   //! the encoded layer stacks: two seams a layer
        t |> success(int64(nenc) >= ran && sm.x + sm.y == nenc * 2 * nl,
            ("{tag} fused: every seam one dispatch ({sm.x} + {sm.y} over {nenc} stacks of {nl} " +
            "layers)"))
        t |> equal(sm.z, 0, "{tag} fused: no plain seam on the sandwich")
        t |> equal(lanes.wide, int64(sm.x + sm.y),
            "{tag} fused: every seam on the decode rows' 1024-lane builders")
        t |> equal(lanes.narrow, 0l, "{tag} fused: no seam on the 256-lane builders")
        return int2(nenc, sm.w)
    }
    t |> equal(sm.x + sm.y, 0, "{tag} split: no one-dispatch seam - the control")
    t |> equal(lanes.narrow + lanes.wide, 0l,
        "{tag} split: no residual one dispatch on either lane width")
    t |> equal(sm.w - fused.y, fused.x * (4 * nl - 1),
        ("{tag} split: every seam's split chain norm passes past the fused run's ({sm.w} " +
        "against {fused.y}, {fused.x} stacks)"))
    return fused
}

//! the families whose tensor twin a box's tune sidecar may crown,
//! forced on so every twin serves; a new tensor family joins it
let private TENSOR_CROWNED = ("attn_avmm,attn_qkmm,gemm64b_q8,gemmb_q8,gemmb_sk_q8,kq_mulmm_k4," +
    "kq_mulmm_k5,kq_mulmm_k6,moe_mulmm_mx4,moe_mulmm_q8,moe_mulmm_k4,moe_mulmm_k5,moe_mulmm_k6," +
    "mulmm_bf16,mulmm_q8,moe_mulmm_k3,moe_mulmm_k2,moe_mulmm_iq4xs,moe_mulmm_iq3s4," +
    "moe_mulmm_iq4nl," +
    "moe_mulmm_q40,moe_mulmm_nv4,moe_mulmm_iq3s,moe_mulmm_iq3xxs,moe_mulmm_iq2s,moe_mulmm_iq2xs," +
    "moe_mulmm_iq2xxs,kq_mulmm_nv4,kq_mulmm_iq3s4,kq_mulmm_q51,kq_mulmm_iq4nl32,kq_mulmm_q4032," +
    "kq_mulmm_q5032,moe_mulmm_q51,moe_mulmm_iq4nl32,moe_mulmm_q4032,moe_mulmm_q5032," +
    "kq_mulmm_iq4xs,kq_mulmm_iq4nl,kq_mulmm_k3,kq_mulmm_iq3s,kq_mulmm_iq3xxs," +
    "kq_mulmm_k2,kq_mulmm_iq2s,kq_mulmm_iq2xs,kq_mulmm_iq2xxs," +
    "kq_mulmm_tq2,kq_mulmm_tq1,kq_mulmm_iq1s,kq_mulmm_iq1m,kq_mulmm_q20,kq_mulmm_q10," +
    "moe_mulmm_tq2,moe_mulmm_tq1,moe_mulmm_iq1s,moe_mulmm_iq1m,moe_mulmm_q20,moe_mulmm_q10," +
    "kq_gemv_iq3s_f4,kq_gemv_iq3xxs_f4,kq_gemv_iq2xxs_f4,hmm_tall")

//! the parameters of one tensor-twin row
struct MatrixTensorRow {
    file : string
    tag : string
}

//! what a tensor-twin row keeps: both sides' generates
struct MatrixTensorKept {
    metal : bool
    s32 : bool   //!< the Metal form's f32 weight scale: its CPU form's
    cpu : bool
    gen : MatrixGenPair
}

//! the tensor twins that served since the census reset, each logged
def private tensor_twins_served() : int {   // nolint:LINT012,LINT019
    var twins = 0
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope cov <- metal_kernel_coverage()
        for (name, n in keys(cov), values(cov)) {
            if (n > 0l && ends_with(name, "_t")) {
                twins++
                to_log(LOG_INFO, "tensor twin served: {name} = {n}\n")
            }
        }
    }
    return twins
}

//! A tensor-twin row on the blob twin: the crowns forced, the drivers
//! shut down so the PSOs recompile under them, a generate with twins
//! in the graph; the crowns put back and the drivers shut down again.
//! Shuts the drivers down mid-cell
[cold_path]
def matrix_tensor_metal(t : T?; var m : Model; form : ModelForm; p : MatrixTensorRow;
                        var kept : MatrixTensorKept) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        matrix_metal_side(t, m, form, family_on(t, "llama") && arm_on(t, "tensor"), p.file,
                kept) {
            let prompt <- encode(m, counting_prompt(1, 30), true, false)
            let crowns0 = get_metal_tensor_crowns()
            //! read when the PSOs compile: set before the rebuild
            set_metal_tensor_crowns(TENSOR_CROWNED)
            metal_decode_shutdown()
            metal_prefill_shutdown()
            kernel_census_reset_metal()
            kept.gen.gpu <- metal_generate(m, prompt, length(prompt) + 8)
            let twins = tensor_twins_served()
            //! without this the cell passes vacuously the day
            //! crowning stops taking effect
            t |> success(twins > 0,
                "{p.tag}: a crowned tmm2d twin actually served ({twins} of them)")
            set_metal_tensor_crowns(crowns0)
            metal_decode_shutdown()
            metal_prefill_shutdown()
        }
    }
}

//! the tensor-twin row's CPU truth: the generate
[cold_path, unused_argument(t)]
def matrix_tensor_cpu(t : T?; var m : Model; form : ModelForm; p : MatrixTensorRow;
                      var kept : MatrixTensorKept) {
    matrix_cpu_side(m, form, kept) {
        let prompt <- encode(m, counting_prompt(1, 30), true, false)
        kept.gen.cpu <- cpu_generate(m, prompt, length(prompt) + 8)
        log_gen_texts(m, p.tag, kept.gen.cpu, kept.gen.gpu)
    }
}

//! the crowned generate token for token the CPU truth
def matrix_tensor_verdict(t : T?; p : MatrixTensorRow; var kept : MatrixTensorKept) {
    gen_pair_verdict(t, p.tag,
        "{p.tag}: crowned tmm2d generate matches the CPU truth token-for-token", kept)
}

//! q8 drives the mulmm and attention twins
def matrix_tensor_q8() : MatrixTensorRow => MatrixTensorRow(
    file = "Llama-3.2-1B-Instruct-Q8_0.gguf", tag = "tensor-twins/q8")

//! the K-quant row drives the kq mulmm twins
def matrix_tensor_k5() : MatrixTensorRow => MatrixTensorRow(
    file = "Llama-3.2-1B-Instruct-Q5_K_M.gguf", tag = "tensor-twins/k5")

//! the iquant crown witness: a local requant with iq2xxs planes, the
//! new crown names reaching a served twin (mulmm + the f4 gemv crown)
def matrix_tensor_iq2xxs() : MatrixTensorRow => MatrixTensorRow(
    file = "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf", tag = "tensor-twins/iq2xxs")

let MATRIX_QWEN4EXP_FILE = ("Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/" +
    "Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf")
let MATRIX_QWEN4EXP_HEAD = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf"

//! The one-row walk with its next step encoded ahead against the same
//! walk with nothing encoded ahead: the held step reads and writes
//! the session's n-gram ring where the slow path does, so the two
//! walks' logits are the same row for row
def private qwen4exp_ahead_cell(t : T?; trg : Model; steps : int64) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vocab = trg.config.vocab_size
        var inscope prompt <- encode(trg, ("Write a complete, well-commented Python module that " +
            "implements an LRU cache with TTL expiry, a thread-safe wrapper around it, and a " +
            "pytest " +
            "test suite."), true)
        var a = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        var b = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        var inscope picks : array<int64>
        var inscope rows : array<array<float>>
        var inscope one <- [0l]
        picks |> reserve(steps)
        rows |> reserve(steps)
        eval_(trg, a, prompt)
        for (_j in range64(steps)) {
            one[0] = argmax(a, vocab)
            picks |> push(one[0])
            rows |> emplace <| logits_of(a, vocab)
            eval_(trg, a, one)
        }
        eval_(trg, b, prompt)
        var worst = 0.0
        var flips = 0l
        var first = -1l
        for (j in range64(steps)) {
            mirror_quiesce()   //! nothing encoded ahead: this walk's every step takes the slow path
            var inscope lb <- logits_of(b, vocab)
            worst = max(worst, logits_maxdiff(rows[j], lb) / max(logits_maxabs(rows[j]), 1.0))
            if (argmax(b, vocab) != picks[j]) {
                flips++
                first = first < 0l ? j : first
            }
            one[0] = picks[j]
            eval_(trg, b, one)
        }
        t |> success(flips == 0l,
            ("qwen4exp ahead: the walk encoded ahead and the walk with nothing " +
            "encoded ahead pick the same token at every step ({flips} of {steps} differ, the " +
            "first at {first})"))
        t |> success(worst <= 2e-3,
            ("qwen4exp ahead: the two walks' logits agree row for row (worst " +
            "{worst} of the row's scale, bar 2e-3)"))
        delete a
        delete b
    }
}

//! the served image's counting walk against the file's frozen
//! fixture: the prefill and every decode step counted, no decline
def private qwen4exp_count_cell(t : T?; trg : Model) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let fx <- spec_by_file(MATRIX_QWEN4EXP_FILE).parity
        t |> success(!empty(fx.gen), "qwen4exp: the file carries its frozen counting fixture")
        let n_prompt = length(fx.prompt)
        let pf0 = metal_prefill_count()
        let de0 = metal_decode_count()
        let pk0 = metal_decode_spec_stats().poked
        var inscope pdecl0 <- metal_prefill_declines()
        var inscope ddecl0 <- metal_decode_declines()
        var inscope ids <- metal_generate(trg, fx.prompt, n_prompt - 1 + length(fx.gen))
        var inscope got <- [for (i in range(n_prompt - 1, length(ids))); ids[i]]
        log_gen_texts(trg, "qwen4exp", fx.gen, got)
        t |> success(ids_equal(fx.gen, got),
            ("qwen4exp: the Metal prefill and decode continue the " +
            "counting prompt token for token the CPU truth ({length(got)} tokens)"))
        t |> equal(metal_prefill_count() - pf0, 1l,
            "qwen4exp prefill: the Metal driver served the prompt")
        let nde = metal_decode_count() - de0
        let npk = metal_decode_spec_stats().poked - pk0
        t |> success(nde >= int64(length(fx.gen) - 1),
            "qwen4exp decode: the Metal driver served every step ({nde})")
        t |> success(npk >= int64(length(fx.gen) - 2),
            ("qwen4exp decode: every step after the first " +
            "was encoded ahead and committed on the poked token ({npk} of {length(fx.gen) - 1})"))
        var inscope pdecl1 <- metal_prefill_declines()
        var inscope ddecl1 <- metal_decode_declines()
        t |> equal("{pdecl1}", "{pdecl0}", "qwen4exp prefill: no decline")
        t |> equal("{ddecl1}", "{ddecl0}", "qwen4exp decode: no decline")
    }
}

//! Qwen3.8-Flash-Next on the served image with its split head pinned
//! (`metal_form(image = true, mtp_head = MATRIX_QWEN4EXP_HEAD)`),
//! speculation off: the planar file and its image do not fit the box
//! together, so the truth is the file's frozen counting fixture
[cold_path]
def matrix_qwen4exp_metal(t : T?; var m : Model; form : ModelForm) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!family_on(t, "qwen4exp") || !arm_on(t, "fam-qwen4exp"))
        if (!form.metal_image || form.mtp_head != MATRIX_QWEN4EXP_HEAD) {
            t |> failure(("qwen4exp: the row runs on the served image with its split head pinned " +
                "(`metal_form(image = true, mtp_head = MATRIX_QWEN4EXP_HEAD)`)"))
            return
        }
        with_ctx_cut(m, MATRIX_CTX) {
            t |> success(m.metal_blob && m.config.hyper_conn,
                "qwen4exp: the served image is the metal-blob hyper-connection carrier")
            let spec0 = get_mtp_spec()
            set_mtp_spec(false)
            qwen4exp_count_cell(t, m)
            //! the counting row's counts are read; the walks prefill
            //! twice more
            with_dasllama_jobque() {
                with_metal_rails(MetalRails()) {
                    qwen4exp_ahead_cell(t, m, 96l)
                }
            }
            set_mtp_spec(spec0)
        }
    }
}

//! a release cycle's footprint bar over the first cycle's close:
//! under a fifth of the E2B blob a cycle would keep
let MATRIX_RELEASE_SLACK_BYTES = 512ul * 1024ul * 1024ul

//! one release cycle's serve on the blob twin `trg` the caller loaded
//! (`MATRIX_CTX`): four greedy steps, the device serving them
def matrix_release_serve(t : T?; trg : Model; tag : string;
                         cycle : int) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompt <- encode(trg, counting_prompt(1, 20), true, false)
        let de0 = metal_decode_count()
        var inscope ids <- metal_generate(trg, prompt, length(prompt) + 4)
        let ngen = length(ids) - length(prompt)
        let served = metal_decode_count() - de0
        t |> success(ngen == 4 && served > 0l,
            "{tag}/release cycle {cycle}: {ngen} steps, {served} served on the device")
    }
}
