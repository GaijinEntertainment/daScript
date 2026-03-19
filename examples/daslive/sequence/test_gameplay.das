options gen2
require dastest/testing_boost public
require gameplay

// ============================================================================
// Phase 1: Board layout tests
// ============================================================================

[test]
def test_board_dimensions(t : T?) {
    t |> equal(BOARD_ROWS, 10)
    t |> equal(BOARD_COLS, 10)
    t |> equal(BOARD_SIZE, 100)
}

[test]
def test_free_corners(t : T?) {
    t |> run("four corners are FREE") <| @(t : T?) {
        t |> success(is_free_space(0, 0), "top-left should be FREE")
        t |> success(is_free_space(0, 9), "top-right should be FREE")
        t |> success(is_free_space(9, 0), "bottom-left should be FREE")
        t |> success(is_free_space(9, 9), "bottom-right should be FREE")
    }
    t |> run("corners detected by is_corner") <| @(t : T?) {
        t |> success(is_corner(0, 0))
        t |> success(is_corner(0, 9))
        t |> success(is_corner(9, 0))
        t |> success(is_corner(9, 9))
        t |> success(!is_corner(0, 1))
        t |> success(!is_corner(5, 5))
    }
}

[test]
def test_exactly_96_non_free_cells(t : T?) {
    var count = 0
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        if (!is_free_space(row, col)) {
            count++
        }
    }
    t |> equal(count, 96)
}

[test]
def test_each_card_appears_twice(t : T?) {
    var counts : table<string; int>
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let name = board_card_name(row, col)
        if (name != FREE) {
            if (key_exists(counts, name)) {
                counts[name] += 1
            } else {
                counts[name] = 1
            }
        }
    }
    t |> equal(length(counts), 48)
    for (name, count in keys(counts), values(counts)) {
        t |> equal(count, 2)
    }
}

[test]
def test_no_jacks_on_board(t : T?) {
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let name = board_card_name(row, col)
        t |> success(!is_jack(name), "no jacks on board at ({row},{col})")
    }
}

[test]
def test_all_card_names_valid(t : T?) {
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let name = board_card_name(row, col)
        if (name != FREE) {
            t |> success(is_valid_card_name(name), "valid card name at ({row},{col}): {name}")
        }
    }
}

[test]
def test_jack_helpers(t : T?) {
    t |> run("black jacks are removers") <| @(t : T?) {
        t |> success(is_black_jack("spade_jack"))
        t |> success(is_black_jack("club_jack"))
        t |> success(!is_black_jack("heart_jack"))
        t |> success(!is_black_jack("diamond_jack"))
    }
    t |> run("red jacks are wild") <| @(t : T?) {
        t |> success(is_red_jack("heart_jack"))
        t |> success(is_red_jack("diamond_jack"))
        t |> success(!is_red_jack("spade_jack"))
        t |> success(!is_red_jack("club_jack"))
    }
    t |> run("is_jack covers all four") <| @(t : T?) {
        t |> success(is_jack("spade_jack"))
        t |> success(is_jack("club_jack"))
        t |> success(is_jack("heart_jack"))
        t |> success(is_jack("diamond_jack"))
        t |> success(!is_jack("spade_1"))
        t |> success(!is_jack("heart_queen"))
    }
}

[test]
def test_board_card_name_bounds(t : T?) {
    t |> equal(board_card_name(-1, 0), "")
    t |> equal(board_card_name(0, -1), "")
    t |> equal(board_card_name(10, 0), "")
    t |> equal(board_card_name(0, 10), "")
    // Valid cell should return non-empty (row 0, col 1 = club_1)
    t |> equal(board_card_name(0, 1), "club_1")
}

// ============================================================================
// Phase 2: Chip state tests
// ============================================================================

[test]
def test_chip_placement(t : T?) {
    clear_all_chips()
    t |> run("place and read back") <| @(t : T?) {
        t |> success(place_chip(3, 4, ChipColor.blue))
        t |> equal(int(get_chip(3, 4)), int(ChipColor.blue))
        t |> success(place_chip(3, 4, ChipColor.red))
        t |> equal(int(get_chip(3, 4)), int(ChipColor.red))
    }
    t |> run("clear restores none") <| @(t : T?) {
        place_chip(5, 5, ChipColor.green)
        clear_all_chips()
        t |> equal(int(get_chip(5, 5)), int(ChipColor.none))
    }
    t |> run("out of bounds returns none") <| @(t : T?) {
        t |> equal(int(get_chip(-1, 0)), int(ChipColor.none))
        t |> equal(int(get_chip(0, 10)), int(ChipColor.none))
        t |> equal(int(get_chip(10, 5)), int(ChipColor.none))
    }
    t |> run("out of bounds place returns false") <| @(t : T?) {
        t |> success(!place_chip(-1, 0, ChipColor.blue))
        t |> success(!place_chip(10, 0, ChipColor.blue))
        t |> success(!place_chip(0, -1, ChipColor.blue))
    }
    clear_all_chips()
}

[test]
def test_chip_color_cycle(t : T?) {
    t |> equal(int(next_chip_color(ChipColor.none)), int(ChipColor.blue))
    t |> equal(int(next_chip_color(ChipColor.blue)), int(ChipColor.green))
    t |> equal(int(next_chip_color(ChipColor.green)), int(ChipColor.red))
    t |> equal(int(next_chip_color(ChipColor.red)), int(ChipColor.yellow))
    t |> equal(int(next_chip_color(ChipColor.yellow)), int(ChipColor.none))
}

[test]
def test_chip_colors_have_alpha(t : T?) {
    t |> run("all chip colors have nonzero alpha") <| @(t : T?) {
        t |> success(chip_color_to_float4(ChipColor.blue).w > 0.0)
        t |> success(chip_color_to_float4(ChipColor.green).w > 0.0)
        t |> success(chip_color_to_float4(ChipColor.red).w > 0.0)
        t |> success(chip_color_to_float4(ChipColor.yellow).w > 0.0)
    }
}

// ============================================================================
// Phase 3: Game state tests
// ============================================================================

[test]
def test_double_deck(t : T?) {
    var deck <- build_double_deck()
    t |> equal(length(deck), 104)
    // Count each card — should appear exactly twice
    var counts : table<string; int>
    for (card in deck) {
        if (key_exists(counts, card)) {
            counts[card] += 1
        } else {
            counts[card] = 1
        }
    }
    t |> equal(length(counts), 52)
    for (name, count in keys(counts), values(counts)) {
        t |> equal(count, 2)
    }
}

[test]
def test_make_game_hand_sizes(t : T?) {
    t |> run("2 players get 7 cards each") <| @(t : T?) {
        make_game(2)
        t |> equal(get_hand_size(0), 7)
        t |> equal(get_hand_size(1), 7)
    }
    t |> run("3 players get 6 cards each") <| @(t : T?) {
        make_game(3)
        t |> equal(get_hand_size(0), 6)
        t |> equal(get_hand_size(1), 6)
        t |> equal(get_hand_size(2), 6)
    }
    t |> run("4 players get 5 cards each") <| @(t : T?) {
        make_game(4)
        t |> equal(get_hand_size(0), 5)
        t |> equal(get_hand_size(1), 5)
        t |> equal(get_hand_size(2), 5)
        t |> equal(get_hand_size(3), 5)
    }
}

[test]
def test_make_game_draw_pile(t : T?) {
    make_game(2)
    // 104 cards - 14 dealt (2 players x 7) = 90 remaining
    t |> equal(length(game.draw_pile), 90)
    make_game(4)
    // 104 cards - 20 dealt (4 players x 5) = 84 remaining
    t |> equal(length(game.draw_pile), 84)
}

[test]
def test_make_game_state(t : T?) {
    make_game(2)
    t |> equal(game.num_players, 2)
    t |> equal(game.current_player, 0)
    t |> equal(int(game.phase), int(GamePhase.playing))
    t |> equal(game.winner, -1)
}

[test]
def test_all_dealt_cards_valid(t : T?) {
    make_game(4)
    for (p in range(4)) {
        for (c in range(get_hand_size(p))) {
            let card = get_hand_card(p, c)
            t |> success(is_valid_card_name(card), "player {p} card {c}: {card}")
        }
    }
}

[test]
def test_shuffle_is_deterministic(t : T?) {
    make_game(2, 123)
    let card_a = get_hand_card(0, 0)
    make_game(2, 123)
    let card_b = get_hand_card(0, 0)
    t |> equal(card_a, card_b)
}

[test]
def test_shuffle_different_seeds(t : T?) {
    make_game(2, 1)
    var hand1 : array<string>
    for (c in range(get_hand_size(0))) {
        hand1 |> push(get_hand_card(0, c))
    }
    make_game(2, 999)
    var different = false
    for (c in range(get_hand_size(0))) {
        if (get_hand_card(0, c) != hand1[c]) {
            different = true
        }
    }
    t |> success(different, "different seeds should produce different hands")
}

[test]
def test_card_suit_rank(t : T?) {
    t |> equal(card_suit("heart_queen"), "heart")
    t |> equal(card_rank("heart_queen"), "queen")
    t |> equal(card_suit("club_10"), "club")
    t |> equal(card_rank("club_10"), "10")
    t |> equal(card_suit("spade_1"), "spade")
    t |> equal(card_rank("spade_1"), "1")
}

[test]
def test_sort_hand(t : T?) {
    make_game(2, 42)
    sort_hand(0)
    // Verify sorted: suits in order, ranks in order within suit
    var prev_suit = -1
    var prev_rank = -1
    for (c in range(get_hand_size(0))) {
        let card = get_hand_card(0, c)
        let s = suit_order(card_suit(card))
        let r = rank_order(card_rank(card))
        if (s == prev_suit) {
            t |> success(r >= prev_rank, "rank should increase within suit: {card}")
        } else {
            t |> success(s > prev_suit, "suit should increase: {card}")
        }
        prev_suit = s
        prev_rank = r
    }
}

[test]
def test_legal_moves_normal_card(t : T?) {
    make_game(2, 42)
    let moves <- legal_moves(0)
    t |> success(length(moves) > 0, "player 0 should have legal moves")
    // Each move should target an empty board cell matching the card
    for (m in moves) {
        let card = get_hand_card(0, m.card_idx)
        if (!is_jack(card)) {
            let board_name = board_card_name(m.row, m.col)
            t |> equal(card, board_name)
            t |> equal(int(get_chip(m.row, m.col)), int(ChipColor.none))
        }
    }
}

[test]
def test_apply_move(t : T?) {
    make_game(2, 42)
    let moves <- legal_moves(0)
    t |> success(length(moves) > 0)
    let m = moves[0]
    let card = get_hand_card(0, m.card_idx)
    t |> success(apply_move(0, m), "move should succeed")
    // Chip placed (or removed for black jack)
    if (!is_black_jack(card)) {
        t |> equal(int(get_chip(m.row, m.col)), int(ChipColor.blue))
    }
    // Turn advanced
    t |> equal(game.current_player, 1)
}

[test]
def test_apply_move_wrong_player(t : T?) {
    make_game(2, 42)
    let moves <- legal_moves(0)
    t |> success(length(moves) > 0)
    // Player 1 can't move on player 0's turn
    t |> success(!apply_move(1, moves[0]), "wrong player should fail")
}

[test]
def test_dead_card_detection(t : T?) {
    clear_all_chips()
    // club_1 appears at (0,1) and (5,2) on the board
    t |> success(!is_dead_card("club_1"), "not dead when empty")
    place_chip(0, 1, ChipColor.blue)
    t |> success(!is_dead_card("club_1"), "not dead when one occupied")
    place_chip(5, 2, ChipColor.green)
    t |> success(is_dead_card("club_1"), "dead when both occupied")
    clear_all_chips()
}

[test]
def test_jacks_never_dead(t : T?) {
    t |> success(!is_dead_card("club_jack"))
    t |> success(!is_dead_card("heart_jack"))
}

[test]
def test_player_chip_colors(t : T?) {
    t |> equal(int(player_chip_color(0)), int(ChipColor.blue))
    t |> equal(int(player_chip_color(1)), int(ChipColor.green))
    t |> equal(int(player_chip_color(2)), int(ChipColor.red))
    t |> equal(int(player_chip_color(3)), int(ChipColor.yellow))
}

[test]
def test_full_game_simulation(t : T?) {
    // Play a full game with random legal moves to ensure no crashes
    make_game(2, 77)
    var turns = 0
    while (game.phase == GamePhase.playing && turns < 200) {
        let p = game.current_player
        discard_dead_cards(p)
        let moves <- legal_moves(p)
        if (length(moves) == 0) {
            break
        }
        let m = moves[turns % length(moves)]
        apply_move(p, m)
        turns++
    }
    t |> success(turns > 0, "should play at least one turn")
}
