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

var @live board_pad_x = 4.0
var @live board_pad_y = 2.0
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

// Build a 90° CW rotation matrix around the card's center in clip space
// m[i] = column i (OpenGL column-major convention)
def make_rotate90_extra(x, y, scale : float; screen_w, screen_h : int) : float4x4 {
    let fw = float(screen_w)
    let fh = float(screen_h)
    // Card center in screen coords
    let cx = x + CARD_DOC_WIDTH * scale * 0.5
    let cy = y + CARD_DOC_HEIGHT * scale * 0.5
    // Card center in NDC
    let ncx = 2.0 * cx / fw - 1.0
    let ncy = 1.0 - 2.0 * cy / fh
    // T(c) * R_ccw * T(-c) in column-major
    // R_ccw(90°): col0=(0,1,0,0) col1=(-1,0,0,0)
    var m : float4x4
    m[0] = float4(0.0, 1.0, 0.0, 0.0)
    m[1] = float4(-1.0, 0.0, 0.0, 0.0)
    m[2] = float4(0.0, 0.0, 1.0, 0.0)
    m[3] = float4(ncx + ncy, ncy - ncx, 0.0, 1.0)
    return m
}

def draw_board(screen_w, screen_h : int) {
    let fw = float(screen_w)
    let fh = float(screen_h)
    // Rotated cell dimensions: card height becomes cell width, card width becomes cell height
    let cell_w = CARD_DOC_HEIGHT
    let cell_h = CARD_DOC_WIDTH
    // Compute scale to fit 10 columns of rotated cards
    let total_pad_x = board_pad_x * float(BOARD_COLS + 1)
    let total_pad_y = board_pad_y * float(BOARD_ROWS + 1)
    let scale_x = (fw - total_pad_x) / (float(BOARD_COLS) * cell_w)
    let scale_y = (fh - total_pad_y) / (float(BOARD_ROWS) * cell_h)
    let scale = min(scale_x, scale_y)
    let cs_w = cell_w * scale
    let cs_h = cell_h * scale
    // Center the board
    let board_w = float(BOARD_COLS) * cs_w + float(BOARD_COLS + 1) * board_pad_x
    let board_h = float(BOARD_ROWS) * cs_h + float(BOARD_ROWS + 1) * board_pad_y
    let ox = (fw - board_w) * 0.5
    let oy = (fh - board_h) * 0.5
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let idx = row * BOARD_COLS + col
            // Grid position (where the rotated card should appear)
            let gx = ox + board_pad_x + float(col) * (cs_w + board_pad_x)
            let gy = oy + board_pad_y + float(row) * (cs_h + board_pad_y)
            // Offset draw_card position so rotation lands at grid position
            let card_x = gx + (CARD_DOC_HEIGHT - CARD_DOC_WIDTH) * scale * 0.5
            let card_y = gy + (CARD_DOC_WIDTH - CARD_DOC_HEIGHT) * scale * 0.5
            let rot = make_rotate90_extra(card_x, card_y, scale, screen_w, screen_h)
            let card_idx = board_card_indices[idx]
            if (card_idx >= 0) {
                draw_card(renderer, deck, card_idx, card_x, card_y, scale, screen_w, screen_h,
                    [extra = rot])
            } elif (back_index >= 0) {
                draw_card(renderer, deck, back_index, card_x, card_y, scale, screen_w, screen_h,
                    [tint = float4(1.0, 0.85, 0.4, 1.0), extra = rot])
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
