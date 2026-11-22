// The cells of the resident driver's mirror regions and device-home sessions, shared by the per-model test files
// (test_gpu_resident_regions*.das): ONE model a file, so a process loads one carrier.

options gen2
options _dasllama_internal = true

module _resident_regions shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama   // create_session, create_batch_workspace, eval_batch
require _scheduler_rig   // the device-mode scheduler cell: make_req, drain, toks_of, finished_of
require math
require daslib/defer
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed; gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // set_gpu_resident_regions_, moe_gpu_resident_active
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 — the grid helpers, read under the vulkan static_if, unused in the darwin world
require ?vulkan dasllama/dasllama_vulkan_common   // the prompt window's rows, where the module exists
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_n_rows: the N-row command served the batched cells' rows
require ?vulkan dasllama/dasllama_vulkan_classes   // the discrete layer's GEMV_COL_TILE through it: the groups cell's column tiles
require ?vulkan dasllama/dasllama_vulkan_prefill   // vk_rdec_prefill_segment_windows: the device-mode tick's segmented-window witness
require ?cuda dasllama/dasllama_cuda_decode   // the same witnesses on the CUDA home
require _model_tier public   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // served_every_call: the shared served witness
require _cpu_gpu_model   // ModelForm, device_form, asking: the rig's forms
require _compares   // the head-rows cell's draft compare
require dasllama/dasllama_ple   // ple_host_gather_rows: the side rows' host witness

// The resident driver's mirror regions (a small carrier under DASLLAMA_GPU=1): two
// sessions hold their K/V on the device at once, one region each. The instrument is the driver
// against itself - a session stepped between another session's steps reads what it reads alone
// within the batched-row bar, because a region is an offset and the kernels are the same - so no
// CPU chain runs.
// The two prompts differ in content and length: a step that read the other region's rows cannot
// land on the alone-run's logits.

//! the rig's context: the mirror regions' cap and every host-cached session's K/V cache - a model max of 131072 is gigabytes of each
let REGIONS_RIG_CTX = 8192l
let private CAPPED_RIG_CTX = 4096l   //! the cap cell's context: the smallest the driver arms a capped mirror at, so a prompt reaches its end in seconds

[init]
def private regions_rig_init() {
    allow_cpu_prefill()   // the pin cell's outsider prefills on the CPU by design
}

let private STEPS = 8l

def private prompt_a() : array<int64> => <- resident_prompt(40l)
def private prompt_b() : array<int64> => <- ramp_ids(57l, 0l, 3000l, 91l, 6000l)
def private feed_a() : array<int64> => <- resident_feed(STEPS)
def private feed_b() : array<int64> => <- ramp_ids(STEPS, 0l, 2500l, 71l, 4500l)
def private prompt_c() : array<int64> => <- ramp_ids(48l, 0l, 1500l, 59l, 5500l)
def private feed_c() : array<int64> => <- ramp_ids(STEPS, 0l, 2200l, 67l, 4200l)

//! one session alone: the prefill's logits, then each fed step's
def private logits_alone(tr : Model; prompt, toks : array<int64>) : array<array<float>> => <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)

//! both rows' argmax, each with its logit and the piece `tr` decodes it to
def private log_peaks(tr : Model; what : string; got, want : array<float>) {
    let pg = argmax_of(got)
    let pw = argmax_of(want)
    to_log(LOG_INFO, "{what}: argmax {pg} '{decode_(tr, [pg])}' ({got[pg]}) against {pw} '{decode_(tr, [pw])}' ({want[pw]})\n")
}

//! a batched step's row against the session alone: the batched rows ride the command's column tiles where the one-row
//! step takes the GEMVs, and the two part by the tiles' rounding (0.88 of a 7.5 peak on the E2B's rows on the RTX PRO
//! 4500, 0.2-0.4 on the others; the same kernels on the RTX 5060 Ti read bit for bit), never by a token
let private BATCH_ROW_BAR_REL = 0.15

//! a row against the session alone: within the batched-row bar of `want`'s peak, with `control` - the session's logits one
//! token off - outside it; under a tq4 mirror the control form, since the 4-bit codec's rounding at a near-tie reads half
//! the peak. Both argmax logged, not asserted the same, since a near-tie inside the bar flips the pick
def private hold_equal(var h : Held; tr : Model; got, want, control : array<float>; what : string) {
    hold_within(h, tr, got, want, control, what, BATCH_ROW_BAR_REL)
}

def private hold_equal(t : T?; tr : Model; got, want, control : array<float>; what : string) {
    hold_within(t, tr, got, want, control, what, BATCH_ROW_BAR_REL)
}

//! `got` within `rel` of `want`'s peak, and `control` - logits one token off - outside it; under a tq4 mirror, `tq4_within`
def private hold_within(t : T?; tr : Model; got, want, control : array<float>; what : string; rel : float = SPLIT_BAR_REL) {
    held_on(t) $(var h) {
        hold_within(h, tr, got, want, control, what, rel)
    }
}

//! `hold_within` held for the asserts after a model's block: the peaks logged now, the rows held under the mirror's form
def private hold_within(var h : Held; tr : Model; got, want, control : array<float>; what : string; rel : float = SPLIT_BAR_REL) {
    log_peaks(tr, what, got, want)
    if (mirror_kv_dtype() == KVDtype.tq4) {
        h |> held_near(what, got, want, control)
    } else {
        h |> held_row(what, got, want, control, rel)
    }
}

//! the fed tokens of two sessions and each one's logits alone, prefilled over `pa` and `pb`: what a two-region cell holds its rows to
struct private AloneRuns {
    fa : array<int64>
    fb : array<int64>
    ra : array<array<float>>
    rb : array<array<float>>
}

def private alone_runs(tr : Model; pa, pb : array<int64>) : AloneRuns {
    var fa <- feed_a()
    var fb <- feed_b()
    var ra <- logits_alone(tr, pa, fa)
    var rb <- logits_alone(tr, pb, fb)
    return <- AloneRuns(fa <- fa, fb <- fb, ra <- ra, rb <- rb)
}

//! two sessions over `pa` and `pb`, a region each, prefilled then stepped one row at a time, interleaved, against each alone
def private interleaved_single_steps_body(var h : Held; tr : Model; pa, pb : array<int64>) {
    var al <- alone_runs(tr, pa, pb)
    let apart = logits_maxdiff(al.ra[0], al.rb[0])
    h |> held_fact(apart > 0.0 && apart < INFINITE_DIFF, "the two prompts read different finite logits ({apart} apart), so a crossed region cannot pass")
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sa, pa, long_length(pa), 0l)
    hold_equal(h, tr, sa.logits, al.ra[0], al.ra[1], "session A's prefill")
    forward_prefill(tr, sb, pb, long_length(pb), 0l)
    hold_equal(h, tr, sb.logits, al.rb[0], al.rb[1], "session B's prefill, A's rows live beside it")
    h |> held_fact(sa.rdec_region != sb.rdec_region, "the sessions sit in different regions ({sa.rdec_region}, {sb.rdec_region})")
    for (st in range64(STEPS)) {
        forward(tr, sa, al.fa[st], long_length(pa) + st)
        hold_equal(h, tr, sa.logits, al.ra[st + 1l], al.ra[st], "session A step {st}")
        forward(tr, sb, al.fb[st], long_length(pb) + st)
        hold_equal(h, tr, sb.logits, al.rb[st + 1l], al.rb[st], "session B step {st}")
    }
    h |> held_served()
    delete sa
    delete sb
    delete al
}

//! the interleaved cell's collection on a two-region load, its asserts held
def regions_interleaved_rows(var h : Held; tr : Model) {
    var pa <- prompt_a()
    var pb <- prompt_b()
    interleaved_single_steps_body(h, tr, pa, pb)
    delete pa
    delete pb
}

//! two host-cached sessions over ONE region: A prefills and takes a one-row step (rows on the device alone), B prefills and takes
//! the region, then the two step in turn, each taking it back - every row what the session reads alone within the batched-row bar, the driver's
//! saved-claim counter up (A's rows read off the device before B's prefill overwrote them) and every call served
//! the cell's run on a one-region load the
//! caller made
def regions_taken_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("two host-cached sessions over one region: the session that loses it keeps its device-only rows") @capture(& tr)(t : T ?) {
            var pa <- prompt_a()
            var pb <- prompt_b()
            var al <- alone_runs(tr, pa, pb)
            let na = long_length(pa)
            let nb = long_length(pb)
            let stashes0 = moe_gpu_resident_stashes()
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sa, pa, na, 0l)
            hold_equal(t, tr, sa.logits, al.ra[0], al.ra[1], "session A's prefill")
            forward(tr, sa, al.fa[0], na)
            hold_equal(t, tr, sa.logits, al.ra[1], al.ra[0], "session A step 0, its rows on the device alone")
            forward_prefill(tr, sb, pb, nb, 0l)
            hold_equal(t, tr, sb.logits, al.rb[0], al.rb[1], "session B's prefill, taking A's region")
            t |> success(moe_gpu_resident_stashes() > stashes0, "A's rows were saved as B took the region ({moe_gpu_resident_stashes() - stashes0} saves)")
            for (st in range64(STEPS - 1l)) {
                forward(tr, sa, al.fa[st + 1l], na + st + 1l)
                hold_equal(t, tr, sa.logits, al.ra[st + 2l], al.ra[st + 1l], "session A step {st + 1l}, the region taken back")
                forward(tr, sb, al.fb[st], nb + st)
                hold_equal(t, tr, sb.logits, al.rb[st + 1l], al.rb[st], "session B step {st}, the region taken back")
            }
            served_every_call(t)
            delete sa
            delete sb
            delete al
            delete pa
            delete pb
        }
    }
}

let private WIDE_ROWS = 3072l   //! the token command's RD_WIDE_POS, asserted equal by the wide cell: its prompts run past it

//! two sessions past the wide position on a two-region load: both prompts run longer than RD_WIDE_POS, so the one-row steps
//! take the token command's wide twin - recorded on the first such step, its stamp names apart from the split form's
def regions_wide_rows(var h : Held; tr : Model) {
    var pa <- resident_prompt(WIDE_ROWS + 8l)
    var pb <- ramp_ids(WIDE_ROWS + 25l, 0l, 3000l, 91l, 6000l)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        h |> held_fact(long_length(pa) > RD_WIDE_POS && long_length(pb) > RD_WIDE_POS, "both prompts run past the wide position ({RD_WIDE_POS})")
    }
    interleaved_single_steps_body(h, tr, pa, pb)
    delete pa
    delete pb
}

def private prose_ids(n, salt : int64) : array<int64> => <- resident_prompt(n, salt)

//! the logits of `prompt`'s first `n` tokens prefilled in one call on a session of its own at the codec `kv`, gone after
def prefill_row(tr : Model; prompt : array<int64>; n : int64; kv : KVDtype) : array<float> {
    var s <- create_session(tr, kv)
    forward_prefill(tr, s, prompt, n, 0l)
    var out <- logits_of(s, tr.config.vocab_size)
    delete s
    return <- out
}

//! the logits of `prompt`'s first `n - 1` tokens on a session of its own at the codec `kv`, gone before the caller makes the
//! compared pair (two regions hold two live sessions): the one-token-short control a prefill compare holds its bar against
def short_control(tr : Model; prompt : array<int64>; n : int64; kv : KVDtype = mirror_kv_dtype()) : array<float> {
    var less <- [for (i in range64(n - 1l)); prompt[i]]
    var out <- prefill_row(tr, less, n - 1l, kv)
    delete less
    return <- out
}

//! `hold_within` for a row two window splits of one prompt produced; under a tq4 mirror the splits on aligned windows read
//! bit for bit and the others are only reported (`tq4_reported` says why)
def private hold_continued(t : T?; tr : Model; got, want, control : array<float>; what : string; rel : float; windows_aligned : bool) {
    if (mirror_kv_dtype() != KVDtype.tq4) {
        hold_within(t, tr, got, want, control, what, rel)
        return
    }
    log_peaks(tr, what, got, want)
    if (windows_aligned) {
        t |> equal(logits_maxdiff(got, want), 0.0, "{what}: bit for bit, the two forms split at the same rows")
        t |> success(logits_maxdiff(got, control) > 0.0, "{what} control: the row one off reads apart")
    } else {
        tq4_reported(t, got, want, control, what)
    }
}

//! the second half of `prompt` from a checkpoint a fresh session adopts: its first `half` rows prefilled, a checkpoint kept,
//! one step past it, the claim parked and adopted behind the step, the id window seeded from the tokens, the continuation
//! served; the adopted row back
def adopted_row(t : T?; tr : Model; prompt : array<int64>; half : int64) : array<float> {
    var first <- [for (i in range64(half)); prompt[i]]
    var rest <- [for (i in range64(half, long_length(prompt))); prompt[i]]
    var sa <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sa, first, half, 0l)
    sa.n_past = half
    t |> success(gpu_device_kv_checkpoint(sa), "the region kept a checkpoint at the first half's end")
    forward(tr, sa, prompt[half], half)
    let claim = gpu_device_kv_park(sa)
    t |> success(claim != 0l, "the stepped session parked its claim")
    var sb <- create_session(tr, mirror_kv_dtype())
    t |> success(gpu_device_kv_adopt(sb, claim, half), "a fresh session adopted the claim at the checkpoint, behind the step")
    if (!empty(sb.ple_prev)) {
        ple_window_seed(sb.ple_prev, prompt, half)
    }
    sb.n_past = half
    forward_prefill(tr, sb, rest, long_length(rest), half)
    served_every_call(t)
    var adopted <- [for (x in sb.logits); x]
    delete sb
    delete sa
    delete first
    delete rest
    return <- adopted
}

//! `prompt` prefilled in one call against the same prompt prefilled as [0, cut) then [cut, n), then four fed steps on both
def continue_at(t : T?; tr : Model; n, cut : int64; rel : float) {
    var prompt <- prose_ids(n, 0l)
    var head <- [for (i in range64(cut)); prompt[i]]
    var tail <- [for (i in range64(n - cut)); prompt[cut + i]]
    var feed <- feed_a()
    var short_logits <- short_control(tr, prompt, n)
    var whole <- create_session(tr, mirror_kv_dtype())
    var split <- create_session(tr, mirror_kv_dtype())
    let served0 = gpu_resident_prefills_()
    forward_prefill(tr, whole, prompt, n, 0l)
    forward_prefill(tr, split, head, cut, 0l)
    forward_prefill(tr, split, tail, n - cut, cut)
    t |> equal(gpu_resident_prefills_(), served0 + 3l, "{n} tokens cut at {cut}: the window chain served all three calls, the continuation among them")
    if (!empty(split.dn_state_prefix)) {
        t |> equal(split.dn_pos, n, "{n} tokens cut at {cut}: the recurrent state stands at the prompt's end after the continuation")
    }
    let windows_aligned = cut % PF_WINDOW_ROWS == 0l
    hold_continued(t, tr, split.logits, whole.logits, short_logits, "{n} tokens cut at {cut}: the continued prefill against the one-call prefill", rel, windows_aligned)
    delete short_logits
    for (st in range64(4l)) {
        var before <- logits_of(whole, tr.config.vocab_size)
        forward(tr, whole, feed[st], n + st)
        forward(tr, split, feed[st], n + st)
        hold_continued(t, tr, split.logits, whole.logits, before, "{n} tokens cut at {cut}, step {st}: decoding over the continued rows", rel, windows_aligned)
        delete before
    }
    delete whole
    delete split
    delete prompt
    delete head
    delete tail
    delete feed
}

//! the continued-prefill cells' two runs on a load the caller made; `rel` the bar the continued rows hold against the one-call
//! rows - the split bar, the routed chain's on a routed model (its windows' expert tiles schedule other row counts, and the
//! router's near-ties part the two)
def regions_continues_cell(t : T?; tr : Model; rel : float) {
    unsafe {
        t |> run("a prefill from a position past zero runs the window chain over the region's rows") @capture(& tr)(t : T ?) {
            return if (!assert_pf_window(t))
            continue_at(t, tr, 57l, 40l, rel)     // a short tail inside the first window
            continue_at(t, tr, PF_WINDOW_ROWS + 88l, PF_WINDOW_ROWS, rel)   // the cut on the window boundary
            continue_at(t, tr, PF_WINDOW_ROWS + 388l, 300l, rel)   // the tail runs a whole window from an unaligned start, then a remainder
            continue_at(t, tr, 42l, 40l, rel)   // a tail shorter than a recurrent layer's conv taps: the ring's leading rows come from the history it continues
            served_every_call(t)
        }
    }
    unsafe {
        t |> run("the chat shape: a prompt, decoded steps, a second turn's prompt over that history, decoded steps") @capture(& tr)(t : T ?) {
            var turn1 <- prose_ids(40l, 0l)
            var said <- feed_a()
            var turn2 <- prose_ids(20l, 7l)
            var chat <- create_session(tr, mirror_kv_dtype())
            var steps <- create_session(tr, mirror_kv_dtype())
            eval(tr, chat, turn1)
            eval(tr, steps, turn1)
            for (st in range64(STEPS)) {
                forward(tr, chat, said[st], 40l + st)
                forward(tr, steps, said[st], 40l + st)
            }
            chat.n_past = 40l + STEPS
            eval(tr, chat, turn2)
            for (i in range64(19l)) {
                forward(tr, steps, turn2[i], 40l + STEPS + i)
            }
            var before <- logits_of(steps, tr.config.vocab_size)
            forward(tr, steps, turn2[19], 40l + STEPS + 19l)
            hold_continued(t, tr, chat.logits, steps.logits, before, "the second turn through the window chain against the same tokens stepped one by one", rel, false)
            delete before
            served_every_call(t)
            delete chat
            delete steps
            delete turn1
            delete said
            delete turn2
        }
    }
}

//! the window's fused q/k norm, rope and K/V store against its split pair (`set_vulkan_prefill_qkn_fuse`, put back as found) on a
//! two-region load of the Vulkan home: one prompt two windows long on each form, the prompt's logits and the fed steps within
//! `bar` with the row one step off as the control, the census counting the fused pass's stamp on the fused form and the norm
//! pass's on the split one
def regions_window_qkn_rows(var h : Held; tr : Model; bar : LogitsBar = LogitsBar.split_bar) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        h |> held_fact(tr.config.qk_norm, "the carrier norms its q and k heads")
        var prompt <- prose_ids(PF_WINDOW_ROWS + 88l, 0l)
        var feed <- feed_a()
        let lever = vulkan_prefill_qkn_fuse()
        var rows : array<array<array<float>>>
        rows |> reserve(2)
        var fused_counts, split_counts : int64[2]
        for (fused in fixed_array(true, false)) {
            set_vulkan_prefill_qkn_fuse(fused)
            let qkr0 = census("qkn_rope_cls_spv") + census("qkn_rope_f16_cls_spv")
            let rms0 = census("qk_rms_cls_spv")
            var s <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, s, prompt, long_length(prompt), 0l)
            let counts = fixed_array(census("qkn_rope_cls_spv") + census("qkn_rope_f16_cls_spv") - qkr0, census("qk_rms_cls_spv") - rms0)
            if (fused) {
                fused_counts = counts
            } else {
                split_counts = counts
            }
            s.n_past = long_length(prompt)
            var r <- steps_after(tr, s, feed)   //! the prompt's logits, then each fed step's
            rows |> emplace(r)
            delete s
        }
        set_vulkan_prefill_qkn_fuse(lever)
        to_log(LOG_INFO, "window qkn: fused form {fused_counts[0]} fused passes and {fused_counts[1]} norm passes, split form {split_counts[0]} and {split_counts[1]}\n")
        h |> held_fact(fused_counts[0] > 0l && fused_counts[1] == 0l, "the fused form's prefill ran the fused pass and no norm pass ({fused_counts[0]}, {fused_counts[1]})")
        h |> held_fact(split_counts[0] == 0l && split_counts[1] > 0l, "the split form's prefill ran the norm pass and no fused pass ({split_counts[0]}, {split_counts[1]})")
        for (st in range(length(rows[0]))) {   //! the two passes reduce the norm in another order: the split bar, the row one step off its control
            hold_bar(h, tr, rows[0][st], rows[1][st], rows[1][st > 0 ? st - 1 : 1], bar, st == 0 ? "the prompt's logits, fused against split" : "step {st - 1}, fused against split")
        }
        h |> held_served()
        delete rows
        delete prompt
        delete feed
    } else {
        h |> held_skip("dasVulkan not present")
    }
}

//! the bar a batched row holds against its session alone: the batched-row bar (`bit_exact`: the same planes and feeds, the tiles'
//! rounding alone), or a bar with the one-token-off control - the split
//! bar on a K-quant dense carrier (its N-row Q8_K sites round apart from the one-row command's, `../followup_vulkan.md`
//! item 75), the wide bar on a K-quant MoE carrier (the readings each bar was set from: `../PERF_LEDGER.md`, the batched-decode
//! arc's gemma dense and MoE sections)
enum LogitsBar {
    bit_exact
    //! bit for bit where the N-row command splits the keys as the row
    //! alone does, else the wide bar: a routed carrier's cut whose
    //! router sits on a near-tie under the split's summation order
    bit_or_wide
    split_bar
    wide_bar
    routed_bar
    ctrl_bar
}

//! the MoE carriers' bar: a tenth of the peak
let private WIDE_BAR_REL = 0.10
//! the biased router's bar: the gpt-oss rows read 0.47 of a 7 peak against the session alone on the RTX PRO 4500 (a router
//! near-tie flips an expert on a row); a plain MoE's row one off sits inside a quarter (1.65 of a 7 peak on the Qwen3.5-35B),
//! so the carriers whose rows read a tenth keep the wide bar and its control
let private ROUTED_BAR_REL = 0.25
//! `ctrl_bar` holds no share of the peak but the control form (`tq4_within`): the gemma-4 26B's
//! double router amplifies a float-class difference at a near-tie into whole logits (15.6 of a 23 peak on the RTX PRO 4500 against 20.7 for the row one off, the support matrix's own finding on the file), so its batched rows
//! are held closer to the session alone than to the row one off, never to a share of the peak

def private bar_rel(bar : LogitsBar) : float {
    if (bar == LogitsBar.routed_bar) return ROUTED_BAR_REL
    return bar == LogitsBar.wide_bar ? WIDE_BAR_REL : SPLIT_BAR_REL
}

//! a row's logits against its session alone under `bar`: within the batched-row, split, wide or routed bar with `control` -
//! the row's previous step - outside it, or the control form alone
def private hold_bar(var h : Held; tr : Model; got, want, control : array<float>; bar : LogitsBar; what : string) {
    if (bar == LogitsBar.ctrl_bar) {
        log_peaks(tr, what, got, want)
        h |> held_near(what, got, want, control)
    } else {
        hold_within(h, tr, got, want, control, what, bar == LogitsBar.bit_exact ? BATCH_ROW_BAR_REL : bar_rel(bar))
    }
}

//! every recurrent layer's device state home, then the host copy's sum: a slot that never came home leaves the sum where it was
def private dn_state_home(var s : Session) : double {
    for (l in range64(long_length(s.dn_state_prefix))) {
        dn_flush_layer(s, l)
    }
    var sum = 0.0lf
    for (v in s.dn_state) {
        sum += double(v)
    }
    return sum
}

//! the armed discrete home's witnesses - the CUDA driver's under the CUDA ask, else the Vulkan driver's; 0 (true, false) with
//! neither module: the segmented windows counted so far, whether the N-row command splits the keys as the row alone does, the rows
//! it steps at once, the side rows gathered off the ids, and the N-row rows whose side / token row the device gathered
def private segment_windows_count : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_prefill_segment_windows() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_prefill_segment_windows()
    }
    return 0l
}

def private home_rows_split_alike(rows, pos : int64) : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_rows_split_alike(rows, pos) if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_rows_split_alike(rows, pos)
    }
    return true
}

def private home_token_n_rows : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_token_n_rows() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_token_n_rows()
    }
    return 0l
}

def private home_side_gather_on : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_side_gather_on() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_side_gather_on()
    }
    return false
}

def private home_nrow_side_gathered : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_nrow_side_gathered() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_nrow_side_gathered()
    }
    return 0l
}

def private home_nrow_embed_gathered : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_nrow_embed_gathered() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_nrow_embed_gathered()
    }
    return 0l
}

//! `bar`, or the split bar where a bit-for-bit claim cannot hold over `rows`: past the unsplit position the N-row command splits
//! a layer's keys into fewer pieces than the row alone wherever its rows' workgroups over the pieces run past one wave of the
//! card's SMs, and the two commands then sum the keys in two orders (`vk_rdec_rows_split_alike`)
def private rows_bar(bar : LogitsBar; rows, pos : int64) : LogitsBar {
    if (bar == LogitsBar.bit_or_wide) {
        return LogitsBar.bit_exact if (home_rows_split_alike(rows, pos))
        to_log(LOG_WARNING, "the N-row command over {rows} rows splits the keys apart from the row alone at {pos} positions on this card: the routed carrier's batched rows hold the wide bar, not bit for bit\n")
        return LogitsBar.wide_bar
    }
    if (bar == LogitsBar.bit_exact && !home_rows_split_alike(rows, pos)) {
        to_log(LOG_WARNING, "the N-row command over {rows} rows splits the keys apart from the row alone at {pos} positions on this card: the batched rows hold the split bar, not bit for bit\n")
        return LogitsBar.split_bar
    }
    return bar
}

def private batched_steps_body(var h : Held; tr : Model; prompts, feeds : array<array<int64>>; bar0 : LogitsBar) {
    let rows = long_length(prompts)
    var furthest = 0l
    for (p in prompts) {
        furthest = max(furthest, long_length(p))
    }
    let bar = rows_bar(bar0, rows, furthest + long_length(feeds[0]))
    h |> held_fact(home_rows_split_alike(1l, furthest + long_length(feeds[0])), "one row splits a layer's keys as one row does")
    let steps_alone0 = rdec_nrow_steps()
    var alone <- [for (i in range64(rows)); logits_alone(tr, prompts[i], feeds[i])]
    h |> held_equal(rdec_nrow_steps() - steps_alone0, 0l, "the sessions alone stepped through the one-row command: the N-row counter's control")
    var sessions : array<Session>
    sessions |> reserve(rows)
    for (i in range64(rows)) {
        sessions |> emplace <| create_session(tr, mirror_kv_dtype())
        eval(tr, sessions[i], prompts[i])   // `eval` advances `n_past`, the position a batched step reads
    }
    let hybrid = tr.config.recr_mask != 0ul
    let home0 = hybrid ? dn_state_home(sessions[rows - 1l]) : 0.0lf   //! the last session's state after its prefill: the batched steps must move what comes home
    var gens <- [for (i in range64(rows)); sessions[i].rdec_gen]
    var sess <- [for (i in range64(rows)); unsafe(addr(sessions[i]))]
    let steps0 = rdec_nrow_steps()
    with_batch_rows(tr, sess) $(var ws; var sx) {
        for (st in range64(STEPS)) {
            var toks <- [for (i in range64(rows)); feeds[i][st]]
            eval_batch(tr, ws, sx, toks)
            delete toks
            for (i in range64(rows)) {
                hold_bar(h, tr, sessions[i].logits, alone[i][st + 1l], alone[i][st], bar, "batched step {st}, row {i}")
            }
        }
    }
    if (hybrid) {
        h |> held_fact(dn_state_home(sessions[rows - 1l]) != home0, "the last session's recurrent state came home from its region's slot after the batched steps")
    }
    for (i in range64(rows)) {
        h |> held_equal(sessions[i].rdec_gen, gens[i], "row {i} kept its claim through every step: its history never came up from the host again")
    }
    h |> held_served()
    let groups = batched_groups(h, rows)
    let stepped = rdec_nrow_steps() - steps0
    h |> held_equal(stepped, STEPS * groups, "every batched step went through the N-row command, {groups} command(s) a step ({stepped} of {STEPS * groups})")
    unsafe {
        delete sess
    }
    delete sessions
    delete gens
    delete alone
}

//! the commands a batched step of `rows` rows takes: the fewest groups of the rows the N-row command steps at once, the command
//! asserted to serve the rows at once (a declined command passes the bars above the same); one where the module is absent
def private batched_groups(var h : Held; rows : int64) : int64 {
    let at_once = home_token_n_rows()
    h |> held_fact(at_once >= 2l, "the N-row token command serves the rows at once ({at_once} rows)")
    return at_once > 0l ? (rows + at_once - 1l) / at_once : 1l
}

def private batched_steps_body(t : T?; tr : Model; prompts, feeds : array<array<int64>>; bar0 : LogitsBar) {
    held_on(t) $(var h) {
        batched_steps_body(h, tr, prompts, feeds, bar0)
    }
}

def private batched_steps_body(t : T?; tr : Model; pa, pb : array<int64>; bar : LogitsBar) {
    held_on(t) $(var h) {
        batched_steps_body(h, tr, pa, pb, bar)
    }
}

//! the two-prompt form over `pa` and `pb` with the rig's feeds
def private batched_steps_body(var h : Held; tr : Model; pa, pb : array<int64>; bar : LogitsBar) {
    var prompts <- [ <- clone_to_move(pa), <- clone_to_move(pb)]
    var feeds <- [ <- feed_a(), <- feed_b()]
    batched_steps_body(h, tr, prompts, feeds, bar)
    delete prompts
    delete feeds
}

//! the side-rows cell's collection on a two-region load gathering its token rows on the device: the E-series side rows at every
//! decode shape - the sessions alone (the one-row command) and the batched steps (the N-row command) gather each row's side row
//! off its token id, the host gathers no table row, and the batched rows still read bit for bit what each session reads alone
def regions_side_rows(var h : Held; tr : Model) {
    h |> held_fact(has_ple(tr.config) && home_side_gather_on(), "the driver gathers the side rows off the ids (the table and the projection on the device)")
    var pa <- prompt_a()
    var pb <- prompt_b()
    let host0 = ple_host_gather_rows()
    let dev0 = home_nrow_side_gathered()
    batched_steps_body(h, tr, pa, pb, LogitsBar.bit_exact)
    h |> held_equal(ple_host_gather_rows() - host0, 0l, "the host gathered no table row")
    h |> held_equal(home_nrow_side_gathered() - dev0, 2l * STEPS, "every batched row's side row gathered off its id")
    delete pa
    delete pb
}

//! two sessions over prompts A and B batched through STEPS steps: every row's logits a step, the N-row command serving every step
def private batched_rows_logits(var h : Held; tr : Model) : array<array<float>> {
    var prompts <- [ <- prompt_a(), <- prompt_b()]
    var feeds <- [ <- feed_a(), <- feed_b()]
    var rows <- batched_logits_rows(h, tr, prompts, feeds, mirror_kv_dtype(), STEPS) <| $ => rdec_nrow_steps()
    h |> held_served()
    delete prompts
    delete feeds
    return <- rows
}

//! one half of the embed-gather cell on a two-region load: the batched rows, and where each token row came from - with the
//! gather on (`gathered`) every one off its id and none embedded on the host; with it off every one embedded on the host,
//! none gathered and no side row gathered off an id, the counters' controls
def regions_embed_rows(var h : Held; tr : Model; gathered : bool) : array<array<float>> {
    let g0 = home_nrow_embed_gathered()
    let h0 = batch_rows_host_embedded()
    let side0 = home_nrow_side_gathered()
    var rows <- batched_rows_logits(h, tr)
    let off_ids = gathered ? 2l * STEPS : 0l
    h |> held_equal(home_nrow_embed_gathered() - g0, off_ids, gathered
        ? "every batched row's token row gathered off its id" : "with the gather off no batched row gathered: the counter's control")
    h |> held_equal(batch_rows_host_embedded() - h0, 2l * STEPS - off_ids, gathered
        ? "the host embedded no batched row" : "with the gather off the host embedded every batched row")
    if (!gathered) {
        h |> held_equal(home_nrow_side_gathered() - side0, 0l, "with the gather off no side row gathered off an id: the side-rows cell's control")
    }
    return <- rows
}

//! the embed-gather cell's halves
//! (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`): the
//! batched rows of a load gathering the token rows on the device off
//! their ids, and of a load with the gather off
//! (`set_gpu_resident_embed(0)`, the `DASLLAMA_VK_GPU_EMBED=0` form)
//! embedding every row on the host; each `_ran` where its form
//! collected
struct GatherPair {
    gathered : array<array<float>>
    host_embedded : array<array<float>>
    gathered_ran : bool
    host_ran : bool
}

//! one half of the embed-gather cell on the form's two-region load
//! into `gp`, its readings held into `h` - `gathered` the gather on;
//! without the dasVulkan module the gathered half holds the skip
[unused_argument(gp, m, file)]   // without the dasVulkan module only the skip remains
def gather_half(var gp : GatherPair; var h : Held; m : Model; gathered : bool; file : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (gathered) {
            delete gp.gathered
            gp.gathered <- regions_embed_rows(h, m, true)
            gp.gathered_ran = true
        } else {
            delete gp.host_embedded
            gp.host_embedded <- regions_embed_rows(h, m, false)
            log_gathered_rows(m, file, gp.gathered, gp.host_embedded, 2l)
            gp.host_ran = true
        }
    } else {
        if (gathered) {
            h |> held_skip("no dasVulkan module on this build: the batched rows' device gather " +
                "is the Vulkan driver's")
        }
    }
}

//! the embed-gather cell's compare under `tag`: the two halves' rows
//! bit for bit, where both ran
def gather_pair_within(t : T?; tag : string; gp : GatherPair) {
    if (gp.gathered_ran && gp.host_ran) {
        gathered_rows_bit_same(t, tag, gp.gathered, gp.host_embedded, 2l)
    }
}

//! the coverage census over the per-32 formats' stamps of one GEMV form (`suffix`), summed; NO_CENSUS with no vulkan module
def private b32_census(suffix : string) : int64 {
    var n = 0l
    for (f in fixed_array("q51", "iq4nl32", "q4032", "q5032", "mx4")) {
        let c = census("{f}{suffix}")
        return NO_CENSUS if (c == NO_CENSUS)
        n += c
    }
    return n
}

//! what the batched cell's census holds of a per-32 routed down
//! (`regions_batched_rows`): nothing; that both commands fold it
//! under the routing weights - the summed down stamps dispatched and
//! no per-slot down leaf, each row's k slots folded as the one-row
//! command folds its own (the gemma-4 MoE); or, on the split one-row
//! forms (the fuse knob off for the load), that no row folds it - the
//! summed down stamps where they were, the fold cell's control
enum DownFold {
    unclaimed
    folded
    split_forms
}

//! the batched cell's collection on a two-region load, the routed
//! down's census as `fold` claims it
def regions_batched_rows(var h : Held; tr : Model; bar : LogitsBar = LogitsBar.bit_exact;
                         fold : DownFold = DownFold.unclaimed) {
    var pa <- prompt_a()
    var pb <- prompt_b()
    let sum0 = b32_census("_gemv_sum_cls_spv")
    let leaf0 = b32_census("_gemv_cls_spv")
    batched_steps_body(h, tr, pa, pb, bar)
    let summed = b32_census("_gemv_sum_cls_spv") - sum0
    if (sum0 != NO_CENSUS && fold == DownFold.folded) {
        h |> held_fact(summed > 0l,
            "the summed down stamps served the routed layers ({summed} dispatches)")
        h |> held_equal(b32_census("_gemv_cls_spv") - leaf0, 0l,
            "no command ran the per-slot down leaf: the rows fold the k slots as the one-row command does")
    } elif (sum0 != NO_CENSUS && fold == DownFold.split_forms) {
        h |> held_equal(summed, 0l,
            "the split forms fold no routed down: no summed down stamp dispatched")
    }
    delete pa
    delete pb
}

//! the two batched-step cells on a two-region load the caller made, each its own cell held to `bar`: at one window
//! (`regions_batched_rows`), and past the unsplit position so the split N-row command records (`regions_split_rows`)
def regions_steps_cells(t : T?; var m : Model; bar : LogitsBar) {
    unsafe {   //! the cells capture the loaded model by reference
        t |> run("two sessions, a region each: a batched step over both reads what each session reads alone, with no history resync") @capture(& m)(t : T ?) {
            held_on(t) $(var h) {
                regions_batched_rows(h, m, bar)
            }
        }
        t |> run("two sessions past the unsplit position: the split N-row command reads what each session reads alone") @capture(& m)(t : T ?) {
            held_on(t) $(var h) {
                regions_split_rows(h, m, bar)
            }
        }
    }
}

//! the split cell's collection on a two-region load: both prompts run longer than RD_UNSPLIT_POS, so the batched steps take the
//! N-row command's split attention form - the arrival counters a row apart, the last piece of each row combining
def regions_split_rows(var h : Held; tr : Model; bar : LogitsBar = LogitsBar.bit_exact) {
    var pa <- resident_prompt(PF_WINDOW_ROWS + 8l)
    var pb <- ramp_ids(PF_WINDOW_ROWS + 25l, 0l, 3000l, 91l, 6000l)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        h |> held_fact(long_length(pa) > RD_UNSPLIT_POS && long_length(pb) > RD_UNSPLIT_POS, "both prompts run past the unsplit position ({RD_UNSPLIT_POS})")
    }
    batched_steps_body(h, tr, pa, pb, bar)
    delete pa
    delete pb
}

//! the bits DASLLAMA_VK_FUSE_BISECT splits one load at a time (`vk_fuse_bisect_pin`), the fused q/k norm's the last
let FUSE_BISECT_BITS = fixed_array(1l, 2l, 4l, 8l, 16l, 32l, 64l)

//! the fuse bisect cell's base, on a two-region load the bisect pinned at no bit: the one-row steps' logits alone
def regions_bisect_base(tr : Model) : array<array<float>> {
    var pa <- prompt_a()
    var fa <- feed_a()
    var base <- logits_alone(tr, pa, fa)
    delete pa
    delete fa
    return <- base
}

//! the fuse bisect cell on a two-region load the bisect pinned at `bit`: the one-row steps within the split bar of `base`
//! (the fused and split forms round apart), the one-token-off control outside it; under the fused q/k norm's bit the
//! batched rows bit for bit the row alone on the split pair
def regions_bisect_rows(var h : Held; tr : Model; bit : int64; base : array<array<float>>) {
    var pa <- prompt_a()
    var fa <- feed_a()
    var got <- logits_alone(tr, pa, fa)
    for (st in range64(STEPS)) {
        hold_within(h, tr, got[st + 1l], base[st + 1l], base[st], "bisect bit {bit} step {st}")
    }
    delete got
    if (bit == 64l) {
        var pb <- prompt_b()
        batched_steps_body(h, tr, pa, pb, LogitsBar.bit_exact)
        delete pb
    }
    delete pa
    delete fa
}

//! one one-row step on each of `sa` and `sb`, fed `ta` and `tb`
def private pair_step(tr : Model; var sa, sb : Session; ta, tb : int64) {
    var one_a <- [ta]
    var one_b <- [tb]
    eval(tr, sa, one_a)
    eval(tr, sb, one_b)
    delete one_a
    delete one_b
}

//! a batched step before any one-row step, a one-row step on each row, a batched step again - every row under the unsplit
//! position, so the batched steps take the unsplit twin whether or not a one-row record came first - on a two-region load
def regions_batched_first_rows(var h : Held; tr : Model) {
    var pa <- prompt_a()
    var pb <- prompt_b()
    var fa <- feed_a()
    var fb <- feed_b()
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    eval(tr, sa, pa)
    eval(tr, sb, pb)
    var got_a, got_b : array<array<float>>
    with_batch_rows(tr, sa, sb) $(var ws; var sess) {
        for (st in range64(STEPS)) {
            if (st == 1l) {
                pair_step(tr, sa, sb, fa[st], fb[st])
            } else {
                var toks <- [fa[st], fb[st]]
                eval_batch(tr, ws, sess, toks)
                delete toks
            }
            got_a |> emplace <| logits_of(sa, tr.config.vocab_size)
            got_b |> emplace <| logits_of(sb, tr.config.vocab_size)
        }
    }
    h |> held_served()
    var ra <- logits_alone(tr, pa, fa)
    var rb <- logits_alone(tr, pb, fb)
    for (st in range64(STEPS)) {
        hold_equal(h, tr, got_a[st], ra[st + 1l], ra[st], "batched-first step {st}, row A")
        hold_equal(h, tr, got_b[st], rb[st + 1l], rb[st], "batched-first step {st}, row B")
    }
    delete sa
    delete sb
    delete got_a
    delete got_b
    delete ra
    delete rb
    delete pa
    delete pb
    delete fa
    delete fb
}

//! the three batched-step cells' collections on one two-region
//! load: a batched step over two sessions, the split form past the
//! unsplit position, and a batched step ahead of any one-row step
def regions_batched_once_rows(var h : Held; tr : Model; bar : LogitsBar = LogitsBar.bit_exact) {
    regions_batched_rows(h, tr, bar)
    regions_split_rows(h, tr, bar)
    regions_batched_first_rows(h, tr)
}

def private device_session(tr : Model) : Session => <- create_device_session(tr, gpu_device_session_dtype())

//! two device-home sessions prefilled over `pa` and `pb` for `body`, each parked and deleted after (one the body parked parks
//! again as a no-op: its claim is gone)
def private with_device_pair(tr : Model; pa, pb : array<int64>; body : block<(var sa : Session; var sb : Session) : void>) {
    var sa <- device_session(tr)
    var sb <- device_session(tr)
    eval(tr, sa, pa)
    eval(tr, sb, pb)
    invoke(body, sa, sb)
    gpu_device_kv_park(sa)
    gpu_device_kv_park(sb)
    delete sa
    delete sb
}

//! the device-home cell's collection on a two-region load
def regions_device_home_rows(var h : Held; tr : Model) {
    h |> held_equal(gpu_device_sessions(), 2l, "the armed driver serves a device-home session per region")
    var pa <- prompt_a()
    var pb <- prompt_b()
    var al <- alone_runs(tr, pa, pb)
    set_resident_prefill_allowed(false)   // the server's setting: the pin binds host-cached sessions, a device-home one prefills on the device all the same
    with_device_pair(tr, pa, pb) $(var sa; var sb) {
        h |> held_equal(length(sa.key_cache) + length(sa.value_cache), 0, "a device-home session allocates no host cache")
        hold_equal(h, tr, sa.logits, al.ra[0], al.ra[1], "device-home A's prefill")
        hold_equal(h, tr, sb.logits, al.rb[0], al.rb[1], "device-home B's prefill")
        let half = STEPS / 2l
        for (st in range64(half)) {
            var ta <- [al.fa[st]]
            var tb <- [al.fb[st]]
            eval(tr, sa, ta)
            hold_equal(h, tr, sa.logits, al.ra[st + 1l], al.ra[st], "device-home A single step {st}")
            eval(tr, sb, tb)
            hold_equal(h, tr, sb.logits, al.rb[st + 1l], al.rb[st], "device-home B single step {st}")
            delete ta
            delete tb
        }
        moe_gpu_hydrate_session(tr, sa)
        h |> held_equal(length(sa.key_cache) + length(sa.value_cache), 0, "hydrating a device-home session brings nothing down: it has no host cache to fill")
        with_batch_rows(tr, sa, sb) $(var ws; var sess) {
            for (st in range64(half, STEPS)) {
                var toks <- [al.fa[st], al.fb[st]]
                eval_batch(tr, ws, sess, toks)
                delete toks
                hold_equal(h, tr, sa.logits, al.ra[st + 1l], al.ra[st], "device-home batched step {st}, row A")
                hold_equal(h, tr, sb.logits, al.rb[st + 1l], al.rb[st], "device-home batched step {st}, row B")
            }
        }
        set_resident_prefill_allowed(true)
        h |> held_served("the device served every call, the prefills under the pin among them")
    }
    delete al
    delete pa
    delete pb
}

//! the pin cell's collection on a two-region load whose planes stay on the host: the outsider's call passes as busy by design
def regions_device_home_pin_rows(var h : Held; tr : Model) {
    var pa <- prompt_a()
    var pb <- prompt_b()
    var al <- alone_runs(tr, pa, pb)
    with_device_pair(tr, pa, pb) $(var sa; var sb) {
        var outsider <- create_session(tr, mirror_kv_dtype())
        eval(tr, outsider, pa)
        h |> held_only_pass("busy", "the outsider's call passed as busy, and only it")
        var ta <- [al.fa[0]]
        var tb <- [al.fb[0]]
        eval(tr, sa, ta)
        hold_equal(h, tr, sa.logits, al.ra[1], al.ra[0], "owner A's next step, its region untouched")
        eval(tr, sb, tb)
        hold_equal(h, tr, sb.logits, al.rb[1], al.rb[0], "owner B's next step, its region untouched")
        gpu_device_kv_park(sa)
        var late <- create_session(tr, mirror_kv_dtype())
        let served0 = gpu_resident_prefills_()
        eval(tr, late, pa)
        h |> held_equal(gpu_resident_prefills_(), served0 + 1l, "a parked region is anyone's: the next outsider prefills on the device")
        delete ta
        delete tb
        delete late
        delete outsider
    }
    delete al
    delete pa
    delete pb
}

//! the cell's run on a two-region load the caller made
def regions_park_adopt_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a parked claim hands its rows to a later session once: the adopter prefills only the rest") @capture(& tr)(t : T ?) {
            var prompt <- prose_ids(57l, 0l)
            var tail <- [for (i in range64(17l)); prompt[40l + i]]
            var short_logits <- short_control(tr, prompt, 57l)
            var first <- device_session(tr)
            eval(tr, first, prompt)
            var want <- logits_of(first, tr.config.vocab_size)
            let claim = gpu_device_kv_park(first)
            delete first
            t |> success(claim != 0l, "a session that prefilled on the device parks a claim")
            var second <- device_session(tr)
            let served0 = gpu_resident_prefills_()
            t |> success(!gpu_device_kv_adopt(second, claim, 999l) && second.n_past == 0l, "a claim over 57 rows refuses 999 of them, and the adopter stays empty")
            t |> success(gpu_device_kv_adopt(second, claim, 40l), "the claim stands: the adopter takes 40 of its 57 rows")
            t |> equal(second.n_past, 40l, "the adopter continues from the adopted rows")
            eval(tr, second, tail)
            t |> equal(gpu_resident_prefills_(), served0 + 1l, "one device prefill: the seventeen tokens past the adopted rows")
            hold_within(t, tr, second.logits, want, short_logits, "the adopter against the whole prompt prefilled in one call")
            delete short_logits
            var third <- device_session(tr)
            t |> success(!gpu_device_kv_adopt(third, claim, 40l), "a claim is spent by its adoption: nobody else takes the rows")
            t |> equal(third.n_past, 0l, "a refused adopter starts empty")
            let claim2 = gpu_device_kv_park(second)
            var a <- create_session(tr, mirror_kv_dtype())
            var b <- create_session(tr, mirror_kv_dtype())
            eval(tr, a, tail)
            eval(tr, b, tail)
            t |> success(!gpu_device_kv_adopt(third, claim2, 40l), "a claim dies when another session takes its region")
            gpu_device_kv_park(third)
            served_every_call(t)
            delete a
            delete b
            delete third
            delete second
            delete want
            delete prompt
            delete tail
        }
    }
}

//! a recurrent model's parked claim keeps its state at its last row beside the rows: an adopter of all of them continues the
//! stream as one call would have, and an adopter of fewer is refused, since the state stands at no earlier row
//! the cell's run on a load the caller made
def regions_park_adopt_recurrent_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a recurrent parked claim hands its rows and its state to a session that takes every row, and refuses a shorter opening") @capture(& tr)(t : T ?) {
            var prompt <- prose_ids(74l, 0l)
            var head <- [for (i in range64(57l)); prompt[i]]
            var tail <- [for (i in range64(17l)); prompt[57l + i]]
            var short_logits <- short_control(tr, prompt, 74l)
            var whole <- device_session(tr)
            eval(tr, whole, prompt)
            var want <- logits_of(whole, tr.config.vocab_size)
            gpu_device_kv_park(whole)
            delete whole
            var first <- device_session(tr)
            eval(tr, first, head)
            let claim = gpu_device_kv_park(first)
            delete first
            t |> equal(gpu_device_kv_parked_pos_(claim), 57l, "the parked claim's recurrent state stands at its last row")
            var second <- device_session(tr)
            t |> success(!gpu_device_kv_adopt(second, claim, 40l) && second.n_past == 0l, "a recurrent claim refuses 40 of its 57 rows, and the adopter stays empty")
            let served0 = gpu_resident_prefills_()
            t |> success(gpu_device_kv_adopt(second, claim, 57l), "the claim hands all 57 rows over")
            t |> equal(second.dn_pos, 57l, "the adopter's recurrent state stands where the parked stream's did")
            eval(tr, second, tail)
            t |> equal(gpu_resident_prefills_(), served0 + 1l, "one device prefill: the seventeen tokens past the adopted rows")
            t |> equal(second.dn_pos, 74l, "the continuation advanced the adopted state")
            hold_within(t, tr, second.logits, want, short_logits, "the adopter against the whole prompt prefilled in one call")
            gpu_device_kv_park(second)
            served_every_call(t)
            delete second
            delete short_logits
            delete want
            delete prompt
            delete head
            delete tail
        }
    }
}

def private cached_of(events : array<SchedEvent>; id : int64) : int64 => finished_of(events, id).n_cached

//! three requests through a two-stream scheduler, then a second turn that opens with the last one's prompt and reply
//! (two slots park two claims: the third request's arrival is what evicts the first's rows); every prompt is one prefill chunk.
//! The second turn samples at top-k 1 under a temperature - the same token as the argmax, through a sampler that is not one,
//! so its rows land logits where the other three land picks (a batched step of both kinds lands both)
def private serve_four(m : Model; device : bool; var events : array<SchedEvent>; page_rows : int64 = 0l) : tuple<on : bool; parked : int; rows_stepped : int64> {
    var sch <- create_scheduler(m, 2l, 4l, 64l, page_rows, 8l, mirror_kv_dtype())   //! the streams' caches on the mirror's codec: under DASLLAMA_VK_KV32=1 a f16 stream is not served
    let on = set_device_kv(sch, device)
    var pa <- prompt_a()
    var pb <- prompt_b()
    var pc <- prose_ids(33l, 11l)
    submit(m, sch, make_req(1l, pa, 8l))
    submit(m, sch, make_req(2l, pb, 8l))
    submit(m, sch, make_req(3l, pc, 8l))
    drain(m, sch, events)
    var turn2 <- prose_ids(33l, 11l)
    turn2 |> push_from(toks_of(events, 3l), prose_ids(20l, 5l))
    var no_stop : array<int64>
    submit(m, sch, make_req(4l, turn2, SamplingParams(temp = 0.7, top_k = 1l), 8l, no_stop))
    drain(m, sch, events)
    delete pa
    delete pb
    delete pc
    delete turn2
    let parked = length(sch.parked)
    let rows_stepped = sch.sum_batch
    delete sch
    return (on = on, parked = parked, rows_stepped = rows_stepped)
}

//! under a block-codec mirror the two sides' steps part by the codec's rounding and a near-tie flips a free-run's token, so
//! the streams hold through the prefill's pick and the first step alone; an f16 mirror holds them token for token
let private CODEC_STREAM_AGREE = 2l

//! requests `first` up to `last` of two runs side by side: eight tokens each on the `got` side, token for token the
//! `want` side's for every id below `same_below` (through `CODEC_STREAM_AGREE` tokens under a block-codec mirror)
def private hold_streams_equal(t : T?; m : Model; want, got : array<SchedEvent>;
                               first, last, same_below : int64; want_name, got_name : string) {
    held_on(t) $(var h) {
        hold_streams_equal(h, m, want, got, first, last, same_below, want_name, got_name)
    }
}

def private hold_streams_equal(var h : Held; m : Model; want, got : array<SchedEvent>;
                               first, last, same_below : int64; want_name, got_name : string) {
    let codec = kv_block_codec(mirror_kv_dtype())
    for (id in range64(first, last)) {
        var tw <- toks_of(want, id)
        var tg <- toks_of(got, id)
        log_gen_texts(m, "request {id} (CPU = {want_name}, GPU = {got_name})", tw, tg)
        h |> held_equal(length(tg), 8, "request {id} ran its eight tokens")
        if (id < same_below) {
            let both = min(long_length(tw), long_length(tg))
            let through = codec ? min(CODEC_STREAM_AGREE, both) : both
            let scope = codec ? " through the first {through} tokens (a block-codec mirror)" : ""
            h |> held_fact(long_length(tw) == long_length(tg) && ids_equal(subarray(tw, range64(0l, through)), subarray(tg, range64(0l, through))),
                           "request {id}: {got_name} {tg} against {want_name} {tw}{scope}")
        }
        delete tw
        delete tg
    }
}

//! the device-mode scheduler cell's collection on a two-region load: the host-cached run, then the device-home run
def regions_scheduler_rows(var h : Held; tr : Model) {
    var host, dev : array<SchedEvent>
    let served0 = gpu_resident_prefills_()
    h |> held_fact(!serve_four(tr, false, host).on, "the control scheduler serves host-cached sessions")
    let served1 = gpu_resident_prefills_()
    let picks0 = rdec_picks_served()
    let segw0 = segment_windows_count()
    let served = serve_four(tr, true, dev)
    h |> held_fact(served.on, "an idle scheduler switches to device-home sessions")
    if (gpu_device_prefill_segments(tr)) {   //! the two prompts admitted together ride one segmented window: the tick's witness
        h |> held_fact(segment_windows_count() - segw0 >= 1l, "the device-home tick handed its prefilling streams to the segmented window ({segment_windows_count() - segw0} windows)")
    } else {
        h |> held_equal(segment_windows_count() - segw0, 0l, "a model the segmented window declines takes a stream's own call a tick")
    }
    //! every decode step of the three argmax requests lands a pick (their first token samples off the prefill's logits inline);
    //! the sampled fourth request's steps land logits, so the count is the rows stepped less its own
    var td4 <- toks_of(dev, 4l)
    let sampled_steps = long_length(td4) - 1l
    delete td4
    h |> held_equal(rdec_picks_served() - picks0, served.rows_stepped - sampled_steps, "the device landed the argmax streams' picks alone, no logits row ({served.rows_stepped} rows stepped, {sampled_steps} of them the sampled request's)")
    h |> held_fact(served.rows_stepped - sampled_steps >= 21l, "the three argmax requests stepped seven decode rows each at least ({served.rows_stepped - sampled_steps})")
    // four turns on two stream slots: the first turn's claim is evicted by the third's, the third's is spent by the turn that adopts it
    h |> held_equal(served.parked, gpu_device_prefill_continues(tr) ? 2 : 0, "parked claims stay within the stream slots, and a model whose prefill cannot continue parks none")
    // one code path on both sides: each prompt enters the window chain once and each step the batched token command;
    // a device-home stream differs by the host copy it skips. The adopting turn splits its prefill, so it holds no token compare
    h |> held_equal(served1 - served0, 4l, "the host-cached side prefilled its four prompts on the device, one call each")
    h |> held_equal(gpu_resident_prefills_() - served1, 4l, "and so did the device-home side")
    let same_below = gpu_device_prefill_continues(tr) ? 4l : 5l
    hold_streams_equal(h, tr, host, dev, 1l, 5l, same_below, "host-cached", "device-home")
    h |> held_equal(cached_of(host, 4l), 0l, "the host-cached flat scheduler keeps no prefix")
    if (gpu_device_prefill_continues(tr)) {
        h |> held_fact(cached_of(dev, 4l) >= 33l, "the second turn adopted the rows of the turn it continues from the device ({cached_of(dev, 4l)} of them)")
    } else {
        h |> held_equal(cached_of(dev, 4l), 0l, "a recurrent model's turn prefills whole, from zero: nothing is adopted")
    }
    h |> held_served("the device served every call of both runs")
    delete host
    delete dev
}

//! the device-mode scheduler's no-cache request on a two-region load, its cache-on control after it
def regions_scheduler_no_cache_rows(var h : Held; tr : Model) {
    var sch <- create_scheduler(tr, 2l, 4l, 64l, 0l, 8l, mirror_kv_dtype())
    h |> held_fact(set_device_kv(sch, true), "the scheduler serves device-home sessions")
    var events : array<SchedEvent>
    var prompt <- prose_ids(33l, 11l)
    submit(tr, sch, make_req(1l, prompt, 8l))
    drain(tr, sch, events)
    let claims = length(sch.parked)
    var uncached <- make_req(2l, prompt, 8l)
    uncached.no_cache = true
    submit(tr, sch, uncached)
    drain(tr, sch, events)
    h |> held_equal(cached_of(events, 2l), 0l, "a no-cache request adopts nothing off a region that holds its prompt")
    h |> held_equal(length(sch.parked), claims, "... and parks no claim of its own ({length(sch.parked)} parked, {claims} before it)")
    submit(tr, sch, make_req(3l, prompt, 8l))
    drain(tr, sch, events)
    if (tr.config.recr_mask != 0ul) {
        h |> held_cell_skip("control: the same prompt with the cache on - a recurrent claim keeps its state at its last row only, so an opening that ends at the prompt boundary adopts nothing")
    } else {
        h |> held_fact(cached_of(events, 3l) > 0l, "control: the same prompt with the cache on adopts the first request's rows ({cached_of(events, 3l)} of them)")
    }
    h |> held_served()
    delete prompt
    delete events
    delete sch
}

//! a paged scheduler's host-cached streams on the resident driver: their pages go up a physical run at a time, the rows the device
//! stepped come down into the pages before the prefix cache takes them, and a second turn attaches the first turn's pages
//! the cell's run on a load the caller made
def regions_scheduler_paged_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a paged scheduler's streams step on the device: token for token the flat scheduler's, and a second turn attaches the turn it continues") @capture(& tr)(t : T ?) {
            var flat, paged : array<SchedEvent>
            serve_four(tr, false, flat)
            let served0 = gpu_resident_prefills_()
            let steps0 = rdec_picks_served()
            serve_four(tr, false, paged, 64l)
            t |> success(gpu_resident_prefills_() - served0 >= 4l, "the paged side's prompts entered the window chain ({gpu_resident_prefills_() - served0} calls)")
            t |> success(rdec_picks_served() - steps0 >= 21l, "the paged side's argmax streams stepped on the device ({rdec_picks_served() - steps0} picks landed)")
            //! the second turn attaches its opening and prefills the rest in a
            //! call of its own: no token compare holds across that split
            hold_streams_equal(t, tr, flat, paged, 1l, 5l, 4l, "flat", "paged")
            t |> success(cached_of(paged, 4l) > 0l, "the second turn attached the pages its first turn left ({cached_of(paged, 4l)} tokens)")
            served_every_call(t)
            delete flat
            delete paged
        }
    }
}

//! two requests on one system prompt, one after the other, on a one-stream scheduler - device-home, or paged host-cached (64-row
//! pages) - every prefill chunk a window, both requests marking the system prompt's end as their stable opening
def private serve_shared_opening(m : Model; device : bool; system, u1, u2 : array<int64>; var events : array<SchedEvent>) : tuple<cached : int64; computed : int64> {
    var sch <- create_scheduler(m, 1l, 4l, PF_WINDOW_ROWS, device ? 0l : 64l, 8l, mirror_kv_dtype())
    set_device_kv(sch, device)
    for (id in [1l, 2l]) {
        var prompt := system
        prompt |> push_from(id == 1l ? u1 : u2)
        var req <- make_req(id, prompt, 8l)
        req.system_at = long_length(system)
        req.stable_at = long_length(system)
        submit(m, sch, req)
        drain(m, sch, events)
        delete prompt
    }
    let out = (cached = sch.cached_tokens, computed = sch.prefill_tokens)
    set_device_kv(sch, false)
    delete sch
    return out
}

//! a device-home recurrent stream keeps its state at the system prompt's end beside its rows: a second request on the same system
//! prompt with another user turn adopts that checkpoint off the parked claim and prefills its own turn alone, token for token what
//! the host-cached paged scheduler - whose prefix cache keeps the same checkpoint - emits
//! the cell's run on a load the caller made
def regions_checkpoint_adopt_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a device-home recurrent stream's checkpoint at the system prompt's end serves the next request on that system prompt, token for token the host-cached prefix cache's run") @capture(& tr)(t : T ?) {
            var system <- prose_ids(100l, 3l)
            var u1 <- prose_ids(30l, 17l)
            var u2 <- prose_ids(30l, 29l)
            var host, dev : array<SchedEvent>
            let h = serve_shared_opening(tr, false, system, u1, u2, host)
            let d = serve_shared_opening(tr, true, system, u1, u2, dev)
            t |> equal(cached_of(dev, 2l), 100l, "the second request adopted the system prompt's checkpoint off the parked claim")
            t |> equal(d.cached, 100l, "the device-home run's cache: the system prompt once ({d.cached} tokens)")
            t |> equal(d.computed, 130l + 30l, "the device-home run computed the first prompt and the second's user turn alone ({d.computed} tokens)")
            t |> equal(cached_of(host, 2l), 100l, "the host-cached run attached the same checkpoint")
            t |> equal(h.computed, d.computed, "both runs computed the same tokens")
            hold_streams_equal(t, tr, host, dev, 1l, 3l, 3l, "host-cached paged", "device-home")
            served_every_call(t)
            delete host
            delete dev
            delete system
            delete u1
            delete u2
        }
    }
}

//! what one five-request run left: the cache's checkpoints, and the pool's row traffic over the run
struct private PooledRun {
    states : int
    evicted : int64
    imports : int64
    exports : int64
    drafted : int64
    accepted : int64
}

//! Five requests one after another on a two-stream scheduler - device-home or host-cached, paged (64-row pages) or flat - over
//! three 100-token system prompts, more openings than the two regions hold: the first three open one each, the fourth opens
//! with the first's system prompt again, the fifth continues the second's conversation (its prompt, its eight tokens, a new
//! turn). Every request marks its system prompt's end as its system and stable opening; `max_states` caps the checkpoints.
def private serve_pooled(m : Model; device : bool; page_rows, max_states : int64; var events : array<SchedEvent>; mtp : bool = false) : PooledRun {
    var sch <- create_scheduler(m, 2l, 8l, PF_WINDOW_ROWS, page_rows, 0l, mirror_kv_dtype(), mtp, -1l, max_states)
    set_device_kv(sch, device)
    let moves0 = gpu_device_kv_pool_moves()
    var systems <- [for (k in range64(3l)); prose_ids(100l, 3l + k)]
    for (id in range64(1l, 6l)) {
        let sys = id <= 3l ? id - 1l : (id == 4l ? 0l : 1l)
        var prompt := systems[sys]
        prompt |> push_from(prose_ids(30l, 37l + 4l * (id == 5l ? 2l : id)))   //! the fifth repeats the second's turn, then its reply and a turn of its own
        if (id == 5l) {
            prompt |> push_from(toks_of(events, 2l), prose_ids(30l, 53l))
        }
        var req <- make_req(id, prompt, 8l)
        req.system_at = 100l
        req.stable_at = 100l
        submit(m, sch, req)
        drain(m, sch, events)
        delete prompt
    }
    let moves = gpu_device_kv_pool_moves()
    let out = PooledRun(states = length(sch.prefix.states), evicted = sch.prefix.n_state_evicted,
                        imports = moves.imports - moves0.imports, exports = moves.exports - moves0.exports,
                        drafted = sch.mtp_drafted, accepted = sch.mtp_accepted)
    set_device_kv(sch, false)
    delete systems
    delete sch
    return out
}

//! the pool scenario's openings: the fourth request attached the first system prompt and the fifth the second
//! conversation's finished turn on both schedulers, two openings came up from the pool, and every request's tokens agree
//! the lanes a speculating cell's routes read, pinned: the resident prefill allowed, one draft row a round, speculation as the
//! cell sets it; `restore_spec_lanes` puts every value back from its getter
struct private SpecLanes {
    spec : bool
    prefill : bool
    depth : int64
}

def private pin_spec_lanes() : SpecLanes {
    let pins = SpecLanes(spec = get_mtp_spec(), prefill = resident_prefill_allowed(), depth = get_mtp_depth_nextn())
    set_resident_prefill_allowed(true)
    set_mtp_depth_nextn(1l)
    return pins
}

def private restore_spec_lanes(pins : SpecLanes) {
    set_mtp_spec(pins.spec)
    set_resident_prefill_allowed(pins.prefill)
    set_mtp_depth_nextn(pins.depth)
}

let private POOLED_HISTORY = 100l + 30l + 8l   //! the fifth request's opening: the second system prompt, its turn and its reply

def private hold_pool_openings(t : T?; m : Model; host, pooled : array<SchedEvent>; p : PooledRun; host_name, pooled_name : string) {
    let history = POOLED_HISTORY
    t |> equal(cached_of(host, 4l), 100l, "the host-cached run attached the first system prompt for the fourth request")
    t |> equal(cached_of(host, 5l), history, "and the second conversation's turn for the fifth")
    t |> equal(cached_of(pooled, 4l), 100l, "the fourth request took the first system prompt up from the pool: the region that held it went to the third request")
    t |> equal(cached_of(pooled, 5l), history, "the fifth took the second conversation's finished turn up ({history} tokens): its region went to the fourth")
    t |> equal(p.imports, 2l, "two openings came up from the pool")
    hold_streams_equal(t, m, host, pooled, 1l, 6l, 6l, host_name, pooled_name)
}

//! The host pool behind the regions: openings a region lost - its parked claim evicted by a later stream, its checkpoint row
//! reused - come back from host RAM. On the paged device-home scheduler the fourth request takes its system prompt's checkpoint
//! (a recurrent model) or its pages (a dense one) up from the pool after two other streams took both regions, and the fifth the
//! second conversation's finished turn, token for token the host-cached paged scheduler, whose prefix cache keeps the same
//! openings; the flat device-home scheduler - no pool - is the control, both requests prefilled whole. On a recurrent model a
//! pool of four checkpoints drops the tails first: the fourth request still finds its opening, the fifth only its system prompt.
//! The host-cached and pooled runs compare token for token: both
//! decode on the device over the same prefill cuts, and the pool
//! moves bytes, not numerics. With DASLLAMA_DEVICE_POOL=0 the cell
//! holds the off rail instead.
//! the cell's run on a load the caller made
def regions_pool_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a device-home scheduler's openings outlive their regions in the host pool: a later request takes them up, token for token the host-cached prefix cache's run") @capture(& tr)(t : T ?) {
            if (!gpu_device_kv_pool_armed()) {
                //! the off rail: the regions alone, lost openings prefilled whole
                var off : array<SchedEvent>
                let o = serve_pooled(tr, true, 64l, 256l, off)
                t |> equal(o.imports, 0l, "DASLLAMA_DEVICE_POOL=0: nothing comes up from a pool")
                t |> equal(o.exports, 0l, "and nothing goes down to one")
                t |> equal(cached_of(off, 4l), 0l, "the fourth request prefills whole")
                t |> equal(cached_of(off, 5l), 0l, "and so does the fifth")
                served_every_call(t)
                delete off
                return
            }
            let recurrent = tr.config.recr_mask != 0ul
            let spec0 = get_mtp_spec()
            set_mtp_spec(false)   //! the pool's moves read it: a carrier with a draft head rides its rows only while speculation is on
            var host, pooled, flat, capped : array<SchedEvent>
            serve_pooled(tr, false, 64l, 256l, host)
            let p = serve_pooled(tr, true, 64l, 256l, pooled)
            let f = serve_pooled(tr, true, 0l, 256l, flat)
            hold_pool_openings(t, tr, host, pooled, p, "host-cached paged", "device-home pooled")
            t |> equal(p.exports, recurrent ? 6l : 3l, "each finished turn went down as its region went to another stream, and on a recurrent model each new system prompt's checkpoint as its stream parked; the last two turns stay the regions' ({p.exports} copies)")
            t |> equal(cached_of(flat, 4l), 0l, "control: with no pool the fourth request prefills whole - its opening's region is gone")
            t |> equal(cached_of(flat, 5l), 0l, "and so does the fifth")
            t |> equal(f.imports, 0l, "the flat scheduler keeps no pool")
            if (recurrent) {
                let c = serve_pooled(tr, true, 64l, 4l, capped)
                t |> equal(cached_of(capped, 4l), 100l, "four checkpoints: the system prompts are openings, kept over the tails")
                t |> equal(cached_of(capped, 5l), 100l, "the second conversation's turn - a tail - went first, so the fifth request finds its system prompt alone")
                t |> success(c.states == 4 && c.evicted == 2l, "the pool held four checkpoints, dropping the two oldest tails ({c.states} standing, {c.evicted} dropped)")
                var tc <- toks_of(capped, 5l)
                t |> equal(length(tc), 8, "the fifth request ran its eight tokens")
                delete tc
            }
            served_every_call(t)
            set_mtp_spec(spec0)
            delete host
            delete pooled
            delete flat
            delete capped
        }
    }
}

//! The pool under a speculating scheduler: a device-home stream's draft head holds its own K/V rows in the region, and an
//! opening that goes down to the pool carries them - a later request that takes the opening up drafts off the head's rows
//! as the host-cached speculating scheduler does, token for token, every round drafted and verified on the device.
//! the cell's run on a load the caller made with the
//! planes on the host
def regions_pool_mtp_cell(t : T?; tr : Model; file : string) {
    unsafe {
        t |> run("a speculating device-home scheduler keeps its openings in the host pool: the draft head's rows ride the moves, and a later request drafts off them as the host-cached speculating scheduler does") @capture(& tr)(t : T ?) {
            if (!gpu_device_kv_pool_armed()) {
                t |> skip("DASLLAMA_DEVICE_POOL=0: the pool is off")
                return
            }
            if (!mtp_capable_(tr)) {
                t |> skip("{file} carries no draft head")
                return
            }
            if (!gpu_resident_mtp_device_home_(tr)) {
                t |> skip("the driver does not run every seat of the round on the device for {file}: a speculating slot keeps host-cached sessions")
                return
            }
            let pins <- pin_spec_lanes()
            set_mtp_spec(false)
            var plain, host, pooled : array<SchedEvent>
            let control = serve_pooled(tr, true, 64l, 256l, plain)
            set_mtp_spec(true)
            let h = serve_pooled(tr, false, 64l, 256l, host, true)
            let p = serve_pooled(tr, true, 64l, 256l, pooled, true)
            restore_spec_lanes(pins)
            t |> equal(control.drafted, 0l, "control: the plain device-home scheduler drafted nothing")
            t |> equal(control.imports, 2l, "control: two openings came up from the pool with nothing speculating")
            t |> equal(cached_of(plain, 4l), 100l, "control: the fourth request took the first system prompt up")
            t |> equal(cached_of(plain, 5l), POOLED_HISTORY, "control: and the fifth the second conversation's finished turn")
            t |> success(h.drafted > 0l, "the host-cached speculating scheduler drafted ({h.drafted} drafts, {h.accepted} accepted)")
            t |> success(p.drafted > 0l, "the device-home speculating scheduler drafted ({p.drafted} drafts, {p.accepted} accepted)")
            hold_pool_openings(t, tr, host, pooled, p, "host-cached paged speculating", "device-home pooled speculating")
            t |> success(p.exports > 0l, "openings went down to the pool ({p.exports})")
            t |> equal(p.drafted, h.drafted, "the device-home scheduler drafted as many rounds as the host-cached one")
            t |> equal(p.accepted, h.accepted, "and accepted as many")
            served_every_call(t)
            delete plain
            delete host
            delete pooled
        }
    }
}

//! The head rows' ride itself, outside the scheduler: a device-home session's opening parked, exported to a paged carrier
//! and imported into a fresh device-home session, which counts the head's rows to the position less one and drafts, off
//! those rows, the logits the session that went down drafts.
//! the cell's run on a load the caller made
def regions_pool_head_cell(t : T?; tr : Model; file : string) {
    unsafe {
        t |> run("an opening down to the pool and up again carries the draft head's rows: the fresh session counts them and drafts what the session that went down drafts") @capture(& tr)(t : T ?) {
            if (!gpu_device_kv_pool_armed()) {
                t |> skip("DASLLAMA_DEVICE_POOL=0: the pool is off")
                return
            }
            if (!mtp_capable_(tr)) {
                t |> skip("{file} carries no draft head")
                return
            }
            if (!gpu_resident_mtp_device_home_(tr)) {
                t |> skip("the driver does not run every seat of the round on the device for {file}")
                return
            }
            let pins <- pin_spec_lanes()
            set_mtp_spec(true)
            let n = 74l
            let tok = 2000l
            var prompt <- prose_ids(n, 0l)
            var down <- device_session(tr)
            eval(tr, down, prompt)
            t |> equal(moe_gpu_resident_head_rows(down), n - 1l, "the window chain warmed the head's rows under the position")
            var h0 <- [for (x in down.mtp_h); x]
            let drafts0 = moe_gpu_resident_drafts()
            var want <- draft_from(tr, down, tok, n - 1l, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the session that goes down drafted on the device")
            let claim = gpu_device_kv_park(down)
            delete down
            var pool <- create_kv_pool(tr, 64l, gpu_device_session_dtype())
            var carrier <- create_session(tr, pool)
            t |> success(gpu_device_kv_export(tr, claim, carrier, 0l, n), "the opening went down to a paged carrier")
            var up <- device_session(tr)
            t |> success(gpu_device_kv_import(tr, up, carrier, n), "and came up into a fresh device-home session")
            t |> equal(moe_gpu_resident_head_rows(up), n - 1l, "the fresh session counts the head's rows to the position less one")
            var got <- draft_from(tr, up, tok, n - 1l, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 2l, "and the fresh session drafted on the device too")
            let peak = logits_maxabs(want.logits)
            let gap = logits_maxdiff(got.logits, want.logits)
            t |> equal(argmax_of(got.logits), argmax_of(want.logits), "the fresh session's draft picks what the session that went down picks")
            t |> success(gap <= 1e-4 * peak, "and its logits are that draft's ({gap} of {peak})")
            gpu_device_kv_park(up)
            served_every_call(t)
            restore_spec_lanes(pins)
            delete up
            delete carrier
            delete pool
            delete got
            delete want
            delete h0
            delete prompt
        }
    }
}

//! the ring-floor cell's collection on a ringed two-region load (`ARCHITECTURE_GPU_KV_RING.md#kv-ring-floor`): a prompt past the
//! window leaves the floor at its rows less the ring's slack, a call under it panics, a prompt from 0 resets the region's
//! high-water mark and the floor with it; a parked claim, an export and an import each refuse a cut under the floor and carry the
//! floor on the rows they move
def regions_ring_floor_rows(var h : Held; tr : Model) {
    let slack = gpu_resident_kv_ring_slack()
    h |> held_fact(slack > 0l, "the ringed load armed its rings (slack {slack})")
    return if (slack <= 0l)
    let n = max(1300l, slack + 300l)   //! past the ring's slack on any card (2048 rows on the RTX PRO 4500), so the floor stands above the 40-row calls
    let floor = n - slack
    var prompt <- prose_ids(n, 0l)
    var short <- prose_ids(40l, 1l)
    var s <- create_session(tr, mirror_kv_dtype())
    eval(tr, s, prompt)
    h |> held_equal(s.kv_ring_floor, floor, "the prompt's rows less the ring's slack are the floor")
    s.n_past = 40l
    let why = panic_text() ${
        var one <- [prompt[40]]
        eval(tr, s, one)
    }
    h |> held_fact(find(why, "from position {floor} on") >= 0, "a call at 40 continues under the floor and panics: {why}")
    s.n_past = 0l
    eval(tr, s, short)
    h |> held_equal(s.kv_ring_floor, 0l, "a prompt from 0 rebuilds the history: its 40 rows reach no ring, so no floor")
    s.n_past = 0l
    eval(tr, s, prompt)
    h |> held_equal(s.kv_ring_floor, floor, "the long prompt again raises the floor")
    s.n_past = 0l
    var row0 <- [short[0]]
    eval(tr, s, row0)
    h |> held_equal(s.kv_ring_floor, 0l, "a one-token decode of row 0 rebuilds the history too: the claim's high-water mark drops with it, so no floor")
    let why1 = panic_text() ${
        var row1 <- [short[1]]
        eval(tr, s, row1)
    }
    h |> held_equal(why1, "", "and the decode at 1 continues (a bench's tg rows after its pp rows)")
    delete row0
    delete s
    var d <- device_session(tr)
    eval(tr, d, prompt)
    h |> held_equal(d.kv_ring_floor, floor, "a device-home session's floor is the same")
    let claim = gpu_device_kv_park(d)
    delete d
    var a <- device_session(tr)
    h |> held_fact(!gpu_device_kv_adopt(a, claim, 40l) && a.n_past == 0l, "a parked claim refuses an opening under its floor")
    h |> held_fact(gpu_device_kv_adopt(a, claim, n), "and hands every row over (the control)")
    h |> held_equal(a.kv_ring_floor, floor, "the adopter carries the claim's floor")
    let claim2 = gpu_device_kv_park(a)
    delete a
    var pool <- create_kv_pool(tr, 64l, gpu_device_session_dtype())
    var carrier <- create_session(tr, pool)
    h |> held_fact(!gpu_device_kv_export(tr, claim2, carrier, 0l, 40l), "an export ending under the floor refuses")
    h |> held_fact(gpu_device_kv_export(tr, claim2, carrier, 0l, n), "an export of every row goes down (the control)")
    h |> held_equal(carrier.kv_ring_floor, floor, "the carrier carries the floor")
    var up <- device_session(tr)
    h |> held_fact(!gpu_device_kv_import(tr, up, carrier, 40l), "an import ending under the carrier's floor refuses")
    h |> held_fact(gpu_device_kv_import(tr, up, carrier, n), "an import of every row comes up (the control)")
    h |> held_equal(up.kv_ring_floor, floor, "the imported session carries the floor")
    gpu_device_kv_park(up)
    delete up
    delete carrier
    delete pool
    delete short
    delete prompt
}

//! the cell's run on a load the caller made
def regions_scheduler_chunks_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a device-home stream prefills in the device's windows, and a model whose prefill cannot continue in one call") @capture(& tr)(t : T ?) {
            var sch <- create_scheduler(tr, 2l, 4l, 64l, 0l, 0l, mirror_kv_dtype())
            t |> success(set_device_kv(sch, true), "an idle scheduler switches to device-home sessions")
            var long_prompt <- prose_ids(PF_WINDOW_ROWS + 188l, 3l)
            var events : array<SchedEvent>
            let served0 = gpu_resident_prefills_()
            submit(tr, sch, make_req(1l, long_prompt, 2l))
            drain(tr, sch, events)
            let calls = gpu_resident_prefills_() - served0
            // the scheduler's own chunk is 64 tokens: eleven calls, were the device window not the floor
            t |> equal(calls, gpu_device_prefill_continues(tr) ? 2l : 1l, "{long_length(long_prompt)} tokens reach the device in window-sized chunks, or whole where the model's prefill cannot continue ({calls} calls)")
            t |> equal(length(toks_of(events, 1l)), 2, "and the stream ran its two tokens")
            served_every_call(t)
            set_device_kv(sch, false)
            delete events
            delete long_prompt
            delete sch
        }
    }
}

//! a device-home scheduler's tick on two streams whose first has fewer rows left than the quantum: where the model's segmented
//! window serves, the tick hands both to one window and the second stream's prompt is cut there (A 300 + B 212, B 512, B 300:
//! four calls counted a segment); where it declines, each tick is one stream's own call (A 300, B 512, B 512: three; A 300,
//! B 1024: two where the model's prefill cannot continue) - the handoff never costs a declining model a window
//! the handoff cell's collection on a two-region load
def regions_scheduler_handoff_rows(var h : Held; tr : Model) {
    var sch <- create_scheduler(tr, 2l, 4l, 64l, 0l, 0l, mirror_kv_dtype())
    h |> held_fact(set_device_kv(sch, true), "an idle scheduler switches to device-home sessions")
    var pa <- prose_ids(300l, 5l)
    var pb <- prose_ids(2l * PF_WINDOW_ROWS, 9l)
    var events : array<SchedEvent>
    let served0 = gpu_resident_prefills_()
    let segw0 = segment_windows_count()
    submit(tr, sch, make_req(1l, pa, 2l))
    submit(tr, sch, make_req(2l, pb, 2l))
    drain(tr, sch, events)
    let calls = gpu_resident_prefills_() - served0
    if (gpu_device_prefill_segments(tr)) {
        h |> held_equal(calls, 4l, "the first tick's window carried both streams and cut the second's prompt there ({calls} calls)")
        h |> held_fact(segment_windows_count() - segw0 >= 1l, "... through the segmented window")
    } else {
        let whole = gpu_device_prefill_continues(tr) ? 2l : 1l
        h |> held_equal(calls, 1l + whole, "each tick took one stream's own call, the second stream's prompt whole windows or whole where the model's prefill cannot continue ({calls} calls)")
        h |> held_equal(segment_windows_count() - segw0, 0l, "... no segmented window")
    }
    h |> held_equal(length(toks_of(events, 1l)), 2, "the first stream ran its two tokens")
    h |> held_equal(length(toks_of(events, 2l)), 2, "the second stream ran its two tokens")
    h |> held_served()
    set_device_kv(sch, false)
    delete events
    delete pa
    delete pb
    delete sch
}

//! the cell's run on a two-region load the caller made: the refused call passes as busy by design, so a
//! caller whose load reads the served witness after resets the passes (`reset_gpu_cpu_passes`)
def regions_guards_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a device-home session that finds no region panics with the reason, and the owners step on") @capture(& tr)(t : T ?) {
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var ra <- logits_alone(tr, pa, fa)
            with_device_pair(tr, pa, pb) $(var sa; var _sb) {
                var third <- device_session(tr)
                var tripped = false
                try {
                    eval(tr, third, pa)
                } recover {
                    tripped = true
                }
                t |> success(tripped, "a third device-home session over two regions has nowhere to keep its rows: the CPU cannot take it, so it panics")
                only_pass(t, "busy", "the refused call was counted as busy, and only it")
                var ta <- [fa[0]]
                eval(tr, sa, ta)
                hold_equal(t, tr, sa.logits, ra[1], ra[0], "owner A's next step after the refused session")
                gpu_device_kv_park(third)
                delete ta
                delete third
            }
            delete ra
            delete pa
            delete pb
            delete fa
        }
    }
}

//! the driver's reasons, plan and status, run on a two-region load the caller made, the region count's lever put
//! back as the run found it
def regions_reports_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("every reason a call can pass to the CPU has words of its own, the plan sizes every region, and the status counts the mirrors") @capture(& tr)(t : T ?) {
            let n = gpu_cpu_pass_reason_count()
            t |> success(n >= 10, "the armed driver registered its pass reasons ({n})")
            for (i in range(n)) {
                note_gpu_cpu_pass(i)
            }
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), n, "every reason fired once")
            var seen : table<string>
            for (p in passes) {
                t |> success(!empty(p.words) && p.words != p.reason && !key_exists(seen, p.words), "{p.reason} reads in words of its own: {p.words}")
                seen |> insert(p.words)
            }
            delete passes
            delete seen
            reset_gpu_cpu_passes()
            var none <- gpu_cpu_passes_()
            t |> equal(length(none), 0, "a reset counter reports nothing")
            delete none
            let kv = mirror_kv_dtype()
            let cap = gpu_device_session_ctx()
            t |> success(cap > 0l && cap <= REGIONS_RIG_CTX, "a region's context is the rig's cap or under it ({cap})")
            let regions0 = gpu_resident_regions_want()
            set_gpu_resident_regions_(1l)
            let one = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(4l)
            let four = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(0l)
            let zero = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(2l)
            let two = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(regions0)
            t |> success(one.kv_bytes > 0l && four.kv_bytes == 4l * one.kv_bytes, "four regions plan four times one region's K/V ({four.kv_bytes} against {one.kv_bytes})")
            t |> equal(zero.kv_bytes, one.kv_bytes, "a region count under one plans one region")
            let held = gpu_tier_status_().vram_bytes
            t |> success(held >= two.kv_bytes, "the status counts both mirror sides of the two regions this load armed ({held} against {two.kv_bytes})")
            let weights_held = held - two.kv_bytes
            t |> success(abs(weights_held - two.weight_bytes) * 50l <= two.weight_bytes, "the plan's weight tally is the upload's within 2% ({two.weight_bytes} planned against {weights_held} held): a plane tallied twice or never lands here")
        }
    }
}

//! `cell_driver_reports`'s decline read on a load the caller made with the whole-model route switched off: the read is the
//! driver's, `tr` the load it reads
[unused_argument(tr)]
def regions_route_off_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a load under a switched-off whole-model route says it was never attempted") @capture(& tr)(t : T ?) {
            t |> success(!moe_gpu_resident_active(), "the whole-model driver did not arm")
            t |> success(gpu_resident_decline_() |> starts_with("not attempted"), "and the reason says so: {gpu_resident_decline_()}")
        }
    }
}

//! the gap pass on a two-region load the caller made with the planes on the host: the call passes as gap by design, so a
//! caller whose load reads the served witness after resets the passes (`reset_gpu_cpu_passes`)
def regions_gap_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a prefill that skips positions passes as a gap, its device rows brought down first") @capture(& tr)(t : T ?) {
            var head <- prose_ids(40l, 0l)
            var tail <- prose_ids(10l, 9l)
            var s <- create_session(tr, mirror_kv_dtype())
            eval(tr, s, head)
            t |> success(!s.rdec_host_current, "the prompt's rows live on the device alone")
            forward_prefill(tr, s, tail, 10l, 50l)
            only_pass(t, "gap", "positions 40 to 49 were never written: the call passed as a gap", 1l)
            t |> success(s.rdec_host_current, "and the CPU took it over rows brought down from the device")
            delete s
            delete head
            delete tail
        }
    }
}

//! the regions' context the cap cell's load arms: the smallest the driver arms a capped mirror at, its host-cached sessions
//! at the rig's context past it
let REGIONS_CAPPED_CTX = CAPPED_RIG_CTX

//! the cap pass on a two-region load the caller made at `REGIONS_CAPPED_CTX` with the planes on the host and the host-cached
//! sessions at the rig's context: the call passes as cap by design, so a caller whose load reads the served witness after
//! resets the passes (`reset_gpu_cpu_passes`)
def regions_cap_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("a prefill past the region's context passes as cap, its device rows brought down first") @capture(& tr)(t : T ?) {
            t |> equal(gpu_device_session_ctx(), CAPPED_RIG_CTX, "the regions armed at the capped context")
            var head <- prose_ids(CAPPED_RIG_CTX - 96l, 0l)
            var tail <- prose_ids(200l, 9l)
            var s <- create_session(tr, mirror_kv_dtype())
            eval(tr, s, head)
            forward_prefill(tr, s, tail, 200l, CAPPED_RIG_CTX - 96l)
            only_pass(t, "cap", "200 tokens from 96 short of the region's end: the call passed as cap", 1l)
            t |> success(s.rdec_host_current, "and the CPU took it over rows brought down from the device")
            delete s
            delete head
            delete tail
        }
    }
}

//! the rig's cells a carrier's two-region form runs on one load, each collection into `h`, `rel` the continuations' bar: the
//! driver's reports first, then two sessions interleaved, a prefill continued past zero, batched steps at one window and past the unsplit position,
//! device-home sessions, a parked claim adopted, the device-home guards (their by-design busy pass reset after), then the
//! device-mode scheduler's cells, whose streams leave parked claims behind
def regions_rig_rows(t : T?; var h : Held; tr : Model; rel : float) {
    regions_reports_cell(t, tr)   //! first: its status reads the load's upload alone, before a cell's batch workspace lands beside it
    regions_interleaved_rows(h, tr)
    regions_continues_cell(t, tr, rel)
    regions_batched_rows(h, tr)
    regions_split_rows(h, tr)
    regions_device_home_rows(h, tr)
    regions_park_adopt_cell(t, tr)
    regions_guards_cell(t, tr)
    reset_gpu_cpu_passes()
    regions_scheduler_rows(h, tr)
    regions_scheduler_no_cache_rows(h, tr)
    regions_scheduler_chunks_cell(t, tr)
    regions_scheduler_handoff_rows(h, tr)
}

//! the rig's cells a carrier's two-region form with the planes on the host runs on one load: the device-home pin's outsider
//! and the gap pass, each pass by design reset after it
def regions_host_rig_rows(t : T?; var h : Held; tr : Model) {
    regions_device_home_pin_rows(h, tr)
    reset_gpu_cpu_passes()
    regions_gap_cell(t, tr)
    reset_gpu_cpu_passes()
}

//! the rig's forms a carrier loads: two regions (the rig), the same with the planes on the host (the pin and the gap pass),
//! at the capped context with the host-cached sessions at the rig's (the cap pass), and the per-op form (the route off)
def regions_rig_forms() : array<ModelForm> {
    return <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX), device_form(regions = 2l, host = true, ctx = REGIONS_RIG_CTX),
        device_form(regions = 2l, host = true, ctx = REGIONS_CAPPED_CTX, host_ctx = REGIONS_RIG_CTX), device_form(perop = true)]
}

//! the rig's forms (`regions_rig_forms`), each asking the codec `kv`
def regions_rig_forms(kv : KVDtype) : array<ModelForm> =>
    <- [for (f in regions_rig_forms()); asking(f, kv)]

//! one of the rig's two-region forms' cells on its load into `h`, `rel` the continuations' bar: the cap pass at the capped
//! context (by design: reset after, as the form's witness reads the calls after it), the host rig with the planes on the
//! host, the rig otherwise
def regions_rig_form_rows(t : T?; var h : Held; tr : Model; form : ModelForm; rel : float) {
    if (form.ctx == REGIONS_CAPPED_CTX) {
        regions_cap_cell(t, tr)
        reset_gpu_cpu_passes()
    } elif (form.host) {
        regions_host_rig_rows(t, h, tr)
    } else {
        regions_rig_rows(t, h, tr, rel)
    }
}

//! the census key of the decode attention stamp the armed mirror codec and `tr`'s head shape pick, its slab asserted
//! `slab_heads` q heads wide ("" with no vulkan module)
def private da_attn_stamp_key(var h : Held; tr : Model; slab_heads : int) : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let kv_mul = tr.config.n_heads / tr.config.n_kv_heads
        h |> held_equal(da_slab_heads(kv_mul), int64(slab_heads), "the carrier's {kv_mul} q heads a kv head pick the {slab_heads}-head slab")
        return "{da_attn_stamp_name(mirror_kv_dtype(), da_slab_of(tr.config.attn_sinks, kv_mul))}_cls_spv"
    } else {
        return ""
    }
}

//! the stamp cell's collection on a two-region load of the Vulkan home: a session stepped once through the driver dispatches the
//! decode attention stamp of the armed mirror codec that the carrier's head shape picks - a slab `slab_heads` q heads wide - the
//! pick a logits compare cannot see, the census can; a held skip without a census
def regions_stamp_rows(var h : Held; tr : Model; slab_heads : int) {
    var pa <- prompt_a()
    var fa <- feed_a()
    var s <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, s, pa, long_length(pa), 0l)
    let key = da_attn_stamp_key(h, tr, slab_heads)
    let before = census(key)
    forward(tr, s, fa[0], long_length(pa))
    let after = census(key)
    if (before == NO_CENSUS) {
        h |> held_skip("no kernel census on this build (no dasVulkan module): the stamp's dispatch cannot be counted")
    } else {
        h |> held_fact(after > before, "the step dispatched {key} ({after - before} dispatches)")
    }
    delete s
    delete pa
    delete fa
}

//! step `st` of the widening order: the two rows batched, then each alone, then all three batched
def private widen_step(tr : Model; var ws : BatchWorkspace; var sa, sb : Session; var two, three : array<Session?>; fa, fb, fc : array<int64>; st : int64) {
    if (st == 0l) {
        var toks <- [fa[st], fb[st]]
        eval_batch(tr, ws, two, toks)
        delete toks
    } elif (st == 1l) {
        pair_step(tr, sa, sb, fa[st], fb[st])
    } else {
        var toks <- [fa[st], fb[st], fc[st - 2l]]
        eval_batch(tr, ws, three, toks)
        delete toks
    }
}

//! a row's stepped logits against its session alone, from the step it joined at
def private widen_hold(var h : Held; tr : Model; got, ref : array<array<float>>; bar : LogitsBar;
                       row : string; first : int64) {
    for (st in range64(long_length(got))) {
        hold_bar(h, tr, got[st], ref[st + 1l], ref[st], bar, "widened step {st + first}, {row}")
    }
}

//! the widening cell's collection on a three-region load: a batched step over two sessions, one of them alone, then a
//! batched step over all three - the command records at a row count it had not recorded, after a one-row step, and every
//! row still reads what its session reads alone (the N-row sets decide their own forms at the first record)
def regions_widen_rows(var h : Held; tr : Model; bar : LogitsBar = LogitsBar.bit_exact) {
    var pa <- prompt_a()
    var pb <- prompt_b()
    var pc <- prompt_c()
    var fa <- feed_a()
    var fb <- feed_b()
    var fc <- feed_c()
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    var sc <- create_session(tr, mirror_kv_dtype())
    eval(tr, sa, pa)
    eval(tr, sb, pb)
    eval(tr, sc, pc)
    var ws <- create_batch_workspace(tr)
    var two <- [unsafe(addr(sa)), unsafe(addr(sb))]
    var three <- [unsafe(addr(sa)), unsafe(addr(sb)), unsafe(addr(sc))]
    var got_a, got_b, got_c : array<array<float>>
    got_a |> reserve(STEPS)
    got_b |> reserve(STEPS)
    got_c |> reserve(STEPS - 2l)
    let steps0 = rdec_nrow_steps()
    for (st in range64(STEPS)) {
        widen_step(tr, ws, sa, sb, two, three, fa, fb, fc, st)
        if (st >= 2l) {
            got_c |> emplace <| logits_of(sc, tr.config.vocab_size)
        }
        got_a |> emplace <| logits_of(sa, tr.config.vocab_size)
        got_b |> emplace <| logits_of(sb, tr.config.vocab_size)
    }
    let stepped = rdec_nrow_steps() - steps0
    h |> held_equal(stepped, STEPS - 1l,
        "the two-row step and every three-row step went through the N-row command")
    h |> held_served()
    var ra <- logits_alone(tr, pa, fa)
    var rb <- logits_alone(tr, pb, fb)
    var fc_short <- [for (i in range64(STEPS - 2l)); fc[i]]
    var rc <- logits_alone(tr, pc, fc_short)
    widen_hold(h, tr, got_a, ra, bar, "row A", 0l)
    widen_hold(h, tr, got_b, rb, bar, "row B", 0l)
    widen_hold(h, tr, got_c, rc, bar, "row C", 2l)
    two |> clear()
    three |> clear()
    unsafe {
        delete ws
        delete two
        delete three
    }
    delete sa
    delete sb
    delete sc
    delete got_a
    delete got_b
    delete got_c
    delete ra
    delete rb
    delete rc
    delete pa
    delete pb
    delete pc
    delete fa
    delete fb
    delete fc
    delete fc_short
}

//! the groups cell's collection on a load of `rows` regions - more than the N-column leaves' eight-column tile, or more than the
//! N-row command steps at once - through STEPS batched steps: each step the fewest commands of at most the rows the command steps
//! at once (`vk_rdec_token_n_rows`), its GEMVs in tiles of eight columns, never a row at a time. The CUDA home's rows tensor leaf
//! is off for the steps (`with_rows_leaf`), so the column tiles serve every site there too, and no leaf stamp launches - the leaf
//! cell's control
def regions_groups_rows(var h : Held; tr : Model; rows : int64; bar : LogitsBar = LogitsBar.split_bar) {
    let at_once = home_token_n_rows()
    h |> held_fact(at_once >= 2l && (at_once < rows || min(rows, at_once) > int64(GEMV_COL_TILE)),
        "the command steps {at_once} rows at once against the {rows} rows: the cell measures the groups or the column tiles past {GEMV_COL_TILE}")
    var prompts <- [for (i in range64(rows)); ramp_ids(40l + i, i, 1000l + 100l * i, 37l + 2l * i, 5000l)]
    var feeds <- [for (i in range64(rows)); ramp_ids(STEPS, i, 2000l, 53l, 4000l)]
    let leaf0 = home_rows_leaf_launches()
    with_rows_leaf(false) {
        batched_steps_body(h, tr, prompts, feeds, bar)
    }
    h |> held_equal(home_rows_leaf_launches() - leaf0, 0l, "the steps launched no rows tensor leaf: the column tiles served every site")
    delete prompts
    delete feeds
}

//! the leaf cell's collection on a load of `rows` regions, the steps on the CUDA home's rows tensor leaf: a leaf stamp launched,
//! and every row nearer its session alone than its previous step (`ctrl_bar`). The leaf's sites round apart from the one-row
//! command's GEMVs, and a routed carrier's router parts the two at a near-tie: the Qwen3.6-35B IQ4_XS rows read to 1.16 of a
//! 9.9 peak on the RTX PRO 4500, 0.27 of the row one off's distance at the most, where a row one off sits as near as 0.2 of the
//! peak - no share of the peak holds both. A held skip on a home with no rows leaf
def regions_leaf_rows(var h : Held; tr : Model; rows : int64) {
    let leaf0 = home_rows_leaf_launches()
    if (leaf0 < 0l) {
        h |> held_skip("the armed home has no rows tensor leaf")
        return
    }
    var prompts <- [for (i in range64(rows)); ramp_ids(40l + i, i, 1000l + 100l * i, 37l + 2l * i, 5000l)]
    var feeds <- [for (i in range64(rows)); ramp_ids(STEPS, i, 2000l, 53l, 4000l)]
    with_rows_leaf(true) {
        batched_steps_body(h, tr, prompts, feeds, LogitsBar.ctrl_bar)
    }
    let launched = home_rows_leaf_launches() - leaf0
    h |> held_fact(launched > 0l, "the batched steps launched the rows tensor leaf ({launched} launches)")
    delete prompts
    delete feeds
}
