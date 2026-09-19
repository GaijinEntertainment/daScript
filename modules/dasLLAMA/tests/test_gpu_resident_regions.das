options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama   // create_session, create_batch_workspace, eval_batch
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed; gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // set_gpu_resident_regions, moe_gpu_resident_active
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// The resident driver's mirror regions (Qwen2.5-0.5B-Instruct-Q8_0 under DASLLAMA_GPU=1): two
// sessions hold their K/V on the device at once, one region each. The instrument is the driver
// against itself - a session stepped between another session's steps reads bit for bit what it
// reads alone, because a region is an offset and the kernels are the same - so no CPU chain runs.
// The two prompts differ in content and length: a step that read the other region's rows cannot
// land on the alone-run's logits.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // no cell prefills on the CPU; the declaration is every model-loading root's
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

def private load_two_regions(t : T?) : Model {
    set_gpu_resident_regions(2l)
    let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_resident_regions(1l)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed at two regions (the load log names any decline)")
    return <- tr
}

def private regions_ready(t : T?) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return false
    }
    if (!model_available(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"))) return false
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return false
    }
    return true
}

[test]
def test_regions_interleaved_single_steps(t : T?) {
    t |> run("two sessions, a region each: single steps interleaved read what each session reads alone") @(t : T?) {
        if (!regions_ready(t)) return
        var tr <- load_two_regions(t)
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

[test]
def test_regions_batched_steps(t : T?) {
    t |> run("two sessions, a region each: a batched step over both reads what each session reads alone, with no history resync") @(t : T?) {
        if (!regions_ready(t)) return
        var tr <- load_two_regions(t)
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
