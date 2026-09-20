options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade - engine + arch registrations + (on Apple) the metal overrides
require dasllama/dasllama_math   // setup_dasllama_jobque_
require _metal_blob_twin   // blob_twin - the blob-flavor carrier every Metal serving cell loads
require dasllama/dasllama_batch   // batch_step_census - how the scheduler's steps were served
require dasllama/dasllama_bench   // bench_tg_batched_rep - the batched row
require dasllama/dasllama_scheduler   // the joint speculative tick's invariance cell drives the scheduler itself
require _scheduler_rig   // toks_of
require _ids   // same_ids
require ?das_metal dasllama/dasllama_metal_common   // metal_batch_decode_stats - the batched driver's own count
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
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
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
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
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
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
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
def test_metal_batched_row_nextn_plain(t : T?) {
    t |> run("the plain batched row serves a NextN-headed model with GPU logits, each row's post-norm hidden landing as its stream's carry (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
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
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
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
def test_metal_batched_row_mtp_joint_invariance(t : T?) {
    t |> run("three self-speculative streams verified together emit what each emits alone (Qwen3.5-0.8B-MTP Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 512l)
            with_job_que() {
                setup_dasllama_jobque_()
                let prev_spec = get_mtp_spec()
                set_mtp_spec(true)
                var prompts <- [ <- encode(tr, "Count from one to twenty: one, two, three,", true, false),
                                 <- encode(tr, "The letters of the alphabet in order: a, b, c, d,", true, false),
                                 <- encode(tr, "List the days of the week: Monday, Tuesday,", true, false)]
                var none : array<int64>
                let greedy = SamplingParams()
                // each stream alone: its rounds verify two rows through the same rows forms
                var refs : array<array<int64>>
                for (pi in range64(3l)) {
                    var sch0 <- create_scheduler(tr, 1l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
                    var events0 : array<SchedEvent>
                    t |> success(submit(tr, sch0, make_req(pi + 1l, prompts[pi], greedy, 20l, none)), "solo stream {pi + 1l} queued")
                    drain(tr, sch0, events0)
                    refs |> emplace(toks_of(events0, pi + 1l))
                    delete sch0
                    delete events0
                }
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
                    t |> success(same_ids(got, refs[pi]), "joint stream {pi} tokens == its solo stream")
                    delete got
                }
                t |> success(sch.mtp_drafted > 0l, "the joint ticks drafted ({sch.mtp_drafted} drafts)")
                t |> success(sch.mtp_accepted > 0l, "some drafts accepted ({sch.mtp_accepted} of {sch.mtp_drafted})")
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
def test_metal_batched_row_deltanet(t : T?) {
    t |> run("the batched row serves a deltanet hybrid through the batched driver's recurrent rows form (Qwen3.5-0.8B Q8)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
            if (!model_available(t, path)) return
            let prior_mode = get_metal_mode()
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_twin(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
                let census0 = batch_step_census()
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "the hybrid never reached the CPU batched stack")
            }
            set_metal_mode(prior_mode)
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}
