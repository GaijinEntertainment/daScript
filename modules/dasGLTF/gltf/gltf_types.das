options gen2
options indenting = 4

module gltf_types shared

require math

// ===== Vertex / mesh =====

struct GltfVertex {
    position : float3
    normal   : float3
    tangent  : float4        // xyz = tangent, w = handedness sign
    uv0      : float2
    uv1      : float2
    color0   : float4 = float4(1., 1., 1., 1.)
    joints0  : float4        // joint indices, decoded to float at parse time
    weights0 : float4
}

enum GltfPrimitiveMode {
    points
    lines
    triangles
}

struct GltfPrimitive {
    vertices : array<GltfVertex>
    indices  : array<uint>
    material : int = -1       // GltfScene.materials index, -1 = default
    mode     : GltfPrimitiveMode = GltfPrimitiveMode.triangles
}

struct GltfMesh {
    name       : string
    primitives : array<GltfPrimitive>
}

// ===== Materials / textures =====

enum GltfAlphaMode {
    opaque
    mask
    blend
}

struct GltfMaterial {
    //! Material factors, texture references, and rendering modes.
    name              : string //! Material name.
    baseColorFactor   : float4 = float4(1., 1., 1., 1.) //! Linear base color and opacity multiplier.
    emissiveFactor    : float3 //! Linear emissive color multiplier.
    metallicFactor    : float = 1. //! Metallic multiplier.
    roughnessFactor   : float = 1. //! Roughness multiplier.
    baseColorTex      : int = -1     //! Base color texture index; -1 uses white.
    metalRoughTex     : int = -1 //! Metallic and roughness texture index; -1 uses white.
    normalTex         : int = -1 //! Normal texture index; -1 uses a flat normal.
    occlusionTex      : int = -1 //! Occlusion texture index; -1 disables occlusion.
    emissiveTex       : int = -1 //! Emissive texture index; -1 uses white.
    normalScale       : float = 1. //! Normal texture strength.
    occlusionStrength : float = 1. //! Occlusion texture strength.
    alphaMode         : GltfAlphaMode = GltfAlphaMode.opaque //! Opaque, masked, or blended alpha handling.
    alphaCutoff       : float = 0.5 //! Masked alpha rejection threshold.
    doubleSided       : bool = false //! Render both sides of the surface.
    unlit             : bool = false //! KHR_materials_unlit: base color without lighting.
}

struct GltfImage {
    width, height : int
    pixels : array<uint8>            // decoded RGBA8, rows top-to-bottom
}

struct GltfSampler {
    wrapS, wrapT : int               // glTF sampler wrap enum (10497 = REPEAT default)
    minFilter, magFilter : int
}

struct GltfTexture {
    image   : int = -1               // GltfScene.images index
    sampler : int = -1               // GltfScene.samplers index, -1 = default sampler
}

// ===== Node hierarchy / skins / animation =====

struct GltfNode {
    name        : string
    translation : float3
    rotation    : float4 = float4(0., 0., 0., 1.)   // quaternion xyzw
    scale       : float3 = float3(1., 1., 1.)
    children    : array<int>
    mesh        : int = -1           // GltfScene.meshes index
    skin        : int = -1           // GltfScene.skins index
    local       : float4x4           // local transform (from TRS or matrix)
    world       : float4x4           // world transform (filled by update_world_transforms)
}

struct GltfSkin {
    joints      : array<int>         // node indices
    inverseBind : array<float4x4>    // one per joint
    skeleton    : int = -1           // common root node, -1 = unspecified
}

enum GltfInterp {
    step
    linear
    cubicspline
}

enum GltfPath {
    translation
    rotation
    scale
    weights
}

struct GltfAnimSampler {
    input  : array<float>            // keyframe times (seconds)
    output : array<float4>           // values, vec-packed (T/S use xyz, R uses xyzw)
    interp : GltfInterp = GltfInterp.linear
}

struct GltfAnimChannel {
    sampler    : int = -1            // GltfAnimation.samplers index
    targetNode : int = -1
    targetPath : GltfPath = GltfPath.translation
}

struct GltfAnimation {
    name     : string
    samplers : array<GltfAnimSampler>
    channels : array<GltfAnimChannel>
    duration : float                 // max keyframe time across samplers
}

// ===== Scene =====

struct GltfScene {
    meshes     : array<GltfMesh>
    materials  : array<GltfMaterial>
    textures   : array<GltfTexture>
    images     : array<GltfImage>
    samplers   : array<GltfSampler>
    nodes      : array<GltfNode>
    roots      : array<int>          // top-level node indices of the default scene
    skins      : array<GltfSkin>
    animations : array<GltfAnimation>
}
