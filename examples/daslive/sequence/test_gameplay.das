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
