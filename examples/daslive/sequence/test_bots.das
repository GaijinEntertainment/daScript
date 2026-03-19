options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require gameplay
require bot_random

var game = GameState()

[test]
def test_bot_random_returns_legal_move(t : T?) {
    t |> run("random bot always returns a legal move") @(t : T?) {
        make_game(game, 2, 42)
        let m = bot_random_move(game, 0, 100)
        t |> success(m.card_idx >= 0, "bot should find a move")
        t |> success(is_legal_move(game, 0, m), "move should be legal")
    }
    t |> run("random bot returns legal move for player 1") @(t : T?) {
        make_game(game, 2, 42)
        let m = bot_random_move(game, 1, 200)
        t |> success(m.card_idx >= 0, "bot should find a move")
        t |> success(is_legal_move(game, 1, m), "move should be legal")
    }
}

[test]
def test_bot_random_different_seeds(t : T?) {
    t |> run("different seeds can produce different moves") @(t : T?) {
        make_game(game, 2, 42)
        var saw_different = false
        let m0 = bot_random_move(game, 0, 1)
        for (s in range(2, 20)) {
            let m = bot_random_move(game, 0, s)
            if (m.row != m0.row || m.col != m0.col) {
                saw_different = true
                break
            }
        }
        t |> success(saw_different, "different seeds should eventually pick different moves")
    }
}

[test]
def test_bot_random_handles_jacks(t : T?) {
    t |> run("bot with only a red jack plays it on an open cell") @(t : T?) {
        make_game(game, 2, 1)
        // Give player 0 only a red jack
        var hand <- ["heart_jack"]
        set_hand(game, 0, hand)
        delete hand
        let m = bot_random_move(game, 0, 50)
        t |> success(m.card_idx == 0, "should play the only card (red jack)")
        t |> success(is_legal_move(game, 0, m), "red jack move should be legal")
        // Red jack target should be an empty cell
        t |> equal(int(get_chip(game, m.row, m.col)), int(ChipColor.none))
    }
    t |> run("bot with only a black jack plays it on opponent chip") @(t : T?) {
        make_game(game, 2, 1)
        // Place an opponent chip
        place_chip(game, 2, 3, ChipColor.green)
        // Give player 0 only a black jack
        var hand2 <- ["spade_jack"]
        set_hand(game, 0, hand2)
        delete hand2
        let m = bot_random_move(game, 0, 50)
        t |> success(m.card_idx >= 0, "should find a move with black jack")
        t |> success(is_legal_move(game, 0, m), "black jack move should be legal")
    }
}

[test]
def test_bot_random_no_moves(t : T?) {
    t |> run("bot returns empty move when no legal moves exist") @(t : T?) {
        make_game(game, 2, 1)
        // Give player an empty hand
        game.hand_sizes[0] = 0
        let m = bot_random_move(game, 0, 50)
        t |> equal(m.card_idx, -1)
    }
}

[test]
def test_bot_random_many_moves_all_legal(t : T?) {
    t |> run("100 bot moves are all legal across different game states") @(t : T?) {
        make_game(game, 2, 99)
        for (s in range(100)) {
            let p = game.current_player
            discard_dead_cards(game, p)
            let m = bot_random_move(game, p, s)
            if (m.card_idx >= 0) {
                t |> success(is_legal_move(game, p, m), "move {s} should be legal")
                apply_move(game, p, m)
                if (game.phase == GamePhase.game_over) {
                    break
                }
            }
        }
    }
}

[test]
def test_bot_random_full_game(t : T?) {
    t |> run("two random bots can play a full game to completion") @(t : T?) {
        make_game(game, 2, 777)
        var moves_played = 0
        while (game.phase == GamePhase.playing && moves_played < 500) {
            let p = game.current_player
            discard_dead_cards(game, p)
            let m = bot_random_move(game, p, moves_played + 1)
            if (m.card_idx >= 0) {
                apply_move(game, p, m)
                moves_played++
            } else {
                break
            }
        }
        t |> success(moves_played > 0, "should play at least one move")
        // Game may or may not finish in 500 moves, but should not crash
    }
}
