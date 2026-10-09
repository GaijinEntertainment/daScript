options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the public facade, reached inside the das_metal halves
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - mtp_seam_declines, inside the das_metal halves
require _metal_blob_twin   // blob_twin - the blob-flavor carrier every Metal serving cell loads
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
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the das_metal halves
require _model_tier

// The batched bench row on the Metal batched driver: `bench_tg_batched_rep` homes its streams on the
// host where no whole-model driver homes them, and every timed step goes through ONE batched
// dispatch - the llama carrier, the E-series (PLE rows form), the shared-expert MoE (rows site),
// the deltanet hybrid (recurrent rows against per-session mirrors) and the NextN carrier (GPU
// logits, the carry landed per row) each serve at a rate; the self-speculative streams tick through
// one joint verify, token-exact against each stream alone. Skips off the JIT, without dasMetal,
// without the carrier under models_dir(), and on the large-tier shexp carrier unless
// DASLLAMA_PARITY_FULL=1; the refusal contract lives in test_batch_decode.das.

[test]
def test_metal_batched_row_llama(t : T?) {
    t |> run("the batched row serves host-cached streams through the batched driver (Llama-3.2-1B Q8)") <| @(t : T?) {
        if (!family_on(t, "llama")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
                t |> equal(gpu_device_sessions(), 0l, "no whole-model driver homes a stream: the row's streams are host-cached")
                let census0 = batch_step_census()
                let steps0 = metal_batch_decode_stats().steps
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "no step ran on the CPU batched stack")
                t |> success(metal_batch_decode_stats().steps - steps0 >= 8l, "the Metal batched driver counts the steps as its own")
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_eseries(t : T?) {
    t |> run("the batched row serves an E-series carrier through the batched driver's PLE rows form (gemma-4-E2B Q8)") <| @(t : T?) {
        if (!family_on(t, "gemma")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
                let census0 = batch_step_census()
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "the blob-only model never reached the CPU batched stack")
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_shexp(t : T?) {
    t |> run("the batched row serves a shared-expert MoE through the batched driver's rows site (Qwen1.5-MoE-A2.7B Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen2moe")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
                let census0 = batch_step_census()
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "the blob-only model never reached the CPU batched stack")
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! the rows and the single GEMV forms round the router's logits apart and a near-tie flips an expert: the plain-MoE bar of
//! test_metal_mtp_parity's q30 arms (reads 1.6e-3 at most on the M1 Max, the one-step-off control 9.7 at least)
let SHEXP_SAMESLAB_BAR = 2.0
let SHEXP_SAMESLAB_ROUNDS = 12l

[test]
def test_metal_batched_row_shexp_sameslab(t : T?) {
    t |> run("one row of a shared-expert MoE through the batched driver's same-slab verify binds its own panels: its logits are the single-row step's (Qwen1.5-MoE-A2.7B Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen2moe")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
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
                        d = max(d, abs(sa.logits[v] - sb.mtp_logits_b[v]))
                        doff = max(doff, abs(prev[v] - sb.mtp_logits_b[v]))
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
                t |> success(off_min > SHEXP_SAMESLAB_BAR, "the one-step-off control lands outside the bar at every round (nearest {off_min})")
                delete ws
                delete one
                delete prev
                delete sa
                delete sb
                delete prompt
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_nextn_plain(t : T?) {
    t |> run("the plain batched row serves a NextN-headed model with GPU logits, each row's post-norm hidden landing as its stream's carry (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_mtp(t : T?) {
    t |> run("the batched row serves self-speculative streams through one joint verify a tick (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
                let prev_spec = get_mtp_spec()
                set_mtp_spec(true)
                let census0 = batch_step_census()
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l, true)
                set_mtp_spec(prev_spec)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed ticks each rode one joint verify ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "no tick reached the CPU batched stack")
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_mtp_depth2(t : T?) {
    t |> run("a depth-2 walk drafts its second token through the host chain and emits what the depth-1 walk emits (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
                var prompt <- encode(tr, "Count from one to twenty: one, two, three,", true, false)
                let total = long_length(prompt) - 1l + 24l
                var toks : array<array<int64>>
                var drafted, accepted : array<int64>
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_mtp_joint_invariance(t : T?) {
    t |> run("three self-speculative streams verified together emit what each emits alone (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
                let prev_spec = get_mtp_spec()
                set_mtp_spec(true)
                var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                                 <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                                 <- encode(tr, "List the days of the week: Monday, Tuesday,", true, false)]
                var none : array<int64>
                let greedy = SamplingParams()
                // each stream alone: its rounds verify two rows through the same rows forms
                var refs : array<array<int64>>
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
                //! the solo rounds draft on the two-row tile, the joint ones on a wider tile: the same drafts, so the same accepts
                t |> success(solo_accepted * 2l > solo_drafted, "the solo rounds accept more than half their drafts ({solo_accepted} of {solo_drafted})")
                // together: one joint verify a tick over every stream's rows
                var sch <- create_scheduler(tr, 3l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
                var events : array<SchedEvent>
                for (pi in range64(3l)) {
                    t |> success(submit(tr, sch, make_req(10l + pi, prompts[pi], greedy, 20l, none)), "joint stream {10l + pi} queued")
                }
                let census0 = batch_step_census()
                drain(tr, sch, events)
                let census1 = batch_step_census()
                set_mtp_spec(prev_spec)
                t |> equal(sch.mtp_joined_max, 3l, "one joint verify carried all three streams (the widest tick carried {sch.mtp_joined_max})")
                for (pi in range64(3l)) {
                    var got <- toks_of(events, 10l + pi)
                    log_gen_texts(tr, "joint stream {pi} (solo, joint)", refs[pi], got)
                    t |> success(ids_equal(got, refs[pi]), "joint stream {pi} tokens == its solo stream")
                    delete got
                }
                t |> success(sch.mtp_drafted > 0l, "the joint ticks drafted ({sch.mtp_drafted} drafts)")
                t |> equal(sch.mtp_drafted, solo_drafted, "the joint ticks drafted what the three solo streams drafted together ({sch.mtp_drafted} against {solo_drafted})")
                t |> equal(sch.mtp_accepted, solo_accepted, "... and accepted the same drafts ({sch.mtp_accepted} against {solo_accepted})")
                t |> success(census1.device - census0.device > 0l, "ticks with every stream decoding rode one joint verify ({census1.device - census0.device} device steps)")
                delete sch
                delete events
                delete refs
                delete prompts
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_mtp_joint_staggered(t : T?) {
    t |> run("a joint verify whose streams sit at different depths across a 64-row chunk boundary emits what each stream emits alone (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 1024l)
            with_dasllama_jobque_() {
                let prev_spec = get_mtp_spec()
                set_mtp_spec(true)
                //! group 0 is shallow, the LAST group deep: the round's chunk count and its attention form must follow the deepest row, not the first group's
                var deep = ""
                for (_i in range(12)) {
                    deep += "The quick brown fox jumps over the lazy dog while the farmer counts his sheep one by one in the field. "
                }
                deep += "Count from one to twenty: one, two, three,"
                var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                                 <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                                 <- encode(tr, deep, true, false)]
                t |> success(long_length(prompts[2]) > 200l, "the deep stream's prompt crosses three chunk boundaries ({long_length(prompts[2])} tokens)")
                var none : array<int64>
                let greedy = SamplingParams()
                var refs : array<array<int64>>
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
                t |> equal(sch.mtp_joined_max, 3l, "one joint verify carried all three streams (the widest tick carried {sch.mtp_joined_max})")
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_deltanet(t : T?) {
    t |> run("the batched row serves a deltanet hybrid through the batched driver's recurrent rows form (Qwen3.5-0.8B Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
                let census0 = batch_step_census()
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "the hybrid never reached the CPU batched stack")
                // nine streams: more rows than the mirror cache's resting size
                let wide = bench_tg_batched_rep(tr, 9l, 32l, 8l, 2l)
                t |> equal(wide.why, "", "the nine-stream row is served: {wide.why}")
                t |> success(wide.tps > 0.0lf, "... at a rate ({wide.tps:.1f} tok/s summed over 9 streams)")
                let census2 = batch_step_census()
                t |> success(census2.device - census1.device >= 8l, "its timed steps went through the device batched driver ({census2.device - census1.device} device steps)")
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

let PAGED_PROMPT = 40l
let PAGED_NEW = 40l   //! prompt plus reply is 80 rows: past one 64-row page, so a stream's rows span two pages

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
[test]
def test_metal_paged_deltanet(t : T?) {
    t |> run("paged sessions on a deltanet hybrid emit what flat sessions emit - the recurrent layers own no K/V rows (Qwen3.5-0.8B Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
                for (nst in [1l, 2l]) {
                    let steps0 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
                    var flat <- served_tokens(tr, nst, 0l, PAGED_NEW)
                    let steps1 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
                    var paged <- served_tokens(tr, nst, 64l, PAGED_NEW)
                    let steps2 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

let PAGED_MTP_NEW = 150l   //! past two 64-row pages, so rounds land on a page's last row

//! A round verifies the row past its token, so a round at a page's last row writes the next page's first.
[test]
def test_metal_paged_mtp(t : T?) {
    t |> run("a paged self-speculative stream emits what a flat one emits across page boundaries (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
                let prev_spec = get_mtp_spec()
                set_mtp_spec(true)
                var prompt <- encode(tr, "Count from one to one hundred: one, two, three,", true, false)
                var none : array<int64>
                let greedy = SamplingParams()
                var toks : array<array<int64>>
                t |> equal(chunk_defaults(tr).idle, METAL_IDLE_CHUNK_TOKENS, "a Metal-served model's idle prefill quantum is the wide one")
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

struct Served {
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

let CKPT_NEW = 24l

//! A recurrent model's prefix cache is a checkpoint at one exact position: the second prompt with an opening leaves it, the third attaches it.
[test]
def test_metal_prefix_checkpoint(t : T?) {
    t |> run("the third prompt on a shared opening attaches the checkpoint the second left, and replies as it does prefilled whole at the same cut (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
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
                // a checkpoint within a chunk and a half is taken in one window; the idle quantum is off, so every tick takes the chunk
                let chunk = cut * 3l / 4l
                var near <- create_scheduler(tr, 1l, 4l, chunk, 64l, 0l, KVDtype.f16, true, 0l)
                var events : array<SchedEvent>
                var none : array<int64>
                var req <- make_req(7l, pb, SamplingParams(), CKPT_NEW, none)
                req.stable_at = cut
                t |> success(submit(tr, near, req), "the request marked at its opening queued")
                scheduler_step(tr, near, events)
                t |> equal(near.prefill_tokens, cut, "a checkpoint {cut} tokens out lands in the first window of a {chunk}-token chunk")
                drain(tr, near, events)
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! A caller that marks its prompt's stable opening gets the checkpoint from the first request on.
[test]
def test_metal_prefix_checkpoint_hinted(t : T?) {
    t |> run("a request marked with its stable opening leaves the checkpoint at once, and the second prompt attaches it (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
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
                t |> equal(plain.prefix.n_state_inserts, 1l, "an unmarked first request leaves its finished turn alone")
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! The reply to a strongly prompted tail survives a lost recurrent state, so the checkpoint's contents are held on the logits.
[test]
def test_metal_prefix_checkpoint_logits(t : T?) {
    t |> run("a session attached at a checkpoint lands the logits of the session that left it, and a checkpoint with its recurrent state zeroed does not (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! A finished turn leaves a checkpoint past its reply, so the conversation's next turn prefills only what is new.
[test]
def test_metal_prefix_checkpoint_next_turn(t : T?) {
    t |> run("the next turn of a conversation attaches the finished turn's checkpoint and continues the count (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_dasllama_jobque_() {
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_pre_encode(t : T?) {
    t |> run("four greedy streams emit the same tokens whether each batched step is encoded at its call or pre-encoded under the step before it (Llama-3.2-1B Q8)") <| @(t : T?) {
        if (!family_on(t, "llama")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_dasllama_jobque_() {
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
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}
