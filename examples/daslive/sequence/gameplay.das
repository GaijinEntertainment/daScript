options gen2
options persistent_heap

module gameplay public

require strings

// ============================================================================
// Board layout — original Sequence board card order
// ============================================================================

// Card naming: "{suit}_{rank}" where suit is club/diamond/heart/spade,
// rank is 1 (ace), 2-10, jack, queen, king. Empty string "" = FREE corner.
// No jacks appear on the board (they are action-only cards).

let FREE = ""

let BOARD_LAYOUT = fixed_array(
    //  col 0            col 1            col 2            col 3            col 4            col 5            col 6            col 7            col 8            col 9
    // Row 0
    FREE,            "club_1",        "club_king",     "club_queen",    "club_10",       "club_9",        "club_8",        "club_7",        "club_6",        FREE,
    // Row 1
    "diamond_1",     "spade_7",       "spade_8",       "spade_9",       "spade_10",      "spade_queen",   "spade_king",    "spade_1",       "spade_5",       "spade_2",
    // Row 2
    "diamond_king",  "spade_6",       "club_10",       "club_9",        "club_8",        "club_7",        "club_6",        "diamond_2",     "club_4",        "spade_3",
    // Row 3
    "diamond_queen", "club_5",        "club_queen",    "heart_8",       "heart_7",       "heart_6",       "club_5",        "diamond_3",     "club_3",        "spade_4",
    // Row 4
    "diamond_10",    "spade_4",       "club_king",     "heart_9",       "heart_2",       "heart_5",       "club_4",        "diamond_4",     "club_2",        "spade_5",
    // Row 5
    "diamond_9",     "spade_3",       "club_1",        "heart_10",      "heart_3",       "heart_4",       "club_3",        "diamond_5",     "heart_1",       "spade_6",
    // Row 6
    "diamond_8",     "spade_2",       "diamond_1",     "heart_queen",   "heart_king",    "heart_1",       "club_2",        "diamond_6",     "heart_king",    "spade_7",
    // Row 7
    "diamond_7",     "heart_2",       "diamond_king",  "diamond_queen", "diamond_10",    "diamond_9",     "diamond_8",     "diamond_7",     "heart_queen",   "spade_8",
    // Row 8
    "diamond_6",     "heart_3",       "heart_4",       "heart_5",       "heart_6",       "heart_7",       "heart_8",       "heart_9",       "heart_10",      "spade_9",
    // Row 9
    FREE,            "diamond_5",     "diamond_4",     "diamond_3",     "diamond_2",     "spade_1",       "spade_king",    "spade_queen",   "spade_10",      FREE
)

let BOARD_ROWS = 10
let BOARD_COLS = 10
let BOARD_SIZE = 100

// ============================================================================
// Types
// ============================================================================

enum ChipColor {
    none
    blue
    green
    red
    yellow
}

// ============================================================================
// Board access
// ============================================================================

def board_card_name(row, col : int) : string {
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) {
        return ""
    }
    return BOARD_LAYOUT[row * BOARD_COLS + col]
}

def is_free_space(row, col : int) : bool {
    return board_card_name(row, col) == FREE
}

def is_corner(row, col : int) : bool {
    return (row == 0 || row == BOARD_ROWS - 1) && (col == 0 || col == BOARD_COLS - 1)
}

// Check if a card name is a jack
def is_jack(card_name : string) : bool {
    return card_name == "club_jack" || card_name == "spade_jack" || card_name == "heart_jack" || card_name == "diamond_jack"
}

// Black jacks (spade, club) remove opponent chips
def is_black_jack(card_name : string) : bool {
    return card_name == "club_jack" || card_name == "spade_jack"
}

// Red jacks (heart, diamond) are wild — place anywhere open
def is_red_jack(card_name : string) : bool {
    return card_name == "heart_jack" || card_name == "diamond_jack"
}

// Validate card name format: {suit}_{rank} or special names
def is_valid_card_name(name : string) : bool {
    return starts_with(name, "club_") || starts_with(name, "diamond_") || starts_with(name, "heart_") || starts_with(name, "spade_")
}
