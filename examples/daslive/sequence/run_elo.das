options gen2

require elo

[export]
def main() {
    var bots : array<BotEntry>
    bots |> push(make_random_bot("random"))
    bots |> push(make_greedy_bot("greedy"))
    bots |> push(make_greedy_bot("strange-boy", GreedyWeights(
        line_4 = 300.0,
        line_3 = 50.0,
        line_2 = 20.0,
        gap_4 = 300.0,
        gap_3 = 50.0,
        block_4 = 900.0,
        block_3 = 150.0,
        block_gap_4 = 900.0,
        disrupt_4 = 800.0,
        disrupt_3 = 100.0,
        disrupt_2 = 20.0,
        useful_after = 50.0
    )))
    run_tournament(bots, 1000)
}
