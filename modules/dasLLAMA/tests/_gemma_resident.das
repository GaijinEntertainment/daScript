// The gemma cells of the whole-model resident driver, shared by the gemma resident test files
// (test_gpu_resident_gemma*.das): a cell loads its carrier through the rig and deletes it before
// it returns, so several carriers share a file. The GPU chains and the CPU chain are not bit-identical
// (GPU exp / f32 accumulation order, the prefill's f16 activation feed against the CPU's Q8
// activation blocks), so the gates are: the forced-feed logits-tolerance form (the same prose
// tokens fed to both arms, the logits within a bar of the CPU chain after the prefill and at every
// step, the one-step-off control past it), the agreement form (the window chain and the token
// command against each other on the same tokens, the CPU chain's own two paths the yardstick),
// and the perplexity form (teacher-forced positions after a prefill, the resident's perplexity
// within a ratio of the CPU chain's) for the models whose logits sit flat over whole stretches.

options gen2
options _dasllama_internal = true

module _gemma_resident shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, Session, forward_prefill, forward
require dasllama/dasllama   // create_session, moe_gpu_drop_model; its umbrella fires each arch [init] registration (gemma3)

//! the CPU pre-step's side row of `tok` on a scratch session: the scaled embedding row, then `ple_pre_decode` (the device
//! pre-step cells' reference on either discrete home)
def cpu_side_row(tr : Model; tok : int64) : array<float> {
    var s <- make_run_state(tr.config)
    embed_text_rows(tr, [tok], s.x, 0l)
    ple_pre_decode(tr, s, tok)
    let n = tr.config.n_layers * tr.config.n_embd_per_layer
    var row <- [for (i in range64(n)); s.ple_inp[i]]
    delete s
    return <- row
}

//! the device's side row `dev` of token `tok` against the CPU pre-step's row at `rel` of its largest magnitude (the token command's
//! row lands within 1e-4 - the projection without the 1/sqrt(dim) reads the eps alone apart; the window's rows feed the projection
//! as f16 and land within 1e-3), another token's CPU row the control
def side_row_within(t : T?; dev : array<float>; tr : Model; tok : int64; what : string; rel : float) {
    var inscope cpu <- cpu_side_row(tr, tok)
    var inscope other <- cpu_side_row(tr, tok + 1l)
    side_rows_within(t, dev, cpu, other, what, rel)
}

//! `side_row_within` on the CPU pre-step's rows already built: `cpu` the token's, `other` the control token's
def side_rows_within(t : T?; dev, cpu, other : array<float>; what : string; rel : float) {
    let mx = logits_maxdiff(dev, cpu)   // a NaN on either side reads as infinite
    var big = 0.0   // the bar's scale: the CPU row's largest finite magnitude
    for (v in cpu) {
        big = max(big, abs(v))
    }
    let bar = rel * big
    to_log(LOG_INFO, "{moe_gpu_backend_name()} e2b pre-step ({what}): device side row against the CPU's: maxdiff {mx} (bar {bar}, largest {big})\n")
    t |> success(mx <= bar, "{what}: the device side row within {rel} of the CPU's largest ({mx} of {bar})")
    check_exceeds(t, "{what}: another token's CPU row against the device row", dev, other, Bar(kind = BarKind.rel_of_max, rel = rel))
}

//! the rig's context: the resident mirror's cap and every session's KV cache - a model max of 131072 would take an
//! 8 GB mirror on a 4B and a 30 GB CPU cache on the 26B for prompts of 600
let GEMMA_RIG_CTX = 32768l

//! one gemma fixture: its file under models_dir() and the arm token that selects its cells
struct GemmaFile {
    file : string
    arm : string
    host_experts : bool   //! the load asks the rig's context whole and at least one routed layer's experts onto the host: the driver arms with the first routed layers' experts on the host on any card
}

[init]
def private gemma_rig_init() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}
require math
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require _model_tier public   // models_dir() + model_available() presence/size-tier gate, arm_on, live_maxabs, SUPPRESSED_LOGIT
require _cpu_gpu_model public   // with_cpu_and_gpu_model, ModelForm, the feed rows
require _resident_feed public   // resident_ready, load_resident, resident_then_cpu_feed, feed_steps_within, feed_logits
require _format_census   // census_cell: the routed files' every site in its own format
require _compares   // INFINITE_DIFF: the agreement form's control read finite
require _child_run   // run_child / child_cmd - the load-log child

//! the logits bar, a fraction of the step's max live |logit|. gemma-3-1b's logits move with the activation rounding itself: the CPU
//! chain's OWN prefill kernel and decode kernel land 0.06-0.10 of the max logit apart on the same
//! tokens (0.87 of 15.3 at eight tokens, 0.45 of 13.4 at forty), and the resident arms sit a further
//! factor off it (the prefill feeds the GEMMs f16 rows where the CPU quantizes Q8 blocks); the
//! one-step-off controls read 5 to 19, so the bar keeps its teeth
let GEMMA_LOGIT_BAR_REL = 0.16
//! the resident window chain and token command may sit this many times further apart than the CPU chain's own two paths on the same tokens (measured 1.6 to 4.8 on gemma-3-1b, by the prefill's activation feed)
let GEMMA_PATH_GAP_RATIO = 6.0
//! the resident driver's teacher-forced perplexity may exceed the CPU chain's by this ratio (12B Q8_0 measured 1.449 vs 1.516 over 150 positions, the reference exe 1.390)
let GEMMA_PPL_RATIO = 1.05
//! the routed 26B's ratio: a router near-tie flips whole positions, so either arm can land the farther one, and a kernel
//! recompile alone moves a 150-position cell - the Q4_K_M file read 1.29 to 1.84 of the CPU chain across the gpt-oss
//! arc's value-identical kernel rewrites, the IQ3_XXS file 0.68 to 1.71 - so the bar sits past that band (the readings: CLAUDE.md)
let GEMMA_MOE_PPL_RATIO = 2.0

//! the fixture loaded on the armed driver, or an empty model (`n_layers` 0) after the skip that says why
def private gemma_model(t : T?; gf : GemmaFile) : Model {
    let path = path_join(models_dir(), gf.file)
    let levers = resident_levers()
    set_gpu_ctx_max(GEMMA_RIG_CTX)   //! the rig's mirror context, read as the driver arms; the levers go back after the load
    var tr <- resident_trunk(t, path, GEMMA_RIG_CTX, " - DASLLAMA_GPU_MIN_CTX=1024 arms the 12B Q8_0 file on a 16 GB card") $() : Model {
        var m <- load_resident_host(path, 0l, gf.host_experts ? GEMMA_RIG_CTX : 0l, gf.host_experts ? 1l : 0l)
        if (gf.host_experts) {   //! the pinned cap and the one routed layer asked onto the host decide the plan whatever the card (a card past the rig's holds the stacks beside the cap): a memory decline is the form missing, never a skip
            t |> success(moe_gpu_resident_active(), "the whole-model driver armed with host experts ({gpu_resident_decline_()})")
        }
        return <- m
    }
    restore_resident_levers(levers)
    return <- tr if (tr.config.n_layers == 0l)
    if (gf.host_experts) {
        let host = gpu_resident_host_layers()
        t |> success(host > 0l && host < tr.config.n_layers, "the first {host} of {tr.config.n_layers} routed layers' experts serve on the host, the rest on the device")
    }
    t |> success(gemma_carrier_arms(tr), GEMMA_CARRIER_ARMS)
    return <- tr
}

let GEMMA_CARRIER_ARMS = "the fixture carries the sandwich norms, a sliding window and the GeGLU FFN (the arms this file gates)"

//! the model carries the arms the gemma cells gate: the sandwich norms, a sliding window and the GeGLU FFN
def gemma_carrier_arms(tr : Model) : bool => tr.config.pre_post_norm && tr.config.sliding_window > 0l && tr.config.ffn_act == FfnAct.gelu

//! the gemma cell's rig: the fixture on the armed driver (or the cell skipped, saying why - its arm filtered among the reasons),
//! `body` inside the engine's job queue, the model deleted after
def with_gemma_rig(t : T?; gf : GemmaFile; body : block<(tr : Model) : void>) {
    return if (!arm_on(t, gf.arm))
    var tr <- gemma_model(t, gf)
    if (tr.config.n_layers > 0l) {
        with_dasllama_jobque_() {
            invoke(body, tr)
        }
    }
    delete tr
}

//! the routed model's weight-format census on the armed driver's load: every site the file carries in its own format, the
//! expert stacks the native-MXFP4 planes exactly where `mx4` names them, then a 40-token prompt and two fed steps on the driver
//! with every call served, the steps' mx4 stamps counted where `mx4` and none dispatched where not
[cold_path]
def gemma_format_cell(t : T?; gf : GemmaFile; mx4 : bool) {
    with_gemma_rig(t, gf) $(tr) {
        gemma_format_census(t, tr, gf.file, mx4)
    }
}

//! `gemma_format_cell`'s census and feed on `tr`, the file `file`
//! armed on the driver by the caller
def gemma_format_census(t : T?; tr : Model; file : string; mx4 : bool) {
    census_cell(t, tr, path_join(models_dir(), file), file, ["attention q/k/v/o", "routed experts", "token table", "classifier"])
    t |> equal(tr.experts_mx4, mx4, "{file}: the expert stacks load as the native-MXFP4 planes exactly where the file carries them")
    var inscope feed <- prose_feed(tr, 40l, 2l)
    var sg <- create_session(tr, mirror_kv_dtype())
    let served0 = gpu_resident_prefills_()
    //! the routed down's stamp: the Vulkan token command folds the k slots
    //! in its summed down; the CUDA home's runs the per-slot GEMV, its
    //! combine summing the slots
    let down_key = gpu_want_cuda() ? "mx4_gemv_cls_spv" : "mx4_gemv_sum_cls_spv"
    let down_name = "{gpu_want_cuda() ? "per-slot down" : "summed down"} ({cuda_knob()})"
    let gu0 = census("mx4_gemv_gu_cls_spv")
    let sum0 = census(down_key)
    var rows <- feed_logits(tr, sg, feed.prompt, feed.toks) $ {}
    t |> equal(gpu_resident_prefills_(), served0 + 1l, "{file}: the resident window chain served the prompt")
    served_every_call(t)
    if (gu0 != NO_CENSUS) {   //! the fed steps' routed sites on the mx4 stamps: the fused gate + up and the down
        let gu_n = census("mx4_gemv_gu_cls_spv") - gu0
        let sum_n = census(down_key) - sum0
        if (mx4) {
            t |> success(gu_n > 0l && sum_n > 0l, "{file}: the steps' routed gate + up ({gu_n}) and {down_name} ({sum_n}) on the mx4 stamps")
        } else {   //! the control: a file with no MXFP4 stack dispatches neither stamp
            t |> equal(gu_n + sum_n, 0l, "{file}: no mx4 stack, no mx4 stamp dispatched (gate + up {gu_n}, {down_name} {sum_n})")
        }
    }
    for (r, i in rows, count()) {
        let bad = length([for (v in r); v; where !is_finite(v)])
        t |> equal(bad, 0, "{file}: row {i}'s logits finite")
    }
    delete rows
    delete sg
}

let RESIDENT_RING_ON = "dasLLAMA: resident driver - the sliding layers' K/V rows ride rings of their window plus"
let RESIDENT_RING_OFF = "dasLLAMA: resident driver - the sliding layers' K/V rows keep every position's row, no ring, as asked"
let RESIDENT_EMBED_OFF = "dasLLAMA: resident driver - the host embeds every token row as asked"

//! the whole-model driver's load log for `gf` - what to_log prints, which no in-process cell reads - from one resident load in
//! the test child (its `resident-load` verb) at the context `ctx` asked whole (0 none) under `envs`; "" with the skip `resident_ready` names
def resident_load_log(t : T?; gf : GemmaFile; ctx : int64; envs : array<tuple<name : string; value : string>>) : string {
    let path = path_join(models_dir(), gf.file)
    return "" if (!resident_ready(t, path))
    var out : string
    let rc = run_child(child_verb_cmd("resident-load", envs, "\"{path}\" {ctx}"), out)
    t |> equal(rc, 0, "the load root ran:\n{out}")
    t |> success(find(out, "RESIDENT_LOAD active=true") >= 0, "the whole-model driver armed in the child:\n{out}")
    return out
}

//! The resident driver's load-time announces off its log under both pin states: the ringed load names its rings, the flat one
//! (`DASLLAMA_GPU_KV_RING=0`) its every-row mirror as asked; with the token gather off (`DASLLAMA_VK_GPU_EMBED=0`) the host embed
//! says so, a gathering load saying nothing
[cold_path]
def ring_load_announces(t : T?; gf : GemmaFile) {
    return if (!arm_on(t, gf.arm))
    let ringed = resident_load_log(t, gf, 0l, [(name = "DASLLAMA_GPU_KV_RING", value = "1"), (name = "DASLLAMA_VK_GPU_EMBED", value = "1"),
        (name = "DAS_LOG_LEVEL", value = "info")])
    return if (empty(ringed))
    let flat = resident_load_log(t, gf, 0l, [(name = "DASLLAMA_GPU_KV_RING", value = "0"), (name = "DASLLAMA_VK_GPU_EMBED", value = "0"),
        (name = "DAS_LOG_LEVEL", value = "info")])
    t |> success(find(ringed, RESIDENT_RING_ON) >= 0 && find(ringed, RESIDENT_RING_OFF) < 0, "the ringed load names its rings alone:\n{ringed}")
    t |> success(find(flat, RESIDENT_RING_OFF) >= 0 && find(flat, RESIDENT_RING_ON) < 0, "the flat load names its every-row mirror alone:\n{flat}")
    t |> success(find(flat, RESIDENT_EMBED_OFF) >= 0, "the host embed under the gather pin says so:\n{flat}")
    t |> success(find(ringed, RESIDENT_EMBED_OFF) < 0, "the gathering load says nothing of the host embed")
}

//! one arm's two paths over the same prose ids: the worst row distance between the window chain and the token command, as
//! a fraction of the row's largest live logit - aligned, one position behind, and against the token command's first row
struct PathGap {
    gap : float
    off : float
    first : float
}

//! `n` prose ids through the window chain against the same ids through the token command, on sessions of codec `kv`; the
//! arm named `name` in the log
def path_gap(tr : Model; kv : KVDtype; n : int64; name : string) : PathGap {
    var toks <- prose_tokens(tr, n + 4l)
    var window_prompt <- [for (i in range64(n)); toks[i]]
    var window_steps <- [for (i in range64(n, n + 4l)); toks[i]]
    var token_prompt <- [toks[0]]
    var token_steps <- [for (i in range64(1l, n + 4l)); toks[i]]
    var a <- feed_alone(tr, kv, window_prompt, window_steps)
    var b <- feed_alone(tr, kv, token_prompt, token_steps)
    var worst = 0.0
    var worst_off = 0.0
    var worst_first = 0.0
    for (k in range64(long_length(a))) {
        let j = n - 1l + k
        let scale = live_maxabs(a[k])
        let rel = logits_maxdiff(a[k], b[j]) / scale
        if (rel > worst) {
            worst = rel
        }
        let off = logits_maxdiff(a[k], b[j - 1l]) / scale   // the token command one position behind: logged, since a one-position shift moves prose logits less than the bar on the 12B and 31B
        if (off > worst_off) {
            worst_off = off
        }
        let first = logits_maxdiff(a[k], b[0]) / scale   // the control: the token command's row at position 0 - the same window read every position off
        if (first > worst_first) {
            worst_first = first
        }
        let ia = argmax_of(a[k])
        let ib = argmax_of(b[j])
        to_log(LOG_INFO, "{name}: position {n + k - 1l}: prefilled-in-one vs decoded-one-by-one logits maxdiff {logits_maxdiff(a[k], b[j])} of max |logit| {scale}, argmax {ia} ({a[k][ia]}) vs {ib} ({b[j][ib]}); one position off {off}, the first position {first}\n")
    }
    delete a
    delete b
    delete toks
    delete window_prompt
    delete window_steps
    delete token_prompt
    delete token_steps
    return PathGap(gap = worst, off = worst_off, first = worst_first)
}

//! the resident arm's two paths agree within `GEMMA_PATH_GAP_RATIO` of the CPU chain's own two, with both controls
def path_gaps_within(t : T?; n : int64; res, cpu : PathGap) {
    let bar = GEMMA_PATH_GAP_RATIO * cpu.gap
    to_log(LOG_INFO, "{n} tokens: the resident paths sit {res.gap} of the max logit apart, the CPU paths {cpu.gap} - ratio {res.gap / cpu.gap} (bar {bar}); one position off: resident {res.off}, cpu {cpu.off}; the first position: resident {res.first}, cpu {cpu.first}\n")
    t |> success(res.gap <= bar, "the resident window chain and token command agree within {GEMMA_PATH_GAP_RATIO}x of the CPU chain's own two paths (resident {res.gap}, cpu {cpu.gap})")
    // the control on the resident route: the window's rows against the token command's first row - the same window read every position off - land outside the bar; a one-position shift cannot (the CPU chain's own paths sit 0.17 to 0.39 apart on the 12B and 31B, and 6x of that is wider than one position moves prose logits, 0.67 to 1.57), so it is logged beside the aligned gap alone
    t |> success(is_finite(res.first) && res.first < INFINITE_DIFF && res.first > bar, "control: on the resident route the window's rows against the token command's first row land outside the bar ({res.first} against {bar})")
    t |> success(res.off > res.gap && cpu.off > cpu.gap, "one position behind, both arms read a wider gap than aligned (resident {res.off} vs {res.gap}, cpu {cpu.off} vs {cpu.gap})")
}

//! the E-series short window's peel (`../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-peel`), the peel lever on and put back: after
//! a 40-row opening each tail of `tails` rows peels as one prefill call (one peel counted), and its last row and four fed steps sit
//! within `rel` of the same tail stepped a row at a time on a second session, the row one step off the control
def e_series_peel_cell(t : T?; tr : Model; tails : array<int64>; rel : float) {
    var inscope peels <- e_series_peel_rows(tr, tails)
    e_series_peels_within(t, peels, rel)
    served_every_call(t)
}

//! one tail of the peel cell: its row count, the peels its prefill counted, the peeled session's rows and the row-at-a-time ones
struct PeelTail {
    rows : int64
    peels : int64
    got : array<array<float>>
    want : array<array<float>>
}

//! `e_series_peel_cell`'s collection on the armed driver: each tail's peel count and both sessions' rows, the argmax rows logged
def e_series_peel_rows(tr : Model; tails : array<int64>) : array<PeelTail> {
    var out : array<PeelTail>
    out |> reserve(length(tails))
    var opening <- resident_prompt(40l)
    var feed <- resident_feed(RDEC_NROW_MAX)
    let lever = vulkan_prefill_peel()
    set_vulkan_prefill_peel(true)
    for (rows in tails) {
        var tail <- [for (i in range64(rows)); feed[i]]
        var steps <- ramp_ids(4l, rows, 2500l, 71l, 4500l)
        var sa <- create_session(tr, mirror_kv_dtype())
        eval(tr, sa, opening)
        let peels0 = moe_gpu_resident_peels()
        forward_prefill(tr, sa, tail, rows, 40l)
        let peels = moe_gpu_resident_peels() - peels0
        sa.n_past = 40l + rows
        var got <- steps_after(tr, sa, steps)
        var sb <- create_session(tr, mirror_kv_dtype())
        eval(tr, sb, opening)
        var inscope one : array<int64>
        one |> resize(1)
        for (i in range64(rows)) {
            one[0] = tail[i]
            eval_(tr, sb, one)
        }
        var want <- steps_after(tr, sb, steps)
        log_argmax_rows(tr, "a tail of {rows} rows: the row-at-a-time rows, then the tail's", got, want)
        out |> emplace(PeelTail(rows = rows, peels = peels, got <- got, want <- want))
        delete sa
        delete sb
        delete tail
        delete steps
    }
    set_vulkan_prefill_peel(lever)
    delete opening
    delete feed
    return <- out
}

//! each collected tail peeled once, its rows within `rel` of the row-at-a-time rows, the row one step off the control
def e_series_peels_within(t : T?; peels : array<PeelTail>; rel : float) {
    for (p in peels) {
        t |> equal(p.peels, 1l, "a tail of {p.rows} rows peels through the token commands")
        rows_within_control(t, "a tail of {p.rows} rows", p.got, p.want, rel, 1l, 1.0, $(r) => r == 0l ? "the tail's last row" : "step {r}")
    }
}

//! teacher-forced negative log-likelihood over `steps` positions after `n` prefilled ids: the sum over the
//! true next ids, the sum over the ids one position off (the control), and the argmax hits on each
//! a teacher-forced walk's sums: the targets' negative log-likelihood, the same against the ids one position off, and the
//! argmax hits of each
typedef ForcedNll = tuple<nll : float; off : float; hits : int64; off_hits : int64>

def forced_nll(tr : Model; var s : Session; ids : array<int64>; n, steps : int64) : ForcedNll {
    var prompt <- [for (i in range64(n)); ids[i]]
    forward_prefill(tr, s, prompt, n, 0l)
    delete prompt
    var nll = 0.0
    var off = 0.0
    var hits = 0l
    var off_hits = 0l
    for (p in range64(n, n + steps)) {
        nll -= target_logp(s.logits, ids[p])
        off -= target_logp(s.logits, ids[p + 1l])
        let am = argmax_of(s.logits)
        if (am == ids[p]) {
            hits++
        }
        if (am == ids[p + 1l]) {
            off_hits++
        }
        if (p + 1l < n + steps) {
            forward(tr, s, ids[p], p)
        }
    }
    return (nll = nll, off = off, hits = hits, off_hits = off_hits)
}

//! `n` prose ids prefilled and `steps` teacher-forced after them on a session of codec `kv`
def prose_forced_nll(tr : Model; kv : KVDtype; n, steps : int64) : ForcedNll {
    var ids <- prose_tokens(tr, n + steps + 1l)
    var s <- create_session(tr, kv)
    let r = forced_nll(tr, s, ids, n, steps)
    delete s
    delete ids
    return r
}

//! the resident walk `g` against the CPU chain's `c` as perplexities over `steps` positions past an `n`-id prefill: the
//! resident within `ppl_ratio` of the CPU chain's, its argmax hits within `hits_slack`, and the one-position-off controls
def ppl_within(t : T?; n, steps : int64; g, c : ForcedNll; hits_slack : int64 = 3l; ppl_ratio : float = GEMMA_PPL_RATIO) {
    let fsteps = float(steps)
    let ppl_g = exp(g.nll / fsteps)
    let ppl_c = exp(c.nll / fsteps)
    let ppl_off = exp(c.off / fsteps)
    let ppl_g_off = exp(g.off / fsteps)   // the device route's own control: its logits scored against the ids one position off
    to_log(LOG_INFO, "{n} prefilled + {steps} teacher-forced [{tile_arm_tag()}]: PPL resident {ppl_g} (hits {g.hits}/{steps}, one-off {ppl_g_off}), cpu {ppl_c} (hits {c.hits}/{steps}, one-off hits {c.off_hits}), one-off control {ppl_off}\n")
    t |> success(ppl_g <= ppl_c * ppl_ratio, "the resident perplexity {ppl_g} sits within {ppl_ratio}x of the CPU chain's {ppl_c}")
    t |> success(g.hits + hits_slack >= c.hits, "the resident argmax hits ({g.hits}) sit within {hits_slack} of the CPU chain's ({c.hits}) over {steps} positions")
    t |> success(is_finite(ppl_g_off) && ppl_g_off > ppl_c * ppl_ratio, "control on the device route: the resident logits scored against the ids one position off read {ppl_g_off}, outside {ppl_ratio}x of the CPU chain's {ppl_c}")
    t |> success(ppl_off >= 3.0 * ppl_c, "control: the one-position-off targets read {ppl_off}, at least three times the CPU chain's {ppl_c}")
    t |> success(c.off_hits + hits_slack < c.hits, "control: the one-position-off targets hit {c.off_hits}, under the hits bar's reach ({c.hits} - {hits_slack})")
}

//! a teacher-forced walk's shape: `n` ids prefilled, `steps` scored after them
typedef GemmaWalk = tuple<n : int64; steps : int64>

//! what one pass collects for a gemma file's cells on the driver's default form: a forced feed a prompt length, the two
//! paths' gap a length with the positions the CUDA home served for it, a teacher-forced walk a `GemmaWalk` with the
//! resident window chains its prompt took and the positions the CUDA home served for it
struct GemmaRows {
    feeds : array<FeedRows>
    gaps : array<PathGap>
    gap_cuda : array<int64>
    nlls : array<ForcedNll>
    nll_prefills : array<int64>
    nll_cuda : array<int64>
}

def gemma_rows(m : Model; form : ModelForm; feeds, gaps : array<int64>; walks : array<GemmaWalk>) : GemmaRows {
    var r : GemmaRows
    r.feeds |> reserve(length(feeds))
    r.gaps |> reserve(length(gaps))
    r.gap_cuda |> reserve(length(gaps))
    r.nlls |> reserve(length(walks))
    r.nll_prefills |> reserve(length(walks))
    r.nll_cuda |> reserve(length(walks))
    for (n in feeds) {
        r.feeds |> emplace <| prose_feed_rows(m, form, n)
    }
    for (n in gaps) {
        let c0 = cuda_positions_served()
        r.gaps |> push(path_gap(m, form.kv, n, form.on_gpu ? "resident" : "cpu"))
        r.gap_cuda |> push(c0 >= 0l ? cuda_positions_served() - c0 : -1l)
    }
    for (w in walks) {
        let p0 = gpu_resident_prefills_()
        let c0 = cuda_positions_served()
        r.nlls |> push(prose_forced_nll(m, form.kv, w.n, w.steps))
        r.nll_prefills |> push(gpu_resident_prefills_() - p0)
        r.nll_cuda |> push(c0 >= 0l ? cuda_positions_served() - c0 : -1l)
    }
    return <- r
}

//! the device pass's collections against the CPU pass's, cell by cell: every feed's rows within the gemma logit bar, every
//! length's two paths within the path-gap ratio, every walk's perplexity within `ppl_ratio` and its argmax hits within
//! `hits_slack`; `tag` opens each message
def gemma_rows_within(t : T?; tag : string; gpu, cpu : GemmaRows; gaps : array<int64>; walks : array<GemmaWalk>; ppl_ratio : float = GEMMA_PPL_RATIO;
                      hits_slack : int64 = 3l) {
    with_dasllama_jobque_() {   //! the compares read each row's argmax, which asks the queue
        for (g, c in gpu.feeds, cpu.feeds) {
            feed_rows_within(t, "{tag}, {g.nprompt} tokens: ", g, c, GEMMA_LOGIT_BAR_REL)
        }
        for (n, g, c, cuda in gaps, gpu.gaps, cpu.gaps, gpu.gap_cuda) {
            if (cuda >= 0l) {
                t |> equal(cuda, 2l * (n + 4l), "{tag}: the CUDA driver served the window chain and the token command over {n + 4l} tokens each")
            }
            path_gaps_within(t, n, g, c)
        }
        for (w, g, c, pf, cuda in walks, gpu.nlls, cpu.nlls, gpu.nll_prefills, gpu.nll_cuda) {
            t |> equal(pf, 1l, "{tag}: the resident window chain served the {w.n}-token prompt of the walk")
            if (cuda >= 0l) {
                t |> success(cuda >= w.n + w.steps - 1l, "{tag}: the CUDA driver served {cuda} positions of the walk's {w.n}-id prefill and {w.steps - 1l} teacher-forced steps")
            }
            ppl_within(t, w.n, w.steps, g, c, hits_slack, ppl_ratio)
        }
    }
}

//! a whole gemma file's teacher-forced walks: the default form beside its CPU form (`main`), the arms the device form's carrier
//! reads, each walk's readings a side
struct GemmaWalkRows {
    main : MainPair
    carrier : Held
    gpu : array<GemmaRows>
    cpu : array<GemmaRows>
}

//! one form's walks of `walks` (`GemmaWalk`s, a fixed or a dynamic array) into `r`: the default form's and the CPU form's, each walk its own readings, the carrier's arms
//! held on the device
def gemma_walk_rows(var r : GemmaWalkRows; m : Model; form : ModelForm; walks) {
    if (form.on_gpu) {
        r.carrier |> held_fact(gemma_carrier_arms(m), GEMMA_CARRIER_ARMS)
    }
    let side = main_side(r.main, form)
    return if (side == MainSide.neither)
    for (w in walks) {
        var inscope rows <- gemma_rows(m, form, array<int64>(), array<int64>(), [w])
        if (side == MainSide.device) {
            r.gpu |> emplace(rows)
        } else {
            r.cpu |> emplace(rows)
        }
    }
}

//! walk `i` of `walks` under `tag` (`gemma_rows_within`) against its CPU twin at `ratio`, the hits within `slack`; a walk with
//! no twin a failure (`twin_collected`)
def gemma_walk_twin_within(t : T?; tag : string; r : GemmaWalkRows; walks; i : int; ratio : float;
                           slack : int64 = 3l) {
    return if (!twin_collected(t, i, length(r.cpu), "{tag}: "))
    gemma_rows_within(t, tag, r.gpu[i], r.cpu[i], array<int64>(), [walks[i]], ratio, slack)
}

//! the same prose through the resident driver and the CPU chain, scored as a perplexity over `steps`
//! teacher-forced positions after an `n`-id prefill: the resident within the ratio of the CPU chain's,
//! its argmax hits within `hits_slack`, and the one-off control at least three times the CPU chain's perplexity.
//! On the 12B the logits sit within a few points at whole stretches of positions (the reference exe's own
//! margins read 0.6 to 16 on this text), so a maxdiff bar on a single position measures the model, not the driver;
//! a routed model's near-tie flips move whole positions, so its slack is wider.
[cold_path]
def ppl_compare(t : T?; n, steps : int64; gf : GemmaFile; hits_slack : int64 = 3l; ppl_ratio : float = GEMMA_PPL_RATIO) {
    with_gemma_rig(t, gf) $(tr) {
        var ids <- prose_tokens(tr, n + steps + 1l)
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = gpu_resident_prefills_()
        let c0 = cuda_positions_served()
        let g = forced_nll(tr, sg, ids, n, steps)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(gpu_resident_prefills_(), served0 + 1l, "the resident window chain served the {n}-token prompt")
        cuda_served_since(t, c0, n + steps - 1l, "the {n}-id prefill and {steps - 1l} teacher-forced steps")
        served_every_call(t)
        delete sg
        moe_gpu_drop_model()   // every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        let c = forced_nll(tr, sc, ids, n, steps)
        delete sc
        ppl_within(t, n, steps, g, c, hits_slack, ppl_ratio)
        delete ids
    }
}

let GEMMA_RING_IDS = 1300l   //! the ring cells' prose ids: past the 512-position window, three windows over

//! a gemma file's block-codec and ring cells: the q8_0 mirror's forced feed of `q8_feed` ids, the tq4 mirror's
//! teacher-forced walk, and the mirror the second ring pair runs on, each cell's note for its message
struct CodecCells {
    q8_feed : int64
    tq4_walk : GemmaWalk
    ring_kv : KVDtype
    q8_note : string
    tq4_note : string
}

//! what those cells collect over a file's forms: the q8_0 feed and
//! the tq4 walk, each a CPU side ([0]) and a device side ([1]), each
//! `_ran` where its device form collected; and the rings - the
//! default codec's (`main`) and the `ring_kv` mirror's (`other`),
//! each ringed and flat
struct CodecRows {
    q8 : GemmaRows[2]
    q8_ran : bool
    tq4 : GemmaRows[2]
    tq4_ran : bool
    rings : RingRows
}

//! one form's share of the codec and ring cells: a CPU form its codec's reference side, the default device form the ringed
//! run, a flat form its flat run, and a ringed form with an asked mirror that mirror's cell and, on `ring_kv`, its ringed run
def codec_rows(var c : CodecRows; m : Model; form : ModelForm; cells : CodecCells) {
    if (!form.on_gpu) {
        if (form.kv == KVDtype.q8_0) {
            c.q8[0] <- gemma_rows(m, form, [cells.q8_feed], array<int64>(), array<GemmaWalk>())
        }
        if (form.kv == KVDtype.tq4) {
            c.tq4[0] <- gemma_rows(m, form, array<int64>(), array<int64>(), [cells.tq4_walk])
        }
        return
    }
    if (form.ring) {
        if (form.kv_asked && form.kv == KVDtype.q8_0) {
            c.q8[1] <- gemma_rows(m, form, [cells.q8_feed], array<int64>(), array<GemmaWalk>())
            c.q8_ran = true
        } elif (form.kv_asked) {
            c.tq4[1] <- gemma_rows(m, form, array<int64>(), array<int64>(), [cells.tq4_walk])
            c.tq4_ran = true
        }
    }
    if (!form.ring || !form.kv_asked || form.kv == cells.ring_kv) {
        ring_rows_into(c.rings, m, form.kv, form.kv_asked, form.ring, GEMMA_RING_IDS)
    }
}

//! the codec cells' compares under `tag`, each where its device form ran
def codec_rows_within(t : T?; tag : string; c : CodecRows; cells : CodecCells) {
    if (c.q8_ran) {
        gemma_rows_within(t, "{tag}, a q8_0 K/V mirror - {cells.q8_note}", c.q8[1], c.q8[0], array<int64>(), array<GemmaWalk>())
    }
    if (c.tq4_ran) {
        gemma_rows_within(t, "{tag}, a tq4 K/V mirror - {cells.tq4_note}", c.tq4[1], c.tq4[0], array<int64>(), [cells.tq4_walk])
    }
}

