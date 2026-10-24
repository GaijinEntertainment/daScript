options gen2
options _dasllama_internal = true

// The fp32 batch GEMM's two constants and its tile against the row nest, raced in one process,
// interleaved: the X block the tile walk holds in L2 (set_f32_batch_x_block), the dot length under
// which the row nest serves (set_f32_batch_tile_min_n), and the tile itself against the nest at the
// conv front's shapes. One line a candidate; the ledger entry that cites them names this script.
//
//   DAS_TUNE_MANIFEST=<sidecar> bin/daslang -jit modules/dasLLAMA/harness/f32_batch_race.das [-- --reps N]

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require daslib/jobque_boost
require daslib/clargs

[CommandLineArgs]
struct Args {
    @clarg_doc = "Timed rounds per candidate, interleaved (default: 7, best kept)"
    reps : int = 7
    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct Operands {
    w : array<float>
    x : array<float>
    y : array<float>
    n : int64
    d : int64
    ntok : int64
}

def private operands(n, d, ntok : int64) : Operands {
    var o = Operands(n = n, d = d, ntok = ntok)
    o.w |> resize(n * d)
    o.x |> resize(ntok * n)
    o.y |> resize(ntok * d)
    for (i in range64(n * d)) {
        o.w[i] = float(i % 13l) * 0.1 - 0.6
    }
    for (i in range64(ntok * n)) {
        o.x[i] = float(i % 7l) * 0.2 - 0.7
    }
    return <- o
}

def private gflops(o : Operands; ms : double) : double => 2.0lf * double(o.n * o.d * o.ntok) / (ms * 1.0e6lf)

def private time_batch_ms(var o : Operands) : double {
    let t0 = ref_time_ticks()
    matmul_batch(o.y, o.w, o.x, o.n, o.d, o.ntok)
    return double(get_time_usec(t0)) / 1000.0lf
}

//! `reps` rounds, each round every candidate once in order, the best of each kept
def private race(name : string; var o : Operands; labels : array<string>; reps : int; arm : block<(i : int) : void>) {
    var best : array<double>
    best |> resize(length(labels))
    for (i in range(length(labels))) {
        best[i] = 1.0e30lf
        invoke(arm, i)
        time_batch_ms(o)   // warm
    }
    for (_r in range(reps)) {
        for (i in range(length(labels))) {
            invoke(arm, i)
            best[i] = min(best[i], time_batch_ms(o))
        }
    }
    for (lbl, ms in labels, best) {
        print("{name} n={o.n} d={o.d} ntok={o.ntok} {lbl}: {ms} ms ({gflops(o, ms)} GFLOP/s)\n")
    }
}

//! past every dot length: the row nest serves every output
let private NEST_ONLY_MIN_N = 1l << 30

[export]
def main() : int {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "f32_batch_race")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "f32_batch_race")
        return 0
    }
    with_dasllama_jobque_() {
        let x_block_default = get_f32_batch_x_block()
        let min_n_default = get_f32_batch_tile_min_n()
        let xblocks <- [64l * 1024l, 128l * 1024l, 256l * 1024l, 512l * 1024l, 1024l * 1024l, 4096l * 1024l]
        let xlabels <- [for (b in xblocks); "x_block={b / 1024l}K"]
        for (shape in [(n = 4320l, d = 480l, ntok = 800l), (n = 1152l, d = 480l, ntok = 1536l)]) {
            var o <- operands(shape.n, shape.d, shape.ntok)
            race("x_block", o, xlabels, cfg.reps) $(i : int) {
                set_f32_batch_x_block(xblocks[i])
            }
            set_f32_batch_x_block(x_block_default)
            race("tile_vs_nest", o, ["tile", "nest"], cfg.reps) $(i : int) {
                set_f32_batch_tile_min_n(i == 0 ? min_n_default : NEST_ONLY_MIN_N)
            }
            set_f32_batch_tile_min_n(min_n_default)
        }
        // the crossover: at each dot length the tile against the nest, the min_n pick sits where the tile wins
        for (n in [8l, 16l, 24l, 32l, 48l, 64l, 128l]) {
            var o <- operands(n, 480l, 3200l)
            race("min_n", o, ["tile", "nest"], cfg.reps) $(i : int) {
                set_f32_batch_tile_min_n(i == 0 ? 1l : NEST_ONLY_MIN_N)
            }
        }
        set_f32_batch_tile_min_n(min_n_default)
    }
    return 0
}
