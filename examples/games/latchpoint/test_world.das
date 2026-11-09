options gen2
require dastest/testing_boost public
require main
require lp_dialogue
require lp_world
require lp_render
require lp_runtime

//! The yard: props, collision, Patch, generator, telemetry.

[test(description = "The actuator rests on top of its crate, the rotated salvage crates are spaced so their tops never overlap, and the generator's collider draws no wall.")]
def test_prop_placement(t : T?) {
    init_barriers()
    t |> run("actuator rests on its crate instead of floating in front") @(t : T?) {
        let center = actuator_center()
        t |> equal(center.xy, ACTUATOR_CRATE_CENTER.xy)
        t |> success(abs(center.z - ACTUATOR_RADIUS -
            (ACTUATOR_CRATE_CENTER.z + ACTUATOR_CRATE_HALF_SIZE.z)) < 0.00001)
    }
    t |> run("rotated crates do not overlap coplanar top faces") @(t : T?) {
        t |> success(SALVAGE_CRATE_SPACING > 2.0 * length(ACTUATOR_CRATE_HALF_SIZE.xy))
    }
    t |> run("generator collider is not drawn as a wall around the machine") @(t : T?) {
        for (wall in barriers) {
            if (wall.center == GENERATOR_POSITION.xy) { t |> success(!wall.render_wall) }
        }
    }
}

[test(description = "Continue restores a valid saved pose and rejects invalid ones, residents and walls block the avatar without tunneling, the gates switch collision, pickups need proximity and vanish once taken, and Patch follows only when he agreed and is repaired.")]
def test_yard(t : T?) {
    init_barriers()
    t |> run("Continue restores valid saved positions and rejects invalid poses") @(t : T?) {
        let pose = SavedPose(player = float3(1.0, -8.0, 1.65), patch = float3(-3.0, 0.0, 0.0), yaw = 0.5, tilt = -0.1)
        t |> success(apply_saved_pose(pose))
        t |> equal(avatar, pose.player)
        t |> equal(heading, 0.5)
        t |> success(!apply_saved_pose(SavedPose(version = 0)))
        t |> success(!apply_saved_pose(SavedPose(player = float3(999.0))))
        t |> equal(avatar, pose.player)
    }
    t |> run("avatar cannot walk through residents") @(t : T?) {
        t |> success(!can_stand(BRACK_POSITION.xy, 0.33, true))
        t |> success(!can_stand(VESPER_POSITION.xy, 0.33, true))
        t |> success(!can_stand(patch_position.xy, 0.33, true))
    }
    t |> run("walls block large steps without tunneling") @(t : T?) {
        let moved = slide_step(float2(0.0, 0.0), float2(-30.0, 0.0))
        t |> success(moved.x > -13.3)
        t |> success(can_stand(moved))
    }
    t |> run("workshop interlock changes collision") @(t : T?) {
        world.workshop_open = false
        t |> success(!can_stand(float2(4.0, 0.3)))
        let stopped = slide_step(float2(2.8, 0.3), float2(2.0, 0.0))
        t |> success(stopped.x < 3.4)
        world.workshop_open = true
        t |> success(can_stand(float2(4.0, 0.3)))
        let passed = slide_step(float2(2.8, 0.3), float2(2.0, 0.0))
        t |> success(passed.x > 4.7)
    }
    t |> run("archive remains collidable until the receipt") @(t : T?) {
        world.archive_open = false
        t |> success(!can_stand(float2(-4.0, 10.3)))
        world.archive_open = true
        t |> success(can_stand(float2(-4.0, 10.3)))
    }
    t |> run("physical interactions require proximity") @(t : T?) {
        avatar = float3(0.0, -9.0, 1.65)
        t |> equal(physical_at_avatar(), "")
        avatar = SALVAGE_POSITION + float3(0.0, -1.0, 1.65)
        t |> equal(physical_at_avatar(), "salvage actuator")
        avatar = COIL_POSITION + float3(0.0, -1.0, 1.65)
        world.workshop_open = false
        t |> equal(physical_at_avatar(), "")
        world.workshop_open = true
        t |> equal(physical_at_avatar(), "salvage coil")
    }
    t |> run("collected salvage no longer offers a pickup prompt") @(t : T?) {
        avatar = SALVAGE_POSITION + float3(0.0, -1.0, 1.65)
        world.salvaged |> push("actuator")
        t |> equal(physical_at_avatar(), "")
        t |> success(!actuator_available())
        avatar = COIL_POSITION + float3(0.0, -1.0, 1.65)
        world.salvaged |> push("coil")
        t |> equal(physical_at_avatar(), "")
    }
    t |> run("Patch follows only after both agreement and repair") @(t : T?) {
        patch_position = float3(-3.0, 0.0, 0.0)
        avatar = float3(-3.0, -6.0, 1.65)
        world.patch_following = true
        world.patch_repaired = false
        tick_follow(0.1)
        t |> equal(patch_position.y, 0.0)
        world.patch_repaired = true
        tick_follow(0.1)
        t |> success(patch_position.y < 0.0)
    }
}

[test(description = "A following Patch turns toward his travel no faster than his turn rate and stops dead when told, and his model's eyes turn with his body heading.")]
def test_patch_facing(suite : T?) {
    suite |> run("following turns toward travel with a bounded turn rate") @(t : T?) {
        init_barriers()
        patch_position = float3(-3.0, 0.0, 0.0)
        avatar = float3(2.0, 0.0, 1.65)
        patch_heading = 0.0
        world.patch_following = true
        world.patch_repaired = true
        tick_follow(0.1)
        t |> success(patch_heading > 0.0 && patch_heading <= 0.30001)
        t |> success(patch_position.x > -3.0)
        t |> success(patch_walk_blend > 0.0)
        world.patch_following = false
        let stopped = patch_position
        tick_follow(1.0)
        t |> equal(patch_position, stopped)
        t |> equal(patch_walk_blend, 0.0)
    }
    suite |> run("model eyes rotate with the body heading") @(t : T?) {
        frame_parts |> clear()
        patch_position = float3(0.0)
        patch_heading = PI * 0.5
        patch_model(0.0)
        var eyes = 0
        for (item in frame_parts) {
            if (item.color == TEAL) {
                eyes++
                t |> success(item.position.x > 0.5)
            }
        }
        t |> equal(eyes, 2)
    }
}

[test(description = "Installing the coil does not start the generator: the manual starter needs unbroken work, a repaired following Patch walks to the clutch and speeds it up, and nobody starts it without the coil or from far away.")]
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

[test(description = "Reading the live scene telemetry twice reports the world state each time without changing it.")]
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
