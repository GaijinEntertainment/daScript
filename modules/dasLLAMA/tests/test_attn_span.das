options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require math
require dasllama/dasllama_math   // setup_dasllama_jobque_ + set_prefill_attn_mode
require dasllama/dasllama_env

// The non-causal image span (eval_embd_/forward_prefill_embd non_causal=true): every query of
// the span attends [the sliding-window floor, span_end) — the non-causal decode semantics gemma
// vision decodes under. The reference here is the MASK DIRECTION itself: under causal
// attention position 0's residual cannot depend on later rows; under the span it must.
// Needs llama2.c's stories15M.bin (test_flash's fixture); skips cleanly when absent.

def private llama2c_dir() : string {
    let env = g_env_test.llama2c_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

let NPOS = 48l

// deterministic pseudo-random embd rows in a tame range; `perturb_last` perturbs ONLY the final row
def private make_embd(dim : int64; perturb_last : int) : array<float> {
    return <- make_embd_at(dim, perturb_last != 0 ? NPOS - 1l : -1l)
}

// same rows; `perturb_row` >= 0 perturbs ONLY that row
def private make_embd_at(dim, perturb_row : int64) : array<float> {
    var e : array<float>
    e |> resize(NPOS * dim)
    var state = 0x12345u
    for (i in range64(NPOS * dim)) {
        state = state * 1664525u + 1013904223u
        e[i] = (float(state >> 8u) / float(1 << 24) - 0.5) * 0.1
    }
    if (perturb_row >= 0l) {
        for (i in range64(dim)) {
            e[perturb_row * dim + i] += 0.05
        }
    }
    return <- e
}

def private row_maxdiff(a, b : array<float>; row, dim : int64) : float {
    var m = 0.0
    for (i in range64(dim)) {
        m = max(m, abs(a[row * dim + i] - b[row * dim + i]))
    }
    return m
}

// one prefill: returns the full residual plane x_b (npos × dim), caller owns
def private span_prefill(t : Model; embd : array<float>; non_causal : bool; keep : bool = true) : array<float> {
    var s <- make_run_state(t.config)
    s.keep_hidden = keep   //! the instrument reads the whole plane - no last-layer tail narrowing
    forward_prefill_embd(t, s, embd, NPOS, 0l, non_causal)
    var plane : array<float>
    plane |> resize(NPOS * t.config.dim)
    for (i in range64(NPOS * t.config.dim)) {
        plane[i] = s.x_b[i]
    }
    delete s
    return <- plane
}

let SPAN_LO = 7l
let SPAN_HI = 29l

// the spliced turn through eval_embd_span_ (fused or the three-eval splice), then one decode
// step at NPOS — its logits fold in every cached row, so any KV divergence between the two
// prefill shapes surfaces regardless of which quantum wrote it
def private span_turn_logits(t : Model; embd : array<float>; fuse : bool) : array<float> {
    set_span_fuse(fuse)
    var s <- make_run_state(t.config)
    eval_embd_span_(t, s, embd, NPOS, SPAN_LO, SPAN_HI)
    forward(t, s, 1l, s.n_past)
    var lg : array<float>
    lg |> resize(long_length(s.logits))
    for (i in range64(long_length(s.logits))) {
        lg[i] = s.logits[i]
    }
    delete s
    set_span_fuse(true)
    return <- lg
}

// one FUSED prefill with the mid-turn span; returns the full x_b plane, caller owns
def private fused_plane(t : Model; embd : array<float>) : array<float> {
    var s <- make_run_state(t.config)
    s.keep_hidden = true   //! the instrument reads the whole plane - no last-layer tail narrowing
    forward_prefill_embd(t, s, embd, NPOS, 0l, span_lo = SPAN_LO, span_hi = SPAN_HI)
    var plane : array<float>
    plane |> resize(NPOS * t.config.dim)
    for (i in range64(NPOS * t.config.dim)) {
        plane[i] = s.x_b[i]
    }
    delete s
    return <- plane
}

def private arr_maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (i in range64(min(long_length(a), long_length(b)))) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

// the eyeball rail for a logits-tolerance compare: each side's argmax index and value
def private log_argmax_sides(tag : string; a, b : array<float>) {
    var ia = 0l
    var ib = 0l
    for (i in range64(long_length(a))) {
        if (a[i] > a[ia]) {
            ia = i
        }
    }
    for (i in range64(long_length(b))) {
        if (b[i] > b[ib]) {
            ib = i
        }
    }
    to_log(LOG_INFO, "{tag}: argmax A = {ia} ({a[ia]}) | argmax B = {ib} ({b[ib]})\n")
}

var private g_probe_calls = 0

[unused_argument(t, s, npos, start_pos)]
def private probe_override(t : Model; var s : Session; npos, start_pos : int64) : bool {
    g_probe_calls++
    return false
}

[test]
def test_attn_span(t : T?) {
    t |> run("the non-causal span: direction, propagation, mode agreement, flag reset") @(tt : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            tt |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let dim = tr.config.dim
        let embd_a <- make_embd(dim, 0)
        let embd_b <- make_embd(dim, 1)   // differs ONLY in the last row
        with_job_que() {
            setup_dasllama_jobque_()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            // the mask direction: causal position 0 is blind to the last row; the span sees it
            let causal_a <- span_prefill(tr, embd_a, false)
            let causal_b <- span_prefill(tr, embd_b, false)
            let span_a <- span_prefill(tr, embd_a, true)
            let span_b <- span_prefill(tr, embd_b, true)
            tt |> equal(row_maxdiff(causal_a, causal_b, 0l, dim), 0.0, "causal: row 0 blind to a last-row perturbation")
            let d0 = row_maxdiff(span_a, span_b, 0l, dim)
            to_log(LOG_INFO, "attn_span: span row-0 sensitivity to last-row perturbation = {d0}\n")
            tt |> success(d0 > 1e-4, "span: row 0 sees the last row (d0 = {d0})")
            // the span rewrites the whole plane, not just row 0
            let dlast = row_maxdiff(causal_a, span_a, NPOS - 1l, dim)
            tt |> success(dlast > 1e-4, "span vs causal differ at the last row too ({dlast})")
            // three independent kernel implementations agree on the span semantics
            set_prefill_attn_mode(AttnPrefillMode.blocked)
            let span_blocked <- span_prefill(tr, embd_a, true)
            set_prefill_attn_mode(AttnPrefillMode.flash)
            let span_flash <- span_prefill(tr, embd_a, true)
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var mode_diff = 0.0
            for (r in range64(NPOS)) {
                mode_diff = max(mode_diff, row_maxdiff(span_a, span_blocked, r, dim))
            }
            tt |> success(mode_diff < 1e-3, "blocked agrees with classic on the span ({mode_diff})")
            var flash_diff = 0.0
            for (r in range64(NPOS)) {
                flash_diff = max(flash_diff, row_maxdiff(span_a, span_flash, r, dim))
            }
            tt |> success(flash_diff < 1e-2, "flash agrees with classic on the span ({flash_diff})")
            // the flag clears: a causal prefill AFTER a span prefill is bit-identical to fresh causal
            var s2 <- make_run_state(tr.config)
            forward_prefill_embd(tr, s2, embd_a, NPOS, 0l, true)
            tt |> equal(s2.attn_uniform_end, 0l, "attn_uniform_end cleared after the span call")
            delete s2
            let causal_after <- span_prefill(tr, embd_a, false)
            var reset_diff = 0.0
            for (r in range64(NPOS)) {
                reset_diff = max(reset_diff, row_maxdiff(causal_a, causal_after, r, dim))
            }
            tt |> equal(reset_diff, 0.0, "causal after span == fresh causal, bit-exact")
        }
        delete tr
    }
}

// the mrope turn (grid positions through the span): prefill + one decode step; the decode
// logits fold in every cached row AND the delta'd decode position
def private mrope_turn_logits(t : Model; embd : array<float>; fuse : bool; nx, ny : int64) : array<float> {
    set_span_fuse(fuse)
    var s <- make_run_state(t.config)
    eval_embd_span_mrope_(t, s, embd, NPOS, SPAN_LO, SPAN_HI, nx, ny)
    forward(t, s, 1l, s.n_past)
    var lg : array<float>
    lg |> resize(long_length(s.logits))
    for (i in range64(long_length(s.logits))) {
        lg[i] = s.logits[i]
    }
    delete s
    set_span_fuse(true)
    return <- lg
}

[test]
def test_mrope_span(t : T?) {
    t |> run("the mrope quantum: fused == splice, the delta advances, the map reaches the math") @(tt : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            tt |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        // the toy is not an mrope arch — stamp sections summing to head_size/2 (stories15M: 48/2)
        tr.config.rope_sections = int4(8, 8, 8, 0)
        tr.config.mrope_interleaved = true   // the qwen3vl proxy walk
        let dim = tr.config.dim
        let embd_a <- make_embd(dim, 0)
        with_job_que() {
            setup_dasllama_jobque_()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            // span [7,29) = 22 rows = an 11x2 grid; position advance = max(11,2) = 11
            let lg_fused <- mrope_turn_logits(tr, embd_a, true, 11l, 2l)
            let lg_split <- mrope_turn_logits(tr, embd_a, false, 11l, 2l)
            let fs = arr_maxdiff(lg_fused, lg_split)
            log_argmax_sides("mrope_span fused-vs-splice", lg_fused, lg_split)
            to_log(LOG_INFO, "mrope_span: fused-vs-splice decode-logits maxdiff = {fs}\n")
            tt |> equal(fs, 0.0, "mrope fused == mrope splice, bit-exact (classic)")
            // the delta: 22 rows advanced the position by 11
            var s <- make_run_state(tr.config)
            eval_embd_span_mrope_(tr, s, embd_a, NPOS, SPAN_LO, SPAN_HI, 11l, 2l)
            tt |> equal(s.rope_pos_delta, 11l - 22l, "rope_pos_delta = max(grid) - span rows")
            tt |> equal(int(long_length(s.mrope_pos)), 0, "the quantum map cleared after the call")
            tt |> equal(s.n_past, NPOS, "KV rows advanced by the ROW count, not the position advance")
            delete s
            // the map must reach the rope math: grid positions differ from sequential ones
            let lg_plain <- span_turn_logits(tr, embd_a, true)
            let dm = arr_maxdiff(lg_fused, lg_plain)
            log_argmax_sides("mrope_span mrope-vs-sequential", lg_fused, lg_plain)
            to_log(LOG_INFO, "mrope_span: mrope-vs-sequential decode-logits maxdiff = {dm}\n")
            tt |> success(dm > 1e-4, "grid rope positions change the outcome ({dm})")
            // a different grid SHAPE of the same span changes the outcome too (h/w axes live)
            let lg_tall <- mrope_turn_logits(tr, embd_a, true, 2l, 11l)
            let dg = arr_maxdiff(lg_fused, lg_tall)
            tt |> success(dg > 1e-4, "an 11x2 vs 2x11 grid differ ({dg})")
        }
        delete tr
    }
}

[test]
def test_attn_span_fused(t : T?) {
    t |> run("the fused mid-turn span: splice equivalence + per-query mask direction") @(tt : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            tt |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let dim = tr.config.dim
        let embd_a <- make_embd(dim, 0)
        with_job_que() {
            setup_dasllama_jobque_()
            // the fused single eval must leave the SAME cache as the three-eval splice: a decode
            // step at NPOS attends every cached row, so its logits are the whole-cache witness
            set_prefill_attn_mode(AttnPrefillMode.classic)
            let lg_split <- span_turn_logits(tr, embd_a, false)
            let lg_fused <- span_turn_logits(tr, embd_a, true)
            tt |> equal(arr_maxdiff(lg_split, lg_fused), 0.0, "classic: fused == splice, bit-exact decode logits")
            set_prefill_attn_mode(AttnPrefillMode.blocked)
            let lg_split_b <- span_turn_logits(tr, embd_a, false)
            let lg_fused_b <- span_turn_logits(tr, embd_a, true)
            tt |> equal(arr_maxdiff(lg_split_b, lg_fused_b), 0.0, "blocked: fused == splice, bit-exact decode logits")
            set_prefill_attn_mode(AttnPrefillMode.flash)
            let lg_split_f <- span_turn_logits(tr, embd_a, false)
            let lg_fused_f <- span_turn_logits(tr, embd_a, true)
            let fdiff = arr_maxdiff(lg_split_f, lg_fused_f)
            to_log(LOG_INFO, "attn_span fused: flash splice-vs-fused decode-logits maxdiff = {fdiff}\n")
            tt |> success(fdiff < 1e-2, "flash: fused == splice within the online-softmax bar ({fdiff})")
            set_prefill_attn_mode(AttnPrefillMode.classic)
            // per-query mask direction inside ONE fused eval: a tail-row perturbation reaches no
            // earlier row (span rows are bounded by uend, head/tail rows are causal)...
            let base <- fused_plane(tr, embd_a)
            let tail_p <- fused_plane(tr, make_embd(dim, 1))
            var pre_tail = 0.0
            for (r in range64(NPOS - 1l)) {
                pre_tail = max(pre_tail, row_maxdiff(base, tail_p, r, dim))
            }
            tt |> equal(pre_tail, 0.0, "fused: rows before a perturbed last row are blind to it")
            tt |> success(row_maxdiff(base, tail_p, NPOS - 1l, dim) > 1e-4, "fused: the perturbed last row itself moved")
            // ...while a perturbation on the LAST span row reaches the FIRST span row (non-causal
            // within the span) and no head row (causal before the span)
            let span_p <- fused_plane(tr, make_embd_at(dim, SPAN_HI - 1l))
            var pre_span = 0.0
            for (r in range64(SPAN_LO)) {
                pre_span = max(pre_span, row_maxdiff(base, span_p, r, dim))
            }
            tt |> equal(pre_span, 0.0, "fused: head rows are blind to a span perturbation")
            let dspan = row_maxdiff(base, span_p, SPAN_LO, dim)
            tt |> success(dspan > 1e-4, "fused: the first span row sees the last span row ({dspan})")
        }
        delete tr
    }
}

[test]
def test_attn_span_guards(t : T?) {   // nolint:STYLE038 — one sequential guard story over one loaded toy: panics → seats → declines → refusals
    t |> run("the guards and the split-span capability seat") @(tt : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            tt |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let dim = tr.config.dim
        let embd_a <- make_embd(dim, 0)
        with_job_que() {
            setup_dasllama_jobque_()
            // the span-bounds panic: a direct forward_prefill_embd caller with an inverted span
            var tripped = false
            var s1 <- make_run_state(tr.config)
            try {
                forward_prefill_embd(tr, s1, embd_a, NPOS, 0l, span_lo = 5l, span_hi = 3l)
            } recover {
                tripped = true
            }
            tt |> success(tripped, "an inverted span [5,3) panics forward_prefill_embd")
            delete s1
            // the hub guard: a foreign write to the session's span fields must panic the next
            // prefill rather than drive a mask nobody set up
            tripped = false
            var s2 <- make_run_state(tr.config)
            s2.attn_uniform_lo = 9l
            s2.attn_uniform_end = 7l
            var toks : array<int64>
            toks |> resize(8l)
            try {
                forward_prefill(tr, s2, toks, 8l, 0l)
            } recover {
                tripped = true
            }
            tt |> success(tripped, "a foreign attn_uniform span [9,7) panics prefill_attention")
            delete toks
            delete s2
            // the recurrent rejection: a fused span on a recurrent graph must refuse up front
            tripped = false
            var shell = Model()
            shell.config.dim = 8l
            shell.config.recr_mask = 1ul
            var s3 = Session()
            var e3 : array<float>
            e3 |> resize(16l)
            try {
                forward_prefill_embd(shell, s3, e3, 2l, 0l, span_lo = 0l, span_hi = 1l)
            } recover {
                tripped = true
            }
            tt |> success(tripped, "a fused span on recr_mask != 0 panics forward_prefill_embd")
            delete e3
            delete s3
            delete shell
            // the split-span capability seat: an override that registers it forces the
            // three-eval splice (3 override calls, each declining to the CPU arm); one that
            // does not gets the fused single eval
            register_prefill_override("test-split", @@probe_override)
            register_prefill_override_split_span("test-split")
            register_prefill_override("test-fused", @@probe_override)
            select_prefill_override("test-split")
            g_probe_calls = 0
            var s4 <- make_run_state(tr.config)
            eval_embd_span_(tr, s4, embd_a, NPOS, SPAN_LO, SPAN_HI)
            delete s4
            tt |> equal(g_probe_calls, 3, "a split-span override sees THREE prefill calls (the splice)")
            select_prefill_override("test-fused")
            g_probe_calls = 0
            var s5 <- make_run_state(tr.config)
            eval_embd_span_(tr, s5, embd_a, NPOS, SPAN_LO, SPAN_HI)
            delete s5
            tt |> equal(g_probe_calls, 1, "a fused-capable override sees ONE prefill call")
            // the mrope seat: an override without it declines the WHOLE quantum by name -
            // the CPU loop serves it, the override is never asked, and the logits match the
            // no-override run bit-exact
            tr.config.rope_sections = int4(8, 8, 8, 0)
            select_prefill_override("")
            var s6 <- make_run_state(tr.config)
            eval_embd_span_mrope_(tr, s6, embd_a, NPOS, SPAN_LO, SPAN_HI, 11l, 2l)
            select_prefill_override("test-fused")
            g_probe_calls = 0
            var s7 <- make_run_state(tr.config)
            eval_embd_span_mrope_(tr, s7, embd_a, NPOS, SPAN_LO, SPAN_HI, 11l, 2l)
            tt |> equal(g_probe_calls, 0, "a seat-less override never sees an mrope quantum (declined by name)")
            tt |> equal(arr_maxdiff(s6.logits, s7.logits), 0.0, "the declined mrope quantum serves on the CPU loop, bit-exact")
            // the deepstack seat: same decline shape for a wide-row quantum
            tr.config.n_deepstack = 2l
            var inscope wide6 <- make_wide_embd(tr.config.dim, 2l, 1)
            g_probe_calls = 0
            var s8 <- make_run_state(tr.config)
            eval_embd_(tr, s8, wide6, NPOS)
            tt |> equal(g_probe_calls, 0, "a seat-less override never sees a deepstack quantum (declined by name)")
            tr.config.n_deepstack = 0l
            delete s6
            delete s7
            delete s8
            select_prefill_override("")
            // the mrope refusals: inverted span bounds, and a config with no rope sections
            var tripped6 = false
            var s9 <- make_run_state(tr.config)
            try {
                eval_embd_span_mrope_(tr, s9, embd_a, NPOS, 9l, 5l, 2l, 2l)
            } recover {
                tripped6 = true
            }
            tt |> success(tripped6, "an inverted mrope span [9,5) panics")
            delete s9
            tr.config.rope_sections = int4(0, 0, 0, 0)
            tripped6 = false
            var s10 <- make_run_state(tr.config)
            try {
                eval_embd_span_mrope_(tr, s10, embd_a, NPOS, SPAN_LO, SPAN_HI, 11l, 2l)
            } recover {
                tripped6 = true
            }
            tt |> success(tripped6, "an mrope span without rope.dimension_sections panics")
            delete s10
        }
        delete tr
    }
}

// wide rows for the deepstack rail: slice 0 = make_embd's rows, tails filled by `mode` —
// 0 = all-zero, 1 = every slice pseudo-random, 2 = only slice `only` nonzero
def private make_wide_embd(dim, n_ds : int64; mode : int; only : int64 = -1l) : array<float> {
    var inscope narrow <- make_embd(dim, 0)
    let w = (1l + n_ds) * dim
    var e : array<float>
    e |> resize(NPOS * w)
    for (r in range64(NPOS)) {
        for (i in range64(dim)) {
            e[r * w + i] = narrow[r * dim + i]
        }
    }
    if (mode > 0) {
        var state = 0x777u
        for (r in range64(NPOS)) {
            for (sl in range64(n_ds)) {
                for (i in range64(dim)) {
                    state = state * 1664525u + 1013904223u
                    if (mode == 1 || sl == only) {
                        e[r * w + (1l + sl) * dim + i] = (float(state >> 8u) / float(1 << 24) - 0.5) * 0.1
                    }
                }
            }
        }
    }
    return <- e
}

def private ds_eval_logits(t : Model; embd : array<float>) : array<float> {
    var s <- make_run_state(t.config)
    eval_embd_(t, s, embd, NPOS)
    var lg : array<float>
    lg |> resize(long_length(s.logits))
    for (i in range64(long_length(s.logits))) {
        lg[i] = s.logits[i]
    }
    delete s
    return <- lg
}

[test]
def test_deepstack_rail(t : T?) {
    t |> run("the deepstack wide-row seam: split, per-layer adds, clean reset") @(tt : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            tt |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        tr.config.n_deepstack = 3l   // stamp: the toy is not a deepstack arch
        let dim = tr.config.dim
        with_job_que() {
            setup_dasllama_jobque_()
            let narrow <- make_embd(dim, 0)
            let wide0 <- make_wide_embd(dim, 3l, 0)
            let wide <- make_wide_embd(dim, 3l, 1)
            let wide_s0 <- make_wide_embd(dim, 3l, 2, 0l)
            let wide_s2 <- make_wide_embd(dim, 3l, 2, 2l)
            let lg_n <- ds_eval_logits(tr, narrow)
            let lg_w0 <- ds_eval_logits(tr, wide0)
            tt |> equal(arr_maxdiff(lg_n, lg_w0), 0.0, "zero-tail wide rows == the narrow eval, bit-exact")
            let lg_w <- ds_eval_logits(tr, wide)
            let dm = arr_maxdiff(lg_n, lg_w)
            log_argmax_sides("deepstack_rail tails-vs-narrow", lg_n, lg_w)
            to_log(LOG_INFO, "deepstack_rail: tails-vs-narrow logits maxdiff = {dm}\n")
            tt |> success(dm > 1e-4, "nonzero tail slices reach the hidden state ({dm})")
            let lg_s0 <- ds_eval_logits(tr, wide_s0)
            let lg_s2 <- ds_eval_logits(tr, wide_s2)
            let ds02 = arr_maxdiff(lg_s0, lg_s2)
            tt |> success(ds02 > 1e-4, "slice 0 and slice 2 add at different depths ({ds02})")
            // the transient plane must not leak into a following plain eval
            var s <- make_run_state(tr.config)
            eval_embd_(tr, s, wide, NPOS)
            tt |> success(!s.ds_active, "ds_active cleared after the wide quantum")
            var one <- [42l]
            eval_(tr, s, one)
            delete one
            var s2 <- make_run_state(tr.config)
            eval_embd_(tr, s2, wide, NPOS)
            var one2 <- [42l]
            eval_(tr, s2, one2)
            delete one2
            var dd = 0.0
            for (i in range64(long_length(s.logits))) {
                dd = max(dd, abs(s.logits[i] - s2.logits[i]))
            }
            tt |> equal(dd, 0.0, "a text eval after the wide quantum is deterministic (no stale plane)")
            delete s
            delete s2
        }
        delete tr
    }
}
