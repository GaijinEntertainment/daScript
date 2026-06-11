options gen2

require elo

[export]
def main() {
    var bots <- [make_random_bot("random"), make_greedy_bot("greedy"), make_lookahead_bot("lookahead")]
    run_tournament(bots, 200)
}
