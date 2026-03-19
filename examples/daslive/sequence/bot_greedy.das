options gen2
options persistent_heap

module bot_greedy public

require gameplay
require math

// Greedy bot: scores each legal move by configurable weights and picks the best.
//
// Placement weights:
//   sequence_complete  — completing a full 5-in-a-row
//   line_4             — extending own line to 4 (one away)
//   line_3             — extending own line to 3
//   line_2             — extending own line to 2
//   gap_4              — gapped 4: 4 own chips + 1 gap in a 5-cell window (e.g. XX_XX)
//   gap_3              — gapped 3: 3 own chips + 2 gaps in a 5-cell window (e.g. X_X_X)
//   block_4            — blocking opponent's 4-in-a-row
//   block_3            — blocking opponent's 3-in-a-row
//   block_gap_4        — blocking opponent's gapped 4
//   adjacency          — per adjacent own chip (8-directional)
//   center             — center proximity multiplier
//
// Removal weights (black jacks):
//   disrupt_4          — disrupting opponent's 4-in-a-row
//   disrupt_3          — disrupting opponent's 3-in-a-row
//   disrupt_2          — disrupting opponent's 2-in-a-row
//   useful_after       — cell useful for own 3+ line after removal
//
// Ties broken by reservoir sampling (seed-based random).
// No lookahead — purely greedy, one move at a time.

struct GreedyWeights {
    sequence_complete : float = 10000.0
    line_4 : float = 500.0
    line_3 : float = 100.0
    line_2 : float = 20.0
    gap_4 : float = 0.0
    gap_3 : float = 0.0
    block_4 : float = 400.0
    block_3 : float = 50.0
    block_gap_4 : float = 0.0
    adjacency : float = 3.0
    center : float = 0.5
    disrupt_4 : float = 800.0
    disrupt_3 : float = 200.0
    disrupt_2 : float = 40.0
    useful_after : float = 30.0
}

let GREEDY_DEFAULT = GreedyWeights()

// Count consecutive chips of `color` starting from (row,col) in direction (dr,dc),
// including FREE corners as wild.
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

// Longest partial sequence through this cell in any direction.
def private best_line_through(game : GameState; row, col : int; color : ChipColor) : int {
    var best = 0
    for (dir in range(4)) {
        let dr = DIR_ROWS[dir]
        let dc = DIR_COLS[dir]
        let fwd = count_run(game, row, col, dr, dc, color)
        let bwd = count_run(game, row + (-dr), col + (-dc), -dr, -dc, color)
        let total = fwd + bwd
        if (total > best) {
            best = total
        }
    }
    return best
}

// Scan all 5-cell windows through (row,col) in all directions.
// For each window containing (row,col), count own chips and gaps (empty non-corner cells).
// Returns best (chips, gaps) pair found — e.g. (4,1) means gapped-4.
// Only counts windows where every non-gap cell matches `color` (no opponent chips blocking).
def private best_gapped_line(game : GameState; row, col : int; color : ChipColor) : int2 {
    var best_chips = 0
    var best_gaps = 5
    for (dir in range(4)) {
        let dr = DIR_ROWS[dir]
        let dc = DIR_COLS[dir]
        // 5 possible window offsets that include (row,col)
        for (offset in range(5)) {
            let start_r = row - dr * offset
            let start_c = col - dc * offset
            var chips = 0
            var gaps = 0
            var valid = true
            for (step in range(SEQUENCE_LENGTH)) {
                let r = start_r + dr * step
                let c = start_c + dc * step
                if (r < 0 || r >= BOARD_ROWS || c < 0 || c >= BOARD_COLS) {
                    valid = false
                    break
                }
                if (cell_matches_color(game, r, c, color)) {
                    chips++
                } elif (game.board_chips[r * BOARD_COLS + c] == ChipColor.none) {
                    gaps++
                } else {
                    valid = false
                    break
                }
            }
            if (valid && chips > best_chips) {
                best_chips = chips
                best_gaps = gaps
            }
        }
    }
    return int2(best_chips, best_gaps)
}

// Best opponent line length through this cell.
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

def private score_placement(game : GameState; row, col : int; player_idx : int; w : GreedyWeights) : float {
    let color = player_chip_color(player_idx)
    var score = 0.0
    // Temporarily place chip to check sequence completion
    var temp_game := game
    temp_game.board_chips[row * BOARD_COLS + col] = color
    find_sequences(temp_game)
    let new_seqs = count_sequences(temp_game, color)
    let old_seqs = count_sequences(game, color)
    if (new_seqs > old_seqs) {
        score += w.sequence_complete
    }
    // Partial sequence extension
    let line = best_line_through(temp_game, row, col, color)
    if (line >= 4) {
        score += w.line_4
    } elif (line == 3) {
        score += w.line_3
    } elif (line == 2) {
        score += w.line_2
    }
    // Gapped sequences (e.g. XX_XX, X_X_X)
    if (w.gap_4 != 0.0 || w.gap_3 != 0.0) {
        let gapped = best_gapped_line(temp_game, row, col, color)
        if (gapped.x >= 4 && gapped.y >= 1) {
            score += w.gap_4
        } elif (gapped.x >= 3 && gapped.y >= 2) {
            score += w.gap_3
        }
    }
    // Blocking
    let threat = opponent_threat_at(game, row, col, player_idx)
    if (threat >= 4) {
        score += w.block_4
    } elif (threat == 3) {
        score += w.block_3
    }
    // Blocking opponent's gapped sequences
    if (w.block_gap_4 != 0.0) {
        for (p in range(game.num_players)) {
            if (p == player_idx) {
                continue
            }
            let opp_color = player_chip_color(p)
            let opp_gapped = best_gapped_line(game, row, col, opp_color)
            if (opp_gapped.x >= 4 && opp_gapped.y >= 1) {
                score += w.block_gap_4
                break
            }
        }
    }
    // Adjacency
    for (dr in range(-1, 2)) {
        for (dc in range(-1, 2)) {
            if (dr == 0 && dc == 0) {
                continue
            }
            let nr = row + dr
            let nc = col + dc
            if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS) {
                if (game.board_chips[nr * BOARD_COLS + nc] == color) {
                    score += w.adjacency
                }
            }
        }
    }
    // Center preference
    let center_dist = abs(float(row) - 4.5) + abs(float(col) - 4.5)
    score += (9.0 - center_dist) * w.center
    return score
}

def private score_removal(game : GameState; row, col : int; player_idx : int; w : GreedyWeights) : float {
    var score = 0.0
    let victim_color = game.board_chips[row * BOARD_COLS + col]
    let threat = best_line_through(game, row, col, victim_color)
    if (threat >= 4) {
        score += w.disrupt_4
    } elif (threat == 3) {
        score += w.disrupt_3
    } elif (threat == 2) {
        score += w.disrupt_2
    }
    let our_color = player_chip_color(player_idx)
    let our_line = best_line_through(game, row, col, our_color)
    if (our_line >= 3) {
        score += w.useful_after
    }
    return score
}

// Main entry point — with explicit weights
def bot_greedy_move(game : GameState; player_idx : int; seed : int; w : GreedyWeights) : Move {
    var moves <- legal_moves(game, player_idx)
    let n = length(moves)
    if (n == 0) {
        return Move()
    }
    var best_score = -1.0e10
    var best_idx = 0
    var num_tied = 0
    var rng = seed
    for (i in range(n)) {
        let m = moves[i]
        let card = get_hand_card(game, player_idx, m.card_idx)
        var score = 0.0
        if (is_black_jack(card)) {
            score = score_removal(game, m.row, m.col, player_idx, w)
        } else {
            score = score_placement(game, m.row, m.col, player_idx, w)
        }
        if (score > best_score) {
            best_score = score
            best_idx = i
            num_tied = 1
        } elif (score == best_score) {
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

// Convenience overload with default weights
def bot_greedy_move(game : GameState; player_idx : int; seed : int) : Move {
    return bot_greedy_move(game, player_idx, seed, GREEDY_DEFAULT)
}
