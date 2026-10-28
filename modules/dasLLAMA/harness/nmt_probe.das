options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_nmt
require dasllama/dasllama_math
require ../performance/profile_common.das   // tune_gate, the standing pre-measure stack
require _race_fixture   // time_rounds, stats, tally, race_exit: the round loop, the row statistic and the exit code
require ../tests/_compares.das   // ids_equal
require ../tests/_nmt_oracle.das   // nmt_seat_*: the build's device translation seat
require daslib/clargs
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost

// The translation homes' race, one Bergamot model over a corpus of sentences (one a line): the arms translate the whole corpus
// in turn, a round each, `--warm` rounds an arm dropped, then `--reps` rounds an arm. Arms: cpu-q8 (the baseline: the q8
// lane on the CPU), cpu-f32 (the exact lane on the CPU - the CPU reference) and, on a build whose device driver serves the
// model, device (the q8 lane on the GPU: Metal, else Vulkan under DASLLAMA_GPU=1; a round the driver does not serve whole
// refuses the row). Each arm's report row is its best round in ms a line, with the median, the worst and
// the cv over its rounds, VOID past 3%. The compare region is the corpus's target token ids: the baseline's row prints how
// many lines are the reference's token for token, bound `--q8-floor` of the lines; the device's row how many are the
// baseline's, bound `--device-floor`. A row whose compare misses its bound prints the miss alone. The run exits 1 with no
// report row, and with any refused or missed row.
//
//   DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json bin/daslang -jit modules/dasLLAMA/harness/nmt_probe.das -- \
//       -m ~/Work/llama.cpp/models/bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin \
//       --corpus modules/dasLLAMA/benchmarks/nmt/corpus/ru.txt -r 5

[CommandLineArgs]
struct ProbeArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "The Bergamot model (`model.<pair>.intgemm.alphas.bin`, its vocabulary beside it)"
    model : string

    @clarg_required
    @clarg_doc = "The corpus: one source sentence a line"
    corpus : string

    @clarg_short = "r"
    @clarg_doc = "Rounds an arm, after the warm-ups (default 5)"
    reps : int = 5

    @clarg_doc = "Rounds an arm dropped before its first (default 1)"
    warm : int = 1

    @clarg_doc = "The fraction of lines the baseline must translate as the reference does (default 0.8)"
    q8_floor : float = 0.8

    @clarg_doc = "The fraction of lines the device must translate as the baseline does (default 0.85)"
    device_floor : float = 0.85

    @clarg_short = "h"
    @clarg_doc = "Show this help"
    help : bool
}

struct Arm {
    name : string
    q8 : bool
    device : bool
    ids : array<array<int64>>
    refused : bool   //!< a device round the seat did not serve whole
}

def agree(a, b : array<array<int64>>) : int {
    var n = 0
    for (x, y in a, b) {
        n += ids_equal(x, y) ? 1 : 0
    }
    return n
}

//! One round of `arm`: every source line through its model on its home; the ids kept from the arm's first round, a device round
//! the seat did not serve whole refusing the arm.
def round(var arm : Arm; m : NmtModel; var st : NmtState; srcs : array<array<int64>>) {
    let served0 = nmt_seat_stats().sentences
    let keep = empty(arm.ids)
    for (src in srcs) {
        var inscope ids <- nmt_translate_ids(m, st, src)
        if (keep) {
            arm.ids |> emplace(ids)
        }
    }
    if (arm.device && nmt_seat_stats().sentences - served0 != long_length(srcs)) {
        arm.refused = true
    }
}

[export]
def main : int {
    var r <- parse_args(type<ProbeArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ProbeArgs>), "nmt_probe")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<ProbeArgs>), "nmt_probe")
        return 0
    }
    if (!tune_gate()) {
        print("tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar\n")
        return 1
    }
    var tally_ = RaceTally()
    with_dasllama_jobque_() {
        var inscope mq <- load_nmt_model(cfg.model, true)
        var inscope mf <- load_nmt_model(cfg.model, false)
        var inscope st = NmtState()
        var lines <- [for (l in split(fread(cfg.corpus), "\n")); l; where !empty(l)]
        var inscope srcs <- [for (l in lines); nmt_encode_text(mq, l)]
        var arms <- [Arm(name = "cpu-q8", q8 = true), Arm(name = "cpu-f32", q8 = false)]
        if (nmt_seat_name() != "" && nmt_seat_ready() && mq.q8) {
            arms |> emplace(Arm(name = "device", q8 = true, device = true))
        }
        let was = nmt_seat_pin(false)
        var inscope times <- time_rounds(length(arms), cfg.reps, cfg.warm, $(i : int) {
            nmt_seat_pin(arms[i].device)
        }, $(i : int) {
            round(arms[i], arms[i].q8 ? mq : mf, st, srcs)
        })
        nmt_seat_pin(was)
        let n = length(lines)
        for (i, a in count(), arms) {
            var compare = ""
            var miss = false
            if (a.name == "cpu-q8") {
                let k = agree(a.ids, arms[1].ids)
                miss = float(k) < cfg.q8_floor * float(n)
                compare = "{k}/{n} lines the cpu-f32 reference's (bound {cfg.q8_floor * float(n):.0f})"
            } elif (a.device) {
                let k = agree(a.ids, arms[0].ids)
                miss = float(k) < cfg.device_floor * float(n)
                compare = "{k}/{n} lines the cpu-q8 baseline's (bound {cfg.device_floor * float(n):.0f})"
            } else {
                compare = "the reference"
            }
            if (a.refused) {
                print("ROW {a.name}: REFUSED - the device seat ({nmt_seat_name()}) did not serve every sentence of a round\n")
                tally(tally_, -1)
                continue
            }
            if (miss) {
                print("ROW {a.name}: MISS - {compare}\n")
                tally(tally_, -1)
                continue
            }
            let sp = stats(times[i])
            let per = float(n)
            let void_tag = sp.cv > 3.0 ? " VOID" : ""
            print("ROW {a.name}: {sp.best / per:.3f} ms/line best ({sp.median / per:.3f} median, {sp.worst / per:.3f} worst), cv {sp.cv:.2f}% over {length(times[i])} rounds, {n} lines; {compare}{void_tag}\n")
            tally(tally_, 1)
        }
        nmt_seat_shutdown()
    }
    return race_exit(tally_, "nmt_probe")
}
