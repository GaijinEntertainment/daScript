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
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed; gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // set_gpu_resident_regions_, moe_gpu_resident_active
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, where the module exists
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_n_rows: the N-row command served the batched cells' rows
require ?vulkan dasllama/dasllama_vulkan_classes   // GEMV_COL_TILE: the groups cell's column tiles
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan dasllama/dasllama_vulkan_prefill   // vk_rdec_prefill_segment_windows: the device-mode tick's segmented-window witness
require _model_tier public   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // served_every_call: the shared served witness
require _compares   // the head-rows cell's draft compare
require dasllama/dasllama_ple   // ple_host_gather_rows: the side rows' host witness

// The resident driver's mirror regions (a small carrier under DASLLAMA_GPU=1): two
// sessions hold their K/V on the device at once, one region each. The instrument is the driver
// against itself - a session stepped between another session's steps reads bit for bit what it
// reads alone, because a region is an offset and the kernels are the same - so no CPU chain runs.
// The two prompts differ in content and length: a step that read the other region's rows cannot
// land on the alone-run's logits.

//! the rig's context: the mirror regions' cap and every host-cached session's K/V cache - a model max of 131072 is gigabytes of each
let REGIONS_RIG_CTX = 8192l
let private CAPPED_RIG_CTX = 4096l   //! the cap cell's context: the smallest the driver arms a capped mirror at, so a prompt reaches its end in seconds

[init]
def private regions_rig_init() {
    allow_cpu_prefill()   // the pin cell's outsider prefills on the CPU by design
    set_gpu_ctx_max(REGIONS_RIG_CTX)
}

var private g_file_kv_found : tuple<pinned : bool; dt : KVDtype>

//! the K/V mirror asked at `dt` for every load of the file that calls it from its `[init]` - a process runs one test file
def pin_file_kv(dt : KVDtype) {
    g_file_kv_found = gpu_kv_dtype_pin()
    set_gpu_kv_dtype(dt)
}

//! the codec pin `pin_file_kv` found put back, from the same file's `[finalize]`
def unpin_file_kv() {
    restore_gpu_kv_dtype(g_file_kv_found)
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

def private hold_equal(t : T?; tr : Model; got, want : array<float>; what : string) {
    log_peaks(tr, what, got, want)
    let diff = logits_maxdiff(got, want)
    t |> equal(diff, 0.0, "{what}: bit for bit the session alone (maxdiff {diff})")
}

//! `got` within `rel` of `want`'s peak, and `control` - logits one token off - outside it; under a tq4 mirror, `tq4_within`
def private hold_within(t : T?; tr : Model; got, want, control : array<float>; what : string; rel : float = SPLIT_BAR_REL) {
    log_peaks(tr, what, got, want)
    if (mirror_kv_dtype() == KVDtype.tq4) {
        tq4_within(t, got, want, control, what)
        return
    }
    let bar = row_bar(want, rel)
    let diff = row_within(t, what, got, want, bar)
    let off = control_past(t, what, got, control, bar)
    to_log(LOG_INFO, "{what}: maxdiff {diff}, the row one off {off}, bar {bar}\n")
}

//! `file` loaded at `regions` regions. A card whose room the plan does not fit registers a skip naming it, any other decline
//! is a red; either way the driver is left unarmed, which the caller reads off `moe_gpu_resident_active()` before it runs rows.
def private load_regions(t : T?; file : string; regions : int64; host_ctx : int64 = REGIONS_RIG_CTX) : Model {
    var tr <- load_resident(path_join(models_dir(), file), regions)   //! the rig measures the resident driver: its route is pinned on for the load whatever the environment says
    tr.config.seq_len = min(tr.config.seq_len, host_ctx)
    resident_admitted(t, "the whole-model driver armed at {regions} regions (the load log names any decline)", " at {regions} regions")
    return <- tr
}

def private load_two_regions(t : T?; file : string) : Model => <- load_regions(t, file, 2l)

//! the resident gate, and for a file that asks a mirror codec (`set_gpu_kv_dtype` in its `[init]`) the skip where the
//! environment arms another over it: DASLLAMA_VK_KV32=1's f32 mirror
def private regions_ready(t : T?; file : string) : bool {
    return false if (!resident_ready(t, path_join(models_dir(), file)))
    let asked = gpu_kv_dtype_pin()
    if (asked.pinned && resident_mirror_dtype() != asked.dt) {
        t |> skip("the environment arms a {resident_mirror_dtype()} mirror over the {asked.dt} this file asks (DASLLAMA_VK_KV32=1) - its cells need that unset")
        return false
    }
    return true
}

//! how a regions cell loads its carrier: the region count, the context pinned for the load, and the fuse knob off for the load
//! (the split one-row forms; a no-op without the Vulkan module)
struct private RegionsLoad {
    regions : int64 = 2l
    ctx : int64 = REGIONS_RIG_CTX
    host_ctx : int64 = REGIONS_RIG_CTX   //! every host-cached session's K/V cache: the rig's cap, or the mirror's context for a cell whose prompts run past it
    split_forms : bool
    ring : int = -1   //! the sliding layers' ring pin for the load (`set_gpu_kv_ring`): -1 the environment's, 0 every row kept
}

//! the two-region cell's rig: the gate, `file` loaded at two regions, `body` inside the engine's job queue, the model deleted after
def private with_regions_rig(t : T?; file : string; body : block<(tr : Model) : void>) {
    with_regions_rig(t, file, RegionsLoad(), body)
}

//! the rig over the load `load` names, every lever it pins put back after the load
def private with_regions_rig(t : T?; file : string; load : RegionsLoad; body : block<(tr : Model) : void>) {
    if (!regions_ready(t, file)) return
    let levers = resident_levers()
    let ring_pin = gpu_kv_ring_pin()
    split_forms_pin(load.split_forms)
    set_gpu_ctx_max(load.ctx)
    set_gpu_kv_ring(load.ring)
    var tr <- load_regions(t, file, load.regions, load.host_ctx)
    set_gpu_kv_ring(ring_pin)
    restore_resident_levers(levers)
    if (moe_gpu_resident_active()) {
        with_dasllama_jobque_() {
            invoke(body, tr)
        }
    }
    delete tr
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
def private interleaved_single_steps_body(t : T?; tr : Model; pa, pb : array<int64>) {
    var al <- alone_runs(tr, pa, pb)
    t |> success(logits_maxdiff(al.ra[0], al.rb[0]) > 0.0, "the two prompts read different logits, so a crossed region cannot pass")
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sa, pa, long_length(pa), 0l)
    hold_equal(t, tr, sa.logits, al.ra[0], "session A's prefill")
    forward_prefill(tr, sb, pb, long_length(pb), 0l)
    hold_equal(t, tr, sb.logits, al.rb[0], "session B's prefill, A's rows live beside it")
    t |> success(sa.rdec_region != sb.rdec_region, "the sessions sit in different regions ({sa.rdec_region}, {sb.rdec_region})")
    for (st in range64(STEPS)) {
        forward(tr, sa, al.fa[st], long_length(pa) + st)
        hold_equal(t, tr, sa.logits, al.ra[st + 1l], "session A step {st}")
        forward(tr, sb, al.fb[st], long_length(pb) + st)
        hold_equal(t, tr, sb.logits, al.rb[st + 1l], "session B step {st}")
    }
    served_every_call(t)
    delete sa
    delete sb
    delete al
}

def cell_regions_interleaved_single_steps(t : T?; file : string) {
    t |> run("two sessions, a region each: single steps interleaved read what each session reads alone") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var pa <- prompt_a()
            var pb <- prompt_b()
            interleaved_single_steps_body(t, tr, pa, pb)
            delete pa
            delete pb
        }
    }
}

//! two host-cached sessions over ONE region: A prefills and takes a one-row step (rows on the device alone), B prefills and takes
//! the region, then the two step in turn, each taking it back - every row bit for bit what the session reads alone, the driver's
//! saved-claim counter up (A's rows read off the device before B's prefill overwrote them) and every call served
[cold_path]
def cell_region_taken_after_decode(t : T?; file : string) {
    t |> run("two host-cached sessions over one region: the session that loses it keeps its device-only rows") @(t : T?) {
        with_regions_rig(t, file, RegionsLoad(regions = 1l)) $(tr) {
            var pa <- prompt_a()
            var pb <- prompt_b()
            var al <- alone_runs(tr, pa, pb)
            let na = long_length(pa)
            let nb = long_length(pb)
            let stashes0 = moe_gpu_resident_stashes()
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sa, pa, na, 0l)
            hold_equal(t, tr, sa.logits, al.ra[0], "session A's prefill")
            forward(tr, sa, al.fa[0], na)
            hold_equal(t, tr, sa.logits, al.ra[1], "session A step 0, its rows on the device alone")
            forward_prefill(tr, sb, pb, nb, 0l)
            hold_equal(t, tr, sb.logits, al.rb[0], "session B's prefill, taking A's region")
            t |> success(moe_gpu_resident_stashes() > stashes0, "A's rows were saved as B took the region ({moe_gpu_resident_stashes() - stashes0} saves)")
            for (st in range64(STEPS - 1l)) {
                forward(tr, sa, al.fa[st + 1l], na + st + 1l)
                hold_equal(t, tr, sa.logits, al.ra[st + 2l], "session A step {st + 1l}, the region taken back")
                forward(tr, sb, al.fb[st], nb + st)
                hold_equal(t, tr, sb.logits, al.rb[st + 1l], "session B step {st}, the region taken back")
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

//! the same past the wide position: both prompts run longer than RD_WIDE_POS, so the one-row steps take the token command's
//! wide twin - recorded on the first such step, its stamp names apart from the split form's
def cell_regions_wide_single_steps(t : T?; file : string) {
    t |> run("two sessions past the wide position: the wide twin reads what each session reads alone") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var pa <- resident_prompt(WIDE_ROWS + 8l)
            var pb <- ramp_ids(WIDE_ROWS + 25l, 0l, 3000l, 91l, 6000l)
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> success(long_length(pa) > RD_WIDE_POS && long_length(pb) > RD_WIDE_POS, "both prompts run past the wide position ({RD_WIDE_POS})")
            }
            interleaved_single_steps_body(t, tr, pa, pb)
            delete pa
            delete pb
        }
    }
}

def private prose_ids(n, salt : int64) : array<int64> => <- resident_prompt(n, salt)

//! the logits of `prompt`'s first `n - 1` tokens on a session of its own, gone before the caller makes the compared pair (two
//! regions hold two live sessions): the one-token-short control a prefill compare holds its bar against
def private short_control(tr : Model; prompt : array<int64>; n : int64) : array<float> {
    var short <- create_session(tr, mirror_kv_dtype())
    var less <- [for (i in range64(n - 1l)); prompt[i]]
    forward_prefill(tr, short, less, n - 1l, 0l)
    var out <- logits_of(short, tr.config.vocab_size)
    delete short
    delete less
    return <- out
}

//! `prompt` prefilled in one call against the same prompt prefilled as [0, cut) then [cut, n), then four fed steps on both
def private continue_at(t : T?; tr : Model; n, cut : int64; rel : float) {
    var prompt <- prose_ids(n, 0l)
    var head <- [for (i in range64(cut)); prompt[i]]
    var tail <- [for (i in range64(n - cut)); prompt[cut + i]]
    var feed <- feed_a()
    var short_logits <- short_control(tr, prompt, n)
    var whole <- create_session(tr, mirror_kv_dtype())
    var split <- create_session(tr, mirror_kv_dtype())
    let served0 = moe_gpu_resident_prefills()
    forward_prefill(tr, whole, prompt, n, 0l)
    forward_prefill(tr, split, head, cut, 0l)
    forward_prefill(tr, split, tail, n - cut, cut)
    t |> equal(moe_gpu_resident_prefills(), served0 + 3l, "{n} tokens cut at {cut}: the window chain served all three calls, the continuation among them")
    if (!empty(split.dn_state_prefix)) {
        t |> equal(split.dn_pos, n, "{n} tokens cut at {cut}: the recurrent state stands at the prompt's end after the continuation")
    }
    hold_within(t, tr, split.logits, whole.logits, short_logits, "{n} tokens cut at {cut}: the continued prefill against the one-call prefill", rel)
    delete short_logits
    for (st in range64(4l)) {
        var before <- logits_of(whole, tr.config.vocab_size)
        forward(tr, whole, feed[st], n + st)
        forward(tr, split, feed[st], n + st)
        hold_within(t, tr, split.logits, whole.logits, before, "{n} tokens cut at {cut}, step {st}: decoding over the continued rows", rel)
        delete before
    }
    delete whole
    delete split
    delete prompt
    delete head
    delete tail
    delete feed
}

//! `rel`: the bar the continued rows hold against the one-call rows - the split bar, the routed chain's on a routed model (its
//! windows' expert tiles schedule other row counts, and the router's near-ties part the two)
[cold_path]
def cell_prefill_continues_on_the_device(t : T?; file : string) {
    cell_prefill_continues_on_the_device(t, file, SPLIT_BAR_REL)
}

def cell_prefill_continues_on_the_device(t : T?; file : string; rel : float) {
    t |> run("a prefill from a position past zero runs the window chain over the region's rows") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            assert_pf_window(t)
            continue_at(t, tr, 57l, 40l, rel)     // a short tail inside the first window
            continue_at(t, tr, PF_WINDOW_ROWS + 88l, PF_WINDOW_ROWS, rel)   // the cut on the window boundary
            continue_at(t, tr, PF_WINDOW_ROWS + 388l, 300l, rel)   // the tail runs a whole window from an unaligned start, then a remainder
            continue_at(t, tr, 42l, 40l, rel)   // a tail shorter than a recurrent layer's conv taps: the ring's leading rows come from the history it continues
            served_every_call(t)
        }
    }
    t |> run("the chat shape: a prompt, decoded steps, a second turn's prompt over that history, decoded steps") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
            hold_within(t, tr, chat.logits, steps.logits, before, "the second turn through the window chain against the same tokens stepped one by one", rel)
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

//! the window's fused q/k norm, rope and K/V store against its split pair (`set_vulkan_prefill_qkn_fuse`, put back as found): one
//! prompt two windows long on each form, the prompt's logits and the fed steps within `bar` with the row one step off as the
//! control, the census counting the fused pass's stamp on the fused form and the norm pass's on the split one
[cold_path]
def cell_window_qkn_fused(t : T?; file : string; bar : LogitsBar = LogitsBar.split_bar) {
    t |> run("the window's q/k norm, rope and store as one fused pass reads within the {bar} of the split pair") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> success(tr.config.qk_norm, "the carrier norms its q and k heads")
                var prompt <- prose_ids(PF_WINDOW_ROWS + 88l, 0l)
                var feed <- feed_a()
                let lever = vulkan_prefill_qkn_fuse()
                var rows : array<array<array<float>>>
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
                t |> success(fused_counts[0] > 0l && fused_counts[1] == 0l, "the fused form's prefill ran the fused pass and no norm pass ({fused_counts[0]}, {fused_counts[1]})")
                t |> success(split_counts[0] == 0l && split_counts[1] > 0l, "the split form's prefill ran the norm pass and no fused pass ({split_counts[0]}, {split_counts[1]})")
                for (st in range(length(rows[0]))) {   //! the two passes reduce the norm in another order: the split bar, the row one step off its control
                    hold_bar(t, tr, rows[0][st], rows[1][st], rows[1][st > 0 ? st - 1 : 1], bar, st == 0 ? "the prompt's logits, fused against split" : "step {st - 1}, fused against split")
                }
                served_every_call(t)
                delete rows
                delete prompt
                delete feed
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

//! the bar a batched row holds against its session alone: bit for bit, or a bar with the one-token-off control - the split
//! bar on a K-quant dense carrier (its N-row Q8_K sites round apart from the one-row command's, `../followup_vulkan.md`
//! item 75), the wide bar on a K-quant MoE carrier (the readings each bar was set from: `../PERF_LEDGER.md`, the batched-decode
//! arc's gemma dense and MoE sections)
enum LogitsBar {
    bit_exact
    split_bar
    wide_bar
}

let private WIDE_BAR_REL = 0.10

//! a row's logits against its session alone under `bar`: bit for bit, or within the split or wide bar with `control` - the
//! row's previous step - outside it
def private hold_bar(t : T?; tr : Model; got, want, control : array<float>; bar : LogitsBar; what : string) {
    if (bar == LogitsBar.bit_exact) {
        hold_equal(t, tr, got, want, what)
    } else {
        hold_within(t, tr, got, want, control, what, bar == LogitsBar.wide_bar ? WIDE_BAR_REL : SPLIT_BAR_REL)
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

//! two sessions over `pa` and `pb`, a region each, through STEPS batched steps against each session alone, every step through the N-row command
//! the segmented windows the Vulkan driver counted so far; 0 with no vulkan module
def private segment_windows_count : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_prefill_segment_windows()
    } else {
        return 0l
    }
}

//! `bar`, or the split bar where a bit-for-bit claim cannot hold over `rows`: past the unsplit position the N-row command splits
//! a layer's keys into fewer pieces than the row alone wherever its rows' workgroups over the pieces run past one wave of the
//! card's SMs, and the two commands then sum the keys in two orders (`vk_rdec_rows_split_alike`)
def private rows_bar(bar : LogitsBar; rows, pos : int64) : LogitsBar {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (bar == LogitsBar.bit_exact && !vk_rdec_rows_split_alike(rows, pos)) {
            to_log(LOG_WARNING, "the N-row command over {rows} rows splits the keys apart from the row alone at {pos} positions on this card: the batched rows hold the split bar, not bit for bit\n")
            return LogitsBar.split_bar
        }
    }
    return bar
}

def private batched_steps_body(t : T?; tr : Model; prompts, feeds : array<array<int64>>; bar0 : LogitsBar) {
    let rows = long_length(prompts)
    var furthest = 0l
    for (p in prompts) {
        furthest = max(furthest, long_length(p))
    }
    let bar = rows_bar(bar0, rows, furthest + long_length(feeds[0]))
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> success(vk_rdec_rows_split_alike(1l, furthest + long_length(feeds[0])), "one row splits a layer's keys as one row does")
    }
    let steps_alone0 = rdec_nrow_steps()
    var alone <- [for (i in range64(rows)); logits_alone(tr, prompts[i], feeds[i])]
    t |> equal(rdec_nrow_steps() - steps_alone0, 0l, "the sessions alone stepped through the one-row command: the N-row counter's control")
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
                hold_bar(t, tr, sessions[i].logits, alone[i][st + 1l], alone[i][st], bar, "batched step {st}, row {i}")
            }
        }
    }
    if (hybrid) {
        t |> success(dn_state_home(sessions[rows - 1l]) != home0, "the last session's recurrent state came home from its region's slot after the batched steps")
    }
    for (i in range64(rows)) {
        t |> equal(sessions[i].rdec_gen, gens[i], "row {i} kept its claim through every step: its history never came up from the host again")
    }
    served_every_call(t)
    let groups = batched_groups(t, rows)
    let stepped = rdec_nrow_steps() - steps0
    t |> equal(stepped, STEPS * groups, "every batched step went through the N-row command, {groups} command(s) a step ({stepped} of {STEPS * groups})")
    unsafe {
        delete sess
    }
    delete sessions
    delete gens
    delete alone
}

//! the commands a batched step of `rows` rows takes: the fewest groups of the rows the N-row command steps at once, the command
//! asserted to serve the rows at once (a declined command passes the bars above the same); one where the module is absent
def private batched_groups(t : T?; rows : int64) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let at_once = vk_rdec_token_n_rows()
        t |> success(at_once >= 2l, "the N-row token command serves the rows at once ({at_once} rows)")
        return at_once > 0l ? (rows + at_once - 1l) / at_once : 1l
    } else {
        return 1l
    }
}

//! the two-prompt form over `pa` and `pb` with the rig's feeds
def private batched_steps_body(t : T?; tr : Model; pa, pb : array<int64>; bar : LogitsBar) {
    var prompts <- [ <- clone_to_move(pa), <- clone_to_move(pb)]
    var feeds <- [ <- feed_a(), <- feed_b()]
    batched_steps_body(t, tr, prompts, feeds, bar)
    delete prompts
    delete feeds
}

//! the E-series side rows on the device at every decode shape: the sessions alone (the one-row command) and the batched steps (the
//! N-row command) gather each row's side row off its token id - the host gathers no table row - and the batched rows still read bit for bit what each session reads alone
def cell_regions_batched_side_rows(t : T?; file : string) {
    t |> run("the side rows of the sessions alone and of the batched steps gather on the device off the rows' ids") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> success(has_ple(tr.config) && vk_rdec_side_gather_on(), "the driver gathers the side rows off the ids (the table and the projection on the device)")
                var pa <- prompt_a()
                var pb <- prompt_b()
                let host0 = ple_host_gather_rows()
                let dev0 = vk_rdec_nrow_side_gathered()
                batched_steps_body(t, tr, pa, pb, LogitsBar.bit_exact)
                t |> equal(ple_host_gather_rows() - host0, 0l, "the host gathered no table row")
                t |> equal(vk_rdec_nrow_side_gathered() - dev0, 2l * STEPS, "every batched row's side row gathered off its id")
                delete pa
                delete pb
            }
        }
    }
}

//! two sessions over prompts A and B batched through STEPS steps: every row's logits a step, the N-row command serving every step
def private batched_rows_logits(t : T?; tr : Model) : array<array<float>> {
    var prompts <- [ <- prompt_a(), <- prompt_b()]
    var feeds <- [ <- feed_a(), <- feed_b()]
    var sessions : array<Session>
    sessions |> reserve(2)
    for (p in prompts) {
        sessions |> emplace <| create_session(tr, mirror_kv_dtype())
        eval(tr, sessions[length(sessions) - 1], p)
    }
    var sess <- [for (i in range(2)); unsafe(addr(sessions[i]))]
    var rows : array<array<float>>
    let steps0 = rdec_nrow_steps()
    with_batch_rows(tr, sess) $(var ws; var sx) {
        for (st in range64(STEPS)) {
            var toks <- [for (i in range(2)); feeds[i][st]]
            eval_batch(tr, ws, sx, toks)
            delete toks
            for (s in sessions) {
                rows |> push_clone(s.logits)
            }
        }
    }
    t |> equal(rdec_nrow_steps() - steps0, STEPS, "every batched step went through the N-row command")
    served_every_call(t)
    unsafe {
        delete sess
    }
    delete sessions
    delete prompts
    delete feeds
    return <- rows
}

//! the batched rows' token rows gathered on the device off their ids (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`): every
//! batched row counted by `vk_rdec_nrow_embed_gathered`, the host embedding none, and each row's logits at every step bit for bit the
//! same steps' on a load with the gather off (`set_gpu_resident_embed(0)`, the `DASLLAMA_VK_GPU_EMBED=0` form: every row embedded on
//! the host and staged up), which moves the counter by nothing and embeds every row on the host - the counters' control
def cell_regions_batched_embed_gather(t : T?; file : string) {
    t |> run("the batched rows gather their token rows on the device, bit for bit the rows the host embeds") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let pin = gpu_resident_embed_pin()
            var dev, host : array<array<float>>
            set_gpu_resident_embed(1)
            with_regions_rig(t, file) $(tr) {
                let g0 = vk_rdec_nrow_embed_gathered()
                let h0 = batch_rows_host_embedded()
                delete dev
                dev <- batched_rows_logits(t, tr)
                t |> equal(vk_rdec_nrow_embed_gathered() - g0, 2l * STEPS, "every batched row's token row gathered off its id")
                t |> equal(batch_rows_host_embedded() - h0, 0l, "the host embedded no batched row")
            }
            set_gpu_resident_embed(0)
            with_regions_rig(t, file) $(tr) {
                let g0 = vk_rdec_nrow_embed_gathered()
                let h0 = batch_rows_host_embedded()
                delete host
                host <- batched_rows_logits(t, tr)
                t |> equal(vk_rdec_nrow_embed_gathered() - g0, 0l, "with the gather off no batched row gathered: the counter's control")
                t |> equal(batch_rows_host_embedded() - h0, 2l * STEPS, "with the gather off the host embedded every batched row")
            }
            set_gpu_resident_embed(pin)
            t |> equal(long_length(dev), long_length(host), "both loads stepped every row")
            for (i in range64(min(long_length(dev), long_length(host)))) {
                let diff = logits_maxdiff(dev[i], host[i])
                t |> equal(diff, 0.0, "step {i / 2l}, row {i % 2l}: the gathered rows' logits bit for bit the host rows' (maxdiff {diff})")
            }
            delete dev
            delete host
        }
    }
}

def cell_regions_batched_steps(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact; split_forms : bool = false) {
    t |> run("two sessions, a region each: a batched step over both reads what each session reads alone, with no history resync") @(t : T?) {
        with_regions_rig(t, file, RegionsLoad(split_forms = split_forms)) $(tr) {
            var pa <- prompt_a()
            var pb <- prompt_b()
            batched_steps_body(t, tr, pa, pb, bar)
            delete pa
            delete pb
        }
    }
}

//! a long-context file's shape: the regions and the context the carrier loads at (every host-cached session's cache sized to
//! that context), the prompt the CPU chain answers and its bar, the continuation's length and cut (a window multiple), and
//! the bar the batched rows hold against each session alone; the streams cell runs `regions` device-home streams at
//! ctx x (28 + 18 i) / 100 positions
struct LongCtx {
    regions : int64 = 2l
    ctx : int64
    cpu_n : int64 = 12288l
    cpu_bar_rel : float = 0.10
    split_n : int64
    split_cut : int64
    bar : LogitsBar = LogitsBar.bit_exact
    streams_bar : LogitsBar = LogitsBar.bit_exact   //! the streams cell's: past the wide position the one-row command's wide twin and the N-row command's split pieces sum the keys in another order, so a carrier bit for bit at short prompts holds the split bar there
    streams_rel : float = 0.0   //! > 0: the streams cell's bar, as a share of the max logit, in place of `streams_bar`'s - a carrier whose reading the file states
}

def private long_load(lc : LongCtx; ring : int = -1) : RegionsLoad => RegionsLoad(regions = lc.regions, ctx = lc.ctx, host_ctx = lc.ctx, ring = ring)

//! a long cell's mirror on a sliding model rings its sliding layers: the plan charges the rings, not the whole context, on every layer
def private rings_armed(t : T?; tr : Model) {
    return if (tr.config.sliding_window == 0l)
    t |> success(gpu_resident_kv_ring_slack() > 0l, "the sliding layers ride rings ({gpu_resident_mirror_bytes() >> 20l} MB of mirror over {gpu_resident_mirror_slabs()} slab pairs)")
}

//! the rows-through-host cells' mirror: in slabs on a flat model, the rings on a sliding one
def private long_mirror_held(t : T?; tr : Model) {
    if (tr.config.sliding_window > 0l) {
        rings_armed(t, tr)
    } else {
        t |> success(gpu_resident_mirror_slabs() > 1l, "the mirror binds more than one slab pair ({gpu_resident_mirror_slabs()})")
    }
}

def private stream_positions(lc : LongCtx) : array<int64> => <- [for (i in range64(lc.regions)); lc.ctx * (28l + 18l * i) / 100l]

//! the mirror in slabs: `regions` regions at `ctx` positions each ask more of the layers' rows than one binding holds, so the
//! plan seats the layers in two or more slab pairs; the interleaved single steps and the batched steps over two sessions
//! then read what each session reads alone - every layer's sets, sync and readback bound the slab its rows sit in
[cold_path]
def cell_regions_slabbed_mirror(t : T?; file : string; lc : LongCtx) {
    t |> run("a mirror of {lc.regions} regions x {lc.ctx} positions, every row kept, binds in slabs, and the steps over two sessions read what each reads alone") @(t : T?) {
        with_regions_rig(t, file, long_load(lc, 0)) $(tr) {
            let slabs = gpu_resident_mirror_slabs()
            t |> success(slabs > 1l, "the mirror binds more than one slab pair at {lc.regions} regions x {lc.ctx} positions ({slabs} slabs)")
            t |> equal(gpu_device_sessions(), lc.regions, "the armed driver serves a device-home session per region asked")
            t |> equal(gpu_device_session_ctx(), lc.ctx, "a region holds the whole context asked - nothing shortened to one binding's range")
            var pa <- prompt_a()
            var pb <- prompt_b()
            let steps0 = rdec_nrow_steps()
            interleaved_single_steps_body(t, tr, pa, pb)
            t |> equal(rdec_nrow_steps(), steps0, "the one-row steps moved no N-row count: the batched cell's counter control")
            batched_steps_body(t, tr, pa, pb, lc.bar)
            delete pa
            delete pb
        }
    }
}

//! a long prompt against the CPU chain: `cpu_n` prose tokens prefilled on the device and eight fed steps, every row within
//! the bar of the CPU chain's row with the one-step-off control - the rows deep in a region read as the host's do
[cold_path]
def cell_long_prompt_against_cpu(t : T?; file : string; lc : LongCtx) {
    t |> run("a {lc.cpu_n}-token prompt on a {lc.regions} x {lc.ctx} mirror: the resident rows against the CPU chain at every step") @(t : T?) {
        with_regions_rig(t, file, long_load(lc)) $(tr) {
            t |> equal(gpu_device_session_ctx(), lc.ctx, "a region holds the whole context asked")
            rings_armed(t, tr)
            forced_feed_over(t, tr, "long prompt: ", lc.cpu_n, STEPS, lc.cpu_bar_rel, 3l) $ {
                served_every_call(t)
            }
        }
    }
}

//! a long prompt continued on the device: `split_n` tokens in one call against the same cut at `split_cut`, then four steps
//! over the continued rows, within the split bar with the one-token-off control
[cold_path]
def cell_long_continuation(t : T?; file : string; lc : LongCtx) {
    t |> run("a {lc.split_n}-token prompt cut at {lc.split_cut}: the continued prefill and the steps over it against the one-call prefill") @(t : T?) {
        with_regions_rig(t, file, long_load(lc)) $(tr) {
            assert_pf_window(t)
            rings_armed(t, tr)
            continue_at(t, tr, lc.split_n, lc.split_cut, SPLIT_BAR_REL)
            served_every_call(t)
        }
    }
}

//! the rows that cross the host at a slab past the first: a session's rows brought down after a device prefill (`rows_n`
//! tokens, every layer's slab read back) carry eight CPU steps within the bar of the CPU chain's, and a session prefilled
//! on the CPU rails under the server's prefill pin (its rows uploaded to every layer's slab) carries eight resident steps
//! within the same bar - a layer seated past slab 0 synced or read through slab 0 lands its keys in another layer's rows
[cold_path]
def cell_long_rows_through_host(t : T?; file : string; lc : LongCtx) {
    let rows_n = lc.cpu_n / 3l
    t |> run("{rows_n} tokens prefilled on the device, the rows brought down: the CPU chain's steps over them against the CPU chain alone") @(t : T?) {
        with_regions_rig(t, file, long_load(lc)) $(tr) {
            long_mirror_held(t, tr)
            var prompt <- prose_ids(rows_n, 5l)
            var toks <- feed_a()
            var s <- create_session(tr, mirror_kv_dtype())
            let served0 = moe_gpu_resident_prefills()
            forward_prefill(tr, s, prompt, rows_n, 0l)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the window chain served the prompt")
            moe_gpu_hydrate_session(tr, s)
            t |> success(s.rdec_host_current, "every layer's rows came down to the host")
            moe_gpu_drop_model()
            var down : array<array<float>>
            down |> emplace <| logits_of(s, tr.config.vocab_size)
            for (st in range64(STEPS)) {
                forward(tr, s, toks[st], rows_n + st)
                down |> emplace <| logits_of(s, tr.config.vocab_size)
            }
            var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
            feed_steps_within(t, tr, "rows down: ", rows_n, down, cpu, toks, lc.cpu_bar_rel, "the CPU chain over the rows brought down")
            delete s
            delete down
            delete cpu
            delete prompt
            delete toks
        }
    }
    t |> run("{rows_n} tokens prefilled on the CPU rails under the prefill pin, the rows uploaded: the resident steps over them against the CPU chain alone") @(t : T?) {
        with_regions_rig(t, file, long_load(lc)) $(tr) {
            long_mirror_held(t, tr)
            var prompt <- prose_ids(rows_n, 5l)
            var toks <- feed_a()
            reset_gpu_cpu_passes()
            set_resident_prefill_allowed(false)
            var sg <- create_session(tr, mirror_kv_dtype())
            var up <- feed_logits(tr, sg, prompt, toks)
            set_resident_prefill_allowed(true)
            only_pass(t, "pinned_off", "the prompt passed to the CPU rails under the server's prefill pin and nothing else did", 1l)
            delete sg
            moe_gpu_drop_model()
            var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
            feed_steps_within(t, tr, "rows up: ", rows_n, up, cpu, toks, lc.cpu_bar_rel, "resident over the rows uploaded")
            delete up
            delete cpu
            delete prompt
            delete toks
        }
    }
}

//! `regions` device-home streams at `stream_positions`, a region each: every prefill bit for bit the host-cached session's,
//! then a batched step over every row at every step reads what each session reads alone under the bar
[cold_path]
def cell_long_streams(t : T?; file : string; lc : LongCtx) {
    t |> run("{lc.regions} device-home streams at {lc.ctx * 28l / 100l} to {lc.ctx * (28l + 18l * (lc.regions - 1l)) / 100l} positions: a batched step over all of them reads what each reads alone") @(t : T?) {
        with_regions_rig(t, file, long_load(lc)) $(tr) {
            rings_armed(t, tr)
            let n = lc.regions
            var at <- stream_positions(lc)
            t |> equal(gpu_device_sessions(), n, "the armed driver serves a device-home session per region")
            var prompts, feeds : array<array<int64>>
            var alone : array<array<array<float>>>
            let steps0 = rdec_nrow_steps()
            for (i in range64(n)) {
                prompts |> emplace <| prose_ids(at[i], 11l * (i + 1l))
                feeds |> emplace <| ramp_ids(STEPS, i, 2000l + 100l * i, 53l, 4000l)
                alone |> emplace <| feed_alone(tr, mirror_kv_dtype(), prompts[i], feeds[i])
            }
            t |> equal(rdec_nrow_steps(), steps0, "the one-row steps alone moved no N-row count: the batched counter's control")
            var owned : array<Session>
            owned |> reserve(n)
            for (_i in range64(n)) {
                owned |> emplace <| device_session(tr)
            }
            for (i in range64(n)) {
                eval(tr, owned[i], prompts[i])
                hold_equal(t, tr, owned[i].logits, alone[i][0], "stream {i}'s {at[i]}-token prefill on the device")
            }
            var sess <- [for (i in range64(n)); unsafe(addr(owned[i]))]
            let streams_bar = rows_bar(lc.streams_bar, n, at[n - 1l] + STEPS)
            with_batch_rows(tr, sess) $(var ws; var rows) {
                for (st in range64(STEPS)) {
                    var toks <- [for (i in range64(n)); feeds[i][st]]
                    eval_batch(tr, ws, rows, toks)
                    delete toks
                    for (i in range64(n)) {
                        let what = "batched step {st}, stream {i} at {at[i] + st} positions"
                        if (lc.streams_rel > 0.0) {
                            hold_within(t, tr, owned[i].logits, alone[i][st + 1l], alone[i][st], what, lc.streams_rel)
                        } else {
                            hold_bar(t, tr, owned[i].logits, alone[i][st + 1l], alone[i][st], streams_bar, what)
                        }
                    }
                }
            }
            unsafe {
                delete sess
            }
            t |> equal(rdec_nrow_steps() - steps0, STEPS, "every batched step went through the N-row command")
            static_if (typeinfo builtin_module_exists(vulkan)) {
                let nrows = vk_rdec_token_n_rows()
                t |> success(nrows >= n, "the N-row token command serves the {n} rows at once ({nrows} rows)")
            }
            served_every_call(t)
            for (i in range64(n)) {
                gpu_device_kv_park(owned[i])
            }
            delete owned
            delete prompts
            delete feeds
            delete alone
            delete at
        }
    }
}

//! the same past the unsplit position: both prompts run longer than RD_UNSPLIT_POS, so the batched steps take the
//! N-row command's split attention form - the arrival counters a row apart, the last piece of each row combining
def cell_regions_batched_steps_split(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact) {
    t |> run("two sessions past the unsplit position: the split N-row command reads what each session reads alone") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var pa <- resident_prompt(PF_WINDOW_ROWS + 8l)
            var pb <- ramp_ids(PF_WINDOW_ROWS + 25l, 0l, 3000l, 91l, 6000l)
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> success(long_length(pa) > RD_UNSPLIT_POS && long_length(pb) > RD_UNSPLIT_POS, "both prompts run past the unsplit position ({RD_UNSPLIT_POS})")
            }
            batched_steps_body(t, tr, pa, pb, bar)
            delete pa
            delete pb
        }
    }
}

//! DASLLAMA_VK_FUSE_BISECT a bit at a time: the one-row steps under each bit within the split bar of the fused run (the fused
//! and split forms round apart), the one-token-off control outside it; under the fused q/k norm's bit the batched rows
//! hold bit for bit to the row alone on the split pair
def cell_regions_fuse_bisect(t : T?; file : string) {
    t |> run("the fuse bisect's every bit serves logits within the split bar of the fused run; the split-pair rows batch bit for bit") @(t : T?) {
        if (!regions_ready(t, file)) return
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var pa <- prompt_a()
            var fa <- feed_a()
            with_dasllama_jobque_() {
                vk_fuse_bisect_pin(0l)
                var tr0 <- load_two_regions(t, file)
                if (!moe_gpu_resident_active()) {
                    delete tr0
                    vk_fuse_bisect_pin(-1l)
                    return
                }
                var base <- logits_alone(tr0, pa, fa)
                delete tr0
                for (bit in fixed_array(1l, 2l, 4l, 8l, 16l, 32l, 64l)) {
                    vk_fuse_bisect_pin(bit)
                    var tr <- load_two_regions(t, file)
                    var got <- logits_alone(tr, pa, fa)
                    for (st in range64(STEPS)) {
                        hold_within(t, tr, got[st + 1l], base[st + 1l], base[st], "bisect bit {bit} step {st}")
                    }
                    delete got
                    if (bit == 64l) {
                        var pb <- prompt_b()
                        batched_steps_body(t, tr, pa, pb, LogitsBar.bit_exact)
                        delete pb
                    }
                    delete tr
                }
                vk_fuse_bisect_pin(-1l)
                delete base
            }
            delete pa
            delete fa
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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
//! position, so the batched steps take the unsplit twin whether or not a one-row record came first
def cell_regions_batched_first(t : T?; file : string) {
    t |> run("a batched step first, one-row steps, a batched step again: the rows read what each session reads alone throughout") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
            served_every_call(t)
            var ra <- logits_alone(tr, pa, fa)
            var rb <- logits_alone(tr, pb, fb)
            for (st in range64(STEPS)) {
                hold_equal(t, tr, got_a[st], ra[st + 1l], "batched-first step {st}, row A")
                hold_equal(t, tr, got_b[st], rb[st + 1l], "batched-first step {st}, row B")
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
    }
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

def cell_device_home_sessions(t : T?; file : string) {
    t |> run("two device-home sessions step between each other and in one batched step, bit for bit a host-cached session alone") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            t |> equal(gpu_device_sessions(), 2l, "the armed driver serves a device-home session per region")
            var pa <- prompt_a()
            var pb <- prompt_b()
            var al <- alone_runs(tr, pa, pb)
            set_resident_prefill_allowed(false)   // the server's setting: the pin binds host-cached sessions, a device-home one prefills on the device all the same
            with_device_pair(tr, pa, pb) $(var sa; var sb) {
                t |> equal(length(sa.key_cache) + length(sa.value_cache), 0, "a device-home session allocates no host cache")
                hold_equal(t, tr, sa.logits, al.ra[0], "device-home A's prefill")
                hold_equal(t, tr, sb.logits, al.rb[0], "device-home B's prefill")
                let half = STEPS / 2l
                for (st in range64(half)) {
                    var ta <- [al.fa[st]]
                    var tb <- [al.fb[st]]
                    eval(tr, sa, ta)
                    hold_equal(t, tr, sa.logits, al.ra[st + 1l], "device-home A single step {st}")
                    eval(tr, sb, tb)
                    hold_equal(t, tr, sb.logits, al.rb[st + 1l], "device-home B single step {st}")
                    delete ta
                    delete tb
                }
                moe_gpu_hydrate_session(tr, sa)
                t |> equal(length(sa.key_cache) + length(sa.value_cache), 0, "hydrating a device-home session brings nothing down: it has no host cache to fill")
                with_batch_rows(tr, sa, sb) $(var ws; var sess) {
                    for (st in range64(half, STEPS)) {
                        var toks <- [al.fa[st], al.fb[st]]
                        eval_batch(tr, ws, sess, toks)
                        delete toks
                        hold_equal(t, tr, sa.logits, al.ra[st + 1l], "device-home batched step {st}, row A")
                        hold_equal(t, tr, sb.logits, al.rb[st + 1l], "device-home batched step {st}, row B")
                    }
                }
                set_resident_prefill_allowed(true)
                var passes <- gpu_cpu_passes_()
                t |> equal(length(passes), 0, "the device served every call, the prefills under the pin among them: {passes}")
                delete passes
            }
            delete al
            delete pa
            delete pb
        }
    }
    cell_device_home_pin(t, file)
}

def private cell_device_home_pin(t : T?; file : string) {
    t |> run("a live device-home session's region is never handed to another session: the outsider passes, the owners step on") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var pa <- prompt_a()
            var pb <- prompt_b()
            var al <- alone_runs(tr, pa, pb)
            with_device_pair(tr, pa, pb) $(var sa; var sb) {
                var outsider <- create_session(tr, mirror_kv_dtype())
                eval(tr, outsider, pa)
                only_pass(t, "busy", "the outsider's call passed as busy, and only it")
                var ta <- [al.fa[0]]
                var tb <- [al.fb[0]]
                eval(tr, sa, ta)
                hold_equal(t, tr, sa.logits, al.ra[1], "owner A's next step, its region untouched")
                eval(tr, sb, tb)
                hold_equal(t, tr, sb.logits, al.rb[1], "owner B's next step, its region untouched")
                gpu_device_kv_park(sa)
                var late <- create_session(tr, mirror_kv_dtype())
                let served0 = moe_gpu_resident_prefills()
                eval(tr, late, pa)
                t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "a parked region is anyone's: the next outsider prefills on the device")
                delete ta
                delete tb
                delete late
                delete outsider
            }
            delete al
            delete pa
            delete pb
        }
    }
}

def cell_device_kv_park_adopt(t : T?; file : string) {
    t |> run("a parked claim hands its rows to a later session once: the adopter prefills only the rest") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
            let served0 = moe_gpu_resident_prefills()
            t |> success(!gpu_device_kv_adopt(second, claim, 999l) && second.n_past == 0l, "a claim over 57 rows refuses 999 of them, and the adopter stays empty")
            t |> success(gpu_device_kv_adopt(second, claim, 40l), "the claim stands: the adopter takes 40 of its 57 rows")
            t |> equal(second.n_past, 40l, "the adopter continues from the adopted rows")
            eval(tr, second, tail)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "one device prefill: the seventeen tokens past the adopted rows")
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
[cold_path]
def cell_device_kv_park_adopt_recurrent(t : T?; file : string) {
    t |> run("a recurrent parked claim hands its rows and its state to a session that takes every row, and refuses a shorter opening") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
            let served0 = moe_gpu_resident_prefills()
            t |> success(gpu_device_kv_adopt(second, claim, 57l), "the claim hands all 57 rows over")
            t |> equal(second.dn_pos, 57l, "the adopter's recurrent state stands where the parked stream's did")
            eval(tr, second, tail)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "one device prefill: the seventeen tokens past the adopted rows")
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

//! requests `first` up to `last` of two runs side by side: eight tokens each on the
//! `got` side, token for token the `want` side's for every id below `same_below`
def private hold_streams_equal(t : T?; m : Model; want, got : array<SchedEvent>;
                               first, last, same_below : int64; want_name, got_name : string) {
    for (id in range64(first, last)) {
        var tw <- toks_of(want, id)
        var tg <- toks_of(got, id)
        log_gen_texts(m, "request {id} (CPU = {want_name}, GPU = {got_name})", tw, tg)
        t |> equal(length(tg), 8, "request {id} ran its eight tokens")
        if (id < same_below) {
            t |> success(ids_equal(tw, tg),
                         "request {id}: {got_name} {tg} against {want_name} {tw}")
        }
        delete tw
        delete tg
    }
}

def cell_scheduler_device_mode(t : T?; file : string) {
    t |> run("a device-mode scheduler emits what the host-cached one does where the two run one code path, and a second turn adopts its first turn's rows") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var host, dev : array<SchedEvent>
            let served0 = moe_gpu_resident_prefills()
            t |> success(!serve_four(tr, false, host).on, "the control scheduler serves host-cached sessions")
            let served1 = moe_gpu_resident_prefills()
            let picks0 = rdec_picks_served()
            let segw0 = segment_windows_count()
            let served = serve_four(tr, true, dev)
            t |> success(served.on, "an idle scheduler switches to device-home sessions")
            if (gpu_device_prefill_segments(tr)) {   //! the two prompts admitted together ride one segmented window: the tick's witness
                t |> success(segment_windows_count() - segw0 >= 1l, "the device-home tick handed its prefilling streams to the segmented window ({segment_windows_count() - segw0} windows)")
            } else {
                t |> equal(segment_windows_count() - segw0, 0l, "a model the segmented window declines takes a stream's own call a tick")
            }
            //! every decode step of the three argmax requests lands a pick (their first token samples off the prefill's logits inline);
            //! the sampled fourth request's steps land logits, so the count is the rows stepped less its own
            var td4 <- toks_of(dev, 4l)
            let sampled_steps = long_length(td4) - 1l
            delete td4
            t |> equal(rdec_picks_served() - picks0, served.rows_stepped - sampled_steps, "the device landed the argmax streams' picks alone, no logits row ({served.rows_stepped} rows stepped, {sampled_steps} of them the sampled request's)")
            t |> success(served.rows_stepped - sampled_steps >= 21l, "the three argmax requests stepped seven decode rows each at least ({served.rows_stepped - sampled_steps})")
            // four turns on two stream slots: the first turn's claim is evicted by the third's, the third's is spent by the turn that adopts it
            t |> equal(served.parked, gpu_device_prefill_continues(tr) ? 2 : 0, "parked claims stay within the stream slots, and a model whose prefill cannot continue parks none")
            // one code path on both sides: each prompt enters the window chain once and each step the batched token command;
            // a device-home stream differs by the host copy it skips. The adopting turn splits its prefill, so it holds no token compare
            t |> equal(served1 - served0, 4l, "the host-cached side prefilled its four prompts on the device, one call each")
            t |> equal(moe_gpu_resident_prefills() - served1, 4l, "and so did the device-home side")
            let same_below = gpu_device_prefill_continues(tr) ? 4l : 5l
            hold_streams_equal(t, tr, host, dev, 1l, 5l, same_below, "host-cached", "device-home")
            t |> equal(cached_of(host, 4l), 0l, "the host-cached flat scheduler keeps no prefix")
            if (gpu_device_prefill_continues(tr)) {
                t |> success(cached_of(dev, 4l) >= 33l, "the second turn adopted the rows of the turn it continues from the device ({cached_of(dev, 4l)} of them)")
            } else {
                t |> equal(cached_of(dev, 4l), 0l, "a recurrent model's turn prefills whole, from zero: nothing is adopted")
            }
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call of both runs: {passes}")
            delete passes
            delete host
            delete dev
        }
    }
}

//! a paged scheduler's host-cached streams on the resident driver: their pages go up a physical run at a time, the rows the device
//! stepped come down into the pages before the prefix cache takes them, and a second turn attaches the first turn's pages
[cold_path]
def cell_scheduler_paged_resident(t : T?; file : string) {
    t |> run("a paged scheduler's streams step on the device: token for token the flat scheduler's, and a second turn attaches the turn it continues") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var flat, paged : array<SchedEvent>
            serve_four(tr, false, flat)
            let served0 = moe_gpu_resident_prefills()
            let steps0 = rdec_picks_served()
            serve_four(tr, false, paged, 64l)
            t |> success(moe_gpu_resident_prefills() - served0 >= 4l, "the paged side's prompts entered the window chain ({moe_gpu_resident_prefills() - served0} calls)")
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
[cold_path]
def cell_device_checkpoint_adopt(t : T?; file : string) {
    t |> run("a device-home recurrent stream's checkpoint at the system prompt's end serves the next request on that system prompt, token for token the host-cached prefix cache's run") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
[cold_path]
def cell_device_pool(t : T?; file : string) {
    t |> run("a device-home scheduler's openings outlive their regions in the host pool: a later request takes them up, token for token the host-cached prefix cache's run") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
[cold_path]
def cell_device_pool_mtp(t : T?; file : string) {
    t |> run("a speculating device-home scheduler keeps its openings in the host pool: the draft head's rows ride the moves, and a later request drafts off them as the host-cached speculating scheduler does") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
[cold_path]
def cell_device_pool_head_rows(t : T?; file : string) {
    t |> run("an opening down to the pool and up again carries the draft head's rows: the fresh session counts them and drafts what the session that went down drafts") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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

def cell_scheduler_device_prefill_chunks(t : T?; file : string) {
    t |> run("a device-home stream prefills in the device's windows, and a model whose prefill cannot continue in one call") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var sch <- create_scheduler(tr, 2l, 4l, 64l, 0l, 0l, mirror_kv_dtype())
            t |> success(set_device_kv(sch, true), "an idle scheduler switches to device-home sessions")
            var long_prompt <- prose_ids(PF_WINDOW_ROWS + 188l, 3l)
            var events : array<SchedEvent>
            let served0 = moe_gpu_resident_prefills()
            submit(tr, sch, make_req(1l, long_prompt, 2l))
            drain(tr, sch, events)
            let calls = moe_gpu_resident_prefills() - served0
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
def cell_scheduler_device_handoff(t : T?; file : string) {
    t |> run("a device-home tick hands two streams to one segmented window where the model's window serves, and takes one stream's call where it declines") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var sch <- create_scheduler(tr, 2l, 4l, 64l, 0l, 0l, mirror_kv_dtype())
            t |> success(set_device_kv(sch, true), "an idle scheduler switches to device-home sessions")
            var pa <- prose_ids(300l, 5l)
            var pb <- prose_ids(2l * PF_WINDOW_ROWS, 9l)
            var events : array<SchedEvent>
            let served0 = moe_gpu_resident_prefills()
            let segw0 = segment_windows_count()
            submit(tr, sch, make_req(1l, pa, 2l))
            submit(tr, sch, make_req(2l, pb, 2l))
            drain(tr, sch, events)
            let calls = moe_gpu_resident_prefills() - served0
            if (gpu_device_prefill_segments(tr)) {
                t |> equal(calls, 4l, "the first tick's window carried both streams and cut the second's prompt there ({calls} calls)")
                t |> success(segment_windows_count() - segw0 >= 1l, "... through the segmented window")
            } else {
                let whole = gpu_device_prefill_continues(tr) ? 2l : 1l
                t |> equal(calls, 1l + whole, "each tick took one stream's own call, the second stream's prompt whole windows or whole where the model's prefill cannot continue ({calls} calls)")
                t |> equal(segment_windows_count() - segw0, 0l, "... no segmented window")
            }
            t |> equal(length(toks_of(events, 1l)), 2, "the first stream ran its two tokens")
            t |> equal(length(toks_of(events, 2l)), 2, "the second stream ran its two tokens")
            served_every_call(t)
            set_device_kv(sch, false)
            delete events
            delete pa
            delete pb
            delete sch
        }
    }
}

def cell_device_home_guards(t : T?; file : string) {
    t |> run("a device-home session that finds no region panics with the reason, and the owners step on") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
                hold_equal(t, tr, sa.logits, ra[1], "owner A's next step after the refused session")
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

def cell_driver_reports(t : T?; file : string) {
    t |> run("every reason a call can pass to the CPU has words of its own, the plan sizes every region, and the status counts the mirrors") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
            set_gpu_resident_regions_(1l)
            let one = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(4l)
            let four = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(0l)
            let zero = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(2l)
            let two = resident_plan(tr, cap, kv, kv)
            set_gpu_resident_regions_(1l)
            t |> success(one.kv_bytes > 0l && four.kv_bytes == 4l * one.kv_bytes, "four regions plan four times one region's K/V ({four.kv_bytes} against {one.kv_bytes})")
            t |> equal(zero.kv_bytes, one.kv_bytes, "a region count under one plans one region")
            let held = gpu_tier_status_().vram_bytes
            t |> success(held >= two.kv_bytes, "the status counts both mirror sides of the two regions this load armed ({held} against {two.kv_bytes})")
            let weights_held = held - two.kv_bytes
            t |> success(abs(weights_held - two.weight_bytes) * 50l <= two.weight_bytes, "the plan's weight tally is the upload's within 2% ({two.weight_bytes} planned against {weights_held} held): a plane tallied twice or never lands here")
        }
    }
    t |> run("a load under a switched-off whole-model route says it was never attempted") @(t : T?) {
        if (!regions_ready(t, file)) return
        let levers = resident_levers()
        set_gpu_resident_route(false)
        var tr <- load_model_(path_join(models_dir(), file), QuantMode.q8)
        restore_resident_levers(levers)
        t |> success(!moe_gpu_resident_active(), "the whole-model driver did not arm")
        t |> success(gpu_resident_decline_() |> starts_with("not attempted"), "and the reason says so: {gpu_resident_decline_()}")
        delete tr
    }
}

def cell_passes_with_their_rows(t : T?; file : string) {
    t |> run("a prefill that skips positions passes as a gap, its device rows brought down first") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
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
    t |> run("a prefill past the region's context passes as cap, its device rows brought down first") @(t : T?) {
        with_regions_rig(t, file, RegionsLoad(ctx = CAPPED_RIG_CTX)) $(tr) {
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

//! the census key of the decode attention stamp the armed mirror codec and `tr`'s head shape pick, its slab asserted
//! `slab_heads` q heads wide ("" with no vulkan module)
def private da_attn_stamp_key(t : T?; tr : Model; slab_heads : int) : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let kv_mul = tr.config.n_heads / tr.config.n_kv_heads
        t |> equal(da_slab_heads(kv_mul), int64(slab_heads), "the carrier's {kv_mul} q heads a kv head pick the {slab_heads}-head slab")
        return "{da_attn_stamp_name(mirror_kv_dtype(), da_slab_of(tr.config.attn_sinks, kv_mul))}_cls_spv"
    } else {
        return ""
    }
}

//! a session stepped once through the driver dispatches the decode attention stamp of the armed mirror codec that the
//! carrier's head shape picks - a slab `slab_heads` q heads wide - the pick a logits compare cannot see, the census can
def cell_regions_stamp_served(t : T?; file : string; slab_heads : int) {
    t |> run("the driver's step dispatches the stamp the carrier's shape picks") @(t : T?) {
        with_regions_rig(t, file) $(tr) {
            var pa <- prompt_a()
            var fa <- feed_a()
            var s <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, s, pa, long_length(pa), 0l)
            let key = da_attn_stamp_key(t, tr, slab_heads)
            let before = census(key)
            forward(tr, s, fa[0], long_length(pa))
            let after = census(key)
            t |> success(before == NO_CENSUS || after > before, "the step dispatched {key} ({after - before} dispatches)")
            delete s
            delete pa
            delete fa
        }
    }
}

//! where `on`, the fuse knob pinned to 0 - the split one-row forms - for the next load
def private split_forms_pin(on : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (on) {
            g_vk_fuse = 0
        }
    }
}

//! the batched cell over the split one-row forms: the fuse knob pinned off for the load and put back after, so the rows
//! hold to the split forms where the fused one-row forms round apart (a K-quant MoE's folded down, a PLE carrier's
//! fused act + proj); without the Vulkan module the knob does not exist and the cell skips
def cell_regions_batched_steps_split_forms(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        cell_regions_batched_steps(t, file, bar, true)
    } else {
        t |> run("the split one-row forms need the Vulkan driver") @(t : T?) {
            t |> skip("no Vulkan module in this build: the split one-row forms are the Vulkan driver's")
        }
    }
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
def private widen_hold(t : T?; tr : Model; got, ref : array<array<float>>; bar : LogitsBar; row : string; first : int64) {
    for (st in range64(long_length(got))) {
        hold_bar(t, tr, got[st], ref[st + 1l], ref[st], bar, "widened step {st + first}, {row}")
    }
}

//! three sessions over three regions: a batched step over two, one of them alone, then a batched step over all three -
//! the command records at a row count it had not recorded, after a one-row step, and every row still reads what its
//! session reads alone (the N-row sets decide their own forms at the first record, never off the one-row command's);
//! `split_forms` pins the fuse knob off for the load, as the split-forms cell does
def cell_regions_batched_widen(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact; split_forms : bool = false) {
    t |> run("a batched step over two rows, one row alone, then a batched step over three: the rows read what each session reads alone") @(t : T?) {
        with_regions_rig(t, file, RegionsLoad(regions = 3l, split_forms = split_forms)) $(tr) {
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
            t |> equal(STEPS - 1l, stepped, "the two-row step and every three-row step went through the N-row command")
            served_every_call(t)
            var ra <- logits_alone(tr, pa, fa)
            var rb <- logits_alone(tr, pb, fb)
            var fc_short <- [for (i in range64(STEPS - 2l)); fc[i]]
            var rc <- logits_alone(tr, pc, fc_short)
            widen_hold(t, tr, got_a, ra, bar, "row A", 0l)
            widen_hold(t, tr, got_b, rb, bar, "row B", 0l)
            widen_hold(t, tr, got_c, rc, bar, "row C", 2l)
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
    }
}

//! `rows` sessions over `rows` regions - more than the N-column leaves' eight-column tile, or more than the N-row command
//! steps at once - through STEPS batched steps: the two-row cell's body over `rows` rows, each step the fewest commands of
//! at most the rows the command steps at once (`vk_rdec_token_n_rows`), its GEMVs in tiles of eight columns, never a row at a time
def cell_regions_batched_groups(t : T?; file : string; rows : int64; bar : LogitsBar = LogitsBar.split_bar) {
    t |> run("{rows} sessions, a region each: a batched step over all of them runs in groups of the command's rows, its GEMVs in column tiles, and every row reads what its session reads alone") @(t : T?) {
        with_regions_rig(t, file, RegionsLoad(regions = rows)) $(tr) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                let at_once = vk_rdec_token_n_rows()
                t |> success(at_once >= 2l && (at_once < rows || min(rows, at_once) > int64(GEMV_COL_TILE)),
                    "the command steps {at_once} rows at once against the {rows} rows: the cell measures the groups or the column tiles past {GEMV_COL_TILE}")
            }
            var prompts <- [for (i in range64(rows)); ramp_ids(40l + i, i, 1000l + 100l * i, 37l + 2l * i, 5000l)]
            var feeds <- [for (i in range64(rows)); ramp_ids(STEPS, i, 2000l, 53l, 4000l)]
            batched_steps_body(t, tr, prompts, feeds, bar)
            delete prompts
            delete feeds
        }
    }
}
