options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_asr
require dasllama/dasllama_audio   // AUDIO_SAMPLE_RATE
require dasllama/dasllama_audio_io   // load_audio_16k_mono
require dasllama/dasllama_math   // with_dasllama_jobque_
require math
require dasllama/dasllama_tower   // the asr_prof rail: the family's encode bucket
require dasllama/dasllama_common   // allow_cpu_prefill
require ?cuda dasllama/dasllama_cuda_common   // the launch census, the carveout seam
require ?cuda dasllama/dasllama_cuda_rail   // the q / k / v fork seam
require ?cuda dasllama/dasllama_cuda_tower_seat   // the column and split-floor seams, the tower's served counters
require ?cuda dasllama/dasllama_cuda_tower_fc   // the FastConformer front handoff seam
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime, the standing pre-measure stack
require _row_stats   // spread_of: the row statistic over rounds
require daslib/clargs
require daslib/jobque_boost
require daslib/fio
require daslib/strings_convert
require strings

// The CUDA tower's in-process race: one model, one clip, a list of candidates - each a set of the tower's in-process seams -
// and `--rounds` rounds, each round running every candidate once before the next round, the order rotated a round. A
// candidate's turn sets its seams (a seam it does not name goes back to the value the race found), runs `--warm` untimed
// passes (the class loads, the graph captures its picks key), then `--reps` timed passes: the family's encode bucket on the
// asr_prof rail, the pass's wall, the transcript's hash, the tower's served counters and the launch census of the kernels
// `--census` names, each a delta over the timed passes, so an arm that declined or never reached the device cannot read as
// a tie. The summary gives each candidate's encode mean, min-max and cv over every timed pass, its distinct transcript
// hashes and its counters a pass.
//
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang -jit modules/dasLLAMA/harness/tower_race.das -- \
//       -m F:/models/whisper.cpp/ggml-large-v3-turbo.bin -w F:/models/jfk.wav --rounds 8 \
//       --cand col=0 --cand col=32 --cand col=128 --cand col=256
//
// A candidate is `key=value` pairs joined by `,`: `col` (32 | 128 | 256, 0 = the tower's own pick:
// `set_cuda_tower_cm2_col`), `floor` (the fc2 site's split-k row floor: `set_cuda_tower_splitk_min_rows`), `fill` (the fc2
// site's split-k fill bound, 0 = none: `set_cuda_tower_splitk_fill`), `carveout` (1 = the full carveout on the tower's
// no-smem kernels: `set_cuda_tower_carveout_full`), `fork` (0 = the q / k / v GEMMs on one stream: `set_cuda_tower_fork`),
// `handoff` (0 = the FastConformer front's round trip: `set_cuda_fc_front_handoff`), `graphs` (0 = the chains launched
// kernel by kernel: `set_cuda_tower_graphs`).

[CommandLineArgs]
struct RaceArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "ASR model path"
    model : string

    @clarg_doc = "mmproj (or encoder) file for two-file models"
    mmproj : string

    @clarg_required
    @clarg_short = "w"
    @clarg_doc = "Audio file"
    wav : string

    @clarg_doc = "A candidate: key=value pairs joined by commas (col, floor, fill, carveout, fork, handoff, graphs); repeat the flag for each"
    cand : array<string>

    @clarg_doc = "Rounds; each runs every candidate once, the order rotated a round (default 6)"
    rounds : int = 6

    @clarg_doc = "Untimed passes on a candidate's turn before its timed ones (default 1)"
    warm : int = 1

    @clarg_doc = "Timed passes on a candidate's turn (default 1)"
    reps : int = 1

    @clarg_doc = "Launch-census kernel names to count, comma-separated substrings (default cm2,splitk)"
    census : string = "cm2,splitk"

    @clarg_doc = "Session language (default en; Qwen3-ASR needs auto)"
    lang : string = "en"

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let private SEAM_KEEP = -1l   //! a seam the candidate does not name: the value the race found

//! one candidate: its seams' values, `SEAM_KEEP` where it names none
struct Cand {
    name : string
    col : int64 = SEAM_KEEP
    floor : int64 = SEAM_KEEP
    fill : int64 = SEAM_KEEP
    carveout : int64 = SEAM_KEEP
    fork : int64 = SEAM_KEEP
    handoff : int64 = SEAM_KEEP
    graphs : int64 = SEAM_KEEP
}

//! a candidate's served counters over its timed passes
struct Served {
    encodes : int64
    declines : int64
    rows : int64
    blocks : int64
    tails : int64
    census : table<string; int64>
}

//! one candidate's record over the race
struct CandRun {
    enc_ms : array<float>
    wall_ms : array<float>
    hashes : array<uint64>
    served : Served
}

//! the candidate `spec` names, or the reason it names none
def private parse_cand(spec : string) : tuple<ok : bool; c : Cand; why : string> {
    var c = Cand(name = spec)
    for (kv in split(spec, ",")) {
        let eq = find(kv, "=")
        return (ok = false, c = c, why = "'{kv}' is not key=value") if (eq <= 0)
        let key = slice(kv, 0, eq)
        let v = try_to_int64(slice(kv, eq + 1))
        return (ok = false, c = c, why = "'{kv}': the value is not an integer") if (v |> is_err)
        let n = v |> unwrap
        if (key == "col") {
            return (ok = false, c = c, why = "col={n}: 0, 32, 128 or 256") if (n != 0l && n != 32l && n != 128l && n != 256l)
            c.col = n
        } elif (key == "floor" || key == "fill") {
            return (ok = false, c = c, why = "{key}={n}: 0 or more") if (n < 0l)
            if (key == "floor") {
                c.floor = n
            } else {
                c.fill = n
            }
        } elif (key == "carveout" || key == "fork" || key == "handoff" || key == "graphs") {
            return (ok = false, c = c, why = "{key}={n}: 0 or 1") if (n != 0l && n != 1l)
            if (key == "carveout") {
                c.carveout = n
            } elif (key == "fork") {
                c.fork = n
            } elif (key == "handoff") {
                c.handoff = n
            } else {
                c.graphs = n
            }
        } else {
            return (ok = false, c = c, why = "unknown seam '{key}' (col, floor, fill, carveout, fork, handoff, graphs)")
        }
    }
    return (ok = true, c = c, why = "")
}

//! the seams' values before the race: what a candidate that names no value for a seam runs on
def private seams_now() : Cand {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return Cand(name = "found", col = cuda_tower_cm2_col(), floor = cuda_tower_splitk_min_rows(),
            fill = cuda_tower_splitk_fill(), carveout = cuda_tower_carveout_full() ? 1l : 0l,
            fork = cuda_tower_fork_on() ? 1l : 0l, handoff = cuda_fc_front_handoff_on() ? 1l : 0l,
            graphs = cuda_tower_graphs_on() ? 1l : 0l)
    } else {
        return Cand(name = "found")
    }
}

def private pick_seam(v, base : int64) : int64 => v != SEAM_KEEP ? v : base

//! the candidate's seams set, each it does not name at `base`'s value
def private apply_seams(c, base : Cand) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_cuda_tower_cm2_col(pick_seam(c.col, base.col))
        set_cuda_tower_splitk_min_rows(pick_seam(c.floor, base.floor))
        set_cuda_tower_splitk_fill(pick_seam(c.fill, base.fill))
        set_cuda_tower_carveout_full(pick_seam(c.carveout, base.carveout) == 1l)
        set_cuda_tower_fork(pick_seam(c.fork, base.fork) == 1l)
        set_cuda_fc_front_handoff(pick_seam(c.handoff, base.handoff) == 1l)
        set_cuda_tower_graphs(pick_seam(c.graphs, base.graphs) == 1l)
    }
}

//! the launch census of every kernel whose name holds one of `subs`
def private census_now(subs : array<string>) : table<string; int64> {
    var counts : table<string; int64>
    static_if (typeinfo builtin_module_exists(cuda)) {
        for (name in cud_kernel_names()) {
            for (s in subs) {
                if (find(name, s) >= 0) {
                    counts |> insert(name, cud_kernel_census_of(name))
                    break
                }
            }
        }
    }
    return <- counts
}

//! the tower's served counters and the census now
def private served_now(subs : array<string>) : Served {
    var sv = Served(census <- census_now(subs))
    static_if (typeinfo builtin_module_exists(cuda)) {
        let st = cuda_tower_stats()
        sv.encodes = st.encodes
        sv.declines = st.declines
        sv.rows = st.rows
        sv.blocks = st.blocks
        sv.tails = cuda_tower_tail_encodes()
    }
    return <- sv
}

//! `acc` += `b` - `a`, counter by counter
def private served_add(var acc : Served; a, b : Served) {
    acc.encodes += b.encodes - a.encodes
    acc.declines += b.declines - a.declines
    acc.rows += b.rows - a.rows
    acc.blocks += b.blocks - a.blocks
    acc.tails += b.tails - a.tails
    for (k, v in keys(b.census), values(b.census)) {
        let d = v - (a.census?[k] ?? 0l)
        if (d != 0l) {
            acc.census[k] = (acc.census?[k] ?? 0l) + d
        }
    }
}

//! the served counters as one line, the census by kernel name in name order
def private served_words(sv : Served; passes : int) : string {
    let p = int64(max(passes, 1))
    var names <- [for (k in keys(sv.census)); k]
    names |> sort()
    let census = join([for (k in names); "{k} {(sv.census?[k] ?? 0l) / p}"], ", ")
    return "encodes {sv.encodes / p}, declines {sv.declines / p}, rows {sv.rows / p}, blocks {sv.blocks / p}, tails {sv.tails / p} a pass; census a pass: {empty(census) ? "none" : census}"
}

//! one pass: a session, the clip transcribed, the transcript
def private transcribe_pass(m : AsrModel; samples : array<float>; lang : string) : string {
    var s <- create_session(m, lang)
    let text = transcribe(m, s, samples)
    delete s
    return text
}

//! candidate `c`'s turn in a round: its seams, its warm passes, its timed passes into `run`
def private cand_turn(cfg : RaceArgs; m : AsrModel; samples : array<float>; c, base : Cand; subs : array<string>; round : int;
                      var run : CandRun) {
    apply_seams(c, base)
    for (_w in range(cfg.warm)) {
        transcribe_pass(m, samples, cfg.lang)
    }
    let bucket = asr_encode_bucket(m)
    run.enc_ms |> reserve(length(run.enc_ms) + cfg.reps)
    run.wall_ms |> reserve(length(run.wall_ms) + cfg.reps)
    run.hashes |> reserve(length(run.hashes) + cfg.reps)
    for (_rep in range(cfg.reps)) {
        let sv0 <- served_now(subs)
        let enc0 = asr_prof_ms(bucket)
        let t0 = ref_time_ticks()
        let text = transcribe_pass(m, samples, cfg.lang)
        let wall = float(get_time_usec(t0)) / 1000.0
        let enc = float(asr_prof_ms(bucket) - enc0)
        let sv1 <- served_now(subs)
        var one : Served
        served_add(one, sv0, sv1)
        served_add(run.served, sv0, sv1)
        let h = hash(text)
        run.enc_ms |> push(enc)
        run.wall_ms |> push(wall)
        run.hashes |> push(h)
        print("RACE round {round} [{c.name}]: encode {enc:.3f} ms, wall {wall:.1f} ms, text {h:x}; {served_words(one, 1)}\n")
    }
}

//! the summary line of candidate `c` over the race
def private cand_summary(c : Cand; var run : CandRun) {
    var distinct <- {for (h in run.hashes); h}
    let sp = spread_of(run.enc_ms)
    let wl = spread_of(run.wall_ms)
    let texts = join([for (h in keys(distinct)); "{h:x}"], " ")
    print("SUMMARY [{c.name}]: encode {sp.mean:.3f} ms mean ({sp.best:.3f}-{sp.worst:.3f}), cv {sp.cv:.2f}% over {length(run.enc_ms)} passes; wall {wl.mean:.1f} ms mean; texts {texts}; {served_words(run.served, length(run.enc_ms))}\n")
}

[export, cold_path]
def main() : int {
    allow_cpu_prefill()   // the LLM-decoder families (qwen3a, canary, gemma4a) prefill their encoder rows on the CPU decoder
    var r <- parse_args(type<RaceArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<RaceArgs>), "tower_race")
        return 1
    }
    var cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<RaceArgs>), "tower_race")
        return 0
    }
    if (!jit_enabled()) {
        print("JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    static_if (!typeinfo builtin_module_exists(cuda)) {
        print("tower_race: this build carries no CUDA module - the seams it races are the CUDA tower's\n")
        return 1
    }
    if (length(cfg.cand) < 2) {
        print("tower_race: a race needs two candidates or more (--cand, repeated)\n")
        return 1
    }
    var cands : array<Cand>
    cands |> reserve(length(cfg.cand))
    for (spec in cfg.cand) {
        let pc = parse_cand(spec)
        if (!pc.ok) {
            print("tower_race: --cand {spec}: {pc.why}\n")
            return 1
        }
        cands |> push(pc.c)
    }
    cfg.rounds = max(cfg.rounds, 1)
    cfg.warm = max(cfg.warm, 1)
    cfg.reps = max(cfg.reps, 1)
    apply_box_profile_runtime()
    if (!tune_gate()) {
        print("tower_race: tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar\n")
        return 1
    }
    let subs <- split(cfg.census, ",")
    let samples <- load_audio_16k_mono(cfg.wav)
    let audio_s = float(length(samples)) / float(AUDIO_SAMPLE_RATE)
    let t_load = ref_time_ticks()
    var m <- empty(cfg.mmproj) ? load_asr_model(cfg.model) : load_asr_model(cfg.model, cfg.mmproj)
    print("RACE model {base_name(cfg.model)}, clip {base_name(cfg.wav)} ({audio_s:.2f} s), exec {asr_exec_fmt(m)}, load {float(get_time_usec(t_load)) / 1000.0:.1f} ms, {length(cands)} candidates, {cfg.rounds} rounds, warm {cfg.warm}, reps {cfg.reps}, encode bucket '{asr_encode_bucket(m)}'\n")
    let base = seams_now()
    print("RACE seams found: col {base.col}, floor {base.floor}, fill {base.fill}, carveout {base.carveout}, fork {base.fork}, handoff {base.handoff}, graphs {base.graphs}\n")
    var runs : array<CandRun>
    runs |> resize(length(cands))
    with_dasllama_jobque_() {
        set_asr_prof(true)
        asr_prof_reset()
        for (round in range(cfg.rounds)) {
            for (i in range(length(cands))) {
                let ci = (i + round) % length(cands)
                cand_turn(cfg, m, samples, cands[ci], base, subs, round, runs[ci])
            }
        }
        set_asr_prof(false)
    }
    apply_seams(base, base)
    for (c, run in cands, runs) {
        cand_summary(c, run)
    }
    return 0
}
