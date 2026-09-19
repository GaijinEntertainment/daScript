// The cells of the resident driver's mirror regions and device-home sessions, shared by the per-model test files
// (test_gpu_resident_regions*.das): ONE model a file, so a process loads one carrier.

options gen2
options _dasllama_internal = true

module _resident_regions shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama   // create_session, create_batch_workspace, eval_batch
require dasllama/dasllama_scheduler   // the device-mode scheduler cell
require math
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed; gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // set_gpu_resident_regions_, moe_gpu_resident_active
require _model_tier public   // models_dir() + model_available() presence/size-tier gate

// The resident driver's mirror regions (a small carrier under DASLLAMA_GPU=1): two
// sessions hold their K/V on the device at once, one region each. The instrument is the driver
// against itself - a session stepped between another session's steps reads bit for bit what it
// reads alone, because a region is an offset and the kernels are the same - so no CPU chain runs.
// The two prompts differ in content and length: a step that read the other region's rows cannot
// land on the alone-run's logits.

//! the rig's context: the mirror regions' cap and every host-cached session's K/V cache - a model max of 131072 is gigabytes of each
let REGIONS_RIG_CTX = 8192l

[init]
def private regions_rig_init() {
    allow_cpu_prefill()   // the pin cell's outsider prefills on the CPU by design
    set_gpu_ctx_max(REGIONS_RIG_CTX)
}

let private STEPS = 8l

def private prompt_a() : array<int64> => <- [for (i in range64(40l)); 1000l + (i * 37l) % 5000l]
def private prompt_b() : array<int64> => <- [for (i in range64(57l)); 3000l + (i * 91l) % 6000l]
def private feed_a() : array<int64> => <- [for (i in range64(STEPS)); 2000l + (i * 53l) % 4000l]
def private feed_b() : array<int64> => <- [for (i in range64(STEPS)); 2500l + (i * 71l) % 4500l]

def private logits_of(s : Session; vocab : int64) : array<float> => <- [for (i in range64(vocab)); s.logits[i]]

//! one session alone: the prefill's logits, then each fed step's
def private alone(tr : Model; prompt, toks : array<int64>) : array<array<float>> {
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

def private hold_equal(t : T?; got, want : array<float>; what : string) {
    let diff = logits_maxdiff(got, want)
    t |> equal(diff, 0.0, "{what}: bit for bit the session alone (maxdiff {diff})")
}

def private load_two_regions(t : T?; file : string) : Model {
    set_gpu_resident_regions_(2l)
    var tr <- load_model_(path_join(models_dir(), file), QuantMode.q8)
    set_gpu_resident_regions_(1l)
    tr.config.seq_len = min(tr.config.seq_len, REGIONS_RIG_CTX)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed at two regions (the load log names any decline)")
    return <- tr
}

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

def cell_regions_interleaved_single_steps(t : T?; file : string) {
    t |> run("two sessions, a region each: single steps interleaved read what each session reads alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var ra <- alone(tr, pa, fa)
            var rb <- alone(tr, pb, fb)
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
            delete pa
            delete pb
            delete fa
            delete fb
        }
        delete tr
    }
}

//! of the last logits' max |logit|: the window chain at two window splits of one prompt reduces its attention in two orders,
//! and the 0.5B's own device-vs-CPU rounding reads 0.30-0.50 of a 9.5-18 max logit (`test_gpu_resident_qwen2.das`'s bar)
let private SPLIT_BAR_REL = 0.06

def private prose_ids(n, salt : int64) : array<int64> => <- [for (i in range64(n)); 1000l + ((i + salt) * 37l) % 5000l]

//! `prompt` prefilled in one call against the same prompt prefilled as [0, cut) then [cut, n), then four fed steps on both
def private continue_at(t : T?; tr : Model; n, cut : int64) {
    var prompt <- prose_ids(n, 0l)
    var head <- [for (i in range64(cut)); prompt[i]]
    var tail <- [for (i in range64(n - cut)); prompt[cut + i]]
    var feed <- feed_a()
    //! the control's session first, and gone before the compared pair: two regions hold two live sessions
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
    let bar = SPLIT_BAR_REL * logits_maxabs(whole.logits)
    let diff = logits_maxdiff(split.logits, whole.logits)
    t |> success(diff <= bar, "{n} tokens cut at {cut}: the continued prefill's logits within {bar} of the one-call prefill's (maxdiff {diff})")
    let off = logits_maxdiff(split.logits, short_logits)
    t |> success(off > bar, "{n} tokens cut at {cut} control: a prompt one token short reads past the bar (maxdiff {off} vs {bar})")
    delete short_logits
    for (st in range64(4l)) {
        forward(tr, whole, feed[st], n + st)
        forward(tr, split, feed[st], n + st)
        let sbar = SPLIT_BAR_REL * logits_maxabs(whole.logits)
        let sdiff = logits_maxdiff(split.logits, whole.logits)
        t |> success(sdiff <= sbar, "{n} tokens cut at {cut}, step {st}: decoding over the continued rows within {sbar} (maxdiff {sdiff})")
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
            continue_at(t, tr, 57l, 40l)     // a short tail inside the first window
            continue_at(t, tr, 600l, 512l)   // the cut on the window boundary
            continue_at(t, tr, 900l, 300l)   // the tail runs a whole window from an unaligned start, then a remainder
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
            eval(tr, chat, turn2)   //! one window-chain call from position 48
            for (i in range64(20l)) {   //! the same twenty tokens through the token command, one position each
                forward(tr, steps, turn2[i], 40l + STEPS + i)
            }
            let bar = SPLIT_BAR_REL * logits_maxabs(steps.logits)
            let diff = logits_maxdiff(chat.logits, steps.logits)
            t |> success(diff <= bar, "the second turn through the window chain within {bar} of the same tokens stepped one by one (maxdiff {diff})")
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

def cell_regions_batched_steps(t : T?; file : string) {
    t |> run("two sessions, a region each: a batched step over both reads what each session reads alone, with no history resync") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var ra <- alone(tr, pa, fa)
            var rb <- alone(tr, pb, fb)
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            eval(tr, sa, pa)   //! `eval` advances `n_past`, the position a batched step reads
            eval(tr, sb, pb)
            var ws <- create_batch_workspace(tr)
            var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
            let gen_a = sa.rdec_gen
            let gen_b = sb.rdec_gen
            for (st in range64(STEPS)) {
                var toks <- [fa[st], fb[st]]
                eval_batch(tr, ws, sess, toks)
                delete toks
                hold_equal(t, sa.logits, ra[st + 1l], "batched step {st}, row A")
                hold_equal(t, sb.logits, rb[st + 1l], "batched step {st}, row B")
            }
            t |> equal(sa.rdec_gen, gen_a, "row A kept its claim through every step: its history never came up from the host again")
            t |> equal(sb.rdec_gen, gen_b, "row B kept its claim through every step")
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
            delete passes
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
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

// ===== device-home sessions: no host cache, the region is the only copy =====

def private device_session(tr : Model) : Session => <- create_device_session(tr, gpu_device_session_dtype())

def cell_device_home_sessions(t : T?; file : string) {
    t |> run("two device-home sessions step between each other and in one batched step, bit for bit a host-cached session alone") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            t |> equal(gpu_device_sessions(tr), 2l, "the armed driver serves a device-home session per region")
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var ra <- alone(tr, pa, fa)
            var rb <- alone(tr, pb, fb)
            var sa <- device_session(tr)
            var sb <- device_session(tr)
            t |> equal(length(sa.key_cache) + length(sa.value_cache), 0, "a device-home session allocates no host cache")
            eval(tr, sa, pa)
            hold_equal(t, sa.logits, ra[0], "device-home A's prefill")
            eval(tr, sb, pb)
            hold_equal(t, sb.logits, rb[0], "device-home B's prefill")
            let half = STEPS / 2l
            for (st in range64(half)) {   //! single steps, interleaved
                var ta <- [fa[st]]
                var tb <- [fb[st]]
                eval(tr, sa, ta)
                hold_equal(t, sa.logits, ra[st + 1l], "device-home A single step {st}")
                eval(tr, sb, tb)
                hold_equal(t, sb.logits, rb[st + 1l], "device-home B single step {st}")
                delete ta
                delete tb
            }
            var ws <- create_batch_workspace(tr)
            var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
            for (st in range64(half, STEPS)) {   //! then batched
                var toks <- [fa[st], fb[st]]
                eval_batch(tr, ws, sess, toks)
                delete toks
                hold_equal(t, sa.logits, ra[st + 1l], "device-home batched step {st}, row A")
                hold_equal(t, sb.logits, rb[st + 1l], "device-home batched step {st}, row B")
            }
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call: {passes}")
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
    t |> run("a live device-home session's region is never handed to another session: the outsider passes, the owners step on") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- prompt_a()
            var pb <- prompt_b()
            var fa <- feed_a()
            var fb <- feed_b()
            var ra <- alone(tr, pa, fa)
            var rb <- alone(tr, pb, fb)
            var sa <- device_session(tr)
            var sb <- device_session(tr)
            eval(tr, sa, pa)
            eval(tr, sb, pb)
            var outsider <- create_session(tr, mirror_kv_dtype())
            eval(tr, outsider, pa)   //! both regions are held: the CPU rails serve the host-cached outsider
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
            var first <- device_session(tr)
            eval(tr, first, prompt)
            var want <- logits_of(first, tr.config.vocab_size)
            let claim = gpu_device_kv_park(first)
            delete first
            t |> success(claim != 0l, "a session that prefilled on the device parks a claim")
            var second <- device_session(tr)
            let served0 = moe_gpu_resident_prefills()
            t |> success(gpu_device_kv_adopt(second, claim, 40l), "the claim stands: the adopter takes 40 of its 57 rows")
            t |> equal(second.n_past, 40l, "the adopter continues from the adopted rows")
            eval(tr, second, tail)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "one device prefill: the seventeen tokens past the adopted rows")
            let bar = SPLIT_BAR_REL * logits_maxabs(want)
            let diff = logits_maxdiff(second.logits, want)
            t |> success(diff <= bar, "the adopter reads the whole prompt's logits within {bar} (maxdiff {diff})")
            var third <- device_session(tr)
            t |> success(!gpu_device_kv_adopt(third, claim, 40l), "a claim is spent by its adoption: nobody else takes the rows")
            t |> equal(third.n_past, 0l, "a refused adopter starts empty")
            let claim2 = gpu_device_kv_park(second)
            var a <- create_session(tr, mirror_kv_dtype())
            var b <- create_session(tr, mirror_kv_dtype())
            eval(tr, a, tail)   //! two outsiders take both regions: the parked rows are gone
            eval(tr, b, tail)
            t |> success(!gpu_device_kv_adopt(third, claim2, 40l), "a claim dies when another session takes its region")
            gpu_device_kv_park(third)
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

// ===== the scheduler's device mode =====

def private make_req(id : int64; prompt : array<int64>; max_new : int64) : PendingReq {
    var r <- PendingReq(id = id, params = SamplingParams(), max_new = max_new)
    r.prompt := prompt
    return <- r
}

def private drain(m : Model; var sch : Scheduler; var events : array<SchedEvent>) {
    var steps = 0
    while (scheduler_step(m, sch, events)) {
        steps ++
        if (steps > 4096) {
            panic("test_gpu_resident_regions: the scheduler did not go idle in 4096 steps")
        }
    }
}

def private toks_of(events : array<SchedEvent>; id : int64) : array<int64>
    => <- [for (e in events); e.token; where e.stream == id && e.kind == SchedEventKind.piece]

def private cached_of(events : array<SchedEvent>; id : int64) : int64 {
    for (e in events) {
        if (e.stream == id && e.kind == SchedEventKind.finished) return e.n_cached
    }
    return -1l
}

//! three requests through a two-stream scheduler, then a second turn that opens with the last one's prompt and reply
//! (two slots park two claims: the third request's arrival is what evicts the first's rows)
def private serve_four(m : Model; device : bool; var events : array<SchedEvent>) : bool {
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
    turn2 |> push_from(toks_of(events, 3l))
    turn2 |> push_from(prose_ids(20l, 5l))
    submit(m, sch, make_req(4l, turn2, 8l))
    drain(m, sch, events)
    delete pa
    delete pb
    delete pc
    delete turn2
    unsafe {
        delete sch
    }
    return on
}

def cell_scheduler_device_mode(t : T?; file : string) {
    t |> run("a device-mode scheduler emits token for token what the host-cached one does, and a second turn adopts its first turn's rows") @(t : T?) {
        if (!regions_ready(t, file)) return
        var tr <- load_two_regions(t, file)
        with_job_que() {
            setup_dasllama_jobque_()
            var host : array<SchedEvent>
            var dev : array<SchedEvent>
            t |> success(!serve_four(tr, false, host), "the control scheduler serves host-cached sessions")
            t |> success(serve_four(tr, true, dev), "an idle scheduler switches to device-home sessions")
            for (id in range64(1l, 5l)) {
                var th <- toks_of(host, id)
                var td <- toks_of(dev, id)
                t |> equal(length(td), 8, "request {id} ran its eight tokens on the device")
                t |> success(length(th) == length(td) && logits_same_ids(th, td), "request {id}: device-home {td} against host-cached {th}")
                delete th
                delete td
            }
            t |> equal(cached_of(host, 4l), 0l, "the host-cached flat scheduler keeps no prefix")
            t |> success(cached_of(dev, 4l) >= 33l, "the second turn adopted the rows of the turn it continues from the device ({cached_of(dev, 4l)} of them)")
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call of both runs: {passes}")
            delete passes
            delete host
            delete dev
        }
        delete tr
    }
}

def private logits_same_ids(a, b : array<int64>) : bool {
    for (x, y in a, b) {
        if (x != y) return false
    }
    return true
}
