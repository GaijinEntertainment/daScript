options gen2
options persistent_heap

require live/glfw_live
require live/opengl_live
require live/live_api
require live/live_vars
require live/live_watch_boost
require live_host
require opengl/opengl_boost
require cards/opengl_cards
require gameplay

// --- Tunable parameters ---

var @live board_pad = 2.0
var @live bg_color = float3(0.1, 0.15, 0.1)

// --- State ---

var renderer : CardRenderer
var deck = GpuDeck()
var back_index : int = -1

// Precomputed card indices for each board cell (100 entries, -1 for FREE corners using back)
var board_card_indices : int[100]

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
    // Center the board
    let board_w = float(BOARD_COLS) * cs_w + total_pad_x
    let board_h = float(BOARD_ROWS) * cs_h + total_pad_y
    let ox = (fw - board_w) * 0.5
    let oy = (fh - board_h) * 0.5
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let idx = row * BOARD_COLS + col
            let card_x = ox + pad + float(col) * (cs_w + pad)
            let card_y = oy + pad + float(row) * (cs_h + pad)
            let card_idx = board_card_indices[idx]
            if (card_idx >= 0) {
                draw_card(renderer, deck, card_idx, card_x, card_y, scale, screen_w, screen_h)
            } elif (back_index >= 0) {
                draw_card(renderer, deck, back_index, card_x, card_y, scale, screen_w, screen_h,
                    [tint = float4(1.0, 0.85, 0.4, 1.0)])
            }
        }
    }
}

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
