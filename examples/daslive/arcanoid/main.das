options gen2

require glfw/glfw_boost
require glfw/glfw_live
require opengl/opengl_boost
require opengl/opengl_gen
require opengl/opengl_cache
require daslib/defer
require daslib/safe_addr
require daslib/decs_boost
// require daslib/decs_live  // our templates don't serialize; restart DECS on reload
require daslib/opengl_live
require daslib/live_commands
require daslib/live_api
require daslib/json
require daslib/json_boost
require daslib/math_boost
require daslib/archive
require opengl/opengl_ttf
require live_host

// --- Constants ---

let LIGHT_DIRECTION = normalize(float3(-1.0, -3.0, 2.0))

let FIELD_W = 10.0
let FIELD_H = 14.0
let WALL_THICKNESS = 0.5
let WALL_HEIGHT = 0.6
let GROUND_Y = -0.1

let WALL_COLOR = float3(0.4, 0.4, 0.5)
let GROUND_COLOR = float3(0.15, 0.12, 0.2)
let PADDLE_COLOR = float3(0.2, 0.6, 1.0)
let PADDLE_W = 2.0
let PADDLE_H = 0.3
let PADDLE_D = 0.4
let PADDLE_Z = 0.5
let PADDLE_SPEED = 8.0

let BALL_COLOR = float3(1.0, 0.9, 0.3)
let BALL_RADIUS = 0.2
let BALL_SPEED = 8.0
let BALL_START_Z = 1.5

let BRICK_COLS = 8
let BRICK_ROWS = 5
let BRICK_W = 1.0
let BRICK_D = 0.5
let BRICK_H = 0.4
let BRICK_GAP = 0.1
let BRICK_START_Z = 8.0

let BRICK_COLORS = fixed_array(
    float3(0.9, 0.2, 0.2),
    float3(0.9, 0.5, 0.1),
    float3(0.9, 0.9, 0.2),
    float3(0.2, 0.8, 0.3),
    float3(0.3, 0.5, 0.9)
)

let BRICK_POINTS = fixed_array(50, 40, 30, 20, 10)
let START_LIVES = 3

// --- Shaders ---

var @in @location = 0 v_position : float3
var @in @location = 1 v_normal   : float3
var @in @location = 2 v_texcoord : float2
var @uniform v_model : float4x4
var @uniform v_view : float4x4
var @uniform v_projection : float4x4
var @inout f_normal : float3
var @inout f_position : float3
var @uniform f_Color : float4
var @out f_FragColor : float4

[vertex_program]
def vs_arcanoid {
    f_normal = normalize(float3x3(v_model) * v_normal)
    let tpos = v_model * float4(v_position, 1.0)
    f_position = tpos.xyz
    gl_Position = v_projection * v_view * tpos
}

[fragment_program]
def fs_arcanoid {
    let normal = normalize(f_normal)
    let k = saturate(-dot(LIGHT_DIRECTION, normal))
    f_FragColor.xyz = f_Color.xyz * (k * 0.8 + 0.2)
    f_FragColor.w = f_Color.w
}

// --- GL objects ---

var program : uint
var cube : OpenGLGeometryFragment
var ground_plane : OpenGLGeometryFragment
var sphere : OpenGLGeometryFragment
var hud_font : Font?

def create_gl_objects() {
    program = cache_shader_program(vs_arcanoid`shader_text, fs_arcanoid`shader_text)
    cube <- create_geometry_fragment <| gen_cube()
    ground_plane <- create_geometry_fragment <| gen_plane(GenDirection.xz)
    sphere <- create_geometry_fragment <| gen_sphere(16, 8, false)
    cache_ttf_objects()
    hud_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
}

def delete_gl_objects() {
    // GL objects are re-created each init; old ones leak but are cheap
}

// --- Drawing helpers ---

var display_w = 0
var display_h = 0

def draw_box(pos : float3; dim : float3; color : float3) {
    v_model = compose(pos, float4(0), dim * 0.5)
    f_Color = float4(color, 1.0)
    vs_arcanoid_bind_uniform(program)
    fs_arcanoid_bind_uniform(program)
    cube |> draw_geometry_fragment()
}

def draw_sphere(pos : float3; radius : float; color : float3) {
    v_model = compose(pos, float4(0), float3(radius))
    f_Color = float4(color, 1.0)
    vs_arcanoid_bind_uniform(program)
    fs_arcanoid_bind_uniform(program)
    sphere |> draw_geometry_fragment()
}

def draw_ground() {
    glDisable(GL_CULL_FACE)
    v_model = compose(float3(0.0, GROUND_Y, FIELD_H * 0.5), float4(0), float3(FIELD_W * 0.6, 1.0, FIELD_H * 0.6))
    f_Color = float4(GROUND_COLOR, 1.0)
    vs_arcanoid_bind_uniform(program)
    fs_arcanoid_bind_uniform(program)
    ground_plane |> draw_geometry_fragment()
    glEnable(GL_CULL_FACE)
}

// --- Walls ---

def draw_walls() {
    let hw = FIELD_W * 0.5
    // Left wall
    draw_box(
        float3(-hw - WALL_THICKNESS * 0.5, WALL_HEIGHT * 0.5, FIELD_H * 0.5),
        float3(WALL_THICKNESS, WALL_HEIGHT, FIELD_H + WALL_THICKNESS),
        WALL_COLOR
    )
    // Right wall
    draw_box(
        float3(hw + WALL_THICKNESS * 0.5, WALL_HEIGHT * 0.5, FIELD_H * 0.5),
        float3(WALL_THICKNESS, WALL_HEIGHT, FIELD_H + WALL_THICKNESS),
        WALL_COLOR
    )
    // Top wall
    draw_box(
        float3(0.0, WALL_HEIGHT * 0.5, FIELD_H + WALL_THICKNESS * 0.5),
        float3(FIELD_W + WALL_THICKNESS * 2.0, WALL_HEIGHT, WALL_THICKNESS),
        WALL_COLOR
    )
}

// --- Game state ---

var score = 0
var lives = START_LIVES
var game_over = false
var game_won = false
var space_was_pressed = false

// --- Paddle state ---

var paddle_x = 0.0

def update_paddle(dt : float) {
    if (glfwGetKey(live_window, int(GLFW_KEY_LEFT)) == int(GLFW_PRESS)) {
        paddle_x += PADDLE_SPEED * dt
    }
    if (glfwGetKey(live_window, int(GLFW_KEY_RIGHT)) == int(GLFW_PRESS)) {
        paddle_x -= PADDLE_SPEED * dt
    }
    let limit = FIELD_W * 0.5 - PADDLE_W * 0.5
    paddle_x = clamp(paddle_x, -limit, limit)
}

def draw_paddle() {
    draw_box(
        float3(paddle_x, PADDLE_H * 0.5, PADDLE_Z),
        float3(PADDLE_W, PADDLE_H, PADDLE_D),
        PADDLE_COLOR
    )
}

// --- Ball (DECS) ---

var tick_dt = 0.0

[decs_template]
struct Ball {
    pos : float3
    vel : float3
}

def spawn_ball(px, vx : float) {
    create_entity <| @(eid, cmp) {
        apply_decs_template(cmp, Ball(
            pos = float3(px, BALL_RADIUS, BALL_START_Z),
            vel = normalize(float3(vx, 0.0, 1.0)) * BALL_SPEED
        ))
    }
}

def has_balls() : bool {
    var found = false
    query <| $(b : Ball) {
        found = true
    }
    return found
}

[decs(stage=update)]
def update_balls(eid : EntityId; var b : Ball) {
    b.pos += b.vel * tick_dt
    let hw = FIELD_W * 0.5 - BALL_RADIUS
    // Left wall (screen-left = +X due to camera)
    if (b.pos.x > hw) {
        b.pos.x = hw
        b.vel.x = -b.vel.x
    }
    // Right wall
    if (b.pos.x < -hw) {
        b.pos.x = -hw
        b.vel.x = -b.vel.x
    }
    // Top wall
    if (b.pos.z > FIELD_H - BALL_RADIUS) {
        b.pos.z = FIELD_H - BALL_RADIUS
        b.vel.z = -b.vel.z
    }
    // Paddle collision
    let pz_min = PADDLE_Z - PADDLE_D * 0.5 - BALL_RADIUS
    let pz_max = PADDLE_Z + PADDLE_D * 0.5 + BALL_RADIUS
    let px_min = paddle_x - PADDLE_W * 0.5 - BALL_RADIUS
    let px_max = paddle_x + PADDLE_W * 0.5 + BALL_RADIUS
    if (b.vel.z < 0.0 && b.pos.z >= pz_min && b.pos.z <= pz_max && b.pos.x >= px_min && b.pos.x <= px_max) {
        b.pos.z = pz_max
        let offset = (b.pos.x - paddle_x) / (PADDLE_W * 0.5)
        let angle = offset * 0.7
        b.vel = normalize(float3(sin(angle), 0.0, cos(angle))) * length(b.vel)
    }
    // Ball fell below paddle — lose a life
    if (b.pos.z < -1.0) {
        delete_entity(eid)
        lives--
        if (lives <= 0) {
            game_over = true
        }
    }
}

[decs(stage=draw)]
def draw_balls(b : Ball) {
    draw_sphere(b.pos, BALL_RADIUS, BALL_COLOR)
}

// --- Bricks (DECS) ---

[decs_template]
struct Brick {
    pos   : float3
    color : float3
    row   : int
}

def spawn_bricks() {
    let total_w = float(BRICK_COLS) * (BRICK_W + BRICK_GAP) - BRICK_GAP
    let x_offset = -total_w * 0.5 + BRICK_W * 0.5
    for (row in range(BRICK_ROWS)) {
        let z = BRICK_START_Z + float(row) * (BRICK_D + BRICK_GAP)
        let color = BRICK_COLORS[row]
        for (col in range(BRICK_COLS)) {
            let x = x_offset + float(col) * (BRICK_W + BRICK_GAP)
            create_entity <| @(eid, cmp) {
                apply_decs_template(cmp, Brick(
                    pos = float3(x, BRICK_H * 0.5, z),
                    color = color,
                    row = row
                ))
            }
        }
    }
}

def has_bricks() : bool {
    var found = false
    query <| $(br : Brick) {
        found = true
    }
    return found
}

[decs(stage=draw)]
def draw_bricks(br : Brick) {
    draw_box(br.pos, float3(BRICK_W, BRICK_H, BRICK_D), br.color)
}


// --- Live commands ---

[live_command(description="Spawn a ball. Args: x, vx")]
def cmd_spawn_ball(input : JsonValue?) : JsonValue? {
    var x = paddle_x
    var vx = 0.3
    if (input != null && input.value is _object) {
        let tab & = unsafe(input.value as _object)
        var xv = tab?["x"] ?? null
        if (xv != null) {
            if (xv.value is _number) {
                x = float(xv.value as _number)
            } elif (xv.value is _longint) {
                x = float(xv.value as _longint)
            }
        }
        var vxv = tab?["vx"] ?? null
        if (vxv != null) {
            if (vxv.value is _number) {
                vx = float(vxv.value as _number)
            } elif (vxv.value is _longint) {
                vx = float(vxv.value as _longint)
            }
        }
    }
    spawn_ball(x, float(vx))
    return JV("\{\"spawned\": true}")
}

[live_command(description="Show score, lives, ball/brick counts")]
def cmd_game_status(input : JsonValue?) : JsonValue? {
    var ball_count = 0
    var brick_count = 0
    var ball_info = ""
    query <| $(b : Ball) {
        ball_count++
        ball_info = "pos=({b.pos.x},{b.pos.y},{b.pos.z}) vel=({b.vel.x},{b.vel.y},{b.vel.z})"
    }
    query <| $(br : Brick) {
        brick_count++
    }
    return JV("\{\"balls\": {ball_count}, \"bricks\": {brick_count}, \"score\": {score}, \"lives\": {lives}, \"game_over\": {game_over}, \"game_won\": {game_won}, \"paddle_x\": {paddle_x}, \"ball\": \"{ball_info}\"}")
}

[live_command(description="Spawn a new set of bricks")]
def cmd_spawn_bricks(input : JsonValue?) : JsonValue? {
    spawn_bricks()
    return JV("\{\"spawned\": true}")
}

[live_command(description="Dump DECS archetypes, entities, and queries")]
def cmd_decs_dump(input : JsonValue?) : JsonValue? {
    return JV("\{\"dump\": \"{debug_dump_string()}\"}")
}





// --- HUD ---

def draw_text(text : string; x, y : int; tint : float3 = float3(1.0)) {
    if (hud_font == null) { return; }
    var quads <- (*hud_font) |> create_quads(text)
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, x, y, tint)
    delete quads
}

def draw_text_centered(text : string; cx, y : int; tint : float3 = float3(1.0)) {
    if (hud_font == null) { return; }
    var quads <- (*hud_font) |> create_quads(text)
    let dim = quads_dim(quads)
    let tw = int(dim.vmax.x - dim.vmin.x)
    let x = cx - tw / 2
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, x, y, tint)
    delete quads
}

def draw_hud() {
    draw_text("SCORE: {score}", 10, 35, float3(1.0, 1.0, 1.0))
    draw_text("LIVES: {lives}", 10, 70, float3(0.3, 0.8, 1.0))
    let cx = display_w / 2
    let cy = display_h / 2
    if (game_over) {
        draw_text_centered("GAME OVER", cx, cy - 20, float3(1.0, 0.2, 0.2))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 25, float3(0.7, 0.7, 0.7))
    } elif (game_won) {
        draw_text_centered("YOU WIN!", cx, cy - 20, float3(0.2, 1.0, 0.3))
        draw_text_centered("SCORE: {score}", cx, cy + 25, float3(1.0, 1.0, 0.5))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 65, float3(0.7, 0.7, 0.7))
    } elif (!has_balls()) {
        draw_text_centered("PRESS SPACE TO LAUNCH", cx, cy + 25, float3(0.5, 0.5, 0.5))
    }
}

// --- State preservation across reloads ---

[before_reload]
def save_game_state() {
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    arch |> serialize(paddle_x)
    arch |> serialize(score)
    arch |> serialize(lives)
    arch |> serialize(game_over)
    arch |> serialize(game_won)
    var data <- ser->extractData()
    live_store_bytes("__arcanoid_state", data)
    unsafe { delete ser; }
}

[after_reload]
def restore_game_state() {
    var data : array<uint8>
    if (live_load_bytes("__arcanoid_state", data) && length(data) > 0) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        arch |> serialize(paddle_x)
        arch |> serialize(score)
        arch |> serialize(lives)
        arch |> serialize(game_over)
        arch |> serialize(game_won)
        unsafe { delete ser; }
    }
}

// --- Lifecycle ---

[export]
def init() {
    live_create_window("Arcanoid", 1024, 768)
    create_gl_objects()
    decs::restart()
    spawn_bricks()
    commit()
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    live_get_framebuffer_size(display_w, display_h)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.05, 0.02, 0.1, 1.0)
    glClearDepth(1.0lf)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)

    let aspect = display_h != 0 ? float(display_w) / float(display_h) : 1.0
    v_projection = perspective_rh_opengl(45.0 * PI / 180.0, aspect, 0.1, 100.0)
    v_view = look_at_rh(
        float3(0.0, 14.0, -4.0),
        float3(0.0, 0.0, 5.0),
        float3(0.0, 1.0, 0.0)
    )

    let dt = get_dt()
    tick_dt = min(dt, 1.0 / 30.0)
    let space_pressed = glfwGetKey(live_window, int(GLFW_KEY_SPACE)) == int(GLFW_PRESS)
    let space_just = space_pressed && !space_was_pressed
    space_was_pressed = space_pressed
    // Restart game
    if ((game_over || game_won) && space_just) {
        decs::restart()
        spawn_bricks()
        commit()
        score = 0
        lives = START_LIVES
        paddle_x = 0.0
        game_over = false
        game_won = false
    }
    if (!game_over && !game_won) {
        update_paddle(dt)
        if (!has_balls() && space_just) {
            spawn_ball(paddle_x, 0.3)
        }
        decs_stage("update")
        commit()
        // Ball-brick collision
        var hit_eid : EntityId
        var hit_ball_idx = -1
        var hit_side = false
        var hit_row = 0
        var ball_idx = 0
        query <| $(var b : Ball) {
            if (hit_ball_idx < 0) {
                find_query <| $(eid : EntityId; br : Brick) {
                    let dx = abs(b.pos.x - br.pos.x) - (BRICK_W * 0.5 + BALL_RADIUS)
                    let dz = abs(b.pos.z - br.pos.z) - (BRICK_D * 0.5 + BALL_RADIUS)
                    if (dx < 0.0 && dz < 0.0) {
                        hit_eid = eid
                        hit_ball_idx = ball_idx
                        hit_side = dx > dz
                        hit_row = br.row
                        if (hit_side) {
                            b.vel.x = -b.vel.x
                        } else {
                            b.vel.z = -b.vel.z
                        }
                        return true
                    }
                    return false
                }
            }
            ball_idx++
        }
        if (hit_ball_idx >= 0) {
            delete_entity(hit_eid)
            commit()
            score += BRICK_POINTS[hit_row]
            if (!has_bricks()) {
                game_won = true
            }
        }
    }

    glUseProgram(program)
    draw_ground()
    draw_walls()
    draw_paddle()
    decs_stage("draw")

    glDisable(GL_CULL_FACE)
    draw_hud()
    live_end_frame()
}

[export]
def shutdown() {
    delete_gl_objects()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
