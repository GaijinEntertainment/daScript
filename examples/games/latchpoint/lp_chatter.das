options gen2
options persistent_heap
options rtti

require lp_audio
require lp_dialogue
require lp_lighting

var @live chatter_enabled = true
var @live chatter_job = InferenceJob()
var @live chatter_stage = ""
var @live chatter_line = ""
var @live chatter_topic = ""
var @live chatter_world = ""
var @live chatter_epoch = 0
var @live chatter_resident = 2
var @live chatter_started = 0.0
var @live chatter_next = 0.0
var @live chatter_count = 0
var chatter_previous_drill = 0.0
var chatter_previous_record = 0.0
var chatter_previous_lamp = 0.0
var chatter_previous_flash = 0.0
var chatter_initialized = false
var @live chatter_last_line = ""

def chatter_world_key() : string {
    return sprint_json(world, false)
}

def chatter_player_busy() : bool {
    return (menu_open || editing || !empty(pending_job) || inference_busy() || starter_engaged ||
        robot_phase[0] == RobotPhase.listening || robot_phase[1] == RobotPhase.listening || robot_phase[2] == RobotPhase.listening)
}

def chatter_still_relevant(time : float) : bool {
    return (chatter_enabled && !chatter_player_busy() && chatter_epoch == player_turn_epoch &&
        time - chatter_started < 40.0 && chatter_world == chatter_world_key() &&
        length(avatar.xy - resident_position(chatter_resident).xy) < 13.0)
}

def acceptable_chatter(text : string) : bool {
    return (!empty(strip(text)) && length(text) <= 180 && length(split(text, " ")) <= 28 &&
        find(text, "\{") < 0 && find(text, "\}") < 0)
}

def begin_chatter(topic : string; resident : int; time : float) {
    return if (chatter_job.done != 0ul || !chatter_enabled || chatter_player_busy() || speech_sid != INVALID_SID ||
        time < chatter_next || length(avatar.xy - resident_position(resident).xy) >= 13.0)
    chatter_topic = clone_string(topic)
    chatter_resident = resident
    chatter_world = chatter_world_key()
    chatter_epoch = player_turn_epoch
    chatter_started = time
    chatter_next = time + 65.0
    chatter_stage = "generating"
    let personality = (resident == 0 ? "Brack: dry, practical mining robot." :
        (resident == 1 ? "Patch: curious maintenance robot, quietly amused." : "Vesper: precise archivist, restrained dry wit."))
    let instructions = ("You write ONE optional background remark for a robot game, not a reply to the player. " +
        "At most 20 words, first person, plain spoken text. No stage directions or speaker labels. " +
        "Comment only on the observed event. No questions, tasks, offers, promises, transfers or actions. " +
        "Never claim to change the world. No tools exist here. Robots consider simulation ordinary, but need not mention it. " +
        "Do not discuss injuries, items, access conditions or repairs unless the observation explicitly states them. " +
        "Be understated. " + personality)
    let observation = write_json(JV((observed = topic, power_running = world.power, patch_repaired = world.patch_repaired,
        previous_line_do_not_repeat = chatter_last_line)))
    let messages <- [ResidentMessage(role = "system", content = instructions), ResidentMessage(role = "user", content = observation)]
    let payload = write_json(JV((model = "qwen3-30b", messages = JV(messages), temperature = 0.65, max_tokens = 64,
        enable_thinking = false, stream = false)))
    chatter_job = launch_inference_job("/chat/completions", payload, false, 20)
    runtime_trace("ambient_requested", topic)
}

def finish_chatter_packet(var packet : InferenceResponse; time : float; allow_followup : bool = true) {
    if (!chatter_still_relevant(time) || packet.code != 200 || !empty(packet.error)) {
        runtime_trace("ambient_discarded", chatter_topic)
        chatter_stage = ""
        return
    }
    if (chatter_stage == "generating") {
        if (!allow_followup) {
            runtime_trace("ambient_discarded", chatter_topic)
            chatter_stage = ""
            return
        }
        var error : string
        let body = read_json(packet.text, error)
        let choice = body?.choices?[0]
        let calls = choice?.message?.tool_calls
        let line = choice?.message?.content ?? ""
        if ((choice?.finish_reason ?? "") != "stop" || !acceptable_chatter(line) || strip(line) == strip(chatter_last_line) ||
            (calls is _array && !empty(calls as _array))) {
            runtime_trace("ambient_invalid_or_repeated", line)
            chatter_stage = ""
            return
        }
        chatter_line = clone_string(line)
        let profile = resident_profile(chatter_resident)
        let payload = write_json(JV((input = line, voice = profile?.voice ?? "expr-voice-4-f",
            speed = profile?.speech_speed ?? 1.0, response_format = "wav")))
        chatter_stage = "synthesizing"
        chatter_job = launch_inference_job("/audio/speech", payload, true, 15)
    } else {
        if (speech_sid != INVALID_SID || !empty(queued_voice) || empty(packet.audio)) { chatter_stage = ""; return }
        delete queued_voice
        queued_voice <- packet.audio
        queued_voice_ambient = true
        voice_resident = chatter_resident
        ambient_caption = clone_string(chatter_line)
        chatter_last_line = clone_string(chatter_line)
        chatter_count++
        runtime_trace("ambient_playback_queued", chatter_line)
        chatter_stage = ""
    }
}

def poll_chatter(time : float; allow_followup : bool = true) {
    if (chatter_job.done != 0ul && inference_job_ready(chatter_job)) {
        var packet <- collect_inference_job(chatter_job)
        chatter_job = InferenceJob()
        finish_chatter_packet(packet, time, allow_followup)
    }
}

def tick_chatter(time : float) {
    if (speech_is_ambient && (!chatter_enabled || chatter_player_busy())) { interrupt_voice() }
    poll_chatter(time, true)
    let drill = drill_contact(time)
    let record = robot_work_cycle(2, time)
    let lamp = inspection_lamp(time)
    let flash = storm_flash(time, reduced_flashes)
    if (!chatter_initialized) {
        chatter_next = max(chatter_next, time + 12.0)
        chatter_initialized = true
    } elif (ambient_activity_allowed) {
        if (chatter_previous_drill > 0.1 && drill <= 0.1) {
            begin_chatter("Brack has just finished testing his drill against a scrap plate.", chatter_count % 2 == 0 ? 2 : 0, time)
        } elif (chatter_previous_record > 0.1 && record <= 0.1) {
            begin_chatter("Vesper has just finished comparing old outpost records on her illuminated slate.", 2, time)
        } elif (chatter_previous_lamp > 0.5 && lamp <= 0.5) {
            begin_chatter("Patch has just finished sweeping an inspection lamp across a fitting.", 1, time)
        } elif (flash > 0.15 && chatter_previous_flash <= 0.15) {
            begin_chatter("Distant lightning has illuminated the cloud bank beyond the yard.", 2, time)
        }
    }
    chatter_previous_drill = drill
    chatter_previous_record = record
    chatter_previous_lamp = lamp
    chatter_previous_flash = flash
}
