options gen2
options persistent_heap   // + explicit deletes: each fixture frees before the next loads
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 — the public facade: engine + arch registrations + (on Apple) the metal overrides
require dasllama/dasllama_math   // set_wscale_f16 — the resident path reads the f32 scale plane
require dasllama/dasllama_image   // load_model_cached — the served (image) flavor for the GPU twin
require dasllama/dasllama_gguf   // set_mtp_head_path — the split-head fixture names its head explicitly
require dasllama/dasllama_batch   // nolint:STYLE030,LINT019 — verify_batch_step + create_batch_workspace_ in the das_metal halves
require dasllama/dasllama_mtp_gemma   // nolint:STYLE030,LINT019 — gemma_drafter_sidecar in the das_metal halves
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // attach_gemma_drafter — the gemma spec round rides the verify fixtures
require ?das_metal dasllama/dasllama_metal_decode   // stats + the forced-reject lever + shutdown
require ?das_metal dasllama/dasllama_metal_kernels   // attn_d_head_ok - the fused attention's head admission
require ?das_metal dasllama/dasllama_metal_prefill   // the metal prefill override for the pair's prompts
require ?das_metal metal/das_metal_boost   // metal_live_object_count for the leak gate
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 — path_join, used only inside the das_metal static_if halves
require math
require strings
require _model_tier
require ../performance/model_specs.das   // nolint:STYLE030,LINT019 — counting_prompt, read only inside the das_metal static_if halves
require _metal_blob_twin   // metal_kernel_count - the census reader

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

// two freeform prose openers - real text with the near-ties counting prompts are chosen to avoid
let PROSE_A = "The old lighthouse keeper watched the storm roll in from the west, counting the seconds between each flash and the thunder that followed. By midnight the waves were breaking over the jetty and"
let PROSE_B = "In the spring of that year the river rose higher than anyone in the valley could remember, and the farmers along the lower fields moved their animals to the ridge while"

// both sessions of a pair prefilled on the GPU twin through the metal prefill override (the
// planar CPU prefill of a 27B costs minutes per session; the metal prefill stashes mtp_h and
// warms the draft slab like the CPU one does)
//! the Metal prefill forced under the short prompts' length - the only prefill a blob-only model has
def private gpu_prefill(trg : Model; var s : Session; prompt : array<int64>) {   // nolint:LINT012,LINT019 — the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let floor_before = get_metal_prefill_min_npos()
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
        eval_(trg, s, prompt)
        select_prefill_override("")
        set_metal_prefill_min_npos(floor_before)
    }
}

def private gpu_prefill_pair(trg : Model; var sa, sb : Session; prompt : array<int64>) {
    gpu_prefill(trg, sa, prompt)
    gpu_prefill(trg, sb, prompt)
}

struct FfStats {
    steps : int64
    drafted : int64       // rounds where the spec rail actually drafted + verified
    cold : int64          // rounds the rail declined (plain step instead)
    served : int64        // metal decode steps served across the pair (0 = the GPU declined)
    flips : int64
    maxd : float
    flip_margin_max : float
    off_min : float = 1e30   //!< the one-step-off control: the smallest distance from B's row to A's PREVIOUS step
}

// The forced-feed pair: session A steps plainly on the GPU, session B runs the speculative round
// with every draft forced to reject (one committed token per round, row 0 promoted to s.logits),
// both fed A's greedy token. `verify = false` is the control (B steps plainly too).
def private ff_pair(tr, trg : Model; prompt : array<int64>; nsteps : int64; verify : bool; depth : int64 = 1l) : FfStats {   // nolint:LINT012,LINT019 — the body is the das_metal half
    var st = FfStats()   // nolint:LINT003,LINT019 — filled in the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_dasllama_jobque() {
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(trg, sa, sb, prompt)   // metal prefill: stashes mtp_h + warms the draft slab, seconds at 27B
            select_decode_override("metal")
            let before = metal_decode_stats().decodes
            var tok = argmax_of(sa.logits, vocab)
            var one <- [tok]
            eval_(trg, sa, one)   // the mirror-building step, both sessions (a spec round needs a live mirror)
            eval_(trg, sb, one)
            tok = argmax_of(sa.logits, vocab)
            if (verify) {
                set_metal_mtp_debug("reject")
                set_mtp_depth(depth)
            }
            var prev : array<float>
            for (_i in range64(nsteps)) {
                var mx = 0.0
                for (a, b in sa.logits, sb.logits) {
                    mx = max(mx, abs(a - b))
                }
                st.maxd = max(st.maxd, mx)
                if (!empty(prev)) {
                    var off = 0.0
                    for (p, b in prev, sb.logits) {
                        off = max(off, abs(p - b))
                    }
                    st.off_min = min(st.off_min, off)
                }
                prev := sa.logits
                if (argmax_of(sb.logits, vocab) != tok) {
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
                tok = argmax_of(sa.logits, vocab)
            }
            set_metal_mtp_debug("")
            set_mtp_depth(1l)
            st.served = metal_decode_stats().decodes - before
            select_decode_override("")
            delete prev
            delete one
            delete sa
            delete sb
        }
    }
    return st
}

//! top-k 1 makes every draw the penalized argmax, so a seeded sampled stream is deterministic and the token-exact gate holds
let SAMPLED_PARAMS = SamplingParams(temp = 0.7, top_k = 1l, penalty = 1.1)

//! Free-running pair on the GPU, greedy or through the session sampler: only valid as a token-exact gate on a counting prompt (near-tie-free by construction).
def private free_run_pair(tr, trg : Model; prompt : array<int64>; n : int64; var plain, spec : array<int64>; depth : int64; var drafted, parked : int64&; sampled : bool = false) : int64 {   // nolint:LINT012,LINT019 — the body is the das_metal half
    var served = 0l   // nolint:LINT003,LINT019 — counted in the das_metal half
    parked = 0l
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var sp = SAMPLED_PARAMS
        with_dasllama_jobque() {
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(trg, sa, sb, prompt)
            select_decode_override("metal")
            let before = metal_decode_stats().decodes
            let t0 = argmax_of(sa.logits, vocab)
            var one <- [t0]
            eval_(trg, sa, one)
            eval_(trg, sb, one)
            if (sampled) {
                set_seed_(sa, 4242)
                set_seed_(sb, 4242)
                sb.spec_params = unsafe(addr(sp))
            }
            var tok = sampled ? sample_(sa, sp) : argmax_of(sa.logits, vocab)
            for (_i in range64(n)) {
                plain |> push(tok)
                one[0] = tok
                eval_(trg, sa, one)
                tok = sampled ? sample_(sa, sp) : argmax_of(sa.logits, vocab)
            }
            // speculative: commit tok, plus every accepted draft of the round
            set_mtp_depth(depth)
            tok = sampled ? sample_(sb, sp) : argmax_of(sb.logits, vocab)
            while (long_length(spec) < n) {
                spec |> push(tok)
                let a = mtp_spec_round(trg, sb, tok)
                for (i in range64(a)) {
                    spec |> push(sb.mtp_vbatch[i + 1l])
                }
                if (sb.mtp_pre_drawn) {
                    parked++
                }
                tok = sampled ? sample_(sb, sp) : argmax_of(sb.logits, vocab)
            }
            sb.spec_params = null
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
//! the plain-MoE bar of the batch-rail fixtures (q30's): the rows and single GEMV forms round the router's
//! logits apart, and a near-tie flips an expert - Flash-Next prose-a reads 0.89 with zero argmax flips
let MOE_FF_TOL = 2.0
//! the nine-row kq mul_mm stages x to half - a wider envelope than the f32-x GEMV forms', flips still zero
let MM_TOL = 1.0
let FF_STEPS = 48l
let COUNT_STEPS = 40l

// one forced-feed cell: engaged-or-skip, then the tolerance gate with its flip telemetry
def private ff_cell(t : T?; tr, trg : Model; tag, pn : string; prompt : array<int64>; depth : int64 = 1l; tol : float = FF_TOL) {
    let st = ff_pair(tr, trg, prompt, FF_STEPS, true, depth)
    if (st.served == 0l) {
        t |> skip("metal decode declined (no Metal device?)")
    } elif (st.drafted == 0l) {
        t |> skip("{tag} {pn}: the spec rail never engaged ({st.cold} cold rounds) - nothing verified")
    } else {
        t |> success(st.cold == 0l, "{tag} {pn}: every round drafted+verified ({st.drafted} drafted, {st.cold} cold)")
        t |> success(st.maxd <= tol, "{tag} {pn}: verify row0 vs plain step within {tol} over {st.steps} forced-feed steps (maxd {st.maxd}, argmax flips {st.flips}, widest flipped margin {st.flip_margin_max})")
        t |> success(st.off_min > tol, "{tag} {pn} control: verify row0 against the PREVIOUS plain step lands outside {tol} at every step (smallest {st.off_min})")
        to_log(LOG_INFO, "metal mtp {tag} {pn} forced-feed: maxd={st.maxd} flips={st.flips}/{st.steps} widest_margin={st.flip_margin_max} drafted={st.drafted} cold={st.cold} one_step_off_min={st.off_min}\n")
    }
}

//! answers the rounds the spec rail drafted - the draft-quality witness a crowned leg holds against the plain run's
def private count_cell(t : T?; tr, trg : Model; tag : string; count : array<int64>; depth : int64; sampled : bool = false) : int64 {
    var plain, spec : array<int64>
    var drafted = 0l
    var parked = 0l
    mtp_pos_reset()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_kernel_coverage_reset()
    }
    var served = 0l
    with_metal_attn_d(true) $ {
        served = free_run_pair(tr, trg, count, COUNT_STEPS, plain, spec, depth, drafted, parked, sampled)
    }
    let kind = sampled ? "sampled" : "counting"
    if (served == 0l) {
        t |> skip("metal decode declined (no Metal device?)")
    } else {
        let c = tr.config
        var fused_shape = false
        static_if (typeinfo builtin_module_exists(das_metal)) {
            fused_shape = attn_d_head_ok(c.head_size) && (c.head_size_swa == 0l || attn_d_head_ok(c.head_size_swa))
        }
        let chunked = metal_kernel_count("MetalSqAttnPartBF16_metal_sq_attn_part_b")
        let fused = metal_kernel_count("MetalSqAttnDF16_metal_sq_attn_d") + metal_kernel_count("MetalSqAttnDWF16_metal_sq_attn_d") + metal_kernel_count("MetalSqAttnDXF16_metal_sq_attn_d")
        to_log(LOG_INFO, "{tag} {kind} k{depth}: prompt {long_length(count)} tokens, head {c.head_size}, fused shape {fused_shape}: {fused} fused dispatches, {chunked} chunked\n")
        if (long_length(count) > 64l && fused_shape) {
            t |> success(fused > 0l && chunked == 0l, "{tag} {kind} k{depth}: the verify's rows took the fused attention under DASLLAMA_METAL_ATTN_D pinned on ({fused} fused dispatches, {chunked} chunked)")
        } elif (long_length(count) > 64l) {
            t |> success(chunked > 0l, "{tag} {kind} k{depth}: a shape the fused kernel does not carry keeps the verify on the chunked pair ({chunked} dispatches)")
        }
        log_gen_texts(tr, "{tag} {kind} k{depth}", plain, spec)
        t |> success(drafted > 0l, "{tag} {kind} k{depth}: the spec rail engaged ({drafted} drafts) - a plain fallback would pass vacuously")
        let positions = mtp_pos_report()
        t |> success(find(positions, "p{depth} ") >= 0, "{tag} {kind} k{depth}: a round drafted position {depth} - the depth the arm names was reached ({positions})")
        t |> success(ids_equal(plain, spec), "{tag} {kind} k{depth}: speculative free-run == plain free-run ({COUNT_STEPS} tokens)")
        t |> success(sampled == (parked > 0l), "{tag} {kind} k{depth}: the sampled walk parks the next draw ({parked} rounds); the argmax walk parks none")
    }
    delete plain
    delete spec
    return drafted
}

//! the half-x leg: under the k4 / k5 / k6 half-x crowns the verify's two-to-eight-row K-quant sites read the f16 twin, the
//! MTP head's own q/k/v among them - its rows the head's norm wrote with no twin, so the flag must be clear there, or the
//! head projects the trunk's last FFN norm and every draft misses silently while the verify still lands the plain tokens;
//! the witness is the drafted-round count, which must hold up to the uncrowned run's within a fifth
def private half_crown_cell(t : T?; tr, trg : Model; tag : string; count : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let plain_drafted = count_cell(t, tr, trg, "{tag} plain-crowns", count, 2l)
        let prev_crowns = get_metal_tensor_crowns()
        let prev_pinned = metal_tensor_crowns_pinned()
        let prev_half = get_metal_kq_half()
        set_metal_kq_half(true)
        pin_metal_tensor_crowns("{kq_half_crown(KqFmt.k4)},{kq_half_crown(KqFmt.k5)},{kq_half_crown(KqFmt.k6)}")
        metal_decode_shutdown()
        let half_drafted = count_cell(t, tr, trg, "{tag} half-x", count, 2l)
        to_log(LOG_INFO, "{tag} half-x leg: drafted {half_drafted} rounds under the half crowns against {plain_drafted} without\n")
        t |> success(half_drafted * 5l >= plain_drafted * 4l, "{tag} half-x: the drafts under the half crowns hold up to the plain run's ({half_drafted} vs {plain_drafted} rounds) - a head fed a stale twin drafts garbage and the count collapses")
        set_metal_kq_half(prev_half)
        if (prev_pinned) {
            pin_metal_tensor_crowns(prev_crowns)
        } else {
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
        }
        metal_decode_shutdown()
    }
}

// per-row logits comparison stats (the harness probe's shape): flips against the plain top-2 margin
struct RowStats {
    rows : int64
    flips : int64
    max_abs : float
    flip_margin_max : float
}

def private compare_row(plain, store : array<float>; soff, n : int64; var st : RowStats; tag : string; pos : int64; verbose : bool) {
    var mx = 0.0
    for (i in range64(n)) {
        mx = max(mx, abs(plain[i] - store[soff + i]))
    }
    st.rows++
    st.max_abs = max(st.max_abs, mx)
    let ap = argmax_of(plain, 0l, n)
    let av = argmax_of(store, soff, n)
    if (ap != av) {
        st.flips++
        let mg = top2_margin(plain, n)
        st.flip_margin_max = max(st.flip_margin_max, mg)
        if (verbose) {
            feint("FLIP {tag} pos={pos}: plain {ap} vs verify {av} margin {mg} maxd {mx}\n")
        }
    }
}

// The batch-rail verify (verify_batch_step: ONE session at nr consecutive positions through the
// batch driver's same-slab mode) against the plain GPU step, forced-feed: A plain-steps the
// reference tokens (recording every step's logits); B verifies the same nr tokens per round and
// each row's logits are compared to A's step at that position. Any model, head or not - this is
// the verify the gemma drafter rides.
def private vff_pair(tr, trg : Model; prompt : array<int64>; nrounds, nr : int64; var st : RowStats; var served : int64&; verbose : bool) {   // nolint:LINT012,LINT019 — the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_dasllama_jobque() {
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(trg, sa, sb, prompt)
            select_decode_override("metal")
            let before = metal_batch_decode_stats().steps
            var ws <- create_batch_workspace_(trg)
            var tok = argmax_of(sa.logits, vocab)
            var one <- [tok]
            eval_(trg, sa, one)   // mirror-building step on both
            eval_(trg, sb, one)
            finish_pending_step()   // the single-stream decode is one step deep - land before reading
            tok = argmax_of(sa.logits, vocab)
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
                    tok = argmax_of(sa.logits, vocab)
                }
                // B: the same nr tokens as one same-slab verify; row i is the step on toks[i]
                if (!verify_batch_step(trg, ws, sb, toks, pos, nr)) {
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
                        to_log(LOG_INFO, "vff round pos={pos + i} row={i} maxd={rmx} plain_top={argmax_of(plain, 0l, vocab)} verify_top={argmax_of(sb.mtp_logits_b, i * vocab, vocab)} margin={top2_margin(plain, vocab)}\n")
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
def private dff_pair(tr, trg : Model; prompts : array<array<int64>>; nsteps : int64; var st : RowStats; var served : int64&; verbose : bool) {   // nolint:LINT012,LINT019 — the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = trg.config
        let vocab = c.vocab_size
        let nb = long_length(prompts)
        with_dasllama_jobque() {
            var sa, sb : array<Session>
            sa |> reserve(nb)
            sb |> reserve(nb)
            dn_mirror_room(2l * nb + 1l)   // a recurrent carrier: every twin and the reference row keep a live mirror, and a prefill mints one - the room comes before them
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
                sessb |> push(unsafe(addr(sb[i])))
                cur |> push(argmax_of(sa[i].logits, vocab))
            }
            var one <- [0l]
            // the reference for row 0's first step: which GPU side the truth sits with - the CPU on a planar
            // twin, the GPU single step on a served-only one (a blob model has no CPU prefill)
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            if (tr.metal_blob) {
                gpu_prefill(tr, sc, prompts[0])
            } else {
                eval_(tr, sc, prompts[0])
            }
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
                    to_log(LOG_INFO, "metal dff cpu-ref B={nb}: cpu-vs-single maxd={mxs} cpu-vs-batch maxd={mxb} cpu top={argmax_of(sc.logits, vocab)} single top={argmax_of(sa[0].logits, vocab)} batch top={argmax_of(sb[0].logits, vocab)}\n")
                }
                for (i in range64(nb)) {
                    compare_row(sa[i].logits, sb[i].logits, 0l, vocab, st, "drow{i}", sb[i].n_past - 1l, verbose)
                    cur[i] = argmax_of(sa[i].logits, vocab)
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
        var inscope sub : array<array<int64>>
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
def private vff_cell(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; tol : float; nr : int64 = 4l; must_serve : bool = false;
                     expect_decline : string = "") {
    var st = RowStats()
    var served = 0l
    var before = 0l   // nolint:LINT003,LINT019 — counted in the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (expect_decline != "") {
            let declines_before <- metal_batch_decode_declines()
            before = declines_before?[expect_decline] ?? 0l
        }
    }
    vff_pair(tr, trg, prompt, 12l, nr, st, served, false)
    if (expect_decline != "") {
        var after = 0l   // nolint:LINT003,LINT019 — counted in the das_metal half
        var by_key = ""   // nolint:LINT003,LINT019 — the whole decline table, so a red names the key that did fire
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let declines_after <- metal_batch_decode_declines()
            after = declines_after?[expect_decline] ?? 0l
            by_key = "{declines_after}"
        }
        t |> success(served == 0l && after > before,
            "{tag}: the metal batch rail declines this graph as {expect_decline} ({before} -> {after}, served {served}; declines {by_key})")
    } elif (served == 0l) {
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

//! the wider nine-row bar's control: the same nine-row verify with the GEMV stage knocked out must
//! land OUTSIDE the bar - a bar nothing exceeds is not known to discriminate
def private vff_control_cell(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; bar : float) {   // nolint:LINT012,LINT019 — the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var st = RowStats()
        var served = 0l
        set_metal_decode_skip("gemv")
        vff_pair(tr, trg, prompt, 4l, 9l, st, served, false)
        set_metal_decode_skip("")
        if (served == 0l) {
            t |> skip("{tag} vff9 control: the metal batch rail declined the same-slab verify")
        } else {
            t |> success(st.max_abs > bar, "{tag} vff9 control: the nine-row verify with its GEMV stage knocked out reds the {bar} bar (maxd {st.max_abs} over {st.rows} rows)")
        }
    }
}

// the nine-row arm: the kq carriers take the wider mul_mm bar, and a bar wider than the file's
// standing one ships its control in the same arm
def private vff9_cell(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; tol : float; must_serve : bool; expect_decline : string) {
    vff_cell(t, tr, trg, tag, prompt, trg.kquant_native ? max(tol, MM_TOL) : tol, 9l, must_serve, expect_decline)
    if (trg.kquant_native && empty(expect_decline)) {
        vff_control_cell(t, tr, trg, tag, prompt, MM_TOL)
    }
}

def private restore_knobs(prior_backend : string; prior_wscale : bool) {
    clear_kernel_backend_pin()
    select_kernel_backend(prior_backend)
    set_wscale_f16(prior_wscale)
}

def private run_fixture(t : T?; file, tag : string) {   // nolint:LINT012,LINT019 — the body is the das_metal half
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
def private run_arms(t : T?; tr, trg : Model; tag : string; prose_a, prose_b, count : array<int64>; tol : float = FF_TOL) {   // nolint:LINT012,LINT019 — the body is the das_metal half
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
            ff_cell(t, tr, trg, tag, "prose-a", prose_a, 1l, tol)
            ff_cell(t, tr, trg, tag, "prose-b", prose_b, 1l, tol)
        }

        // the batch-rail same-slab verify (the gemma drafter's verify) vs plain steps, GPU-vs-GPU
        if (arm_on(t, "mtp-vff-{tag}")) {
            vff_cell(t, tr, trg, tag, prose_a, FF_TOL)
        }

        // depth N under forced reject: every round verifies k+1 rows and replays row 0 from the
        // pre-verify recurrent state - the rollback path exercised on every single round
        if (arm_on(t, "mtp-ffk-{tag}")) {
            ff_cell(t, tr, trg, "{tag} k2", "prose-a", prose_a, 2l, tol)
            ff_cell(t, tr, trg, "{tag} k4", "prose-b", prose_b, 4l, tol)
        }

        // counting: free-running speculation == free-running plain, token for token, at depth 1
        // and at depth 4 (full accepts, partial accepts and rejects all land on the counting stream)
        if (arm_on(t, "mtp-count-{tag}")) {
            count_cell(t, tr, trg, tag, count, 1l)
            count_cell(t, tr, trg, tag, count, 2l)
            count_cell(t, tr, trg, tag, count, 4l)
        }

        // the deepest round: depth 8 verifies MTP_MAX_ROWS rows, one past the eight-row GEMV rail
        if (arm_on(t, "mtp-count8-{tag}")) {
            count_cell(t, tr, trg, tag, count, 8l)
        }

        // the half-x crowns over the verify's K-quant sites, the MTP head's among them
        if (arm_on(t, "mtp-ffh-{tag}")) {
            half_crown_cell(t, tr, trg, tag, count)
        }

        // the sampled walk: every verify row drawn through the session's sampler, seeded - free-run == plain sampled free-run
        if (arm_on(t, "mtp-sampled-{tag}")) {
            count_cell(t, tr, trg, tag, count, 1l, true)
            count_cell(t, tr, trg, tag, count, 2l, true)
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
def test_metal_mtp_parity_4b(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.5-4B-MTP-Q8_0") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        run_fixture(t, "Qwen3.5-4B-MTP-Q8_0.gguf", "4b")
    }
}

[test]
def test_metal_mtp_parity_9b(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.5-9B-MTP-UD-Q5_K_XL (large tier)") <| @(t : T?) {
        if (!family_on(t, "qwen35")) return
        run_fixture(t, "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf", "9b")
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

// The split-head carrier: Qwen3.8-27B's NextN head ships as its own file (the trunk is converted
// without it), and the served pair here is the Q4_K_M trunk with the Q8_0 head - a
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

// A served-only fixture: the arms run GPU against GPU on the served twin alone (the form every arm
// here takes - the planar model only lends its config and tokenizer), so a carrier whose planar and
// blob forms would not fit the box together still gets the rail's gates. The twin maps under the rig's
// own identity (no wscale or backend pin), so the fixture mints no image of its own.
def private run_fixture_served(t : T?; file, head, tag : string; tol : float) {   // nolint:LINT012,LINT019 — the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let path = path_join(models_dir(), file)
        let hpath = path_join(models_dir(), head)
        if (!model_available(t, path) || !model_available(t, hpath)) {
            return
        }
        set_mtp_spec(true)
        set_mtp_head_path(hpath)
        var trg <- served_twin(path, 512l)
        clear_mtp_head_path()
        if (!trg.metal_blob || trg.config.n_layer_nextn <= 0l) {
            t |> skip("{file}: the served image is not a metal-blob NextN carrier on this box - GPU arms have nothing to run")
            set_mtp_spec(false)
            delete trg
            return
        }
        var prose_a <- encode(trg, PROSE_A, true, false)
        var prose_b <- encode(trg, PROSE_B, true, false)
        var count <- encode(trg, counting_prompt(1, 40), true, false)
        if (arm_on(t, "mtp-dff-{tag}")) {   // the batch rail on the twin alone: its reference row is the GPU single step
            dff_cell(t, trg, trg, tag, prose_a, prose_b, tol, false, true)
        }
        run_arms(t, trg, trg, tag, prose_a, prose_b, count, tol)
        metal_decode_shutdown()
        metal_prefill_shutdown()
        set_mtp_spec(false)
        delete prose_a
        delete prose_b
        delete count
        delete trg
        t |> equal(metal_live_object_count(), 0l, "{tag}: no leaked Metal objects after shutdown")
    } else {
        t |> skip("no das_metal on this build")
    }
}

// The hyper-connection carrier: Qwen3.8-Flash-Next UD-IQ4_XS (three shards, 94 GB) with its split
// `shared` Q8_0 head - the wide-residual carry into the draft head, the n-gram side input's panel form
// and the sigmoid deltanet gate all ride the verify rows here and nowhere else. Large tier.
[test]
def test_metal_mtp_parity_flashnext(t : T?) {
    t |> run("metal MTP rail vs plain GPU decode - Qwen3.8-Flash-Next UD-IQ4_XS + split shared head (large tier)") <| @(t : T?) {
        run_fixture_served(t, "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf",
            "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf", "3.8fn", MOE_FF_TOL)
    }
}

// A head-less fixture: only the batch-rail verify arm runs. gemma4's router amplifies any
// float-class difference to ~2.5 logits/step (the support-matrix row's finding), so its bar is
// the row's, not the 0.5 one.
// one row through the batch driver, plain and under the unfused add+rms pin - the row chains'
// shared adapter takes the add + rms pair instead of the fused kernel
def private vff1_cells(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; tol : float; must_serve : bool; expect_decline : string) {
    vff_cell(t, tr, trg, tag, prompt, tol, 1l, must_serve, expect_decline)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_batch_addrms_unfused(true)
        vff_cell(t, tr, trg, "{tag} unfused add+rms", prompt, tol, 1l, must_serve, expect_decline)
        set_metal_batch_addrms_unfused(false)
    }
}

//! the nine-row verify under a mirror ceiling ONE slice fits and nine do not: the rows share the
//! session's slice, so the step's arena demand is that slice once, not once a row
def private vffc_cell(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; tol : float) {   // nolint:LINT012,LINT019 — the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_decode_shutdown()
        metal_prefill_shutdown()
        let prev_cap = get_metal_decode_mirror_cap_mb()
        set_metal_decode_mirror_cap_mb(64)
        vff_cell(t, tr, trg, "{tag} ceiling 64MB", prompt, trg.kquant_native ? max(tol, MM_TOL) : tol, 9l, true)
        set_metal_decode_mirror_cap_mb(prev_cap)
    }
}

//! the same-slab verify arms by row count
def private vff_arms(t : T?; tr, trg : Model; tag : string; prompt : array<int64>; tol : float; must_serve : bool; expect_decline : string) {
    if (arm_on(t, "mtp-vff-{tag}")) {
        vff_cell(t, tr, trg, tag, prompt, tol, 4l, must_serve, expect_decline)
    }
    if (arm_on(t, "mtp-vff1-{tag}")) {   // one row through the batch driver: the encoder alone, no row mixing
        vff1_cells(t, tr, trg, tag, prompt, tol, must_serve, expect_decline)
    }
    if (arm_on(t, "mtp-vff5-{tag}")) {   // five rows: the batch driver's GEMM rail (mp 32) under the same-slab verify
        vff_cell(t, tr, trg, tag, prompt, tol, 5l, must_serve, expect_decline)
    }
    if (arm_on(t, "mtp-vff9-{tag}")) {   // nine rows = MTP_MAX_ROWS: the batch driver's mul_mm rail (kq planes stage x to half)
        vff9_cell(t, tr, trg, tag, prompt, tol, must_serve, expect_decline)
    }
    if (arm_on(t, "mtp-vffc-{tag}") && empty(expect_decline)) {
        vffc_cell(t, tr, trg, tag, prompt, tol)
    }
}

def private run_verify_fixture(t : T?; file, tag : string; tol : float; must_serve : bool = false; expect_decline : string = "") {   // nolint:LINT012,LINT019 — the body is the das_metal half
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
        vff_arms(t, tr, trg, tag, prose_a, tol, must_serve, expect_decline)
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
                set_gemma_draft_on_cpu(arm_on(t, "mtp-count-cpu"))
                set_gemma_carry_pre_norm(arm_on(t, "mtp-count-pre"))
                for (depth in [1l, 2l, 4l]) {
                    count_cell(t, tr, trg, tag, count, depth)
                    let st = gemma_drafter_stats()
                    to_log(LOG_INFO, "gemma drafter {tag} k{depth}: rounds={st.rounds} drafted={st.drafted} accepted={st.accepted}\n")
                    if (depth < 4l && arm_on(t, "mtp-sampled-{tag}")) {
                        count_cell(t, tr, trg, tag, count, depth, true)
                    }
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

// the hybrid carrier (qwen35: deltanet layers, the 2x-wide gated Q), HEAD-LESS so the batch rail's GPU
// logits gate admits it: the batch driver serves a hybrid's rows against one DnMirror per row, and a
// same-slab verify is one session at k+1 positions - the second row finds no mirror at its position,
// so the rail must decline dn_state by name (served, it would run the recurrent state twice per step)
[test]
def test_metal_verify_qwen38_27b(t : T?) {
    t |> run("batch-rail same-slab verify declines a recurrent session's rows as dn_state - Qwen3.8-27B-Q4_K_M, no head (deltanet + gated Q, large tier)") <| @(t : T?) {
        run_verify_fixture(t, "Qwen3.8-27B-Q4_K_M.gguf", "q38", 2.0, false, "dn_state")
    }
}

// plain-MoE control (qwen3moe: routed experts, no parallel dense branch) for the gemma-MoE cell
[test]
def test_metal_verify_qwen30a3(t : T?) {
    t |> run("batch-rail same-slab verify vs plain GPU decode - Qwen3-30B-A3B-Instruct-2507-Q4_K_M (large tier)") <| @(t : T?) {
        run_verify_fixture(t, "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", "q30", 2.0)
    }
}
