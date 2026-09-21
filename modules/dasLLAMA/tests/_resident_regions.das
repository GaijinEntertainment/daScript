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
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed; gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // set_gpu_resident_regions_, moe_gpu_resident_active
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, where the module exists
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_n_rows: the N-row command served the batched cells' rows
require _model_tier public   // models_dir() + model_available() presence/size-tier gate

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

let private STEPS = 8l
let private WINDOW_ROWS = 512l   //! the engine's PF_WINDOW, asserted equal by the continuation cell: its cuts are placed against it

def private ramp_ids(n, salt, base, stride, span : int64) : array<int64> => <- [for (i in range64(n)); base + ((i + salt) * stride) % span]
def private prompt_a() : array<int64> => <- ramp_ids(40l, 0l, 1000l, 37l, 5000l)
def private prompt_b() : array<int64> => <- ramp_ids(57l, 0l, 3000l, 91l, 6000l)
def private feed_a() : array<int64> => <- ramp_ids(STEPS, 0l, 2000l, 53l, 4000l)
def private feed_b() : array<int64> => <- ramp_ids(STEPS, 0l, 2500l, 71l, 4500l)
def private prompt_c() : array<int64> => <- ramp_ids(48l, 0l, 1500l, 59l, 5500l)
def private feed_c() : array<int64> => <- ramp_ids(STEPS, 0l, 2200l, 67l, 4200l)

def private logits_of(s : Session; vocab : int64) : array<float> => <- [for (i in range64(vocab)); s.logits[i]]

//! one session alone: the prefill's logits, then each fed step's
def private logits_alone(tr : Model; prompt, toks : array<int64>) : array<array<float>> {
    var s <- create_session(tr, mirror_kv_dtype())
    let n = long_length(prompt)
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    out |> emplace <| logits_of(s, tr.config.vocab_size)
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| logits_of(s, tr.config.vocab_size)
    }
    delete s
    return <- out
}

def private peak_of(v : array<float>) : int64 {
    var at = 0l
    for (i in range64(long_length(v))) {
        if (v[i] > v[at]) {
            at = i
        }
    }
    return at
}

def private log_peaks(what : string; got, want : array<float>) {
    let pg = peak_of(got)
    let pw = peak_of(want)
    to_log(LOG_INFO, "{what}: argmax {pg} ({got[pg]}) against {pw} ({want[pw]})\n")
}

def private hold_equal(t : T?; got, want : array<float>; what : string) {
    log_peaks(what, got, want)
    let diff = logits_maxdiff(got, want)
    t |> equal(diff, 0.0, "{what}: bit for bit the session alone (maxdiff {diff})")
}

//! `got` within `rel` of `want`'s peak, and `control` - logits one token off - outside it
def private hold_within(t : T?; got, want, control : array<float>; what : string; rel : float = SPLIT_BAR_REL) {
    log_peaks(what, got, want)
    let bar = rel * logits_maxabs(want)
    let diff = logits_maxdiff(got, want)
    t |> success(diff <= bar, "{what}: within {bar} (maxdiff {diff})")
    let off = logits_maxdiff(got, control)
    t |> success(off > bar, "{what} control: logits one token off read past the bar (maxdiff {off} vs {bar})")
}

def private load_regions(t : T?; file : string; regions : int64) : Model {
    set_gpu_resident_regions_(regions)
    var tr <- load_model_(path_join(models_dir(), file), QuantMode.q8)
    set_gpu_resident_regions_(1l)
    tr.config.seq_len = min(tr.config.seq_len, REGIONS_RIG_CTX)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed at {regions} regions (the load log names any decline)")
    return <- tr
}

def private load_two_regions(t : T?; file : string) : Model => <- load_regions(t, file, 2l)

def private regions_ready(t : T?; file : string) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return false
    }
    if (!model_available(t, path_join(models_dir(), file))) return false
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return false
    }
    return true
}

//! two sessions over `pa` and `pb`, a region each, prefilled then stepped one row at a time, interleaved, against each alone
def private interleaved_single_steps_body(t : T?; tr : Model; pa, pb : array<int64>) {
    var fa <- feed_a()
    var fb <- feed_b()
    var ra <- logits_alone(tr, pa, fa)
    var rb <- logits_alone(tr, pb, fb)
    t |> success(logits_maxdiff(ra[0], rb[0]) > 0.0, "the two prompts read different logits, so a crossed region cannot pass")
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sa, pa, long_length(pa), 0l)
    hold_equal(t, sa.logits, ra[0], "session A's prefill")
    forward_prefill(tr, sb, pb, long_length(pb), 0l)
    hold_equal(t, sb.logits, rb[0], "session B's prefill, A's rows live beside it")
    t |> success(sa.rdec_region != sb.rdec_region, "the sessions sit in different regions ({sa.rdec_region}, {sb.rdec_region})")
    for (st in range64(STEPS)) {
        forward(tr, sa, fa[st], long_length(pa) + st)
        hold_equal(t, sa.logits, ra[st + 1l], "session A step {st}")
        forward(tr, sb, fb[st], long_length(pb) + st)
        hold_equal(t, sb.logits, rb[st + 1l], "session B step {st}")
    }
    var passes <- gpu_cpu_passes_()
    t |> equal(length(passes), 0, "the device served every call: {passes}")
    delete passes
    delete sa
    delete sb
    delete ra
    delete rb
    delete fa
    delete fb
}

def cell_regions_interleaved_single_steps(t : T?; file : string) {
    t |> run("two sessions, a region each: single steps interleaved read what each session reads alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            interleaved_single_steps_body(t, tr, pa, pb)
            delete pa
            delete pb
        }
        delete tr
    }
}

let private WIDE_ROWS = 3072l   //! the token command's RD_WIDE_POS, asserted equal by the wide cell: its prompts run past it

//! the same past the wide position: both prompts run longer than RD_WIDE_POS, so the one-row steps take the token command's
//! wide twin - recorded on the first such step, its stamp names apart from the split form's
def cell_regions_wide_single_steps(t : T?; file : string) {
    t |> run("two sessions past the wide position: the wide twin reads what each session reads alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- ramp_ids(WIDE_ROWS + 8l, 0l, 1000l, 37l, 5000l)
            var pb <- ramp_ids(WIDE_ROWS + 25l, 0l, 3000l, 91l, 6000l)
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> success(long_length(pa) > RD_WIDE_POS && long_length(pb) > RD_WIDE_POS, "both prompts run past the wide position ({RD_WIDE_POS})")
            }
            interleaved_single_steps_body(t, tr, pa, pb)
            delete pa
            delete pb
        }
        delete tr
    }
}

//! of the last logits' max |logit|: the window chain at two window splits of one prompt reduces its attention in two orders,
//! and the 0.5B's own device-vs-CPU rounding reads 0.30-0.50 of a 9.5-18 max logit (`test_gpu_resident_qwen2.das`'s bar)
let private SPLIT_BAR_REL = 0.06

def private prose_ids(n, salt : int64) : array<int64> => <- ramp_ids(n, salt, 1000l, 37l, 5000l)

//! `prompt` prefilled in one call against the same prompt prefilled as [0, cut) then [cut, n), then four fed steps on both
def private continue_at(t : T?; tr : Model; n, cut : int64) {
    var prompt <- prose_ids(n, 0l)
    var head <- [for (i in range64(cut)); prompt[i]]
    var tail <- [for (i in range64(n - cut)); prompt[cut + i]]
    var feed <- feed_a()
    // the control's session first, and gone before the compared pair: two regions hold two live sessions
    var short <- create_session(tr, mirror_kv_dtype())
    var less <- [for (i in range64(n - 1l)); prompt[i]]
    forward_prefill(tr, short, less, n - 1l, 0l)
    var short_logits <- logits_of(short, tr.config.vocab_size)
    delete short
    var whole <- create_session(tr, mirror_kv_dtype())
    var split <- create_session(tr, mirror_kv_dtype())
    let served0 = moe_gpu_resident_prefills()
    forward_prefill(tr, whole, prompt, n, 0l)
    forward_prefill(tr, split, head, cut, 0l)
    forward_prefill(tr, split, tail, n - cut, cut)
    t |> equal(moe_gpu_resident_prefills(), served0 + 3l, "{n} tokens cut at {cut}: the window chain served all three calls, the continuation among them")
    hold_within(t, split.logits, whole.logits, short_logits, "{n} tokens cut at {cut}: the continued prefill against the one-call prefill")
    delete short_logits
    for (st in range64(4l)) {
        var before <- logits_of(whole, tr.config.vocab_size)
        forward(tr, whole, feed[st], n + st)
        forward(tr, split, feed[st], n + st)
        hold_within(t, split.logits, whole.logits, before, "{n} tokens cut at {cut}, step {st}: decoding over the continued rows")
        delete before
    }
    delete whole
    delete split
    delete prompt
    delete head
    delete tail
    delete less
    delete feed
}

def cell_prefill_continues_on_the_device(t : T?; file : string) {
    t |> run("a prefill from a position past zero runs the window chain over the region's rows") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> equal(PF_WINDOW, WINDOW_ROWS, "the cuts are placed against the engine's own window")
            }
            continue_at(t, tr, 57l, 40l)     // a short tail inside the first window
            continue_at(t, tr, WINDOW_ROWS + 88l, WINDOW_ROWS)   // the cut on the window boundary
            continue_at(t, tr, WINDOW_ROWS + 388l, 300l)   // the tail runs a whole window from an unaligned start, then a remainder
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
        }
        delete tr
    }
    t |> run("the chat shape: a prompt, decoded steps, a second turn's prompt over that history, decoded steps") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
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
            hold_within(t, chat.logits, steps.logits, before, "the second turn through the window chain against the same tokens stepped one by one")
            delete before
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
            delete chat
            delete steps
            delete turn1
            delete said
            delete turn2
        }
        delete tr
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

//! two sessions over `pa` and `pb`, a region each, through STEPS batched steps against each session alone; `served`
//! says the carrier has an N-row form (a PLE or hybrid carrier's command declines by design and steps a row at a time)
def private batched_steps_body(t : T?; tr : Model; pa, pb : array<int64>; bar : LogitsBar; served : bool) {
    var fa <- feed_a()
    var fb <- feed_b()
    var ra <- logits_alone(tr, pa, fa)
    var rb <- logits_alone(tr, pb, fb)
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    eval(tr, sa, pa)   // `eval` advances `n_past`, the position a batched step reads
    eval(tr, sb, pb)
    var ws <- create_batch_workspace(tr)
    var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
    let gen_a = sa.rdec_gen
    let gen_b = sb.rdec_gen
    let steps0 = rdec_nrow_steps()
    for (st in range64(STEPS)) {
        var toks <- [fa[st], fb[st]]
        eval_batch(tr, ws, sess, toks)
        delete toks
        if (bar == LogitsBar.bit_exact) {
            hold_equal(t, sa.logits, ra[st + 1l], "batched step {st}, row A")
            hold_equal(t, sb.logits, rb[st + 1l], "batched step {st}, row B")
        } else {
            let rel = bar == LogitsBar.wide_bar ? WIDE_BAR_REL : SPLIT_BAR_REL
            hold_within(t, sa.logits, ra[st + 1l], ra[st], "batched step {st}, row A", rel)
            hold_within(t, sb.logits, rb[st + 1l], rb[st], "batched step {st}, row B", rel)
        }
    }
    t |> equal(sa.rdec_gen, gen_a, "row A kept its claim through every step: its history never came up from the host again")
    t |> equal(sb.rdec_gen, gen_b, "row B kept its claim through every step")
    var passes <- gpu_cpu_passes_()
    t |> equal(length(passes), 0, "the device served every call: {passes}")
    delete passes
    static_if (typeinfo builtin_module_exists(vulkan)) {   //! the rows stepped through the N-row command, not a row at a time: a declined command passes the bars above the same
        let nrows = vk_rdec_token_n_rows()
        t |> success(served ? nrows >= 2l : nrows == 0l, served ? "the N-row token command serves the two rows at once ({nrows} rows)" : "the N-row token command declines this carrier by design ({nrows} rows)")
    }
    let stepped = rdec_nrow_steps() - steps0
    t |> success(served ? stepped == STEPS : stepped == 0l, served ? "every batched step went through the N-row command ({stepped} of {STEPS})" : "no batched step went through the N-row command ({stepped})")
    sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
    delete sa
    delete sb
    delete ra
    delete rb
    delete fa
    delete fb
}

def cell_regions_batched_steps(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact; served : bool = true) {
    t |> run("two sessions, a region each: a batched step over both reads what each session reads alone, with no history resync") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            batched_steps_body(t, tr, pa, pb, bar, served)
            delete pa
            delete pb
        }
        delete tr
    }
}

//! the same past the unsplit position: both prompts run longer than RD_UNSPLIT_POS, so the batched steps take the
//! N-row command's split attention form - the arrival counters a row apart, the last piece of each row combining
def cell_regions_batched_steps_split(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact; served : bool = true) {
    t |> run("two sessions past the unsplit position: the split N-row command reads what each session reads alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- ramp_ids(WINDOW_ROWS + 8l, 0l, 1000l, 37l, 5000l)
            var pb <- ramp_ids(WINDOW_ROWS + 25l, 0l, 3000l, 91l, 6000l)
            static_if (typeinfo builtin_module_exists(vulkan)) {
                t |> success(long_length(pa) > RD_UNSPLIT_POS && long_length(pb) > RD_UNSPLIT_POS, "both prompts run past the unsplit position ({RD_UNSPLIT_POS})")
            }
            batched_steps_body(t, tr, pa, pb, bar, served)
            delete pa
            delete pb
        }
        delete tr
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
            with_job_que() {
                setup_dasllama_jobque_()
                vk_fuse_bisect_pin(0l)
                var tr0 <- load_two_regions(t, file)
                var base <- logits_alone(tr0, pa, fa)
                delete tr0
                for (bit in fixed_array(1l, 2l, 4l, 8l, 16l, 32l, 64l)) {
                    vk_fuse_bisect_pin(bit)
                    var tr <- load_two_regions(t, file)
                    var got <- logits_alone(tr, pa, fa)
                    for (st in range64(STEPS)) {
                        hold_within(t, got[st + 1l], base[st + 1l], base[st], "bisect bit {bit} step {st}")
                    }
                    delete got
                    if (bit == 64l) {
                        var pb <- prompt_b()
                        batched_steps_body(t, tr, pa, pb, LogitsBar.bit_exact, true)
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

//! a batched step before any one-row step, a one-row step on each row, a batched step again - every row under the unsplit
//! position, so the batched steps take the unsplit twin whether or not a one-row record came first
def cell_regions_batched_first(t : T?; file : string) {
    t |> run("a batched step first, one-row steps, a batched step again: the rows read what each session reads alone throughout") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            eval(tr, sa, pa)
            eval(tr, sb, pb)
            var ws <- create_batch_workspace(tr)
            var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
            var got_a : array<array<float>>
            var got_b : array<array<float>>
            for (st in range64(STEPS)) {
                if (st == 1l) {
                    var one_a <- [fa[st]]
                    var one_b <- [fb[st]]
                    eval(tr, sa, one_a)
                    eval(tr, sb, one_b)
                    delete one_a
                    delete one_b
                } else {
                    var toks <- [fa[st], fb[st]]
                    eval_batch(tr, ws, sess, toks)
                    delete toks
                }
                got_a |> emplace <| logits_of(sa, tr.config.vocab_size)
                got_b |> emplace <| logits_of(sb, tr.config.vocab_size)
            }
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
            var ra <- logits_alone(tr, pa, fa)
            var rb <- logits_alone(tr, pb, fb)
            for (st in range64(STEPS)) {
                hold_equal(t, got_a[st], ra[st + 1l], "batched-first step {st}, row A")
                hold_equal(t, got_b[st], rb[st + 1l], "batched-first step {st}, row B")
            }
            sess |> clear()
            unsafe {
                delete ws
                delete sess
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
        delete tr
    }
}

def private device_session(tr : Model) : Session => <- create_device_session(tr, gpu_device_session_dtype())

def cell_device_home_sessions(t : T?; file : string) {
    t |> run("two device-home sessions step between each other and in one batched step, bit for bit a host-cached session alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            t |> equal(gpu_device_sessions(), 2l, "the armed driver serves a device-home session per region")
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var ra <- logits_alone(tr, pa, fa)
            var rb <- logits_alone(tr, pb, fb)
            set_resident_prefill_allowed(false)   // the server's setting: the pin binds host-cached sessions, a device-home one prefills on the device all the same
            var sa <- device_session(tr)
            var sb <- device_session(tr)
            t |> equal(length(sa.key_cache) + length(sa.value_cache), 0, "a device-home session allocates no host cache")
            eval(tr, sa, pa)
            hold_equal(t, sa.logits, ra[0], "device-home A's prefill")
            eval(tr, sb, pb)
            hold_equal(t, sb.logits, rb[0], "device-home B's prefill")
            let half = STEPS / 2l
            for (st in range64(half)) {
                var ta <- [fa[st]]
                var tb <- [fb[st]]
                eval(tr, sa, ta)
                hold_equal(t, sa.logits, ra[st + 1l], "device-home A single step {st}")
                eval(tr, sb, tb)
                hold_equal(t, sb.logits, rb[st + 1l], "device-home B single step {st}")
                delete ta
                delete tb
            }
            moe_gpu_hydrate_session(tr, sa)
            t |> equal(length(sa.key_cache) + length(sa.value_cache), 0, "hydrating a device-home session brings nothing down: it has no host cache to fill")
            var ws <- create_batch_workspace(tr)
            var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
            for (st in range64(half, STEPS)) {
                var toks <- [fa[st], fb[st]]
                eval_batch(tr, ws, sess, toks)
                delete toks
                hold_equal(t, sa.logits, ra[st + 1l], "device-home batched step {st}, row A")
                hold_equal(t, sb.logits, rb[st + 1l], "device-home batched step {st}, row B")
            }
            set_resident_prefill_allowed(true)
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call, the prefills under the pin among them: {passes}")
            delete passes
            sess |> clear()
            unsafe {
                delete ws
                delete sess
            }
            gpu_device_kv_park(sa)
            gpu_device_kv_park(sb)
            delete sa
            delete sb
            delete ra
            delete rb
            delete pa
            delete pb
            delete fa
            delete fb
        }
        delete tr
    }
    cell_device_home_pin(t, file)
}

def private cell_device_home_pin(t : T?; file : string) {
    t |> run("a live device-home session's region is never handed to another session: the outsider passes, the owners step on") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var ra <- logits_alone(tr, pa, fa)
            var rb <- logits_alone(tr, pb, fb)
            var sa <- device_session(tr)
            var sb <- device_session(tr)
            eval(tr, sa, pa)
            eval(tr, sb, pb)
            var outsider <- create_session(tr, mirror_kv_dtype())
            eval(tr, outsider, pa)
            var passes <- gpu_cpu_passes_()
            t |> success(length(passes) == 1 && passes[0].reason == "busy", "the outsider's call passed as busy, and only it: {passes}")
            delete passes
            var ta <- [fa[0]]
            var tb <- [fb[0]]
            eval(tr, sa, ta)
            hold_equal(t, sa.logits, ra[1], "owner A's next step, its region untouched")
            eval(tr, sb, tb)
            hold_equal(t, sb.logits, rb[1], "owner B's next step, its region untouched")
            gpu_device_kv_park(sa)
            var late <- create_session(tr, mirror_kv_dtype())
            let served0 = moe_gpu_resident_prefills()
            eval(tr, late, pa)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "a parked region is anyone's: the next outsider prefills on the device")
            gpu_device_kv_park(sb)
            delete ta
            delete tb
            delete late
            delete outsider
            delete sa
            delete sb
            delete ra
            delete rb
            delete pa
            delete pb
            delete fa
            delete fb
        }
        delete tr
    }
}

def cell_device_kv_park_adopt(t : T?; file : string) {
    t |> run("a parked claim hands its rows to a later session once: the adopter prefills only the rest") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- prose_ids(57l, 0l)
            var tail <- [for (i in range64(17l)); prompt[40l + i]]
            var less <- [for (i in range64(56l)); prompt[i]]
            var short <- create_session(tr, mirror_kv_dtype())
            eval(tr, short, less)
            var short_logits <- logits_of(short, tr.config.vocab_size)
            delete short
            delete less
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
            hold_within(t, second.logits, want, short_logits, "the adopter against the whole prompt prefilled in one call")
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
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
            delete a
            delete b
            delete third
            delete second
            delete want
            delete prompt
            delete tail
        }
        delete tr
    }
}

def private cached_of(events : array<SchedEvent>; id : int64) : int64 => finished_of(events, id).n_cached

//! three requests through a two-stream scheduler, then a second turn that opens with the last one's prompt and reply
//! (two slots park two claims: the third request's arrival is what evicts the first's rows); every prompt is one prefill chunk
def private serve_four(m : Model; device : bool; var events : array<SchedEvent>) : tuple<on : bool; parked : int> {
    var sch <- create_scheduler(m, 2l, 4l, 64l)
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
    submit(m, sch, make_req(4l, turn2, 8l))
    drain(m, sch, events)
    delete pa
    delete pb
    delete pc
    delete turn2
    let parked = length(sch.parked)
    delete sch
    return (on = on, parked = parked)
}

def cell_scheduler_device_mode(t : T?; file : string) {
    t |> run("a device-mode scheduler emits what the host-cached one does where the two run one code path, and a second turn adopts its first turn's rows") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var host : array<SchedEvent>
            var dev : array<SchedEvent>
            let served0 = moe_gpu_resident_prefills()
            t |> success(!serve_four(tr, false, host).on, "the control scheduler serves host-cached sessions")
            let served1 = moe_gpu_resident_prefills()
            let served = serve_four(tr, true, dev)
            t |> success(served.on, "an idle scheduler switches to device-home sessions")
            // four turns on two stream slots: the first turn's claim is evicted by the third's, the third's is spent by the turn that adopts it
            t |> equal(served.parked, gpu_device_prefill_continues(tr) ? 2 : 0, "parked claims stay within the stream slots, and a model whose prefill cannot continue parks none")
            // one code path on both sides: each prompt enters the window chain once and each step the batched token command;
            // a device-home stream differs by the host copy it skips. The adopting turn splits its prefill, so it holds no token compare
            t |> equal(served1 - served0, 4l, "the host-cached side prefilled its four prompts on the device, one call each")
            t |> equal(moe_gpu_resident_prefills() - served1, 4l, "and so did the device-home side")
            for (id in range64(1l, 5l)) {
                var th <- toks_of(host, id)
                var td <- toks_of(dev, id)
                log_gen_texts(tr, "request {id} (CPU = host-cached, GPU = device-home)", th, td)
                t |> equal(length(td), 8, "request {id} ran its eight tokens on the device")
                if (id < 4l || !gpu_device_prefill_continues(tr)) {
                    t |> success(ids_equal(th, td), "request {id}: device-home {td} against host-cached {th}")
                }
                delete th
                delete td
            }
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
        delete tr
    }
}

def cell_scheduler_device_prefill_chunks(t : T?; file : string) {
    t |> run("a device-home stream prefills in the device's windows, and a model whose prefill cannot continue in one call") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var sch <- create_scheduler(tr, 2l, 4l, 64l)
            t |> success(set_device_kv(sch, true), "an idle scheduler switches to device-home sessions")
            var long_prompt <- prose_ids(WINDOW_ROWS + 188l, 3l)
            var events : array<SchedEvent>
            let served0 = moe_gpu_resident_prefills()
            submit(tr, sch, make_req(1l, long_prompt, 2l))
            drain(tr, sch, events)
            let calls = moe_gpu_resident_prefills() - served0
            // the scheduler's own chunk is 64 tokens: eleven calls, were the device window not the floor
            t |> equal(calls, gpu_device_prefill_continues(tr) ? 2l : 1l, "{long_length(long_prompt)} tokens reach the device in window-sized chunks, or whole where the model's prefill cannot continue ({calls} calls)")
            t |> equal(length(toks_of(events, 1l)), 2, "and the stream ran its two tokens")
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
            set_device_kv(sch, false)
            delete events
            delete long_prompt
            delete sch
        }
        delete tr
    }
}

def cell_device_home_guards(t : T?; file : string) {
    t |> run("a device-home session that finds no region panics with the reason, and the owners step on") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var ra <- logits_alone(tr, pa, fa)
            var sa <- device_session(tr)
            var sb <- device_session(tr)
            eval(tr, sa, pa)
            eval(tr, sb, pb)
            var third <- device_session(tr)
            var tripped = false
            try {
                eval(tr, third, pa)
            } recover {
                tripped = true
            }
            t |> success(tripped, "a third device-home session over two regions has nowhere to keep its rows: the CPU cannot take it, so it panics")
            var passes <- gpu_cpu_passes_()
            t |> success(length(passes) == 1 && passes[0].reason == "busy", "the refused call was counted as busy, and only it: {passes}")
            delete passes
            var ta <- [fa[0]]
            eval(tr, sa, ta)
            hold_equal(t, sa.logits, ra[1], "owner A's next step after the refused session")
            gpu_device_kv_park(sa)
            gpu_device_kv_park(sb)
            gpu_device_kv_park(third)
            delete ta
            delete third
            delete sa
            delete sb
            delete ra
            delete pa
            delete pb
            delete fa
        }
        delete tr
    }
}

def cell_driver_reports(t : T?; file : string) {
    t |> run("every reason a call can pass to the CPU has words of its own, the plan sizes every region, and the status counts the mirrors") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
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
        delete tr
    }
    t |> run("a load under a switched-off whole-model route says it was never attempted") @(t : T?) {
        if (!regions_ready(t, file)) return
        let route0 = gpu_want_resident()
        set_gpu_resident_route(false)
        var tr <- load_model_(path_join(models_dir(), file), QuantMode.q8)
        set_gpu_resident_route(route0)
        t |> success(!moe_gpu_resident_active(), "the whole-model driver did not arm")
        t |> success(gpu_resident_decline_() |> starts_with("not attempted"), "and the reason says so: {gpu_resident_decline_()}")
        delete tr
    }
}

def cell_passes_with_their_rows(t : T?; file : string) {
    t |> run("a prefill that skips positions passes as a gap, its device rows brought down first") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var head <- prose_ids(40l, 0l)
            var tail <- prose_ids(10l, 9l)
            var s <- create_session(tr, mirror_kv_dtype())
            eval(tr, s, head)
            t |> success(!s.rdec_host_current, "the prompt's rows live on the device alone")
            forward_prefill(tr, s, tail, 10l, 50l)
            var passes <- gpu_cpu_passes_()
            t |> success(length(passes) == 1 && passes[0].reason == "gap" && passes[0].count == 1l, "positions 40 to 49 were never written: the call passed as a gap ({passes})")
            t |> success(s.rdec_host_current, "and the CPU took it over rows brought down from the device")
            delete passes
            delete s
            delete head
            delete tail
        }
        delete tr
    }
    t |> run("a prefill past the region's context passes as cap, its device rows brought down first") @(t : T?) {
        if (!regions_ready(t, file)) return
        set_gpu_ctx_max(CAPPED_RIG_CTX)
        var tr <- load_two_regions(t, file)
        set_gpu_ctx_max(REGIONS_RIG_CTX)
        with_job_que() {
            setup_dasllama_jobque_()
            t |> equal(gpu_device_session_ctx(), CAPPED_RIG_CTX, "the regions armed at the capped context")
            var head <- prose_ids(CAPPED_RIG_CTX - 96l, 0l)
            var tail <- prose_ids(200l, 9l)
            var s <- create_session(tr, mirror_kv_dtype())
            eval(tr, s, head)
            forward_prefill(tr, s, tail, 200l, CAPPED_RIG_CTX - 96l)
            var passes <- gpu_cpu_passes_()
            t |> success(length(passes) == 1 && passes[0].reason == "cap" && passes[0].count == 1l, "200 tokens from 96 short of the region's end: the call passed as cap ({passes})")
            t |> success(s.rdec_host_current, "and the CPU took it over rows brought down from the device")
            delete passes
            delete s
            delete head
            delete tail
        }
        delete tr
    }
}

//! the kernel census's count over a stamp's two codec twins
def private stamps_dispatched(stamp_f16, stamp_f32 : string) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_coverage_of(stamp_f16) + vk_kernel_coverage_of(stamp_f32)
    } else {
        return 0l
    }
}

//! a session stepped once through the driver dispatches the stamp (either codec twin) the carrier's shape picks -
//! the pick a logits compare cannot see, the census can
def cell_regions_stamp_served(t : T?; file : string; stamp_f16, stamp_f32 : string) {
    t |> run("the driver's step dispatches the stamp the carrier's shape picks") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var fa <- feed_a()
            var s <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, s, pa, long_length(pa), 0l)
            let before = stamps_dispatched(stamp_f16, stamp_f32)
            forward(tr, s, fa[0], long_length(pa))
            let after = stamps_dispatched(stamp_f16, stamp_f32)
            t |> success(after > before, "the step dispatched {stamp_f16} or {stamp_f32} ({after - before} dispatches)")
            delete s
            delete pa
            delete fa
        }
        delete tr
    }
}

//! the fuse knob's pin where `on`: 0 pins the split one-row forms for the next load, -1 hands the knob back to the environment
def private fuse_pin(on : bool; value : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (on) {
            g_vk_fuse = value
        }
    }
}

//! the batched cell over the split one-row forms: the fuse knob pinned off for the load and unpinned after, so the rows
//! hold to the split forms where the fused one-row forms round apart (a K-quant MoE's folded down, a PLE carrier's
//! fused act + proj); without the Vulkan module the knob does not exist and the cell skips
def cell_regions_batched_steps_split_forms(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        fuse_pin(true, 0)
        cell_regions_batched_steps(t, file, bar)
        fuse_pin(true, -1)
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
        var one_a <- [fa[st]]
        var one_b <- [fb[st]]
        eval(tr, sa, one_a)
        eval(tr, sb, one_b)
        delete one_a
        delete one_b
    } else {
        var toks <- [fa[st], fb[st], fc[st - 2l]]
        eval_batch(tr, ws, three, toks)
        delete toks
    }
}

//! a row's stepped logits against its session alone, from the step it joined at
def private widen_hold(t : T?; got, ref : array<array<float>>; bar : LogitsBar; row : string; first : int64) {
    for (st in range64(long_length(got))) {
        if (bar == LogitsBar.bit_exact) {
            hold_equal(t, got[st], ref[st + 1l], "widened step {st + first}, {row}")
        } else {
            let rel = bar == LogitsBar.wide_bar ? WIDE_BAR_REL : SPLIT_BAR_REL
            hold_within(t, got[st], ref[st + 1l], ref[st], "widened step {st + first}, {row}", rel)
        }
    }
}

//! three sessions over three regions: a batched step over two, one of them alone, then a batched step over all three -
//! the command records at a row count it had not recorded, after a one-row step, and every row still reads what its
//! session reads alone (the N-row sets decide their own forms at the first record, never off the one-row command's);
//! `split_forms` pins the fuse knob off for the load, as the split-forms cell does
def cell_regions_batched_widen(t : T?; file : string; bar : LogitsBar = LogitsBar.bit_exact; split_forms : bool = false) {
    t |> run("a batched step over two rows, one row alone, then a batched step over three: the rows read what each session reads alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        fuse_pin(split_forms, 0)
        var tr <- load_regions(t, file, 3l)
        fuse_pin(split_forms, -1)
        with_job_que() {
            setup_dasllama_jobque_()
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
            var got_a : array<array<float>>
            var got_b : array<array<float>>
            var got_c : array<array<float>>
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
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
            var ra <- logits_alone(tr, pa, fa)
            var rb <- logits_alone(tr, pb, fb)
            var fc_short <- [for (i in range64(STEPS - 2l)); fc[i]]
            var rc <- logits_alone(tr, pc, fc_short)
            widen_hold(t, got_a, ra, bar, "row A", 0l)
            widen_hold(t, got_b, rb, bar, "row B", 0l)
            widen_hold(t, got_c, rc, bar, "row C", 2l)
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
        delete tr
    }
}
