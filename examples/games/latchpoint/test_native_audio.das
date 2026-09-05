options gen2
options persistent_heap
require dastest/testing_boost public
require lp_audio
require lp_render

[test]
def test_feedback_and_sound(suite : T?) {
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
