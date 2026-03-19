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

// --- State ---

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
    delete_chip_renderer(chip_renderer)
    chip_renderer = create_chip_renderer()
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
    // Pass 1: draw all cards
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let idx = row * BOARD_COLS + col
            let card_x = board_ox + pad + float(col) * (cs_w + pad)
            let card_y = board_oy + pad + float(row) * (cs_h + pad)
            var tint = float4(1.0, 1.0, 1.0, 1.0)
            let chip = get_chip(row, col)
            if (chip != ChipColor.none) {
                let cc = chip_color_to_float4(chip)
                tint = float4(lerp(1.0, cc.x, 0.15), lerp(1.0, cc.y, 0.15), lerp(1.0, cc.z, 0.15), 1.0)
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
    // Pass 2: draw chips on top
    let chip_r = cs_w * float(chip_radius_frac)
    for (row in range(BOARD_ROWS)) {
        for (col in range(BOARD_COLS)) {
            let chip = get_chip(row, col)
            if (chip != ChipColor.none) {
                let cx = board_ox + pad + float(col) * (cs_w + pad) + cs_w * 0.5
                let cy = board_oy + pad + float(row) * (cs_h + pad) + cs_h * 0.5
                let color = chip_color_to_float4(chip)
                let outline = float4(0.0, 0.0, 0.0, 0.9)
                draw_chip(chip_renderer, cx, cy, chip_r, color, outline, screen_w, screen_h)
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
