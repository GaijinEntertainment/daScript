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
