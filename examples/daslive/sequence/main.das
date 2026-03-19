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
require gameplay

// --- Tunable parameters ---

var @live board_pad = 2.0
var @live bg_color = float3(0.1, 0.15, 0.1)
var @live hover_tint = float4(1.3, 1.3, 1.5, 1.0)

// --- State ---

var renderer : CardRenderer
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
    if (pressed && !mouse_was_pressed && hover_row >= 0 && hover_col >= 0) {
        let cur = get_chip(hover_row, hover_col)
        let nxt = next_chip_color(cur)
        place_chip(hover_row, hover_col, nxt)
    }
    mouse_was_pressed = pressed
}

// --- Lifecycle ---

[export]
def init() {
    live_create_window("Sequence", 1280, 800)
    renderer = create_card_renderer()
    deck <- load_gpu_deck()
    back_index = find_card(deck, "back")
    compute_board_card_indices()
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
        draw_board(w, h)
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
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let idx = row * BOARD_COLS + col
            let card_x = board_ox + pad + float(col) * (cs_w + pad)
            let card_y = board_oy + pad + float(row) * (cs_h + pad)
            // Determine tint: hover highlight, chip color, or default
            var tint = float4(1.0, 1.0, 1.0, 1.0)
            let chip = get_chip(row, col)
            if (chip != ChipColor.none) {
                tint = chip_color_to_float4(chip)
            }
            if (row == hover_row && col == hover_col) {
                tint *= hover_tint
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
    place_chip(args.row, args.col, unsafe(reinterpret<ChipColor>(args.color)))
    return JV(args)
}

[live_command(description="Clear all chips from the board")]
def cmd_clear_board(input : JsonValue?) : JsonValue? {
    clear_all_chips()
    return JV("cleared")
}

// --- Lifecycle ---

[export]
def shutdown() {
    print("sequence: shutdown\n")
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
