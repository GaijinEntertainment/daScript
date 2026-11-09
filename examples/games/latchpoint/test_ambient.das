options gen2
options rtti
options persistent_heap
require dastest/testing_boost public
require lp_chatter
require lp_dialogue
require lp_audio
require lp_lighting
require lp_render
require lp_runtime
require lp_geology

//! Ambient life: robot activity, chatter, sound, and scenery.

def reset_ambient_test() {
    delete world
    world = WorldState()
    ambient_activity_allowed = true
    for (phase in robot_phase) { phase = RobotPhase.idle }
    starter_engaged = false
    menu_open = false
    editing = false
    pending_job = ""
    inference_done = 0ul
    chatter_enabled = true
    chatter_resident = 2
    chatter_started = 0.0
    chatter_epoch = player_turn_epoch
    avatar = VESPER_POSITION + float3(0.0, -3.0, 1.65)
    chatter_world = chatter_world_key()
    chatter_stage = "synthesizing"
    chatter_line = "Those records disagree with the weather."
    speech_sid = INVALID_SID
    queued_voice |> clear()
}

[test(description = "The drill sparks only while spinning at full extension against the plate, stays clear of the track, and stops while its robot listens; Patch's work lamp follows its schedule and his listening without ever turning his eyes off.")]
def test_robot_activity(suite : T?) {
    suite |> run("sparks only occur with a spinning drill fully touching the plate") @(t : T?) {
        reset_ambient_test()
        for (i in range(480)) {
            let time = float(i) * 0.1
            let head = drill_head_position(time)
            let contact = drill_contact_position()
            // Track front is y=-0.93; keep the plate and widest cutter clear.
            t |> success(contact.y + 0.36 < BRACK_POSITION.y - 0.93)
            t |> success(head.y + 0.245 < BRACK_POSITION.y - 0.93)
            t |> success(head.z - 0.47 >= contact.z - 0.0001)
            if (drill_contact(time) > 0.0) {
                t |> success(abs(head.z - 0.47 - contact.z) < 0.003)
                t |> success(drill_extension(time) > 0.99)
                t |> success(drill_speed(time) > 0.99)
            }
        }
        t |> equal(drill_contact(9.0), 0.0)
        t |> success(drill_contact(12.0) > 0.99)
        t |> success(drill_rotation(16.0) > drill_rotation(15.0))
        t |> equal(drill_rotation(18.0), drill_rotation(20.0))
        robot_phase[0] = RobotPhase.listening
        t |> equal(drill_contact(12.0), 0.0)
        t |> equal(drill_speed(12.0), 0.0)
    }
    suite |> run("Patch switches the work lamp without extinguishing his eyes") @(t : T?) {
        reset_ambient_test()
        t |> equal(inspection_lamp(38.0), 0.0)
        t |> success(inspection_lamp(49.0) > 0.99)
        t |> success(robot_indicator(1, 38.0) > 0.0)
        robot_phase[1] = RobotPhase.listening
        t |> equal(inspection_lamp(38.0), 1.0)
    }
}

[test(description = "Background chatter cannot run a tool call or change the game, is dropped when the player is busy, takes a turn, changes the world, walks away, disables it or lets it expire, and queues its audio without replacing the player's conversation.")]
def test_chatter_guards(suite : T?) {
    suite |> run("ambient tool calls are rejected before synthesis and cannot change gameplay") @(t : T?) {
        reset_ambient_test()
        chatter_stage = "generating"
        var packet = InferenceResponse(code = 200,
            text = write_json(%json~ {"choices":[{"finish_reason":"stop","message":{"content":"Opening now.","tool_calls":[{"id":"fake"}]}}]} %%))
        finish_chatter_packet(packet, 5.0)
        t |> success(chatter_job.done == 0ul && empty(queued_voice))
        t |> success(!world.archive_open && !world.workshop_open && empty(world.inventory))
    }
    suite |> run("player attention and state changes invalidate background speech") @(t : T?) {
        reset_ambient_test()
        t |> success(chatter_still_relevant(5.0))
        editing = true
        t |> success(!chatter_still_relevant(5.0))
        editing = false
        player_turn_epoch++
        t |> success(!chatter_still_relevant(5.0))
        chatter_epoch = player_turn_epoch
        world.power = true
        t |> success(!chatter_still_relevant(5.0))
    }
    suite |> run("disabled distant and expired chatter is discarded") @(t : T?) {
        reset_ambient_test()
        chatter_enabled = false
        t |> success(!chatter_still_relevant(5.0))
        chatter_enabled = true
        t |> success(!chatter_still_relevant(41.0))
        avatar = float3(100.0)
        t |> success(!chatter_still_relevant(5.0))
    }
    suite |> run("a stale packet cannot queue a voice or mutate the game") @(t : T?) {
        reset_ambient_test()
        editing = true
        var packet = InferenceResponse(code = 200, audio <- [uint8(1), uint8(2)])
        finish_chatter_packet(packet, 5.0)
        t |> success(empty(queued_voice))
        t |> success(!world.power && !world.archive_open && empty(world.inventory))
    }
    suite |> run("valid audio is queued separately without replacing the player's reply") @(t : T?) {
        reset_ambient_test()
        reply_text = "The player's conversation."
        conversation_resident = 0
        var packet = InferenceResponse(code = 200, audio <- [uint8(1), uint8(2)])
        finish_chatter_packet(packet, 5.0)
        t |> success(queued_voice_ambient)
        t |> equal(voice_resident, 2)
        t |> equal(reply_text, "The player's conversation.")
        t |> equal(conversation_resident, 0)
        queued_voice |> clear()
    }
}

[test(description = "Music ducking combines capture and speech into one target, mechanical loops stay bounded with silent seams, the robot indicator is brighter listening than thinking and follows the speech level, robot lamps sit at their animation attachment, and background rocks stay outside the playable yard.")]
def test_feedback_and_sound(suite : T?) {
    suite |> run("one music target combines capture and speech ducking") @(t : T?) {
        t |> equal(music_duck_factor(false, false), 1.0)
        t |> equal(music_duck_factor(false, true), 0.27)
        t |> equal(music_duck_factor(true, false), 0.1)
        t |> equal(music_duck_factor(true, true), 0.1, "microphone and ASR remain intelligible over robot speech")
    }
    suite |> run("generated mechanical loops have bounded samples and quiet seams") @(t : T?) {
        let samples <- mechanical_pcm(0.5, 60.0, 0.15, 0.5, true)
        t |> equal(length(samples), EFFECT_RATE / 2)
        t |> equal(samples[0], 0.0)
        t |> equal(samples[length(samples) - 1], 0.0)
        var energy = 0.0
        for (value in samples) { t |> success(abs(value) < 0.6); energy += abs(value) }
        t |> success(energy > 1.0)
    }
    suite |> run("listening is brighter than thinking and speech follows the envelope") @(t : T?) {
        robot_phase[1] = RobotPhase.listening
        let listening = robot_indicator(1, 0.0)
        robot_phase[1] = RobotPhase.thinking
        t |> success(listening > robot_indicator(1, 0.0))
        robot_phase[1] = RobotPhase.speaking
        robot_voice_level[1] = 0.0
        let quiet = robot_indicator(1, 0.0)
        robot_voice_level[1] = 1.0
        t |> success(robot_indicator(1, 0.0) > quiet)
    }
    suite |> run("robot lamps share the same attachment as animation and voice") @(t : T?) {
        update_yard_lights(3.0)
        for (resident in range(3)) {
            let attachment = robot_attachment(resident, 3.0)
            t |> success(length(lp_light_pos[resident].xyz - attachment.position) < 0.0001)
            t |> success(length(lp_light_direction[resident].xyz - attachment.direction) < 0.0001)
        }
    }
    suite |> run("background rock geometry stays outside the playable yard") @(t : T?) {
        init_barriers()
        update_yard_lights(0.0)
        build_yard()
        for (item in scene_parts) {
            if (item.shape != Shape.rock && item.shape != Shape.rock_alternate) { continue }
            let extent = length(item.size.xy)
            t |> success(abs(item.position.x) - extent > 13.6 || item.position.y - extent > 14.6 || item.position.y + extent < -12.6)
        }
    }
}

[test(description = "Robot voice processing colors each resident's voice slightly, stays within range and leaves the samples untouched when switched off, and outcrop meshes stay in their unit box with unit normals.")]
def test_finishing_materials(suite : T?) {
    suite |> run("voice coloration is subtle, bounded and bypassable") @(t : T?) {
        let original <- mechanical_pcm(0.2, 210.0, 0.3, 0.2)
        for (resident in range(3)) {
            var inscope samples := original
            voice_processing = true
            process_robot_voice(samples, EFFECT_RATE, 1, resident)
            var error = 0.0
            for (sample, dry in samples, original) {
                t |> success(abs(sample) <= 1.0)
                error += abs(sample - dry)
            }
            t |> success(error > 0.0 && error / float(length(samples)) < 0.03)
            samples := original
            voice_processing = false
            process_robot_voice(samples, EFFECT_RATE, 1, resident)
            t |> equal(samples, original)
        }
        voice_processing = true
    }
    suite |> run("outcrop variants have bounded vertices and flat valid faces") @(t : T?) {
        for (seed in fixed_array(1.0, 3.7)) {
            let mesh <- fractured_outcrop(seed)
            t |> success(length(mesh.indices) > 100)
            for (vertex in mesh.vertices) {
                t |> success(abs(vertex.xyz.x) <= 1.0 && abs(vertex.xyz.y) <= 1.0 && abs(vertex.xyz.z) <= 1.0)
                t |> success(abs(length(vertex.normal) - 1.0) < 0.0001)
            }
        }
    }
}
