options gen2

// (1+λ) Evolution Strategy for tuning GreedyWeights.
// Phase 2: parallel evaluation via job_que.
// Run: bin/Release/daslang.exe examples/daslive/sequence/evolve.das
// JIT: bin/Release/daslang.exe -jit examples/daslive/sequence/evolve.das

require elo
require math
require daslib/random
require daslib/jobque_boost

// --- ES parameters ---
let ES_LAMBDA = 63         // children per generation (saturate 64c/128t CPU)
let ES_GENERATIONS = 60
let ES_GAMES_PER_PAIR = 200
let ES_SIGMA_INIT = 0.3
let ES_SIGMA_DECAY = 0.95
let ES_SIGMA_MIN = 0.02
let ES_MAX_WEIGHT = 50000.0
let ES_MIN_SCALE = 1.0     // floor for proportional noise (prevents zero-stuck)

// --- Weight names for printing ---
let WEIGHT_NAMES = fixed_array(
    "seq_complete", "line_4", "line_3", "line_2",
    "gap_4", "gap_3",
    "block_4", "block_3", "block_gap_4",
    "adjacency", "center",
    "disrupt_4", "disrupt_3", "disrupt_2", "useful_after"
)

// --- Weight <-> array<float> conversion ---

def weights_to_array(w : GreedyWeights) : array<float> {
    return <- [w.sequence_complete, w.line_4, w.line_3, w.line_2,
        w.gap_4, w.gap_3,
        w.block_4, w.block_3, w.block_gap_4,
        w.adjacency, w.center,
        w.disrupt_4, w.disrupt_3, w.disrupt_2, w.useful_after]
}

def array_to_weights(a : array<float>) : GreedyWeights {
    return GreedyWeights(
        sequence_complete = a[0],
        line_4 = a[1],
        line_3 = a[2],
        line_2 = a[3],
        gap_4 = a[4],
        gap_3 = a[5],
        block_4 = a[6],
        block_3 = a[7],
        block_gap_4 = a[8],
        adjacency = a[9],
        center = a[10],
        disrupt_4 = a[11],
        disrupt_3 = a[12],
        disrupt_2 = a[13],
        useful_after = a[14]
    )
}

// --- Gaussian RNG via Box-Muller ---

def random_gaussian(var seed : int4&) : float {
    var u1 = random_float(seed)
    if (u1 < 1.0e-7) {
        u1 = 1.0e-7
    }
    let u2 = random_float(seed)
    return sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2)
}

// --- Mutation ---

def mutate_weights(parent : array<float>; sigma : float; var seed : int4&) : array<float> {
    var child : array<float>
    for (v in parent) {
        let noise = random_gaussian(seed)
        let scale = max(abs(v), ES_MIN_SCALE)
        child |> push(clamp(v + sigma * scale * noise, 0.0, ES_MAX_WEIGHT))
    }
    return <- child
}

// --- Print weights compactly ---

def print_weights(a : array<float>) {
    let n = length(WEIGHT_NAMES)
    for (i in range(n)) {
        if (i >= length(a)) {
            break
        }
        if (i > 0) {
            print(", ")
        }
        let name = WEIGHT_NAMES[i]
        let val = a[i]
        print("{name}={val:.1f}")
    }
    print("\n")
}

// --- Play N games between two weight configs, return wins for A ---

def head_to_head(weights_a, weights_b : GreedyWeights; num_games, base_seed : int) : int {
    let bot_a = make_greedy_bot("a", weights_a)
    let bot_b = make_greedy_bot("b", weights_b)
    var a_wins = 0
    for (g in range(num_games)) {
        let seed = base_seed + g
        var winner : int
        if (g % 2 == 0) {
            winner = play_headless_game(bot_a, bot_b, seed)
        } else {
            let w = play_headless_game(bot_b, bot_a, seed)
            winner = w < 0 ? -1 : (w == 0 ? 1 : 0)
        }
        if (winner == 0) {
            a_wins++
        }
    }
    return a_wins
}

// --- Result struct for channel communication ---

struct EvalResult {
    child_idx : int
    wins : int
}

// --- ES main loop ---

[export]
def main() {
    print("(1+{ES_LAMBDA}) Evolution Strategy — {ES_GENERATIONS} generations, {ES_GAMES_PER_PAIR} games/pair [parallel]\n\n")

    var seed = random_seed(12345)
    var sigma = ES_SIGMA_INIT
    var parent <- weights_to_array(GreedyWeights())
    let t_start = ref_time_ticks()

    with_job_que() {
        for (gen in range(ES_GENERATIONS)) {
            // Generate λ children (sequential — fast, needs shared seed)
            var children : array<array<float>>
            for (i in range(ES_LAMBDA)) {
                children |> emplace(mutate_weights(parent, sigma, seed))
            }

            // Evaluate all children vs parent in parallel
            var best_child_idx = -1
            var best_child_wins = 0
            let parent_w = array_to_weights(parent)
            with_channel(ES_LAMBDA) $(ch) {
                with_wait_group(ES_LAMBDA) $(wg) {
                    for (i in range(ES_LAMBDA)) {
                        let child_w = array_to_weights(children[i])
                        let child_idx = i
                        let base_seed = gen * 10000 + i * 1000
                        new_job() @() {
                            let wins = head_to_head(child_w, parent_w, ES_GAMES_PER_PAIR, base_seed)
                            ch |> push_clone(EvalResult(child_idx = child_idx, wins = wins))
                            ch |> notify_and_release
                            wg |> done
                        }
                    }
                }

                // Collect results
                ch |> for_each_clone() $(r : EvalResult#) {
                    if (r.wins > best_child_wins) {
                        best_child_wins = r.wins
                        best_child_idx = r.child_idx
                    }
                }
            }

            // Select winner: child must win majority to replace parent
            let parent_wins = ES_GAMES_PER_PAIR - best_child_wins
            if (best_child_idx >= 0 && best_child_wins > parent_wins) {
                delete parent
                parent <- children[best_child_idx]
                let pct = float(best_child_wins) * 100.0 / float(ES_GAMES_PER_PAIR)
                print("Gen {gen}: child_{best_child_idx} selected ({pct:.0f}%)  sigma={sigma:.4f}\n")
            } else {
                print("Gen {gen}: parent retained     sigma={sigma:.4f}\n")
            }

            // Quick baseline check — parent vs default
            let final_w = array_to_weights(parent)
            let vs_default_wins = head_to_head(final_w, GreedyWeights(), ES_GAMES_PER_PAIR, gen * 10000 + 9500)
            print("  vs_default: {vs_default_wins}/{ES_GAMES_PER_PAIR}\n")
            print("  weights: ")
            print_weights(parent)

            // Decay sigma
            sigma = max(sigma * ES_SIGMA_DECAY, ES_SIGMA_MIN)

            delete children
        }
    }

    // Final report
    let elapsed_ms = get_time_usec(t_start) / 1000
    print("\nCompleted in {elapsed_ms} ms ({elapsed_ms / 1000} sec)\n")
    print("\n=== Final Optimized Weights ===\n")
    print("GreedyWeights(\n")
    let n = length(WEIGHT_NAMES)
    for (i in range(n)) {
        if (i >= length(parent)) {
            break
        }
        let name = WEIGHT_NAMES[i]
        let val = parent[i]
        let comma = i < n - 1 ? "," : ""
        print("    {name} = {val:.1f}{comma}\n")
    }
    print(")\n")

    delete parent
}
