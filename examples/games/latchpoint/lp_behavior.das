options gen2
require lp_world public

enum RobotPhase {
    idle
    listening
    thinking
    speaking
    acting
    failed
}

var @live robot_phase : RobotPhase[3]
var @live robot_phase_since : float[3]
var robot_voice_level : float[3]
var feedback_text = ""
var feedback_until = 0.0
var feedback_serial = 0
var feedback_positive = true
var handover_until = 0.0
var ambient_activity_allowed = true

def activity_time(resident : int; time : float) : float {
    let period = 48.0 + float(resident) * 7.0
    let shifted = time + float(resident) * 17.0
    return shifted - floor(shifted / period) * period
}

def activity_available(resident : int) : bool {
    return (ambient_activity_allowed && robot_phase[resident] == RobotPhase.idle &&
        !(resident == 1 && (world.patch_following || starter_engaged)))
}

def drill_extension(time : float) : float {
    if (!activity_available(0)) { return 0.0 }
    let age = activity_time(0, time)
    return smoothstep(8.0, 11.0, age) * (1.0 - smoothstep(15.0, 18.0, age))
}

def drill_speed(time : float) : float {
    if (!activity_available(0)) { return 0.0 }
    let age = activity_time(0, time)
    return smoothstep(9.0, 11.0, age) * (1.0 - smoothstep(14.5, 17.0, age))
}

def drill_contact(time : float) : float {
    if (!activity_available(0)) { return 0.0 }
    let age = activity_time(0, time)
    return smoothstep(11.1, 11.4, age) * (1.0 - smoothstep(14.0, 14.4, age))
}

def drill_rotation(time : float) : float {
    if (!activity_available(0)) { return 0.0 }
    let age = activity_time(0, time)
    let rise = clamp((age - 9.0) / 2.0, 0.0, 1.0)
    let fall = clamp((age - 14.5) / 2.5, 0.0, 1.0)
    return 18.0 * (2.0 * (rise * rise * rise - 0.5 * rise * rise * rise * rise) +
        clamp(age - 11.0, 0.0, 3.5) + 2.5 * (fall - fall * fall * fall + 0.5 * fall * fall * fall * fall))
}

def drill_contact_position() : float3 {
    return BRACK_POSITION + float3(1.12, -0.90, 0.26)
}

def inspection_lamp(time : float) : float {
    if (robot_phase[1] != RobotPhase.idle || world.patch_following || starter_engaged) { return 1.0 }
    if (!ambient_activity_allowed) { return 0.0 }
    let age = activity_time(1, time)
    return smoothstep(7.0, 7.3, age) * (1.0 - smoothstep(16.0, 16.4, age))
}

def notify_feedback(text : string; positive : bool = true; handover : bool = false) {
    feedback_text = clone_string(text)
    feedback_positive = positive
    feedback_until = get_uptime() + 4.5
    feedback_serial++
    if (handover) { handover_until = get_uptime() + 2.0 }
}

def robot_work_cycle(resident : int; time : float) : float {
    if (!activity_available(resident)) { return 0.0 }
    let age = activity_time(resident, time)
    return smoothstep(7.0, 10.0, age) * (1.0 - smoothstep(16.0, 19.0, age))
}

def set_robot_phase(resident : int; phase : RobotPhase) {
    return if (resident < 0 || resident > 2)
    if (robot_phase[resident] != phase) {
        robot_phase[resident] = phase
        robot_phase_since[resident] = get_uptime()
    }
}

def robot_indicator(resident : int; time : float) : float {
    let phase = robot_phase[resident]
    if (phase == RobotPhase.listening) { return 1.5 }
    if (phase == RobotPhase.thinking) { return 0.30 + (sin(time * 2.4) * 0.5 + 0.5) * 0.28 }
    if (phase == RobotPhase.speaking) { return 0.9 + robot_voice_level[resident] * 0.9 }
    if (phase == RobotPhase.failed) { return 0.25 }
    return 0.72
}

def robot_attention(resident : int; time : float) : float {
    let focus = resident == 1 && starter_engaged && patch_can_assist() ? GENERATOR_POSITION : avatar
    let delta = focus - resident_position(resident)
    let base = resident == 1 ? patch_heading : 0.0
    let angle = atan2(delta.x, -delta.y) - base
    let toward = atan2(sin(angle), cos(angle))
    let limited = clamp(toward, -0.65, 0.65)
    let active = robot_phase[resident] != RobotPhase.idle
    if (active) { return limited }
    if (resident == 1) { return sin((activity_time(1, time) - 9.0) * 0.5) * 0.42 * inspection_lamp(time) }
    return sin(time * 0.21 + float(resident) * 2.0) * 0.16
}

struct RobotAttachment {
    position : float3
    direction : float3
    rotation : float4
}

def attachment_rotation(angle : float) : float4 {
    return float4(0.0, 0.0, sin(angle * 0.5), cos(angle * 0.5))
}

def robot_attachment(resident : int; time : float) : RobotAttachment {
    let base = resident == 1 ? patch_heading : 0.0
    let angle = base + robot_attention(resident, time)
    let rotation = attachment_rotation(angle)
    let nod = robot_phase[resident] == RobotPhase.speaking ? sin(time * 3.1) * 0.025 : sin(time * 0.8) * 0.015
    let head = (resident == 0 ? float3(0.0, -0.10, 1.98 + sin(time * 0.7) * 0.025) :
        (resident == 1 ? float3(0.0, -0.37, 1.46 + sin(time * 1.5) * 0.025) : float3(0.0, -0.21, 1.43)))
    let anchor = resident_position(resident) + quat_mul_vec(resident == 2 ? rotation : attachment_rotation(base), head)
    let offset = resident == 0 ? float3(-0.34, -0.44, -0.12) : (resident == 1 ? float3(0.0, -0.24, -0.04) : float3(0.0, -0.08, 0.0))
    return RobotAttachment(position = anchor + quat_mul_vec(rotation, offset),
        direction = normalize(quat_mul_vec(rotation, float3(0.0, -1.0, -0.30 + nod))), rotation = rotation)
}

def tick_behavior(dt : float) {
    if (starter_engaged && patch_can_assist()) { set_robot_phase(1, RobotPhase.acting) }
    if (workshop_aperture < 0.0) { workshop_aperture = world.workshop_open ? 1.0 : 0.0 }
    if (archive_aperture < 0.0) { archive_aperture = world.archive_open ? 1.0 : 0.0 }
    workshop_aperture += clamp((world.workshop_open ? 1.0 : 0.0) - workshop_aperture, -dt * 0.5, dt * 0.5)
    archive_aperture += clamp((world.archive_open ? 1.0 : 0.0) - archive_aperture, -dt * 0.4, dt * 0.4)
    for (resident in range(3)) {
        if ((robot_phase[resident] == RobotPhase.failed || robot_phase[resident] == RobotPhase.acting) &&
            get_uptime() - robot_phase_since[resident] > 1.5) { set_robot_phase(resident, RobotPhase.idle) }
    }
}
