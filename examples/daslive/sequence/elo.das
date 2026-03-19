options gen2

require gameplay
require bot_random
require bot_greedy
require math

// ELO rating system + headless tournament runner.
// Run: bin/Release/daslang.exe examples/daslive/sequence/elo.das
// JIT: bin/Release/daslang.exe -jit examples/daslive/sequence/elo.das

let ELO_K = 32.0
let ELO_START = 1500.0

struct BotEntry {
    name : string
    bot_type : string       // "random" or "greedy"
    weights : GreedyWeights = GreedyWeights() // used when bot_type == "greedy"
    elo : float
    wins : int
    losses : int
    draws : int
}

def make_random_bot(name : string) : BotEntry {
    return BotEntry(name = name, bot_type = "random", elo = ELO_START)
}

def make_greedy_bot(name : string; w : GreedyWeights = GreedyWeights()) : BotEntry {
    return BotEntry(name = name, bot_type = "greedy", weights = w, elo = ELO_START)
}

def elo_expected(rating_a, rating_b : float) : float {
    return 1.0 / (1.0 + pow(10.0, (rating_b - rating_a) / 400.0))
}

def elo_update(var a_elo, b_elo : float&; a_won : bool) {
    let ea = elo_expected(a_elo, b_elo)
    let eb = 1.0 - ea
    let sa = a_won ? 1.0 : 0.0
    let sb = a_won ? 0.0 : 1.0
    a_elo += ELO_K * (sa - ea)
    b_elo += ELO_K * (sb - eb)
}

def pick_move(bot : BotEntry; game : GameState; player_idx, seed : int) : Move {
    if (bot.bot_type == "random") {
        return bot_random_move(game, player_idx, seed)
    } elif (bot.bot_type == "greedy") {
        return bot_greedy_move(game, player_idx, seed, bot.weights)
    }
    return Move()
}

// Play a single headless game. Returns 0 if bot_a wins, 1 if bot_b wins, -1 for draw.
def play_headless_game(bot_a, bot_b : BotEntry; seed : int; max_turns : int = 500) : int {
    var game = GameState()
    make_game(game, 2, seed)
    var turn = 0
    var rng = seed
    while (game.phase == GamePhase.playing && turn < max_turns) {
        let p = game.current_player
        rng++
        var m = Move()
        if (p == 0) {
            m = pick_move(bot_a, game, p, rng)
        } else {
            m = pick_move(bot_b, game, p, rng)
        }
        if (m.card_idx < 0) {
            discard_dead_cards(game, p)
            game.current_player = (game.current_player + 1) % game.num_players
            turn++
            continue
        }
        apply_move(game, p, m)
        turn++
    }
    if (game.phase == GamePhase.game_over) {
        return game.winner
    }
    return -1
}

def run_tournament(var bots : array<BotEntry>; num_games : int) {
    let n = length(bots)
    print("Tournament: {num_games} games per pair, {n} bots\n")
    print("---\n")
    var game_seed = 42
    for (a in range(n)) {
        for (b in range(a + 1, n)) {
            var a_wins = 0
            var b_wins = 0
            var draws = 0
            for (g in range(num_games)) {
                game_seed++
                var winner : int
                if (g % 2 == 0) {
                    winner = play_headless_game(bots[a], bots[b], game_seed)
                } else {
                    let w = play_headless_game(bots[b], bots[a], game_seed)
                    winner = w < 0 ? -1 : (w == 0 ? 1 : 0)
                }
                if (winner == 0) {
                    a_wins++
                    elo_update(bots[a].elo, bots[b].elo, true)
                } elif (winner == 1) {
                    b_wins++
                    elo_update(bots[a].elo, bots[b].elo, false)
                } else {
                    draws++
                }
            }
            bots[a].wins += a_wins
            bots[a].losses += b_wins
            bots[a].draws += draws
            bots[b].wins += b_wins
            bots[b].losses += a_wins
            bots[b].draws += draws
            let pct = num_games > 0 ? float(a_wins) * 100.0 / float(num_games) : 0.0
            print("{bots[a].name} vs {bots[b].name}: {a_wins}W {b_wins}L {draws}D ({pct:.1f}%)\n")
        }
    }
    // Sort by ELO descending
    for (i in range(1, n)) {
        var j = i
        while (j > 0 && bots[j].elo > bots[j - 1].elo) {
            let tmp = bots[j]
            bots[j] = bots[j - 1]
            bots[j - 1] = tmp
            j--
        }
    }
    print("\n--- Final Ratings ---\n")
    for (bot in bots) {
        print("{bot.name}: ELO={int(bot.elo)}  W={bot.wins} L={bot.losses} D={bot.draws}\n")
    }
}

[export]
def main() {
    var bots : array<BotEntry>
    bots |> push(make_random_bot("random"))
    bots |> push(make_greedy_bot("defensive", GreedyWeights(
        block_4 = 900.0, block_3 = 150.0,
        line_4 = 300.0, line_3 = 50.0
    )))
    bots |> push(make_greedy_bot("gap-boy", GreedyWeights(
        block_4 = 900.0, block_3 = 150.0,
        line_4 = 300.0, line_3 = 50.0,
        gap_4 = 400.0, gap_3 = 100.0, block_gap_4 = 300.0
    )))
    run_tournament(bots, 1000)
}
