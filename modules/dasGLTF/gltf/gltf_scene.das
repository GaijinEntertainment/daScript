options gen2
options indenting = 4

module gltf_scene shared

require gltf/gltf_types
require math

def private update_node_world(var scene : GltfScene; ni : int; parent : float4x4; var visited : array<bool>) {
    // out of range, or already visited (cycle guard: a malformed glTF hierarchy could loop forever)
    if (ni < 0 || ni >= length(scene.nodes) || visited[ni]) return
    visited[ni] = true
    scene.nodes[ni].world = parent * scene.nodes[ni].local
    let w = scene.nodes[ni].world
    for (c in scene.nodes[ni].children) {
        update_node_world(scene, c, w, visited)
    }
}

def update_world_transforms(var scene : GltfScene) {
    //! Recompute every node's `world` transform by walking the hierarchy from the
    //! scene roots (world = parent.world * local).
    let id = identity4x4()
    var visited : array<bool>
    visited |> resize(length(scene.nodes))
    for (r in scene.roots) {
        update_node_world(scene, r, id, visited)
    }
    delete visited
}

def gltf_scene_bounds(scene : GltfScene) : tuple<float3; float3> {
    //! World-space axis-aligned bounds (min, max) over every mesh vertex, using each node's `world`
    //! transform. Returns (0, 0) for a scene with no drawable vertices. Handy for auto-framing a camera.
    var lo = float3(1.0e30, 1.0e30, 1.0e30)
    var hi = float3(-1.0e30, -1.0e30, -1.0e30)
    var any = false
    for (node in scene.nodes) {
        if (node.mesh < 0 || node.mesh >= length(scene.meshes)) {
            continue
        }
        for (prim in scene.meshes[node.mesh].primitives) {
            for (v in prim.vertices) {
                let wp = (node.world * float4(v.position, 1.0)).xyz
                lo = min(lo, wp)
                hi = max(hi, wp)
                any = true
            }
        }
    }
    if (!any) {
        return (float3(0., 0., 0.), float3(0., 0., 0.))
    }
    return (lo, hi)
}

// The keyframe value for output index `k`. CUBICSPLINE packs [inTangent, value, outTangent] per
// keyframe, so its value sits at 3k+1; STEP/LINEAR store one value per keyframe at k.
def private value_at(samp : GltfAnimSampler; k : int) : float4 {
    let idx = samp.interp == GltfInterp.cubicspline ? 3 * k + 1 : k
    return idx >= 0 && idx < length(samp.output) ? samp.output[idx] : float4(0., 0., 0., 0.)
}

def private spline_in_tangent(samp : GltfAnimSampler; k : int) : float4 {
    let idx = 3 * k
    return idx >= 0 && idx < length(samp.output) ? samp.output[idx] : float4(0., 0., 0., 0.)
}

def private spline_out_tangent(samp : GltfAnimSampler; k : int) : float4 {
    let idx = 3 * k + 2
    return idx >= 0 && idx < length(samp.output) ? samp.output[idx] : float4(0., 0., 0., 0.)
}

// Sample one animation sampler at time `t`. Rotations interpolate with quat_slerp (LINEAR) and are
// renormalized (CUBICSPLINE); translation/scale interpolate componentwise (value in .xyz). Times outside
// the keyframe range clamp to the first/last value, and exact keyframes return that value bit-for-bit.
def private eval_sampler(samp : GltfAnimSampler; t : float; isRotation : bool) : float4 {
    let n = length(samp.input)
    if (n == 0) return float4(0., 0., 0., isRotation ? 1. : 0.)
    if (n == 1 || t <= samp.input[0]) return value_at(samp, 0)
    if (t >= samp.input[n - 1]) return value_at(samp, n - 1)
    var k = 0
    for (i in range(n - 1)) {
        if (t < samp.input[i + 1]) {
            k = i
            break
        }
    }
    let t0 = samp.input[k]
    let t1 = samp.input[k + 1]
    let dt = t1 - t0
    let f = dt > 0. ? (t - t0) / dt : 0.
    if (samp.interp == GltfInterp.step || f <= 0.) return value_at(samp, k)
    if (f >= 1.) return value_at(samp, k + 1)
    if (samp.interp == GltfInterp.cubicspline) {
        // Hermite basis; tangents are scaled by the segment duration per the glTF spec.
        let p0 = value_at(samp, k)
        let p1 = value_at(samp, k + 1)
        let m0 = spline_out_tangent(samp, k) * dt
        let m1 = spline_in_tangent(samp, k + 1) * dt
        let f2 = f * f
        let f3 = f2 * f
        let r = (p0 * (2. * f3 - 3. * f2 + 1.)
               + m0 * (f3 - 2. * f2 + f)
               + p1 * (-2. * f3 + 3. * f2)
               + m1 * (f3 - f2))
        return isRotation ? normalize(r) : r
    }
    let v0 = value_at(samp, k)
    let v1 = value_at(samp, k + 1)
    return isRotation ? quat_slerp(f, v0, v1) : lerp(v0, v1, f)
}

def evaluate_animation(var scene : GltfScene; animation : int; t : float) {
    //!@nolint
    //! Evaluate animation `animation` at time `t` (seconds): sample each channel and write the result into
    //! its target node's TRS, then recompose those nodes' local matrices and refresh every world transform.
    //! Nodes given by an explicit `matrix` are never animation targets (glTF forbids it), so only touched
    //! (hence TRS-defined) nodes are recomposed — matrix-defined nodes keep their parsed local.
    if (animation < 0 || animation >= length(scene.animations)) return
    evaluate_animation(scene, scene.animations[animation], t)
}

def evaluate_animation(var scene : GltfScene; clip : GltfAnimation; t : float) {
    //! Evaluate `clip` at `t` seconds into `scene`, allowing multiple scene instances to share
    //! animation data without cloning it. Channel targets update node TRS values, animated local
    //! matrices are recomposed, and world transforms are refreshed; invalid channels are skipped.
    let nch = length(clip.channels)
    for (ci in range(nch)) {
        let ch = clip.channels[ci]
        if (ch.targetNode < 0 || ch.targetNode >= length(scene.nodes) ||
            ch.sampler < 0 || ch.sampler >= length(clip.samplers)) continue
        let val = eval_sampler(clip.samplers[ch.sampler], t,
            ch.targetPath == GltfPath.rotation)
        if (ch.targetPath == GltfPath.translation) {
            scene.nodes[ch.targetNode].translation = val.xyz
        } elif (ch.targetPath == GltfPath.rotation) {
            scene.nodes[ch.targetNode].rotation = val
        } elif (ch.targetPath == GltfPath.scale) {
            scene.nodes[ch.targetNode].scale = val.xyz
        }
        // GltfPath.weights (morph targets) is deferred.
    }
    // Second pass: recompose the local of each animated node from its finalized TRS (recompose is
    // idempotent, so a node driven by multiple channels is fine).
    for (ci in range(nch)) {
        let tn = clip.channels[ci].targetNode
        if (tn >= 0 && tn < length(scene.nodes)) {
            scene.nodes[tn].local = compose(scene.nodes[tn].translation,
                scene.nodes[tn].rotation, scene.nodes[tn].scale)
        }
    }
    update_world_transforms(scene)
}

def gltf_joint_matrix(scene : GltfScene; skinIndex, j : int) : float4x4 {
    //!@nolint
    //! Skinning matrix for joint slot `j` of skin `skinIndex`: jointNode.world * inverseBind[j]. This is the
    //! palette the GPU multiplies each skinned vertex by; the mesh node's own transform is intentionally
    //! omitted (glTF renders skinned geometry directly in skin space). Returns identity for an invalid index.
    if (skinIndex < 0 || skinIndex >= length(scene.skins) ||
        j < 0 || j >= length(scene.skins[skinIndex].joints)) return identity4x4()
    let jn = scene.skins[skinIndex].joints[j]
    let jointWorld = jn >= 0 && jn < length(scene.nodes) ? scene.nodes[jn].world : identity4x4()
    if (j < length(scene.skins[skinIndex].inverseBind)) {
        return jointWorld * scene.skins[skinIndex].inverseBind[j]
    }
    return jointWorld
}
