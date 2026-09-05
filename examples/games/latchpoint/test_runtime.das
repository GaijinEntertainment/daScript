options gen2
options persistent_heap
require dastest/testing_boost public
require lp_runtime
require lp_behavior

def reset_test_world() {
    delete world
    world = WorldState()
    delete histories
    workshop_aperture = -1.0
    archive_aperture = -1.0
}

[test]
def test_native_runtime(suite : T?) {
    suite |> run("only residents with authority can change access") @(t : T?) {
        reset_test_world()
        world.patch_repaired = true
        world.power = true
        t |> success(!runtime_action(0, "open_archive").ok)
        t |> success(!runtime_action(1, "open_workshop").ok)
        t |> success(!world.archive_open && !world.workshop_open)
        t |> success(runtime_action(2, "open_archive").ok)
    }
    suite |> run("power-first route requires the real agreement and both archive conditions") @(t : T?) {
        reset_test_world()
        t |> success(!runtime_action(0, "open_workshop").ok)
        t |> success(runtime_action(0, "accept_task", "restore_power").ok)
        t |> success(runtime_action(0, "open_workshop").ok)
        t |> success(runtime_physical("salvage coil").ok)
        t |> success(runtime_physical("install coil").ok)
        avatar = GENERATOR_POSITION + float3(0.0, 2.7, 1.65)
        for (_i in range(190)) { advance_starter(0.05, true) }
        t |> success(!runtime_action(2, "open_archive").ok)
        t |> success(runtime_action(0, "lend_actuator").ok)
        t |> success(runtime_physical("install actuator").ok)
        t |> success(runtime_action(2, "open_archive").ok)
        t |> success(world.archive_open && world.power && world.patch_repaired)
    }
    suite |> run("repair-first route and one-use inventory stay authoritative") @(t : T?) {
        reset_test_world()
        t |> success(!runtime_physical("install actuator").ok)
        t |> success(runtime_physical("salvage actuator").ok)
        t |> success(!runtime_physical("salvage actuator").ok)
        t |> success(runtime_physical("install actuator").ok)
        t |> success(!runtime_physical("install actuator").ok)
        t |> success(runtime_action(0, "open_workshop").ok)
        t |> success(empty(world.inventory))
    }
    suite |> run("free spare and follow requests cannot duplicate items or bypass repair") @(t : T?) {
        reset_test_world()
        t |> success(!runtime_action(1, "follow").ok)
        t |> success(runtime_action(0, "lend_actuator").ok)
        t |> success(!runtime_action(0, "lend_actuator").ok)
        t |> equal(length(world.inventory), 1)
        t |> success(runtime_physical("install actuator").ok)
        t |> success(runtime_action(1, "follow").ok)
        t |> success(world.patch_following)
        t |> success(runtime_action(1, "stop_follow").ok)
        t |> success(!world.patch_following)
    }
}

[test]
def test_native_save_and_motion(suite : T?) {
    suite |> run("snapshot preserves ownership and separate resident memories") @(t : T?) {
        reset_test_world()
        runtime_action(0, "lend_actuator")
        remember_exchange(0, "hello", "Hello, operator.")
        let snapshot = runtime_snapshot()
        t |> equal(length(world.inventory), 1)
        t |> equal(length(history_messages(0, 12)), 2)
        t |> equal(length(history_messages(1, 12)), 0)
        reset_test_world()
        t |> success(runtime_load(snapshot))
        t |> equal(length(world.inventory), 1)
        t |> equal(length(history_messages(0, 12)), 2)
        t |> success(!runtime_load("\{\"version\":2\}"))
        t |> equal(length(world.inventory), 1)
    }
    suite |> run("partial save is rejected without destroying the current world") @(t : T?) {
        reset_test_world()
        runtime_action(0, "lend_actuator")
        t |> success(!runtime_load("\{\"version\":1,\"world\":\{\},\"conversations\":\{\}\}"))
        t |> equal(length(world.inventory), 1)
    }
    suite |> run("gate motion retains collision until there is head clearance") @(t : T?) {
        reset_test_world()
        init_barriers()
        workshop_aperture = 0.0
        archive_aperture = 0.0
        world.workshop_open = true
        t |> success(!can_stand(float2(4.0, 0.3)))
        tick_behavior(0.5)
        t |> success(workshop_aperture > 0.0 && workshop_aperture < 0.7)
        t |> success(!can_stand(float2(4.0, 0.3)))
        tick_behavior(2.0)
        t |> equal(workshop_aperture, 1.0)
        t |> success(can_stand(float2(4.0, 0.3)))
    }
}
