options gen2
options persistent_heap

require audio/audio_record
require lp_audio
require lp_dialogue
require strudel/strudel_player

var microphone_active = false
var microphone_samples : array<float>
var microphone_scratch : array<float>
var microphone_since = 0.0
var microphone_level = 0.0
let MICROPHONE_RATE = 16000
let MICROPHONE_LIMIT = MICROPHONE_RATE * 20

def cancel_microphone() {
    if (microphone_active) { sound_record_stop() }
    microphone_active = false
    microphone_samples |> clear()
    set_robot_phase(conversation_resident, RobotPhase.idle)
}

def begin_microphone() {
    return if (editing || menu_open || inference_busy() || microphone_active)
    let resident = nearest_resident()
    if (resident < 0) { reply_text = "Move close to a robot before speaking."; return }
    interrupt_voice()
    player_turn_epoch++
    conversation_resident = resident
    if (!sound_record_start(MICROPHONE_RATE, 1, MICROPHONE_RATE * 2, -1)) {
        reply_text = "Microphone unavailable. Press E to type instead."
        return
    }
    microphone_samples |> clear()
    microphone_samples |> reserve(MICROPHONE_LIMIT)
    microphone_scratch |> reserve(4096)
    microphone_scratch |> resize(4096)
    microphone_since = get_uptime()
    microphone_active = true
    conversation_stage = "listening"
    set_robot_phase(resident, RobotPhase.listening)
    reply_text = "MIC LIVE / hold V to speak, release to transcribe. Escape discards the recording."
}

def drain_microphone() {
    return if (!microphone_active)
    let count = min(sound_record_read(microphone_scratch), MICROPHONE_LIMIT - length(microphone_samples))
    var energy = 0.0
    for (i in range(count)) {
        microphone_samples |> push(microphone_scratch[i])
        energy += abs(microphone_scratch[i])
    }
    microphone_level = count > 0 ? saturate(energy / float(count) * 8.0) : 0.0
}

def finish_microphone() {
    return if (!microphone_active)
    drain_microphone()
    sound_record_stop()
    microphone_active = false
    if (length(microphone_samples) < MICROPHONE_RATE / 4) {
        reply_text = "Recording was too short. Hold V while speaking, or press E to type."
        set_robot_phase(conversation_resident, RobotPhase.idle)
        return
    }
    let path = path_join(get_das_root(), "logs/latchpoint-input/microphone.wav")
    mkdir_rec(dir_name(path))
    if (!write_wav(path, microphone_samples, uint(MICROPHONE_RATE), 1)) {
        reply_text = "Could not prepare microphone audio. Press E to type."
        set_robot_phase(conversation_resident, RobotPhase.failed)
        return
    }
    microphone_samples |> clear()
    discard_inference = false
    pending_job = "native"
    conversation_stage = "asr"
    submitted_at = get_uptime()
    reply_text = "Recognizing speech locally. The transcript will be editable before sending."
    launch_inference("/audio/transcriptions", path)
}

def tick_microphone() {
    drain_microphone()
    if (microphone_active && (length(microphone_samples) >= MICROPHONE_LIMIT || get_uptime() - microphone_since > 20.0)) {
        finish_microphone()
    }
    if (microphone_active || conversation_stage == "asr") { strudel_set_volume(music_volume * 0.1, 0.1) }
}
