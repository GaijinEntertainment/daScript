options gen2
require gltf/gltf_motion
require gltf/gltf_types
require math
require dastest/testing_boost
[test]
def motion_pose_retains_previous_nodes_without_aliasing(t : T?){
    var inscope scene = GltfScene()
    resize(scene.nodes, 1); scene.nodes[0].world = identity4x4(); scene.nodes[0].world[3].x = 2.0
    var inscope pose = GltfMotionPose(); var root = identity4x4(); root[3].y = 3.0
    gltf_motion_capture(pose, scene, root)
    scene.nodes[0].world[3].x = 7.0
    t |> equal(pose.nodes[0][3].x, 2.0); t |> equal(pose.nodes[0][3].y, 3.0)
    gltf_motion_capture(pose, scene, root); t |> equal(pose.nodes[0][3].x, 7.0)
    clear(scene.nodes); gltf_motion_capture(pose, scene, root); t |> equal(length(pose.nodes), 0)
}
[test]
def motion_palette_tracks_deformation_and_root(t : T?){
    var inscope scene = GltfScene(); resize(scene.nodes, 1); scene.nodes[0].world = identity4x4()
    resize(scene.skins, 1); scene.skins[0].joints |> push(0); scene.skins[0].inverseBind |> push(identity4x4())
    var inscope pose = GltfMotionPose(); var root = identity4x4(); root[3].x = 4.0
    gltf_motion_capture(pose, scene, root)
    scene.nodes[0].world[3].y = 2.0
    t |> equal(pose.joints[0][3].x, 4.0); t |> equal(pose.joints[0][3].y, 0.0)
    gltf_motion_capture(pose, scene, root); t |> equal(pose.joints[0][3].y, 2.0)
}

[test]
def motion_history_rejects_incomplete_skin_ranges(t : T?){
    var inscope scene = GltfScene(); resize(scene.nodes, 1); scene.nodes[0].world = identity4x4(); scene.nodes[0].skin = 0
    resize(scene.skins, 1); scene.skins[0].joints |> push(0); scene.skins[0].inverseBind |> push(identity4x4())
    var inscope pose = GltfMotionPose(); gltf_motion_capture(pose, scene, identity4x4())
    t |> success(gltf_motion_has_node(pose, scene, 0))
    t |> success(!gltf_motion_has_node(pose, scene, -1))
    t |> success(!gltf_motion_has_node(pose, scene, 1))
    pose.offsets[0] = -1; t |> success(!gltf_motion_has_node(pose, scene, 0))
    pose.offsets[0] = 1; t |> success(!gltf_motion_has_node(pose, scene, 0))
    pose.offsets[0] = 0; clear(pose.joints); t |> success(!gltf_motion_has_node(pose, scene, 0))
    clear(pose.offsets); t |> success(!gltf_motion_has_node(pose, scene, 0))
    scene.nodes[0].skin = -1; t |> success(gltf_motion_has_node(pose, scene, 0))
}
