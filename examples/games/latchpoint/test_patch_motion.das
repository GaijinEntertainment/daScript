options gen2
require dastest/testing_boost public
require lp_render

[test]
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
