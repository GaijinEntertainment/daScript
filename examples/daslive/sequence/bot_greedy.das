options gen2
options persistent_heap

module bot_greedy public

require gameplay
require math

// Greedy bot: scores each legal move by heuristic evaluation and picks the best.
//
// Placement scoring (normal cards + red jacks):
//   +10000  complete a sequence (verified by temporary placement + find_sequences)
//   +500    extends own line to 4 (one away from sequence)
//   +100    extends own line to 3
//   +20     extends own line to 2
//   +400    blocks opponent's 4-in-a-row through this cell
//   +50     blocks opponent's 3-in-a-row
//   +3      per adjacent own chip (8-directional)
//   +0..4.5 center proximity bonus
//
// Removal scoring (black jacks):
//   +800    disrupts opponent's 4-in-a-row
//   +200    disrupts opponent's 3-in-a-row
//   +40     disrupts opponent's 2-in-a-row
//   +30     cell useful for own 3+ line after removal
//
// Ties broken by reservoir sampling (seed-based random).
// No lookahead — purely greedy, one move at a time.

// Count consecutive chips of `color` starting from (row,col) in direction (dr,dc),
// including FREE corners as wild. Returns count (0 if starting cell doesn't match).
def private count_run(game : GameState; row, col, dr, dc : int; color : ChipColor) : int {
    var count = 0
    var r = row
    var c = col
    while (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS) {
        if (!cell_matches_color(game, r, c, color)) {
            break
        }
        count++
        r += dr
        c += dc
    }
    return count
}

// How many of the 5-in-a-row lines through (row,col) have potential?
// For each direction, count consecutive own chips + empty + corners extending from (row,col).
// Returns the longest partial sequence through this cell (in any direction).
def private best_line_through(game : GameState; row, col : int; color : ChipColor) : int {
    var best = 0
    for (dir in range(4)) {
        let dr = DIR_ROWS[dir]
        let dc = DIR_COLS[dir]
        // Count forward (including this cell) and backward
        let fwd = count_run(game, row, col, dr, dc, color)
        let bwd = count_run(game, row + (-dr), col + (-dc), -dr, -dc, color)
        let total = fwd + bwd  // fwd includes (row,col), bwd does not
        if (total > best) {
            best = total
        }
    }
    return best
}

// Count how many of the opponent's partial sequences pass through (row,col)
// and how long the best one is. Used for blocking evaluation.
def private opponent_threat_at(game : GameState; row, col : int; player_idx : int) : int {
    var max_threat = 0
    for (p in range(game.num_players)) {
        if (p == player_idx) {
            continue
        }
        let opp_color = player_chip_color(p)
        let line = best_line_through(game, row, col, opp_color)
        if (line > max_threat) {
            max_threat = line
        }
    }
    return max_threat
}

// Score a placement move (normal card or red jack placing a chip)
def private score_placement(game : GameState; row, col : int; player_idx : int) : float {
    let color = player_chip_color(player_idx)
    var score = 0.0
    // Temporarily place chip to evaluate
    var temp_game := game
    temp_game.board_chips[row * BOARD_COLS + col] = color
    // Check if this completes a sequence
    find_sequences(temp_game)
    let new_seqs = count_sequences(temp_game, color)
    let old_seqs = count_sequences(game, color)
    if (new_seqs > old_seqs) {
        score += 10000.0  // Completing a sequence is top priority
    }
    // Evaluate partial sequence extension
    let line = best_line_through(temp_game, row, col, color)
    if (line >= 4) {
        score += 500.0  // One away from sequence
    } elif (line == 3) {
        score += 100.0
    } elif (line == 2) {
        score += 20.0
    }
    // Blocking: check if this cell was valuable to opponents
    let threat = opponent_threat_at(game, row, col, player_idx)
    if (threat >= 4) {
        score += 400.0  // Block opponent about to complete
    } elif (threat == 3) {
        score += 50.0
    }
    // Adjacency: prefer cells near own chips
    for (dr in range(-1, 2)) {
        for (dc in range(-1, 2)) {
            if (dr == 0 && dc == 0) {
                continue
            }
            let nr = row + dr
            let nc = col + dc
            if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS) {
                if (game.board_chips[nr * BOARD_COLS + nc] == color) {
                    score += 3.0
                }
            }
        }
    }
    // Slight center preference
    let center_dist = abs(float(row) - 4.5) + abs(float(col) - 4.5)
    score += (9.0 - center_dist) * 0.5
    return score
}

// Score a removal move (black jack removing an opponent's chip)
def private score_removal(game : GameState; row, col : int; player_idx : int) : float {
    var score = 0.0
    let victim_color = game.board_chips[row * BOARD_COLS + col]
    // Prioritize removing from opponent's longest partial sequence
    let threat = best_line_through(game, row, col, victim_color)
    if (threat >= 4) {
        score += 800.0  // Disrupt near-complete sequence
    } elif (threat == 3) {
        score += 200.0
    } elif (threat == 2) {
        score += 40.0
    }
    // Also consider if this cell would be good for us after removal
    let our_color = player_chip_color(player_idx)
    let our_line = best_line_through(game, row, col, our_color)
    if (our_line >= 3) {
        score += 30.0  // We could use this spot
    }
    return score
}

def bot_greedy_move(game : GameState; player_idx : int; seed : int) : Move {
    var moves <- legal_moves(game, player_idx)
    let n = length(moves)
    if (n == 0) {
        return Move()
    }
    // Score all moves
    var best_score = -1.0e10
    var best_idx = 0
    var num_tied = 0
    var rng = seed
    for (i in range(n)) {
        let m = moves[i]
        let card = get_hand_card(game, player_idx, m.card_idx)
        var score : float
        if (is_black_jack(card)) {
            score = score_removal(game, m.row, m.col, player_idx)
        } else {
            score = score_placement(game, m.row, m.col, player_idx)
        }
        if (score > best_score) {
            best_score = score
            best_idx = i
            num_tied = 1
        } elif (score == best_score) {
            // Reservoir sampling for ties
            num_tied++
            rng = rng * 1103515245 + 12345
            if (abs(rng) % num_tied == 0) {
                best_idx = i
            }
        }
    }
    let result = moves[best_idx]
    delete moves
    return result
}
