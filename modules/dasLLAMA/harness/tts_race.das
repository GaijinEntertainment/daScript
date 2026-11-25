options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_tower   // TowerDev
require dasllama/dasllama_tower_tts   // tts_home_stats: the CUDA home's served seats and declines
require ?cuda dasllama/dasllama_cuda_common   // the carveout seam
require ?cuda dasllama/dasllama_cuda_rail   // the speech shot's launch-order seam
require _row_stats   // spread_of: the row statistic over rounds
require _seam_race   // the candidates over the seam table, the preamble, the rotated rounds
require tts_corpus   // tts_corpus_path / tts_corpus_head - the rig's sentences
require daslib/clargs   // [CommandLineArgs]
require daslib/fio
require math

// The CUDA speech seats' in-process race, `tower_race.das`'s form over a TTS model: one model and voice, the
// rig's sentence fixture, a list of candidates - each a set of the CUDA home's in-process seams - and `--rounds`
// rounds, each running every candidate once before the next round, the order rotated a round. A candidate's
// turn sets its seams (a seam it does not name goes back to the value the race found), synthesizes the first
// `--warm` sentences untimed, then the timed pass: every sentence of the fixture's head (`--limit`, 0 = all),
// the synthesis seconds the engine's own timings sum (front end through waveform, `tts_synth.das`'s figure), the
// pass's wall, the audio's hash and the CUDA home's served seats and declines over the pass, so an arm that
// declined to the CPU cannot read as a tie. The summary gives each candidate's synthesis mean, min-max and cv
// over its passes, its distinct audio hashes and its counters a pass.
//
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang --disable-module dasVulkan -jit modules/dasLLAMA/harness/tts_race.das -- \
//       -m /root/models/kitten-mini.gguf -v Bella --rounds 8 \
//       --cand carveout=0,pdl=0 --cand carveout=1,pdl=0 --cand carveout=0,pdl=1
//
// A candidate is `key=value` pairs joined by `,`: `carveout` (1 = the full carveout on the scoped kernels,
// `cud_l1_scoped`: `set_cuda_tower_carveout_full`), `pdl` (1 = a speech shot under the home's programmatic
// dependent launch: `set_cuda_tts_shot_pdl`).

[CommandLineArgs]
struct RaceArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "the TTS model GGUF"
    model : string

    @clarg_required
    @clarg_short = "v"
    @clarg_doc = "voice name or alias"
    voice : string

    @clarg_doc = "A candidate: key=value pairs joined by commas (carveout, pdl); repeat the flag for each"
    cand : array<string>

    @clarg_doc = "Rounds; each runs every candidate once, the order rotated a round (default 6)"
    rounds : int = 6

    @clarg_doc = "Sentences synthesized untimed on a candidate's turn before its timed pass (default 8)"
    warm : int = 8

    @clarg_doc = "Sentences of the fixture a timed pass speaks (0 = all)"
    limit : int = 0

    @clarg_doc = "the sentence fixture (default: tests/_tts_fixtures/g2p_corpus.json)"
    fixture : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! the race's seams, in `apply_seams`' order
let private SEAMS <- [
    RaceSeam(key = "carveout", allowed <- [0l, 1l], words = "0 or 1"),
    RaceSeam(key = "pdl", allowed <- [0l, 1l], words = "0 or 1")
]

//! one candidate's record over the race
struct CandRun {
    synth_s : array<float>
    wall_s : array<float>
    hashes : array<uint64>
    served : int64
    declines : int64
}

//! the seams' values before the race, in `SEAMS`' order: what a candidate that names no value for a seam runs on
def private seams_found() : array<int64> {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return <- [cuda_tower_carveout_full() ? 1l : 0l, cuda_tts_shot_pdl_on() ? 1l : 0l]
    } else {
        return <- array<int64>()
    }
}

//! the seams set to `vals`, in `SEAMS`' order
def private apply_seams(vals : array<int64>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_cuda_tower_carveout_full(vals[0] == 1l)
        set_cuda_tts_shot_pdl(vals[1] == 1l)
    }
}

//! one pass over `rows`: the synthesis seconds the engine's timings sum, and the audio's hash
def private speak_pass(var m : TtsModel; rows : array<TtsSentence>; voice : string) : tuple<synth_s : float; h : uint64> {
    var us = 0l
    var h = 0ul
    for (r in rows) {
        var inscope a <- synthesize(m, r.text, voice)
        us += a.timings.total_us
        h = h * 1099511628211ul + hash(a.pcm)
    }
    return (synth_s = float(double(us) / 1e6lf), h = h)
}

//! candidate `c`'s turn in a round: its seams, its warm sentences, its timed pass into `run`
def private cand_turn(cfg : RaceArgs; var m : TtsModel; rows, warm : array<TtsSentence>; c : RaceCand;
                      found : array<int64>; round : int; var run : CandRun) {
    apply_seams(race_resolve(c, found))
    speak_pass(m, warm, cfg.voice)
    let st0 = tts_home_stats(TowerDev.cuda)
    let t0 = ref_time_ticks()
    let p = speak_pass(m, rows, cfg.voice)
    let wall = float(get_time_usec(t0)) / 1e6
    let st1 = tts_home_stats(TowerDev.cuda)
    run.synth_s |> push(p.synth_s)
    run.wall_s |> push(wall)
    run.hashes |> push(p.h)
    run.served += st1.encodes - st0.encodes
    run.declines += st1.declines - st0.declines
    print("RACE round {round} [{c.name}]: synthesis {p.synth_s:.3f} s, wall {wall:.3f} s, audio {p.h:x}; CUDA seats {st1.encodes - st0.encodes}, declines {st1.declines - st0.declines}\n")
}

//! the summary line of candidate `c` over the race
def private cand_summary(c : RaceCand; var run : CandRun) {
    let passes = length(run.synth_s)
    let sp = spread_of(run.synth_s)
    let wl = spread_of(run.wall_s)
    let audio = race_distinct_hashes(run.hashes)
    let p = int64(max(passes, 1))
    print("SUMMARY [{c.name}]: synthesis {sp.mean:.3f} s mean ({sp.best:.3f}-{sp.worst:.3f}), cv {sp.cv:.2f}% over {passes} passes; wall {wl.mean:.3f} s mean; audio {audio}; CUDA seats {run.served / p}, declines {run.declines / p} a pass\n")
}

[export, cold_path]
def main() : int {
    return race_main("tts_race", "the CUDA home's", type<RaceArgs>, SEAMS) $(var cfg; cands) {
        let fixture = empty(cfg.fixture) ? tts_corpus_path() : cfg.fixture
        let text = fread(fixture)
        var inscope rows <- tts_corpus_head(text, cfg.limit > 0 ? cfg.limit : INT_MAX)
        var inscope warm <- tts_corpus_head(text, cfg.warm)
        if (empty(rows)) {
            print("tts_race: {fixture} does not parse, or holds no sentence\n")
            return 1
        }
        let found <- seams_found()
        var runs : array<CandRun>
        runs |> resize(length(cands))
        with_dasllama_jobque_() {
            let t_load = ref_time_ticks()
            var inscope m <- load_tts_model(cfg.model)
            print("RACE model {base_name(cfg.model)}, voice {cfg.voice}, {length(rows)} sentences, load {float(get_time_usec(t_load)) / 1000.0:.1f} ms, {length(cands)} candidates, {cfg.rounds} rounds, warm {cfg.warm} sentences\n")
            print(race_seams_line(SEAMS, found))
            race_rounds(cfg.rounds, length(cands)) $(ci, round) {
                cand_turn(cfg, m, rows, warm, cands[ci], found, round, runs[ci])
            }
        }
        apply_seams(found)
        for (c, run in cands, runs) {
            cand_summary(c, run)
        }
        return 0
    }
}
