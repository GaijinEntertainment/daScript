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
require _row_stats   // spread_of: the row statistic over rounds
require _seam_race   // the candidates over the seam table, the preamble, the rotated rounds
require daslib/clargs   // [CommandLineArgs]
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
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
// site's split-k fill bound, 0 = none: `set_cuda_tower_splitk_fill`), `carveout` (1 = the full carveout on the scoped
// kernels, `cud_l1_scoped`: `set_cuda_tower_carveout_full`), `fork` (0 = the q / k / v GEMMs on one stream:
// `set_cuda_tower_fork`), `handoff` (0 = the FastConformer front's round trip: `set_cuda_fc_front_handoff`), `graphs`
// (0 = the chains launched kernel by kernel: `set_cuda_tower_graphs`).

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

//! the race's seams, in `apply_seams`' order
let private SEAMS <- [
    RaceSeam(key = "col", allowed <- [0l, 32l, 128l, 256l], words = "0, 32, 128 or 256"),
    RaceSeam(key = "floor", words = "0 or more"),
    RaceSeam(key = "fill", words = "0 or more"),
    RaceSeam(key = "carveout", allowed <- [0l, 1l], words = "0 or 1"),
    RaceSeam(key = "fork", allowed <- [0l, 1l], words = "0 or 1"),
    RaceSeam(key = "handoff", allowed <- [0l, 1l], words = "0 or 1"),
    RaceSeam(key = "graphs", allowed <- [0l, 1l], words = "0 or 1")
]

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

//! the seams' values before the race, in `SEAMS`' order: what a candidate that names no value for a seam runs on
def private seams_found() : array<int64> {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return <- [cuda_tower_cm2_col(), cuda_tower_splitk_min_rows(), cuda_tower_splitk_fill(),
            cuda_tower_carveout_full() ? 1l : 0l, cuda_tower_fork_on() ? 1l : 0l,
            cuda_fc_front_handoff_on() ? 1l : 0l, cuda_tower_graphs_on() ? 1l : 0l]
    } else {
        return <- array<int64>()
    }
}

//! the seams set to `vals`, in `SEAMS`' order
def private apply_seams(vals : array<int64>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_cuda_tower_cm2_col(vals[0])
        set_cuda_tower_splitk_min_rows(vals[1])
        set_cuda_tower_splitk_fill(vals[2])
        set_cuda_tower_carveout_full(vals[3] == 1l)
        set_cuda_tower_fork(vals[4] == 1l)
        set_cuda_fc_front_handoff(vals[5] == 1l)
        set_cuda_tower_graphs(vals[6] == 1l)
    }
}

//! candidate `c`'s turn in a round: its seams, its warm passes, its timed passes into `run`
def private cand_turn(cfg : RaceArgs; m : AsrModel; samples : array<float>; c : RaceCand; found : array<int64>;
                      subs : array<string>; round : int; var run : CandRun) {
    apply_seams(race_resolve(c, found))
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
def private cand_summary(c : RaceCand; var run : CandRun) {
    let sp = spread_of(run.enc_ms)
    let wl = spread_of(run.wall_ms)
    let texts = race_distinct_hashes(run.hashes)
    print("SUMMARY [{c.name}]: encode {sp.mean:.3f} ms mean ({sp.best:.3f}-{sp.worst:.3f}), cv {sp.cv:.2f}% over {length(run.enc_ms)} passes; wall {wl.mean:.1f} ms mean; texts {texts}; {served_words(run.served, length(run.enc_ms))}\n")
}

[export, cold_path]
def main() : int {
    allow_cpu_prefill()   // the LLM-decoder families (qwen3a, canary, gemma4a) prefill their encoder rows on the CPU decoder
    return race_main("tower_race", "the CUDA tower's", type<RaceArgs>, SEAMS) $(var cfg; cands) {
        cfg.reps = max(cfg.reps, 1)
        let subs <- split(cfg.census, ",")
        let samples <- load_audio_16k_mono(cfg.wav)
        let audio_s = float(length(samples)) / float(AUDIO_SAMPLE_RATE)
        let t_load = ref_time_ticks()
        var m <- empty(cfg.mmproj) ? load_asr_model(cfg.model) : load_asr_model(cfg.model, cfg.mmproj)
        print("RACE model {base_name(cfg.model)}, clip {base_name(cfg.wav)} ({audio_s:.2f} s), exec {asr_exec_fmt(m)}, load {float(get_time_usec(t_load)) / 1000.0:.1f} ms, {length(cands)} candidates, {cfg.rounds} rounds, warm {cfg.warm}, reps {cfg.reps}, encode bucket '{asr_encode_bucket(m)}'\n")
        let found <- seams_found()
        print(race_seams_line(SEAMS, found))
        var runs : array<CandRun>
        runs |> resize(length(cands))
        with_dasllama_jobque_() {
            set_asr_prof(true)
            asr_prof_reset()
            race_rounds(cfg.rounds, length(cands)) $(ci, round) {
                cand_turn(cfg, m, samples, cands[ci], found, subs, round, runs[ci])
            }
            set_asr_prof(false)
        }
        apply_seams(found)
        for (c, run in cands, runs) {
            cand_summary(c, run)
        }
        return 0
    }
}
