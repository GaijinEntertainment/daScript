options gen2
options rtti
options persistent_heap
require dastest/testing_boost public
require lp_chatter
require lp_dialogue
require lp_audio
require lp_lighting

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

[test]
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

[test]
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
