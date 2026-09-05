options gen2
options persistent_heap
options rtti

require dastest/testing_boost public
require lp_dialogue
require daslib/fio

[test]
def test_new_game_replaces_a_corrupt_save(t : T?) {
    var error : string
    let root = create_temp_directory("latchpoint-save", error)
    t |> success(!empty(root), "temporary save directory: {error}")
    return if (empty(root))
    let old_path = save_path
    let path = path_join(root, "scene.json")
    let real_pose_path = avatar_save_path()
    let real_pose_before = fread(real_pose_path)
    let real_pose_stat = stat(real_pose_path)
    save_path = path
    save_valid = false
    fwrite(path, "corrupt save")
    world.power = true
    submit_interaction("new_game", "")
    t |> success(save_valid, "New Game re-arms saving after a corrupt save")
    t |> success(!world.power, "New Game installs the fresh world")
    t |> success(runtime_load(fread(path)), "the replacement save is valid")
    t |> equal(fread(path + ".corrupt"), "corrupt save", "the rejected save remains recoverable")
    let pose_path = path_join(root, "scene-pose.json")
    t |> success(fexist(pose_path), "New Game writes its pose beside the selected world save")
    t |> equal(fread(real_pose_path), real_pose_before, "New Game does not change the normal pose save")
    let real_pose_after = stat(real_pose_path)
    if (real_pose_stat.is_valid && real_pose_after.is_valid) {
        t |> equal(real_pose_after.mtime, real_pose_stat.mtime, "New Game does not rewrite the normal pose save")
    }
    save_path = old_path
    rmdir_rec(root)
}

[test]
def test_save_recovery_and_missing_save(t : T?) {
    var error : string
    let root = create_temp_directory("latchpoint-recovery", error)
    t |> success(!empty(root), "temporary recovery directory: {error}")
    return if (empty(root))
    let old_path = save_path
    let path = path_join(root, "scene.json")
    delete world
    world = WorldState(power = true, coil_installed = true)
    let previous = runtime_snapshot()
    fwrite(path, "broken primary")
    fwrite(path + ".previous", previous)
    delete world
    world = WorldState()
    load_runtime_save(path)
    t |> success(save_valid && world.power, "a valid previous save recovers the world")
    t |> equal(fread(path + ".corrupt"), "broken primary", "recovery preserves the rejected primary")
    t |> success(runtime_load(fread(path)), "recovery republishes a valid primary")
    remove(path)
    delete world
    world = WorldState()
    load_runtime_save(path)
    t |> success(save_valid && world.power, "a missing primary recovers from the previous save")
    t |> success(runtime_load(fread(path)), "missing-primary recovery republishes the primary")
    save_valid = false
    load_runtime_save(path_join(root, "missing.json"))
    t |> success(save_valid, "a missing save starts a writable runtime")
    save_path = old_path
    rmdir_rec(root)
}

[test]
def test_new_game_rolls_back_when_publish_fails(t : T?) {
    var error : string
    let root = create_temp_directory("latchpoint-rollback", error)
    t |> success(!empty(root), "temporary rollback directory: {error}")
    return if (empty(root))
    let old_path = save_path
    save_path = path_join(path_join(root, "missing"), "scene.json")
    save_valid = true
    delete world
    world = WorldState(power = true, coil_installed = true)
    native_new_game()
    t |> success(world.power && world.coil_installed, "a failed New Game save restores the old world")
    t |> equal(reply_text, "Could not save the new game.")
    save_path = old_path
    rmdir_rec(root)
}
