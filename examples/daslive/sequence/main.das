options gen2
options persistent_heap

require live/glfw_live
require live/opengl_live
require live/live_api
require live/live_commands
require live/live_vars
require live/live_watch_boost
require live_host
require opengl/opengl_boost
require glfw/glfw_boost
require cards/opengl_cards
require daslib/json_boost
require daslib/safe_addr
require daslib/math_boost
require gameplay
require bot_random

// --- Chip shader ---

var @in @location chip_v_position : float2
var @inout chip_f_color : float4
var @uniform chip_u_mvp : float4x4
var @uniform chip_u_color : float4

[vertex_program]
def vs_chip {
    chip_f_color = chip_u_color
    gl_Position = chip_u_mvp * float4(chip_v_position, 0.0, 1.0)
}

var @out chip_frag_color : float4

[fragment_program]
def fs_chip {
    chip_frag_color = chip_f_color
}

[vertex_buffer]
struct ChipVertex {
    xy : float2
}

let CHIP_SEGMENTS = 32

struct ChipRenderer {
    program : uint
    fill_vao : uint
    fill_vbo : uint
    outline_vao : uint
    outline_vbo : uint
}

def create_chip_renderer() : ChipRenderer {
    var cr : ChipRenderer
    cr.program = create_shader_program(@@vs_chip, @@fs_chip)
    // Fill circle: triangle fan (center + CHIP_SEGMENTS + 1 vertices)
    let num_fill = CHIP_SEGMENTS + 2
    var fill_vao_tmp : uint
    var fill_vbo_tmp : uint
    glGenVertexArrays(1, safe_addr(fill_vao_tmp))
    glBindVertexArray(fill_vao_tmp)
    glGenBuffers(1, safe_addr(fill_vbo_tmp))
    glBindBuffer(GL_ARRAY_BUFFER, fill_vbo_tmp)
    var fill : array<ChipVertex>
    fill |> resize(num_fill)
    fill[0] = ChipVertex(xy = float2(0.0, 0.0))
    for (i in range(CHIP_SEGMENTS + 1)) {
        let a = 2.0 * PI * float(i) / float(CHIP_SEGMENTS)
        fill[i + 1] = ChipVertex(xy = float2(cos(a), sin(a)))
    }
    glBufferData(GL_ARRAY_BUFFER, fill, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<ChipVertex>)
    cr.fill_vao = fill_vao_tmp
    cr.fill_vbo = fill_vbo_tmp
    // Outline circle: line loop (CHIP_SEGMENTS vertices)
    var outline_vao_tmp : uint
    var outline_vbo_tmp : uint
    glGenVertexArrays(1, safe_addr(outline_vao_tmp))
    glBindVertexArray(outline_vao_tmp)
    glGenBuffers(1, safe_addr(outline_vbo_tmp))
    glBindBuffer(GL_ARRAY_BUFFER, outline_vbo_tmp)
    var outline : array<ChipVertex>
    outline |> resize(CHIP_SEGMENTS)
    for (i in range(CHIP_SEGMENTS)) {
        let a = 2.0 * PI * float(i) / float(CHIP_SEGMENTS)
        outline[i] = ChipVertex(xy = float2(cos(a), sin(a)))
    }
    glBufferData(GL_ARRAY_BUFFER, outline, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<ChipVertex>)
    cr.outline_vao = outline_vao_tmp
    cr.outline_vbo = outline_vbo_tmp
    glBindVertexArray(0u)
    return cr
}

def delete_chip_renderer(var cr : ChipRenderer&) {
    if (cr.fill_vao != 0u) {
        var v = cr.fill_vao
        var b = cr.fill_vbo
        glDeleteVertexArrays(1, safe_addr(v))
        glDeleteBuffers(1, safe_addr(b))
        cr.fill_vao = 0u
        cr.fill_vbo = 0u
    }
    if (cr.outline_vao != 0u) {
        var v = cr.outline_vao
        var b = cr.outline_vbo
        glDeleteVertexArrays(1, safe_addr(v))
        glDeleteBuffers(1, safe_addr(b))
        cr.outline_vao = 0u
        cr.outline_vbo = 0u
    }
}

def draw_chip(cr : ChipRenderer; cx, cy, radius : float; color : float4; outline_color : float4;
              screen_w, screen_h : int) {
    let sx = 2.0 / float(screen_w)
    let sy = 2.0 / float(screen_h)
    // MVP: scale by radius, then translate to center, then to NDC
    var mvp : float4x4
    mvp[0] = float4(radius * sx, 0.0, 0.0, 0.0)
    mvp[1] = float4(0.0, -radius * sy, 0.0, 0.0)
    mvp[2] = float4(0.0, 0.0, 1.0, 0.0)
    mvp[3] = float4(cx * sx - 1.0, 1.0 - cy * sy, 0.0, 1.0)
    glUseProgram(cr.program)
    chip_u_mvp = mvp
    // Fill
    chip_u_color = color
    vs_chip_bind_uniform(cr.program)
    fs_chip_bind_uniform(cr.program)
    glBindVertexArray(cr.fill_vao)
    glDrawArrays(GL_TRIANGLE_FAN, 0, CHIP_SEGMENTS + 2)
    // Outline
    chip_u_color = outline_color
    vs_chip_bind_uniform(cr.program)
    glBindVertexArray(cr.outline_vao)
    glLineWidth(2.0)
    glDrawArrays(GL_LINE_LOOP, 0, CHIP_SEGMENTS)
}

// --- Tunable parameters ---

var @live board_pad = 2.0
var @live bg_color = float3(0.1, 0.15, 0.1)
var @live hover_tint = float4(1.3, 1.3, 1.5, 1.0)
var @live chip_radius_frac = 0.3  // chip radius as fraction of card width
var @live hand_margin = 4.0       // margin from screen edges for hand display
var @live hand_row_gap = 2.0      // gap between suit rows in hand display
var @live bot_stack_offset = 8.0  // pixel offset between stacked bot cards
var @live cheat_mode = false      // show all player hands
var @live seq_pulse_speed = 2.0  // speed of sequence highlight pulsing
var @live seq_pulse_min = 0.7   // minimum brightness of pulse
var @live seq_card_tint = 0.35  // how strongly sequence cards are tinted with chip color
var @live bot_delay = 0.5       // seconds between bot moves
var selected_card = -1      // index of selected card in human hand (-1 = none
var bot_timer = 0.0         // countdown until next bot move
var bot_rng_seed = 12345    // RNG seed for bot moves, incremented each move

// --- State ---

var game = GameState()
var renderer : CardRenderer
var chip_renderer : ChipRenderer
var deck = GpuDeck()
var back_index : int = -1

// Precomputed card indices for each board cell (100 entries, -1 for FREE corners using back)
var board_card_indices : int[100]

// Board layout (computed each frame for hit-testing)
var board_ox : float = 0.0
var board_oy : float = 0.0
var board_scale : float = 1.0
var board_cs_w : float = 0.0
var board_cs_h : float = 0.0
var board_pad_cur : float = 0.0

// Mouse state
var hover_row : int = -1
var hover_col : int = -1
var mouse_was_pressed : bool = false
var mouse_clicked : bool = false  // true on the frame mouse was first pressed

// --- Helpers ---

def compute_board_card_indices() {
    for (i in range(BOARD_SIZE)) {
        let row = i / BOARD_COLS
        let col = i % BOARD_COLS
        let name = board_card_name(row, col)
        if (name == FREE) {
            board_card_indices[i] = -1
        } else {
            board_card_indices[i] = find_card(deck, name)
        }
    }
}

// Convert screen coords to board cell. Returns (-1,-1) if outside board.
def screen_to_cell(mx, my : float) : int2 {
    let rel_x = mx - board_ox - board_pad_cur
    let rel_y = my - board_oy - board_pad_cur
    if (rel_x < 0.0 || rel_y < 0.0) {
        return int2(-1, -1)
    }
    let step_x = board_cs_w + board_pad_cur
    let step_y = board_cs_h + board_pad_cur
    let col = int(rel_x / step_x)
    let row = int(rel_y / step_y)
    if (col < 0 || col >= BOARD_COLS || row < 0 || row >= BOARD_ROWS) {
        return int2(-1, -1)
    }
    // Check we're within the card, not in padding
    let in_x = rel_x - float(col) * step_x
    let in_y = rel_y - float(row) * step_y
    if (in_x > board_cs_w || in_y > board_cs_h) {
        return int2(-1, -1)
    }
    return int2(col, row)
}

def handle_mouse() {
    let mpos = glfwGetCursorPos(live_window)
    let cell = screen_to_cell(mpos.x, mpos.y)
    hover_col = cell.x
    hover_row = cell.y
    let pressed = glfwGetMouseButton(live_window, int(GLFW_MOUSE_BUTTON_1)) == int(GLFW_PRESS)
    mouse_clicked = pressed && !mouse_was_pressed
    mouse_was_pressed = pressed
    // Right-click deselects
    let right_pressed = glfwGetMouseButton(live_window, int(GLFW_MOUSE_BUTTON_2)) == int(GLFW_PRESS)
    if (right_pressed && selected_card >= 0) {
        selected_card = -1
    }
    if (mouse_clicked) {
        if (game.phase == GamePhase.game_over) {
            // Click anywhere to restart
            make_game(game, game.num_players, game.num_players + int(get_uptime() * 1000.0))
            sort_hand(game, 0)
            selected_card = -1
            bot_timer = float(bot_delay)
            return
        }
        if (game.phase == GamePhase.playing && game.current_player == 0 && selected_card >= 0) {
            // Try to play selected card on hovered board cell
            if (hover_row >= 0 && hover_col >= 0) {
                let m = Move(card_idx = selected_card, row = hover_row, col = hover_col)
                if (apply_move(game, 0, m)) {
                    selected_card = -1
                    sort_hand(game, 0)
                    bot_timer = float(bot_delay)  // give bot time before its turn
                }
            }
        }
    }
}

// --- Hand display ---

// Compute horizontal step to fit max_cards cards within avail_width pixels
// Returns the x step between card left edges
def compute_hand_step_x(cs_w, avail_width : float; max_cards : int) : float {
    if (max_cards <= 1) {
        return cs_w
    }
    // Need: (max_cards - 1) * step + cs_w <= avail_width
    let step = (avail_width - cs_w) / float(max_cards - 1)
    return min(step, cs_w)  // don't space more than card width
}

// Draw a player's hand as rows (one per suit), sorted by rank
// anchor_x, anchor_y: corner anchor point
// grow_right: cards extend right from anchor (true) or left (false)
// grow_down: suit rows extend down from anchor (true) or up (false)
def draw_hand_by_suit(player_idx : int; anchor_x, anchor_y : float; scale : float;
                      step_x : float; screen_w, screen_h : int;
                      grow_right, grow_down : bool) {
    sort_hand(game, player_idx)
    let cs_w = CARD_DOC_WIDTH * scale
    let cs_h = CARD_DOC_HEIGHT * scale
    let row_step = cs_h + float(hand_row_gap)
    let is_human = player_idx == 0
    // Group cards by suit
    var suit_starts : int[5]
    var num_suits = 0
    var prev_suit = ""
    for (c in range(get_hand_size(game, player_idx))) {
        let card = get_hand_card(game, player_idx, c)
        let s = card_suit(card)
        if (s != prev_suit) {
            suit_starts[num_suits] = c
            num_suits++
            prev_suit = s
        }
    }
    suit_starts[num_suits] = get_hand_size(game, player_idx)
    // Draw each suit row
    for (row in range(num_suits)) {
        let row_start = suit_starts[row]
        let row_end = suit_starts[row + 1]
        for (c in range(row_start, row_end)) {
            let card = get_hand_card(game, player_idx, c)
            let ci = c - row_start
            let ry = grow_down ? float(row) : float(row - num_suits)
            let rx = grow_right ? float(ci) : -float(ci) - 1.0
            let card_x = anchor_x + rx * step_x + (grow_right ? 0.0 : (step_x - cs_w))
            let card_y = anchor_y + ry * row_step
            let card_idx = find_card(deck, card)
            var tint = float4(1.0, 1.0, 1.0, 1.0)
            if (is_human && selected_card >= 0) {
                if (c == selected_card) {
                    tint = float4(1.4, 1.4, 1.0, 1.0)
                } else {
                    tint = float4(0.5, 0.5, 0.5, 1.0)
                }
            }
            if (card_idx >= 0) {
                draw_card(renderer, deck, card_idx, card_x, card_y, scale, screen_w, screen_h,
                    [tint = tint])
            }
        }
    }
    // Handle click on human hand cards
    if (is_human && game.phase == GamePhase.playing && game.current_player == 0 && mouse_clicked) {
        let mpos = glfwGetCursorPos(live_window)
        var clicked = -1
        for (row in range(num_suits)) {
            let row_start = suit_starts[row]
            let row_end = suit_starts[row + 1]
            for (c in range(row_start, row_end)) {
                let ci = c - row_start
                let ry = grow_down ? float(row) : float(row - num_suits)
                let rx = grow_right ? float(ci) : -float(ci) - 1.0
                let card_x = anchor_x + rx * step_x + (grow_right ? 0.0 : (step_x - cs_w))
                let card_y = anchor_y + ry * row_step
                if (mpos.x >= card_x && mpos.x <= card_x + cs_w && mpos.y >= card_y && mpos.y <= card_y + cs_h) {
                    clicked = c
                }
            }
        }
        if (clicked >= 0) {
            selected_card = (selected_card == clicked) ? -1 : clicked
        }
    }
}

// Draw bot hand as diagonal stack of face-down cards
def draw_hand_stack(player_idx : int; anchor_x, anchor_y : float; scale : float;
                    screen_w, screen_h : int; grow_right, grow_down : bool) {
    let n = get_hand_size(game, player_idx)
    if (n == 0 || back_index < 0) {
        return
    }
    let cs_w = CARD_DOC_WIDTH * scale
    let cs_h = CARD_DOC_HEIGHT * scale
    let ox = float(bot_stack_offset) * (grow_right ? 1.0 : -1.0)
    let oy = float(bot_stack_offset) * (grow_down ? 1.0 : -1.0)
    let color = player_chip_color(player_idx)
    let tint = chip_color_to_float4(color) * float4(1.0, 1.0, 1.0, 0.5) + float4(0.5, 0.5, 0.5, 0.5)
    for (i in range(n)) {
        let card_x = anchor_x + float(i) * ox + (grow_right ? 0.0 : -cs_w)
        let card_y = anchor_y + float(i) * oy + (grow_down ? 0.0 : -cs_h)
        draw_card(renderer, deck, back_index, card_x, card_y, scale, screen_w, screen_h,
            [tint = tint])
    }
}

// Draw all player hands in their respective corners
// Layout: P1=bottom-left, P2=top-left, P3=top-right, P4=bottom-right
// Board is centered; hands go in left/right margins at board scale
def draw_hands(screen_w, screen_h : int) {
    let fw = float(screen_w)
    let fh = float(screen_h)
    let scale = board_scale
    let cs_w = CARD_DOC_WIDTH * scale
    let cs_h = CARD_DOC_HEIGHT * scale
    let margin = float(hand_margin)
    // Available width for hands = margin from screen edge to board edge
    let left_avail = board_ox - margin * 2.0
    let right_avail = fw - (board_ox + float(BOARD_COLS) * cs_w + float(BOARD_COLS + 1) * board_pad_cur) - margin * 2.0
    // Compute step_x to fit 7 cards (max hand) in available width
    let left_step = compute_hand_step_x(cs_w, left_avail, MAX_HAND_SIZE)
    let right_step = compute_hand_step_x(cs_w, right_avail, MAX_HAND_SIZE)
    // Vertical midpoint for splitting top/bottom hands
    let mid_y = fh * 0.5
    // P1 (human): bottom-left — always show by suit, growing up from bottom
    draw_hand_by_suit(0, margin, fh - margin, scale, left_step, screen_w, screen_h, true, false)
    // Other players
    for (p in range(1, game.num_players)) {
        if (cheat_mode) {
            if (p == 1) {
                draw_hand_by_suit(1, margin, margin, scale, left_step, screen_w, screen_h, true, true)
            } elif (p == 2) {
                draw_hand_by_suit(2, fw - margin, margin, scale, right_step, screen_w, screen_h, false, true)
            } elif (p == 3) {
                draw_hand_by_suit(3, fw - margin, fh - margin, scale, right_step, screen_w, screen_h, false, false)
            }
        } else {
            if (p == 1) {
                draw_hand_stack(1, margin, margin, scale, screen_w, screen_h, true, true)
            } elif (p == 2) {
                draw_hand_stack(2, fw - margin, margin, scale, screen_w, screen_h, false, true)
            } elif (p == 3) {
                draw_hand_stack(3, fw - margin, fh - margin, scale, screen_w, screen_h, false, false)
            }
        }
    }
}

// --- Game over overlay ---

def draw_game_over(screen_w, screen_h : int) {
    // Draw a large colored chip in the center to indicate the winner
    let cx = float(screen_w) * 0.5
    let cy = float(screen_h) * 0.5
    let r = min(float(screen_w), float(screen_h)) * 0.08
    let winner_color = player_chip_color(game.winner)
    let color = chip_color_to_float4(winner_color)
    // Pulsing winner chip
    let pulse = 0.7 + 0.3 * sin(get_uptime() * 2.0)
    let bright = color * float4(pulse, pulse, pulse, 1.0)
    draw_chip(chip_renderer, cx, cy, r * 1.5, bright, float4(1.0, 1.0, 1.0, 1.0), screen_w, screen_h)
    // Inner ring
    draw_chip(chip_renderer, cx, cy, r * 0.8, color, float4(0.0, 0.0, 0.0, 1.0), screen_w, screen_h)
}

// --- Lifecycle ---

[export]
def init() {
    live_create_window("Sequence", 1280, 800)
    renderer = create_card_renderer()
    delete_chip_renderer(chip_renderer)
    chip_renderer = create_chip_renderer()
    deck <- load_gpu_deck()
    back_index = find_card(deck, "back")
    compute_board_card_indices()
    if (game.phase == GamePhase.not_started) {
        make_game(game, 2)
        sort_hand(game, 0)
    }
    print("sequence: init (is_reload={is_reload()}, cards={card_count(deck)})\n")
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(bg_color.x, bg_color.y, bg_color.z, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
    glDisable(GL_DEPTH_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    if (deck.loaded) {
        handle_mouse()
        // Bot auto-play
        if (game.phase == GamePhase.playing && game.current_player > 0) {
            bot_timer -= get_dt()
            if (bot_timer <= 0.0) {
                let bot_player = game.current_player
                discard_dead_cards(game, bot_player)
                bot_rng_seed++
                let m = bot_random_move(game, bot_player, bot_rng_seed)
                if (m.card_idx >= 0) {
                    apply_move(game, bot_player, m)
                }
                bot_timer = float(bot_delay)
            }
        }
        draw_board(w, h)
        if (game.phase == GamePhase.playing || game.phase == GamePhase.game_over) {
            draw_hands(w, h)
        }
        if (game.phase == GamePhase.game_over) {
            draw_game_over(w, h)
        }
    }
    glDisable(GL_BLEND)
    live_end_frame()
}

def draw_board(screen_w, screen_h : int) {
    let fw = float(screen_w)
    let fh = float(screen_h)
    // Scale to fit 10x10 grid of upright cards with padding
    let pad = board_pad
    let total_pad_x = pad * float(BOARD_COLS + 1)
    let total_pad_y = pad * float(BOARD_ROWS + 1)
    let scale = min((fw - total_pad_x) / (float(BOARD_COLS) * CARD_DOC_WIDTH),
                    (fh - total_pad_y) / (float(BOARD_ROWS) * CARD_DOC_HEIGHT))
    let cs_w = CARD_DOC_WIDTH * scale
    let cs_h = CARD_DOC_HEIGHT * scale
    // Store layout for hit-testing
    board_ox = (fw - float(BOARD_COLS) * cs_w - total_pad_x) * 0.5
    board_oy = (fh - float(BOARD_ROWS) * cs_h - total_pad_y) * 0.5
    board_scale = scale
    board_cs_w = cs_w
    board_cs_h = cs_h
    board_pad_cur = pad
    // Pass 1: draw all cards
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let idx = row * BOARD_COLS + col
            let card_x = board_ox + pad + float(col) * (cs_w + pad)
            let card_y = board_oy + pad + float(row) * (cs_h + pad)
            var tint = float4(1.0, 1.0, 1.0, 1.0)
            let chip = get_chip(game, row, col)
            if (chip != ChipColor.none) {
                let cc = chip_color_to_float4(chip)
                if (is_in_sequence(game, row, col)) {
                    // Stronger tint for sequence cards
                    let st = float(seq_card_tint)
                    tint = float4(lerp(1.0, cc.x, st), lerp(1.0, cc.y, st), lerp(1.0, cc.z, st), 1.0)
                } else {
                    tint = float4(lerp(1.0, cc.x, 0.15), lerp(1.0, cc.y, 0.15), lerp(1.0, cc.z, 0.15), 1.0)
                }
            }
            if (row == hover_row && col == hover_col && !is_corner(row, col)) {
                if (selected_card >= 0 && game.phase == GamePhase.playing && game.current_player == 0) {
                    let m = Move(card_idx = selected_card, row = row, col = col)
                    if (is_legal_move(game, 0, m)) {
                        tint *= float4(0.6, 1.4, 0.6, 1.0)  // green = legal
                    } else {
                        tint *= float4(1.4, 0.5, 0.5, 1.0)  // red = illegal
                    }
                } else {
                    tint *= hover_tint
                }
            }
            let card_idx = board_card_indices[idx]
            if (card_idx >= 0) {
                draw_card(renderer, deck, card_idx, card_x, card_y, scale, screen_w, screen_h,
                    [tint = tint])
            } elif (back_index >= 0) {
                draw_card(renderer, deck, back_index, card_x, card_y, scale, screen_w, screen_h,
                    [tint = tint * float4(1.0, 0.85, 0.4, 1.0)])
            }
        }
    }
    // Pass 2: draw chips on top (sequence chips pulse)
    let chip_r = cs_w * float(chip_radius_frac)
    let pulse = float(seq_pulse_min) + (1.0 - float(seq_pulse_min)) * (0.5 + 0.5 * sin(get_uptime() * float(seq_pulse_speed)))
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let chip = get_chip(game, row, col)
            if (chip != ChipColor.none) {
                let cx = board_ox + pad + float(col) * (cs_w + pad) + cs_w * 0.5
                let cy = board_oy + pad + float(row) * (cs_h + pad) + cs_h * 0.5
                var color = chip_color_to_float4(chip)
                var outline = float4(0.0, 0.0, 0.0, 0.9)
                var r = chip_r
                if (is_in_sequence(game, row, col)) {
                    // Gentle pulse + white outline
                    let boost = 0.15 * pulse
                    color = float4(min(color.x + boost, 1.0), min(color.y + boost, 1.0), min(color.z + boost, 1.0), 1.0)
                    outline = float4(1.0, 1.0, 1.0, 0.6 + 0.4 * pulse)
                    r *= 1.0 + 0.1 * pulse
                }
                draw_chip(chip_renderer, cx, cy, r, color, outline, screen_w, screen_h)
            }
        }
    }
}

// --- Live commands ---

struct CmdPlaceChipArgs {
    row : int
    col : int
    color : int = 1  // 1=blue, 2=green, 3=red, 4=yellow, 0=none
}

[live_command(description="Place chip. Args: row, col, color (0=none,1=blue,2=green,3=red,4=yellow)")]
def cmd_place_chip(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdPlaceChipArgs>)
    place_chip(game, args.row, args.col, unsafe(reinterpret<ChipColor>(args.color)))
    return JV(args)
}

[live_command(description="Clear all chips from the board")]
def cmd_clear_board(input : JsonValue?) : JsonValue? {
    clear_all_chips(game)
    return JV("cleared")
}

struct CmdNewGameArgs {
    num_players : int = 2
    seed : int = -1  // -1 = use uptime as seed
}

[live_command(description="Start a new game. Args: num_players (2-4), seed (-1=random)")]
def cmd_new_game(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdNewGameArgs>)
    let seed = args.seed >= 0 ? args.seed : int(get_uptime() * 1000.0)
    make_game(game, args.num_players, seed)
    sort_hand(game, 0)
    selected_card = -1
    bot_timer = float(bot_delay)
    return JV("{game.num_players} players, seed={seed}")
}

struct CmdGameStatusResult {
    phase : string
    num_players : int
    current_player : int
    draw_pile_size : int
    cheat_mode : bool
}

[live_command(description="Get current game status")]
def cmd_game_status(input : JsonValue?) : JsonValue? {
    var phase_str = "not_started"
    if (game.phase == GamePhase.playing) {
        phase_str = "playing"
    } elif (game.phase == GamePhase.game_over) {
        phase_str = "game_over"
    }
    return JV(CmdGameStatusResult(
        phase = phase_str,
        num_players = game.num_players,
        current_player = game.current_player,
        draw_pile_size = length(game.draw_pile),
        cheat_mode = cheat_mode
    ))
}

[live_command(description="Debug hand layout info")]
def cmd_debug_hands(input : JsonValue?) : JsonValue? {
    var info = "board_scale={board_scale} board_ox={board_ox} board_cs_w={board_cs_w} board_cs_h={board_cs_h}\n"
    for (p in range(game.num_players)) {
        info += "P{p}: {get_hand_size(game, p)} cards: "
        for (c in range(get_hand_size(game, p))) {
            info += get_hand_card(game, p, c)
            if (c < get_hand_size(game, p) - 1) {
                info += ", "
            }
        }
        info += "\n"
    }
    return JV(info)
}

[live_command(description="Find and display all completed sequences")]
def cmd_check_sequences(input : JsonValue?) : JsonValue? {
    find_sequences(game)
    var info = "{length(game.sequences)} sequences found\n"
    for (seq in game.sequences) {
        var color_name = "unknown"
        if (seq.color == ChipColor.blue) { color_name = "blue"; }
        elif (seq.color == ChipColor.green) { color_name = "green"; }
        elif (seq.color == ChipColor.red) { color_name = "red"; }
        elif (seq.color == ChipColor.yellow) { color_name = "yellow"; }
        info += "  {color_name}: "
        for (i in range(5)) {
            if (i > 0) { info += ", "; }
            info += "({seq.cells[i].y},{seq.cells[i].x})"
        }
        info += "\n"
    }
    let w = check_win(game)
    if (w >= 0) {
        info += "WINNER: player {w}\n"
    }
    return JV(info)
}

[live_command(description="Toggle cheat mode (show all hands)")]
def cmd_toggle_cheat(input : JsonValue?) : JsonValue? {
    cheat_mode = !cheat_mode
    return JV(cheat_mode)
}

struct CmdSetHandArgs {
    player : int = 0
    cards : array<string>
}

[live_command(description="Set a player's hand. Args: player (0-3), cards (array of card names)")]
def cmd_set_hand(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdSetHandArgs>)
    if (game.phase == GamePhase.not_started) {
        game.phase = GamePhase.playing
    }
    if (args.player >= game.num_players) {
        game.num_players = args.player + 1
    }
    set_hand(game, args.player, args.cards)
    sort_hand(game, args.player)
    selected_card = -1
    return JV("{length(args.cards)} cards set for player {args.player}")
}

struct CmdForceMoveArgs {
    player : int = -1   // -1 = current player
    card_idx : int = 0
    row : int
    col : int
}

[live_command(description="Force a move through apply_move (triggers sequences/win). Args: player, card_idx, row, col")]
def cmd_force_move(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdForceMoveArgs>)
    let p = args.player >= 0 ? args.player : game.current_player
    let m = Move(card_idx = args.card_idx, row = args.row, col = args.col)
    let card = get_hand_card(game, p, args.card_idx)
    if (apply_move(game, p, m)) {
        return JV("P{p} played {card} at ({args.row},{args.col}), phase={game.phase}, winner={game.winner}")
    }
    return JV("move failed: P{p} card_idx={args.card_idx} at ({args.row},{args.col})")
}

struct CmdSetPhaseArgs {
    phase : string = "playing"  // "not_started", "playing", "game_over"
    winner : int = -1
}

[live_command(description="Set game phase. Args: phase (not_started/playing/game_over), winner (-1=none)")]
def cmd_set_phase(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdSetPhaseArgs>)
    if (args.phase == "not_started") {
        game.phase = GamePhase.not_started
    } elif (args.phase == "playing") {
        game.phase = GamePhase.playing
    } elif (args.phase == "game_over") {
        game.phase = GamePhase.game_over
    }
    game.winner = args.winner
    selected_card = -1
    return JV("phase={args.phase}, winner={game.winner}")
}

struct CmdSetPlayerArgs {
    player : int = 0
}

[live_command(description="Set current player. Args: player (0-3)")]
def cmd_set_player(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdSetPlayerArgs>)
    game.current_player = clamp(args.player, 0, game.num_players - 1)
    selected_card = -1
    return JV("current_player={game.current_player}")
}

[live_command(description="Trigger one bot move for current player (or specified player)")]
def cmd_bot_move(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<CmdSetPlayerArgs>)  // reuse: just needs player
    let p = args.player >= 0 ? args.player : game.current_player
    if (p == 0) {
        return JV("player 0 is human, use board click")
    }
    if (game.phase != GamePhase.playing) {
        return JV("game not in playing phase")
    }
    discard_dead_cards(game, p)
    bot_rng_seed++
    let m = bot_random_move(game, p, bot_rng_seed)
    if (m.card_idx >= 0) {
        let card = get_hand_card(game, p, m.card_idx)
        if (apply_move(game, p, m)) {
            bot_timer = float(bot_delay)
            return JV("P{p} played {card} at ({m.row},{m.col}), next=P{game.current_player}")
        }
    }
    return JV("P{p} has no legal moves")
}

[live_command(description="Reset board: clear chips, sequences, set phase to playing")]
def cmd_reset_board(input : JsonValue?) : JsonValue? {
    clear_all_chips(game)
    game.sequences |> clear()
    game.phase = GamePhase.playing
    game.winner = -1
    selected_card = -1
    return JV("board reset, phase=playing")
}

// --- Lifecycle ---

[export]
def shutdown() {
    print("sequence: shutdown\n")
    delete_chip_renderer(chip_renderer)
    live_destroy_window()
}

// Dual-mode: also works with regular daslang.exe
[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
