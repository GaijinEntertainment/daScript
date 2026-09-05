options gen2
options persistent_heap

require audio/audio_boost public
require daslib/jobque_boost
require strudel/strudel
require strudel/strudel_player
require lp_dialogue
require lp_lighting

var audio_channels : AudioSystemChannels
var audio_ready = false
var speech_sid : SID = INVALID_SID
var speech_status : SeqBox?
var speech_pcm : array<float>
var speech_rate = 24000
var speech_channels = 1
var speaking_resident = -1
var speech_is_ambient = false
var ambient_caption = ""
var generator_sid : SID = INVALID_SID
var workshop_sid : SID = INVALID_SID
var archive_sid : SID = INVALID_SID
var wind_sid : SID = INVALID_SID
var servo_pcm : array<float>
var click_pcm : array<float>
var thunder_pcm : array<float>
var cue_pcm : array<float>
var previous_phases : RobotPhase[3]
var previous_gate = float2(0.0)
var previous_avatar = float3(0.0)
var step_distance = 0.0
var previous_flash = 0.0
var thunder_at = -1.0
var creak_at = 0.0
var last_patch = float3(0.0)
var servo_at = 0.0
var music_powered = false
var harmony_track = -1
var drill_sid : SID = INVALID_SID
var grind_sid : SID = INVALID_SID
var last_inspection_lamp = 0.0
var heard_feedback = 0
var @live music_volume = 0.35
var @live effects_volume = 0.65
var @live voice_volume = 1.0
var @live voice_processing = true
let EFFECT_RATE = 24000

def mechanical_pcm(duration, frequency, volume, noise_amount : float; looping : bool = false) : array<float> {
    let count = int(duration * float(EFFECT_RATE))
    var samples : array<float>
    samples |> reserve(count)
    samples |> resize(count)
    var seed = 51231u
    var filtered = 0.0
    for (i in range(count)) {
        let time = float(i) / float(EFFECT_RATE)
        seed = seed * 1664525u + 1013904223u
        let noise = float(seed & 65535u) / 32768.0 - 1.0
        filtered += (noise - filtered) * 0.045
        let tone = sin(time * frequency * PI * 2.0) + sin(time * frequency * PI * 4.0) * 0.20
        let envelope = looping ? 1.0 : sin(PI * min(time / 0.02, 1.0) * 0.5) * exp(-time * 5.0 / duration)
        let seam = min(min(float(i), float(count - 1 - i)) / 240.0, 1.0)
        samples[i] = (tone * (1.0 - noise_amount) + filtered * noise_amount * 3.0) * volume * envelope * seam
    }
    return <- samples
}

def positional_cue(samples : array<float>; position : float3; volume : float = 1.0) {
    return if (!audio_ready)
    var pcm <- clone(samples)
    let sid = play_3d_sound_from_pcm(position, inverse_distance_attenuation(3.0), EFFECT_RATE, 1, pcm)
    set_volume(sid, volume * effects_volume)
}

def yard_music_event(event : string) {
    if (event == "power_on" && harmony_track < 0) {
        let harmony <- note("<d3 f3 a3 c4>", "sine") |> slow(4.0lf) |> gain(0.08) |> attack(1.0) |> release(2.0) |> room(0.35)
        harmony_track = strudel_add_track(harmony)
    } elif (event == "power_off" && harmony_track >= 0) {
        strudel_remove_track(harmony_track)
        harmony_track = -1
    }
}

def yard_music() {
    strudel_set_bpm(52.0lf)
    let drone <- note("<d1 a1 d1 c2>", "sine") |> slow(8.0lf) |> gain(0.16) |> attack(1.8) |> release(3.0) |> lpf(160.0)
    let metal <- note("d4 ~ ~ a4 ~ eb4 ~ ~", "sine") |> slow(4.0lf) |> fm(1.4) |> fmh(2.71) |> gain(0.07) |> decay(0.6) |> release(1.8) |> room(0.45)
    let pulse <- note("d2 ~ ~ d2 ~ ~ a1 ~", "triangle") |> slow(2.0lf) |> gain(0.07) |> lpf(280.0) |> attack(0.04) |> release(0.4)
    strudel_add_track(drone)
    strudel_add_track(metal)
    strudel_add_track(pulse)
    strudel_play(@@yard_music_event)
}

def init_yard_audio() {
    audio_channels = audio_system_create()
    audio_ready = true
    set_hrtf_budget(8)
    delete servo_pcm
    delete click_pcm
    delete cue_pcm
    delete thunder_pcm
    servo_pcm <- mechanical_pcm(0.22, 155.0, 0.13, 0.45)
    click_pcm <- mechanical_pcm(0.10, 320.0, 0.14, 0.70)
    cue_pcm <- mechanical_pcm(0.13, 670.0, 0.08, 0.03)
    thunder_pcm <- mechanical_pcm(3.8, 38.0, 0.45, 0.92)
    var engine <- mechanical_pcm(1.0, 60.0, 0.13, 0.16, true)
    generator_sid = play_3d_sound_loop_from_pcm(GENERATOR_POSITION + float3(0.0, 0.0, 1.4), inverse_distance_attenuation(3.0), EFFECT_RATE, 1, engine)
    var motor <- mechanical_pcm(1.0, 97.0, 0.10, 0.3, true)
    var other_motor <- clone(motor)
    workshop_sid = play_3d_sound_loop_from_pcm(float3(4.0, 0.3, 1.5), inverse_distance_attenuation(6.0), EFFECT_RATE, 1, motor)
    archive_sid = play_3d_sound_loop_from_pcm(float3(-4.0, 10.3, 1.5), inverse_distance_attenuation(6.0), EFFECT_RATE, 1, other_motor)
    set_volume(workshop_sid, 0.0)
    set_volume(archive_sid, 0.0)
    set_volume(generator_sid, world.power ? effects_volume : 0.0)
    var wind <- mechanical_pcm(4.0, 24.0, 0.07, 0.97, true)
    wind_sid = play_sound_loop_from_pcm(EFFECT_RATE, 1, wind)
    set_volume(wind_sid, effects_volume)
    var drill <- mechanical_pcm(1.0, 230.0, 0.12, 0.3, true)
    var grind <- mechanical_pcm(1.0, 135.0, 0.20, 0.88, true)
    drill_sid = play_3d_sound_loop_from_pcm(drill_contact_position(), inverse_distance_attenuation(4.0), EFFECT_RATE, 1, drill)
    grind_sid = play_3d_sound_loop_from_pcm(drill_contact_position(), inverse_distance_attenuation(4.0), EFFECT_RATE, 1, grind)
    set_volume(drill_sid, 0.0)
    set_volume(grind_sid, 0.0)
    speech_status = seq_box_create()
    strudel_init(@@yard_music)
    strudel_set_volume(music_volume, 1.5)
    previous_avatar = avatar
    last_patch = patch_position
    previous_gate = float2(workshop_aperture, archive_aperture)
}

def interrupt_voice() {
    if (speech_sid != INVALID_SID) { stop(speech_sid, 0.12); unset_status_update(speech_sid) }
    set_robot_phase(speaking_resident, RobotPhase.idle)
    speech_sid = INVALID_SID
    speaking_resident = -1
    speech_is_ambient = false
    delete speech_pcm
    queued_voice |> clear()
}

def process_robot_voice(var samples : array<float>; rate, channels, resident : int) {
    return if (!voice_processing || rate <= 0 || channels < 1 || channels > 2)
    let wet = resident == 0 ? 0.20 : (resident == 1 ? 0.15 : 0.07)
    let delay_frames = max(1, int(float(rate) * (resident == 2 ? 0.0007 : 0.0013)))
    var inscope original := samples
    var bass : float[2]
    let alpha = 1.0 - exp(-2.0 * PI * 180.0 / float(rate))
    for (i in range(length(samples))) {
        let channel = i % channels
        let dry = original[i]
        bass[channel] += (dry - bass[channel]) * alpha
        let delayed = i >= delay_frames * channels ? original[i - delay_frames * channels] : 0.0
        let colored = dry + (resident == 0 ? bass[channel] * 0.20 : -bass[channel] * 0.25) + delayed * 0.15
        let driven = colored * 1.08 / (1.0 + abs(colored) * 0.20)
        samples[i] = clamp(lerp(dry, driven, wet), -1.0, 1.0)
    }
}

def begin_robot_voice() {
    return if (empty(queued_voice))
    var channels, rate : int
    var samples <- decode_audio(queued_voice, channels, rate)
    queued_voice |> clear()
    if (empty(samples) || channels < 1 || channels > 2 || rate <= 0) {
        set_robot_phase(voice_resident, RobotPhase.failed)
        conversation_stage = "failed"
        return
    }
    let ambient = queued_voice_ambient
    interrupt_voice()
    speech_is_ambient = ambient
    speech_rate = rate
    speech_channels = channels
    speaking_resident = voice_resident
    process_robot_voice(samples, rate, channels, speaking_resident)
    speech_pcm := samples
    speech_sid = play_3d_sound_from_pcm(resident_position(speaking_resident) + float3(0.0, 0.0, 1.5),
        inverse_distance_attenuation(5.0), rate, channels, samples)
    set_volume(speech_sid, voice_volume)
    set_status_update(speech_sid, speech_status)
    set_robot_phase(speaking_resident, RobotPhase.speaking)
    if (ambient) {
        var observation <- [ResidentMessage(role = "assistant", content = clone_string(ambient_caption))]
        histories[resident_id(speaking_resident)] |> emplace(observation)
        if (!runtime_save()) { runtime_trace("ambient_memory_save_failed", ambient_caption) }
    }
    if (!ambient) { conversation_stage = "speaking" }
}

def tick_robot_voice() {
    begin_robot_voice()
    return if (speech_sid == INVALID_SID)
    set_position(speech_sid, robot_attachment(speaking_resident, get_uptime()).position)
    set_volume(speech_sid, voice_volume, 0.1)
    speech_status |> read() $(status : AudioChannelStatus) {
        let start = int(status.playback_position) * speech_channels
        var energy = 0.0
        let count = clamp(length(speech_pcm) - start, 0, 240)
        for (i in range(count)) { energy += abs(speech_pcm[start + i]) }
        robot_voice_level[speaking_resident] = count > 0 ? saturate(energy / float(count) * 5.0) : 0.0
        if (status.state == AudioChannelState.stopped) {
            robot_voice_level[speaking_resident] = 0.0
            let ambient = speech_is_ambient
            interrupt_voice()
            if (!ambient) { conversation_stage = "idle" }
        }
    }
}

def tick_yard_audio(time : float) {
    return if (!audio_ready)
    set_head_position(avatar, float3(sin(heading), cos(heading), 0.0))
    tick_robot_voice()
    if (heard_feedback != feedback_serial) {
        positional_cue(feedback_positive ? cue_pcm : click_pcm, avatar, 0.65)
        heard_feedback = feedback_serial
    }
    if (music_powered != world.power) {
        strudel_command(world.power ? "power_on" : "power_off")
        music_powered = world.power
    }
    strudel_set_volume(music_volume * (speech_sid == INVALID_SID ? 1.0 : 0.27), 0.25)
    set_volume(generator_sid, effects_volume * (world.power ? 1.0 : (starter_engaged ? 0.45 : 0.0)), 0.3)
    set_volume(wind_sid, effects_volume, 0.3)
    let moving = float2(abs(workshop_aperture - previous_gate.x), abs(archive_aperture - previous_gate.y))
    set_volume(workshop_sid, moving.x > 0.0001 ? effects_volume * 2.3 : 0.0, 0.08)
    set_volume(archive_sid, moving.y > 0.0001 ? effects_volume * 2.3 : 0.0, 0.08)
    if ((moving.x > 0.0001 && previous_gate.x == 0.0) || (moving.y > 0.0001 && previous_gate.y == 0.0)) {
        positional_cue(click_pcm, moving.x > 0.0001 ? float3(4.0, 0.3, 1.5) : float3(-4.0, 10.3, 1.5), 1.6)
    }
    if (workshop_aperture == 1.0 && previous_gate.x < 1.0) { positional_cue(click_pcm, float3(4.0, 0.3, 2.0)) }
    if (archive_aperture == 1.0 && previous_gate.y < 1.0) { positional_cue(click_pcm, float3(-4.0, 10.3, 2.0)) }
    previous_gate = float2(workshop_aperture, archive_aperture)
    for (resident in range(3)) {
        if (robot_phase[resident] != previous_phases[resident]) {
            if (robot_phase[resident] == RobotPhase.thinking) { positional_cue(cue_pcm, resident_position(resident) + float3(0.0, 0.0, 1.4)) }
            previous_phases[resident] = robot_phase[resident]
        }
    }
    if (length(patch_position - last_patch) > 0.05 && time > servo_at) {
        positional_cue(servo_pcm, patch_position, 0.35)
        servo_at = time + 0.55
        last_patch = patch_position
    }
    step_distance += min(length(avatar - previous_avatar), 0.25)
    if (step_distance > 1.3) { positional_cue(click_pcm, avatar - float3(0.0, 0.0, 1.4), 0.3); step_distance = 0.0 }
    previous_avatar = avatar
    tick_weather_audio(time)
    tick_activity_audio(time)
}

def tick_activity_audio(time : float) {
    let speed = drill_speed(time)
    set_volume(drill_sid, speed * effects_volume * 1.25, 0.12)
    set_pitch(drill_sid, 0.45 + speed * 0.65)
    set_volume(grind_sid, drill_contact(time) * effects_volume * 1.45, 0.05)
    let lamp = inspection_lamp(time)
    if ((lamp > 0.5) != (last_inspection_lamp > 0.5) && activity_available(1)) {
        positional_cue(click_pcm, robot_attachment(1, time).position, 0.22)
    }
    last_inspection_lamp = lamp
}

def tick_weather_audio(time : float) {
    let flash = storm_flash(time, reduced_flashes)
    if (flash > 0.15 && previous_flash <= 0.15 && thunder_at < 0.0) { thunder_at = time + 5.0 }
    if (thunder_at > 0.0 && time >= thunder_at) {
        positional_cue(thunder_pcm, avatar + float3(8.0, 22.0, 8.0), 3.0)
        thunder_at = -1.0
    }
    previous_flash = flash
    if (time > creak_at) { positional_cue(servo_pcm, hanging_lamp(time), 0.18); creak_at = time + 7.3 }
}

def shutdown_yard_audio() {
    return if (!audio_ready)
    interrupt_voice()
    strudel_shutdown()
    speech_status |> seq_box_release()
    audio_system_finalize(audio_channels.command, audio_channels.next_sid)
    audio_ready = false
}
