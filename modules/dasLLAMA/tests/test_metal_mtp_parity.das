options gen2
options persistent_heap   // + explicit deletes: each fixture frees before the next loads
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations + (on Apple) the metal overrides
require dasllama/dasllama_math   // set_wscale_f16 — the resident path reads the f32 scale plane
require dasllama/dasllama_image   // load_model_cached — the served (image) flavor for the GPU twin
require dasllama/dasllama_gguf   // set_mtp_head_path — the split-head fixture names its head explicitly
require ?das_metal dasllama/dasllama_metal_decode   // stats + the forced-reject lever + shutdown
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
def private ff_pair(tr, trg : Model; prompt : array<int64>; nsteps : int64; verify : bool) : FfStats {
    var st = FfStats()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(tr, sa, prompt)   // planar CPU prefill: warms the CPU draft slab + stashes mtp_h
            eval_(tr, sb, prompt)
            select_decode_override("metal")
            let before = metal_decode_stats().decodes
            var tok = greedy_top(sa.logits, vocab)
            var one <- [tok]
            eval_(trg, sa, one)   // the mirror-building step, both sessions (a spec round needs a live mirror)
            eval_(trg, sb, one)
            tok = greedy_top(sa.logits, vocab)
            if (verify) {
                set_metal_mtp_debug("reject")
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
                    var acc = -1l
                    mtp_spec_eval(trg, sb, tok, acc)
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
def private free_run_pair(tr, trg : Model; prompt : array<int64>; n : int64; var plain, spec : array<int64>) : int64 {
    var served = 0l
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(tr, sa, prompt)
            eval_(tr, sb, prompt)
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
            // speculative: commit tok, plus the accepted draft when the round accepts
            tok = greedy_top(sb.logits, vocab)
            while (long_length(spec) < n) {
                spec |> push(tok)
                var acc = -1l
                if (mtp_spec_eval(trg, sb, tok, acc)) {
                    spec |> push(acc)
                }
                tok = greedy_top(sb.logits, vocab)
            }
            spec |> resize(n)
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
def private ff_cell(t : T?; tr, trg : Model; tag, pn : string; prompt : array<int64>) {
    let st = ff_pair(tr, trg, prompt, FF_STEPS, true)
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

        // counting: free-running speculation == free-running plain, token for token
        if (arm_on(t, "mtp-count-{tag}")) {
            var plain : array<int64>
            var spec : array<int64>
            let served = free_run_pair(tr, trg, count, COUNT_STEPS, plain, spec)
            if (served == 0l) {
                t |> skip("metal decode declined (no Metal device?)")
            } else {
                log_gen_texts(tr, "{tag} counting", plain, spec)
                t |> success(same(plain, spec), "{tag} counting: speculative free-run == plain free-run ({COUNT_STEPS} tokens)")
            }
            delete plain
            delete spec
        }

        metal_decode_shutdown()
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
