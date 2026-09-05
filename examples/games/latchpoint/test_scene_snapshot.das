options gen2
require dastest/testing_boost public
require main
require lp_dialogue

[test]
def test_scene_snapshot(suite : T?) {
    suite |> run("live telemetry does not consume authoritative state") @(t : T?) {
        world.workshop_open = true
        delete world.inventory
        world.inventory <- ["actuator"]
        let first = cmd_scene_state(null)
        let second = cmd_scene_state(null)
        t |> success(world.workshop_open)
        t |> equal(length(world.inventory), 1)
        t |> success(first?.world?.workshop_open ?? false)
        t |> success(second?.world?.workshop_open ?? false)
    }
}
