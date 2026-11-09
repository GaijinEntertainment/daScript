options gen2
require dastest/testing_boost public
require gameplay

var game = GameState()

// ============================================================================
// Phase 1: Board layout tests
// ============================================================================

[test(description = "The board is 10 by 10, 100 cells.")]
def test_board_dimensions(t : T?) {
    t |> equal(BOARD_ROWS, 10)
    t |> equal(BOARD_COLS, 10)
    t |> equal(BOARD_SIZE, 100)
}

[test(description = "The four corners are FREE spaces and is_corner is true for them and false elsewhere.")]
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

[test(description = "Exactly 96 board cells are not FREE.")]
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

[test(description = "The board shows 48 distinct cards and each appears exactly twice.")]
def test_each_card_appears_twice(t : T?) {
    var counts : table<int; int>
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let card = board_card(row, col)
        if (card != Card.card_none) {
            let k = int(card)
            if (key_exists(counts, k)) {
                counts[k]++
            } else {
                counts[k] = 1
            }
        }
    }
    t |> equal(length(counts), 48)
    for (_k, count in keys(counts), values(counts)) {
        t |> equal(count, 2)
    }
}

[test(description = "No board cell shows a jack.")]
def test_no_jacks_on_board(t : T?) {
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let card = board_card(row, col)
        t |> success(!is_jack(card), "no jacks on board at ({row},{col})")
    }
}

[test(description = "Every cell except the corners shows a card.")]
def test_all_board_cards_valid(t : T?) {
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let card = board_card(row, col)
        if (!is_corner(row, col)) {
            t |> success(card != Card.card_none, "valid card at ({row},{col})")
        }
    }
}

[test(description = "Spades and clubs jacks are black removers, hearts and diamonds jacks are red wilds, and is_jack is true for all four and no other card.")]
def test_jack_helpers(t : T?) {
    t |> run("black jacks are removers") <| @(t : T?) {
        t |> success(is_black_jack(Card.spade_jack))
        t |> success(is_black_jack(Card.club_jack))
        t |> success(!is_black_jack(Card.heart_jack))
        t |> success(!is_black_jack(Card.diamond_jack))
    }
    t |> run("red jacks are wild") <| @(t : T?) {
        t |> success(is_red_jack(Card.heart_jack))
        t |> success(is_red_jack(Card.diamond_jack))
        t |> success(!is_red_jack(Card.spade_jack))
        t |> success(!is_red_jack(Card.club_jack))
    }
    t |> run("is_jack covers all four") <| @(t : T?) {
        t |> success(is_jack(Card.spade_jack))
        t |> success(is_jack(Card.club_jack))
        t |> success(is_jack(Card.heart_jack))
        t |> success(is_jack(Card.diamond_jack))
        t |> success(!is_jack(Card.spade_1))
        t |> success(!is_jack(Card.heart_queen))
    }
}

[test(description = "board_card answers no card outside the 10x10 board and club_1 at row 0, column 1.")]
def test_board_card_bounds(t : T?) {
    t |> equal(int(board_card(-1, 0)), int(Card.card_none))
    t |> equal(int(board_card(0, -1)), int(Card.card_none))
    t |> equal(int(board_card(10, 0)), int(Card.card_none))
    t |> equal(int(board_card(0, 10)), int(Card.card_none))
    t |> equal(int(board_card(0, 1)), int(Card.club_1))
}

[test(description = "card_to_string gives the enum name of a card and an empty string for no card.")]
def test_card_to_string(t : T?) {
    t |> equal(card_to_string(Card.heart_queen), "heart_queen")
    t |> equal(card_to_string(Card.club_10), "club_10")
    t |> equal(card_to_string(Card.spade_1), "spade_1")
    t |> equal(card_to_string(Card.card_none), "")
}

[test(description = "Suit and rank indices count clubs 0 through spades 3 and ace 0 through king 12.")]
def test_card_suit_rank_idx(t : T?) {
    t |> equal(card_suit_idx(Card.heart_queen), 2)  // heart = 2
    t |> equal(card_rank_idx(Card.heart_queen), 11) // queen = 11
    t |> equal(card_suit_idx(Card.club_1), 0)       // club = 0
    t |> equal(card_rank_idx(Card.club_1), 0)       // ace = 0
    t |> equal(card_suit_idx(Card.spade_king), 3)   // spade = 3
    t |> equal(card_rank_idx(Card.spade_king), 12)  // king = 12
}

// ============================================================================
// Phase 2: Chip state tests
// ============================================================================

[test(description = "A placed chip reads back and can be overwritten, clearing restores no chip, and reads or places outside the board answer none or false.")]
def test_chip_placement(t : T?) {
    clear_all_chips(game)
    t |> run("place and read back") <| @(t : T?) {
        t |> success(place_chip(game, 3, 4, ChipColor.blue))
        t |> equal(int(get_chip(game, 3, 4)), int(ChipColor.blue))
        t |> success(place_chip(game, 3, 4, ChipColor.red))
        t |> equal(int(get_chip(game, 3, 4)), int(ChipColor.red))
    }
    t |> run("clear restores none") <| @(t : T?) {
        place_chip(game, 5, 5, ChipColor.green)
        clear_all_chips(game)
        t |> equal(int(get_chip(game, 5, 5)), int(ChipColor.none))
    }
    t |> run("out of bounds returns none") <| @(t : T?) {
        t |> equal(int(get_chip(game, -1, 0)), int(ChipColor.none))
        t |> equal(int(get_chip(game, 0, 10)), int(ChipColor.none))
        t |> equal(int(get_chip(game, 10, 5)), int(ChipColor.none))
    }
    t |> run("out of bounds place returns false") <| @(t : T?) {
        t |> success(!place_chip(game, -1, 0, ChipColor.blue))
        t |> success(!place_chip(game, 10, 0, ChipColor.blue))
        t |> success(!place_chip(game, 0, -1, ChipColor.blue))
    }
    clear_all_chips(game)
}

[test(description = "next_chip_color cycles none, blue, green, red, yellow and back to none.")]
def test_chip_color_cycle(t : T?) {
    t |> equal(int(next_chip_color(ChipColor.none)), int(ChipColor.blue))
    t |> equal(int(next_chip_color(ChipColor.blue)), int(ChipColor.green))
    t |> equal(int(next_chip_color(ChipColor.green)), int(ChipColor.red))
    t |> equal(int(next_chip_color(ChipColor.red)), int(ChipColor.yellow))
    t |> equal(int(next_chip_color(ChipColor.yellow)), int(ChipColor.none))
}

[test(description = "Every chip color converts to a visible color with nonzero alpha.")]
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

[test(description = "The double deck holds 104 cards: each of the 52 cards twice.")]
def test_double_deck(t : T?) {
    var deck <- build_double_deck()
    t |> equal(length(deck), 104)
    var counts : table<int; int>
    for (card in deck) {
        let k = int(card)
        if (key_exists(counts, k)) {
            counts[k]++
        } else {
            counts[k] = 1
        }
    }
    t |> equal(length(counts), 52)
    for (_k, count in keys(counts), values(counts)) {
        t |> equal(count, 2)
    }
}

[test(description = "A new game deals 7 cards each to 2 players, 6 to 3 players and 5 to 4 players.")]
def test_make_game_hand_sizes(t : T?) {
    t |> run("2 players get 7 cards each") <| @(t : T?) {
        make_game(game, 2)
        t |> equal(get_hand_size(game, 0), 7)
        t |> equal(get_hand_size(game, 1), 7)
    }
    t |> run("3 players get 6 cards each") <| @(t : T?) {
        make_game(game, 3)
        t |> equal(get_hand_size(game, 0), 6)
        t |> equal(get_hand_size(game, 1), 6)
        t |> equal(get_hand_size(game, 2), 6)
    }
    t |> run("4 players get 5 cards each") <| @(t : T?) {
        make_game(game, 4)
        t |> equal(get_hand_size(game, 0), 5)
        t |> equal(get_hand_size(game, 1), 5)
        t |> equal(get_hand_size(game, 2), 5)
        t |> equal(get_hand_size(game, 3), 5)
    }
}

[test(description = "After the deal the draw pile holds the rest of the 104 cards: 90 for 2 players and 84 for 4.")]
def test_make_game_draw_pile(t : T?) {
    make_game(game, 2)
    t |> equal(length(game.draw_pile), 90)
    make_game(game, 4)
    t |> equal(length(game.draw_pile), 84)
}

[test(description = "A new game starts in the playing phase with player 0 to move and no winner.")]
def test_make_game_state(t : T?) {
    make_game(game, 2)
    t |> equal(game.num_players, 2)
    t |> equal(game.current_player, 0)
    t |> equal(int(game.phase), int(GamePhase.playing))
    t |> equal(game.winner, -1)
}

[test(description = "Every card dealt to four players is a real card.")]
def test_all_dealt_cards_valid(t : T?) {
    make_game(game, 4)
    for (p in range(4)) {
        for (c in range(get_hand_size(game, p))) {
            let card = get_hand_card(game, p, c)
            t |> success(card != Card.card_none, "player {p} card {c} should be valid")
        }
    }
}

[test(description = "The same shuffle seed deals the same first card.")]
def test_shuffle_is_deterministic(t : T?) {
    make_game(game, 2, 123)
    let card_a = get_hand_card(game, 0, 0)
    make_game(game, 2, 123)
    let card_b = get_hand_card(game, 0, 0)
    t |> equal(int(card_a), int(card_b))
}

[test(description = "Two different shuffle seeds deal player 0 a different hand.")]
def test_shuffle_different_seeds(t : T?) {
    make_game(game, 2, 1)
    var hand1 : array<int>
    hand1 |> reserve(get_hand_size(game, 0))
    for (c in range(get_hand_size(game, 0))) {
        hand1 |> push(int(get_hand_card(game, 0, c)))
    }
    make_game(game, 2, 999)
    var different = false
    for (c in range(get_hand_size(game, 0))) {
        if (int(get_hand_card(game, 0, c)) != hand1[c]) {
            different = true
        }
    }
    t |> success(different, "different seeds should produce different hands")
}

[test(description = "Suit and rank indices are right for a queen, a ten and an ace: ranks count from ace 0, so the ten is 9.")]
def test_card_suit_rank(t : T?) {
    t |> equal(card_suit_idx(Card.heart_queen), 2)  // heart
    t |> equal(card_rank_idx(Card.heart_queen), 11) // queen
    t |> equal(card_suit_idx(Card.club_10), 0)      // club
    t |> equal(card_rank_idx(Card.club_10), 9)      // 10 (0-indexed: ace=0..10=9)
    t |> equal(card_suit_idx(Card.spade_1), 3)      // spade
    t |> equal(card_rank_idx(Card.spade_1), 0)      // ace
}

[test(description = "sort_hand orders a hand by card enum value, suit first then rank.")]
def test_sort_hand(t : T?) {
    make_game(game, 2, 42)
    sort_hand(game, 0)
    // Sorted by enum value = suit-then-rank order
    var prev_val = 0
    for (c in range(get_hand_size(game, 0))) {
        let card = get_hand_card(game, 0, c)
        let val = int(card)
        t |> success(val >= prev_val, "hand should be sorted")
        prev_val = val
    }
}

[test(description = "Every legal move for a non-jack card targets an empty cell that shows that same card.")]
def test_legal_moves_normal_card(t : T?) {
    make_game(game, 2, 42)
    let moves <- legal_moves(game, 0)
    t |> success(!empty(moves), "player 0 should have legal moves")
    for (m in moves) {
        let card = get_hand_card(game, 0, m.card_idx)
        if (!is_jack(card)) {
            let bc = board_card(m.row, m.col)
            t |> equal(int(card), int(bc))
            t |> equal(int(get_chip(game, m.row, m.col)), int(ChipColor.none))
        }
    }
}

[test(description = "Applying a legal move puts the mover's chip on the cell, unless the card is a black jack, and passes the turn to the next player.")]
def test_apply_move(t : T?) {
    make_game(game, 2, 42)
    let moves <- legal_moves(game, 0)
    t |> success(!empty(moves))
    let m = moves[0]
    let card = get_hand_card(game, 0, m.card_idx)
    t |> success(apply_move(game, 0, m), "move should succeed")
    if (!is_black_jack(card)) {
        t |> equal(int(get_chip(game, m.row, m.col)), int(ChipColor.blue))
    }
    t |> equal(game.current_player, 1)
}

[test(description = "A move applied for a player whose turn it is not is refused.")]
def test_apply_move_wrong_player(t : T?) {
    make_game(game, 2, 42)
    let moves <- legal_moves(game, 0)
    t |> success(!empty(moves))
    t |> success(!apply_move(game, 1, moves[0]), "wrong player should fail")
}

[test(description = "A card is dead only when both of its board cells are taken.")]
def test_dead_card_detection(t : T?) {
    clear_all_chips(game)
    // club_1 appears at (0,1) and (5,2) on the board
    t |> success(!is_dead_card(game, Card.club_1), "not dead when empty")
    place_chip(game, 0, 1, ChipColor.blue)
    t |> success(!is_dead_card(game, Card.club_1), "not dead when one occupied")
    place_chip(game, 5, 2, ChipColor.green)
    t |> success(is_dead_card(game, Card.club_1), "dead when both occupied")
    clear_all_chips(game)
}

[test(description = "A jack is never a dead card.")]
def test_jacks_never_dead(t : T?) {
    t |> success(!is_dead_card(game, Card.club_jack))
    t |> success(!is_dead_card(game, Card.heart_jack))
}

[test(description = "Players 0 to 3 play blue, green, red and yellow chips.")]
def test_player_chip_colors(t : T?) {
    t |> equal(int(player_chip_color(0)), int(ChipColor.blue))
    t |> equal(int(player_chip_color(1)), int(ChipColor.green))
    t |> equal(int(player_chip_color(2)), int(ChipColor.red))
    t |> equal(int(player_chip_color(3)), int(ChipColor.yellow))
}

[test(description = "A two-player game played from the legal move list runs for up to 200 turns without getting stuck before the first turn.")]
def test_full_game_simulation(t : T?) {
    make_game(game, 2, 77)
    var turns = 0
    while (game.phase == GamePhase.playing && turns < 200) {
        let p = game.current_player
        discard_dead_cards(game, p)
        var inscope moves <- legal_moves(game, p)
        if (empty(moves)) {
            break
        }
        let m = moves[turns % length(moves)]
        apply_move(game, p, m)
        turns++
    }
    t |> success(turns > 0, "should play at least one turn")
}

[test(description = "Every move from legal_moves passes is_legal_move, while a normal card on a cell showing another card, a move off the board, and a normal card on a taken cell are all illegal.")]
def test_is_legal_move(t : T?) {
    make_game(game, 2, 42)
    let moves <- legal_moves(game, 0)
    t |> success(!empty(moves))
    for (m in moves) {
        t |> success(is_legal_move(game, 0, m), "legal_moves entry should be legal")
    }
    t |> run("normal card on wrong cell") <| @(t : T?) {
        make_game(game, 2, 42)
        let card = get_hand_card(game, 0, 0)
        if (!is_jack(card)) {
            for (i in range(BOARD_SIZE)) {
                let row = i / BOARD_COLS
                let col = i % BOARD_COLS
                let bc = board_card(row, col)
                if (bc != card && bc != Card.card_none) {
                    t |> success(!is_legal_move(game, 0, Move(card_idx = 0, row = row, col = col)), "wrong cell should be illegal")
                    break
                }
            }
        }
    }
    t |> run("out of bounds is illegal") <| @(t : T?) {
        make_game(game, 2, 42)
        t |> success(!is_legal_move(game, 0, Move(card_idx = 0, row = -1, col = 0)))
        t |> success(!is_legal_move(game, 0, Move(card_idx = 0, row = 10, col = 0)))
        t |> success(!is_legal_move(game, 0, Move(card_idx = -1, row = 0, col = 0)))
    }
    t |> run("occupied cell is illegal for normal card") <| @(t : T?) {
        make_game(game, 2, 42)
        let moves <- legal_moves(game, 0)
        if (!empty(moves)) {
            let m = moves[0]
            let card = get_hand_card(game, 0, m.card_idx)
            if (!is_jack(card)) {
                place_chip(game, m.row, m.col, ChipColor.blue)
                t |> success(!is_legal_move(game, 0, m), "occupied cell should be illegal")
                clear_all_chips(game)
            }
        }
    }
}

// ============================================================================
// Phase 4: Sequence detection and win condition tests
// ============================================================================

def place_horizontal(var game : GameState; row, start_col : int; color : ChipColor) {
    for (c in range(5)) {
        place_chip(game, row, start_col + c, color)
    }
}

def place_vertical(var game : GameState; start_row, col : int; color : ChipColor) {
    for (r in range(5)) {
        place_chip(game, start_row + r, col, color)
    }
}

[test(description = "Five chips of one color in a row are found as a sequence.")]
def test_horizontal_sequence(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    place_horizontal(game, 1, 1, ChipColor.blue)
    find_sequences(game)
    let n = count_sequences(game, ChipColor.blue)
    t |> success(n >= 1, "should find at least 1 horizontal sequence, found {n}")
    clear_all_chips(game)
}

[test(description = "Five chips of one color in a column are found as a sequence.")]
def test_vertical_sequence(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    place_vertical(game, 1, 1, ChipColor.blue)
    find_sequences(game)
    let n = count_sequences(game, ChipColor.blue)
    t |> success(n >= 1, "should find at least 1 vertical sequence, found {n}")
    clear_all_chips(game)
}

[test(description = "Five chips of one color on a down-right diagonal are found as a sequence.")]
def test_diagonal_sequence(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    for (i in range(5)) {
        place_chip(game, 1 + i, 1 + i, ChipColor.blue)
    }
    find_sequences(game)
    let n = count_sequences(game, ChipColor.blue)
    t |> success(n >= 1, "should find at least 1 diagonal sequence, found {n}")
    clear_all_chips(game)
}

[test(description = "Five chips of one color on a down-left diagonal are found as a sequence.")]
def test_anti_diagonal_sequence(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    for (i in range(5)) {
        place_chip(game, 1 + i, 5 - i, ChipColor.blue)
    }
    find_sequences(game)
    let n = count_sequences(game, ChipColor.blue)
    t |> success(n >= 1, "should find at least 1 anti-diagonal sequence, found {n}")
    clear_all_chips(game)
}

[test(description = "A FREE corner counts as a chip of any color: four chips next to it complete a sequence for blue and for green.")]
def test_free_corner_in_sequence(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    for (c in range(1, 5)) {
        place_chip(game, 0, c, ChipColor.blue)
    }
    find_sequences(game)
    let n = count_sequences(game, ChipColor.blue)
    t |> success(n >= 1, "FREE corner should complete horizontal sequence, found {n}")
    clear_all_chips(game)
    for (c in range(1, 5)) {
        place_chip(game, 0, c, ChipColor.green)
    }
    find_sequences(game)
    let ng = count_sequences(game, ChipColor.green)
    t |> success(ng >= 1, "FREE corner should work for green too, found {ng}")
    clear_all_chips(game)
}

[test(description = "Four chips in a row are not a sequence.")]
def test_no_sequence_with_4(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    for (c in range(4)) {
        place_chip(game, 3, 1 + c, ChipColor.blue)
    }
    find_sequences(game)
    t |> equal(count_sequences(game, ChipColor.blue), 0)
    clear_all_chips(game)
}

[test(description = "Nine chips in a row form two sequences, which share one cell.")]
def test_overlapping_sequences_share_one_cell(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    for (c in range(1, 10)) {
        place_chip(game, 3, c, ChipColor.blue)
    }
    find_sequences(game)
    let n = count_sequences(game, ChipColor.blue)
    t |> success(n >= 2, "9 in a row should form 2+ sequences, found {n}")
    clear_all_chips(game)
}

[test(description = "With two players the winner needs two sequences: one is not a win and a second one wins for player 0.")]
def test_win_condition_2_players(t : T?) {
    make_game(game, 2)
    clear_all_chips(game)
    t |> equal(sequences_to_win(2), 2)
    place_horizontal(game, 1, 1, ChipColor.blue)
    find_sequences(game)
    t |> equal(check_win(game), -1)
    place_horizontal(game, 3, 1, ChipColor.blue)
    find_sequences(game)
    t |> equal(check_win(game), 0)
    clear_all_chips(game)
}

[test(description = "With three players one sequence wins: a green sequence wins for player 1.")]
def test_win_condition_3_players(t : T?) {
    make_game(game, 3)
    clear_all_chips(game)
    t |> equal(sequences_to_win(3), 1)
    place_horizontal(game, 1, 1, ChipColor.green)
    find_sequences(game)
    t |> equal(check_win(game), 1)
    clear_all_chips(game)
}

[test(description = "A chip that is part of a finished sequence cannot be removed.")]
def test_black_jack_cant_remove_from_sequence(t : T?) {
    make_game(game, 2, 42)
    clear_all_chips(game)
    place_horizontal(game, 1, 1, ChipColor.green)
    find_sequences(game)
    t |> success(count_sequences(game, ChipColor.green) >= 1)
    for (c in range(1, 6)) {
        t |> success(!is_in_sequence(game, 1, c) || !is_legal_move(game, 0, Move(card_idx = 0, row = 1, col = c)),
            "should not be able to remove from completed sequence at col {c}")
    }
    clear_all_chips(game)
}

[test(description = "The move that completes a winning sequence ends the game with the mover as the winner.")]
def test_apply_move_triggers_win(t : T?) {
    make_game(game, 3, 42)
    clear_all_chips(game)
    for (c in range(4)) {
        place_chip(game, 2, 1 + c, ChipColor.blue)
    }
    let target_card = board_card(2, 5)
    var cards : array<Card>
    cards |> reserve(6)
    cards |> push(target_card)
    for (_i in range(1, 6)) {
        cards |> push(Card.club_1)
    }
    set_hand(game, 0, cards)
    game.current_player = 0
    game.phase = GamePhase.playing
    let m = Move(card_idx = 0, row = 2, col = 5)
    let ok = apply_move(game, 0, m)
    t |> success(ok, "move should succeed")
    t |> equal(int(game.phase), int(GamePhase.game_over))
    t |> equal(game.winner, 0)
    clear_all_chips(game)
}
