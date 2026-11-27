options gen2
options _dasllama_internal = true

module _metal_decode_cells shared public

//! The Metal whole-decode rail's cells per model file: every Metal
//! leg prefills and decodes on the form's blob twin and keeps its
//! tokens, logits and stored K/V rows; the CPU form replays the
//! controls on the same ids, and the verdict compares them

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 - the public facade
require dasllama/dasllama_math   // nolint:STYLE030,LINT019 - SEL_HDR, the job queue, das_metal halves
require _metal_blob_twin   // with_metal_rails, batch_greedy_rows, with_metal_attn_d
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - stats, rails
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - chain stats
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - kq half crowns
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - stats, shutdown
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the das_metal halves
require math
require _model_tier
require _model_form   // with_ctx_cut, ModelForm, own_load
require ../performance/model_specs.das   // nolint:STYLE030,LINT019 - counting_prompt
require _kernel_census   // nolint:STYLE029,STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves

//! the context each model file's cells cut their form's model to:
//! the file's largest
let DECODE_CTX_LLAMA_1B_Q8 = 2176l
let DECODE_CTX_LLAMA_3B_Q8 = 4224l
let DECODE_CTX_LLAMA_1B_Q4KM = 2048l
let DECODE_CTX_LLAMA_1B_Q5KM = 512l
let DECODE_CTX_LLAMA31_8B_Q8 = 512l
let DECODE_CTX_QWEN35_08B_Q8OUT = 512l
//! each of the four per-32 carriers' (`DecodePer32Row`)
let DECODE_CTX_PER32 = 512l

let private BATCH_CTX = 512l
let private MAIN_CTX = 2048l
let private DEEP_CTX = 2176l
let private DEEP3_CTX = 4224l

//! the shape of one token run a check makes on either form
enum DecodeRunKind {
    greedy        //! one session: the prompt, then greedy steps
    batch         //! the batched step's rows (`eval_batch_`)
    paged         //! two paged sessions' chunks, steps on the second
    paged_batch   //! paged sessions' chunks, then batched steps
    rewind        //! eight steps, a rewind to 0, the second prompt
}

//! one token run: the Metal leg and the CPU control run it alike
struct DecodeRun {
    kind : DecodeRunKind
    prompts : array<array<int64>>
    n : int
    kdt : KVDtype
    ctx : int64
    shrink_at : int   //!< the step after which row 1 retires: -1 none
    single_every : int     //!< past 0: every this many steps a one-row step
    page_rows : int64
    //! a blob's prefill driver shut down between the prompts and the
    //! steps: the steps' forms as with no prefill driver up
    pf_down : bool
}

//! a token compare the verdict makes: the Metal leg's streams
//! against the CPU control `control` runs on the CPU form
struct DecodeCheck {
    key : string   //!< the control's name: one CPU run however many checks read it
    control : DecodeRun
    what : string  //!< the assert's text: "" a control nothing compares against
    gpu : array<array<int64>>
    cpu : array<array<int64>>
    cpu_ran : bool
    rows : int     //!< the leading rows compared: 0 every row and the row count
    metal_bad : bool   //!< a Metal-side condition the assert also holds came out false
    log_tag : string     //!< the CPU form logs `log_rows` rows' texts under it
    log_rows : int
    len_assert : bool    //!< the control's first stream holds `control.n` tokens
    counter_ctl : bool   //!< the control moves no batched Metal step
}

//! one side of a one-step compare: the steps the Metal decode
//! served, the fed ids, each row's logits and stored K/V row
struct DecodeStepSide {
    served : int64
    ids : array<int64>
    logits : array<array<float>>
    kv : array<array<uint8>>
}

//! a one-step logits and stored-K/V-row compare: the Metal side
//! prefills, picks the ids and steps, and keeps its sessions; the CPU
//! side rewinds them one position and takes the same step
struct DecodeGate {
    tag : string
    kdt : KVDtype
    tol_logits : float
    tol_kv : float
    batched : bool
    ctx : int64
    vocab : int64
    kv_dims : array<int64>
    prompts : array<array<int64>>
    gpu : DecodeStepSide
    cpu : DecodeStepSide
    cpu_ran : bool
    //! the Metal side's sessions past its step, until the CPU replay
    ss : array<Session>
}

//! what one model file's Metal cells hand its CPU form and verdict
struct DecodeKept {
    checks : array<DecodeCheck>
    gates : array<DecodeGate>
    //! the Metal form's f32 weight scale: the CPU form the controls
    //! wait for loads on the same scale (`decode_cpu_form`)
    wscale_f32 : bool
}

//! the CPU form a Metal form's controls run on: the portable pin, the
//! f32 weight scale where the Metal form loaded on it
def decode_cpu_form(wscale_f32 : bool = false) : ModelForm =>
    cpu_cells_form(backend = "portable", wscale_f32 = wscale_f32)

//! both overrides on the model's home - "metal" on a blob, "" on a
//! CPU load - the floor left
def private home_rails(m : Model) : MetalRails =>
    MetalRails(rail = m.metal_blob ? "metal" : "", min_npos = 0l)

def private continuation(m : Model; prompt : array<int64>; n : int; kdt : KVDtype) : array<int64> {
    var got : array<int64>
    with_dasllama_jobque() {
        var s = make_run_state(m.config, kdt, kdt)
        eval_(m, s, prompt)
        got <- greedy_eval_steps(m, s, n) // nolint:PERF030 - fresh; queue runs once
        delete s
    }
    return <- got
}

//! two paged sessions' prompts in alternating page-sized chunks, so
//! the second's rows gather through several physical runs; then `n`
//! greedy steps on it
def private paged_greedy(m : Model; prompt : array<int64>; page_rows : int64;
                         n : int) : array<int64> {
    var got : array<int64>
    with_dasllama_jobque() {
        var pool <- create_kv_pool_(m.config, page_rows, KVDtype.f16, KVDtype.f16)
        var sa <- make_run_state_paged(m.config, pool)
        var sb <- make_run_state_paged(m.config, pool)
        let np = long_length(prompt)
        var at = 0l
        while (at < np) {
            let hi = min(at + page_rows, np)
            var inscope chunk <- [for (i in range64(at, hi)); prompt[i]]
            eval_(m, sa, chunk)
            eval_(m, sb, chunk)
            delete chunk
            at = hi
        }
        got <- greedy_eval_steps(m, sb, n) // nolint:PERF030 - fresh; queue runs once
        release_kv_pages_(sa)
        release_kv_pages_(sb)
        delete sa
        delete sb
        delete pool
    }
    return <- got
}

//! decode, rewind to 0 (the ASR session reuse), a SHORTER different
//! prompt, decode again: stale mirror rows past the rewind would
//! poison the tokens
def private rewind_greedy(m : Model; prompt, prompt2 : array<int64>; n : int) : array<int64> {
    var got : array<int64>
    with_dasllama_jobque() {
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, s, prompt)
        var warm <- greedy_eval_steps(m, s, 8)
        delete warm
        s.n_past = 0l
        eval_(m, s, prompt2)
        got <- greedy_eval_steps(m, s, n) // nolint:PERF030 - fresh; queue runs once
        delete s
    }
    return <- got
}

//! paged sessions' prompts in alternating page-sized chunks
//! (multi-run mirror gathers), then batched greedy steps
def private paged_batch_greedy(m : Model; prompts : array<array<int64>>; page_rows : int64;
                               nsteps : int) : array<array<int64>> {
    let c = m.config
    let vocab = c.vocab_size
    let nb = length(prompts)
    var got : array<array<int64>>
    got |> resize(nb)
    with_dasllama_jobque() {
        var pool <- create_kv_pool_(c, page_rows, KVDtype.f16, KVDtype.f16)
        var ss : array<Session>
        ss |> reserve(nb)
        for (_i in range(nb)) {
            ss |> emplace <| make_run_state_paged(c, pool)
        }
        var longest = 0l
        for (p in prompts) {
            longest = max(longest, long_length(p))
        }
        var at = 0l
        while (at < longest) {
            for (i in range(nb)) {
                let hi = min(at + page_rows, long_length(prompts[i]))
                if (at < hi) {
                    var inscope chunk <- [for (j in range64(at, hi)); prompts[i][j]]
                    eval_(m, ss[i], chunk)
                    delete chunk
                }
            }
            at += page_rows
        }
        var ws <- create_batch_workspace_(m)
        var sess : array<Session?>
        var cur : array<int64>
        for (i in range(nb)) {
            sess |> push(unsafe(addr(ss[i])))
            cur |> push(argmax_of(ss[i].logits, vocab))
        }
        for (_st in range(nsteps)) {
            eval_batch_(m, ws, sess, cur)
            for (k in range(nb)) {
                cur[k] = argmax_of(ss[k].logits, vocab)
                got[k] |> push(cur[k])
            }
        }
        for (i in range(nb)) {
            release_kv_pages_(ss[i])
        }
        sess |> clear()   // the borrowed handles: delete of array<T?> frees the pointees
        unsafe {
            delete ws
            delete sess
        }
        delete cur
        delete ss
        delete pool
    }
    return <- got
}

def private run_kind(m : Model; r : DecodeRun) : array<array<int64>> {
    if (r.kind == DecodeRunKind.batch) {
        var rows : array<array<int64>>
        with_dasllama_jobque() {
            rows <- batch_greedy_rows(m, r.prompts, r.n, // nolint:PERF030 - fresh; runs once
                r.kdt, r.shrink_at, r.single_every, r.pf_down)
        }
        return <- rows
    }
    if (r.kind == DecodeRunKind.paged_batch) {
        return <- paged_batch_greedy(m, r.prompts, r.page_rows, r.n)
    }
    var one : array<array<int64>>
    if (r.kind == DecodeRunKind.greedy) {
        one |> emplace <| continuation(m, r.prompts[0], r.n, r.kdt)
    } elif (r.kind == DecodeRunKind.paged) {
        one |> emplace <| paged_greedy(m, r.prompts[0], r.page_rows, r.n)
    } else {
        one |> emplace <| rewind_greedy(m, r.prompts[0], r.prompts[1], r.n)
    }
    return <- one
}

//! `r` on the model's home, its context cut to the run's
def private run_streams(var m : Model; r : DecodeRun) : array<array<int64>> {
    var got : array<array<int64>>
    with_ctx_cut(m, r.ctx) {
        with_metal_rails(home_rails(m)) {
            got <- run_kind(m, r) // nolint:PERF030 - fresh; the block runs once
        }
    }
    return <- got
}

def private run_of(kind : DecodeRunKind; prompts : array<array<int64>>; n : int; ctx : int64;
                   kdt : KVDtype = KVDtype.f16; pf_down : bool = false) : DecodeRun {
    var r : DecodeRun
    r.kind = kind
    r.prompts := prompts
    r.n = n
    r.ctx = ctx
    r.kdt = kdt
    r.shrink_at = -1
    r.pf_down = pf_down
    return <- r
}

def private one_of(p : array<int64>) : array<array<int64>> {
    var a : array<array<int64>>
    a |> push_clone(p)
    return <- a
}

def private counting_set(m : Model; spans : array<int2>) : array<array<int64>> =>
    <- [for (s in spans); encode(m, counting_prompt(s.x, s.y), true, false)]

//! `nb` counting prompts, the i-th from `first + i` to `base + step *
//! i`
def private counting_rows(m : Model; nb, first, base, step : int) : array<array<int64>> {
    return <- [for (i in range(nb)); encode(m, counting_prompt(first + i, base + step * i), true,
                                            false)]
}

def private push_check(var kept : DecodeKept; key, what : string; r : DecodeRun;
                       var gpu : array<array<int64>>; log_tag : string = "";
                       log_rows : int = 0) : int {
    var c <- DecodeCheck(key = key, what = what, log_tag = log_tag, log_rows = log_rows)
    c.control := r
    c.gpu <- gpu
    kept.checks |> emplace(c)
    return length(kept.checks) - 1
}

//! the Metal leg of `r` kept against the CPU control `key` running
//! `r` the same way
def private metal_leg(var m : Model; var kept : DecodeKept; key, what : string; r : DecodeRun;
                      log_tag : string = ""; log_rows : int = 0) {
    var gpu <- run_streams(m, r)
    push_check(kept, key, what, r, gpu, log_tag, log_rows)
}

def private streams_match(a, b : array<array<int64>>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (!ids_equal(x, y))
    }
    return true
}

def private rows_match(a, b : array<array<int64>>; rows : int) : bool {
    return false if (length(a) < rows || length(b) < rows)
    for (r in range(rows)) {
        return false if (!ids_equal(a[r], b[r]))
    }
    return true
}

//! one session's stored K/V row `pos`: every layer's K row, then its
//! V row, in the session's codecs
def private kv_row_of(s : Session; c : Config; pos : int64) : array<uint8> {
    var row : array<uint8>
    for (l in range64(c.n_layers)) {
        let kv_dim = layer_kv_dim(c, l)
        let rk = kv_row_bytes(s.kv_dtype_k, kv_dim)
        let rv = kv_row_bytes(s.kv_dtype_v, kv_dim)
        let at = long_length(row)
        row |> resize(at + rk + rv)
        unsafe {
            memcpy(addr(row[at]), addr(s.key_cache[kv_base_k(s, l, c.seq_len) + pos * rk]), rk)
            let bv = kv_base_v(s, l, c.seq_len) + pos * rv
            memcpy(addr(row[at + rk]), addr(s.value_cache[bv]), rv)
        }
    }
    return <- row
}

//! max |d| between two `kv_row_of` rows across every layer's K and V
def private kv_rows_max_diff(a, b : array<uint8>; kv_dims : array<int64>; dt : KVDtype) : float {
    return 0.0 if (empty(a) || length(a) != length(b))
    var maxd = 0.0
    var at = 0l
    for (kv_dim in kv_dims) {
        maxd = max(maxd, kv_row_max_diff(a, b, at, dt, kv_dim))
        at += kv_row_bytes(dt, kv_dim)
        maxd = max(maxd, kv_row_max_diff(a, b, at, dt, kv_dim))
        at += kv_row_bytes(dt, kv_dim)
    }
    return maxd
}

//! one step of every session on the model's home, fed `ids`, batched
//! through `eval_batch_`: each row's logits and stored K/V row
def private gate_step(m : Model; var ss : array<Session>; ids : array<int64>;
                      batched : bool) : DecodeStepSide {
    var side : DecodeStepSide
    side.ids := ids
    with_dasllama_jobque() {
        let before = batched ? metal_batch_step_count() : metal_decode_count()
        if (batched) {
            var ws <- create_batch_workspace_(m)
            var sess <- [for (i in range(length(ss))); unsafe(addr(ss[i]))]
            eval_batch_(m, ws, sess, ids)
            sess |> clear()   // the borrowed handles: delete of array<T?> frees the pointees
            unsafe {
                delete ws
                delete sess
            }
        } else {
            var one <- [ids[0]]
            eval_(m, ss[0], one)
            delete one
        }
        side.served = (batched ? metal_batch_step_count() : metal_decode_count()) - before
    }
    side.logits |> reserve(length(ss))
    side.kv |> reserve(length(ss))
    for (s in ss) {
        side.logits |> push_clone(s.logits)
        side.kv |> emplace <| kv_row_of(s, m.config, s.n_past - 1l)
    }
    return <- side
}

//! the Metal side of a one-step gate on `prompts`: the prefills, each
//! one's argmax stepped, the sessions kept for the CPU replay
def private gate_metal(t : T?; var m : Model; var kept : DecodeKept; tag : string;
                       var prompts : array<array<int64>>; kdt : KVDtype; tol_logits, tol_kv : float;
                       batched : bool; ctx : int64) {
    var g <- DecodeGate(tag = tag, kdt = kdt, tol_logits = tol_logits, tol_kv = tol_kv,
        batched = batched, vocab = m.config.vocab_size)
    g.prompts <- prompts
    g.kv_dims <- [for (l in range64(m.config.n_layers)); layer_kv_dim(m.config, l)]
    with_ctx_cut(m, ctx) {
        g.ctx = m.config.seq_len
        let asked = MetalRails(prefill = false, decode = !batched, min_npos = 0l)
        with_metal_rails(t, tag, asked) {
            with_metal_rails(home_rails(m)) {
                var ids : array<int64>
                with_dasllama_jobque() {
                    g.ss |> reserve(length(g.prompts))
                    for (p, i in g.prompts, count()) {
                        g.ss |> emplace <| make_run_state(m.config, kdt, kdt)
                        eval_(m, g.ss[i], p)
                        ids |> push(argmax_of(g.ss[i].logits, g.vocab))
                    }
                }
                g.gpu <- gate_step(m, g.ss, ids, batched)
                delete ids
            }
        }
    }
    if (g.gpu.served == 0l) {
        delete g.ss
    }
    kept.gates |> emplace(g)
}

//! the CPU side of a one-step gate: the kept sessions rewound one
//! position, the Metal side's step taken again, the sessions freed
def private gate_replay(t : T?; var m : Model; var g : DecodeGate) {
    with_ctx_cut(m, g.ctx) {
        if (m.config.seq_len != g.ctx) {
            t |> failure(("{g.tag}: the CPU form's context {m.config.seq_len} is not the kept " +
                "sessions' {g.ctx}"))
            return
        }
        for (s in g.ss) {
            s.n_past--
        }
        with_metal_rails(home_rails(m)) {
            g.cpu <- gate_step(m, g.ss, g.gpu.ids, g.batched)
        }
        g.cpu_ran = true
    }
    delete g.ss
}

//! one CPU step and one GPU step from the same prompt: logits and
//! stored-K/V-row envelope
def private single_gate(t : T?; var m : Model; var kept : DecodeKept; prompt : array<int64>;
                        kdt : KVDtype; tol_logits, tol_kv : float; tag : string; ctx : int64) {
    var prompts <- one_of(prompt)
    gate_metal(t, m, kept, tag, prompts, kdt, tol_logits, tol_kv, false, ctx)
}

//! the batch twin of `single_gate`: `nb` counting prompts, `plen`
//! scaling them down for the big dim-ladder models
def private batch_gate(t : T?; var m : Model; var kept : DecodeKept; nb : int; kdt : KVDtype;
                       tol_logits, tol_kv : float; tag : string; ctx : int64; plen : int = 40) {
    var prompts <- counting_rows(m, nb, 1, plen, 7)
    gate_metal(t, m, kept, tag, prompts, kdt, tol_logits, tol_kv, true, ctx)
}

let private PROSE_C = ("The committee met for the third time that week, and by the end of the " +
    "afternoon it was clear to everyone in the room that the budget would not survive the vote " +
    "unless")
let private PROSE_D = ("She had learned to read the weather from the colour of the hills, and on " +
    "the morning the fog failed to lift she packed the cart early and told the boys")

//! a batch-ff pair's counters: the steps fed, each rail's served
//! steps, the argmax flips and the widest logits distance
struct private BatchFf {
    steps : int64
    served_single : int64
    served_batch : int64
    flips : int64
    maxd : float
}

//! GPU-vs-GPU forced feed over real text: each stream fed the same
//! token to its one-row twin and its batched twin, the two logits
//! rows compared before either can drift - the kernel-swap envelope
def private batch_ff_pair(m : Model; prompts : array<array<int64>>;
                          nsteps : int) : BatchFf {
    var st = BatchFf()   // nolint:LINT003,LINT019 - filled in the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = m.config
        let vocab = c.vocab_size
        let nb = length(prompts)
        with_dasllama_jobque() {
            var sa, sb : array<Session>
            sa |> reserve(nb)
            sb |> reserve(nb)
            for (i in range(nb)) {
                sa |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                sb |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                eval_(m, sa[i], prompts[i])
                eval_(m, sb[i], prompts[i])
            }
            var ws <- create_batch_workspace_(m)
            var sessb : array<Session?>
            var cur : array<int64>
            for (i in range(nb)) {
                sessb |> push(unsafe(addr(sb[i])))
                cur |> push(argmax_of(sa[i].logits, vocab))
            }
            let d0 = metal_decode_stats().decodes
            let b0 = metal_batch_decode_stats().steps
            for (_st in range(nsteps)) {
                for (i in range(nb)) {
                    st.maxd = max(st.maxd, logits_maxdiff(sa[i].logits, sb[i].logits))
                    if (argmax_of(sb[i].logits, vocab) != cur[i]) {
                        st.flips++
                    }
                }
                for (i in range(nb)) {
                    forward(m, sa[i], cur[i], sa[i].n_past)
                    sa[i].n_past++
                }
                eval_batch_(m, ws, sessb, cur)
                for (i in range(nb)) {
                    cur[i] = argmax_of(sa[i].logits, vocab)
                }
                st.steps++
            }
            st.served_single = metal_decode_stats().decodes - d0
            st.served_batch = metal_batch_decode_stats().steps - b0
            sessb |> clear()   // the borrowed handles: delete of array<T?> frees the pointees
            unsafe {
                delete ws
                delete sessb
            }
            delete cur
            delete sa
            delete sb
        }
    }
    return st
}

//! arm batch-ff: B=2 (the B2 GEMV form) and B=4 (the B4 form), the
//! batched step against the one-row step on identical tokens
def private batch_ff_cell(t : T?; var m : Model) {   // nolint:LINT014,LINT019 - das_metal half
    let texts = fixed_array(PROSE_LIGHTHOUSE, PROSE_RIVER, PROSE_C, PROSE_D)
    var prose <- [for (p in texts); encode(m, p, true, false)]
    for (nb in [2, 4]) {
        var inscope sub <- [for (i in range(nb)); clone_to_move(prose[i])]
        var st : BatchFf
        with_metal_rails(home_rails(m)) {
            st = batch_ff_pair(m, sub, 32)
        }
        if (st.served_single == 0l || st.served_batch == 0l) {
            t |> skip("batch-ff B={nb}: a GPU rail declined (single {st.served_single}, batch " +
                "{st.served_batch} steps)")
        } else {
            t |> success(st.maxd <= 0.5, ("batch-ff B={nb}: batched step vs single step within " +
                "0.5 " +
                "over {st.steps} forced-feed steps of real text (maxd {st.maxd}, argmax flips " +
                "{st.flips})"))
            to_log(LOG_INFO, ("metal batch-ff B={nb}: maxd={st.maxd} flips={st.flips} " +
                "steps={st.steps}\n"))
        }
    }
    delete prose
}

//! the batched leg of `r` with its served-steps assert
def private served_batch_leg(t : T?; var m : Model; var kept : DecodeKept; key, what : string;
                             r : DecodeRun; served_what : string) {
    let before = metal_batch_step_count()
    metal_leg(m, kept, key, what, r)
    t |> success(metal_batch_step_count() - before > 0l, served_what)
}

//! the single-row leg of `r` with its served-steps assert
def private served_decode_leg(t : T?; var m : Model; var kept : DecodeKept; key, what : string;
                              r : DecodeRun; served_what : string) {
    let before = metal_decode_count()
    metal_leg(m, kept, key, what, r)
    t |> success(metal_decode_count() - before > 0l, served_what)
}

//! arms B1 (B=3 ragged + mid-run shrink) and B1b: false where the
//! batched step declined (no Metal device), the rest skipped
def private batch_b1_arms(var m : Model; var kept : DecodeKept;
                          prompts3 : array<array<int64>>) : bool {
    var r3 <- run_of(DecodeRunKind.batch, prompts3, 24, BATCH_CTX)
    r3.shrink_at = 8
    let before = metal_batch_step_count()
    metal_leg(m, kept, "cpu3", ("B=3 ragged + shrink: batched greedy streams match the CPU " +
        "control " +
        "(GEMV path)"), r3)
    if (metal_batch_step_count() - before == 0l) {
        kept.checks |> pop()
        feint("metal batch decode declined (no Metal device?); batch parity skipped\n")
        return false
    }
    var inscope prompts2 <- counting_set(m, [int2(2, 50), int2(8, 38)])
    metal_leg(m, kept, "cpu2", "B=2: batched greedy streams match the CPU control (B2 GEMV path)",
        run_of(DecodeRunKind.batch, prompts2, 20, BATCH_CTX))
    return true
}

//! arms B2 (B=6, the M-pad-32 GEMM path) and B3 (interleaved one-row
//! steps: batch <-> single mirror consistency)
def private batch_b2_b3_arms(t : T?; var m : Model; var kept : DecodeKept;
                             prompts3, prompts6 : array<array<int64>>) {
    served_batch_leg(t, m, kept, "cpu6", ("B=6: batched greedy streams match the CPU control " +
        "(GEMM path)"),
        run_of(DecodeRunKind.batch, prompts6, 16, BATCH_CTX), "B=6 arm served on the GPU")
    var rm <- run_of(DecodeRunKind.batch, prompts3, 18, BATCH_CTX)
    rm.single_every = 3
    metal_leg(m, kept, "cpum", "mixed batch/single schedule matches the CPU control", rm)
}

//! arm B4: one-step logits + stored-K/V-row tolerance, both paths x
//! every mirror codec (tq4 at its quant-step-scale envelopes)
def private batch_b4_arm(t : T?; var m : Model; var kept : DecodeKept) {
    batch_gate(t, m, kept, 3, KVDtype.f16, 0.5, 0.25, "B=3 f16", BATCH_CTX)
    batch_gate(t, m, kept, 6, KVDtype.f16, 0.5, 0.25, "B=6 f16", BATCH_CTX)
    batch_gate(t, m, kept, 3, KVDtype.f32, 0.5, 0.25, "B=3 f32", BATCH_CTX)
    batch_gate(t, m, kept, 6, KVDtype.f32, 0.5, 0.25, "B=6 f32", BATCH_CTX)
    batch_gate(t, m, kept, 3, KVDtype.q8_0, 0.5, 0.25, "B=3 q8_0", BATCH_CTX)
    batch_gate(t, m, kept, 6, KVDtype.q8_0, 0.5, 0.25, "B=6 q8_0", BATCH_CTX)
    batch_gate(t, m, kept, 3, KVDtype.tq4, 1.0, 2.0, "B=3 tq4", BATCH_CTX)
    batch_gate(t, m, kept, 6, KVDtype.tq4, 1.0, 2.0, "B=6 tq4", BATCH_CTX)
}

//! arms B4b (q8_0 streams: GEMV + chunked partB at B=3, GEMM at B=6),
//! B4c (tq4 streams) and B5 (paged sessions' multi-run gathers)
def private batch_codec_arms(t : T?; var m : Model; var kept : DecodeKept;
                             prompts3, prompts6 : array<array<int64>>) {
    served_batch_leg(t, m, kept, "cpu8b", "B=3 q8_0 batched greedy streams match the CPU control",
        run_of(DecodeRunKind.batch, prompts3, 24, BATCH_CTX, KVDtype.q8_0),
        "B=3 q8_0 arm served on the GPU")
    metal_leg(m, kept, "cpu8b6", ("B=6 q8_0 batched greedy streams match the CPU control (GEMM " +
        "path)"),
        run_of(DecodeRunKind.batch, prompts6, 12, BATCH_CTX, KVDtype.q8_0))
    served_batch_leg(t, m, kept, "cpu4b", "B=3 tq4 batched greedy streams match the CPU control",
        run_of(DecodeRunKind.batch, prompts3, 24, BATCH_CTX, KVDtype.tq4),
        "B=3 tq4 arm served on the GPU")
    var rp <- run_of(DecodeRunKind.paged_batch, prompts3, 8, BATCH_CTX)
    rp.page_rows = 64l
    served_batch_leg(t, m, kept, "paged-b", "paged batched streams match the CPU control", rp,
        "paged batch arm served on the GPU")
}

//! the batched-decode arms B1-B5 and batch-ff on the 1B Q8's form
def private batch_arms_l1b(t : T?; var m : Model; var kept : DecodeKept) {
    with_ctx_cut(m, BATCH_CTX) {
        var inscope prompts3 <- counting_set(m, [int2(1, 60), int2(7, 45), int2(3, 70)])
        var inscope prompts6 <- counting_set(m, [int2(1, 40), int2(5, 52), int2(2, 33), int2(9, 61),
                                                 int2(4, 47), int2(6, 55)])
        if (batch_b1_arms(m, kept, prompts3)) {
            batch_b2_b3_arms(t, m, kept, prompts3, prompts6)
            batch_b4_arm(t, m, kept)
            batch_codec_arms(t, m, kept, prompts3, prompts6)
            if (arm_on(t, "batch-ff")) {
                batch_ff_cell(t, m)
            }
            static_if (typeinfo builtin_module_exists(das_metal)) {
                metal_decode_stage_report()
            }
        }
    }
}

//! arm 1: the 40-token continuation's GPU decode; false where it
//! declined (no Metal device), the rest skipped
def private main_arm1(t : T?; var m : Model; var kept : DecodeKept; cont : DecodeRun) : bool {
    var gpu16 : array<array<int64>>
    var served = 0l
    with_metal_rails(t, "arm 1", MetalRails(prefill = false, min_npos = 0l)) {
        let before = metal_decode_count()
        gpu16 <- run_streams(m, cont) // nolint:PERF030 - fresh; the block runs once
        served = metal_decode_count() - before
    }
    if (served == 0l) {
        feint("metal decode declined (no Metal device?); GPU parity skipped\n")
        return false
    }
    push_check(kept, "cpu16", ("40-token continuation matches the CPU control (GPU prefill -> " +
        "GPU " +
        "decode)"), cont, gpu16)
    return true
}

//! arm 2: the prefill on the Metal prefill, the decode on the GPU
def private main_arm2(t : T?; var m : Model; var kept : DecodeKept;
                      cont : DecodeRun) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var hyb : array<array<int64>>
        var hyb_decodes = 0l
        var hyb_prefills = 0l
        with_metal_rails(t, "arm 2", MetalRails(decode = false)) {
            select_decode_override("metal")
            let before = metal_decode_stats().decodes
            let pf_before = metal_prefill_stats().prefills
            hyb <- run_streams(m, cont) // nolint:PERF030 - fresh; the block runs once
            hyb_decodes = metal_decode_stats().decodes - before
            hyb_prefills = metal_prefill_stats().prefills - pf_before
        }
        t |> success(hyb_decodes > 0l && hyb_prefills > 0l, "hybrid arm served on both devices")
        push_check(kept, "cpu16", ("40-token continuation matches the CPU control (Metal " +
            "prefill -> " +
            "GPU decode)"), cont, hyb)
    }
}

//! arm 13: the concurrent encoder and hazard tracker - PARANOID
//! isolates the encoder flip, STRICT proves declaration coverage with
//! barrier elision live
def private main_arm13(t : T?; var m : Model; var kept : DecodeKept;
                       cont : DecodeRun) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let paranoid0 = g_hz_paranoid
        let strict0 = g_hz_strict
        let conc0 = get_metal_decode_concurrent()
        set_metal_decode_concurrent(1)
        set_metal_decode_hazard(true, false)
        metal_leg(m, kept, "cpu16", ("40-token continuation matches the CPU control (PARANOID " +
            "concurrent encode)"), cont)
        set_metal_decode_hazard(false, true)
        let h0 = metal_hazard_stats()
        metal_leg(m, kept, "cpu16", ("40-token continuation matches the CPU control (STRICT " +
            "concurrent encode)"), cont)
        let hz = metal_hazard_stats()
        set_metal_decode_hazard(paranoid0, strict0)
        set_metal_decode_concurrent(conc0)
        t |> success(hz.gated > h0.gated, "hz_gate saw the step's dispatches")
        t |> success(hz.barriers - h0.barriers < hz.gated - h0.gated,
            "barrier elision engaged (barriers < gated dispatches)")
        t |> equal(hz.undeclared, h0.undeclared)
    }
}

//! arms 4 (paged: interleaved page tables, multi-run mirror gathers)
//! and 5 (rewind: an n_past = 0 reuse re-mirrors from scratch)
def private main_paged_rewind(t : T?; var m : Model; var kept : DecodeKept;
                              prompt, prompt2 : array<int64>) {
    if (arm_on(t, "arm4-paged")) {
        var rp <- run_of(DecodeRunKind.paged, one_of(prompt), 16, MAIN_CTX)
        rp.page_rows = 64l
        served_decode_leg(t, m, kept, "paged-16", ("16-token paged continuation matches the CPU " +
            "control"),
            rp, "paged arm served on the GPU")
    }
    if (arm_on(t, "arm5-rewind")) {
        var inscope both : array<array<int64>>
        both |> push_clone(prompt)
        both |> push_clone(prompt2)
        let before = metal_decode_count()
        var gpu <- run_streams(m, run_of(DecodeRunKind.rewind, both, 16, MAIN_CTX))
        t |> success(metal_decode_count() - before > 0l, "rewind arm served on the GPU")
        push_check(kept, "rewind-ctl", "16-token post-rewind continuation matches the CPU control",
            run_of(DecodeRunKind.greedy, one_of(prompt2), 16, MAIN_CTX), gpu)
    }
}

//! arm 6: session churn under a 1 MiB LRU cap - the retired
//! sessions' mirrors evict
def private main_arm6(t : T?; var m : Model; prompt2 : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_cap = get_metal_decode_mirror_cap_mb()
        set_metal_decode_mirror_cap_mb(1)
        with_metal_rails(home_rails(m)) {
            with_dasllama_jobque() {
                for (_i in range(3)) {
                    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                    eval_(m, s, prompt2)
                    var ids <- greedy_eval_steps(m, s, 4)
                    delete ids
                    delete s
                }
            }
        }
        t |> equal(metal_decode_mirror_stats().mirrors, 1l)
        set_metal_decode_mirror_cap_mb(prev_cap)
    }
}

//! the two streams arms 6b and 6c run: the second prompt, then the
//! first, six batched steps
def private two_streams(prompt, prompt2 : array<int64>) : DecodeRun {
    var two : array<array<int64>>
    two |> push_clone(prompt2)
    two |> push_clone(prompt)
    return <- run_of(DecodeRunKind.batch, two, 6, MAIN_CTX)
}

//! arm 6b: the idle cap bounds no live step - two streams past a
//! 1 MiB cap many times over still take the batched GPU step
def private main_arm6b(t : T?; var m : Model; var kept : DecodeKept;
                       r : DecodeRun) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_cap = get_metal_decode_mirror_cap_mb()
        set_metal_decode_mirror_cap_mb(1)
        var row_bytes = 0l
        for (l in range64(m.config.n_layers)) {
            if (layer_kv_source(m.config, l) == l) {
                row_bytes += 2l * kv_row_bytes(KVDtype.f16, layer_kv_dim(m.config, l))
            }
        }
        let demand = row_bytes * (long_length(r.prompts[0]) + long_length(r.prompts[1]) + 12l)
        t |> success(demand > 8l * 1048576l, ("the two streams' K/V ({demand} bytes) passes the " +
            "1 MiB " +
            "cap many times over"))
        let bst0 = metal_batch_decode_stats()
        var gpu_two <- run_streams(m, r)
        let bst1 = metal_batch_decode_stats()
        t |> success(bst1.steps - bst0.steps >= 6l, ("the batched steps served on the GPU under " +
            "a 1 MiB " +
            "idle cap ({bst1.steps - bst0.steps} of 6)"))
        t |> equal(bst1.declines - bst0.declines, 0l, "and none declined")
        let i = push_check(kept, "cpu-two", "both streams match the CPU control", r, gpu_two,
            "arm6b", 2)
        kept.checks[i].counter_ctl = true
        with_ctx_cut(m, MAIN_CTX) {
            t |> equal(device_kv_max_rows(m, KVDtype.f16, KVDtype.f16), m.config.seq_len,
                ("with the device up the room is the model's own context: the device's largest " +
                "buffer holds far more rows"))
        }
        set_metal_decode_mirror_cap_mb(prev_cap)
    }
}

//! arm 6c: a step whose mirror demand passes the device's largest
//! buffer (the probe seam at 1 MiB) declines by name, once, and again
//! after the mirrors' reset
def private main_arm6c(t : T?; var m : Model; var kept : DecodeKept;
                       r : DecodeRun) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_probe = get_metal_mirror_device_max_probe_()
        set_metal_mirror_device_max_probe_(1ul << 20ul)
        t |> success(!metal_mirror_refusal_said(), "nothing refused yet")
        let bst0 = metal_batch_decode_stats()
        var gpu_two <- run_streams(m, r)
        let bst1 = metal_batch_decode_stats()
        t |> success(metal_mirror_refusal_said(), "the device-bound refusal was said")
        t |> success(bst1.declines - bst0.declines >= 1l, ("the batched step declined under a 1 " +
            "MiB " +
            "device bound ({bst1.declines - bst0.declines} declines)"))
        push_check(kept, "cpu-two",
            "the streams still match the CPU control off the declined step", r,
            gpu_two, "arm6c", 1)
        metal_decode_shutdown()
        metal_prefill_shutdown()
        t |> success(!metal_mirror_refusal_said(),
            "the mirrors' reset clears the refusal, so the next model says its own")
        set_metal_mirror_device_max_probe_(prev_probe)
        var gpu_again <- run_streams(m, r)
        let said = metal_mirror_refusal_said()
        let i = push_check(kept, "cpu-two", ("with the device's own bound back the step serves " +
            "and " +
            "nothing is refused"), r, gpu_again)
        kept.checks[i].rows = 1
        kept.checks[i].metal_bad = said
    }
}

//! arms 7 (q8_0 mirror: rope+quantize store, block-dequant attention)
//! and 7b (tq4 mirror: FWHT store, nibble attention, envelopes at
//! ~2x the q8_0 arm's)
def private main_codec_arms(t : T?; var m : Model; var kept : DecodeKept; prompt : array<int64>) {
    if (arm_on(t, "arm7-q8kv")) {
        served_decode_leg(t, m, kept, "cpu8", ("40-token q8_0-cache continuation matches the CPU " +
            "control"),
            run_of(DecodeRunKind.greedy, one_of(prompt), 40, MAIN_CTX, KVDtype.q8_0),
            "q8_0 arm served on the GPU")
        single_gate(t, m, kept, prompt, KVDtype.q8_0, 0.5, 0.5, "q8_0", BATCH_CTX)
    }
    if (arm_on(t, "arm7b-tq4kv")) {
        served_decode_leg(t, m, kept, "cpu4", ("40-token tq4-cache continuation matches the CPU " +
            "control"),
            run_of(DecodeRunKind.greedy, one_of(prompt), 40, MAIN_CTX, KVDtype.tq4),
            "tq4 arm served on the GPU")
        single_gate(t, m, kept, prompt, KVDtype.tq4, 1.5, 2.0, "tq4", BATCH_CTX)
    }
}

//! the 2030 tokens of the deep arms' prompt, its length asserted: a
//! shorter one makes every "past 2048" claim vacuous
def private deep_prompt(t : T?; m : Model; what : string) : array<int64> {
    var deep_p <- encode(m, counting_prompt(1, 700), true, false)
    t |> success(long_length(deep_p) >= 2030l, what)
    deep_p |> resize(2030)
    return <- deep_p
}

//! the fused single-pass attention's census over the deep steps:
//! every layer of 30 steps and more, the chunked pair none
def private fused_census(t : T?; m : Model; fused, chunked, what : string) {
    let n_layers = m.config.n_layers
    let nf = metal_kernel_count(fused)
    t |> success(nf >= 30l * n_layers && nf % n_layers == 0l, "{what} ({nf} dispatches)")
    t |> equal(metal_kernel_count(chunked), 0l, "and the chunked pair none")
}

//! arm 11 (1B part): ~2030 prompt tokens + 40 steps across depth
//! 2048, f16 and q8_0 mirrors, the fused attention under ATTN_D on,
//! the chunked pair under off (the control)
def private main_deep_1b(t : T?; var m : Model;
                         var kept : DecodeKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_ctx_cut(m, DEEP_CTX) {
            var inscope deep_p <- deep_prompt(t, m, "deep prompt reaches 2030 tokens")
            let r16 <- run_of(DecodeRunKind.greedy, one_of(deep_p), 40, DEEP_CTX)
            let r8 <- run_of(DecodeRunKind.greedy, one_of(deep_p), 40, DEEP_CTX, KVDtype.q8_0)
            with_metal_attn_d(true) {
                let before = metal_decode_stats().decodes
                kernel_census_reset_metal()
                metal_leg(m, kept, "deep16", ("40-token continuation crossing depth 2048 " +
                    "matches the CPU " +
                    "control"), r16)
                t |> success(metal_decode_stats().decodes - before > 30l,
                    "deep arm: decode steps past 2048 served on the GPU")
                fused_census(t, m, "MetalSqAttnDF16_metal_sq_attn_d",
                    "MetalSqAttnPartF16_metal_sq_attn_part",
                    ("the fused attention served every layer of the deep steps under " +
                    "DASLLAMA_METAL_ATTN_D pinned on"))
                kernel_census_reset_metal()
                metal_leg(m, kept, "deep8", ("40-token q8_0-KV continuation crossing depth 2048 " +
                    "matches " +
                    "the CPU control"), r8)
                fused_census(t, m, "MetalSqAttnDQ8_metal_sq_attn_d",
                    "MetalSqAttnPartQ8_metal_sq_attn_part",
                    ("the fused attention served every layer of the deep q8_0 steps under " +
                    "DASLLAMA_METAL_ATTN_D pinned on"))
                set_metal_attn_d(false)
                kernel_census_reset_metal()
                metal_leg(m, kept, "deep16", ("the chunked pair's 40-token continuation matches " +
                    "the CPU control too"),
                    r16)
                let part = metal_kernel_count("MetalSqAttnPartF16_metal_sq_attn_part")
                let fused = metal_kernel_count("MetalSqAttnDF16_metal_sq_attn_d")
                t |> success(part >= 30l * m.config.n_layers && fused == 0l,
                    ("DASLLAMA_METAL_ATTN_D off: the chunked pair serves every deep step and " +
                        "the fused " +
                    "form none ({part} chunked, {fused} fused)"))
            }
        }
    }
}

//! one arm15 chain's sampled generation: the chain on (adaptive) or
//! off, 24 tokens from seed 7 into `ids`; the chain hits it took
def private spec_chain_run(t : T?; m : Model; on : bool; prompt2 : array<int64>;
                           var ids : array<int64>) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let what = on ? "on" : "off"
        var sampled = SamplingParams(temp = 0.8, top_k = 40l, top_p = 0.95)
        set_metal_decode_speculate(on ? -1 : 0)
        var ss = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        set_seed_(ss, 7)
        ss.spec_params = unsafe(addr(sampled))
        let sp0 = metal_decode_spec_stats()
        let sel0 = sel_selections()
        let n_sampled = generate_(m, ss, prompt2, sampled, 24l) $(id : int64;
                                                                  _piece : string) : bool {
            ids |> push(id)
            return true
        }
        let sp1 = metal_decode_spec_stats()
        ss.spec_params = null
        t |> success(n_sampled == 24l, "arm15: the sampled generation ran 24 tokens (chain {what})")
        t |> success(sel_selections() > sel0, ("arm15: the sampled steps landed their selection " +
            "(chain {what})"))
        let hits = sp1.hits - sp0.hits
        if (on) {
            t |> success(hits > 0l && sp1.misses == sp0.misses, ("arm15: a decode sampled at " +
                "temp 0.8 / " +
                "top-k 40 rides the sampled chain ({hits} hits, {sp1.misses - sp0.misses} misses)"))
        } else {
            t |> success(sp1.hits == sp0.hits && sp1.misses == sp0.misses,
                "arm15: the chain pinned off chains nothing")
        }
        delete ss
        return hits
    } else {
        return 0l
    }
}

//! arm15's penalized and greedy decodes: the penalty leaves the chain
//! alone, the greedy decode rides it
def private spec_plain_runs(t : T?; m : Model; prompt2 : array<int64>;
                            hits_on : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_decode_speculate(-1)
        var sp = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        set_seed_(sp, 7)
        var penalized = SamplingParams(temp = 0.8, top_k = 40l, penalty = 1.1)
        sp.spec_params = unsafe(addr(penalized))
        let spp0 = metal_decode_spec_stats()
        generate_(m, sp, prompt2, penalized, 24l) $(_id : int64; _piece : string) : bool {
            return true
        }
        let spp1 = metal_decode_spec_stats()
        sp.spec_params = null
        t |> success(spp1.hits == spp0.hits && spp1.misses == spp0.misses, ("arm15: a penalized " +
            "sampled " +
            ("decode leaves the chain alone ({spp1.hits - spp0.hits} hits, " +
                "{spp1.misses - spp0.misses} ") +
            "misses)"))
        delete sp
        var sg = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        let greedy = SamplingParams()
        let sp2 = metal_decode_spec_stats()
        generate_(m, sg, prompt2, greedy, 24l) $(_id : int64; _piece : string) : bool {
            return true
        }
        let sp3 = metal_decode_spec_stats()
        t |> success(sp3.hits > sp2.hits, ("arm15: a greedy decode rides the chain " +
            "({sp3.hits - sp2.hits} " +
            "hits, {sp3.misses - sp2.misses} misses)"))
        to_log(LOG_INFO, ("arm15 eyeball: sampled chain {hits_on} hits, greedy " +
            "{sp3.hits - sp2.hits} hits " +
            "/ {sp3.misses - sp2.misses} misses\n"))
        delete sg
    }
}

//! arm15: the speculative hint - a sampled stream on the chain walks
//! the chain-off stream's tokens; a penalty keeps off it
def private spec_hint_cell(t : T?; var m : Model;
                           prompt2 : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_dasllama_jobque() {
            let prev_spec = metal_decode_spec_mode()
            with_metal_rails(MetalRails()) {
                var sampled_ids : array<array<int64>>
                sampled_ids |> resize(2)
                let hits_on = spec_chain_run(t, m, true, prompt2, sampled_ids[0])
                spec_chain_run(t, m, false, prompt2, sampled_ids[1])
                t |> equal(sampled_ids[0], sampled_ids[1],
                    "arm15: the sampled chain's stream is the chain-off stream token for token")
                to_log(LOG_INFO, "arm15 sampled chain: {decode(m, sampled_ids[0])}\n")
                spec_plain_runs(t, m, prompt2, hits_on)
                set_metal_decode_speculate(prev_spec)
                delete sampled_ids
            }
        }
    }
}

//! the main arms 1-7b on the 1B Q8's form, cut to 2048; arm 6c shuts
//! the decode driver down
def private main_arms_l1b(t : T?; var m : Model; var kept : DecodeKept) {
    with_ctx_cut(m, MAIN_CTX) {
        var inscope prompt <- encode(m, counting_prompt(1, 120), true, false)
        var inscope prompt2 <- encode(m, counting_prompt(7, 60), true, false)
        let cont <- run_of(DecodeRunKind.greedy, one_of(prompt), 40, MAIN_CTX)
        var none : array<array<int64>>
        let i = push_check(kept, "cpu16", "", cont, none)
        kept.checks[i].len_assert = true
        return if (arm_on(t, "arm1-basic") && !main_arm1(t, m, kept, cont))
        if (arm_on(t, "arm2-hybrid")) {
            main_arm2(t, m, kept, cont)
        }
        if (arm_on(t, "arm13-conc")) {
            main_arm13(t, m, kept, cont)
        }
        if (arm_on(t, "arm3-step")) {
            single_gate(t, m, kept, prompt, KVDtype.f16, 0.5, 0.25, "f16", BATCH_CTX)
            single_gate(t, m, kept, prompt, KVDtype.f32, 0.5, 0.25, "f32", BATCH_CTX)
        }
        main_paged_rewind(t, m, kept, prompt, prompt2)
        main_late_arms_l1b(t, m, kept, prompt, prompt2)
    }
}

//! the main arms 6-16 on the 1B Q8's form
def private main_late_arms_l1b(t : T?; var m : Model; var kept : DecodeKept;
                               prompt, prompt2 : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (arm_on(t, "arm6-churn")) {
            main_arm6(t, m, prompt2)
        }
        let two <- two_streams(prompt, prompt2)
        if (arm_on(t, "arm6b-capfit")) {
            main_arm6b(t, m, kept, two)
        }
        if (arm_on(t, "arm6c-capfit-refuse")) {
            main_arm6c(t, m, kept, two)
        }
        main_codec_arms(t, m, kept, prompt)
        if (arm_on(t, "arm12-dim")) {
            let unfused0 = get_metal_batch_addrms_unfused()
            set_metal_batch_addrms_unfused(true)
            batch_gate(t, m, kept, 3, KVDtype.f16, 0.5, 0.25, "B=3 unfused add+rms pair",
                BATCH_CTX, 12)
            set_metal_batch_addrms_unfused(unfused0)
        }
        if (arm_on(t, "arm15-spec-hint")) {
            spec_hint_cell(t, m, prompt2)
        }
    }
}

let private SEL_STEPS = 12
let private SEL_LANDED_MIN = 8   //! the steps the window's compares cover

//! the softmax denominator of `logits` at `temp` as the selection
//! kernel sums it: every lane against the row's max, floored at -80
def private host_sel_den(logits : array<float>; temp : float) : float {
    var mx = -3.4e38
    for (v in logits) {
        mx = max(mx, v)
    }
    var den = 0.0
    for (v in logits) {
        den += exp(max((v - mx) / temp, -80.0))
    }
    return den
}

//! the selection the arm16 step `step` landed against the host's: its
//! denominator pushed beside the host's at temp and at temp 1, every
//! recent id's raw logit asserted
def private sel_step_landed(t : T?; s : Session; step : int; sp : SamplingParams;
                            var landed, at_temp, at_one : array<float>) {
    let row = sel_row_of(s, 0l)
    let hden = uint_bits_to_float(unsafe(g_sel_landing.hdr[row * SEL_HDR + SEL_HDR_DEN]))
    let den_temp = host_sel_den(s.logits, sp.temp)
    let den_one = host_sel_den(s.logits, 1.0)
    landed |> push(hden)
    at_temp |> push(den_temp)
    at_one |> push(den_one)
    let nrec = length(g_sel_ask_ids)
    var rec_off = 0
    for (id, j in g_sel_ask_ids, count()) {
        if (unsafe(g_sel_landing.rec[row * g_sel_landing.recent_cap + int64(j)]) != s.logits[id]) {
            rec_off++
        }
    }
    to_log(LOG_INFO, ("arm16 step {step}: landed den {hden}, host den at temp {sp.temp} " +
        "{den_temp}, at " +
        "temp 1 {den_one}; {nrec} recent ids, {rec_off} landed raw logits off the row's\n"))
    t |> success(nrec > 0 && rec_off == 0, ("step {step}: the ask listed {nrec} recent ids and " +
        "the step " +
        "landed every one's raw logit ({rec_off} off)"))
}

//! arm16: consecutive sampled single-row steps land the denominator
//! at the stream's temperature 0.5 (top-k off, so the top-p cut reads
//! it), never the temp-1 one (the bar's control), on either landing
//! slot
def private sel_ask_cell(t : T?; var m : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_landing = get_select_landing()
        let prior_spec = metal_decode_spec_mode()
        set_select_landing(true)
        set_metal_decode_speculate(0)
        with_dasllama_jobque_() {
            var prompt <- encode(m, "Write a short poem about the sea.", true, false)
            var sp = SamplingParams(temp = 0.5, top_k = 0l, top_p = 0.9, presence_penalty = 1.5)
            var s <- create_session(m)
            set_seed(s, 7)
            eval_(m, s, prompt)
            s.spec_params = unsafe(addr(sp))
            var tok = sample(s, sp)
            s.recent |> push(tok)
            var landed, at_temp, at_one : array<float>
            var toks : array<int64>
            for (step in range(SEL_STEPS)) {
                forward(m, s, tok, s.n_past)
                s.n_past++
                if (!sel_landed(s, 0l)) {
                    to_log(LOG_INFO, ("arm16 step {step}: no selection landed (the driver's " +
                        "first steps land their rows)\n"))
                } else {
                    sel_step_landed(t, s, step, sp, landed, at_temp, at_one)
                }
                tok = sample(s, sp)
                toks |> push(tok)
                s.recent |> push(tok)
            }
            s.spec_params = null
            to_log(LOG_INFO, "arm16 sampled text: {eyeball_text(m, toks)}\n")
            t |> success(length(landed) >= SEL_LANDED_MIN, ("arm16: {length(landed)} of " +
                "{SEL_STEPS} steps " +
                "landed their selection (at least {SEL_LANDED_MIN})"))
            let bar = Bar(rel = 1e-3, poison_rel = 0.5)
            check_rows(t, "arm16: the landed denominators against the host's at temp 0.5", landed,
                at_temp, bar, FLAT_ROWS)
            check_exceeds(t, "arm16 control: the landed denominators against the host's at temp 1",
                landed, at_one, bar)
            delete toks
            delete at_one
            delete at_temp
            delete landed
            delete s
            delete prompt
        }
        set_metal_decode_speculate(prior_spec)
        set_select_landing(prior_landing)
    }
}

//! Llama-3.2-1B Q8's Metal legs on its blob twin (`metal_form(ctx =
//! DECODE_CTX_LLAMA_1B_Q8)`); arm 6c shuts the decode driver down
[cold_path]
def decode_parity_llama_1b_q8_metal(t : T?; var m : Model; form : ModelForm;
                                    var kept : DecodeKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "Llama-3.2-1B-Instruct-Q8_0.gguf",
                                    "decode")
                   || !family_on(t, "llama") || !jit_only(t))
        kept.wscale_f32 = form.wscale_f32
        if (arm_on(t, "batch")) {
            batch_arms_l1b(t, m, kept)
        }
        main_arms_l1b(t, m, kept)
        if (arm_on(t, "arm11-depth")) {   //! past the main arms' 2048 cut
            main_deep_1b(t, m, kept)
        }
        if (arm_on(t, "arm16-sel-ask")) {
            with_ctx_cut(m, BATCH_CTX) {
                sel_ask_cell(t, m)
            }
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! the B8 kq legs before the half-x crowns: B=3, B=2 q8_0-KV, B=12
//! (the kq mul_mm form) and B=9 (the first past the small forms' 8)
def private kq_batch_legs(t : T?; var m : Model; var kept : DecodeKept;
                          promptsk, promptsk2 : array<array<int64>>) {
    served_batch_leg(t, m, kept, "kq-3", "B=3 kq batched greedy streams match the CPU control",
        run_of(DecodeRunKind.batch, promptsk, 16, BATCH_CTX), "kq batch arm served on the GPU")
    metal_leg(m, kept, "kq-2", "B=2 kq q8_0-KV batched greedy streams match the CPU control",
        run_of(DecodeRunKind.batch, promptsk2, 14, BATCH_CTX, KVDtype.q8_0))
    var inscope p12 <- counting_rows(m, 12, 1, 34, 2)
    served_batch_leg(t, m, kept, "kq-12", ("B=12 kq batched greedy streams match the CPU " +
        "control (kq " +
        "mul_mm)"), run_of(DecodeRunKind.batch, p12, 10, BATCH_CTX), ("B=12 kq arm served on the " +
            "GPU"))
    var inscope p9 <- counting_rows(m, 9, 2, 36, 2)
    let before = metal_batch_step_count()
    metal_leg(m, kept, "kq-9", "B=9 kq batched greedy streams match the CPU control",
        run_of(DecodeRunKind.batch, p9, 10, BATCH_CTX))
    t |> success(metal_batch_step_count() - before >= 10l,
        "B=9 kq arm served on the GPU, every step batched")
}

//! the half-x crowns' k4 wide forms and k6 small-batch half stamps
def private k6_half_stamps() : int64 => (metal_kernel_count("MetalKqMvB2K6H_metal_kq_mv_k6")
    + metal_kernel_count("MetalKqMvB4K6H_metal_kq_mv_k6")
    + metal_kernel_count("MetalKqMvB8K6H_metal_kq_mvb8_k6"))

def private k6_f32_forms() : int64 => (metal_kernel_count("MetalKqMvB2K6_metal_kq_mv_k6")
    + metal_kernel_count("MetalKqMvB4K6_metal_kq_mv_k6")
    + metal_kernel_count("MetalKqMvB8K6_metal_kq_mvb8_k6"))

//! the half-x legs under the crowns: B=2 / B=3 / B=8 on k4's two-,
//! four- and eight-column wide forms and k6's half stamps; the k4
//! census and the k6 half count returned through `k6h`; each leg's
//! steps run with the prefill driver down, its rows8 form off them
def private half_x_on(t : T?; var m : Model; var kept : DecodeKept; promptsk, promptsk2,
                      promptsk8 : array<array<int64>>) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let r8 <- run_of(DecodeRunKind.batch, promptsk8, 12, BATCH_CTX, KVDtype.f16, true)
        kernel_census_reset_metal()
        let steps_h0 = metal_batch_decode_stats().steps
        metal_leg(m, kept, "kq-2", ("B=2 kq batched greedy streams on the half-x two-column " +
            "form match the " +
            "CPU control"), run_of(DecodeRunKind.batch, promptsk2, 14, BATCH_CTX, KVDtype.q8_0, true),
            "half-x B=2", 1)
        metal_leg(m, kept, "kq-3", ("B=3 kq batched greedy streams on the half-x four-column " +
            "form match the " +
            "CPU control"), run_of(DecodeRunKind.batch, promptsk, 16, BATCH_CTX, KVDtype.f16, true),
            "half-x B=3", 1)
        metal_leg(m, kept, "kq-8", ("B=8 kq batched greedy streams on the half-x eight-column " +
            "form match " +
            "the CPU control"), r8, "half-x B=8", 8)
        let steps = metal_batch_decode_stats().steps - steps_h0
        t |> success(steps >= 3l * 12l, ("the three half-x legs' steps were the batched driver's " +
            "({steps} steps)"))
        let h2 = metal_kernel_coverage_of("MetalKqMvW2K4H_metal_kq_mvw_k4")
        let h4 = metal_kernel_coverage_of("MetalKqMvW4K4H_metal_kq_mvw_k4")
        let h8 = metal_kernel_coverage_of("MetalKqMvW8K4H_metal_kq_mvw_k4")
        let k6h = k6_half_stamps()
        t |> success(h2 > 0l && h4 > 0l && h8 > 0l, ("the half-x forms served the k4 sites " +
            "under the crown " +
            "(two-column {h2}, four-column {h4}, eight-column {h8} dispatches)"))
        to_log(LOG_INFO, ("half-x leg: k4 wide forms {h2} / {h4} / {h8} dispatches, k6 half " +
            "stamps {k6h}\n"))
        return k6h
    } else {
        return 0l
    }
}

//! the rail-off control: the f32 forms, the half counts at zero, and
//! k6's f32 count saying whether the k6 half count had to move
def private half_x_off(t : T?; var m : Model; var kept : DecodeKept;
                       promptsk8 : array<array<int64>>;
                       k6h : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_kq_half(false)
        kernel_census_reset_metal()
        let steps_f0 = metal_batch_decode_stats().steps
        metal_leg(m, kept, "kq-8", ("B=8 kq batched greedy streams on the f32 forms match the " +
            "CPU control"),
            run_of(DecodeRunKind.batch, promptsk8, 12, BATCH_CTX, KVDtype.f16, true), "rail-off B=8",
            1)
        let steps = metal_batch_decode_stats().steps - steps_f0
        t |> success(steps >= 12l, ("the rail-off leg's steps were the batched driver's ({steps} " +
            "steps)"))
        let k6f = k6_f32_forms()
        to_log(LOG_INFO, "half-x leg: k6 f32 forms with the rail off {k6f}\n")
        t |> equal(metal_kernel_coverage_of("MetalKqMvW8K4H_metal_kq_mvw_k4"), 0l,
            "the rail off keeps the f32 forms: no eight-column half dispatch")
        t |> success((k6f > 0l) == (k6h > 0l), ("the k6 half stamps served exactly where the " +
            "rail-off run " +
            "served k6's f32 small-batch forms (half {k6h}, f32 {k6f} dispatches)"))
    }
}

//! the half-x leg: the crowns pinned and the drivers shut down so the
//! sites recompile under them, then the crowns put back and the
//! drivers shut down again
def private half_x_leg(t : T?; var m : Model; var kept : DecodeKept; promptsk,
                       promptsk2 : array<array<int64>>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_crowns = get_metal_tensor_crowns()
        let prev_pinned = metal_tensor_crowns_pinned()
        let prev_half = get_metal_kq_half()
        set_metal_kq_half(true)
        pin_metal_tensor_crowns("{kq_half_crown(KqFmt.k4)},{kq_half_crown(KqFmt.k6)}")
        metal_decode_shutdown()
        metal_prefill_shutdown()
        var inscope promptsk8 <- counting_rows(m, 8, 2, 30, 3)
        t |> equal(2, length(promptsk2), "the B=2 leg runs two streams")
        t |> equal(3, length(promptsk), "the B=3 leg runs three streams")
        t |> equal(8, length(promptsk8), "the B=8 leg runs eight streams")
        let k6h = half_x_on(t, m, kept, promptsk, promptsk2, promptsk8)
        half_x_off(t, m, kept, promptsk8, k6h)
        set_metal_kq_half(prev_half)
        if (prev_pinned) {
            pin_metal_tensor_crowns(prev_crowns)
        } else {
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
        }
        metal_decode_shutdown()
        metal_prefill_shutdown()
    }
}

//! arm 10: the K-quant 40-token continuation and its one-step gates;
//! the envelope is wider than q8's (the CPU kq dots run
//! Q8_K-quantized activations, the GPU kernels f32 x)
def private kq_main_arm(t : T?; var m : Model; var kept : DecodeKept) {
    with_ctx_cut(m, MAIN_CTX) {
        var inscope prompt <- encode(m, counting_prompt(1, 120), true, false)
        served_decode_leg(t, m, kept, "kq-cont", ("40-token K-quant continuation matches the CPU " +
            "control"),
            run_of(DecodeRunKind.greedy, one_of(prompt), 40, MAIN_CTX), "kq arm served on the GPU")
        single_gate(t, m, kept, prompt, KVDtype.f16, 0.8, 0.5, "kq/f16", BATCH_CTX)
        single_gate(t, m, kept, prompt, KVDtype.q8_0, 0.8, 0.5, "kq/q8_0", BATCH_CTX)
    }
}

//! Llama-3.2-1B Q4_K_M's Metal legs on its blob twin (`metal_form(ctx
//! = DECODE_CTX_LLAMA_1B_Q4KM)`); the half-x leg shuts the decode
//! driver down twice
[cold_path]
def decode_parity_llama_1b_q4km_metal(t : T?; var m : Model; form : ModelForm;
                                      var kept : DecodeKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "Llama-3.2-1B-Instruct-Q4_K_M.gguf",
                                    "decode")
                   || !family_on(t, "llama") || !jit_only(t))
        kept.wscale_f32 = form.wscale_f32
        if (arm_on(t, "batch") && arm_on(t, "batchB8-kq")) {
            with_ctx_cut(m, BATCH_CTX) {
                var inscope promptsk <- counting_set(m, [int2(1, 52), int2(6, 41), int2(3, 64)])
                var inscope promptsk2 <- [clone_to_move(promptsk[0]), clone_to_move(promptsk[1])]
                kq_batch_legs(t, m, kept, promptsk, promptsk2)
                half_x_leg(t, m, kept, promptsk, promptsk2)
            }
        }
        if (arm_on(t, "arm10-kq")) {
            kq_main_arm(t, m, kept)
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! Llama-3.2-1B Q5_K_M's Metal legs on its blob twin (`metal_form(ctx
//! = DECODE_CTX_LLAMA_1B_Q5KM)`): B <= 8 on the kq ext mv twins
[cold_path]
def decode_parity_llama_1b_q5km_metal(t : T?; var m : Model; form : ModelForm;
                                      var kept : DecodeKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "Llama-3.2-1B-Instruct-Q5_K_M.gguf",
                                    "decode")
                   || !family_on(t, "llama") || !jit_only(t) || !arm_on(t, "batch")
                   || !arm_on(t, "batchB8-kq"))
        kept.wscale_f32 = form.wscale_f32
        with_ctx_cut(m, BATCH_CTX) {
            var inscope prompts5 <- counting_set(m, [int2(2, 47), int2(4, 58), int2(7, 36)])
            served_batch_leg(t, m, kept, "k5-3", ("B=3 k5 batched greedy streams match the CPU " +
                "control (kq " +
                "ext mv twins)"), run_of(DecodeRunKind.batch, prompts5, 16, BATCH_CTX),
                "k5 batch arm served on the GPU")
            var inscope prompts52 <- [clone_to_move(prompts5[0]), clone_to_move(prompts5[1])]
            metal_leg(m, kept, "k5-2", ("B=2 k5 batched greedy streams match the CPU control " +
                "(ext2 form)"),
                run_of(DecodeRunKind.batch, prompts52, 14, BATCH_CTX))
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! arm B7: hs=128, the fused single-pass partD attention (the 1B's
//! hs=64 heads keep the chunked pair)
def private partd_batch_arm(t : T?; var m : Model; var kept : DecodeKept) {
    with_ctx_cut(m, BATCH_CTX) {
        var inscope promptsd <- counting_set(m, [int2(1, 48), int2(5, 62), int2(3, 39)])
        served_batch_leg(t, m, kept, "partd-3", ("B=3 hs=128 batched greedy streams match the " +
            "CPU control " +
            "(partD attention)"), run_of(DecodeRunKind.batch, promptsd, 16, BATCH_CTX),
            "hs=128 partD arm served on the GPU")
        batch_gate(t, m, kept, 3, KVDtype.f16, 0.5, 0.25, "B=3 hs128 f16", BATCH_CTX)
        batch_gate(t, m, kept, 3, KVDtype.f32, 0.5, 0.25, "B=3 hs128 f32", BATCH_CTX)
        batch_gate(t, m, kept, 3, KVDtype.q8_0, 0.5, 0.25, "B=3 hs128 q8_0", BATCH_CTX)
        batch_gate(t, m, kept, 3, KVDtype.tq4, 1.0, 2.0, "B=3 hs128 tq4", BATCH_CTX)
    }
}

//! arm 11 (3B part): partD's 16-simdgroup slice loop past depth 2048,
//! the context's mirror (~470 MB) growing the fresh 256 MB arena
def private partd_deep_arm(t : T?; var m : Model; var kept : DecodeKept) {
    with_ctx_cut(m, DEEP3_CTX) {
        var inscope deep_p3 <- deep_prompt(t, m, "deep partD prompt reaches 2030 tokens")
        let before = metal_decode_count()
        metal_leg(m, kept, "deep3", ("40-token hs=128 continuation crossing depth 2048 matches " +
            "the CPU " +
            "control (partD)"), run_of(DecodeRunKind.greedy, one_of(deep_p3), 40, DEEP3_CTX))
        t |> success(metal_decode_count() - before > 30l, "deep partD arm served on the GPU")
    }
}

//! Llama-3.2-3B Q8's Metal legs on its blob twin (`metal_form(ctx =
//! DECODE_CTX_LLAMA_3B_Q8)`)
[cold_path]
def decode_parity_llama_3b_q8_metal(t : T?; var m : Model; form : ModelForm;
                                    var kept : DecodeKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "Llama-3.2-3B-Instruct-Q8_0.gguf",
                                    "decode")
                   || !family_on(t, "llama") || !jit_only(t))
        kept.wscale_f32 = form.wscale_f32
        if (arm_on(t, "batch") && arm_on(t, "batchB7-partd")) {
            partd_batch_arm(t, m, kept)
        }
        if (arm_on(t, "arm11-depth")) {
            partd_deep_arm(t, m, kept)
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! Meta-Llama-3.1-8B Q8's Metal leg on its blob twin (`metal_form(ctx
//! = DECODE_CTX_LLAMA31_8B_Q8)`): dim 4096 past B=4 on the q8 GEMM
//! tier
[cold_path]
def decode_parity_llama31_8b_q8_metal(t : T?; var m : Model; form : ModelForm;
                                      var kept : DecodeKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf",
                                    "decode")
                   || !family_on(t, "llama") || !jit_only(t) || !arm_on(t, "arm12-dim"))
        kept.wscale_f32 = form.wscale_f32
        batch_gate(t, m, kept, 8, KVDtype.f16, 0.5, 0.25, "B=8 dim4096 f16", BATCH_CTX, 12)
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! one GPU-vs-GPU cell's batched run: `nb` counting prompts, twelve
//! steps; its one-row twin sets `single_every = 1`
def private rows_run(m : Model; nb : int) : DecodeRun {
    var inscope prompts <- counting_rows(m, nb, 1, 30, 3)
    return <- run_of(DecodeRunKind.batch, prompts, 12, BATCH_CTX)
}

//! the mixed twin's cells: a recurrent layer's Q8_0 out plane beside
//! K-quant qkv / gate planes, every batched site dispatching by its
//! own tag, at 3, 9 and 12 rows
def private mixed_cells(t : T?; var m : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var mixed = false
        for (l in range64(m.config.n_layers)) {
            if (layer_is_recurrent(m.config, l) && fmt_at(m.dnout_fmt, l) == KqFmt.q8
                    && fmt_at(m.dnqkv_fmt, l) != KqFmt.q8) {
                mixed = true
            }
        }
        t |> success(mixed, "a recurrent layer reads a Q8_0 out plane beside a K-quant qkv plane")
        for (nb in [3, 9, 12]) {
            var inscope rb <- rows_run(m, nb)
            var inscope rs := rb
            rs.single_every = 1
            let b0 = metal_batch_decode_stats()
            var inscope batched <- run_streams(m, rb)
            let b1 = metal_batch_decode_stats()
            var inscope single <- run_streams(m, rs)
            t |> equal(b1.steps - b0.steps, 12l, ("B={nb}: every step was the batched driver's " +
                "({b1.steps - b0.steps} of 12)"))
            t |> equal(b1.declines, b0.declines, "B={nb}: the batch declined no step")
            t |> success(streams_match(single, batched), ("B={nb}: the batched streams are the " +
                "single-row step's token for token"))
            log_gen_texts(m, "mixed B={nb} stream 0", single[0], batched[0])
        }
    }
}

//! Qwen3.5-0.8B Q4_K_M with a Q8_0 deltanet out plane on its blob
//! twin (`metal_form(ctx = DECODE_CTX_QWEN35_08B_Q8OUT)`): the
//! batched driver at 3, 9 and 12 rows, token for token the one-row
//! step's
[cold_path]
def decode_mixed_sites_qwen35_08b_metal(t : T?; var m : Model;
                                        form : ModelForm) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf",
                                    "decode")
                   || !family_on(t, "qwen35") || !jit_only(t) || !arm_on(t, "batch-mixed"))
        with_ctx_cut(m, BATCH_CTX) {
            mixed_cells(t, m)
        }
    } else {
        t |> skip("dasMetal not built")
    }
}

//! one per-32 carrier: a GGUF under the models root (`file`) or a
//! stocked safetensors snapshot (`repo`, the affine members)
struct DecodePer32Row {
    file : string
    repo : string
    fam : string
}

def decode_per32_q5_1() : DecodePer32Row => DecodePer32Row(
    file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf", fam = "llama")

def decode_per32_iq4_nl() : DecodePer32Row => DecodePer32Row(
    file = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf", fam = "qwen2")

def decode_per32_mlx_1b_8bit() : DecodePer32Row => DecodePer32Row(
    repo = "mlx-community/Llama-3.2-1B-Instruct-8bit", fam = "llama")

def decode_per32_mlx_3b_4bit() : DecodePer32Row => DecodePer32Row(
    repo = "mlx-community/Llama-3.2-3B-Instruct-4bit", fam = "llama")

//! a k/v plane narrow at `nb` rows (`b32_narrow`): the K-split
//! stamps' carrier; false off das_metal
def private kv_narrow(m : Model; nb : int) : bool {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let kv_rows = m.config.n_kv_heads * m.config.head_size
        return b32_narrow(kv_rows, m.config.dim, int64(nb))
    } else {
        return false
    }
}

//! a dense model's per-32 planes the matrix-unit rows8 form takes at
//! `nb` rows (`metal_rows8_aff_on`: the MLX affine formats, the
//! prefill driver up): none below its row floor or past eight rows
def private per32_rows8(m : Model; nb : int) : int64 {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return 0l if (!rows8_rows(int64(nb)))
        return sites_count(m, true, false,
            $(f : KqFmt) : bool => kq_dense_b32(f) && metal_rows8_aff_on(f))
    } else {
        return 0l
    }
}

//! the rows8 form's affine dispatches so far
def private rows8_aff_count() : int64 {
    return (metal_kernel_count("MetalRows8Mlx4_metal_rows8")
        + metal_kernel_count("MetalRows8Mlx8_metal_rows8")
        + metal_kernel_count("MetalRows8Mlx4b_metal_rows8"))
}

//! the small-batch counters' asserts of one per-32 row count; `r8` the
//! rows8 form's affine dispatches over the same steps
def private per32_form_asserts(t : T?; m : Model; nb : int; np32, disp, rows, ks, r8 : int64) {
    t |> equal(rows, disp * int64(nb), ("B={nb}: every small-batch dispatch carried all {nb} " +
        "rows ({disp} dispatches, {rows} rows)"))
    let on8 = per32_rows8(m, nb)
    if (on8 > 0l) {
        t |> success(r8 > 0l && disp + r8 >= 12l * np32, ("B={nb}: the affine per-32 sites " +
            "rode the matrix-unit rows8 form, the rest the small-batch form ({r8} and {disp} " +
            "dispatches, {12l * np32} planes x steps)"))
    } elif (nb <= 8) {
        t |> success(disp >= 12l * np32, ("B={nb}: the per-32 sites rode the small-batch form " +
            "({disp} dispatches, {12l * np32} planes x steps)"))
        if (kv_narrow(m, nb)) {
            t |> success(ks > 0l, ("B={nb}: the narrow per-32 planes rode the K-split stamps " +
                "({ks} dispatches)"))
        }
    } else {
        t |> equal(disp, 0l, "B={nb}: past eight rows the per-32 sites ride the mul_mm tile")
    }
}

//! one per-32 row count: the batched streams, every step the batched
//! driver's, against the one-row step's
def private per32_cell(t : T?; var m : Model; nb : int; np32 : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope rb <- rows_run(m, nb)
        var inscope rs := rb
        rs.single_every = 1
        let b0 = metal_batch_decode_stats()
        let m0 = metal_b32_mvb_stats()
        let ks0 = metal_b32_ks_count()
        let r80 = rows8_aff_count()
        var inscope batched <- run_streams(m, rb)
        let b1 = metal_batch_decode_stats()
        let m1 = metal_b32_mvb_stats()
        let ks = metal_b32_ks_count() - ks0
        let r8 = rows8_aff_count() - r80
        var inscope single <- run_streams(m, rs)
        t |> equal(b1.steps - b0.steps, 12l, ("B={nb}: every step was the batched driver's " +
            "({b1.steps - b0.steps} of 12)"))
        t |> equal(b1.declines, b0.declines, "B={nb}: the batch declined no step")
        per32_form_asserts(t, m, nb, np32, m1.dispatches - m0.dispatches, m1.rows - m0.rows, ks,
            r8)
        t |> success(streams_match(single, batched), ("B={nb}: the batched streams are the " +
            "single-row step's token for token"))
        log_gen_texts(m, "per-32 B={nb} stream 0", single[0], batched[0])
    }
}

//! a per-32 carrier's Metal cells on its blob twin (`metal_form(ctx =
//! DECODE_CTX_PER32)`): two to eight rows on the small-batch form,
//! nine and twelve on the mul_mm tile, token for token the one-row
//! step
[cold_path]
def decode_per32_metal(t : T?; var m : Model; form : ModelForm;
                       row : DecodePer32Row) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.any, "{row.file}{row.repo}",
                                    "decode")
                   || !family_on(t, row.fam) || !jit_only(t) || !arm_on(t, "batch-per32"))
        with_ctx_cut(m, BATCH_CTX) {
            let np32 = per32_plane_count(m, false)
            t |> success(np32 > 0l, ("{row.file}: the dense planes load on their per-32 planes " +
                "({np32} planes)"))
            for (nb in [2, 3, 5, 8, 9, 12]) {
                per32_cell(t, m, nb, np32)
            }
        }
    } else {
        t |> skip("dasMetal not built")
    }
}

//! one check's CPU control: a control an earlier check of the same
//! key ran is copied, unless the check asserts on the control's own
//! run
def private check_cpu(t : T?; var m : Model; var kept : DecodeKept; i : int;
                      var done : table<string; int>) {
    let key = kept.checks[i].key
    let own_run = kept.checks[i].len_assert || kept.checks[i].counter_ctl
    if (!own_run && key_exists(done, key)) {
        let j = done?[key] ?? 0
        kept.checks[i].cpu := kept.checks[j].cpu
    } else {
        let before = metal_batch_step_count()
        var cpu <- run_streams(m, kept.checks[i].control)
        if (kept.checks[i].counter_ctl) {
            t |> equal(metal_batch_step_count(), before, ("the CPU control moves no batched GPU " +
                "step (the counter's control)"))
        }
        if (kept.checks[i].len_assert) {
            t |> equal(length(cpu[0]), kept.checks[i].control.n)
        }
        kept.checks[i].cpu <- cpu
        done[key] = i
    }
    kept.checks[i].cpu_ran = true
    for (r in range(kept.checks[i].log_rows)) {
        let tag = kept.checks[i].log_tag
        log_gen_texts(m, "{tag} row {r}", kept.checks[i].cpu[r], kept.checks[i].gpu[r])
    }
}

//! every check's CPU control and every gate's CPU replay the Metal
//! form asked for, on the pass of `decode_cpu_form(kept.wscale_f32)`
//! alone, each at the context the Metal leg ran at; other passes pass
//! by
[cold_path]
def decode_parity_cpu(t : T?; var m : Model; form : ModelForm; var kept : DecodeKept) {
    return if (!alone_pass_of(form, decode_cpu_form(kept.wscale_f32)))
    var done : table<string; int>
    for (i in range(length(kept.checks))) {
        check_cpu(t, m, kept, i, done)
    }
    delete done
    for (g in kept.gates) {
        continue if (g.gpu.served == 0l)
        gate_replay(t, m, g)
    }
}

def private check_verdict(t : T?; c : DecodeCheck) {
    return if (empty(c.what))
    if (!c.cpu_ran) {
        t |> failure("{c.what}: the CPU control never ran (no `decode_cpu_form` pass)")
        return
    }
    let same = c.rows == 0 ? streams_match(c.cpu, c.gpu) : rows_match(c.cpu, c.gpu, c.rows)
    t |> success(!c.metal_bad && same, c.what)
}

def private gate_verdict(t : T?; g : DecodeGate) {
    if (g.gpu.served == 0l) {
        if (g.batched) {
            feint("{g.tag}: metal batch decode declined; batch step gate skipped\n")
        } else {
            feint("{g.tag}: metal decode declined; single-step gate skipped\n")
        }
        return
    }
    if (!g.cpu_ran) {
        t |> failure(("{g.tag}: the CPU replay of the one-step gate never ran (no " +
            "`decode_cpu_form` pass)"))
        return
    }
    var maxl = 0.0
    var maxkv = 0.0
    for (i in range(length(g.gpu.logits))) {
        for (v in range64(g.vocab)) {
            maxl = max(maxl, nan_wide_diff(g.cpu.logits[i][v], g.gpu.logits[i][v]))
        }
        maxkv = max(maxkv, kv_rows_max_diff(g.cpu.kv[i], g.gpu.kv[i], g.kv_dims, g.kdt))
    }
    if (g.batched) {
        t |> success(maxl <= g.tol_logits, ("{g.tag}: one-batch-step logits within " +
            "{g.tol_logits} (max diff {maxl})"))
        t |> success(maxkv <= g.tol_kv, ("{g.tag}: stored KV rows within {g.tol_kv} (max diff " +
            "{maxkv})"))
        to_log(LOG_INFO, ("metal batch decode {g.tag}: logits maxd={maxl} (tol {g.tol_logits}), " +
            "kv rows maxd={maxkv} (tol {g.tol_kv})\n"))
    } else {
        t |> success(maxl <= g.tol_logits, ("{g.tag}: one-step logits within {g.tol_logits} (max " +
            "diff {maxl})"))
        t |> success(maxkv <= g.tol_kv, ("{g.tag}: stored KV row within {g.tol_kv} (max diff " +
            "{maxkv})"))
        to_log(LOG_INFO, ("metal decode {g.tag} single-step: logits maxd={maxl} (tol " +
            "{g.tol_logits}), kv row maxd={maxkv} (tol {g.tol_kv})\n"))
    }
}

//! the token compares and one-step gates the Metal and CPU forms
//! kept, after the block call; what `kept` holds deleted
[cold_path]
def decode_parity_verdict(t : T?; var kept : DecodeKept) {
    for (c in kept.checks) {
        check_verdict(t, c)
    }
    for (g in kept.gates) {
        gate_verdict(t, g)
    }
    delete kept
}

//! prefill on `trp` (a planar load), then `n` greedy steps on `trd`
//! under the decode override `rail`
def private split_continuation(trp, trd : Model; prompt : array<int64>; n : int;
                               rail : string) : array<int64> {
    var got : array<int64>
    with_metal_rails(MetalRails(decode = false, rail = "", min_npos = 0l)) {
        select_decode_override(rail)
        with_dasllama_jobque() {
            var s = make_run_state(trp.config, KVDtype.f16, KVDtype.f16)
            eval_(trp, s, prompt)
            got <- greedy_eval_steps(trd, s, n) // nolint:PERF030 - fresh; queue runs once
            delete s
        }
    }
    return <- got
}

//! arm 9's context: its pairs' sessions (`decode_reload_warmed`,
//! `decode_reload_within`)
let DECODE_RELOAD_CTX = MAIN_CTX

//! arm 9's first pair, the planar load `tr` and its blob twin `trg`
//! (`DECODE_RELOAD_CTX`) the caller loaded: a 40-token continuation
//! through the Metal decode; false where the decode declined it (no
//! Metal device), the reload pair then moot
def decode_reload_warmed(tr, trg : Model) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompt <- encode(tr, counting_prompt(1, 120), true, false)
        let before = metal_decode_stats().decodes
        var inscope warm <- split_continuation(tr, trg, prompt, 40, "metal")
        if (metal_decode_stats().decodes - before == 0l) {
            feint("metal decode declined (no Metal device?); GPU parity skipped\n")
            return false
        }
        return true
    } else {
        return false
    }
}

//! arm 9's reload pair `trr` / `trrg`, loaded beside the first pair,
//! live and served - the weight caches key by host address, and the
//! load epoch's flush keeps a stale hit off the dead model's buffers:
//! the 40-token continuation through the Metal decode token for token
//! the CPU control's
def decode_reload_within(t : T?; trr, trrg : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompt <- encode(trr, counting_prompt(1, 120), true, false)
        var inscope cpu_r <- split_continuation(trr, trr, prompt, 40, "")
        let before_r = metal_decode_stats().decodes
        var inscope gpu_r <- split_continuation(trr, trrg, prompt, 40, "metal")
        t |> success(metal_decode_stats().decodes - before_r > 0l, "reload arm served on the GPU")
        t |> success(ids_equal(cpu_r, gpu_r), ("40-token post-reload continuation matches the " +
            "CPU control (weights-epoch guard)"))
    }
}

//! the poison gate's forced feed: both states prefilled on the planar
//! model, the CPU side's greedy continuation forced through both
//! decoders, the full logits compared every step
def private forced_maxd_pair(tr, trg : Model; prompt : array<int64>; nsteps : int) : float {
    var maxd = 0.0
    with_metal_rails(MetalRails(rail = "", min_npos = 0l)) {
        with_dasllama_jobque() {
            var sa = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sa, prompt)
            eval_(tr, sb, prompt)
            select_decode_override("metal")
            let vocab = tr.config.vocab_size
            for (_i in range(nsteps)) {
                let tok = argmax_of(sa.logits, vocab)
                maxd = max(maxd, logits_maxdiff(sa.logits, sb.logits))
                var inscope one <- [tok]
                eval_(tr, sa, one)
                eval_(trg, sb, one)
                delete one
            }
            maxd = max(maxd, logits_maxdiff(sa.logits, sb.logits))
            delete sa
            delete sb
        }
    }
    return maxd
}

//! one Metal prefill on a throwaway session - the poison: prefill's
//! PLAIN per-tensor region uploads claim the entries decode looks up
//! as cat3 CONCATS
def private foreign_gpu_prefill(tr, trg : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_dasllama_jobque() {
            var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let p <- encode(tr, counting_prompt(3, 44), true, false)
            with_metal_rails(MetalRails(decode = false)) {
                eval_(trg, s, p)
            }
            delete s
        }
    }
}

//! arm 14's context: its pairs' sessions (`decode_poison_clean`,
//! `decode_poison_within`)
let DECODE_POISON_CTX = BATCH_CTX

//! arm 14's clean control on a fresh pair, the planar load `tr` and
//! its blob twin `trg` (`DECODE_POISON_CTX`) the caller loaded: the
//! forced-feed Metal decode within 4.0 of the CPU logits; `served`
//! false where the decode declined (no Metal device), the poisoned
//! pair then moot
def decode_poison_clean(t : T?; tr, trg : Model) : tuple<served : bool; maxd : float> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompt <- encode(tr, counting_prompt(1, 30), true, false)
        let de0 = metal_decode_stats().decodes
        let d_clean = forced_maxd_pair(tr, trg, prompt, 8)
        if (metal_decode_stats().decodes - de0 == 0l) {
            feint("metal decode declined (no Metal device?); poison gate skipped\n")
            return (served = false, maxd = d_clean)
        }
        t |> success(d_clean < 4.0, ("clean control: forced-feed GPU decode within 4 of CPU " +
            "(measured {d_clean})"))
        return (served = true, maxd = d_clean)
    } else {
        return (served = false, maxd = 0.0)
    }
}

//! arm 14 on a second fresh pair `tr2` / `trg2` (the weights-epoch
//! flush isolates fresh loads) after one foreign Metal prefill: the
//! forced-feed Metal decode still within 4.0 of the CPU logits;
//! `d_clean` the clean control's measure, logged beside it
def decode_poison_within(t : T?; tr2, trg2 : Model; d_clean : float) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompt <- encode(tr2, counting_prompt(1, 30), true, false)
        foreign_gpu_prefill(tr2, trg2)
        let d_poison = forced_maxd_pair(tr2, trg2, prompt, 8)
        to_log(LOG_INFO, "arm14-poison eyeball: clean maxd {d_clean} vs poisoned {d_poison}\n")
        t |> success(d_poison < 4.0, ("after a foreign GPU prefill: forced-feed GPU decode still " +
            "within 4 of CPU (measured {d_poison})"))
    }
}
