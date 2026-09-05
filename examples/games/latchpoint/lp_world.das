options gen2
options persistent_heap

require daslib/math_boost public
require live/live_vars public
require live_host public
require daslib/fio

struct SavedPose {
    version : int = 1
    player : float3
    patch : float3
    yaw : float
    tilt : float
    patch_yaw : float
}

struct WorldState {
    inventory : array<string>
    tasks : array<string>
    patch_repaired : bool
    patch_following : bool
    power : bool
    coil_installed : bool
    workshop_open : bool
    archive_open : bool
    archive_inspected : bool
    brack_actuator_lent : bool
    salvaged : array<string>
    events : array<string>
}

struct Barrier {
    center : float2
    half_size : float2
    render_wall : bool = true
}

var @live avatar = float3(0.0, -9.0, 1.65)
var @live heading = 0.0
var @live pitch = -0.06
var @live patch_position = float3(-3.2, 0.2, 0.0)
var @live patch_heading = 0.0
var patch_walk_blend = 0.0
var @live world : WorldState
var @live workshop_aperture = -1.0
var @live archive_aperture = -1.0
var starter_progress = 0.0
var starter_engaged = false
let PATCH_SERVICE_POSITION = float3(7.5, 9.5, 0.0)

def patch_can_assist() : bool {
    return world.patch_repaired && world.patch_following && length(patch_position.xy - PATCH_SERVICE_POSITION.xy) < 0.9
}
var barriers : array<Barrier>
let BRACK_POSITION = float3(1.6, 3.5, 0.0)
let VESPER_POSITION = float3(-4.4, 8.1, 0.0)
let GENERATOR_POSITION = float3(7.5, 7.2, 0.0)
let COIL_POSITION = float3(10.0, 1.0, 0.0)
let ARCHIVE_RECORD_POSITION = float3(-4.0, 13.1, 1.4)
let SALVAGE_POSITION = float3(-9.0, 2.5, 0.0)
let ACTUATOR_CRATE_CENTER = float3(-9.2, 3.0, 0.5)
let ACTUATOR_CRATE_HALF_SIZE = float3(0.4, 0.4, 0.5)
let ACTUATOR_RADIUS = 0.12
let SALVAGE_CRATE_SPACING = 1.2

def actuator_center() : float3 {
    return ACTUATOR_CRATE_CENTER + float3(0.0, 0.0, ACTUATOR_CRATE_HALF_SIZE.z + ACTUATOR_RADIUS)
}

def actuator_available() : bool {
    return find_index(world.salvaged, "actuator") < 0
}

def reset_avatar(pose_path : string = "") {
    avatar = float3(0.0, -9.0, 1.65)
    heading = 0.0
    pitch = -0.06
    patch_position = float3(-3.2, 0.2, 0.0)
    patch_heading = 0.0
    let destination = empty(pose_path) ? avatar_save_path() : pose_path
    if (!save_avatar_to(destination)) { to_log(LOG_WARNING, "Could not save Latchpoint avatar position.") }
}

def avatar_save_allowed(menu_is_open : bool) : bool {
    return !menu_is_open
}

def avatar_save_path() : string {
    return path_join(get_das_root(), "examples/games/latchpoint/saves/scene-pose.json")
}

def save_avatar_to(path : string) : bool {
    let pose = SavedPose(player = avatar, patch = patch_position, yaw = heading, tilt = pitch, patch_yaw = patch_heading)
    return fwrite(path, sprint_json(pose, false))
}

def save_avatar_on_shutdown(menu_is_open : bool; path : string) : bool {
    return false if (!avatar_save_allowed(menu_is_open))
    return save_avatar_to(path)
}

def save_avatar() {
    if (!save_avatar_to(avatar_save_path())) { to_log(LOG_WARNING, "Could not save Latchpoint avatar position.") }
}

def apply_saved_pose(pose : SavedPose) : bool {
    if (pose.version != 1 || !(abs(pose.player.x) < 24.0 && abs(pose.player.y) < 24.0 &&
            pose.player.z > 0.2 && pose.player.z < 8.0 && abs(pose.yaw) < 100000.0 && abs(pose.tilt) <= 1.1 &&
            abs(pose.patch.x) < 24.0 && abs(pose.patch.y) < 24.0 && abs(pose.patch.z) < 1.0 && abs(pose.patch_yaw) < 100000.0)) return false
    avatar = pose.player
    patch_position = pose.patch
    patch_heading = pose.patch_yaw
    heading = pose.yaw
    pitch = pose.tilt
    return true
}

def load_avatar() {
    let path = avatar_save_path()
    return if (!fexist(path))
    var pose = SavedPose(version = 0)
    if (sscan_json(fread(path), pose)) { apply_saved_pose(pose) }
}

def resident_position(index : int) : float3 {
    return BRACK_POSITION if (index == 0)
    return patch_position if (index == 1)
    return VESPER_POSITION
}

def resident_id(index : int) : string {
    return "brack" if (index == 0)
    return "patch" if (index == 1)
    return "vesper"
}

def resident_name(index : int) : string {
    return "BRACK" if (index == 0)
    return "PATCH" if (index == 1)
    return "VESPER"
}

def init_barriers() {
    delete barriers
    barriers <- [
        Barrier(center = float2(0.0, -13.0), half_size = float2(14.0, 0.4)),
        Barrier(center = float2(0.0, 15.0), half_size = float2(14.0, 0.4)),
        Barrier(center = float2(-14.0, 1.0), half_size = float2(0.4, 14.0)),
        Barrier(center = float2(14.0, 1.0), half_size = float2(0.4, 14.0)),
        Barrier(center = float2(7.5, 7.2), half_size = float2(1.6, 1.3), render_wall = false),
        Barrier(center = float2(8.0, -3.8), half_size = float2(4.0, 0.3)),
        Barrier(center = float2(8.0, 4.8), half_size = float2(4.0, 0.3)),
        Barrier(center = float2(12.0, 0.5), half_size = float2(0.3, 4.6)),
        Barrier(center = float2(4.0, -2.6), half_size = float2(0.3, 1.5)),
        Barrier(center = float2(4.0, 3.1), half_size = float2(0.3, 1.7)),
        Barrier(center = float2(-7.0, 12.5), half_size = float2(0.3, 2.5)),
        Barrier(center = float2(-1.0, 12.5), half_size = float2(0.3, 2.5)),
        Barrier(center = float2(-6.1, 10.3), half_size = float2(0.9, 0.3)),
        Barrier(center = float2(-1.8, 10.3), half_size = float2(0.8, 0.3))]
}

def circle_blocked(p : float2; radius : float; wall : Barrier) : bool {
    let delta = abs(p - wall.center) - wall.half_size
    return length(max(delta, float2(0.0))) < radius
}

def can_stand(p : float2; radius : float = 0.33; avoid_residents : bool = false) : bool {
    for (wall in barriers) {
        return false if (circle_blocked(p, radius, wall))
    }
    if (avoid_residents) {
        for (i in range(3)) {
            let body_radius = i == 0 ? 0.95 : 0.60
            return false if (length(p - resident_position(i).xy) < radius + body_radius)
        }
    }
    return !(((!world.workshop_open || (workshop_aperture >= 0.0 && workshop_aperture < 0.70)) && circle_blocked(p, radius,
        Barrier(center = float2(4.0, 0.3), half_size = float2(0.3, 1.15)))) ||
        ((!world.archive_open || (archive_aperture >= 0.0 && archive_aperture < 0.70)) && circle_blocked(p, radius,
        Barrier(center = float2(-4.0, 10.3), half_size = float2(1.2, 0.3)))))
}

def slide_step(p, delta : float2; radius : float = 0.33; avoid_residents : bool = false) : float2 {
    var moved = p
    let steps = max(1, int(ceil(length(delta) / 0.12)))
    let step = delta / float(steps)
    for (_step in range(steps)) {
        if (can_stand(float2(moved.x + step.x, moved.y), radius, avoid_residents)) { moved.x += step.x }
        if (can_stand(float2(moved.x, moved.y + step.y), radius, avoid_residents)) { moved.y += step.y }
    }
    return moved
}

def nearest_resident() : int {
    var nearest = -1
    var distance = 3.5
    for (i in range(3)) {
        let d = length(avatar.xy - resident_position(i).xy)
        if (d < distance) {
            distance = d
            nearest = i
        }
    }
    return nearest
}

def physical_at_avatar() : string {
    if (world.archive_open && avatar.y > 10.65 && length(avatar.xy - ARCHIVE_RECORD_POSITION.xy) < 2.0) { return "inspect archive record" }
    if (length(avatar.xy - patch_position.xy) < 2.7 && !world.patch_repaired) return "install actuator"
    if (length(avatar.xy - GENERATOR_POSITION.xy) < 3.1 && !world.power) return world.coil_installed ? "start generator" : "install coil"
    if (length(avatar.xy - COIL_POSITION.xy) < 2.0 && world.workshop_open && find_index(world.salvaged, "coil") < 0) return "salvage coil"
    if (length(avatar.xy - SALVAGE_POSITION.xy) < 2.0 && actuator_available()) return "salvage actuator"
    return ""
}

def tick_follow(dt : float) {
    patch_walk_blend = max(0.0, patch_walk_blend - dt * 6.0)
    return if (!world.patch_following || !world.patch_repaired)
    let servicing = world.coil_installed && !world.power && length(avatar.xy - GENERATOR_POSITION.xy) < 3.5 && length(patch_position.xy - GENERATOR_POSITION.xy) < 6.0
    let destination = servicing ? PATCH_SERVICE_POSITION.xy : avatar.xy
    let stop_distance = servicing ? 0.15 : 2.1
    let delta = destination - patch_position.xy
    let distance = length(delta)
    if (distance > 0.01 || servicing) {
        let attention = servicing && distance < 0.9 ? GENERATOR_POSITION.xy - patch_position.xy : delta
        let target = atan2(attention.x, -attention.y)
        let turn = atan2(sin(target - patch_heading), cos(target - patch_heading))
        patch_heading += clamp(turn, -dt * 3.0, dt * 3.0)
    }
    if (distance > stop_distance) {
        let previous = patch_position.xy
        patch_position = float3(slide_step(patch_position.xy,
            delta / distance * min(dt * 1.6, distance - stop_distance), 0.65), 0.0)
        if (length(patch_position.xy - previous) > 0.0001) { patch_walk_blend = min(1.0, patch_walk_blend + dt * 12.0) }
    }
}
