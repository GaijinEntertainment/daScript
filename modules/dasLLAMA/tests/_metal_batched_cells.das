options gen2
options _dasllama_internal = true

module _metal_batched_cells shared public

//! the Metal batched driver's cells per model file, each on the Metal form its model's block loads; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the public facade, reached inside the das_metal halves
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - mtp_seam_declines, inside the das_metal halves
require dasllama/dasllama_math   // nolint:STYLE030,LINT019 - parallel_argmax, inside the das_metal halves
require _metal_blob_twin   // segments_window, metal_kernel_count_prefix: the window's and the scan's witnesses
require dasllama/dasllama_batch   // nolint:STYLE030,LINT019 - batch_step_census, inside the das_metal halves
require dasllama/dasllama_bench   // nolint:STYLE030,LINT019 - bench_tg_batched_rep, inside the das_metal halves
require dasllama/dasllama_scheduler   // nolint:STYLE030,LINT019 - the scheduler the joint cell drives, inside the das_metal halves
require _scheduler_rig   // nolint:STYLE030,LINT019 - toks_of, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_common   // metal_batch_decode_stats - the batched driver's own count
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - the pre-encode lever and its taken count, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_shutdown before the leak gate, inside the das_metal halves
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - metal_live_object_count for the leak gate, inside the das_metal halves
require daslib/env_registry   // nolint:STYLE030,LINT019 - env_is_set, inside the das_metal halves
require daslib/jobque_boost
require math   // nolint:STYLE030,LINT019 - max, min, inside the das_metal halves
require _model_tier
require _model_form   // with_ctx_cut: a cell's own context on the form's one load
require _resident_feed   // nolint:STYLE029,LINT019 - batched_logits_rows, inside the das_metal half

// The batched bench row on the Metal batched driver: `bench_tg_batched_rep` homes its streams on the
// host where no whole-model driver homes them, and every timed step goes through ONE batched
// dispatch - the llama carrier, the E-series (PLE rows form), the shared-expert MoE (rows site),
// the deltanet hybrid (recurrent rows against per-session mirrors) and the NextN carrier (GPU
// logits, the carry landed per row) each serve at a rate; the self-speculative streams tick through
// their own rounds, token-exact against each stream alone; a two-row sampled tick the driver declines steps its rows one at a
// time, each stream token-exact against sampling right after its own forward. The refusal contract lives in
// test_batch_decode.das.
// Each cell runs on the Metal form of its model's block (`metal_form`, the blob twin under `MetalMode.when_available`), inside
// the block's job queue, its sessions' context cut to the cell's own (`metal_cell`): the form skips as one cell on a build
// without das_metal, off the JIT and without the carrier under models_dir() - the large-tier shexp carrier unless
// DASLLAMA_PARITY_FULL=1 - and a cell skips where the run's family filter leaves its family out.

//! `cell` on the Metal form's model `tr` as the subtest `name`: under the family filter `family`, the sessions' context cut to
//! `cap` positions for it and put back after
def private metal_cell(t : T?; name, family : string; var tr : Model; cap : int64;
                       cell : function<(t : T?; tr : Model) : void>) {
    unsafe {
        t |> run(name) @capture(& tr)(t : T ?) {
            return if (!family_on(t, family))
            with_ctx_cut(tr, cap) {
                invoke(cell, t, tr)
            }
        }
    }
}

//! what one served batched row reports: `bench_tg_batched_rep`'s refusal and rate over 4 streams of 32 positions and 8 steps,
//! and the steps the device batched driver, the CPU batched stack and the Metal batched driver each counted over it
struct private BatchedRow {
    why : string
    tps : double
    device_steps : int64
    cpu_stack_steps : int64
    metal_steps : int64
}

//! the batched row timed on `tr` and the step counters around it (`BatchedRow`), for the cell's own asserts
def private batched_row_run(tr : Model) : BatchedRow {   // nolint:LINT012,LINT019 - the body is the das_metal half
    var r : BatchedRow
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let census0 = batch_step_census()
        let steps0 = metal_batch_decode_stats().steps
        let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
        let census1 = batch_step_census()
        r = BatchedRow(why = served.why, tps = served.tps, device_steps = census1.device - census0.device,
            cpu_stack_steps = census1.cpu_stack - census0.cpu_stack, metal_steps = metal_batch_decode_stats().steps - steps0)
    }
    return r
}

def private batched_row_llama(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> equal(gpu_device_sessions(), 0l, "no whole-model driver homes a stream: the row's streams are host-cached")
        let r = batched_row_run(tr)
        t |> equal(r.why, "", "the row is served: {r.why}")
        t |> success(r.tps > 0.0lf, "... at a rate ({r.tps:.1f} tok/s summed over 4 streams)")
        t |> success(r.device_steps >= 8l, "the timed steps went through the device batched driver ({r.device_steps} device steps)")
        t |> equal(r.cpu_stack_steps, 0l, "no step ran on the CPU batched stack")
        t |> success(r.metal_steps >= 8l, "the Metal batched driver counts the steps as its own")
    }
}

def private batched_row_eseries(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let r = batched_row_run(tr)
        t |> equal(r.why, "", "the row is served: {r.why}")
        t |> success(r.tps > 0.0lf, "... at a rate ({r.tps:.1f} tok/s summed over 4 streams)")
        t |> success(r.device_steps >= 8l, "the timed steps went through the device batched driver ({r.device_steps} device steps)")
        t |> equal(r.cpu_stack_steps, 0l, "the blob-only model never reached the CPU batched stack")
    }
}

let private GATHER_STEPS = 6l
let private LLAMA_FILE = "Llama-3.2-1B-Instruct-Q8_0.gguf"
let private E2B_FILE = "gemma-4-E2B-it-Q8_0.gguf"

//! three host-cached sessions over three prompts batched through GATHER_STEPS steps on the Metal batched driver: every row's
//! logits a step, with the driver's own step count asserted
def private metal_batched_logits(t : T?; tr : Model) : array<array<float>> {   // nolint:LINT012,LINT019 — reached from the das_metal halves
    var rows : array<array<float>>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                         <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                         <- encode(tr, "List the days of the week: Monday, Tuesday,", true, false)]
        var feeds <- [for (i in range(3)); [for (st in range64(GATHER_STEPS)); 400l + 37l * st + 11l * int64(i)]]
        rows <- batched_logits_rows(t, tr, prompts, feeds, KVDtype.f16, GATHER_STEPS) <| $ => metal_batch_decode_stats().steps
        delete feeds
        delete prompts
    }
    return <- rows
}

//! the distinct-session batched step gathers its rows off their ids on the device (`metal_batch_rows_gathered` up by every row,
//! the host embedding none, `batch_rows_host_embedded`), each row's logits at every step bit for bit the same steps' with the
//! gather off (`set_metal_batch_gather(false)`: the host embeds every row and the step pokes it up), which gathers none - the control
def private batch_gather(t : T?; tr : Model; file : string) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let gather0 = metal_batch_gather()
        set_metal_batch_gather(true)
        let g0 = metal_batch_rows_gathered()
        let h0 = batch_rows_host_embedded()
        var dev <- metal_batched_logits(t, tr)
        t |> equal(metal_batch_rows_gathered() - g0, 3l * GATHER_STEPS, "every batched row gathered off its id")
        t |> equal(batch_rows_host_embedded() - h0, 0l, "the host embedded no batched row")
        set_metal_batch_gather(false)
        let g1 = metal_batch_rows_gathered()
        let h1 = batch_rows_host_embedded()
        var host <- metal_batched_logits(t, tr)
        t |> equal(metal_batch_rows_gathered() - g1, 0l, "with the gather off no batched row gathered: the counter's control")
        t |> equal(batch_rows_host_embedded() - h1, 3l * GATHER_STEPS, "with the gather off the host embedded every batched row")
        set_metal_batch_gather(gather0)
        gathered_rows_bit_same(t, tr, file, dev, host, 3l)
        delete dev
        delete host
    }
}

def private batch_gather_llama(t : T?; tr : Model) {
    batch_gather(t, tr, LLAMA_FILE)
}

def private batch_gather_eseries(t : T?; tr : Model) {
    batch_gather(t, tr, E2B_FILE)
}




def private batched_row_shexp(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        cut_within(t, "", tr, 4l)
        let r = batched_row_run(tr)
        t |> equal(r.why, "", "the row is served: {r.why}")
        t |> success(r.tps > 0.0lf, "... at a rate ({r.tps:.1f} tok/s summed over 4 streams)")
        t |> success(r.device_steps >= 8l, "the timed steps went through the device batched driver ({r.device_steps} device steps)")
        t |> equal(r.cpu_stack_steps, 0l, "the blob-only model never reached the CPU batched stack")
    }
}

//! the rows and the single GEMV forms round the router's logits apart and a near-tie flips an expert: the plain-MoE bar of
//! the q30 MTP verify row (`mtp_verify_qwen3_30b_a3b_cut`: 1.6e-3 at most on the M1 Max, the one-step-off control 9.7 at least)
let private SHEXP_SAMESLAB_BAR = 2.0
let private SHEXP_SAMESLAB_ROUNDS = 12l

def private batched_row_shexp_sameslab(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        cut_within(t, "", tr, 4l)
        let c = tr.config
        let vocab = c.vocab_size
        var prompt <- encode(tr, "The capital of France is Paris, the capital of Italy is", true, false)
        var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
        var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
        let floor_before = get_metal_prefill_min_npos()
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
        eval_(tr, sa, prompt)
        eval_(tr, sb, prompt)
        select_prefill_override("")
        set_metal_prefill_min_npos(floor_before)
        select_decode_override("metal")
        var ws <- create_batch_workspace_(tr)
        let before = metal_batch_decode_stats().steps
        var one <- [parallel_argmax(sa.logits, vocab)]
        eval_(tr, sa, one)   // the mirror-building step on both
        eval_(tr, sb, one)
        finish_pending_step()
        var prev := sa.logits
        var maxd = 0.0
        var off_min = 1e30
        var finite = true
        var served_rounds = 0l
        for (r in range64(SHEXP_SAMESLAB_ROUNDS)) {
            one[0] = parallel_argmax(sa.logits, vocab)
            eval_(tr, sa, one)
            finish_pending_step()
            let pos = sb.n_past
            if (!verify_batch_step(tr, ws, sb, one, pos, 1l)) {
                t |> failure("round {r}: the same-slab verify declined one row")
                break
            }
            served_rounds++
            sb.n_past = pos + 1l
            var d = 0.0
            var doff = 0.0
            for (v in range64(vocab)) {
                d = max(d, nan_wide_diff(sa.logits[v], sb.mtp_logits_b[v]))
                doff = max(doff, nan_wide_diff(prev[v], sb.mtp_logits_b[v]))
                finite = finite && is_finite(prev[v]) && is_finite(sb.mtp_logits_b[v])
            }
            maxd = max(maxd, d)
            off_min = min(off_min, doff)
            let single_top = parallel_argmax(sa.logits, vocab)
            let verify_top = parallel_argmax(sb.mtp_logits_b, vocab)
            to_log(LOG_INFO, "shexp same-slab round {r} pos={pos} maxd={d} off={doff} single_top={single_top} '{eyeball_text(tr, [single_top])}' verify_top={verify_top} '{eyeball_text(tr, [verify_top])}'\n")
            prev := sa.logits
        }
        select_decode_override("")
        t |> equal(served_rounds, SHEXP_SAMESLAB_ROUNDS, "every round's one-row verify served")
        t |> success(metal_batch_decode_stats().steps - before >= SHEXP_SAMESLAB_ROUNDS, "the batched driver stepped every round ({metal_batch_decode_stats().steps - before} steps)")
        t |> success(maxd <= SHEXP_SAMESLAB_BAR, "the one-row verify's logits sit within {SHEXP_SAMESLAB_BAR} of the single-row step's at every round (widest {maxd})")
        t |> success(finite, "both steps' logits are finite at every round before the one-step-off control")
        t |> success(off_min > SHEXP_SAMESLAB_BAR, "the one-step-off control lands outside the bar at every round (nearest {off_min})")
        delete ws
        delete one
        delete prev
        delete sa
        delete sb
        delete prompt
    }
}

def private batched_row_nextn_plain(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(false)
        let census0 = batch_step_census()
        let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
        set_mtp_spec(prev_spec)
        t |> equal(served.why, "", "the row is served: {served.why}")
        t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
        let census1 = batch_step_census()
        t |> success(census1.device - census0.device >= 8l, "the timed steps rode the batched driver ({census1.device - census0.device} device steps)")
        t |> equal(census1.cpu_stack, census0.cpu_stack, "no step reached the CPU batched stack (the blob-only classifier tail would panic)")
        // the carry: after plain batched steps a stream's speculative round starts warm
        var sch <- create_scheduler(tr, 2l, 2l, 32l, 0l, 0l, KVDtype.f16, false)
        var events : array<SchedEvent>
        for (i in range64(2l)) {
            var req <- PendingReq(id = i + 1l, prompt <- bench_prompt(32l, i + 1l), max_new = 32l)
            verify(submit(tr, sch, req), "the queue holds the two streams")
        }
        for (_i in range64(10l)) {   // two prefill quanta, then batched decode steps
            scheduler_step(tr, sch, events)
            delete events
        }
        for (st in sch.streams) {
            //! the encode-ahead pipeline lands a step at the next step's start, so the carry trails n_past by at most one
            t |> success(st.session.mtp_h_pos1 >= st.session.n_past - 1l && st.session.n_past > 36l, "stream {st.id}: the plain batched steps landed the row's post-norm hidden as the carry (h_pos1 {st.session.mtp_h_pos1}, n_past {st.session.n_past})")
        }
        let ids <- [for (st in sch.streams); st.id]
        for (id in ids) {
            evict(sch, id, events)
        }
        scheduler_step(tr, sch, events)
        delete events
        delete sch
    }
}

def private batched_row_mtp(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        let census0 = batch_step_census()
        let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l, true)
        set_mtp_spec(prev_spec)
        t |> success(find(served.why, "one at a time") >= 0, "the row refuses by name - a speculative tick runs each stream's round alone: {served.why}")
        t |> equal(served.tps, 0.0lf, "... and reads no rate")
        let census1 = batch_step_census()
        t |> equal(census1.device - census0.device, 0l, "no tick counted as a batched device step ({census1.device - census0.device})")
        t |> equal(census1.cpu_stack, census0.cpu_stack, "no tick reached the CPU batched stack")
    }
}

def private batched_row_mtp_depth2(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var prompt <- encode(tr, "Count from one to twenty: one, two, three,", true, false)
        let total = long_length(prompt) - 1l + 24l
        var toks : array<array<int64>>
        var drafted, accepted : array<int64>
        toks |> reserve(2)
        drafted |> reserve(2)
        accepted |> reserve(2)
        for (k in range64(1l, 3l)) {   //! the scheduler's round is depth 1 by contract; the depth-k entry is the greedy walk's
            set_mtp_depth(k)
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var nd = 0l
            var na = 0l
            toks |> emplace(generate_mtp_greedy(tr, s, prompt, total, nd, na))
            drafted |> push(nd)
            accepted |> push(na)
            delete s
        }
        clear_mtp_depth()
        log_gen_texts(tr, "depth 1, depth 2", toks[0], toks[1])
        t |> success(ids_equal(toks[1], toks[0]), "depth 2 tokens == depth 1 tokens")
        t |> success(drafted[1] > drafted[0], "the depth-2 rounds drafted more than the depth-1 rounds over the same reply ({drafted[1]} against {drafted[0]}): the second draft is made")
        t |> success(accepted[1] > 0l, "depth 2 accepted drafts ({accepted[1]} of {drafted[1]})")
        delete toks
        delete drafted
        delete accepted
        delete prompt
    }
}

def private pick_ask(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(false)   //! the plain step's pick alone: a round's picks are the verify seat's
        var prompt <- encode(tr, "Count from one to twenty: one, two, three,", true, false)
        var none : array<int64>
        let greedy = SamplingParams()
        var ref : array<int64>
        var rs <- create_session(tr)
        generate(tr, rs, prompt, greedy, 20l) $(id, _pc) {
            ref |> push(id)
            return true
        }
        delete rs
        let picks0 = metal_picks_landed()
        var sch <- create_scheduler(tr, 1l, 4l, 64l, 0l, 0l, KVDtype.f16, false)
        var events : array<SchedEvent>
        t |> success(submit(tr, sch, make_req(1l, prompt, greedy, 20l, none)), "greedy stream queued")
        drain(tr, sch, events)
        let landed = metal_picks_landed() - picks0
        var got <- toks_of(events, 1l)
        log_gen_texts(tr, "pick ask (generate, scheduler)", ref, got)
        t |> success(ids_equal(got, ref), "the greedy stream's tokens == generate()'s, which reads its rows on the host")
        t |> success(landed > 0l && landed <= long_length(got), "the stream's steps landed their picks ({landed} picks over {length(got)} tokens)")
        delete sch
        delete events
        delete got
        let picks1 = metal_picks_landed()
        var sch2 <- create_scheduler(tr, 1l, 4l, 64l, 0l, 0l, KVDtype.f16, false)
        var events2 : array<SchedEvent>
        t |> success(submit(tr, sch2, make_req(2l, prompt, SamplingParams(temp = 0.7, top_k = 1l, penalty = 1.1), 20l, none)), "sampling stream queued")
        drain(tr, sch2, events2)
        t |> equal(metal_picks_landed() - picks1, 0l, "a stream whose sampler is not a bare argmax lands no pick (the control)")
        set_mtp_spec(prev_spec)
        delete sch2
        delete events2
        delete ref
        delete prompt
    }
}

def private batched_row_mtp_joint_invariance(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                         <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                         <- encode(tr, "List the days of the week: Monday, Tuesday,", true, false)]
        var none : array<int64>
        let greedy = SamplingParams()
        // each stream alone: its rounds verify two rows through the same rows forms
        var refs : array<array<int64>>
        refs |> reserve(3)
        var solo_drafted = 0l
        var solo_accepted = 0l
        for (pi in range64(3l)) {
            var sch0 <- create_scheduler(tr, 1l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
            var events0 : array<SchedEvent>
            t |> success(submit(tr, sch0, make_req(pi + 1l, prompts[pi], greedy, 20l, none)), "solo stream {pi + 1l} queued")
            drain(tr, sch0, events0)
            refs |> emplace(toks_of(events0, pi + 1l))
            solo_drafted += sch0.mtp_drafted
            solo_accepted += sch0.mtp_accepted
            delete sch0
            delete events0
        }
        t |> success(solo_accepted * 2l > solo_drafted, "the solo rounds accept more than half their drafts ({solo_accepted} of {solo_drafted})")
        var sch <- create_scheduler(tr, 3l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
        var events : array<SchedEvent>
        for (pi in range64(3l)) {
            t |> success(submit(tr, sch, make_req(10l + pi, prompts[pi], greedy, 20l, none)), "joint stream {10l + pi} queued")
        }
        let census0 = batch_step_census()
        drain(tr, sch, events)
        let census1 = batch_step_census()
        set_mtp_spec(prev_spec)
        t |> equal(sch.mtp_joined_max, 0l, "every tick steps its streams through their own rounds (the widest tick carried {sch.mtp_joined_max})")
        for (pi in range64(3l)) {
            var got <- toks_of(events, 10l + pi)
            log_gen_texts(tr, "joint stream {pi} (solo, joint)", refs[pi], got)
            t |> success(ids_equal(got, refs[pi]), "joint stream {pi} tokens == its solo stream")
            delete got
        }
        t |> success(sch.mtp_drafted > 0l, "the joint ticks drafted ({sch.mtp_drafted} drafts)")
        t |> equal(sch.mtp_drafted, solo_drafted, "the joint ticks drafted what the three solo streams drafted together ({sch.mtp_drafted} against {solo_drafted})")
        t |> equal(sch.mtp_accepted, solo_accepted, "... and accepted the same drafts ({sch.mtp_accepted} against {solo_accepted})")
        t |> equal(census1.device - census0.device, 0l, "a tick of three speculating streams is no batched step: each stream runs its own round ({census1.device - census0.device} device steps)")
        delete sch
        delete events
        delete refs
        delete prompts
    }
}

def private batched_row_mtp_joint_staggered(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        //! group 0 is shallow, the LAST group deep: the round's chunk count and its attention form must follow the deepest row, not the first group's
        let fox = repeat("The quick brown fox jumps over the lazy dog while the farmer counts his sheep one by one in the field. ", 12)
        let deep = "{fox}Count from one to twenty: one, two, three,"
        var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                         <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                         <- encode(tr, deep, true, false)]
        t |> success(long_length(prompts[2]) > 200l, "the deep stream's prompt crosses three chunk boundaries ({long_length(prompts[2])} tokens)")
        var none : array<int64>
        let greedy = SamplingParams()
        var refs : array<array<int64>>
        refs |> reserve(3)
        for (pi in range64(3l)) {
            var sch0 <- create_scheduler(tr, 1l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
            var events0 : array<SchedEvent>
            t |> success(submit(tr, sch0, make_req(pi + 1l, prompts[pi], greedy, 24l, none)), "solo stream {pi + 1l} queued")
            drain(tr, sch0, events0)
            refs |> emplace(toks_of(events0, pi + 1l))
            delete sch0
            delete events0
        }
        var sch <- create_scheduler(tr, 3l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
        var events : array<SchedEvent>
        for (pi in range64(3l)) {
            t |> success(submit(tr, sch, make_req(10l + pi, prompts[pi], greedy, 24l, none)), "joint stream {10l + pi} queued")
        }
        drain(tr, sch, events)
        set_mtp_spec(prev_spec)
        t |> equal(sch.mtp_joined_max, 0l, "every tick steps its streams through their own rounds (the widest tick carried {sch.mtp_joined_max})")
        for (pi in range64(3l)) {
            var got <- toks_of(events, 10l + pi)
            log_gen_texts(tr, "staggered stream {pi} (solo, joint)", refs[pi], got)
            t |> success(ids_equal(got, refs[pi]), "staggered stream {pi} tokens == its solo stream")
            delete got
        }
        delete sch
        delete events
        delete refs
        delete prompts
    }
}

//! the recurrent scan's dispatches on both stamps since the census reset; 0 before the drivers compiled them
//! the recurrent stage's dispatches: every stamp of the scan template (`MetalDnScanT`, the f32 and half forms) and the fat
//! chain's fused step (`MetalDnStepFused`: conv, scan and gate as one dispatch over the step's groups)
def private dn_scan_dispatches() : int64 => metal_kernel_count_prefix("MetalDnScan") + metal_kernel_count_prefix("MetalDnStepFused")

def private batched_row_deltanet(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let census0 = batch_step_census()
        let scans0 = dn_scan_dispatches()
        let calls0 = metal_decode_stats().decodes + metal_prefill_stats().prefills
        let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
        t |> equal(served.why, "", "the row is served: {served.why}")
        t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
        let census1 = batch_step_census()
        t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
        t |> equal(census1.cpu_stack, census0.cpu_stack, "the hybrid never reached the CPU batched stack")
        //! the recurrent stages run once a layer a step over every row (`ARCHITECTURE_GPU_MTP.md#dn-group-stages`): a
        //! row-at-a-time step would scan four times a layer
        let nrec = recurrent_layer_count(tr.config)
        let calls = census1.device - census0.device + metal_decode_stats().decodes + metal_prefill_stats().prefills - calls0
        t |> equal(dn_scan_dispatches() - scans0, calls * nrec, "one scan dispatch a recurrent layer a device call ({calls} calls, {nrec} recurrent layers)")
        // nine streams: more rows than the mirror cache's resting size
        let wide = bench_tg_batched_rep(tr, 9l, 32l, 8l, 2l)
        t |> equal(wide.why, "", "the nine-stream row is served: {wide.why}")
        t |> success(wide.tps > 0.0lf, "... at a rate ({wide.tps:.1f} tok/s summed over 9 streams)")
        let census2 = batch_step_census()
        t |> success(census2.device - census1.device >= 8l, "its timed steps went through the device batched driver ({census2.device - census1.device} device steps)")
    }
}

let private PAGED_PROMPT = 40l
let private PAGED_NEW = 40l   //! prompt plus reply is 80 rows: past one 64-row page, so a stream's rows span two pages

//! every stream's greedy tokens after `nst` requests of `nnew` tokens each run to the end on a scheduler of `page_rows`-row pages (0 = flat sessions)
[cold_path]
def private served_tokens(tr : Model; nst, page_rows, nnew : int64) : array<array<int64>> {
    var sch <- create_scheduler(tr, nst, nst, 64l, page_rows, 0l, KVDtype.f16, false)
    var events : array<SchedEvent>
    for (i in range64(nst)) {
        var req <- PendingReq(id = i + 1l, prompt <- bench_prompt(PAGED_PROMPT, i + 1l), max_new = nnew)
        verify(submit(tr, sch, req), "the queue holds the streams")
    }
    drain(tr, sch, events)
    var toks <- [for (i in range64(nst)); toks_of(events, i + 1l)]
    delete events
    delete sch
    return <- toks
}

// Both sides run `served_tokens` on the same blob model through the same Metal drivers, and the
// mirror gathers a paged session's pool rows byte for byte, so the two streams are one code path
// over one set of bytes: the compare is exact. Each side's driver steps are counted on their own.
def private paged_deltanet(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (nst in [1l, 2l]) {
            let steps0 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
            let segw0 = metal_prefill_segment_windows()
            var flat <- served_tokens(tr, nst, 0l, PAGED_NEW)
            let steps1 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
            var paged <- served_tokens(tr, nst, 64l, PAGED_NEW)
            let steps2 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
            //! two streams submitted together prefill as one window on the flat and the paged scheduler alike
            t |> equal(metal_prefill_segment_windows() - segw0, nst == 2l ? 2l : 0l, "{nst} stream(s): the prompts rode {nst == 2l ? "one window a scheduler" : "windows of their own"}")
            var first <- served_tokens(tr, nst, 64l, 1l)   // the control: a reply of one token takes it off the prefill
            let steps3 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
            to_log(LOG_INFO, "{nst} stream(s): Metal decode steps - flat {steps1 - steps0}, paged {steps2 - steps1}, a one-token reply {steps3 - steps2}\n")
            t |> success(steps1 - steps0 >= PAGED_NEW - 1l, "{nst} stream(s): the Metal drivers served the flat sessions' steps ({steps1 - steps0})")
            t |> success(steps2 - steps1 >= PAGED_NEW - 1l, "{nst} stream(s): the Metal drivers served the paged sessions' steps ({steps2 - steps1})")
            t |> success(steps3 - steps2 < PAGED_NEW - 1l, "{nst} stream(s): a one-token reply stays under the floor ({steps3 - steps2} steps)")
            for (i in range64(nst)) {
                t |> equal(long_length(flat[i]), PAGED_NEW, "{nst} stream(s), stream {i + 1l}: the flat session emitted every token")
                var diverged = empty(flat[i])
                for (a in flat[i]) {
                    diverged ||= a != flat[i][0]
                }
                to_log(LOG_INFO, "{nst} stream(s), stream {i + 1l}, flat text: {eyeball_text(tr, flat[i])}\n")
                to_log(LOG_INFO, "{nst} stream(s), stream {i + 1l}, paged text: {eyeball_text(tr, paged[i])}\n")
                t |> success(ids_equal(flat[i], paged[i]), "{nst} stream(s), stream {i + 1l}: the paged session's tokens are the flat session's")
                t |> success(diverged, "{nst} stream(s), stream {i + 1l}: the stream is more than one token repeated, so a dead session would show")
            }
            delete flat
            delete paged
            delete first
        }
    }
}

let private SEG_DN_NEW = 24l   //! the greedy steps after a segmented window: enough for a divergent state to show as text

//! Two sessions' tails as ONE window on the deltanet hybrid: each recurrent layer walks a segment's rows against that
//! session's own state and conv history, so the steps after it read what the single-window session's read.
def private segments_deltanet(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let pa <- encode(tr, "The quick brown fox jumps over the lazy dog while the farmer counts his sheep one by one, and then the", true, false)
        let pb <- encode(tr, "In the beginning there was a small village by the river, where every morning", true, false)
        let ca = long_length(pa) * 2l / 3l
        let cb = long_length(pb) / 2l
        var a1 <- [for (i in range64(ca)); pa[i]]
        var a2 <- [for (i in range64(ca, long_length(pa))); pa[i]]
        var b1 <- [for (i in range64(cb)); pb[i]]
        var b2 <- [for (i in range64(cb, long_length(pb))); pb[i]]
        var ra <- two_window_greedy(tr, a1, a2, SEG_DN_NEW)
        var rb <- two_window_greedy(tr, b1, b2, SEG_DN_NEW)
        var sa = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        var sb = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        eval_(tr, sa, a1)
        eval_(tr, sb, b1)
        segments_window(t, tr, sa, sb, a2, b2, "the deltanet tails")
        let dec0 = metal_decode_stats().decodes
        var ga2 <- greedy_from(tr, sa, long_length(pa), SEG_DN_NEW)
        var gb2 <- greedy_from(tr, sb, long_length(pb), SEG_DN_NEW)
        t |> equal(metal_decode_stats().decodes - dec0, 2l * SEG_DN_NEW, "the greedy steps were the device's")
        to_log(LOG_INFO, "segments first, single windows: {eyeball_text(tr, ra.ids)}\n")
        to_log(LOG_INFO, "segments first, one window:     {eyeball_text(tr, ga2)}\n")
        to_log(LOG_INFO, "segments second, single windows: {eyeball_text(tr, rb.ids)}\n")
        to_log(LOG_INFO, "segments second, one window:     {eyeball_text(tr, gb2)}\n")
        t |> success(ids_equal(ra.ids, ga2), "the first session's {SEG_DN_NEW} greedy steps after the segmented window are its single windows'")
        t |> success(ids_equal(rb.ids, gb2), "the second session's {SEG_DN_NEW} greedy steps after the segmented window are its single windows'")
        t |> success(!ids_equal(ra.ids, rb.ids), "the two sessions' streams differ, so a crossed state would show")
        delete ra
        delete rb
        delete sa
        delete sb
        delete ga2
        delete gb2
        delete a1
        delete a2
        delete b1
        delete b2
    }
}

//! A recurrent mirror evicted between two steps leaves its live state in a host stash, and the session's next step
//! rebuilds the device state from it (`ARCHITECTURE_GPU_MTP.md#dn-mirror-pool`): on a blob-only load there is no CPU
//! chain to fall to, so the steps after the eviction must serve on the device and read what an unevicted session reads.
def private deltanet_evicted_mirror(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let pa <- encode(tr, "The quick brown fox jumps over the lazy dog while the farmer counts his sheep one by one, and then the", true, false)
        let np = long_length(pa)
        var sa = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        var sb = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        eval_(tr, sa, pa)
        eval_(tr, sb, pa)
        var ref <- greedy_from(tr, sa, np, 12l)
        var head <- greedy_from(tr, sb, np, 4l)
        let rest0 = dn_mirror_restores()
        let dec0 = metal_decode_stats().decodes
        dn_mirror_evict_now(sb.uid)
        var tail <- greedy_from(tr, sb, np + 4l, 8l)
        t |> equal(dn_mirror_restores() - rest0, 1l, "the step after the eviction rebuilt the mirror from the stash")
        t |> equal(metal_decode_stats().decodes - dec0, 8l, "every step after the eviction was the device's")
        var got <- [for (i in range64(12l)); i < 4l ? head[i] : tail[i - 4l]]
        to_log(LOG_INFO, "unevicted: {eyeball_text(tr, ref)}\n")
        to_log(LOG_INFO, "evicted:   {eyeball_text(tr, got)}\n")
        t |> success(ids_equal(ref, got), "the evicted session's 12 greedy steps are the unevicted session's")
        delete ref
        delete head
        delete tail
        delete got
        delete sa
        delete sb
    }
}

//! A released session's recurrent mirror and stash leave the pool (`ARCHITECTURE_GPU_MTP.md#dn-mirror-pool`), and a live
//! session's evicted state stays in its stash however many sessions step in turn: on a blob-only load a lost state has no
//! CPU chain to fall to, so a dropped live entry would decline `dn_state` and panic.
def private deltanet_released_sessions(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let n = 2l * dn_mirrors_cap() + 1l
        let steps = 6l
        var prompts : array<array<int64>>
        prompts |> reserve(n)
        for (i in range64(n)) {
            var p <- encode(tr, "Session {i} counts slowly: {i}, {i + 1l}, {i + 2l}, {i + 3l}, and then", true, false)
            prompts |> emplace(p)
        }
        var refs : array<array<int64>>
        refs |> reserve(n)
        for (i in range64(n)) {
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, s, prompts[i])
            var r <- greedy_from(tr, s, long_length(prompts[i]), steps)
            refs |> emplace(r)
            let uid = s.uid
            t |> success(dn_mirror_holds(uid), "session {i}'s state is held while it lives")
            delete s
            t |> success(!dn_mirror_holds(uid), "session {i}'s mirror and stash left with its release")
        }
        var inscope live : array<Session>
        live |> reserve(n)
        for (i in range64(n)) {
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, s, prompts[i])
            live |> emplace(s)
        }
        var got : array<array<int64>>
        got |> resize(n)
        for (g in got) {
            g |> resize(steps)
        }
        let rest0 = dn_mirror_restores()
        let dec0 = metal_decode_stats().decodes
        for (k in range64(steps)) {
            for (i in range64(n)) {
                let next = argmax(live[i], tr.config.vocab_size)
                got[i][k] = next
                forward(tr, live[i], next, long_length(prompts[i]) + k)
            }
        }
        t |> equal(metal_decode_stats().decodes - dec0, n * steps, "every step of {n} sessions in turn was the device's")
        t |> success(dn_mirror_restores() - rest0 >= n * (steps - 1l), "the sessions past the cap rebuilt their mirrors from their stashes")
        for (i in range64(n)) {
            to_log(LOG_INFO, "session {i} alone:   {eyeball_text(tr, refs[i])}\n")
            to_log(LOG_INFO, "session {i} in turn: {eyeball_text(tr, got[i])}\n")
            t |> success(ids_equal(refs[i], got[i]), "session {i}'s {steps} steps in turn are its steps alone")
        }
        let uid0 = live[0].uid
        delete live
        t |> success(!dn_mirror_holds(uid0), "the live sessions' states left with their release")
        delete prompts
        delete refs
        delete got
    }
}

//! `n` steps on `s` from `pos` fed the greedy pick except at every `miss_every`-th step (0 = none), which feeds the pick's
//! next id instead: the fed tokens, and every step's logits concatenated
def private fed_steps(tr : Model; var s : Session; pos, n, miss_every : int64; var logits : array<float>) : array<int64> {
    var ids : array<int64>
    ids |> reserve(n)
    for (i in range64(n)) {
        let pick = argmax(s, tr.config.vocab_size)
        let next = miss_every > 0l && (i + 1l) % miss_every == 0l ? (pick + 1l) % tr.config.vocab_size : pick
        ids |> push(next)
        forward(tr, s, next, pos + i)
        logits |> push_from(s.logits)
    }
    return <- ids
}

//! The single row's greedy chain on a recurrent hybrid: the chained step reads the recurrent region the step in flight writes and
//! writes the other, so a step the chain mispredicted leaves the live state whole (`ARCHITECTURE_GPU_MTP_DECODE_PICK.md#greedy-chain`)
def private deltanet_greedy_chain(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_spec = metal_decode_spec_mode()
        let pa <- encode(tr, "The quick brown fox jumps over the lazy dog while the farmer counts his sheep one by one, and then the", true, false)
        let np = long_length(pa)
        for (miss_every in [0l, 3l]) {
            var inscope plain_l, chain_l : array<float>
            set_metal_decode_speculate(0)
            var sa = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sa, pa)
            var plain <- fed_steps(tr, sa, np, CHAIN_STEPS, miss_every, plain_l)
            set_metal_decode_speculate(1)
            let sp0 = metal_decode_spec_stats()
            let dec0 = metal_decode_stats().decodes
            var sb = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sb, pa)
            var chain <- fed_steps(tr, sb, np, CHAIN_STEPS, miss_every, chain_l)
            let sp1 = metal_decode_spec_stats()
            let hits = sp1.hits - sp0.hits
            let misses = sp1.misses - sp0.misses
            let tag = miss_every == 0l ? "greedy" : "fed off the pick every {miss_every} steps"
            to_log(LOG_INFO, "chain {tag}: plain {eyeball_text(tr, plain)}\n")
            to_log(LOG_INFO, "chain {tag}: chain {eyeball_text(tr, chain)}\n")
            t |> equal(metal_decode_stats().decodes - dec0, CHAIN_STEPS, "{tag}: every step the device's")
            t |> success(hits > 0l, "{tag}: the chain served steps on the hybrid ({hits} hits, {misses} misses)")
            if (miss_every > 0l) {
                t |> success(misses > 0l, "{tag}: the off-pick feeds mispredicted chained steps ({misses})")
            } else {
                t |> equal(misses, 0l, "{tag}: a greedy feed mispredicts nothing")
            }
            t |> success(ids_equal(plain, chain), "{tag}: the chained run's tokens are the plain run's")
            t |> equal(logits_maxdiff(plain_l, chain_l), 0.0, "{tag}: every step's logits are the plain step's to the bit")
            delete plain
            delete chain
            delete sa
            delete sb
        }
        set_metal_decode_speculate(prior_spec)
    }
}

let private CHAIN_STEPS = 24l   //! the steps a chain cell feeds

let private PAGED_MTP_NEW = 150l   //! past two 64-row pages, so rounds land on a page's last row

//! A round verifies the row past its token, so a round at a page's last row writes the next page's first.
def private paged_mtp(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        var prompt <- encode(tr, "Count from one to one hundred: one, two, three,", true, false)
        var none : array<int64>
        let greedy = SamplingParams()
        var toks : array<array<int64>>
        toks |> reserve(2)
        t |> equal(chunk_defaults(tr).idle, GPU_IDLE_CHUNK_TOKENS, "a Metal-served model's idle prefill quantum is the wide one")
        t |> equal(chunk_defaults(tr).chunk, DEFAULT_CHUNK_TOKENS, "and its chunk beside a decoding stream is one window")
        for (page_rows in [0l, 64l]) {
            let kind = page_rows == 0l ? "flat" : "paged"
            var sch <- create_scheduler(tr, 1l, 4l, 64l, page_rows, 0l, KVDtype.f16, true)
            var events : array<SchedEvent>
            t |> success(submit(tr, sch, make_req(1l, prompt, greedy, PAGED_MTP_NEW, none)), "{kind} stream queued")
            drain(tr, sch, events)
            toks |> emplace(toks_of(events, 1l))
            t |> equal(long_length(toks[length(toks) - 1]), PAGED_MTP_NEW, "{kind}: the stream emitted every token")
            t |> success(sch.mtp_accepted > 0l, "{kind}: the rounds drafted and accepted ({sch.mtp_accepted} of {sch.mtp_drafted}) - a plain fallback would pass vacuously")
            delete sch
            delete events
        }
        set_mtp_spec(prev_spec)
        t |> success(long_length(prompt) + PAGED_MTP_NEW > 128l, "the stream's rows span three pages ({long_length(prompt)} prompt tokens)")
        log_gen_texts(tr, "speculative stream (flat, paged)", toks[0], toks[1])
        t |> success(ids_equal(toks[0], toks[1]), "the paged stream's tokens are the flat stream's")
        delete toks
        delete prompt
    }
}

struct private Served {
    toks : array<int64>
    cached : int64
}

//! one greedy request through `sch`, alone: its reply and the tokens its admit attached
def private serve_one(t : T?; tr : Model; var sch : Scheduler; id : int64; prompt : array<int64>; max_new : int64; stable_at : int64 = 0l) : Served {
    var events : array<SchedEvent>
    var none : array<int64>
    var req <- make_req(id, prompt, SamplingParams(), max_new, none)
    req.stable_at = stable_at
    t |> success(submit(tr, sch, req), "request {id} queued")
    drain(tr, sch, events)
    var res <- Served(toks <- toks_of(events, id), cached = finished_of(events, id).n_cached)
    delete events
    return <- res
}

def private opening_of(a, b : array<int64>) : int64 {
    var n = 0l
    while (n < long_length(a) && n < long_length(b) && a[n] == b[n]) {
        n ++
    }
    return n
}

let private CKPT_NEW = 24l

//! A recurrent model's prefix cache is a checkpoint at one exact position: the second prompt with an opening leaves it, the third attaches it.
def private prefix_checkpoint(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        let opening = repeat("You are a careful assistant. Answer briefly and precisely. ", 12)
        var pa <- encode(tr, "{opening}Alpha: repeat the word apple five times:", true, false)
        var pb <- encode(tr, "{opening}Beta: count from one to ten: one, two,", true, false)
        var pc <- encode(tr, "{opening}Gamma: list the days of the week: Monday,", true, false)
        let cut = opening_of(pa, pb)
        t |> success(cut >= 128l && cut % 64l != 0l, "the prompts share {cut} tokens - past two 64-row pages and off a page boundary, so the attach copies a partial page")
        t |> equal(opening_of(pa, pc), cut, "the third prompt leaves the opening where the second did")
        var sch <- create_scheduler(tr, 1l, 4l, 64l, 64l, 0l, KVDtype.f16, true)
        var ra <- serve_one(t, tr, sch, 1l, pa, CKPT_NEW)
        var rb <- serve_one(t, tr, sch, 2l, pb, CKPT_NEW)
        let seam_declines = mtp_seam_declines()
        var rc <- serve_one(t, tr, sch, 3l, pc, CKPT_NEW)
        t |> equal(ra.cached, 0l, "the first prompt attaches nothing")
        t |> equal(rb.cached, 0l, "the second attaches nothing: no checkpoint stands at the opening yet")
        t |> equal(rc.cached, cut, "the third attaches the opening")
        t |> equal(mtp_seam_declines(), seam_declines, "the continuation window's seam row was written on the device, not declined")
        t |> equal(sch.prefix.n_state_hits, 1l, "one checkpoint hit")
        t |> success(sch.mtp_accepted > 0l, "the rounds drafted and accepted ({sch.mtp_accepted} of {sch.mtp_drafted})")
        // the reference prefills every prompt as [0, cut) and the rest, with no cache: the cached run's own cuts
        var ref_sch <- create_scheduler(tr, 1l, 4l, cut, 64l, 0l, KVDtype.f16, true, 0l, 0l)
        var xb <- serve_one(t, tr, ref_sch, 2l, pb, CKPT_NEW)
        var xc <- serve_one(t, tr, ref_sch, 3l, pc, CKPT_NEW)
        t |> equal(xc.cached, 0l, "the reference scheduler keeps no checkpoint")
        log_gen_texts(tr, "third prompt (whole, attached)", xc.toks, rc.toks)
        log_gen_texts(tr, "second prompt (whole, checkpointed)", xb.toks, rb.toks)
        t |> equal(long_length(rc.toks), CKPT_NEW, "the attached stream emitted every token")
        t |> success(ids_equal(xc.toks, rc.toks), "the attached stream's tokens are the whole prefill's")
        t |> success(ids_equal(xb.toks, rb.toks), "the stream that left the checkpoint emits what it emits with no cache")
        t |> success(!ids_equal(rb.toks, rc.toks), "the second and third replies differ, so a replayed session would show")
        let chunk = cut * 3l / 4l
        var near <- create_scheduler(tr, 1l, 4l, chunk, 64l, 0l, KVDtype.f16, true, 0l)
        var events : array<SchedEvent>
        var none : array<int64>
        var req <- make_req(7l, pb, SamplingParams(), CKPT_NEW, none)
        req.stable_at = cut
        t |> success(submit(tr, near, req), "the request marked at its opening queued")
        scheduler_step(tr, near, events)
        t |> equal(near.prefill_tokens, chunk, "the first window is the {chunk}-token chunk: a checkpoint {cut} tokens out no longer ends it")
        t |> equal(near.prefix.n_state_inserts, 0l, "... and no checkpoint stands yet")
        scheduler_step(tr, near, events)
        t |> success(near.prefix.n_state_inserts >= 1l, "the second window carried the stop at {cut} inside it and left its checkpoint ({near.prefix.n_state_inserts} standing)")
        drain(tr, near, events)
        to_log(LOG_INFO, "near: {near.prefix.n_state_inserts} checkpoints inserted over the prompt and the finished turn\n")
        t |> equal(near.prefix.n_state_inserts, 2l, "the opening's checkpoint stands, and the finished turn's after it")
        delete events
        delete near
        delete sch
        delete ref_sch
        delete ra
        delete rb
        delete rc
        delete xb
        delete xc
        delete pa
        delete pb
        delete pc
        set_mtp_spec(prev_spec)
    }
}

//! A caller that marks its prompt's stable opening gets the checkpoint from the first request on.
def private prefix_checkpoint_hinted(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        let opening = repeat("You are a careful assistant. Answer briefly and precisely. ", 12)
        var pa <- encode(tr, "{opening}Alpha: repeat the word apple five times:", true, false)
        var pc <- encode(tr, "{opening}Gamma: list the days of the week: Monday,", true, false)
        let cut = opening_of(pa, pc)
        var sch <- create_scheduler(tr, 1l, 4l, 64l, 64l, 0l, KVDtype.f16, true)
        var ra <- serve_one(t, tr, sch, 1l, pa, CKPT_NEW, cut)
        t |> equal(sch.prefix.n_state_inserts, 2l, "the first request leaves two checkpoints: its opening, and its finished turn")
        var rc <- serve_one(t, tr, sch, 2l, pc, CKPT_NEW, cut)
        t |> equal(rc.cached, cut, "the second prompt attaches the opening ({cut} tokens)")
        var plain <- create_scheduler(tr, 1l, 4l, 64l, 64l, 0l, KVDtype.f16, true)
        var rn <- serve_one(t, tr, plain, 1l, pa, CKPT_NEW)
        t |> equal(plain.prefix.n_state_inserts, 2l, "an unmarked first request leaves its prompt's end and its finished turn, no opening")
        var at_cut = 0
        for (st in plain.prefix.states) {
            if (long_length(st.tokens) == cut) {
                at_cut++
            }
        }
        t |> equal(0, at_cut, "none of the unmarked request's checkpoints stands at the opening ({cut} tokens)")
        var ref_sch <- create_scheduler(tr, 1l, 4l, cut, 64l, 0l, KVDtype.f16, true, 0l, 0l)
        var xc <- serve_one(t, tr, ref_sch, 2l, pc, CKPT_NEW)
        log_gen_texts(tr, "second prompt (whole, attached)", xc.toks, rc.toks)
        t |> success(ids_equal(xc.toks, rc.toks), "the attached stream's tokens are the whole prefill's at the same cut")
        delete sch
        delete plain
        delete ref_sch
        delete ra
        delete rc
        delete rn
        delete xc
        delete pa
        delete pc
        set_mtp_spec(prev_spec)
    }
}

//! The reply to a strongly prompted tail survives a lost recurrent state, so the checkpoint's contents are held on the logits.
def private prefix_checkpoint_logits(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        let opening = repeat("You are a careful assistant. Answer briefly and precisely. ", 12)
        var prompt <- encode(tr, "{opening}Gamma: list the days of the week: Monday,", true, false)
        let np = long_length(prompt)
        let cut = np - 11l
        t |> success(cut > 64l && cut % 64l != 0l, "the cut sits past one 64-row page and off a page boundary ({cut} of {np} tokens)")
        var head <- [for (i in range64(cut)); prompt[i]]
        var tail <- [for (i in range64(cut, np)); prompt[i]]
        var pool <- create_kv_pool(tr, 64l, KVDtype.f16)
        var cache <- create_prefix_cache()
        var donor <- create_session(tr, pool)
        eval(tr, donor, head)
        t |> equal(donor.mtp_h_pos1, cut, "the prefill left the draft head's carry at the cut")
        prefix_insert(cache, pool, donor, prompt)
        eval(tr, donor, tail)
        var warm <- create_session(tr, pool)
        t |> equal(prefix_attach(cache, pool, warm, prompt), cut, "the checkpoint attaches at the cut ({cut} of {np} tokens, off a page boundary)")
        t |> equal(warm.mtp_h_pos1, cut, "the attach restores the carry's position")
        t |> equal(length(warm.block_table), length(donor.block_table) - (np > (cut / 64l + 1l) * 64l ? 1 : 0), "the attached session holds the checkpoint's pages")
        t |> success(warm.block_table[length(warm.block_table) - 1] != donor.block_table[int(cut / 64l)], "the partial page is the attached session's own copy")
        eval(tr, warm, tail)
        to_log(LOG_INFO, "next token after the tail - donor: '{eyeball_text(tr, [argmax_of(donor.logits)])}' attached: '{eyeball_text(tr, [argmax_of(warm.logits)])}'\n")
        check_exact(t, "attached logits against the donor's", warm.logits, donor.logits)
        for (st in cache.states) {
            for (v in st.snap.state) {
                v = 0.0
            }
        }
        var blank <- create_session(tr, pool)
        t |> equal(prefix_attach(cache, pool, blank, prompt), cut, "the zeroed checkpoint attaches too")
        eval(tr, blank, tail)
        let off = logits_maxdiff(blank.logits, donor.logits)
        let top = logits_maxabs(donor.logits)
        to_log(LOG_INFO, "zeroed recurrent state: logits off by {off} of a largest {top}, next token '{eyeball_text(tr, [argmax_of(blank.logits)])}'\n")
        t |> success(off > 0.05 * top, "a checkpoint with its recurrent state zeroed lands other logits ({off} of {top})")
        release_kv_pages(donor)
        release_kv_pages(warm)
        release_kv_pages(blank)
        prefix_release(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "every page is back in the pool")
        delete donor
        delete warm
        delete blank
        delete cache
        delete pool
        delete head
        delete tail
        delete prompt
        set_mtp_spec(prev_spec)
    }
}

//! one request marked at `cut` through `sch`, its whole prompt one prefill window: the checkpoint at `cut` is the window's
//! capture
def private serve_marked_one_window(t : T?; tr : Model; var sch : Scheduler; prompt : array<int64>; cut : int64) {
    let np = long_length(prompt)
    t |> success(chunk_defaults(tr).idle >= np, "the idle quantum ({chunk_defaults(tr).idle}) takes the whole {np}-token prompt as one window")
    var events : array<SchedEvent>
    var none : array<int64>
    var req <- make_req(1l, prompt, SamplingParams(), CKPT_NEW, none)
    req.stable_at = cut
    t |> success(submit(tr, sch, req), "the request marked at {cut} queued")
    scheduler_step(tr, sch, events)
    t |> equal(sch.prefill_evals, 1l, "the prompt took one prefill call")
    t |> equal(sch.prefill_tokens, np, "... over the whole prompt, the stop at {cut} inside it")
    t |> equal(sch.prefix.n_state_inserts, 1l, "the window left the checkpoint at its stop: a capture")
    drain(tr, sch, events)
    var at_cut = 0
    for (st in sch.prefix.states) {
        if (long_length(st.tokens) == cut) {
            at_cut++
        }
    }
    t |> equal(at_cut, 1, "one checkpoint stands at the stop ({length(sch.prefix.states)} standing)")
    delete events
}

//! the capture cell's bar, of the largest logit: the capture stands inside a window of the whole prompt and the reference's
//! state at the end of a window of the opening alone, so the two scans and the GEMM tiles round apart - 0.055 % on the M5
//! Max, the prompt as one window 0.049 % off the reference, the capture with its state zeroed 17.5 %
let private CAPTURE_BAR_REL = 0.002

//! A checkpoint a stop inside one prefill window leaves is the arena's capture, not the window's end state, so its contents
//! are held on the logits.
def private prefix_checkpoint_capture_logits(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        let opening = repeat("You are a careful assistant. Answer briefly and precisely. ", 12)
        var prompt <- encode(tr, "{opening}Gamma: list the days of the week: Monday,", true, false)
        let np = long_length(prompt)
        let cut = np - 11l
        t |> success(cut > 128l && cut % 64l != 0l, "the stop sits past two 64-row pages and off a page boundary ({cut} of {np} tokens)")
        var head <- [for (i in range64(cut)); prompt[i]]
        var tail <- [for (i in range64(cut, np)); prompt[i]]
        var sch <- create_scheduler(tr, 1l, 4l, 64l, 64l, 0l, KVDtype.f16, true)
        serve_marked_one_window(t, tr, sch, prompt, cut)
        var cold <- create_session(tr, sch.pool)
        eval(tr, cold, head)
        eval(tr, cold, tail)
        var warm <- create_session(tr, sch.pool)
        t |> equal(prefix_attach(sch.prefix, sch.pool, warm, prompt), cut, "the capture attaches at the stop ({cut} of {np} tokens)")
        eval(tr, warm, tail)
        to_log(LOG_INFO, "next token after the tail - uncached: '{eyeball_text(tr, [argmax_of(cold.logits)])}' attached: '{eyeball_text(tr, [argmax_of(warm.logits)])}'\n")
        var whole <- create_session(tr, sch.pool)
        eval(tr, whole, prompt)
        let top = logits_maxabs(cold.logits)
        let bar = CAPTURE_BAR_REL * top
        let near = logits_maxdiff(warm.logits, cold.logits)
        to_log(LOG_INFO, "attached at the capture: logits off the uncached prefill's by {near} of a largest {top} (bar {bar}); the prompt as one window sits {logits_maxdiff(whole.logits, cold.logits)} off it\n")
        t |> success(near <= bar, "the logits attached at the capture sit within {bar} of the uncached prefill's ({near})")
        release_kv_pages(whole)
        delete whole
        for (st in sch.prefix.states) {
            if (long_length(st.tokens) == cut) {
                for (v in st.snap.state) {
                    v = 0.0
                }
            }
        }
        var blank <- create_session(tr, sch.pool)
        t |> equal(prefix_attach(sch.prefix, sch.pool, blank, prompt), cut, "the zeroed capture attaches too")
        eval(tr, blank, tail)
        t |> success(all_finite(blank.logits), "the zeroed capture's logits are finite")
        let off = logits_maxdiff(blank.logits, cold.logits)
        to_log(LOG_INFO, "zeroed captured state: logits off by {off} of a largest {top}, next token '{eyeball_text(tr, [argmax_of(blank.logits)])}'\n")
        t |> success(off > 0.05 * top, "a capture with its recurrent state zeroed lands logits past the bar ({off} of {top})")
        release_kv_pages(cold)
        release_kv_pages(warm)
        release_kv_pages(blank)
        delete cold
        delete warm
        delete blank
        delete sch
        delete head
        delete tail
        delete prompt
        set_mtp_spec(prev_spec)
    }
}

//! A finished turn leaves a checkpoint past its reply, so the conversation's next turn prefills only what is new.
def private prefix_checkpoint_next_turn(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_spec = get_mtp_spec()
        set_mtp_spec(true)
        var prompt <- encode(tr, "Count from one to one hundred: one, two, three,", true, false)
        var ref_sch <- create_scheduler(tr, 1l, 4l, 64l, 64l, 0l, KVDtype.f16, true, -1l, 0l)
        var whole <- serve_one(t, tr, ref_sch, 1l, prompt, 80l)
        t |> equal(long_length(whole.toks), 80l, "the uncached stream emitted every token")
        var sch <- create_scheduler(tr, 1l, 4l, 64l, 64l, 0l, KVDtype.f16, true)
        var first <- serve_one(t, tr, sch, 1l, prompt, 40l)
        var next : array<int64> := prompt
        next |> reserve(length(prompt) + 41)
        for (i in range(41)) {
            next |> push(whole.toks[i])
        }
        var second <- serve_one(t, tr, sch, 2l, next, 39l)
        var tail <- [for (i in range(41, 80)); whole.toks[i]]
        var head <- [for (i in range(40)); whole.toks[i]]
        log_gen_texts(tr, "the count's head (uncached, first turn)", head, first.toks)
        log_gen_texts(tr, "the count's tail (uncached, next turn)", tail, second.toks)
        t |> success(ids_equal(head, first.toks), "the first turn is the uncached stream's first 40 tokens")
        t |> equal(second.cached, long_length(prompt) + 40l, "the next turn attaches the finished turn: its prompt and its 40 tokens")
        t |> success(ids_equal(tail, second.toks), "the next turn continues the count token for token")
        delete sch
        delete ref_sch
        delete whole
        delete first
        delete second
        delete next
        delete tail
        delete head
        delete prompt
        set_mtp_spec(prev_spec)
    }
}

//! four greedy streams through the scheduler with the pre-encode rail off, then on: token for token, the on arm's steps
//! committed from the step pre-encoded under the one before
def private pre_encode(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_pre = metal_batch_pre_enabled()
        if (!env_is_set("DASLLAMA_METAL_BATCH_PRE")) {
            t |> success(!prior_pre, "the pre-encode rail ships off")
        }
        var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                         <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                         <- encode(tr, "List the days of the week: Monday, Tuesday,", true, false),
                         <- encode(tr, "The planets of the solar system, from the sun outward: Mercury, Venus,", true, false)]
        var none : array<int64>
        let greedy = SamplingParams()
        var refs : array<array<int64>>
        refs |> reserve(4)
        for (arm in range(2)) {
            let pre = arm == 1
            set_metal_batch_pre(pre)
            let taken0 = metal_batch_pre_taken()
            var sch <- create_scheduler(tr, 4l, 4l, 64l, 0l, 0l, KVDtype.f16, false)
            var events : array<SchedEvent>
            for (pi in range64(4l)) {
                t |> success(submit(tr, sch, make_req(pi + 1l, prompts[pi], greedy, 24l, none)), "stream {pi + 1l} queued (pre {pre})")
            }
            drain(tr, sch, events)
            let taken = metal_batch_pre_taken() - taken0
            if (!pre) {
                t |> equal(taken, 0l, "rail off: no step committed from a pre-encode")
                for (pi in range64(4l)) {
                    refs |> emplace(toks_of(events, pi + 1l))
                }
            } else {
                t |> success(taken >= 16l, "rail on: {taken} steps committed the step pre-encoded by the call before them")
                for (pi in range64(4l)) {
                    var got <- toks_of(events, pi + 1l)
                    log_gen_texts(tr, "stream {pi} (encoded at call, pre-encoded)", refs[pi], got)
                    t |> success(ids_equal(got, refs[pi]), "stream {pi}: the pre-encoded steps emit the reference tokens")
                    delete got
                }
            }
            delete sch
            delete events
        }
        //! the rail's retire under a PARKED step: the on arm leaves the next step pre-encoded; the lever's off retires it, the drivers' shutdown drops the rest, and the live-object count reads what either left behind
        set_metal_batch_pre(false)
        metal_decode_shutdown()
        metal_prefill_shutdown()
        t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after the parked pre-encoded step's retire and the drivers' shutdown")
        set_metal_batch_pre(prior_pre)
        delete refs
        delete prompts
    }
}

//! two greedy streams held by value in an array, as a scheduler holds its streams, through `eval_batch_`: a step over both, a
//! second after the array moved them, the first stream deleted and erased, then steps over the second alone. `pre` arms the
//! pre-encode rail and `pipe` the pipelined landing; the tokens of every step come back, the second stream's last
def private landing_walk(t : T?; tr : Model; prompts : array<array<int64>>; pre, pipe : bool) : array<int64> {   // nolint:LINT012,LINT019 — the body is the das_metal half
    var got : array<int64>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let greedy = SamplingParams()
        let tag = "pre {pre}, pipe {pipe}"
        set_metal_batch_pre(pre)
        set_metal_batch_pipe(pipe)
        var pool : array<Session>
        pool |> reserve(length(prompts))
        got |> reserve(8)
        for (pr in prompts) {
            var s <- create_session(tr)
            eval_(tr, s, pr)
            pool |> emplace(s)
        }
        var ws <- create_batch_workspace_(tr)
        var rows <- [unsafe(addr(pool[0])), unsafe(addr(pool[1]))]
        var toks <- [sample(pool[0], greedy), sample(pool[1], greedy)]
        got |> push_from(toks)
        let taken0 = metal_batch_pre_taken()
        eval_batch_(tr, ws, rows, toks)
        t |> equal(metal_batch_landing_parked(), pre || pipe, "{tag}: after the step a landing {pre || pipe ? "names" : "names no"} session")
        metal_decode_flush() if (pipe)   //! the pipelined rail's sampling contract: the step in flight lands before its logits are read
        //! the array moves its sessions with no release: the slots a step encoded ahead was built over hold moved-out sessions
        var moved : array<Session>
        moved |> reserve(2)
        for (i in range(2)) {
            moved |> emplace(pool[i])
        }
        rows[0] = unsafe(addr(moved[0]))
        rows[1] = unsafe(addr(moved[1]))
        for (i in range(2)) {
            toks[i] = sample(moved[i], greedy)
        }
        got |> push_from(toks)
        eval_batch_(tr, ws, rows, toks)
        if (pre && !pipe) {
            t |> equal(metal_batch_pre_taken() - taken0, 1l, "{tag}: the second step committed the step encoded ahead of the move")
            for (s in moved) {
                t |> equal(s.mtp_h_pos1, s.n_past, "{tag}: the landing moved the carry mark of the session where it stands now")
            }
        }
        //! the scheduler's reap: the session finalized in place, then the erase that moves the one after it
        let landed0 = metal_batch_decode_stats().steps
        delete moved[0]
        t |> success(!metal_batch_landing_parked(), "{tag}: a session's release leaves no landing that names a session")
        if (pipe) {
            t |> equal(metal_batch_decode_stats().steps - landed0, 1l, "{tag}: the release landed the step in flight")
        }
        moved |> erase(0)
        rows |> resize(1)
        rows[0] = unsafe(addr(moved[0]))
        toks |> resize(1)
        for (_ in range(3)) {
            toks[0] = sample(moved[0], greedy)
            got |> push(toks[0])
            eval_batch_(tr, ws, rows, toks)
            metal_decode_flush()
        }
        got |> push(sample(moved[0], greedy))
        rows |> clear()   //! borrowed pointers
        delete toks
        delete ws
        delete moved
        delete pool
    }
    return <- got
}

//! a landing never outlives the sessions it names: the walk with every step built at its call is the reference, and the
//! pre-encode rail and the pipelined landing each emit its tokens across a move of the sessions and a release of one
def private landing_release(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_pre = metal_batch_pre_enabled()
        let prior_pipe = metal_batch_pipe_enabled()
        set_metal_mtp_carry_hidden(true)   //! every landed row moves its session's carry mark: the write a stale pointer loses
        var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                         <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false)]
        var want <- landing_walk(t, tr, prompts, false, false)
        var pre <- landing_walk(t, tr, prompts, true, false)
        t |> success(ids_equal(pre, want), "the pre-encode rail emits the reference tokens across the move and the release: {pre} against {want}")
        var pipe <- landing_walk(t, tr, prompts, false, true)
        t |> success(ids_equal(pipe, want), "the pipelined landing emits the reference tokens across the move and the release: {pipe} against {want}")
        set_metal_mtp_carry_hidden(false)
        set_metal_batch_pipe(prior_pipe)
        set_metal_batch_pre(false)
        metal_decode_shutdown()
        metal_prefill_shutdown()
        t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after the walks and the drivers' shutdown")
        set_metal_batch_pre(prior_pre)
        delete pipe
        delete pre
        delete want
        delete prompts
    }
}

let private PAIR_STEPS = 12l

//! one f16 session's K/V mirror on `tr`: both planes of every layer that keeps rows of its own, a row each of its context
def private session_mirror_bytes(tr : Model) : int64 {
    var row_bytes = 0l
    for (l in range64(tr.config.n_layers)) {
        if (layer_kv_source(tr.config, l) == l) {
            row_bytes += 2l * kv_row_bytes(KVDtype.f16, layer_kv_dim(tr.config, l))
        }
    }
    return row_bytes * tr.config.seq_len
}

//! two sampled f16 streams over `prompts`, `PAIR_STEPS` steps each on the Metal decode override: each row samples right after
//! its own forward, or - `batched` - both rows step through one `eval_batch_` call and sample after it, the device's largest
//! buffer stood in for by `device_max` around the tick (0 = the device's own)
def private sampled_pair(tr : Model; prompts : array<array<int64>>; var sp : SamplingParams; batched : bool; device_max : uint64) : array<array<int64>> {   // nolint:LINT012,LINT019 — the body is the das_metal half
    var got : array<array<int64>>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var flat : array<int64>   //! step-major: both rows' tokens a step
        flat |> reserve(2l * PAIR_STEPS)
        var sa <- create_session(tr)
        var sb <- create_session(tr)
        set_seed(sa, 11)
        set_seed(sb, 22)
        eval_(tr, sa, prompts[0])
        eval_(tr, sb, prompts[1])
        sa.spec_params = unsafe(addr(sp))   //! the served stream's ask: each row's step lands its selection for the sampler
        sb.spec_params = unsafe(addr(sp))
        var ws <- create_batch_workspace_(tr)
        var both <- [unsafe(addr(sa)), unsafe(addr(sb))]
        var toks <- [sample(sa, sp), sample(sb, sp)]
        for (_ in range64(PAIR_STEPS)) {
            if (batched) {
                let prior_max = get_metal_mirror_device_max_probe_()
                set_metal_mirror_device_max_probe_(device_max)
                eval_batch_(tr, ws, both, toks)
                set_metal_mirror_device_max_probe_(prior_max)
                for (i in range(2)) {
                    toks[i] = sample(*both[i], sp)
                }
            } else {
                for (i in range(2)) {
                    forward(tr, *both[i], toks[i], both[i].n_past)
                    both[i].n_past++
                    toks[i] = sample(*both[i], sp)
                }
            }
            flat |> push_from(toks)
        }
        got <- [for (i in range64(2l)); [for (st in range64(PAIR_STEPS)); flat[st * 2l + i]]]
        sa.spec_params = null
        sb.spec_params = null
        both |> clear()   //! borrowed pointers: the sessions are this function's locals
        delete flat
        delete ws
        delete sa
        delete sb
    }
    return <- got
}

//! the selection landing is one a process: a two-row tick the batched driver declines (both rows' mirrors past the device's
//! largest buffer, stood in for by the probe seam at one and a half sessions' mirrors - one row's alone fits) steps its rows one
//! at a time (`batch_rows_one_at_a_time`) and lands each row's selection into its own logits before the next row's forward
//! overwrites it. Both sides run each row through the one `forward` entry point on the same session state, so their logits are
//! bit-identical and the streams token-exact; the device's own bound (the batched driver serves the tick) and the landing off
//! are the census's and the selection count's controls
def private per_row_tick_sampled(t : T?; tr : Model) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_landing = get_select_landing()
        set_select_landing(true)
        var prompts <- [ <- encode(tr, "Once upon a time", true, false),
                         <- encode(tr, "The capital of France is", true, false)]
        var sp = SamplingParams(temp = 0.8, top_k = 40l)
        let one = session_mirror_bytes(tr)
        let one_fits = uint64(one + one / 2l)
        var solo <- sampled_pair(tr, prompts, sp, false, 0ul)
        let census0 = batch_step_census()
        let sel0 = sel_selections()
        var rows <- sampled_pair(tr, prompts, sp, true, one_fits)
        let census1 = batch_step_census()
        let landed = sel_selections() - sel0
        t |> equal(census1.per_row - census0.per_row, PAIR_STEPS, "every two-row tick stepped its rows one at a time")
        t |> success(landed >= 2l * PAIR_STEPS, "every row's step landed its selection ({landed} landings over {2l * PAIR_STEPS} row steps)")
        for (i in range(2)) {
            log_gen_texts(tr, "stream {i} (the CPU line sampled after its own forward, the GPU line the per-row tick; both on Metal)", solo[i], rows[i])
            t |> success(ids_equal(rows[i], solo[i]), "stream {i}: the per-row tick's tokens == the tokens sampled right after its own forward")
        }
        set_select_landing(false)
        let sel1 = sel_selections()
        var unlanded <- sampled_pair(tr, prompts, sp, true, one_fits)
        t |> equal(sel_selections() - sel1, 0l, "with the landing off no step landed a selection: the count's control")
        set_select_landing(true)
        let census2 = batch_step_census()
        var device <- sampled_pair(tr, prompts, sp, true, 0ul)
        let census3 = batch_step_census()
        t |> equal(census3.per_row - census2.per_row, 0l, "under the device's own bound no tick stepped a row at a time: the census's control")
        delete device
        delete unlanded
        delete rows
        delete solo
        delete prompts
        set_select_landing(prior_landing)
    }
}

let METAL_ROW_CTX = 256l   //! the context most cells cut to: the Llama, E2B and shared-expert files' Metal forms'
let METAL_DELTANET_CTX = 512l   //! the Qwen3.5-0.8B Q8_0 Metal form's: its paged and segmented cells' context
let METAL_NEXTN_CTX = 1024l   //! the Qwen3.5-0.8B-MTP Q8_0 Metal form's: the staggered cell's deep prompt

//! the Llama-3.2-1B Q8_0 Metal form's cells on its model `tr`, a blob twin of 256 positions or more
def metal_llama_1b_cells(t : T?; var tr : Model) {
    metal_cell(t, "the batched row serves host-cached streams through the batched driver (Llama-3.2-1B Q8)",
        "llama", tr, METAL_ROW_CTX, @@batched_row_llama)
    metal_cell(t, "the batched step gathers every row off its id, bit for bit the rows the host embeds (Llama-3.2-1B-Instruct-Q8_0.gguf)",
        "llama", tr, METAL_ROW_CTX, @@batch_gather_llama)
    metal_cell(t, "four greedy streams emit the same tokens whether each batched step is encoded at its call or pre-encoded under the step before it (Llama-3.2-1B Q8)",
        "llama", tr, METAL_ROW_CTX, @@pre_encode)
    metal_cell(t, "a batched step's landing follows its sessions through a move and never outlives a released one (Llama-3.2-1B Q8)",
        "llama", tr, METAL_ROW_CTX, @@landing_release)
    metal_cell(t, "a two-row tick stepped one row at a time on Metal: each sampled stream emits what it emits sampling right after its own forward (Llama-3.2-1B Q8)",
        "llama", tr, METAL_ROW_CTX, @@per_row_tick_sampled)
}

//! the gemma-4-E2B Q8_0 Metal form's cells on its model `tr`, a blob twin of 256 positions or more
def metal_gemma4_e2b_cells(t : T?; var tr : Model) {
    metal_cell(t, "the batched row serves an E-series carrier through the batched driver's PLE rows form (gemma-4-E2B Q8)",
        "gemma4e", tr, METAL_ROW_CTX, @@batched_row_eseries)
    metal_cell(t, "the batched step gathers every row off its id, bit for bit the rows the host embeds (gemma-4-E2B-it-Q8_0.gguf)",
        "gemma", tr, METAL_ROW_CTX, @@batch_gather_eseries)
}

//! the Qwen1.5-MoE-A2.7B-4L Q8_0 Metal form's cells on its model `tr`, a blob twin of 256 positions or more
def metal_qwen_moe_cells(t : T?; var tr : Model) {
    metal_cell(t, "the batched row serves a shared-expert MoE through the batched driver's rows site (Qwen1.5-MoE-A2.7B-4L Q8_0)",
        "qwen2moe", tr, METAL_ROW_CTX, @@batched_row_shexp)
    metal_cell(t, "one row of a shared-expert MoE through the batched driver's same-slab verify binds its own panels: its logits are the single-row step's (Qwen1.5-MoE-A2.7B-4L Q8_0)",
        "qwen2moe", tr, METAL_ROW_CTX, @@batched_row_shexp_sameslab)
}

//! the Qwen3.5-0.8B-MTP Q8_0 Metal form's cells on its model `tr`, a blob twin of 1024 positions or more
def metal_qwen35_mtp_cells(t : T?; var tr : Model) {
    metal_cell(t, "the plain batched row serves a NextN-headed model with GPU logits, each row's post-norm hidden landing as its stream's carry (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@batched_row_nextn_plain)
    metal_cell(t, "the batched row refuses self-speculative streams: each stream runs its own round a tick, no batched step (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@batched_row_mtp)
    metal_cell(t, "a depth-2 walk drafts its second token through the host chain and emits what the depth-1 walk emits (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@batched_row_mtp_depth2)
    metal_cell(t, "a greedy stream's steps land their pick in the logits' place and emit what generate() emits; a sampling stream lands none (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@pick_ask)
    metal_cell(t, "three self-speculative streams through one scheduler emit what each emits alone (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@batched_row_mtp_joint_invariance)
    metal_cell(t, "three speculating streams at different depths across a 64-row chunk boundary emit what each emits alone (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, METAL_NEXTN_CTX, @@batched_row_mtp_joint_staggered)
    metal_cell(t, "a paged self-speculative stream emits what a flat one emits across page boundaries (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@paged_mtp)
    metal_cell(t, "the third prompt on a shared opening attaches the checkpoint the second left, and replies as it does prefilled whole at the same cut (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@prefix_checkpoint)
    metal_cell(t, "a request marked with its stable opening leaves the checkpoint at once, and the second prompt attaches it (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@prefix_checkpoint_hinted)
    metal_cell(t, "a session attached at a checkpoint lands the logits of the session that left it, and a checkpoint with its recurrent state zeroed does not (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@prefix_checkpoint_logits)
    metal_cell(t, "a session attached at a checkpoint captured inside one prefill window lands an uncached prefill's logits, and the capture with its recurrent state zeroed does not (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@prefix_checkpoint_capture_logits)
    metal_cell(t, "the next turn of a conversation attaches the finished turn's checkpoint and continues the count (Qwen3.5-0.8B-MTP Q8)",
        "qwen35", tr, 512l, @@prefix_checkpoint_next_turn)
}

//! the Qwen3.5-0.8B Q8_0 Metal form's cells on its model `tr`, a blob twin of 512 positions or more
def metal_qwen35_cells(t : T?; var tr : Model) {
    metal_cell(t, "the batched row serves a deltanet hybrid through the batched driver's recurrent rows form (Qwen3.5-0.8B Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@batched_row_deltanet)
    metal_cell(t, "paged sessions on a deltanet hybrid emit what flat sessions emit - the recurrent layers own no K/V rows (Qwen3.5-0.8B Q8)",
        "qwen35", tr, 512l, @@paged_deltanet)
    metal_cell(t, "two sessions' chunks as one window on a deltanet hybrid emit what single windows emit - each segment's state its own (Qwen3.5-0.8B Q8)",
        "qwen35", tr, 512l, @@segments_deltanet)
    metal_cell(t, "a deltanet session whose mirror is evicted mid-stream steps on the device from its stash, token for token the unevicted session (Qwen3.5-0.8B Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@deltanet_evicted_mirror)
    metal_cell(t, "after as many sessions were released, more than twice the mirror cap's deltanet sessions stepping in turn serve every step on the device, each token for token itself alone (Qwen3.5-0.8B Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@deltanet_released_sessions)
    metal_cell(t, "the greedy chain serves a deltanet hybrid: chained steps token for token the plain steps, and a mispredicted step's rerun reads the state the chain did not touch (Qwen3.5-0.8B Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@deltanet_greedy_chain)
    metal_cell(t, "four greedy streams on a deltanet hybrid emit the same tokens with the batched step pre-encoded under the one before (Qwen3.5-0.8B Q8)",
        "qwen35", tr, METAL_ROW_CTX, @@pre_encode)
}
