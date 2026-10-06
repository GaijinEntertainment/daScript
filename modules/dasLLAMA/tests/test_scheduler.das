options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // each test frees its model/scheduler before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_scheduler   // the scheduler under test
require _scheduler_rig   // make_req, drain, toks_of, finished_of
require dasllama/dasllama_batch   // batch_step_census: how the speculative ticks were served
require dasllama/dasllama                               // facade: load/encode/generate references + engine knobs
require dasllama/dasllama_blocks   // embed_text_rows: the media arms build rows from the prompt's own tokens
require dasllama/dasllama_common   // layer_is_recurrent: the deltanet-on-GPU cell finds a recurrent layer to probe
require dasllama/dasllama_gpu_tier   // moe_gpu_dnd_on_gpu: that cell skips unless the tier serves the decode step; moe_gpu_tier_installed / gpu_want_auto
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / moe_gpu_resident_verifies: the resident pick-ask cell's witnesses
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost   // join: the decoded stream logs beside the token-exact compares
require math
require strings
require _model_tier

// Unit gates for the continuous-batching scheduler — all through the SchedEvent seam, zero HTTP:
// scheduler ≡ generate bit-exact (1 stream), chunk-size invariance under classic prefill, staggered
// admit + queue overflow, stop/budget/context-length finishes, evict mid-run. Model-gated + JIT-only
// (SmolLM2-135M GGUF via DASLLAMA_MODELS_DIR), same skip contract as test_openai_server.

let MODEL_PATH = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")

def private model_missing(t : T?) : bool {
    return true if (!jit_only(t))
    if (!stat(MODEL_PATH).is_valid) {
        t |> skip("{MODEL_PATH} not present")
        return true
    }
    return false
}

def private load_capped_cpu(cap : int64) : Model {
    var m <- load_model_(MODEL_PATH, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, cap)
    moe_gpu_drop_model()
    return <- m
}

// One stream's reference: generate() with the wiring's stop protocol (stop token ends the stream
// unemitted; otherwise the budget runs out) — what the scheduler must reproduce.
struct private RefStream {
    toks : array<int64>
    pieces : array<string>
    finish : string
}

def private ref_generate(m : Model; prompt : array<int64>; params : SamplingParams;
                         max_new : int64; stop_ids : array<int64>; seed : int = 0) : RefStream {
    var s <- create_session(m)
    if (seed != 0) {
        set_seed(s, seed)
    }
    var r : RefStream
    var stops := stop_ids
    var hit_stop = false
    generate(m, s, prompt, params, max_new) $(id, pc) {
        for (sid in stops) {
            if (id == sid) {
                hit_stop = true
                return false
            }
        }
        r.toks |> push(id)
        r.pieces |> push(pc)
        return true
    }
    r.finish = hit_stop ? "stop" : "length"
    delete s
    return <- r
}

def private pieces_of(events : array<SchedEvent>; id : int64) : array<string> =>
    <- [for (e in events); e.text; where e.stream == id && e.kind == SchedEventKind.piece]

// both sides of a stream compare, decoded - a near-tie synonym flip and a broken walk read alike as ids
def private log_streams(la : string; ea : array<SchedEvent>; ida : int64; lb : string; eb : array<SchedEvent>; idb : int64) {
    to_log(LOG_INFO, "{la}: {join(pieces_of(ea, ida), "")}\n{lb}: {join(pieces_of(eb, idb), "")}\n")
}

let DN_MODEL_PATH = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")

def private has_gpu_dn_layer(c : Config) : bool {
    for (l in range64(c.n_layers)) {
        if (layer_is_recurrent(c, l) && moe_gpu_dnd_on_gpu(l)) {
            return true
        }
    }
    return false
}

def private text_of(pieces : array<string>) : string {
    return build_string() $(var w) {
        for (p in pieces) {
            w |> write(p)
        }
    }
}

// ===== two deltanet streams turn about on the GPU tier: each reads its own recurrent state =====

[test]
def test_scheduler_batching_deltanet_gpu(t : T?) {
    t |> run("two interleaved deltanet streams on the GPU tier match their solo references (needs Qwen3.5-0.8B-Q8_0, DASLLAMA_GPU=1)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(DN_MODEL_PATH).is_valid) {
            t |> skip("{DN_MODEL_PATH} not present")
            return
        }
        with_dasllama_jobque() {
            var m <- load_model_(DN_MODEL_PATH, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, 256l)
            let prev_dnd_route = moe_gpu_dnd_route()
            set_moe_gpu_dnd_route(true)   // the claim is about the tier's decode step: pin its route on, whatever an earlier cell left
            if (!has_gpu_dn_layer(m.config)) {
                t |> skip("the GPU tier's deltanet decode step is not armed - needs DASLLAMA_GPU=1 on a device that serves it")
                set_moe_gpu_dnd_route(prev_dnd_route)
                delete m
                return
            }
            let greedy = SamplingParams()
            var none : array<int64>
            var pa <- encode(m, "Count from one to ten in words.", true, false)
            var pb <- encode(m, "Name three primary colors.", true, false)
            var refs <- [ <- ref_generate(m, pa, greedy, 8l, none), <- ref_generate(m, pb, greedy, 8l, none)]
            var sch1 <- create_scheduler(m, 1l, 2l, 64l)
            var events1 : array<SchedEvent>
            t |> success(submit(m, sch1, make_req(10l, pa, greedy, 8l, none)), "stream A queued (one-stream scheduler)")
            t |> success(submit(m, sch1, make_req(11l, pb, greedy, 8l, none)), "stream B queued (one-stream scheduler)")
            drain(m, sch1, events1)
            var sa <- toks_of(events1, 10l)
            var sb <- toks_of(events1, 11l)
            to_log(LOG_INFO, "one-stream scheduler A: \"{text_of(pieces_of(events1, 10l))}\" | generate A: \"{text_of(refs[0].pieces)}\"\n")
            to_log(LOG_INFO, "one-stream scheduler B: \"{text_of(pieces_of(events1, 11l))}\" | generate B: \"{text_of(refs[1].pieces)}\"\n")
            t |> success(ids_equal(sa, refs[0].toks), "stream A through a one-stream scheduler matches generate ({sa} vs {refs[0].toks})")
            t |> success(ids_equal(sb, refs[1].toks), "stream B through a one-stream scheduler matches generate ({sb} vs {refs[1].toks})")
            var sch <- create_scheduler(m, 2l, 2l, 64l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(20l, pa, greedy, 8l, none)), "stream A queued")
            t |> success(submit(m, sch, make_req(21l, pb, greedy, 8l, none)), "stream B queued")
            drain(m, sch, events)
            var ga <- toks_of(events, 20l)
            var gb <- toks_of(events, 21l)
            to_log(LOG_INFO, "two-stream A: \"{text_of(pieces_of(events, 20l))}\" | solo A: \"{text_of(refs[0].pieces)}\"\n")
            to_log(LOG_INFO, "two-stream B: \"{text_of(pieces_of(events, 21l))}\" | solo B: \"{text_of(refs[1].pieces)}\"\n")
            t |> success(ids_equal(ga, refs[0].toks), "stream A, decoded turn about with B, matches its solo reference ({ga} vs {refs[0].toks})")
            t |> success(ids_equal(gb, refs[1].toks), "stream B, decoded turn about with A, matches its solo reference ({gb} vs {refs[1].toks})")
            t |> success(sch.sum_batch >= 2l * 2l, "the two streams shared decode steps (sum batch {sch.sum_batch})")
            delete sa
            delete sb
            delete events1
            delete sch1
            delete ga
            delete gb
            delete events
            delete sch
            delete refs
            delete pa
            delete pb
            set_moe_gpu_dnd_route(prev_dnd_route)
            delete m
        }
    }
}

// ===== scheduler ≡ generate (1 stream): greedy bit-exact, seeded sampling, stop finish =====

[test]
def test_scheduler_matches_generate(t : T?) {   // nolint:STYLE038 — one model load per test: every assertion runs against that model inside the same jobque scope
    t |> run("scheduler == generate: bit-exact stream, seeded sampling, stop/budget (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            var prompt <- encode(m, "Once upon a time", true, false)
            var none : array<int64>
            // greedy, single-chunk prefill: the eval sequence is identical to generate()'s, so the
            // emitted stream must be BIT-exact (B==1 eval_batch delegates to the same forward())
            let greedy = SamplingParams()
            var ref <- ref_generate(m, prompt, greedy, 10l, none)
            var sch <- create_scheduler(m, 1l, 4l, 512l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(1l, prompt, greedy, 10l, none)), "greedy stream queued")
            drain(m, sch, events)
            var got <- toks_of(events, 1l)
            var gotp <- pieces_of(events, 1l)
            t |> success(ids_equal(got, ref.toks), "greedy tokens == generate() bit-exact")
            t |> equal(length(gotp), length(ref.pieces))
            for (i in range(length(gotp))) {
                t |> equal(gotp[i], ref.pieces[i])
            }
            let fin = finished_of(events, 1l)
            t |> equal(fin.finish, "length")
            t |> equal(fin.n_gen, 10l)
            t |> equal(fin.n_prompt, int64(length(prompt)))
            t |> success(fin.ttft_us > 0l, "TTFT recorded")
            t |> equal(sch.gen_tokens, 10l)
            t |> success(sch.decode_steps > 0l, "decode steps counted")
            // seeded sampling (temp + top-k): the scheduler makes the same sample() calls in the
            // same order, so a seeded run reproduces generate() exactly
            let sp = SamplingParams(temp = 0.8, top_k = 40l)
            var ref2 <- ref_generate(m, prompt, sp, 10l, none, 4242)
            events |> clear()
            t |> success(submit(m, sch, make_req(2l, prompt, sp, 10l, none, 4242)), "seeded stream queued")
            drain(m, sch, events)
            var got2 <- toks_of(events, 2l)
            t |> success(ids_equal(got2, ref2.toks), "seeded sampled tokens == generate()")
            // stop finish: pick the first greedy token that did not occur earlier in the stream,
            // declare it a stop id — the scheduler must end there, stop token unemitted
            var kstop = -1
            for (k in range(1, length(ref.toks))) {
                var seen = false
                for (j in range(k)) {
                    seen ||= ref.toks[j] == ref.toks[k]
                }
                if (!seen) {
                    kstop = k
                    break
                }
            }
            if (kstop > 0) {
                var stops <- [ref.toks[kstop]]
                var ref3 <- ref_generate(m, prompt, greedy, 10l, stops)
                events |> clear()
                t |> success(submit(m, sch, make_req(3l, prompt, greedy, 10l, stops)), "stop stream queued")
                drain(m, sch, events)
                var got3 <- toks_of(events, 3l)
                t |> equal(length(got3), kstop)
                t |> success(ids_equal(got3, ref3.toks), "stop-bounded tokens == generate()")
                let fin3 = finished_of(events, 3l)
                t |> equal(fin3.finish, "stop")
                t |> equal(fin3.n_gen, int64(kstop))
                delete got3
                delete ref3
                delete stops
            } else {
                t |> success(true, "no unique stop candidate in 10 greedy tokens - stop arm skipped")
            }
            delete got
            delete gotp
            delete got2
            delete ref
            delete ref2
            delete events
            delete sch
            delete prompt
            delete m
        }
    }
}

// ===== chunked prefill: {1,3,17,64} produce the identical stream under classic prefill =====

[test]
def test_scheduler_chunk_sizes(t : T?) {
    t |> run("prefill chunk sizes 1/3/17/64 give the same stream - classic mode (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            // classic prefill is bit-exact under ANY chunking (test_prefill's contract) — the same
            // pin as test_chat's render-seam case, so every chunk size must yield ONE stream
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery and counts every bird that crosses the evening sky", true, false)
            t |> success(int64(length(prompt)) > 17l, "prompt spans multiple chunks ({length(prompt)} tokens)")
            var none : array<int64>
            let greedy = SamplingParams()
            var first <- [-1l, -1l, -1l, -1l]
            var streams : array<array<int64>>
            for (ci, K in range(4), [1l, 3l, 17l, 64l]) {
                var sch <- create_scheduler(m, 1l, 4l, K, [idle_chunk_tokens = 0l])
                var inscope events : array<SchedEvent>
                t |> success(submit(m, sch, make_req(int64(ci), prompt, greedy, 6l, none)), "stream queued (K={K})")
                drain(m, sch, events)
                var inscope got <- toks_of(events, int64(ci))
                first[ci] = !empty(got) ? got[0] : -1l
                to_log(LOG_INFO, "K={K}: \"{text_of(pieces_of(events, int64(ci)))}\" {got}\n")
                streams |> emplace(got)
                delete events
                delete sch
            }
            t |> success(first[0] >= 0l, "chunked runs produced tokens")
            for (ci in range(1, 4)) {
                t |> equal(first[ci], first[0])
                t |> success(ids_equal(streams[ci], streams[0]), "K arm {ci}: full stream identical to K=1")
            }
            set_prefill_attn_mode(saved_mode)
            delete first
            delete streams
            delete prompt
            delete m
        }
    }
}

[test]
def test_scheduler_idle_quantum(t : T?) {
    t |> run("a lone stream prefills in the idle quantum, a stream beside a decoding one in the chunk (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)   // bit-exact under any chunking: the two quanta must yield one stream
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery and counts every bird that crosses the evening sky", true, false)
            var short <- encode(m, "Once upon a time", true, false)
            let np = long_length(prompt)
            let K = 8l
            t |> success(np > 3l * K && long_length(short) <= K, "the long prompt spans chunks, the short one fits one ({np} and {length(short)} tokens)")
            var none : array<int64>
            let greedy = SamplingParams()
            var streams : array<array<int64>>
            for (idle in [-1l, 0l]) {   // the backend's default, then off
                var sch <- create_scheduler(m, 1l, 4l, K, [idle_chunk_tokens = idle])
                var inscope events : array<SchedEvent>
                t |> success(submit(m, sch, make_req(1l, prompt, greedy, 6l, none)), "lone stream queued (idle quantum {idle})")
                scheduler_step(m, sch, events)
                t |> equal(sch.idle_chunk_tokens, idle < 0l ? chunk_defaults(m).idle : idle, "idle quantum {idle} resolves to {sch.idle_chunk_tokens}")
                t |> equal(sch.prefill_tokens, idle != 0l ? np : K, "idle quantum {idle}: the first tick prefilled {sch.prefill_tokens} of {np} tokens")
                drain(m, sch, events)
                streams |> emplace(toks_of(events, 1l))
                delete events
                delete sch
            }
            log_gen_texts(m, "idle quantum (chunked, then widened)", streams[1], streams[0])
            t |> success(!empty(streams[0]) && ids_equal(streams[0], streams[1]), "the widened prefill emits the chunked prefill's stream")
            var dsch <- create_scheduler(m)
            t |> equal(dsch.chunk_tokens, chunk_defaults(m).chunk, "an unset chunk is the serving backend's ({dsch.chunk_tokens})")
            t |> equal(dsch.idle_chunk_tokens, chunk_defaults(m).idle, "an unset idle size is the serving backend's ({dsch.idle_chunk_tokens})")
            t |> equal(chunk_default(true), DEFAULT_CHUNK_TOKENS, "where a GPU prefills an unset chunk is one window")
            t |> equal(chunk_default(false), CPU_CHUNK_TOKENS, "and the short one on the CPU")
            delete dsch
            // a stream decoding: the next prompt's chunk is the stall bound again
            var sch <- create_scheduler(m, 2l, 4l, K)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(2l, short, greedy, 64l, none)), "decoding stream queued")
            scheduler_step(m, sch, events)
            t |> equal(sch.prefill_tokens, long_length(short), "the short prompt prefilled in its first tick")
            t |> success(submit(m, sch, make_req(3l, prompt, greedy, 6l, none)), "second stream queued beside it")
            let before = sch.prefill_tokens
            scheduler_step(m, sch, events)
            t |> equal(sch.prefill_tokens - before, K, "beside a decoding stream the tick prefilled one chunk ({sch.prefill_tokens - before} tokens)")
            drain(m, sch, events)
            set_prefill_attn_mode(saved_mode)
            delete events
            delete sch
            delete streams
            delete prompt
            delete short
            delete m
        }
    }
}

// ===== staggered admit, queue overflow, queued eviction, multi-stream token match =====

[test]
def test_scheduler_batching(t : T?) {
    t |> run("staggered admit + queue overflow: every stream matches its solo reference (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            // the batch worker mirrors the std decode path — pin the fused chain off so the solo
            // generate() references share its numerics (test_batch_decode's contract)
            let prev_fused = get_fused_decode()
            set_fused_decode(false)
            let greedy = SamplingParams()
            var none : array<int64>
            var pa <- encode(m, "Once upon a time", true, false)
            var pb <- encode(m, "The little robot", true, false)
            var pc <- encode(m, "In a small village", true, false)
            var refs <- [ <- ref_generate(m, pa, greedy, 6l, none), <- ref_generate(m, pb, greedy, 6l, none),
                         <- ref_generate(m, pc, greedy, 6l, none)]
            var sch <- create_scheduler(m, 2l, 2l, 64l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(10l, pa, greedy, 6l, none)), "stream A queued")
            t |> success(submit(m, sch, make_req(11l, pb, greedy, 6l, none)), "stream B queued")
            // 3 ticks: A and B admit (cap 2) and start decoding; C arrives mid-run — staggered
            for (_i in range(3)) {
                scheduler_step(m, sch, events)
            }
            t |> equal(n_active(sch), 2l)
            t |> success(submit(m, sch, make_req(12l, pc, greedy, 6l, none)), "stream C queued mid-run")
            t |> success(submit(m, sch, make_req(13l, pa, greedy, 6l, none)), "stream D queued (fills the queue)")
            t |> success(!submit(m, sch, make_req(14l, pb, greedy, 6l, none)), "queue full: submit rejected")
            t |> success(evict(sch, 13l, events), "queued stream D evicted")
            t |> equal(finished_of(events, 13l).finish, "evicted")
            t |> equal(n_queued(sch), 1l)
            drain(m, sch, events)
            t |> success(empty(sch.batch_rows), "no row pointer outlives the step that reaped its batched stream (a validating heap collect between steps walks them)")
            var ga <- toks_of(events, 10l)
            var gb <- toks_of(events, 11l)
            var gc <- toks_of(events, 12l)
            to_log(LOG_INFO, "batched A: \"{text_of(pieces_of(events, 10l))}\" {ga} | solo A: \"{text_of(refs[0].pieces)}\" {refs[0].toks}\n")
            to_log(LOG_INFO, "batched B: \"{text_of(pieces_of(events, 11l))}\" {gb} | solo B: \"{text_of(refs[1].pieces)}\" {refs[1].toks}\n")
            to_log(LOG_INFO, "batched C: \"{text_of(pieces_of(events, 12l))}\" {gc} | solo C: \"{text_of(refs[2].pieces)}\" {refs[2].toks}\n")
            t |> success(ids_equal(ga, refs[0].toks), "stream A (batched) matches its solo reference")
            t |> success(ids_equal(gb, refs[1].toks), "stream B (batched) matches its solo reference")
            t |> success(ids_equal(gc, refs[2].toks), "stream C (admitted after a slot freed) matches its solo reference")
            t |> equal(finished_of(events, 12l).finish, "length")
            t |> equal(length(toks_of(events, 13l)), 0)
            t |> equal(sch.peak_active, 2l)
            t |> equal(sch.n_evicted, 1l)
            t |> equal(sch.n_finished, 3l)
            t |> success(sch.sum_batch >= 2l * 2l, "decode steps actually batched (sum batch {sch.sum_batch})")
            set_fused_decode(prev_fused)
            delete ga
            delete gb
            delete gc
            delete events
            delete sch
            delete refs
            delete pa
            delete pb
            delete pc
            delete m
        }
    }
}

// ===== paged serving + prefix cache: a repeat request attaches pages and skips their prefill =====

[test]
def test_scheduler_prefix_cache(t : T?) {   // nolint:STYLE038 — one model load per test: every assertion runs against that model inside the same jobque scope
    t |> run("paged scheduler: repeat request attaches cached pages, identical stream (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            // classic prefill: the warm run re-chunks the prompt (tail-only), and only classic is
            // bit-exact under re-chunking — the same pin as the chunk-size arm above
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var prompt <- encode(m, "Once upon a time in a small village near the mountains there lived a very curious little robot who loved counting stars", true, false)
            let np = int64(length(prompt))
            t |> success(np > 17l, "prompt spans multiple pages at P=16 ({np} tokens)")
            var none : array<int64>
            let greedy = SamplingParams()
            var sch <- create_scheduler(m, 1l, 4l, 64l, 16l)   // paged, P=16, unbounded prefix cache
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(1l, prompt, greedy, 8l, none)), "cold stream queued")
            drain(m, sch, events)
            var cold <- toks_of(events, 1l)
            t |> equal(finished_of(events, 1l).n_cached, 0l)
            t |> success(prefix_held_groups(sch.prefix) > 0l, "the finished stream donated its pages")
            t |> equal(sch.cached_tokens, 0l)
            events |> clear()
            t |> success(submit(m, sch, make_req(2l, prompt, greedy, 8l, none)), "warm stream queued")
            drain(m, sch, events)
            var warm <- toks_of(events, 2l)
            let fin2 = finished_of(events, 2l)
            t |> equal(fin2.n_cached, np - 1l)   // the whole pages shared and the rows of the next page copied, capped one token below np
            t |> success(ids_equal(warm, cold), "warm stream identical to cold")
            t |> equal(sch.cached_tokens, fin2.n_cached)
            // idle scheduler: the only live page holds are the cache's
            t |> equal(int64(length(sch.pool.free_groups)),
                       sch.pool.capacity_groups - prefix_held_groups(sch.prefix))

            // ===== stop-finished streams donate an ALIGNED history: continuation attach == cold.
            // A "stop" finish must donate rows matching prompt + emitted + close exactly — the stop
            // was sampled from the last emitted token's eval, so only the CLOSE tokens are pending.
            // Re-evaling the last token would shift every row past the reply boundary against the
            // page's token list, poisoning multi-turn continuation attaches.
            var prompt3 <- encode(m, "The little robot climbed the old bakery roof to count the evening birds", true, false)
            var ref3 <- ref_generate(m, prompt3, greedy, 8l, none)
            var kstop = -1
            for (k in range(1, length(ref3.toks))) {   // first token unique so far — a safe stop id
                var seen = false
                for (j in range(k)) {
                    seen ||= ref3.toks[j] == ref3.toks[k]
                }
                if (!seen) {
                    kstop = k
                    break
                }
            }
            if (kstop >= 1) {
                var stops <- [ref3.toks[kstop]]
                var close <- [100l, 200l]   // any valid ids: the donated history must include them
                var sch2 <- create_scheduler(m, 1l, 4l, 64l, 16l)
                events |> clear()
                var req <- make_req(50l, prompt3, greedy, 8l, stops)
                req.close_toks := close
                t |> success(submit(m, sch2, req), "stop stream queued")
                drain(m, sch2, events)
                t |> equal(finished_of(events, 50l).finish, "stop")
                var emitted <- toks_of(events, 50l)
                t |> equal(length(emitted), kstop)
                // continuation: the next turn's prompt replays prompt + reply + close, then diverges
                var prompt4 : array<int64>
                prompt4 |> push_from(prompt3, emitted, close)
                for (i in range64(16l)) {
                    prompt4 |> push(500l + i)
                }
                events |> clear()
                t |> success(submit(m, sch2, make_req(51l, prompt4, greedy, 6l, none)), "warm continuation queued")
                drain(m, sch2, events)
                var warm4 <- toks_of(events, 51l)
                let fin4 = finished_of(events, 51l)
                t |> success(fin4.n_cached > int64(length(prompt3)),
                    "continuation attached past the reply boundary (cached {fin4.n_cached})")
                // cold reference: the same continuation with no cache
                var sch3 <- create_scheduler(m, 1l, 4l, 64l, 16l)
                events |> clear()
                t |> success(submit(m, sch3, make_req(52l, prompt4, greedy, 6l, none)), "cold continuation queued")
                drain(m, sch3, events)
                var cold4 <- toks_of(events, 52l)
                t |> success(ids_equal(warm4, cold4), "continuation attach bit-exact vs cold - donated rows aligned")
                delete cold4
                delete sch3
                delete warm4
                delete prompt4
                delete emitted
                delete sch2
                delete close
                delete stops
            } else {
                t |> success(true, "no unique stop candidate in 8 greedy tokens - continuation arm skipped")
            }
            delete ref3
            delete prompt3
            set_prefill_attn_mode(saved_mode)
            delete cold
            delete warm
            delete events
            delete sch
            delete prompt
            delete m
        }
    }
}

// ===== active eviction mid-run + the context-exhaustion length pre-check =====

[test]
def test_scheduler_evict_and_context_length(t : T?) {
    t |> run("evict mid-run leaves the survivor exact; context exhaustion finishes 'length' (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(64l)
            let prev_fused = get_fused_decode()
            set_fused_decode(false)
            let greedy = SamplingParams()
            var none : array<int64>
            var pa <- encode(m, "Once upon a time", true, false)
            var pb <- encode(m, "The little robot", true, false)
            var ref_a <- ref_generate(m, pa, greedy, 8l, none)
            var sch <- create_scheduler(m, 2l, 4l, 64l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(20l, pa, greedy, 8l, none)), "survivor queued")
            t |> success(submit(m, sch, make_req(21l, pb, greedy, 8l, none)), "victim queued")
            for (_i in range(4)) {   // both prefilled + a few decode steps at B=2
                scheduler_step(m, sch, events)
            }
            let b_before = length(toks_of(events, 21l))
            t |> success(b_before > 0, "victim generated before eviction")
            t |> success(evict(sch, 21l, events), "victim evicted mid-run")
            t |> equal(finished_of(events, 21l).finish, "evicted")
            drain(m, sch, events)
            t |> equal(length(toks_of(events, 21l)), b_before)
            var ga <- toks_of(events, 20l)
            to_log(LOG_INFO, "survivor: \"{text_of(pieces_of(events, 20l))}\" {ga} | solo: \"{text_of(ref_a.pieces)}\" {ref_a.toks}\n")
            t |> success(ids_equal(ga, ref_a.toks), "survivor stream unaffected by the eviction")
            t |> equal(finished_of(events, 20l).finish, "length")
            // context exhaustion: budget far beyond the capped seq_len — the decode pre-check must
            // finish "length" (n_gen = seq_len - n_prompt + 1) instead of reaching the engine panic
            events |> clear()
            t |> success(submit(m, sch, make_req(22l, pa, greedy, 512l, none)), "exhaustion stream queued")
            drain(m, sch, events)
            let fin = finished_of(events, 22l)
            t |> equal(fin.finish, "length")
            t |> equal(fin.n_gen, m.config.seq_len - int64(length(pa)) + 1l)
            set_fused_decode(prev_fused)
            delete ga
            delete events
            delete sch
            delete ref_a
            delete pa
            delete pb
            delete m
        }
    }
}

// ===== scheduler mtp: greedy self-spec ticks are output-invariant vs the plain scheduler =====

let MTP_MODEL_PATH = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")

[test]
def test_scheduler_mtp(t : T?) {   // nolint:STYLE038 — one model load per test: the greedy and the sampled leg run against that model inside the same jobque scope
    t |> run("scheduler mtp: self-spec greedy and sampled streams == plain scheduler, chunked prefill + paged (needs Qwen3.5-0.8B-MTP)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MTP_MODEL_PATH).is_valid) {
            t |> skip("{MTP_MODEL_PATH} not present")
            return
        }
        // pin the bit-exact kernels (same reasoning as test_mtp): the spec arm's verify rides the
        // BATCH kernels while the plain arm decodes per-token — near-tie flips are not bugs, so the
        // token-exact gate must not ride them. Both arms run paged/f16 so only spec mechanics differ.
        let prev_attn = get_prefill_attn_mode()
        set_prefill_attn_mode(AttnPrefillMode.classic)
        let prev_act = get_act_vec()
        set_act_vec(false)
        let prev_dn = get_deltanet_prefill_mode()
        set_deltanet_prefill_mode(DnPrefillMode.recurrent)
        with_dasllama_jobque() {
            var m <- load_model_(MTP_MODEL_PATH, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, 512l)
            var prompt <- encode(m, "Count from one to twenty: one, two, three,", true, false)
            var none : array<int64>
            let greedy = SamplingParams()
            // reference: the PLAIN scheduler (mtp off), paged, 8-token prefill quanta
            var sch0 <- create_scheduler(m, 1l, 4l, 8l, 16l, [idle_chunk_tokens = 0l])
            var events0 : array<SchedEvent>
            t |> success(submit(m, sch0, make_req(1l, prompt, greedy, 24l, none)), "plain stream queued")
            drain(m, sch0, events0)
            var ref_toks <- toks_of(events0, 1l)
            delete sch0
            // spec: mtp on — the chunked prefill crosses the warm's seam rows, and the paged
            // sessions ride the pool's draft-block slab (the server default rail)
            let prev_spec = get_mtp_spec()
            set_mtp_spec(true)
            var sch <- create_scheduler(m, 1l, 4l, 8l, 16l, 0l, KVDtype.f16, true, 0l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(2l, prompt, greedy, 24l, none)), "spec stream queued")
            let census0 = batch_step_census()
            drain(m, sch, events)
            let census1 = batch_step_census()
            set_mtp_spec(prev_spec)
            //! the planar carrier has no joint-verify seat: every speculative tick steps its stream alone, and the census says so - the batched row's refusal reads it
            t |> success(census1.per_row - census0.per_row > 0l, "a speculative tick no joint verify carried counts as per-row ({census1.per_row - census0.per_row} per-row ticks)")
            t |> equal(census1.device, census0.device, "... and never as a device step")
            var got <- toks_of(events, 2l)
            to_log(LOG_INFO, "spec: \"{text_of(pieces_of(events, 2l))}\" {got} | plain: \"{text_of(pieces_of(events0, 1l))}\" {ref_toks}\n")
            t |> success(ids_equal(got, ref_toks), "spec stream tokens == plain scheduler")
            t |> success(sch.mtp_drafted > 0l, "the spec tick actually drafted ({sch.mtp_drafted} drafts)")
            t |> success(sch.mtp_accepted > 0l, "some drafts accepted ({sch.mtp_accepted} of {sch.mtp_drafted})")
            t |> equal(finished_of(events, 2l).n_gen, 24l)
            //! top-k 1 makes every draw the penalized argmax, so the seeded sampled stream is deterministic and must equal the plain scheduler's
            let sp = SamplingParams(temp = 0.7, top_k = 1l, penalty = 1.1)
            var sch1 <- create_scheduler(m, 1l, 4l, 8l, 16l, [idle_chunk_tokens = 0l])
            var events1 : array<SchedEvent>
            t |> success(submit(m, sch1, make_req(3l, prompt, sp, 24l, none, 4242)), "plain sampled stream queued")
            drain(m, sch1, events1)
            var ref_s <- toks_of(events1, 3l)
            delete sch1
            set_mtp_spec(true)
            var sch2 <- create_scheduler(m, 1l, 4l, 8l, 16l, 0l, KVDtype.f16, true, 0l)
            var events2 : array<SchedEvent>
            t |> success(submit(m, sch2, make_req(4l, prompt, sp, 24l, none, 4242)), "spec sampled stream queued")
            drain(m, sch2, events2)
            set_mtp_spec(prev_spec)
            var got_s <- toks_of(events2, 4l)
            log_streams("sampled plain", events1, 3l, "sampled spec", events2, 4l)
            t |> success(ids_equal(got_s, ref_s), "sampled spec stream tokens == plain scheduler (temp 0.7, top-k 1, penalty 1.1)")
            t |> success(sch2.mtp_drafted > 0l, "the sampled tick drafted ({sch2.mtp_drafted} drafts)")
            t |> success(sch2.mtp_accepted > 0l, "the sampled walk accepted drafts ({sch2.mtp_accepted} of {sch2.mtp_drafted})")
            t |> equal(finished_of(events2, 4l).n_gen, 24l)
            // penalized greedy: the plain scheduler picks the PENALIZED argmax every step, so the
            // spec tick must draw its verify rows through the sampler too - a raw-argmax walk would
            // accept drafts the penalty rejects
            let gp = SamplingParams(penalty = 1.3)
            var sch3 <- create_scheduler(m, 1l, 4l, 8l, 16l, [idle_chunk_tokens = 0l])
            var events3 : array<SchedEvent>
            t |> success(submit(m, sch3, make_req(5l, prompt, gp, 24l, none)), "plain penalized-greedy stream queued")
            drain(m, sch3, events3)
            var ref_p <- toks_of(events3, 5l)
            delete sch3
            set_mtp_spec(true)
            var sch4 <- create_scheduler(m, 1l, 4l, 8l, 16l, 0l, KVDtype.f16, true, 0l)
            var events4 : array<SchedEvent>
            t |> success(submit(m, sch4, make_req(6l, prompt, gp, 24l, none)), "spec penalized-greedy stream queued")
            drain(m, sch4, events4)
            set_mtp_spec(prev_spec)
            var got_p <- toks_of(events4, 6l)
            log_streams("penalized-greedy plain", events3, 5l, "penalized-greedy spec", events4, 6l)
            t |> success(ids_equal(got_p, ref_p), "penalized-greedy spec stream tokens == plain scheduler (penalty 1.3, temp 0)")
            t |> success(sch4.mtp_drafted > 0l, "the penalized-greedy tick drafted ({sch4.mtp_drafted} drafts)")
            delete got_p
            delete ref_p
            delete events4
            delete events3
            delete sch4
            delete got_s
            delete ref_s
            delete events2
            delete events1
            delete sch2
            delete got
            delete events
            delete sch
            delete ref_toks
            delete events0
            delete prompt
            delete m
        }
        set_prefill_attn_mode(prev_attn)
        set_act_vec(prev_act)
        set_deltanet_prefill_mode(prev_dn)
    }
}

// ===== the speculative tick under the resident driver: a greedy stream asks a pick every tick and consumes it =====

[test]
def test_scheduler_mtp_resident_picks(t : T?) {
    t |> run("scheduler mtp under the resident driver: every tick of a greedy stream asks a pick - the device verify lands picks alone and sample_ consumes each - the stream token for token the plain scheduler's on the same device rail, every round's verify served on the device (needs Qwen3.5-0.8B-MTP, DASLLAMA_GPU=1)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MTP_MODEL_PATH).is_valid) {
            t |> skip("{MTP_MODEL_PATH} not present")
            return
        }
        if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
            t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
            return
        }
        if (mirror_kv_dtype() != KVDtype.f16) {
            t |> skip("the scheduler's sessions are f16; under DASLLAMA_VK_KV32=1 the mirror is f32 and every call passes as codec")
            return
        }
        with_dasllama_jobque() {
            var m <- load_model_(MTP_MODEL_PATH, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, 512l)
            t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the NextN-headed hybrid (the load log names any decline)")
            var prompt <- encode(m, "Count from one to twenty: one, two, three,", true, false)
            var none : array<int64>
            let greedy = SamplingParams()
            var sch0 <- create_scheduler(m, 1l, 4l, 64l)   // the plain scheduler on unpaged f16 sessions: the device rail, its picks asked a step
            var events0 : array<SchedEvent>
            t |> success(submit(m, sch0, make_req(1l, prompt, greedy, 24l, none)), "plain stream queued")
            drain(m, sch0, events0)
            var ref_toks <- toks_of(events0, 1l)
            delete sch0
            let prev_spec = get_mtp_spec()
            set_mtp_spec(true)
            let verifies0 = moe_gpu_resident_verifies()
            var sch <- create_scheduler(m, 1l, 4l, 64l, 0l, 0l, KVDtype.f16, true)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(2l, prompt, greedy, 24l, none)), "spec stream queued")
            drain(m, sch, events)
            set_mtp_spec(prev_spec)
            var got <- toks_of(events, 2l)
            log_streams("plain", events0, 1l, "spec (resident)", events, 2l)
            t |> success(ids_equal(got, ref_toks), "the spec stream under the resident driver == the plain scheduler's stream ({got} vs {ref_toks})")
            t |> equal(finished_of(events, 2l).n_gen, 24l)
            t |> success(sch.mtp_drafted > 0l, "the speculative tick drafted ({sch.mtp_drafted} drafts, {sch.mtp_accepted} accepted)")
            let served = moe_gpu_resident_verifies() - verifies0
            t |> equal(served, sch.mtp_drafted, "every drafted round's verify served on the device under the tick's pick ask ({served} of {sch.mtp_drafted}); a pick left unconsumed would have refused the next round's opening")
            delete got
            delete events
            delete sch
            delete ref_toks
            delete events0
            delete prompt
            delete m
        }
    }
}

// ===== paged + SAMPLED: the combination the server actually serves =====

// A sampled stream consults the RNG once per token, so an extra or missing sample() call in the
// paged path shows up as a diverged stream, not as a crash.
[test]
def test_scheduler_paged_sampled(t : T?) {
    t |> run("paged scheduler, seeded sampling: stream == generate() (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)   // the warm run re-chunks (tail-only)
            var prompt <- encode(m, "Once upon a time in a small village near the mountains there lived a very curious little robot who loved counting stars", true, false)
            var none : array<int64>
            let sp = SamplingParams(temp = 0.8, top_k = 40l)
            var ref <- ref_generate(m, prompt, sp, 12l, none, 4242)
            t |> success(!empty(ref.toks), "the sampled reference produced tokens")
            var sch <- create_scheduler(m, 1l, 4l, 64l, 16l)   // paged, P=16, unbounded prefix cache
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(1l, prompt, sp, 12l, none, 4242)), "cold sampled stream queued")
            drain(m, sch, events)
            var cold <- toks_of(events, 1l)
            t |> equal(finished_of(events, 1l).n_cached, 0l)
            t |> success(ids_equal(cold, ref.toks), "paged cold sampled stream == generate()")
            // and again over a WARM cache: the attach must not perturb the sampler either
            events |> clear()
            t |> success(submit(m, sch, make_req(2l, prompt, sp, 12l, none, 4242)), "warm sampled stream queued")
            drain(m, sch, events)
            var warm <- toks_of(events, 2l)
            t |> success(finished_of(events, 2l).n_cached > 0l, "the warm run attached cached pages")
            t |> success(ids_equal(warm, ref.toks), "paged WARM sampled stream == generate()")
            set_prefill_attn_mode(saved_mode)
            delete cold
            delete warm
            delete events
            delete sch
            delete ref
            delete prompt
            delete m
        }
    }
}

// ===== media splice: soft-token rows prefill between two token spans =====

// Build a media request whose ROWS are the prompt's own embedding rows for [lo, hi): the tokens
// outside the span stay ids, so the stream must reproduce the all-token reference exactly. Any
// splice bug — wrong position, a head chunk running past media_at, rows evaled twice or never —
// changes the KV and diverges.
def private media_req(m : Model; id : int64; prompt : array<int64>; lo, hi : int64;
                      non_causal : bool; params : SamplingParams; max_new : int64;
                      grid : int2 = int2(0)) : PendingReq {
    let np = long_length(prompt)
    var r <- PendingReq(id = id, params = params, max_new = max_new,
                        media_at = lo, n_media = hi - lo, media_non_causal = non_causal,
                        media_grid = grid)
    r.prompt <- [for (k in range64(np)); prompt[k]; where k < lo || k >= hi]
    var mid <- [for (k in range64(lo, hi)); prompt[k]]
    r.media_embd |> resize((hi - lo) * m.config.dim)
    embed_text_rows(m, mid, r.media_embd, 0l)
    delete mid
    return <- r
}

// The whole prompt as embedding rows — `generate_embd`'s non-causal-span form is the reference
// the scheduler's non-causal media arm must reproduce (chat.das's image turn runs on it).
def private embd_span_generate(m : Model; prompt : array<int64>; lo, hi : int64;
                               params : SamplingParams; max_new : int64; seed : int = 0) : array<int64> {
    var s <- create_session(m)
    if (seed != 0) {
        set_seed(s, seed)
    }
    var e : array<float>
    e |> reserve(long_length(prompt) * m.config.dim)
    e |> resize(long_length(prompt) * m.config.dim)
    embed_text_rows(m, prompt, e, 0l)
    var out : array<int64>
    generate_embd(m, s, e, long_length(prompt), params, max_new, lo, hi) $(id, _pc) {
        out |> push(id)
        return true
    }
    delete e
    delete s
    return <- out
}

let private SPAN_SEED = 4242
let private SPAN_MOVES_LOGITS = 0.02   //! of the causal row's peak: the whole-prompt span reads 0.10 of it (3.1 at 29.8)

//! the last row's logits after the same embedding prefill: rows [lo, hi) a non-causal span, every row causal where hi <= lo
def private embd_span_logits(m : Model; prompt : array<int64>; lo, hi : int64) : array<float> {
    var s <- create_session(m)
    var e : array<float>
    e |> reserve(long_length(prompt) * m.config.dim)
    e |> resize(long_length(prompt) * m.config.dim)
    embed_text_rows(m, prompt, e, 0l)
    if (hi > lo) {
        eval_embd_span_(m, s, e, long_length(prompt), lo, hi)
    } else {
        eval_embd_(m, s, e, long_length(prompt))
    }
    var out := s.logits
    delete e
    delete s
    return <- out
}

[test]
def test_scheduler_media_splice(t : T?) {   // nolint:STYLE038 — one model load per test: every assertion runs against that model inside the same jobque scope
    t |> run("media rows == the same tokens as ids: splice at head/middle/tail (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            // classic prefill: the splice necessarily moves the eval boundaries, and only classic
            // is bit-exact under re-chunking — the same pin as the chunk-size arm above
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery and counts every bird that crosses the evening sky", true, false)
            let np = long_length(prompt)
            t |> success(np > 12l, "prompt is long enough to splice ({np} tokens)")
            var none : array<int64>
            let greedy = SamplingParams()
            var base <- ref_generate(m, prompt, greedy, 6l, none)
            t |> success(!empty(base.toks), "the all-token reference produced tokens")
            // head (no leading tokens), middle (both spans), tail (no trailing tokens)
            for (arm, lo, hi in ["head", "middle", "tail"], [0l, 4l, np - 5l], [5l, 4l + 6l, np]) {
                var sch <- create_scheduler(m, 1l, 4l, 64l)
                var inscope events : array<SchedEvent>
                var req <- media_req(m, 1l, prompt, lo, hi, false, greedy, 6l)
                t |> success(submit(m, sch, req), "media stream queued (arm {arm}: rows [{lo},{hi}))")
                drain(m, sch, events)
                var inscope got <- toks_of(events, 1l)
                t |> success(ids_equal(got, base.toks), "arm {arm}: media stream == the all-token stream")
                t |> equal(finished_of(events, 1l).n_prompt, np)   // usage counts the rows as positions
                t |> equal(sch.prefill_evals, 1l, "arm {arm}: the span and the text around it are one prefill call")
                delete got
                delete events
                delete sch
            }
            // a chunk of text is all a span's call carries: the head rides it, the tail past the chunk evals on its own
            var sch_cut <- create_scheduler(m, 1l, 4l, 8l, idle_chunk_tokens = 0l)
            var events_cut : array<SchedEvent>
            var req_cut <- media_req(m, 2l, prompt, 4l, 10l, false, greedy, 6l)
            t |> success(submit(m, sch_cut, req_cut), "chunk-bounded media stream queued")
            drain(m, sch_cut, events_cut)
            var got_cut <- toks_of(events_cut, 2l)
            t |> success(ids_equal(got_cut, base.toks), "a body cut at the chunk == the all-token stream")
            t |> equal(sch_cut.prefill_evals, 1l + (np - 14l + 7l) / 8l, "the body takes the head, the span and four tail tokens; the rest goes a chunk a call")
            delete got_cut
            delete events_cut
            delete sch_cut
            // a non-causal span with text either side is generate_embd's span form in one call
            var mid_ref <- embd_span_generate(m, prompt, 4l, 10l, SamplingParams(temp = 0.8, top_k = 40l), 8l, SPAN_SEED)
            var sch_mid <- create_scheduler(m, 1l, 4l, 64l)
            var events_mid : array<SchedEvent>
            var req_mid <- media_req(m, 3l, prompt, 4l, 10l, true, SamplingParams(temp = 0.8, top_k = 40l), 8l)
            req_mid.seed = SPAN_SEED
            t |> success(submit(m, sch_mid, req_mid), "non-causal mid-prompt media stream queued")
            drain(m, sch_mid, events_mid)
            var got_mid <- toks_of(events_mid, 3l)
            t |> success(ids_equal(got_mid, mid_ref), "a non-causal span between text == generate_embd's span form")
            t |> equal(sch_mid.prefill_evals, 1l)
            delete got_mid
            delete events_mid
            delete sch_mid
            delete mid_ref
            // the non-causal flag reaches the kernel THROUGH the scheduler: with it set the arm must
            // match generate_embd's span form, not the causal one. The span is [1, np) — everything
            // past BOS — and the streams are sampled under one seed: the span moves the prompt's
            // logits three points at a thirty-point peak, which leaves the greedy top-1 of eight
            // tokens unmoved on some kernel builds, and the check goes vacuous there
            let lo = 1l
            let hi = np
            let sampled = SamplingParams(temp = 0.8, top_k = 40l)
            var causal_ref <- embd_span_generate(m, prompt, 0l, 0l, sampled, 8l, SPAN_SEED)
            var span_ref <- embd_span_generate(m, prompt, lo, hi, sampled, 8l, SPAN_SEED)
            var causal_logits <- embd_span_logits(m, prompt, 0l, 0l)
            var span_logits <- embd_span_logits(m, prompt, lo, hi)
            let moved = logits_maxdiff(span_logits, causal_logits)
            to_log(LOG_INFO, "the span form's stream: {eyeball_text(m, span_ref)}\n")
            to_log(LOG_INFO, "the causal form's stream: {eyeball_text(m, causal_ref)}\n")
            to_log(LOG_INFO, "the prompt's logits: the span form {moved} from the causal form at a {logits_maxabs(causal_logits)} peak\n")
            t |> success(moved > SPAN_MOVES_LOGITS * logits_maxabs(causal_logits), "the span moves the prompt's logits ({moved} at a {logits_maxabs(causal_logits)} peak)")
            delete causal_logits
            delete span_logits
            t |> success(!ids_equal(span_ref, causal_ref), "the span moves the stream (the flag is load-bearing here)")
            var sch_nc <- create_scheduler(m, 1l, 4l, 64l)
            var events_nc : array<SchedEvent>
            var req_nc <- media_req(m, 7l, prompt, lo, hi, true, sampled, 8l)
            req_nc.seed = SPAN_SEED
            t |> success(submit(m, sch_nc, req_nc), "non-causal media stream queued")
            drain(m, sch_nc, events_nc)
            var got_nc <- toks_of(events_nc, 7l)
            t |> success(ids_equal(got_nc, span_ref), "non-causal arm == generate_embd's span form")
            delete causal_ref
            delete got_nc
            delete events_nc
            delete sch_nc
            delete span_ref

            // paged: a media span's positions enter the prompt as ids of its rows' content, so the prefix cache
            // matches across a span as across text. Prime with a plain stream carrying the media stream's text.
            let mlo = 20l
            let mhi = 26l
            var schp <- create_scheduler(m, 1l, 4l, 64l, 16l)
            var ev : array<SchedEvent>
            var text_only <- [for (k in range64(np)); prompt[k]; where k < mlo || k >= mhi]
            t |> success(long_length(text_only) > 16l, "the priming prompt fills a page ({length(text_only)} tokens)")
            t |> success(submit(m, schp, make_req(9l, text_only, greedy, 4l, none)), "priming text stream queued")
            drain(m, schp, ev)
            let primed = prefix_held_groups(schp.prefix)
            t |> success(primed > 0l, "the priming stream donated its pages")
            ev |> clear()
            var r1 <- media_req(m, 10l, prompt, mlo, mhi, false, greedy, 6l)
            t |> success(submit(m, schp, r1), "paged media stream queued")
            drain(m, schp, ev)
            t |> equal(finished_of(ev, 10l).n_cached, mlo)   // the text ahead of the span attaches; the span's ids are no token's, so the hit stops there
            var paged_toks <- toks_of(ev, 10l)
            t |> success(ids_equal(paged_toks, base.toks), "paged media stream == the all-token stream")
            t |> success(prefix_held_groups(schp.prefix) > primed, "the media stream donates its pages")
            // the same media again: everything but the last position attaches, the span included
            ev |> clear()
            var r2 <- media_req(m, 11l, prompt, mlo, mhi, false, greedy, 6l)
            t |> success(submit(m, schp, r2), "the same media stream queued again")
            drain(m, schp, ev)
            t |> success(finished_of(ev, 11l).n_cached > mhi, "asked again, the span attaches off the cache ({finished_of(ev, 11l).n_cached} of {np} positions)")
            var again_toks <- toks_of(ev, 11l)
            t |> success(ids_equal(again_toks, base.toks), "the attached span reads the same stream")
            // ...and with no rows at all: the request counts on the cache for the span
            ev |> clear()
            var r3 <- media_req(m, 12l, prompt, mlo, mhi, false, greedy, 6l)
            splice_to_span(r3)
            delete r3.spans[0].rows
            t |> success(submit(m, schp, r3), "a rowless request queued")
            drain(m, schp, ev)
            var rowless_toks <- toks_of(ev, 12l)
            t |> equal(finished_of(ev, 12l).finish, "length")
            t |> success(ids_equal(rowless_toks, base.toks), "a span the cache holds needs no rows")
            // other rows at the same place share only the text ahead of them (the control)
            ev |> clear()
            var other <- [for (k in range64(np)); k >= mlo && k < mhi ? prompt[np - 1l - (k - mlo)] : prompt[k]]
            var r4 <- media_req(m, 13l, other, mlo, mhi, false, greedy, 6l)
            t |> success(submit(m, schp, r4), "a stream with other rows queued")
            drain(m, schp, ev)
            t |> equal(finished_of(ev, 13l).n_cached, mlo)
            // a rowless request on a cache that never saw the span is told so, and donates nothing
            var schl <- create_scheduler(m, 1l, 4l, 64l, 16l)
            var evl : array<SchedEvent>
            var r5 <- media_req(m, 14l, prompt, mlo, mhi, false, greedy, 6l)
            splice_to_span(r5)
            delete r5.spans[0].rows
            t |> success(submit(m, schl, r5), "a rowless request on a cold cache queued")
            drain(m, schl, evl)
            t |> equal(finished_of(evl, 14l).finish, "media_lost")
            t |> equal(prefix_held_groups(schl.prefix), 0l)
            delete evl
            delete schl
            delete other
            delete rowless_toks
            delete again_toks
            delete paged_toks
            delete text_only
            delete ev
            delete schp
            set_prefill_attn_mode(saved_mode)
            delete base
            delete prompt
            delete m
        }
    }
}

// ===== media validation: submit refuses malformed shapes by panic =====

[test]
def test_scheduler_device_mode_switch(t : T?) {
    t |> run("only an idle scheduler switches its session kind (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            var prompt <- encode(m, "The quick brown fox", true, false)
            var none : array<int64>
            var sch <- create_scheduler(m, 1l, 4l, 64l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(1l, prompt, SamplingParams(), 4l, none)), "a request queued")
            scheduler_step(m, sch, events)
            t |> success(!set_device_kv(sch, true), "a scheduler with a live stream keeps the session kind it has")
            drain(m, sch, events)
            t |> success(set_device_kv(sch, true), "the same scheduler, idle, switches")
            t |> success(!set_device_kv(sch, false), "and back")
            delete events
            delete sch
            delete prompt
            delete m
        }
    }
    t |> run("submit refuses a media request in device mode, and takes it with the mode off (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            var prompt <- encode(m, "The quick brown fox", true, false)
            var sch <- create_scheduler(m, 1l, 4l, 64l)
            var taken = false
            var tripped = false
            for (device in fixed_array(false, true)) {
                set_device_kv(sch, device)
                tripped = false
                try {
                    var req <- PendingReq(id = device ? 2l : 1l, params = SamplingParams(), max_new = 1l, media_at = 1l, n_media = 2l)
                    req.prompt := prompt
                    req.media_embd |> resize(2l * m.config.dim)
                    taken = submit(m, sch, req)
                } recover {
                    tripped = true
                }
                if (device) {
                    t |> success(tripped, "device mode: a device-home session serves no media span, so submit panics")
                } else {
                    t |> success(taken && !tripped, "host-cached mode takes the same request (the control)")
                    var inscope events : array<SchedEvent>
                    drain(m, sch, events)
                    delete events
                }
            }
            set_device_kv(sch, false)
            delete sch
            delete prompt
            delete m
        }
    }
}

[test]
def test_scheduler_media_validation(t : T?) {
    t |> run("submit panics on malformed media shapes (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            var prompt <- encode(m, "The quick brown fox", true, false)
            let greedy = SamplingParams()
            var sch <- create_scheduler(m, 1l, 4l, 64l)
            var tripped = false
            try {
                // a negative count clears every guard, lowers the seq_len check, and leaves a
                // stream whose completion predicate can never hold - an infinite step loop
                var bad <- PendingReq(id = 9l, params = greedy, max_new = 4l, n_media = -1l)
                bad.prompt := prompt
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "negative n_media panics")
            tripped = false
            try {
                var bad <- PendingReq(id = 10l, params = greedy, max_new = 4l, media_at = 99l, n_media = 2l)
                bad.prompt := prompt
                bad.media_embd |> resize(2l * m.config.dim)
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "media_at outside the prompt panics")
            tripped = false
            try {
                var bad <- PendingReq(id = 11l, params = greedy, max_new = 4l, media_at = 1l, n_media = 2l)
                bad.prompt := prompt
                bad.media_embd |> resize(4l)   // far short of n_media * dim
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "undersized media rows panic")
            tripped = false
            try {
                var bad <- PendingReq(id = 12l, params = greedy, max_new = 4l)
                bad.prompt := prompt
                bad.media_embd |> resize(2l * m.config.dim)   // rows with n_media == 0
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "media rows with n_media == 0 panic instead of dropping silently")
            tripped = false
            try {
                var bad <- PendingReq(id = 13l, params = greedy, max_new = 4l, media_at = 1l, n_media = 2l)
                bad.prompt := prompt
                bad.media_embd |> resize(3l * m.config.dim)   // oversized: the exact-width guard refuses slop
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "oversized media rows panic")
            tripped = false
            try {
                m.config.n_deepstack = 3l   // a deepstack decoder: image rows arrive (1+n)·dim-wide
                var bad <- PendingReq(id = 14l, params = greedy, max_new = 4l, media_at = 1l, n_media = 2l, media_non_causal = true)
                bad.prompt := prompt
                bad.media_embd |> resize(2l * m.config.dim)   // narrow feed = the slice adds would silently skip
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            m.config.n_deepstack = 0l
            t |> success(tripped, "narrow image rows on a deepstack decoder panic")
            delete sch
            delete prompt
            delete m
        }
    }
}

// ===== mrope media: the qwen grid reaches the quantum's angles and the batched decode =====

// embd_span_generate's mrope twin (generate_embd's grid form) — the reference the scheduler's
// media_grid arm must reproduce, decode included: the session's rope_pos_delta shifts every
// post-image position, so a decode that ropes at n_past instead diverges.
def private embd_mrope_generate(m : Model; prompt : array<int64>; lo, hi : int64; grid : int2;
                                params : SamplingParams; max_new : int64) : array<int64> {
    var s <- create_session(m)
    var e : array<float>
    e |> reserve(long_length(prompt) * m.config.dim)
    e |> resize(long_length(prompt) * m.config.dim)
    embed_text_rows(m, prompt, e, 0l)
    var out : array<int64>
    generate_embd(m, s, e, long_length(prompt), params, max_new, lo, hi, grid) $(id, _pc) {
        out |> push(id)
        return true
    }
    delete e
    delete s
    return <- out
}

//! Paged: the grid span donates, and the same request asked again attaches it off the cache - the session's rope advance
//! comes back with it, or every position past the span ropes that advance off. Both streams run the flat reference's lane
//! (classic prefill, the std decode, the CPU rails - the caller's pins); the paged K/V layout alone differs.
def private mrope_paged_leg(t : T?; m : Model; prompt : array<int64>; lo, hi : int64; grid : int2; mref : array<int64>) {
    var schp <- create_scheduler(m, 1l, 4l, 64l, 16l)
    var ep : array<SchedEvent>
    for (id in fixed_array(31l, 32l)) {
        var rqp <- media_req(m, id, prompt, lo, hi, true, SamplingParams(), 8l, grid)
        t |> success(submit(m, schp, rqp), "paged mrope stream {id} queued")
        scheduler_step(m, schp, ep)   // admits the stream and prefills what the cache does not hold
        t |> success(length(schp.streams) == 1 && schp.streams[0].session.rope_pos_delta == int64(max(grid.x, grid.y)) - (hi - lo),
            "paged mrope stream {id}: the session ropes past the span by the grid's advance ({max(grid.x, grid.y)} positions for {hi - lo} rows)")
        drain(m, schp, ep)
        var inscope gp <- toks_of(ep, id)
        to_log(LOG_INFO, "paged mrope stream {id} (cached {finished_of(ep, id).n_cached}): {eyeball_text(m, gp)}\nits grid reference: {eyeball_text(m, mref)}\n")
        t |> success(ids_equal(gp, mref), "paged mrope stream {id} == the grid reference")
        delete gp
    }
    t |> equal(finished_of(ep, 31l).n_cached, 0l, "the first paged stream finds a cold cache")
    t |> success(finished_of(ep, 32l).n_cached >= hi, "asked again, the grid span attaches off the cache ({finished_of(ep, 32l).n_cached} of {length(prompt)} positions, the span ends at {hi})")
    delete ep
    delete schp
}

[test]
def test_scheduler_mrope(t : T?) {
    t |> run("media_grid: the mrope walk reaches the quantum and survives the batched decode (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            m.config.rope_sections = int4(12, 10, 10, 0)   // stamp: the toy is not an mrope arch (rotary 64 -> half 32)
            m.config.mrope_interleaved = true
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            let prev_fused = get_fused_decode()
            set_fused_decode(false)   // the batch worker mirrors the std decode path (test_scheduler_batching's pin)
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery", true, false)
            let np = long_length(prompt)
            let lo = 4l
            let hi = lo + 12l   // 12 rows = a 4x3 merged grid; position advance 4, so rope_pos_delta lands at -8
            let grid = int2(4, 3)
            t |> success(np > hi + 2l, "prompt long enough to leave a tail after the span ({np} tokens)")
            let greedy = SamplingParams()
            var none : array<int64>
            var mref <- embd_mrope_generate(m, prompt, lo, hi, grid, greedy, 8l)
            var sref <- embd_span_generate(m, prompt, lo, hi, greedy, 8l)
            t |> success(!ids_equal(mref, sref), "grid angles move the stream (the mrope walk is load-bearing here)")
            // (1) the grid reaches the quantum: scheduler stream == generate_embd's grid form
            var sch <- create_scheduler(m, 1l, 4l, 64l)
            var events : array<SchedEvent>
            var req <- media_req(m, 1l, prompt, lo, hi, true, greedy, 8l, grid)
            t |> success(submit(m, sch, req), "mrope media stream queued")
            drain(m, sch, events)
            var got <- toks_of(events, 1l)
            t |> success(ids_equal(got, mref), "scheduler mrope stream == generate_embd's grid form")
            t |> equal(finished_of(events, 1l).n_prompt, np)
            t |> equal(sch.mrope_streams, 1l)   // the telemetry counter the server stats surface reads
            // (2) batched decode: the mrope stream and a text stream decode as one 2-row batch —
            // the delta'd rope position must survive the gather (ws.rope_positions, not n_past)
            var tref <- ref_generate(m, prompt, greedy, 8l, none)
            var schb <- create_scheduler(m, 2l, 4l, 64l)
            var eb : array<SchedEvent>
            var rq1 <- media_req(m, 21l, prompt, lo, hi, true, greedy, 8l, grid)
            t |> success(submit(m, schb, rq1), "mrope stream queued (batch arm)")
            t |> success(submit(m, schb, make_req(22l, prompt, greedy, 8l, none)), "text stream queued (batch arm)")
            drain(m, schb, eb)
            var g1 <- toks_of(eb, 21l)
            var g2 <- toks_of(eb, 22l)
            t |> success(ids_equal(g1, mref), "batched mrope stream == its solo grid reference")
            t |> success(ids_equal(g2, tref.toks), "batched text stream == its solo reference")
            t |> success(schb.sum_batch >= 4l, "decode steps actually batched (sum batch {schb.sum_batch})")
            mrope_paged_leg(t, m, prompt, lo, hi, grid, mref)
            set_fused_decode(prev_fused)
            set_prefill_attn_mode(saved_mode)
            delete g1
            delete g2
            delete eb
            delete schb
            delete tref
            delete got
            delete events
            delete sch
            delete mref
            delete sref
            delete prompt
            delete m
        }
    }
}

// ===== mrope validation: submit refuses malformed grids by panic =====

[test]
def test_scheduler_mrope_validation(t : T?) {
    t |> run("submit panics on malformed media grids (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            var prompt <- encode(m, "The quick brown fox", true, false)
            let greedy = SamplingParams()
            var sch <- create_scheduler(m, 1l, 4l, 64l)
            var tripped = false
            try {
                var bad <- media_req(m, 30l, prompt, 1l, 3l, true, greedy, 4l, int2(3, 1))   // 3 rows shaped for 2
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "a grid shaping more rows than n_media panics")
            tripped = false
            try {
                var bad <- media_req(m, 31l, prompt, 1l, 3l, false, greedy, 4l, int2(2, 1))   // causal rows
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "a grid on causal media rows panics")
            tripped = false
            try {
                var bad <- media_req(m, 32l, prompt, 1l, 3l, true, greedy, 4l, int2(2, 0))   // one-sided grid
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "a one-sided grid panics")
            tripped = false
            try {
                var bad <- PendingReq(id = 33l, params = greedy, max_new = 4l, media_grid = int2(2, 1))
                bad.prompt := prompt
                submit(m, sch, bad)
            } recover {
                tripped = true
            }
            t |> success(tripped, "a grid with n_media == 0 panics instead of dropping silently")
            delete sch
            delete prompt
            delete m
        }
    }
}

// ===== inline spans: a prefix hit stops short of a span it would cut, and submit refuses malformed spans by panic =====

//! [lo, hi) of `prompt` INLINE as one non-causal span: the rows are the prompt's own embedding rows, the position ids
//! `key_a`'s for the span's first `split` rows and `key_b`'s past them
def private inline_span_req(m : Model; id : int64; prompt : array<int64>; lo, hi, split : int64; key_a, key_b : uint64;
                            params : SamplingParams; max_new : int64) : PendingReq {
    var r <- PendingReq(id = id, params = params, max_new = max_new)
    r.prompt <- [for (k in range64(long_length(prompt))); (k < lo || k >= hi) ? prompt[k] : media_position_id(k - lo < split ? key_a : key_b, k - lo)]
    var mid <- [for (k in range64(lo, hi)); prompt[k]]
    var sp <- MediaSpan(at = lo, n = hi - lo, non_causal = true)
    sp.rows |> resize((hi - lo) * m.config.dim)
    embed_text_rows(m, mid, sp.rows, 0l)
    r.spans |> emplace(sp)
    delete mid
    return <- r
}

[test]
def test_scheduler_span_cut(t : T?) {
    t |> run("a prefix hit that ends inside a span is no hit: the span evals whole (needs SmolLM2-135M)") @(t : T?) {
        return if (model_missing(t))
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)   // the span's call moves the eval boundaries: classic alone is bit-exact under them
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery and counts every bird that crosses the evening sky", true, false)
            let np = long_length(prompt)
            let lo = 4l
            let hi = 24l
            let split = 10l   // the second request shares the text and the span's first 10 ids: its hit would end at 14, inside [4, 24)
            t |> success(np > hi + 4l, "the prompt leaves a tail past the span ({np} tokens)")
            var other <- [for (k in range64(np)); (k >= lo + split && k < hi) ? prompt[np - 1l - (k - lo - split)] : prompt[k]]
            t |> success(!ids_equal(other, prompt), "the second span's rows past the split are other tokens'")
            let greedy = SamplingParams()
            var ref_a <- embd_span_generate(m, prompt, lo, hi, greedy, 6l)
            var ref_b <- embd_span_generate(m, other, lo, hi, greedy, 6l)
            var sch <- create_scheduler(m, 1l, 4l, 64l, 16l)
            var ev : array<SchedEvent>
            var ra <- inline_span_req(m, 1l, prompt, lo, hi, hi - lo, 0xa11ul, 0xa11ul, greedy, 6l)
            t |> success(submit(m, sch, ra), "the first span stream queued")
            drain(m, sch, ev)
            var got_a <- toks_of(ev, 1l)
            to_log(LOG_INFO, "span stream A: {eyeball_text(m, got_a)}\nits reference: {eyeball_text(m, ref_a)}\n")
            t |> success(ids_equal(got_a, ref_a), "the first span stream == generate_embd's span form")
            t |> success(prefix_held_groups(sch.prefix) > 0l, "the first stream donates its pages")
            var rb <- inline_span_req(m, 2l, other, lo, hi, split, 0xa11ul, 0xb22ul, greedy, 6l)
            t |> success(submit(m, sch, rb), "the second span stream queued")
            drain(m, sch, ev)
            var got_b <- toks_of(ev, 2l)
            to_log(LOG_INFO, "span stream B (cached {finished_of(ev, 2l).n_cached}): {eyeball_text(m, got_b)}\nits reference: {eyeball_text(m, ref_b)}\n")
            t |> success(finished_of(ev, 2l).n_cached <= lo, "a hit ending inside the span attaches nothing past the span's start ({finished_of(ev, 2l).n_cached}, the span opens at {lo})")
            t |> success(ids_equal(got_b, ref_b), "the second span stream == its own generate_embd span form")
            delete got_b
            delete rb
            delete got_a
            delete ra
            delete ev
            delete sch
            delete ref_b
            delete ref_a
            delete other
            delete prompt
            set_prefill_attn_mode(saved_mode)
            delete m
        }
    }
}

//! whether `submit` refuses `req` by panic
def private submit_panics(m : Model; var sch : Scheduler; var req : PendingReq) : bool {
    var tripped = false
    try {
        submit(m, sch, req)
    } recover {
        tripped = true
    }
    return tripped
}

//! a well-formed request: the prompt's [4, 10) inline as one span under one key
def private span_ok_req(m : Model; id : int64; prompt : array<int64>) : PendingReq =>
    <- inline_span_req(m, id, prompt, 4l, 10l, 6l, 0x5a5ul, 0x5a5ul, SamplingParams(), 4l)

//! the well-formed request's prompt with its one span replaced by rowless spans at `spans`' (at, n) pairs
def private spans_req(m : Model; id : int64; prompt : array<int64>; spans : array<int2>) : PendingReq {
    var r <- span_ok_req(m, id, prompt)
    delete r.spans
    r.spans |> reserve(length(spans))
    for (s in spans) {
        r.spans |> emplace(MediaSpan(at = int64(s.x), n = int64(s.y), non_causal = true))
    }
    return <- r
}

[test]
def test_scheduler_span_validation(t : T?) {
    t |> run("submit panics on malformed inline spans (needs SmolLM2-135M)") @(t : T?) {
        return if (model_missing(t))
        with_dasllama_jobque() {
            var m <- load_capped_cpu(128l)
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches", true, false)
            let np = long_length(prompt)
            t |> success(np > 12l, "the prompt holds the span and text past it ({np} tokens)")
            var sch <- create_scheduler(m, 1l, 64l, 64l)
            var ok <- span_ok_req(m, 40l, prompt)
            t |> success(!submit_panics(m, sch, ok), "a well-formed span submits (the control)")
            var empty_span <- spans_req(m, 41l, prompt, [int2(5, 0)])   // both rows it reads, [4] and [5], are ids
            t |> success(submit_panics(m, sch, empty_span), "a span of no rows panics")
            var backward <- spans_req(m, 42l, prompt, [int2(8, 2), int2(4, 2)])
            t |> success(submit_panics(m, sch, backward), "spans out of prompt order panic")
            var overlap <- spans_req(m, 43l, prompt, [int2(4, 4), int2(6, 4)])
            t |> success(submit_panics(m, sch, overlap), "overlapping spans panic")
            var past <- spans_req(m, 44l, prompt, [int2(int(np) - 2, 3)])
            t |> success(submit_panics(m, sch, past), "a span past the prompt's end panics")
            var on_text <- spans_req(m, 45l, prompt, [int2(3, 7)])
            t |> success(submit_panics(m, sch, on_text), "a span whose first row is a token panics")
            var off_end <- spans_req(m, 46l, prompt, [int2(4, 7)])
            t |> success(submit_panics(m, sch, off_end), "a span whose last row is a token panics")
            var wide <- span_ok_req(m, 47l, prompt)
            wide.spans[0].rows |> push(0.0)
            t |> success(submit_panics(m, sch, wide), "rows one float past the span's width panic")
            var narrow <- span_ok_req(m, 48l, prompt)
            narrow.spans[0].rows |> resize(length(narrow.spans[0].rows) - int(m.config.dim))
            t |> success(submit_panics(m, sch, narrow), "rows a row short of the span panic")
            var both <- span_ok_req(m, 49l, prompt)
            both.n_media = 2l
            both.media_at = np   // a splice past the inline span: folded in, the spans would stand in order
            both.media_embd |> resize(2l * m.config.dim)
            t |> success(submit_panics(m, sch, both), "a request with both a splice and inline spans panics")
            var schd <- create_scheduler(m, 1l, 64l, 64l)
            set_device_kv(schd, true)
            var dev <- span_ok_req(m, 50l, prompt)
            t |> success(submit_panics(m, schd, dev), "device mode: a device-home session serves no inline span either")
            set_device_kv(schd, false)
            // a panic inside submit's walk over the spans leaves the refused requests' span arrays locked, so they are
            // not deleted - the heap goes with the context
            delete schd
            delete ok
            delete sch
            delete prompt
            delete m
        }
    }
}
