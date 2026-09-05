options gen2
require dastest/testing_boost public
require lp_runtime

[test]
def test_generator_coil(suite : T?) {
    suite |> run("installing a coil does not magically start the jammed generator") @(t : T?) {
        delete world
        world = WorldState()
        world.inventory |> push("coil")
        t |> success(runtime_physical("install coil").ok)
        t |> success(world.coil_installed)
        t |> success(!world.power)
        t |> success(empty(world.inventory))
    }
    suite |> run("manual start is deterministic and interrupted work resets") @(t : T?) {
        delete world
        world = WorldState(coil_installed = true)
        avatar = GENERATOR_POSITION + float3(0.0, 2.7, 1.65)
        starter_progress = 0.0
        for (_i in range(60)) { advance_starter(0.05, true) }
        t |> success(starter_progress > 0.3 && !world.power)
        advance_starter(0.05, false)
        t |> equal(starter_progress, 0.0)
        for (_i in range(181)) { advance_starter(0.05, true) }
        t |> success(world.power)
    }
    suite |> run("repaired following Patch helps only at the clutch") @(t : T?) {
        delete world
        world = WorldState(coil_installed = true, patch_repaired = true, patch_following = true)
        avatar = GENERATOR_POSITION + float3(0.0, 2.7, 1.65)
        patch_position = PATCH_SERVICE_POSITION
        starter_progress = 0.0
        t |> success(patch_can_assist())
        for (_i in range(61)) { advance_starter(0.05, true) }
        t |> success(world.power)
        world.power = false
        world.patch_following = false
        t |> success(!patch_can_assist())
    }
    suite |> run("no coil or remote player cannot start the generator") @(t : T?) {
        delete world
        world = WorldState()
        avatar = GENERATOR_POSITION + float3(0.0, 2.7, 1.65)
        t |> success(!advance_starter(100.0, true))
        world.coil_installed = true
        avatar = float3(0.0, -9.0, 1.65)
        for (_i in range(200)) { advance_starter(0.05, true) }
        t |> success(!world.power)
    }
    suite |> run("following Patch approaches the clutch and faces the machine") @(t : T?) {
        init_barriers()
        delete world
        world = WorldState(coil_installed = true, patch_repaired = true, patch_following = true)
        avatar = GENERATOR_POSITION + float3(0.0, 2.8, 1.65)
        patch_position = float3(5.0, 10.8, 0.0)
        patch_heading = PI
        for (_i in range(160)) { tick_follow(0.05) }
        t |> success(patch_can_assist())
        t |> success(cos(patch_heading) > 0.95)
    }
}
