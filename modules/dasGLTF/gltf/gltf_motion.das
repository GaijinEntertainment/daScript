options gen2
module gltf_motion shared public
require math
require gltf_types
require gltf_scene

struct GltfMotionPose {
    //! Previous root-space node transforms and skin palettes, with independent ownership.
    nodes : array<float4x4>
    joints : array<float4x4>
    offsets : array<int>
}
def gltf_motion_capture(var pose : GltfMotionPose; scene : GltfScene; root : float4x4){
    //! Replace the previous pose with root-space transforms and at most 64 joints per skin.
    resize(pose.nodes, length(scene.nodes)); for (node, i in scene.nodes, count()){pose.nodes[i] = root * node.world}
    resize(pose.offsets, length(scene.skins)); var total = 0
    for (skin, i in scene.skins, count()){pose.offsets[i] = total; total += min(length(skin.joints), 64)}
    resize(pose.joints, total)
    for (skin, i in scene.skins, count()){for (j in range(min(length(skin.joints), 64))){pose.joints[pose.offsets[i] + j] = root * gltf_joint_matrix(scene, i, j)}}
}

def gltf_motion_has_node(pose : GltfMotionPose; scene : GltfScene; node_index : int) : bool {
    //! True when the previous pose contains the node and its complete bounded skin palette.
    if (node_index < 0 || node_index >= length(scene.nodes) || node_index >= length(pose.nodes)){return false}
    let skin = scene.nodes[node_index].skin
    if (skin < 0){return true}
    if (skin >= length(scene.skins) || skin >= length(pose.offsets)){return false}
    let offset = pose.offsets[skin]
    let count = min(length(scene.skins[skin].joints), 64)
    return offset >= 0 && offset <= length(pose.joints) && count <= length(pose.joints) - offset
}
