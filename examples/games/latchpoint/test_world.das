options gen2
require dastest/testing_boost public
require lp_world

[test]
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

[test]
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
