options gen2
options indenting = 4

require dastest/testing_boost public
require gltf/gltf_boost
require math

def private model_path(name : string) : string => "{get_das_root()}/tests/dasGLTF/models/{name}"

// CPU mirror of the PBR vertex shader's linear-blend skinning: blend the vertex through each joint's
// palette matrix by its weight. Used to prove the palette + animation math without a GL context.
def private skin_vertex(scene : GltfScene; skinIndex : int; v : GltfVertex) : float3 {
    let p = float4(v.position, 1.0)
    let j = v.joints0
    let w = v.weights0
    var acc = float4(0., 0., 0., 0.)
    acc += (gltf_joint_matrix(scene, skinIndex, int(j.x)) * p) * w.x
    acc += (gltf_joint_matrix(scene, skinIndex, int(j.y)) * p) * w.y
    acc += (gltf_joint_matrix(scene, skinIndex, int(j.z)) * p) * w.z
    acc += (gltf_joint_matrix(scene, skinIndex, int(j.w)) * p) * w.w
    return acc.xyz
}

[test]
def test_skin_bind_pose(t : T?) {
    // At the bind pose (no animation applied), each joint's palette = jointWorld * inverseBind is the
    // identity, so any skinned vertex maps to its original position. SimpleSkin's two joints both bind to
    // identity: joint 0 (node 1) is the world origin, joint 1 (node 2) is translate(0,1,0) cancelled by
    // its inverse-bind translate(0,-1,0).
    t |> run("SimpleSkin bind-pose palette + skinning") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        t |> equal(gltf_joint_matrix(scene, 0, 0), identity4x4())
        t |> equal(gltf_joint_matrix(scene, 0, 1), identity4x4())
        // vertex 0: fully weighted to joint 0; vertex 5: split 0.5/0.5 across joints 0 and 1
        let v0 = scene.meshes[0].primitives[0].vertices[0]
        let v5 = scene.meshes[0].primitives[0].vertices[5]
        t |> equal(skin_vertex(scene, 0, v0), v0.position)
        t |> equal(skin_vertex(scene, 0, v5), v5.position)
        delete scene
    }
}

[test]
def test_anim_keyframe(t : T?) {
    // At an exact keyframe time a LINEAR sampler returns that keyframe's value verbatim (no slerp fuzz).
    // input[2] == 1.0 -> node 2's rotation must equal sampler output[2].
    t |> run("SimpleSkin animation exact keyframe") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        let key_time = scene.animations[0].samplers[0].input[2]
        evaluate_animation(scene, 0, key_time)
        t |> equal(scene.nodes[2].rotation, scene.animations[0].samplers[0].output[2])
        // recompose kept the node's (unanimated) translation column intact
        t |> equal(scene.nodes[2].local[3], float4(0., 1., 0., 1.))
        delete scene
    }
}

[test]
def test_anim_interpolation(t : T?) {
    // Halfway between keyframes 1 (t=0.5) and 2 (t=1.0), the LINEAR rotation is quat_slerp at f=0.5.
    t |> run("SimpleSkin animation slerp midpoint") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        let t1 = scene.animations[0].samplers[0].input[1]
        let t2 = scene.animations[0].samplers[0].input[2]
        let expected = quat_slerp(0.5, scene.animations[0].samplers[0].output[1],
            scene.animations[0].samplers[0].output[2])
        evaluate_animation(scene, 0, (t1 + t2) * 0.5)
        t |> equal(scene.nodes[2].rotation, expected)
        delete scene
    }
}

[test]
def test_anim_skin_invariant(t : T?) {
    // The animation rotates only joint 1 (node 2). A vertex bound entirely to joint 0 (node 1, never a
    // channel target) must keep its position under animation, while a joint-1-bound vertex must move.
    t |> run("SimpleSkin animated skin invariants") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        let base_orig = scene.meshes[0].primitives[0].vertices[0].position   // weights (1,0,0,0): joint 0
        let tip_orig = skin_vertex(scene, 0, scene.meshes[0].primitives[0].vertices[9]) // top row: joint 1
        evaluate_animation(scene, 0, 1.0)                                    // 90 deg about Z on joint 1
        t |> equal(skin_vertex(scene, 0, scene.meshes[0].primitives[0].vertices[0]), base_orig)
        let tip_now = skin_vertex(scene, 0, scene.meshes[0].primitives[0].vertices[9])
        t |> equal(length(tip_now - tip_orig) > 0.1, true)
        delete scene
    }
}
