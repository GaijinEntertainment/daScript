options gen2
options persistent_heap

require rr_globals public
require river
require gameplay

// Live-command surface: drives the game from the REST API so a headless
// session can start a run, park the camera on a chosen section palette, and
// stage objects for a screenshot without touching the keyboard.

struct GameStatusResult {
    state : string
    score : int
    lives : int
    fuel : float
    section : int
    section_dist : float
    player_x : float
    player_y : float
    fwd_speed : float
    god_mode : bool
    // Input diagnostics: `focused` is the GLFW window attribute, the rest are
    // live glfwGetKey states. If focused is false the app is not receiving key
    // events at all, which is an activation problem, not a game-logic one.
    focused : bool
    key_left : bool
    key_right : bool
    key_up : bool
    key_down : bool
    key_space : bool
    boats : int
    planes : int
    helis : int
    bridges : int
    depots : int
    islands : int
    trees : int
    houses : int
    bonuses : int
    particles : int
}

[live_command(description = "Score, lives, fuel, section and per-kind entity counts")]
def cmd_game_status(_input : JsonValue?) : JsonValue? {
    var result = GameStatusResult(
        state = "{game_state}",
        score = score,
        lives = player_lives,
        fuel = player_fuel,
        section = current_section,
        section_dist = section_dist,
        player_x = player_pos.x,
        player_y = player_pos.y,
        fwd_speed = player_fwd_speed,
        god_mode = god_mode,
        focused = glfwGetWindowAttrib(live_window, int(GLFW_FOCUSED)) != 0,
        key_left = glfwGetKey(live_window, GLFW_KEY_LEFT) == GLFW_PRESS,
        key_right = glfwGetKey(live_window, GLFW_KEY_RIGHT) == GLFW_PRESS,
        key_up = glfwGetKey(live_window, GLFW_KEY_UP) == GLFW_PRESS,
        key_down = glfwGetKey(live_window, GLFW_KEY_DOWN) == GLFW_PRESS,
        key_space = glfwGetKey(live_window, GLFW_KEY_SPACE) == GLFW_PRESS
    )
    query() $(_b : EnemyBoat) { result.boats++; }
    query() $(_p : EnemyPlane) { result.planes++; }
    query() $(_h : EnemyHelicopter) { result.helis++; }
    query() $(_br : Bridge) { result.bridges++; }
    query() $(_d : FuelDepot) { result.depots++; }
    query() $(_i : Island) { result.islands++; }
    query() $(_t : RiverTree) { result.trees++; }
    query() $(_ho : RiverHouse) { result.houses++; }
    query() $(_bo : BonusPickup) { result.bonuses++; }
    query() $(_pa : Particle) { result.particles++; }
    return JV(result)
}

[live_command(description = "Restart the run from section 0 and enter playing state")]
def cmd_reset_game(_input : JsonValue?) : JsonValue? {
    reset_game()
    commit()
    return JV(true)
}

struct CmdSetStateArgs {
    state : GameState = GameState.playing
}

[live_command(description = "Force a game state. Args: state (menu|playing|paused|game_over_state|win_state)")]
def cmd_set_state(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdSetStateArgs>)
    game_state = args.state
    restart_input_lock = 0.0
    return JV(args)
}

struct CmdGodModeArgs {
    enabled : bool = true
}

[live_command(description = "Invulnerable + no fuel drain, for unattended capture. Args: enabled")]
def cmd_god_mode(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdGodModeArgs>)
    god_mode = args.enabled
    if (god_mode) {
        player_fuel = PLAYER_FUEL_MAX
    }
    return JV(args)
}

struct CmdSteerArgs {
    x : float = 0.0
    speed : float = 0.0
}

[live_command(description = "Teleport the player across the channel and optionally pin forward speed. Args: x, speed")]
def cmd_steer(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdSteerArgs>)
    let bounds = river_clamp_x_for(player_pos.y, args.x)
    let margin = PLAYER_SIZE * 1.5
    player_pos.x = clamp(args.x, bounds.x + margin, bounds.y - margin)
    player_vel.x = 0.0
    if (args.speed > 0.0) {
        player_fwd_speed = clamp(args.speed, PLAYER_FWD_SPEED_MIN,
            PLAYER_FWD_SPEED_MAX * section_speed_mult())
    }
    return JV(args)
}

struct CmdSetSectionArgs {
    section : int = 0
}

[live_command(description = "Jump to a section palette and repopulate it. Args: section (0..9)")]
def cmd_set_section(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdSetSectionArgs>)
    current_section = clamp(args.section, 0, MAX_SECTIONS - 1)
    section_dist = 0.0
    set_env_immediate(section_idx())
    spawn_section_objects(current_section, player_pos.y + 40.0)
    commit()
    return JV(args)
}

struct CmdBoomArgs {
    ahead : float = 14.0
    offset : float = 0.0
    height : float = 1.0
    scale : float = 1.0
}

[live_command(description = "Detonate an explosion ahead of the player, for tuning. Args: ahead, offset, height, scale")]
def cmd_boom(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdBoomArgs>)
    let at = float3(player_pos.x + args.offset, player_pos.y + args.ahead, args.height)
    spawn_explosion(at, float3(1.0, 0.62, 0.12), 1.0, args.scale)
    add_screen_shake(SHAKE_MED)
    commit()
    return JV(args)
}

struct CmdFxFreezeArgs {
    enabled : bool = true
}

[live_command(description = "Halt particle ageing so a blast can be inspected frame by frame. Args: enabled")]
def cmd_fx_freeze(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdFxFreezeArgs>)
    fx_freeze = args.enabled
    return JV(args)
}

struct CmdFxStepArgs {
    dt : float = 0.05
}

[live_command(description = "Advance frozen particles by dt seconds. Args: dt")]
def cmd_fx_step(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdFxStepArgs>)
    step_particles(args.dt)
    return JV(args)
}

enum SpawnKind {
    boat
    plane
    heli
    depot
    bridge
    island
    tree
    house
    bonus
}

struct CmdSpawnArgs {
    kind : SpawnKind = SpawnKind.boat
    ahead : float = 22.0
    offset : float = 0.0
}

[live_command(description = "Stage one object ahead of the player. Args: kind, ahead, offset")]
def cmd_spawn(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdSpawnArgs>)
    let y = player_pos.y + args.ahead
    let bounds = river_clamp_x_for(y, player_pos.x + args.offset)
    let x = clamp(player_pos.x + args.offset, bounds.x + 0.6, bounds.y - 0.6)
    if (args.kind == SpawnKind.boat) {
        spawn_enemy_boat(float3(x, y, BOAT_Z))
    } elif (args.kind == SpawnKind.plane) {
        spawn_enemy_plane(float3(x, y, PLANE_Z), current_section)
    } elif (args.kind == SpawnKind.heli) {
        spawn_enemy_heli(float3(x, y, ENEMY_HELI_Z))
    } elif (args.kind == SpawnKind.depot) {
        spawn_fuel_depot(float3(x, y, FUEL_DEPOT_Z))
    } elif (args.kind == SpawnKind.bridge) {
        spawn_bridge(float3(0.0, y, BRIDGE_Z), current_section)
    } elif (args.kind == SpawnKind.island) {
        spawn_island(float3(x, y, ISLAND_Z))
    } elif (args.kind == SpawnKind.tree) {
        spawn_river_tree(float3(bounds.y + 3.0, y, 0.0), 2.0, 3, 0.5, 0.0, 0.35)
    } elif (args.kind == SpawnKind.house) {
        spawn_river_house(float3(bounds.y + 5.0, y, 0.0), 1.6, 0.3, 0.4)
    } else {
        spawn_bonus_pickup(float3(x, y, 0.0), BonusType.multishot)
    }
    commit()
    return JV(args)
}
