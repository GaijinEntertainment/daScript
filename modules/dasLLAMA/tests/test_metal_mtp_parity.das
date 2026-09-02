options gen2
options persistent_heap   // + explicit deletes: each fixture frees before the next loads
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations + (on Apple) the metal overrides
require dasllama/dasllama_math   // set_wscale_f16 — the resident path reads the f32 scale plane
require dasllama/dasllama_image   // load_model_cached — the served (image) flavor for the GPU twin
require dasllama/dasllama_gguf   // set_mtp_head_path — the split-head fixture names its head explicitly
require dasllama/dasllama_batch   // eval_verify_batch_ + create_batch_workspace_ — the batch-rail verify arm
require dasllama/dasllama_mtp_gemma   // gemma_drafter_sidecar — the assistant drafter beside a gemma-4 target
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // attach_gemma_drafter — the gemma spec round rides the verify fixtures
require ?das_metal dasllama/dasllama_metal_decode   // stats + the forced-reject lever + shutdown
require ?das_metal dasllama/dasllama_metal_prefill   // the metal prefill override for the pair's prompts
require ?das_metal metal/das_metal_boost   // metal_live_object_count for the leak gate
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 — path_join, used only inside the das_metal static_if halves
require math
require strings
require _model_tier

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the planar prefill stage runs on the CPU by design (the July engagement shape)
}

// End-to-end gates for the metal MTP/NextN self-speculative rail (`metal_mtp_spec_eval`: draft +
// same-slab B=2 verify + accept/reject), GPU-vs-GPU on the same binary. Two oracles, per the
// tests/REVIEW.md contract: freeform text rides the FORCED-FEED logits-tolerance form (the plain
// metal step and the verify's row 0 consume the identical token every round, so their logits
// must agree within the kernel-swap envelope - a free-running comparison would fork at the
// first near-tie and prove nothing); counting prompts stay token-exact (spec free-run == plain
// free-run). A plain-vs-plain control pins the harness at exactly zero. Skips cleanly without
// the model, off-Apple, or with no Metal device; the large fixtures need DASLLAMA_PARITY_FULL.

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}

def private greedy_top(logits : array<float>; vocab : int64) : int64 {
    var best = 0l
    var bv = logits[0]
    for (v in range64(vocab)) {
        if (logits[v] > bv) {
            bv = logits[v]
            best = v
        }
    }
    return best
}

// plain top-2 margin: the tie distance an argmax flip crossed
def private top2_margin(a : array<float>; n : int64) : float {
    var m1 = -3.4e38
    var m2 = -3.4e38
    for (i in range64(n)) {
        let v = a[i]
        if (v > m1) {
            m2 = m1
            m1 = v
        } elif (v > m2) {
            m2 = v
        }
    }
    return m1 - m2
}

// "start, start+1, …, last," — the kernel-robust confident fixture family (test_parity.das)
def private counting_prompt(start, last : int) : string {
    return build_string() $(var w) {
        for (i in range(start, last + 1)) {
            w |> write(i)
            w |> write(", ")
        }
    }
}

// two freeform prose openers - real text with the near-ties counting prompts are chosen to avoid
let PROSE_A = "The old lighthouse keeper watched the storm roll in from the west, counting the seconds between each flash and the thunder that followed. By midnight the waves were breaking over the jetty and"
let PROSE_B = "In the spring of that year the river rose higher than anyone in the valley could remember, and the farmers along the lower fields moved their animals to the ridge while"

// both sessions of a pair prefilled on the GPU twin through the metal prefill override (the
// planar CPU prefill of a 27B costs minutes per session; the metal prefill stashes mtp_h and
// warms the draft slab like the CPU one does)
def private gpu_prefill_pair(trg : Model; var sa, sb : Session; prompt : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
        eval_(trg, sa, prompt)
        eval_(trg, sb, prompt)
        select_prefill_override("")
        set_metal_prefill_min_npos(64l)
    }
}

struct FfStats {
    steps : int64
    drafted : int64       // rounds where the spec rail actually drafted + verified
    cold : int64          // rounds the rail declined (plain step instead)
    served : int64        // metal decode steps served across the pair (0 = the GPU declined)
    flips : int64
    maxd : float
    flip_margin_max : float
}

// The forced-feed pair: session A steps plainly on the GPU, session B runs the speculative round
// with every draft forced to reject (one committed token per round, row 0 promoted to s.logits),
// both fed A's greedy token. `verify = false` is the control (B steps plainly too).
def private ff_pair(tr, trg : Model; prompt : array<int64>; nsteps : int64; verify : bool; depth : int64 = 1l) : FfStats {
    var st = FfStats()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(trg, sa, sb, prompt)   // metal prefill: stashes mtp_h + warms the draft slab, seconds at 27B
            select_decode_override("metal")
            let before = metal_decode_stats().decodes
            var tok = greedy_top(sa.logits, vocab)
            var one <- [tok]
            eval_(trg, sa, one)   // the mirror-building step, both sessions (a spec round needs a live mirror)
            eval_(trg, sb, one)
            tok = greedy_top(sa.logits, vocab)
            if (verify) {
                set_metal_mtp_debug("reject")
                set_mtp_depth(depth)
            }
            for (_i in range64(nsteps)) {
                var mx = 0.0
                for (a, b in sa.logits, sb.logits) {
                    mx = max(mx, abs(a - b))
                }
                st.maxd = max(st.maxd, mx)
                if (greedy_top(sb.logits, vocab) != tok) {
                    st.flips++
                    st.flip_margin_max = max(st.flip_margin_max, top2_margin(sa.logits, vocab))
                }
                one[0] = tok
                eval_(trg, sa, one)
                if (verify) {
                    let nd0 = sb.mtp_drafted
                    mtp_spec_round(trg, sb, tok)   // forced reject: one committed token per round, row 0 promoted to s.logits
                    if (sb.mtp_drafted == nd0) {
                        st.cold++
                    } else {
                        st.drafted++
                    }
                } else {
                    eval_(trg, sb, one)
                }
                st.steps++
                tok = greedy_top(sa.logits, vocab)
            }
            set_metal_mtp_debug("")
            set_mtp_depth(1l)
            st.served = metal_decode_stats().decodes - before
            select_decode_override("")
            delete one
            delete sa
            delete sb
        }
    }
    return st
}

// Free-running pair on the GPU: plain greedy vs the speculative rail's committed stream. Only
// valid as a token-exact gate on a counting prompt (near-tie-free by construction).
def private free_run_pair(tr, trg : Model; prompt : array<int64>; n : int64; var plain, spec : array<int64>; depth : int64;
                          var drafted : int64&) : int64 {
    var served = 0l
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(trg, sa, sb, prompt)
            select_decode_override("metal")
            let before = metal_decode_stats().decodes
            let t0 = greedy_top(sa.logits, vocab)
            var one <- [t0]
            eval_(trg, sa, one)
            eval_(trg, sb, one)
            // plain
            var tok = greedy_top(sa.logits, vocab)
            for (_i in range64(n)) {
                plain |> push(tok)
                one[0] = tok
                eval_(trg, sa, one)
                tok = greedy_top(sa.logits, vocab)
            }
            // speculative: commit tok, plus every accepted draft of the round
            set_mtp_depth(depth)
            tok = greedy_top(sb.logits, vocab)
            while (long_length(spec) < n) {
                spec |> push(tok)
                let a = mtp_spec_round(trg, sb, tok)
                for (i in range64(a)) {
                    spec |> push(sb.mtp_vbatch[i + 1l])
                }
                tok = greedy_top(sb.logits, vocab)
            }
            set_mtp_depth(1l)
            spec |> resize(n)
            drafted = sb.mtp_drafted
            served = metal_decode_stats().decodes - before
            select_decode_override("")
            delete one
            delete sa
            delete sb
        }
    }
    return served
}

// The GPU twin of a planar fixture: the SERVED flavor - the metal-blob image the load walk mints
// (`load_model_cached` under metal mode), not a direct transform of the owned load. The direct
// transform declines on the 27B-MTP (an owned-load weight offset misses the GPU bind alignment
// the image builder pads), and the image is what serves anyway. CPU stages run the planar
// model, GPU stages the twin; sessions are geometry-bound so one session spans both.
def private served_twin(path : string; seq_cap : int64) : Model {
    let prior = get_metal_mode()
    set_metal_mode(MetalMode.required)
    var tg <- load_model_cached(path, QuantMode.q8)
    set_metal_mode(prior)
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}

// GPU-vs-GPU forced-feed envelope: two metal rails on the same feeds differ by kernel spelling
// only (the standing kernel-swap gates sit near 0.01); 0.5 is the one-step decode gate's bar.
let FF_TOL = 0.5
let FF_STEPS = 48l
let COUNT_STEPS = 40l

// one forced-feed cell: engaged-or-skip, then the tolerance gate with its flip telemetry
def private ff_cell(t : T?; tr, trg : Model; tag, pn : string; prompt : array<int64>; depth : int64 = 1l) {
    let st = ff_pair(tr, trg, prompt, FF_STEPS, true, depth)
    if (st.served == 0l) {
        t |> skip("metal decode declined (no Metal device?)")
    } elif (st.drafted == 0l) {
        t |> skip("{tag} {pn}: the spec rail never engaged ({st.cold} cold rounds) - nothing verified")
    } else {
        t |> success(st.cold == 0l, "{tag} {pn}: every round drafted+verified ({st.drafted} drafted, {st.cold} cold)")
        t |> success(st.maxd <= FF_TOL, "{tag} {pn}: verify row0 vs plain step within {FF_TOL} over {st.steps} forced-feed steps (maxd {st.maxd}, argmax flips {st.flips}, widest flipped margin {st.flip_margin_max})")
        to_log(LOG_INFO, "metal mtp {tag} {pn} forced-feed: maxd={st.maxd} flips={st.flips}/{st.steps} widest_margin={st.flip_margin_max} drafted={st.drafted} cold={st.cold}\n")
    }
}

// the process-wide knobs every exit path of run_fixture puts back
def private count_cell(t : T?; tr, trg : Model; tag : string; count : array<int64>; depth : int64) {
    var plain : array<int64>
    var spec : array<int64>
    var drafted = 0l
    let served = free_run_pair(tr, trg, count, COUNT_STEPS, plain, spec, depth, drafted)
    if (served == 0l) {
        t |> skip("metal decode declined (no Metal device?)")
    } else {
        log_gen_texts(tr, "{tag} counting k{depth}", plain, spec)
        t |> success(drafted > 0l, "{tag} counting k{depth}: the spec rail engaged ({drafted} drafts) - a plain fallback would pass vacuously")
        t |> success(same(plain, spec), "{tag} counting k{depth}: speculative free-run == plain free-run ({COUNT_STEPS} tokens)")
    }
    delete plain
    delete spec
}

// per-row logits comparison stats (the harness probe's shape): flips against the plain top-2 margin
struct RowStats {
    rows : int64
    flips : int64
    max_abs : float
    flip_margin_max : float
}

def private argmax_at(v : array<float>; off, n : int64) : int64 {
    var best = 0l
    var bv = v[off]
    for (i in range64(1l, n)) {
        if (v[off + i] > bv) {
            bv = v[off + i]
            best = i
        }
    }
    return best
}

def private compare_row(plain, store : array<float>; soff, n : int64; var st : RowStats; tag : string; pos : int64; verbose : bool) {
    var mx = 0.0
    for (i in range64(n)) {
        mx = max(mx, abs(plain[i] - store[soff + i]))
    }
    st.rows++
    st.max_abs = max(st.max_abs, mx)
    let ap = argmax_at(plain, 0l, n)
    let av = argmax_at(store, soff, n)
    if (ap != av) {
        st.flips++
        let mg = top2_margin(plain, n)
        st.flip_margin_max = max(st.flip_margin_max, mg)
        if (verbose) {
            feint("FLIP {tag} pos={pos}: plain {ap} vs verify {av} margin {mg} maxd {mx}\n")
        }
    }
}

// The batch-rail verify (eval_verify_batch_: ONE session at nr consecutive positions through the
// batch driver's same-slab mode) against the plain GPU step, forced-feed: A plain-steps the
// reference tokens (recording every step's logits); B verifies the same nr tokens per round and
// each row's logits are compared to A's step at that position. Any model, head or not - this is
// the verify the gemma drafter rides.
def private vff_pair(tr, trg : Model; prompt : array<int64>; nrounds, nr : int64; var st : RowStats; var served : int64&; verbose : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(trg, sa, sb, prompt)
            select_decode_override("metal")
            let before = metal_batch_decode_stats().steps
            var ws <- create_batch_workspace_(trg)
            var tok = greedy_top(sa.logits, vocab)
            var one <- [tok]
            eval_(trg, sa, one)   // mirror-building step on both
            eval_(trg, sb, one)
            finish_pending_step()   // the single-stream decode is one step deep - land before reading
            tok = greedy_top(sa.logits, vocab)
            var pos = sb.n_past
            var toks : array<int64>
            var store : array<float>
            var rowmax : array<float>   // per verify row: which rows carry the deviation
            toks |> resize(nr)
            store |> resize(nr * vocab)
            rowmax |> resize(nr)
            for (_r in range64(nrounds)) {
                if (pos + nr + 1l >= c.seq_len) {
                    break
                }
                // A: nr plain steps, recording the logits each produces
                for (i in range64(nr)) {
                    toks[i] = tok
                    one[0] = tok
                    eval_(trg, sa, one)
                    finish_pending_step()
                    for (v in range64(vocab)) {
                        store[i * vocab + v] = sa.logits[v]
                    }
                    tok = greedy_top(sa.logits, vocab)
                }
                // B: the same nr tokens as one same-slab verify; row i is the step on toks[i]
                if (!eval_verify_batch_(trg, ws, sb, toks, pos, nr)) {
                    break
                }
                sb.n_past = pos + nr
                for (i in range64(nr)) {
                    var plain : array<float>
                    plain |> resize(vocab)
                    for (v in range64(vocab)) {
                        plain[v] = store[i * vocab + v]
                    }
                    compare_row(plain, sb.mtp_logits_b, i * vocab, vocab, st, "vrow{i}", pos + i, verbose)
                    var rmx = 0.0
                    for (v in range64(vocab)) {
                        rmx = max(rmx, abs(plain[v] - sb.mtp_logits_b[i * vocab + v]))
                    }
                    rowmax[i] = max(rowmax[i], rmx)
                    if (verbose) {
                        to_log(LOG_INFO, "vff round pos={pos + i} row={i} maxd={rmx} plain_top={argmax_at(plain, 0l, vocab)} verify_top={argmax_at(sb.mtp_logits_b, i * vocab, vocab)} margin={top2_margin(plain, vocab)}\n")
                    }
                    delete plain
                }
                pos += nr
            }
            served = metal_batch_decode_stats().steps - before
            to_log(LOG_INFO, "metal vff per-row maxd: {rowmax}\n")
            select_decode_override("")
            delete ws
            delete one
            delete toks
            delete store
            delete rowmax
            delete sa
            delete sb
        }
    }
}

// the same twin, DISTINCT sessions: GPU single step vs GPU batch step on identical tokens - the
// batch rail's own parity on this model, so a same-slab failure can be told from a rail failure
def private dff_pair(tr, trg : Model; prompts : array<array<int64>>; nsteps : int64; var st : RowStats; var served : int64&; verbose : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = trg.config
        let vocab = c.vocab_size
        let nb = long_length(prompts)
        with_job_que() {
            setup_dasllama_jobque()
            var sa : array<Session>
            var sb : array<Session>
            sa |> reserve(nb)
            sb |> reserve(nb)
            for (i in range64(nb)) {
                sa |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                sb |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                gpu_prefill_pair(trg, sa[i], sb[i], prompts[i])
            }
            select_decode_override("metal")
            let before = metal_batch_decode_stats().steps
            var ws <- create_batch_workspace_(trg)
            var sessb : array<Session?>
            var cur : array<int64>
            sessb |> reserve(nb)
            cur |> reserve(nb)
            for (i in range64(nb)) {
                unsafe {
                    sessb |> push(addr(sb[i]))
                }
                cur |> push(greedy_top(sa[i].logits, vocab))
            }
            var one <- [0l]
            // CPU reference for row 0's first step: which GPU side the truth sits with
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sc, prompts[0])
            one[0] = cur[0]
            eval_(tr, sc, one)
            for (step in range64(nsteps)) {
                for (i in range64(nb)) {
                    one[0] = cur[i]
                    eval_(trg, sa[i], one)
                    finish_pending_step()   // one-deep pipelined single decode - land before the read
                }
                eval_batch_(trg, ws, sessb, cur)
                if (step == 0l) {
                    var mxs = 0.0
                    var mxb = 0.0
                    for (v in range64(vocab)) {
                        mxs = max(mxs, abs(sc.logits[v] - sa[0].logits[v]))
                        mxb = max(mxb, abs(sc.logits[v] - sb[0].logits[v]))
                    }
                    to_log(LOG_INFO, "metal dff cpu-ref B={nb}: cpu-vs-single maxd={mxs} cpu-vs-batch maxd={mxb} cpu top={greedy_top(sc.logits, vocab)} single top={greedy_top(sa[0].logits, vocab)} batch top={greedy_top(sb[0].logits, vocab)}\n")
                }
                for (i in range64(nb)) {
                    compare_row(sa[i].logits, sb[i].logits, 0l, vocab, st, "drow{i}", sb[i].n_past - 1l, verbose)
                    cur[i] = greedy_top(sa[i].logits, vocab)
                }
            }
            delete sc
            served = metal_batch_decode_stats().steps - before
            select_decode_override("")
            sessb |> clear()
            unsafe {
                delete sessb
            }
            delete ws
            delete one
            delete cur
            delete sa
            delete sb
        }
    }
}

//! `must_serve`: a carrier the batch rail is contracted to serve fails on a decline instead of skipping -
//! a skip hid the partial-rope gate mismatch (rows built at rope-dim width, the gate asking head width)
def private dff_cell(t : T?; tr, trg : Model; tag : string; prose_a, prose_b : array<int64>; tol : float; wide : bool = false; must_serve : bool = false) {
    for (nb in (wide ? [5l, 8l] : [2l, 4l])) {
        var sub : array<array<int64>>
        sub |> reserve(nb)
        for (i in range64(nb)) {
            sub |> emplace <| clone_to_move((i & 1l) == 0l ? prose_a : prose_b)
        }
        var st = RowStats()
        var served = 0l
        dff_pair(tr, trg, sub, 24l, st, served, false)
        if (served == 0l) {
            if (must_serve) {
                t |> failure("{tag} dff B={nb}: the metal batch rail declined distinct-session steps on a carrier it must serve")
            } else {
                t |> skip("{tag} dff B={nb}: the metal batch rail declined distinct-session steps")
            }
        } else {
            t |> success(st.max_abs <= tol, "{tag} dff B={nb}: GPU batch step vs GPU single step within {tol} over {st.rows} rows (maxd {st.max_abs}, argmax flips {st.flips}, widest flipped margin {st.flip_margin_max})")
            to_log(LOG_INFO, "metal dff {tag} B={nb}: maxd={st.max_abs} flips={st.flips}/{st.rows} widest_margin={st.flip_margin_max} steps={served}\n")
        }
        delete sub
    }
}

//! the same-slab verify lands GPU logits only on head-less models (the batch driver's gpu_logits gate
//! excludes NextN carriers - their round has its own verify), so a NextN fixture's decline is a skip;
//! `must_serve` marks a head-less carrier the rail is contracted to serve - a decline there is a failure
def private vff_cell(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; tol : float; nr : int64 = 4l; must_serve : bool = false) {
    var st = RowStats()
    var served = 0l
    vff_pair(tr, trg, prompt, 12l, nr, st, served, false)
    if (served == 0l) {
        if (must_serve) {
            t |> failure("{tag}: the metal batch rail declined the same-slab verify on a carrier it must serve")
        } else {
            t |> skip("{tag}: the metal batch rail declined the same-slab verify")
        }
    } else {
        t |> success(st.rows > 0l, "{tag} vff: rows compared ({st.rows})")
        t |> success(st.max_abs <= tol, "{tag} vff: batch same-slab verify rows vs plain steps within {tol} over {st.rows} rows (maxd {st.max_abs}, argmax flips {st.flips}, widest flipped margin {st.flip_margin_max})")
        to_log(LOG_INFO, "metal vff {tag}: maxd={st.max_abs} flips={st.flips}/{st.rows} widest_margin={st.flip_margin_max} steps={served}\n")
    }
}

def private restore_knobs(prior_backend : string; prior_wscale : bool) {
    clear_kernel_backend_pin()
    select_kernel_backend(prior_backend)
    set_wscale_f16(prior_wscale)
}

def private run_fixture(t : T?; file, tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let path = path_join(models_dir(), file)
        if (!model_available(t, path)) {
            return
        }
        let prior_wscale = get_wscale_f16()             // process-wide knobs — restored below
        let prior_backend = active_kernel_backend()     // (every exit path runs the restore)
        set_wscale_f16(false)       // the resident path reads the f32 scale plane (declines on s16)
        pin_kernel_backend("portable")  // ...and ROW-MAJOR weights (the blob transform declines repacked planes)
        set_mtp_spec(true)
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        if (tr.config.n_layer_nextn <= 0l) {
            t |> failure("{file}: no NextN draft head - wrong fixture")
            delete tr
            set_mtp_spec(false)
            restore_knobs(prior_backend, prior_wscale)
            return
        }
        var trg <- served_twin(path, 512l)
        if (!trg.metal_blob) {
            t |> skip("{file}: the served image is not the metal-blob flavor on this box - GPU arms have nothing to run")
            set_mtp_spec(false)
            restore_knobs(prior_backend, prior_wscale)
            delete tr
            delete trg
            return
        }
        var prose_a <- encode(tr, PROSE_A, true, false)
        var prose_b <- encode(tr, PROSE_B, true, false)
        var count <- encode(tr, counting_prompt(1, 40), true, false)
        run_arms(t, tr, trg, tag, prose_a, prose_b, count)
        metal_decode_shutdown()
        metal_prefill_shutdown()   // the pairs prefill on the GPU - its panels release here too
        set_mtp_spec(false)
        restore_knobs(prior_backend, prior_wscale)
        delete prose_a
        delete prose_b
        delete count
        delete tr
        delete trg
        t |> equal(metal_live_object_count(), 0l, "{tag}: no leaked Metal objects after shutdown")
    } else {
        t |> skip("no das_metal on this build")
    }
}

// the fixture's arms, one gate each
def private run_arms(t : T?; tr, trg : Model; tag : string; prose_a, prose_b, count : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        // control: plain vs plain on the GPU, the SAME feeds - the harness must read exactly zero
        if (arm_on(t, "mtp-ctrl-{tag}")) {
            let st = ff_pair(tr, trg, prose_a, 16l, false)
            if (st.served == 0l) {
                t |> skip("metal decode declined (no Metal device?)")
            } else {
                t |> success(st.maxd == 0.0 && st.flips == 0l, "{tag} control: plain-vs-plain forced feed is bit-identical (maxd {st.maxd}, flips {st.flips})")
            }
        }

        // forced-feed: the verify's row 0 vs the plain step, identical tokens every round
        if (arm_on(t, "mtp-ff-{tag}")) {
            ff_cell(t, tr, trg, tag, "prose-a", prose_a)
            ff_cell(t, tr, trg, tag, "prose-b", prose_b)
        }

        // the batch-rail same-slab verify (the gemma drafter's verify) vs plain steps, GPU-vs-GPU
        if (arm_on(t, "mtp-vff-{tag}")) {
            vff_cell(t, tr, trg, tag, prose_a, FF_TOL)
        }

        // depth N under forced reject: every round verifies k+1 rows and replays row 0 from the
        // pre-verify recurrent state - the rollback path exercised on every single round
        if (arm_on(t, "mtp-ffk-{tag}")) {
            ff_cell(t, tr, trg, "{tag} k2", "prose-a", prose_a, 2l)
            ff_cell(t, tr, trg, "{tag} k4", "prose-b", prose_b, 4l)
        }

        // counting: free-running speculation == free-running plain, token for token, at depth 1
        // and at depth 4 (full accepts, partial accepts and rejects all land on the counting stream)
        if (arm_on(t, "mtp-count-{tag}")) {
            count_cell(t, tr, trg, tag, count, 1l)
            count_cell(t, tr, trg, tag, count, 2l)
            count_cell(t, tr, trg, tag, count, 4l)
        }
    }
}

[test]
def test_metal_mtp_parity_0_8b(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.5-0.8B-MTP-Q8_0") <| @(t : T?) {
        run_fixture(t, "Qwen3.5-0.8B-MTP-Q8_0.gguf", "0.8b")
    }
}

[test]
def test_metal_mtp_parity_27b(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.6-27B-MTP-Q4_K_M (large tier)") <| @(t : T?) {
        run_fixture(t, "Qwen3.6-27B-MTP-Q4_K_M.gguf", "27b")
    }
}

[test]
def test_metal_mtp_parity_35b(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.6-35B-A3B-MTP-UD-Q4_K_M (large tier)") <| @(t : T?) {
        run_fixture(t, "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", "35b")
    }
}

// The split-head carrier: ggml-org ships Qwen3.8-27B's NextN head as its own file (the trunk is
// converted --no-mtp), and the served pair here is the Q4_K_M trunk with the Q8_0 head - a
// mixed-quant pair the mtp-<trunk basename> adjacency cannot name, so the fixture sets the
// override knob (restored on exit) and the head rides the load as the last shard.
[test]
def test_metal_mtp_parity_qwen38_split_head(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.8-27B-Q4_K_M + split head mtp-Qwen3.8-27B-Q8_0 (large tier)") <| @(t : T?) {
        let head = path_join(models_dir(), "mtp-Qwen3.8-27B-Q8_0.gguf")
        if (!stat(head).is_valid) {
            t |> skip("mtp-Qwen3.8-27B-Q8_0.gguf not present")
            return
        }
        set_mtp_head_path(head)
        run_fixture(t, "Qwen3.8-27B-Q4_K_M.gguf", "3.8-27b")
        clear_mtp_head_path()
    }
}

// A head-less fixture: only the batch-rail verify arm runs (the model has no draft head yet - the
// gemma assistant drafter lands in S3). gemma4's router amplifies any float-class difference to
// ~2.5 logits/step (the support-matrix row's finding), so its bar is the row's, not the 0.5 one.
def private run_verify_fixture(t : T?; file, tag : string; tol : float; must_serve : bool = false) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let path = path_join(models_dir(), file)
        if (!model_available(t, path)) {
            return
        }
        let prior_wscale = get_wscale_f16()
        let prior_backend = active_kernel_backend()
        set_wscale_f16(false)
        pin_kernel_backend("portable")
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        var trg <- served_twin(path, 512l)
        if (!trg.metal_blob) {
            t |> skip("{file}: the served image is not the metal-blob flavor on this box")
            restore_knobs(prior_backend, prior_wscale)
            delete tr
            delete trg
            return
        }
        var prose_a <- encode(tr, PROSE_A, true, false)
        var prose_b <- encode(tr, PROSE_B, true, false)
        if (arm_on(t, "mtp-dff-{tag}")) {
            dff_cell(t, tr, trg, tag, prose_a, prose_b, tol)
        }
        if (arm_on(t, "mtp-dff8-{tag}")) {   // B=5 and B=8: the GEMM / mv8 rails of the batch driver
            dff_cell(t, tr, trg, tag, prose_a, prose_b, tol, true)
        }
        if (arm_on(t, "mtp-vff-{tag}")) {
            vff_cell(t, tr, trg, tag, prose_a, tol, 4l, must_serve)
        }
        if (arm_on(t, "mtp-vff1-{tag}")) {   // one row through the batch driver: the encoder alone, no row mixing
            vff_cell(t, tr, trg, tag, prose_a, tol, 1l, must_serve)
        }
        if (arm_on(t, "mtp-vff5-{tag}")) {   // five rows: the batch driver's GEMM rail (mp 32) under the same-slab verify
            vff_cell(t, tr, trg, tag, prose_a, tol, 5l, must_serve)
        }
        // the assistant drafter's round (gemma-4 targets with an mtp-* sidecar beside them): counting
        // free-run, speculative == plain at depth 1, 2 and 4
        if (arm_on(t, "mtp-count-{tag}")) {
            let side = gemma_drafter_sidecar(path)
            if (empty(side)) {
                t |> skip("{tag}: no assistant drafter sidecar beside {file}")
            } elif (!attach_gemma_drafter(trg, side)) {
                t |> skip("{tag}: the drafter {side} did not attach")
            } else {
                var count <- encode(tr, counting_prompt(1, 40), true, false)
                set_gemma_trace(arm_on(t, "mtp-count-trace"))
                set_gemma_draft_on_cpu(arm_on(t, "mtp-count-cpu"))   // bring-up bisect: the CPU oracle drafts
                set_gemma_carry_pre_norm(arm_on(t, "mtp-count-pre"))   // the memo's open question: pre-norm residual as the drafter's h
                for (depth in [1l, 2l, 4l]) {
                    count_cell(t, tr, trg, tag, count, depth)
                    let st = gemma_drafter_stats()
                    to_log(LOG_INFO, "gemma drafter {tag} k{depth}: rounds={st.rounds} drafted={st.drafted} accepted={st.accepted}\n")
                }
                set_gemma_trace(false)
                set_gemma_draft_on_cpu(false)
                set_gemma_carry_pre_norm(false)
                detach_gemma_drafter()
                delete count
            }
        }
        metal_decode_shutdown()
        metal_prefill_shutdown()
        restore_knobs(prior_backend, prior_wscale)
        delete prose_a
        delete prose_b
        delete tr
        delete trg
        t |> equal(metal_live_object_count(), 0l, "{tag}: no leaked Metal objects after shutdown")
    } else {
        t |> skip("no das_metal on this build")
    }
}

[test]
def test_metal_verify_gemma26(t : T?) {
    t |> run("batch-rail same-slab verify vs plain GPU decode - gemma-4-26B-A4B-it-Q4_K_M (large tier)") <| @(t : T?) {
        // the support matrix's gemma4moe Q4_K_M forced-step bar: the routed experts amplify any
        // float-class difference at a near-tie (measured: floor 0.003-0.01, isolated spikes to 11,
        // argmax never flips); token parity is not the instrument for this model
        run_verify_fixture(t, "gemma-4-26B-A4B-it-Q4_K_M.gguf", "g26", 24.0)
    }
}

// the control for the batch-rail same-slab verify: the batch parity fixture itself (attention-only,
// batch B=2/3/6 token-exact vs the CPU control) - a failure here is the same-slab plumbing, not gemma
[test]
def test_metal_verify_llama1b(t : T?) {
    t |> run("batch-rail same-slab verify vs plain GPU decode - Llama-3.2-1B-Instruct-Q8_0 (control)") <| @(t : T?) {
        run_verify_fixture(t, "Llama-3.2-1B-Instruct-Q8_0.gguf", "l1b", 0.5)
    }
}

[test]
def test_metal_verify_gemma12(t : T?) {
    t |> run("batch-rail same-slab verify vs plain GPU decode - gemma-4-12B-it-Q4_K_M (large tier)") <| @(t : T?) {
        run_verify_fixture(t, "gemma-4-12B-it-Q4_K_M.gguf", "g12", 0.5)
    }
}

// the partial-rope carrier (qwen35: head 256, rope_dim 64), HEAD-LESS so the batch rail's GPU logits
// gate admits it: the rail's rope rows are built at rope-dim width and its gate must ask for that
// width, not the head's - the decline read rope_rows and had hidden behind a skip
[test]
def test_metal_verify_qwen38_27b(t : T?) {
    t |> run("batch-rail same-slab verify vs plain GPU decode - Qwen3.8-27B-Q4_K_M, no head (partial rope, large tier)") <| @(t : T?) {
        run_verify_fixture(t, "Qwen3.8-27B-Q4_K_M.gguf", "q38", 2.0, true)
    }
}

// plain-MoE control (qwen3moe: routed experts, no parallel dense branch) for the gemma-MoE cell
[test]
def test_metal_verify_qwen30a3(t : T?) {
    t |> run("batch-rail same-slab verify vs plain GPU decode - Qwen3-30B-A3B-Instruct-2507-Q4_K_M (large tier)") <| @(t : T?) {
        run_verify_fixture(t, "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", "q30", 2.0)
    }
}
