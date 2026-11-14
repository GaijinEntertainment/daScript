options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_common   // set_metal_mode, allow_cpu_prefill, forward_prefill, mtp_spec_round, the block drafter's seams
require dasllama/dasllama_math   // parallel_argmax
require dasllama/dasllama_dflash   // attach_dflash_drafter, detach_dflash_drafter, set_dflash_draft_temp
require dasllama/dasllama_sampling   // set_mtp_draft_temp
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - set_metal_kq_fat, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the drafter's device pass registers at init
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - set_metal_decode_speculate, metal_decode_stats, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - set_metal_decode_concurrent, set_metal_decode_fuse, inside the das_metal half
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, inside the das_metal half
require _parity_dump   // ParityDump: an arm's rows to a file and back
require _prompt_files   // read_ids
require _forced_drafter   // the forced drafter in the real one's place
require ../tests/_compares.das   // logits_maxdiff, logits_of, top2_margin
require ../tests/_model_tier.das   // live_maxabs: a row's scale without the suppressed ids
require daslib/clargs
require daslib/fio
require math
require strings
require daslib/strings_boost

// The one-row decode step against the eight-row verify, teacher-forced over one sequence: the prompt, then the DFlash2
// greedy round's own continuation (or --ids). The one-row arm feeds the continuation a step at a time; the rows arm runs the
// trunk's speculative round with a block drafter whose drafts ARE the continuation, so every verify's eight rows are forced
// tokens over a committed forced context, and a row's logits are taken from the first verify that covers its position.
// Both arms' row j predicts the continuation's token j (row 0 is the prefill's last row). Per position: top-1 of each, the
// largest difference against the reference row's live max, and each arm's margin of its own top-1 over the other's; the
// first position whose top-1 parts, a summary a 32 rows, and a knob ladder of one-row walks over the first
// --ladder-positions rows (a rerun, every step encoded on its own token with no step encoded ahead, the fat chain's modes,
// the serial encoder, the split seams, the greedy chain) against the rows.
// --cpu runs the arms on the CPU route; --save writes each arm's rows as a parity dump and the ids, --ref compares every
// arm run against such dumps (an env knob such as DASLLAMA_METAL_HC_MIX8 is a process of its own).
//
//   bin/daslang -jit modules/dasLLAMA/harness/rows_vs_one_probe.das -- -m <trunk> -r <drafter.gguf> --prompt-ids <file>
//       [--positions 128] [--ladder-positions 64] [--arms rows,one,rows2,ladder] [--cpu] [--ids <file>] [--save <prefix>]
//       [--ref <a.dump,b.dump>] [--detail one:rows]

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Trunk model path (a GGUF or a snapshot directory)"
    model : string
    @clarg_short = "r"   //! not -d: after the `--` the JIT runner reads -d as its own --jit-dump
    @clarg_doc = "DFlash2 drafter GGUF path: its greedy round draws the continuation, and its target layers are the forced drafter's"
    drafter : string
    @clarg_doc = "a file of comma-separated token ids: the prompt"
    prompt_ids : string
    @clarg_doc = "a file of comma-separated token ids: the continuation, instead of the drafter's round"
    ids : string
    @clarg_doc = "rows compared, the prefill's last row first (default 128)"
    positions : int = 128
    @clarg_doc = "rows each ladder rung walks (default 64)"
    ladder_positions : int = 64
    @clarg_doc = "the arms, a comma list of rows, one, rows2 (the rows arm again), ladder (default all four)"
    arms : string = "rows,one,rows2,ladder"
    @clarg_doc = "the ladder's rungs, a comma list of rerun, nopre, fat0, fat1, conc0, fuse0, chain0, chain1 (default every rung)"
    rungs : string
    @clarg_doc = "run the arms on the CPU route (no Metal; the ladder is the Metal driver's and is skipped)"
    cpu : bool
    @clarg_doc = "a path prefix: <prefix>.ids holds the continuation, <prefix>.<arm>.dump each arm's rows"
    save : string
    @clarg_doc = "comma-separated parity dumps of earlier runs, each compared against every arm"
    ref : string
    @clarg_doc = "the pair printed a row a position, a:b (b the reference; default one:rows)"
    detail : string = "one:rows"
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let VERIFY_ROWS = 8l   //! the DFlash2 round's verify: the anchor and seven drafts

var private g_one_unencoded = false   //! the one-row arm retires the step the driver encoded ahead before each step: every step encodes on its own token

//! the trunk layers the attached drafter captures
def private drafter_layers(c : Config) : array<int64> => <- [for (l in range64(c.n_layers + 1l)); l; where mtp_block_layer_slot(l) >= 0l]

def private shown_piece(m : Model; id : int64) : string => replace(piece(m, id), "\n", "\\n")

//! the greedy speculative walk with the real drafter: `steps` ids after the prompt
def private spec_walk(m : Model; prompt : array<int64>; steps : int64) : array<int64> {
    let vocab = m.config.vocab_size
    let np = long_length(prompt)
    var ids : array<int64>
    ids |> reserve(steps)
    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_mtp_spec(true)
    forward_prefill(m, s, prompt, np, 0l)
    s.n_past = np
    var tok = parallel_argmax(s.logits, vocab)
    var rounds = 0
    var accepted = 0l
    while (long_length(ids) < steps) {
        ids |> push(tok)
        break if (long_length(ids) >= steps)
        let a = mtp_spec_round(m, s, tok)
        rounds++
        accepted += a
        for (j in range64(a)) {
            break if (long_length(ids) >= steps)
            ids |> push(s.mtp_vbatch[j + 1l])
        }
        break if (long_length(ids) >= steps)
        tok = parallel_argmax(s.logits, vocab)
    }
    delete s
    print("spec walk: {length(ids)} ids in {rounds} rounds, {accepted} drafts accepted\n")
    return <- ids
}

//! an arm's rows (row j predicts the continuation's id j) and, for the rows arm, the verify row each came from (-1 = the prefill)
struct ArmRun {
    name : string
    rows : array<array<float>>
    src : array<int64>
}

//! a fresh session with the prompt prefilled the way the probe's walks prefill it, `spec` the round's gate
def private prefilled(m : Model; prompt : array<int64>; spec : bool) : Session {
    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_mtp_spec(spec)
    forward_prefill(m, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
    return <- s
}

//! the eight-row arm: the trunk's round over the forced drafts, the first verify covering a position its row
def private rows_arm(m : Model; name : string; np, npos : int64) : ArmRun {
    let vocab = m.config.vocab_size
    var run = ArmRun(name = name)
    run.rows |> resize(npos)
    run.src |> resize(npos)
    var inscope prompt <- [for (i in range64(np)); g_forced_seq[i]]
    var s <- prefilled(m, prompt, true)
    run.rows[0] <- logits_of(s, vocab)
    run.src[0] = -1l
    var covered = 1l
    var rounds = 0
    var served = 0
    var short = 0
    var rejects = 0
    while (covered < npos) {
        let pos = s.n_past
        if (pos + VERIFY_ROWS > long_length(g_forced_seq)) {
            to_log(LOG_WARNING, "rows_vs_one: the continuation ends before row {covered} - pass more ids\n")
            break
        }
        s.mtp_picks |> clear()
        let a = mtp_spec_round(m, s, g_forced_seq[pos])
        let n = g_forced_drafted + 1l
        rounds++
        served += long_length(s.mtp_picks) == n ? 1 : 0
        short += n < VERIFY_ROWS ? 1 : 0
        rejects += a < g_forced_drafted ? 1 : 0
        for (i in range64(n)) {
            let j = pos + i - np + 1l
            if (j >= covered && j < npos) {
                run.rows[j] <- [for (v in range64(vocab)); s.mtp_logits_b[i * vocab + v]]
                run.src[j] = i
            }
        }
        covered = clamp(pos + n - np + 1l, covered, npos)
        if (s.n_past != pos + a + 1l) {
            to_log(LOG_ERROR, "rows_vs_one: the round at {pos} left n_past {s.n_past}, not {pos + a + 1l}\n")
            break
        }
    }
    delete s
    print("arm {name}: {rounds} rounds, {served} served on the verify seat, {short} short of {VERIFY_ROWS} rows, {rejects} rejected a forced draft\n")
    return <- run
}

//! the one-row arm: the continuation fed a step at a time
def private one_arm(m : Model; name : string; np, npos : int64) : ArmRun {
    let vocab = m.config.vocab_size
    var run = ArmRun(name = name)
    run.rows |> reserve(npos)
    var inscope prompt <- [for (i in range64(np)); g_forced_seq[i]]
    var s <- prefilled(m, prompt, false)
    run.rows |> emplace <| logits_of(s, vocab)
    var one <- [0l]
    for (j in range64(1l, npos)) {
        one[0] = g_forced_seq[np + j - 1l]
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (g_one_unencoded) {
                mirror_quiesce()
            }
        }
        eval_(m, s, one)
        run.rows |> emplace <| logits_of(s, vocab)
    }
    delete one
    delete s
    return <- run
}

//! one position of `a` against the reference `b`
struct PosCmp {
    top_a : int64
    top_b : int64
    rel : float          //! max |a - b| over b's live max
    margin_a : float     //! a's own top-1 over b's top-1, in a's row
    margin_b : float     //! b's own top-1 over a's top-1, in b's row
    gap_b : float        //! b's top-1 over its runner-up
}

def private cmp_row(a, b : array<float>; vocab : int64) : PosCmp {
    let ta = parallel_argmax(a, vocab)
    let tb = parallel_argmax(b, vocab)
    return PosCmp(top_a = ta, top_b = tb, rel = logits_maxdiff(a, b) / max(live_maxabs(b), 1.0e-30),
        margin_a = a[ta] - a[tb], margin_b = b[tb] - b[ta], gap_b = top2_margin(b, vocab))
}

//! the first row whose top-1 is not the forced id, -1 where every row's is
def private first_off_forced(m : Model; r : ArmRun; np : int64) : int64 {
    for (j in range64(long_length(r.rows))) {
        return j if (parallel_argmax(r.rows[j], m.config.vocab_size) != g_forced_seq[np + j])
    }
    return -1l
}

//! `a` against the reference `b` over their common rows: the first top-1 split, every split, a summary a 32 rows; with
//! `detail`, a line a row
def private report_pair(m : Model; a, b : ArmRun; np : int64; detail : bool) {
    let vocab = m.config.vocab_size
    let n = min(long_length(a.rows), long_length(b.rows))
    print("=== {a.name} against {b.name} over {n} rows ===\n")
    var first = -1l
    var flips = 0
    var worst = 0.0
    var bworst = 0.0
    var bsum = 0.0
    var bagree = 0
    for (j in range64(n)) {
        let p = cmp_row(a.rows[j], b.rows[j], vocab)
        let split = p.top_a != p.top_b
        if (split) {
            flips++
            if (first < 0l) {
                first = j
            }
        }
        worst = max(worst, p.rel)
        bworst = max(bworst, p.rel)
        bsum += p.rel
        bagree += split ? 0 : 1
        if (detail) {
            let src = j < long_length(b.src) ? b.src[j] : (j < long_length(a.src) ? a.src[j] : -2l)
            let ma = fmt(":.3f", p.margin_a)
            let mb = fmt(":.3f", p.margin_b)
            let tail = split ? " | SPLIT margin {a.name} {ma} {b.name} {mb}" : ""
            print("row {j} (pos {np + j - 1l}, src {src}): forced {g_forced_seq[np + j]} | {b.name} {p.top_b} '{shown_piece(m,p.top_b)}' gap {fmt(":.3f", p.gap_b)} | {a.name} {p.top_a} '{shown_piece(m,p.top_a)}' | rel {fmt(":.2e", p.rel)}{tail}\n")
        } elif (split) {
            print("split at row {j}: {b.name} {p.top_b} '{shown_piece(m,p.top_b)}' | {a.name} {p.top_a} '{shown_piece(m,p.top_a)}' | margin {a.name} {fmt(":.3f", p.margin_a)} {b.name} {fmt(":.3f", p.margin_b)} | rel {fmt(":.2e", p.rel)}\n")
        }
        if ((j + 1l) % 32l == 0l || j + 1l == n) {
            let lo = j - j % 32l
            print("rows {lo}..{j}: top-1 agree {bagree}/{j - lo + 1l}, worst rel {fmt(":.2e", bworst)}, mean rel {fmt(":.2e", bsum / float(j - lo + 1l))}\n")
            bworst = 0.0
            bsum = 0.0
            bagree = 0
        }
    }
    if (first >= 0l) {
        let p = cmp_row(a.rows[first], b.rows[first], vocab)
        print("FIRST SPLIT {a.name} vs {b.name} at row {first} (pos {np + first - 1l}): {b.name} {p.top_b} '{shown_piece(m,p.top_b)}', {a.name} {p.top_a} '{shown_piece(m,p.top_a)}'; margin {a.name} {fmt(":.3f", p.margin_a)}, {b.name} {fmt(":.3f", p.margin_b)}, {b.name}'s gap {fmt(":.3f", p.gap_b)}; {flips} splits in {n}; worst rel {fmt(":.2e", worst)}\n")
    } else {
        print("NO SPLIT {a.name} vs {b.name} over {n} rows; worst rel {fmt(":.2e", worst)}\n")
    }
    print("off the forced ids first: {a.name} at {first_off_forced(m, a, np)}, {b.name} at {first_off_forced(m, b, np)}\n")
}

//! a ladder rung's one-line verdict against the rows arm and the one-row arm
def private report_rung(m : Model; rung : string; r, rows, one : ArmRun) {
    let vocab = m.config.vocab_size
    let n = min(long_length(r.rows), long_length(rows.rows))
    var first = -1l
    var flips = 0
    var worst = 0.0
    var sum = 0.0
    for (j in range64(n)) {
        let p = cmp_row(r.rows[j], rows.rows[j], vocab)
        if (p.top_a != p.top_b) {
            flips++
            first = first < 0l ? j : first
        }
        worst = max(worst, p.rel)
        sum += p.rel
    }
    var against_one = 0.0
    for (j in range64(min(n, long_length(one.rows)))) {
        against_one = max(against_one, logits_maxdiff(r.rows[j], one.rows[j]) / max(live_maxabs(one.rows[j]), 1.0e-30))
    }
    let rel1 = n > 1l ? cmp_row(r.rows[1], rows.rows[1], vocab).rel : 0.0
    print("LADDER {rung}: against rows first split {first}, {flips}/{n} splits, rel at row 1 {fmt(":.2e", rel1)}, mean {fmt(":.2e", sum / float(max(n, 1l)))}, worst {fmt(":.2e", worst)}; against the one-row arm worst rel {fmt(":.2e", against_one)}\n")
}

let private LADDER = fixed_array<string>("rerun", "nopre", "fat0", "fat1", "conc0", "fuse0", "chain0", "chain1")

//! a rung's knob set (`on`) or put back
def private ladder_knob(rung : string; on : bool; chain_mode : int) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (rung == "fat0" || rung == "fat1") {
            set_metal_kq_fat(on ? (rung == "fat0" ? 0 : 1) : -1)
        } elif (rung == "nopre") {
            g_one_unencoded = on
        } elif (rung == "conc0") {
            set_metal_decode_concurrent(on ? 0 : -1)
        } elif (rung == "fuse0") {
            set_metal_decode_fuse(on ? 0 : -1)
        } elif (rung == "chain0" || rung == "chain1") {
            set_metal_decode_speculate(on ? (rung == "chain0" ? 0 : 1) : chain_mode)
        }
    }
}

def private ladder(m : Model; np, npos : int64; rows, one : ArmRun; rungs : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let chain_mode = metal_decode_spec_mode()
        var inscope asked <- [for (w in split(rungs, ",")); strip(w); where !empty(strip(w))]
        for (rung in LADDER) {
            continue if (!empty(asked) && find_index(asked, rung) < 0)
            ladder_knob(rung, true, chain_mode)
            let st0 = metal_decode_stats()
            var inscope r <- one_arm(m, rung, np, npos)
            let st1 = metal_decode_stats()
            ladder_knob(rung, false, chain_mode)
            to_log(LOG_INFO, "rows_vs_one: rung {rung}: {st1.decodes - st0.decodes} device decodes, {st1.declines - st0.declines} declines\n")
            report_rung(m, rung, r, rows, one)
        }
    }
}

def private save_arm(a : Args; m : Model; r : ArmRun; np : int64) {
    return if (empty(a.save))
    let route = a.cpu ? "cpu" : "metal"
    var d = ParityDump(model = a.model, arm = "{route}-{r.name}", vocab = m.config.vocab_size)
    d.prompt <- [for (i in range64(np)); g_forced_seq[i]]
    d.feed <- [for (j in range64(long_length(r.rows) - 1l)); g_forced_seq[np + j]]
    d.rows := r.rows
    let path = "{a.save}.{r.name}.dump"
    parity_dump_write(path, d)
    print("saved {path}\n")
    delete d
}

def private save_ids(a : Args; cont : array<int64>) {
    return if (empty(a.save))
    let path = "{a.save}.ids"
    let text = join([for (id in cont); "{id}"], ",")
    if (!fwrite(path, text)) {
        to_log(LOG_ERROR, "rows_vs_one: cannot write {path}\n")
    }
}

//! every --ref dump as an arm, refused where its prompt or feed is not this run's sequence
def private load_refs(a : Args; np : int64) : array<ArmRun> {
    var refs : array<ArmRun>
    return <- refs if (empty(a.ref))
    for (path in split(a.ref, ",")) {
        let p = strip(path)
        continue if (empty(p))
        let why = parity_dump_refusal(p)
        if (!empty(why)) {
            print("ref refused: {why}\n")
            continue
        }
        var inscope d <- parity_dump_read(p)
        var same = long_length(d.prompt) == np
        for (i in range64(min(np, long_length(d.prompt)))) {
            same = same && d.prompt[i] == g_forced_seq[i]
        }
        for (j in range64(long_length(d.feed))) {
            same = same && np + j < long_length(g_forced_seq) && d.feed[j] == g_forced_seq[np + j]
        }
        if (!same) {
            print("ref refused: {p} holds another prompt or feed\n")
            continue
        }
        var r = ArmRun(name = "ref:{d.arm}")
        r.rows <- d.rows
        refs |> emplace(r)
    }
    return <- refs
}

//! the arm named `name` among `arms`, -1 where none
def private arm_index(arms : array<ArmRun>; name : string) : int {
    for (r, i in arms, count()) {
        return i if (r.name == name)
    }
    return -1
}

def private run_arms(a : Args; m : Model; np : int64) : int {
    let npos = int64(a.positions)
    var inscope want <- [for (w in split(a.arms, ",")); strip(w)]
    var inscope arms : array<ArmRun>
    for (w in want) {
        if (w == "rows" || w == "rows2") {
            arms |> emplace <| rows_arm(m, w, np, npos)
        } elif (w == "one") {
            arms |> emplace <| one_arm(m, w, np, npos)
        }
    }
    for (r in arms) {
        save_arm(a, m, r, np)
    }
    var inscope refs <- load_refs(a, np)
    let pair <- split(a.detail, ":")
    let ia = length(pair) == 2 ? arm_index(arms, pair[0]) : -1
    let ib = length(pair) == 2 ? arm_index(arms, pair[1]) : -1
    if (ia >= 0 && ib >= 0) {
        report_pair(m, arms[ia], arms[ib], np, true)
    }
    for (i in range(length(arms))) {
        for (j in range(length(arms))) {
            if (i < j && !(i == ia && j == ib) && !(i == ib && j == ia)) {
                report_pair(m, arms[j], arms[i], np, false)
            }
        }
        for (r in refs) {
            report_pair(m, arms[i], r, np, false)
        }
    }
    let irows = arm_index(arms, "rows")
    let ione = arm_index(arms, "one")
    if (!a.cpu && find_index(want, "ladder") >= 0) {
        if (irows < 0 || ione < 0) {
            print("the ladder needs the rows and one arms\n")
        } else {
            ladder(m, np, min(npos, int64(a.ladder_positions)), arms[irows], arms[ione], a.rungs)
        }
    }
    return 0
}

def private run(a : Args) : int {
    allow_cpu_prefill(true)
    if (a.cpu) {
        set_metal_mode(MetalMode.off)
    } else {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_mode(MetalMode.required)
            apply_box_profile_runtime()
            return 1 if (!tune_gate())
            select_decode_override("metal")
        } else {
            print("das_metal is not built into this binary\n")
            return 1
        }
    }
    set_mtp_accept(MtpAccept.exact)
    set_dflash_draft_temp(0.0)
    set_mtp_draft_temp(0.0)
    var rc = 0
    with_dasllama_jobque() {
        var m <- load_model(a.model, QuantMode.q8)
        var inscope prompt <- read_ids(a.prompt_ids)
        let np = long_length(prompt)
        let why = attach_dflash_drafter(m, a.drafter)
        if (!empty(why) || np == 0l) {
            if (np == 0l) {
                print("no prompt ids in '{a.prompt_ids}'\n")
            } else {
                print("drafter refused: {why}\n")
            }
            rc = 1
        } else {
            var inscope layers <- drafter_layers(m.config)
            let layer_list = join([for (l in layers); "{l}"], ",")
            print("drafter {base_name(a.drafter)} attached; target layers {layer_list}; prompt {np} ids\n")
            let need = int64(a.positions) + VERIFY_ROWS
            var inscope cont <- empty(a.ids) ? spec_walk(m, prompt, need) : read_ids(a.ids)
            save_ids(a, cont)
            detach_dflash_drafter()
            attach_forced_drafter(layers, VERIFY_ROWS - 1l)
            g_forced_seq |> clear()
            g_forced_seq |> push_from(prompt, cont)
            print("continuation {length(cont)} ids: {replace(decode(m, cont), "\n", "\\n")}\n")
            rc = run_arms(a, m, np)
            unregister_mtp_block_drafter()
        }
        delete m
    }
    return rc
}

[export]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "rows_vs_one_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model) || empty(a.drafter) || empty(a.prompt_ids)) {
        print_help(get_command_info(type<Args>), "rows_vs_one_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
