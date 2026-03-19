options gen2
options persistent_heap

module gameplay public

require strings
require math

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

// ============================================================================
// Chip helpers (color utilities — no game state needed)
// ============================================================================

// Cycle through chip colors for debug placement
def next_chip_color(color : ChipColor) : ChipColor {
    if (color == ChipColor.none) {
        return ChipColor.blue
    } elif (color == ChipColor.blue) {
        return ChipColor.green
    } elif (color == ChipColor.green) {
        return ChipColor.red
    } elif (color == ChipColor.red) {
        return ChipColor.yellow
    }
    return ChipColor.none
}

// Get display color for a chip
def chip_color_to_float4(color : ChipColor) : float4 {
    if (color == ChipColor.blue) {
        return float4(0.2, 0.4, 0.9, 0.9)
    } elif (color == ChipColor.green) {
        return float4(0.2, 0.8, 0.3, 0.9)
    } elif (color == ChipColor.red) {
        return float4(0.9, 0.2, 0.2, 0.9)
    } elif (color == ChipColor.yellow) {
        return float4(0.9, 0.8, 0.1, 0.9)
    }
    return float4(1.0, 1.0, 1.0, 1.0)
}

// ============================================================================
// Standard deck (two 52-card decks = 104 cards for Sequence)
// ============================================================================

let SUITS = fixed_array("club", "diamond", "heart", "spade")
let RANKS = fixed_array("1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king")
let SUIT_COUNT = 4
let RANK_COUNT = 13

def card_suit(card_name : string) : string {
    let idx = find(card_name, "_")
    if (idx < 0) {
        return ""
    }
    return slice(card_name, 0, idx)
}

def card_rank(card_name : string) : string {
    let idx = find(card_name, "_")
    if (idx < 0) {
        return ""
    }
    return slice(card_name, idx + 1)
}

// Rank order for sorting within a suit (ace=0, 2=1, ..., king=12)
def rank_order(rank : string) : int {
    for (i in range(RANK_COUNT)) {
        if (RANKS[i] == rank) {
            return i
        }
    }
    return -1
}

// Suit order for sorting (club=0, diamond=1, heart=2, spade=3)
def suit_order(suit : string) : int {
    for (i in range(SUIT_COUNT)) {
        if (SUITS[i] == suit) {
            return i
        }
    }
    return -1
}

def build_standard_deck() : array<string> {
    var deck : array<string>
    for (s in range(SUIT_COUNT)) {
        for (r in range(RANK_COUNT)) {
            deck |> push("{SUITS[s]}_{RANKS[r]}")
        }
    }
    return <- deck
}

def build_double_deck() : array<string> {
    var deck : array<string>
    // Two standard decks
    for (copy in range(2)) {
        for (s in range(SUIT_COUNT)) {
            for (r in range(RANK_COUNT)) {
                deck |> push("{SUITS[s]}_{RANKS[r]}")
            }
        }
    }
    return <- deck
}

def shuffle_deck(var deck : array<string>&; seed : int) {
    var rng_seed = seed
    let n = length(deck)
    var i = n - 1
    while (i > 0) {
        // Simple LCG random
        rng_seed = rng_seed * 1103515245 + 12345
        let j = abs(rng_seed) % (i + 1)
        let tmp = deck[i]
        deck[i] = deck[j]
        deck[j] = tmp
        i--
    }
}

// ============================================================================
// Game state
// ============================================================================

let MAX_PLAYERS = 4
let MAX_HAND_SIZE = 7

enum GamePhase {
    not_started
    playing
    game_over
}

struct GameState {
    num_players : int = 2
    current_player : int = 0
    phase : GamePhase = GamePhase.not_started
    hands : string[MAX_PLAYERS][MAX_HAND_SIZE]
    hand_sizes : int[MAX_PLAYERS]
    draw_pile : array<string>
    discard_pile : array<string>
    board_chips : ChipColor[100]
    winner : int = -1
}

struct Move {
    card_idx : int = -1   // Index in player's hand
    row : int = -1        // Board row to place/remove chip
    col : int = -1        // Board col to place/remove chip
}

def hand_size_for_players(num_players : int) : int {
    if (num_players == 2) {
        return 7
    } elif (num_players == 3) {
        return 6
    }
    return 5  // 4 players
}

def player_chip_color(player_idx : int) : ChipColor {
    if (player_idx == 0) {
        return ChipColor.blue
    } elif (player_idx == 1) {
        return ChipColor.green
    } elif (player_idx == 2) {
        return ChipColor.red
    }
    return ChipColor.yellow
}

// ============================================================================
// Chip state (on GameState)
// ============================================================================

def get_chip(game : GameState; row, col : int) : ChipColor {
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) {
        return ChipColor.none
    }
    return game.board_chips[row * BOARD_COLS + col]
}

def place_chip(var game : GameState; row, col : int; color : ChipColor) : bool {
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) {
        return false
    }
    game.board_chips[row * BOARD_COLS + col] = color
    return true
}

def clear_all_chips(var game : GameState) {
    for (i in range(BOARD_SIZE)) {
        game.board_chips[i] = ChipColor.none
    }
}

// ============================================================================
// Game setup and hand management
// ============================================================================

def make_game(var game : GameState; num_players : int; seed : int = 42) {
    game.num_players = clamp(num_players, 2, MAX_PLAYERS)
    game.current_player = 0
    game.phase = GamePhase.playing
    game.winner = -1
    // Clear hands
    for (p in range(MAX_PLAYERS)) {
        game.hand_sizes[p] = 0
        for (c in range(MAX_HAND_SIZE)) {
            game.hands[p][c] = ""
        }
    }
    // Build and shuffle double deck
    game.draw_pile <- build_double_deck()
    shuffle_deck(game.draw_pile, seed)
    game.discard_pile |> clear()
    // Clear board
    clear_all_chips(game)
    // Deal hands
    let hs = hand_size_for_players(game.num_players)
    for (c in range(hs)) {
        for (p in range(game.num_players)) {
            if (length(game.draw_pile) > 0) {
                game.hands[p][game.hand_sizes[p]] = game.draw_pile[length(game.draw_pile) - 1]
                game.hand_sizes[p]++
                game.draw_pile |> resize(length(game.draw_pile) - 1)
            }
        }
    }
}

def draw_card_from_pile(var game : GameState; player_idx : int) : bool {
    if (player_idx < 0 || player_idx >= game.num_players) {
        return false
    }
    if (game.hand_sizes[player_idx] >= MAX_HAND_SIZE) {
        return false
    }
    if (length(game.draw_pile) == 0) {
        return false
    }
    game.hands[player_idx][game.hand_sizes[player_idx]] = game.draw_pile[length(game.draw_pile) - 1]
    game.hand_sizes[player_idx]++
    game.draw_pile |> resize(length(game.draw_pile) - 1)
    return true
}

def remove_card_from_hand(var game : GameState; player_idx, card_idx : int) : string {
    if (player_idx < 0 || player_idx >= game.num_players) {
        return ""
    }
    if (card_idx < 0 || card_idx >= game.hand_sizes[player_idx]) {
        return ""
    }
    let card = game.hands[player_idx][card_idx]
    // Shift remaining cards left
    for (i in range(card_idx, game.hand_sizes[player_idx] - 1)) {
        game.hands[player_idx][i] = game.hands[player_idx][i + 1]
    }
    game.hand_sizes[player_idx]--
    game.hands[player_idx][game.hand_sizes[player_idx]] = ""
    return card
}

def get_hand_card(game : GameState; player_idx, card_idx : int) : string {
    if (player_idx < 0 || player_idx >= game.num_players) {
        return ""
    }
    if (card_idx < 0 || card_idx >= game.hand_sizes[player_idx]) {
        return ""
    }
    return game.hands[player_idx][card_idx]
}

def get_hand_size(game : GameState; player_idx : int) : int {
    if (player_idx < 0 || player_idx >= game.num_players) {
        return 0
    }
    return game.hand_sizes[player_idx]
}

// ============================================================================
// Dead card detection
// ============================================================================

// A card is "dead" if both board spaces for it are already occupied by chips
def is_dead_card(game : GameState; card_name : string) : bool {
    if (is_jack(card_name)) {
        return false  // Jacks are never dead
    }
    var occupied = 0
    var total = 0
    for (i in range(BOARD_SIZE)) {
        if (BOARD_LAYOUT[i] == card_name) {
            total++
            if (game.board_chips[i] != ChipColor.none) {
                occupied++
            }
        }
    }
    return total > 0 && occupied == total
}

// Discard dead cards from hand and redraw
def discard_dead_cards(var game : GameState; player_idx : int) {
    var changed = true
    while (changed) {
        changed = false
        for (c in range(game.hand_sizes[player_idx])) {
            let card = game.hands[player_idx][c]
            if (is_dead_card(game, card)) {
                remove_card_from_hand(game, player_idx, c)
                game.discard_pile |> push(card)
                draw_card_from_pile(game, player_idx)
                changed = true
                break
            }
        }
    }
}

// ============================================================================
// Legal moves
// ============================================================================

def legal_moves(game : GameState; player_idx : int) : array<Move> {
    var moves : array<Move>
    let color = player_chip_color(player_idx)
    for (c in range(game.hand_sizes[player_idx])) {
        let card = game.hands[player_idx][c]
        if (is_red_jack(card)) {
            // Wild: place on any open non-corner space
            for (i in range(BOARD_SIZE)) {
                let row = i / BOARD_COLS
                let col = i % BOARD_COLS
                if (!is_corner(row, col) && game.board_chips[i] == ChipColor.none) {
                    moves |> push(Move(card_idx = c, row = row, col = col))
                }
            }
        } elif (is_black_jack(card)) {
            // Remove: any opponent chip (not from a completed sequence — TODO Phase 4)
            for (i in range(BOARD_SIZE)) {
                let row = i / BOARD_COLS
                let col = i % BOARD_COLS
                let chip = game.board_chips[i]
                if (chip != ChipColor.none && chip != color) {
                    moves |> push(Move(card_idx = c, row = row, col = col))
                }
            }
        } else {
            // Normal card: place on matching empty board space
            for (i in range(BOARD_SIZE)) {
                let row = i / BOARD_COLS
                let col = i % BOARD_COLS
                if (BOARD_LAYOUT[i] == card && game.board_chips[i] == ChipColor.none) {
                    moves |> push(Move(card_idx = c, row = row, col = col))
                }
            }
        }
    }
    return <- moves
}

def is_legal_move(game : GameState; player_idx : int; m : Move) : bool {
    if (m.card_idx < 0 || m.card_idx >= game.hand_sizes[player_idx]) {
        return false
    }
    if (m.row < 0 || m.row >= BOARD_ROWS || m.col < 0 || m.col >= BOARD_COLS) {
        return false
    }
    let card = game.hands[player_idx][m.card_idx]
    let idx = m.row * BOARD_COLS + m.col
    let color = player_chip_color(player_idx)
    if (is_red_jack(card)) {
        return !is_corner(m.row, m.col) && game.board_chips[idx] == ChipColor.none
    } elif (is_black_jack(card)) {
        let chip = game.board_chips[idx]
        return chip != ChipColor.none && chip != color
    } else {
        return BOARD_LAYOUT[idx] == card && game.board_chips[idx] == ChipColor.none
    }
}

def has_legal_moves(game : GameState; player_idx : int) : bool {
    // Quick check without building the full array
    for (c in range(game.hand_sizes[player_idx])) {
        let card = game.hands[player_idx][c]
        if (is_red_jack(card)) {
            for (i in range(BOARD_SIZE)) {
                if (!is_corner(i / BOARD_COLS, i % BOARD_COLS) && game.board_chips[i] == ChipColor.none) {
                    return true
                }
            }
        } elif (is_black_jack(card)) {
            let color = player_chip_color(player_idx)
            for (i in range(BOARD_SIZE)) {
                let chip = game.board_chips[i]
                if (chip != ChipColor.none && chip != color) {
                    return true
                }
            }
        } else {
            for (i in range(BOARD_SIZE)) {
                if (BOARD_LAYOUT[i] == card && game.board_chips[i] == ChipColor.none) {
                    return true
                }
            }
        }
    }
    return false
}

// ============================================================================
// Apply move
// ============================================================================

def apply_move(var game : GameState; player_idx : int; move : Move) : bool {
    if (game.phase != GamePhase.playing) {
        return false
    }
    if (player_idx != game.current_player) {
        return false
    }
    if (move.card_idx < 0 || move.card_idx >= game.hand_sizes[player_idx]) {
        return false
    }
    let card = game.hands[player_idx][move.card_idx]
    let color = player_chip_color(player_idx)
    if (is_black_jack(card)) {
        // Remove opponent chip
        let chip = game.board_chips[move.row * BOARD_COLS + move.col]
        if (chip == ChipColor.none || chip == color) {
            return false
        }
        game.board_chips[move.row * BOARD_COLS + move.col] = ChipColor.none
    } else {
        // Place chip (normal or red jack)
        if (game.board_chips[move.row * BOARD_COLS + move.col] != ChipColor.none) {
            return false
        }
        game.board_chips[move.row * BOARD_COLS + move.col] = color
    }
    // Remove card from hand, discard it
    let removed = remove_card_from_hand(game, player_idx, move.card_idx)
    game.discard_pile |> push(removed)
    // Draw replacement
    draw_card_from_pile(game, player_idx)
    // Discard any newly-dead cards
    discard_dead_cards(game, player_idx)
    // Advance turn
    game.current_player = (game.current_player + 1) % game.num_players
    return true
}

// ============================================================================
// Hand management (set, sort)
// ============================================================================

// Replace a player's entire hand with the given cards
def set_hand(var game : GameState; player_idx : int; cards : array<string>) {
    if (player_idx < 0 || player_idx >= MAX_PLAYERS) {
        return
    }
    let n = min(length(cards), MAX_HAND_SIZE)
    for (c in range(MAX_HAND_SIZE)) {
        game.hands[player_idx][c] = c < n ? cards[c] : ""
    }
    game.hand_sizes[player_idx] = n
}

// Sort a player's hand by suit then rank
def sort_hand(var game : GameState; player_idx : int) {
    let n = game.hand_sizes[player_idx]
    // Simple insertion sort
    for (i in range(1, n)) {
        let card = game.hands[player_idx][i]
        let s = suit_order(card_suit(card))
        let r = rank_order(card_rank(card))
        var j = i - 1
        while (j >= 0) {
            let cj = game.hands[player_idx][j]
            let sj = suit_order(card_suit(cj))
            let rj = rank_order(card_rank(cj))
            if (sj > s || (sj == s && rj > r)) {
                game.hands[player_idx][j + 1] = game.hands[player_idx][j]
                j--
            } else {
                break
            }
        }
        game.hands[player_idx][j + 1] = card
    }
}
