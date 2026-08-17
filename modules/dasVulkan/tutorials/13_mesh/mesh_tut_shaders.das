options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 13 -- mesh shaders (VK_EXT_mesh_shader): the GPU-driven cluster-culling pipeline.
//
// There is no vertex shader and no vertex/index buffer. Geometry is amplified on the GPU by a
// two-stage pipeline:
//
//   TASK  (one workgroup, MAX_MESHLETS-wide cull) decides which "meshlets" survive and writes them
//         into a @task_payload struct: a count + the surviving meshlet indices. EmitMeshTasksEXT then
//         dispatches exactly `count` mesh workgroups, carrying the payload to them. Here the cull is a
//         deterministic stand-in -- it drops meshlets 3 and 4 to carve a visible gap in the grid -- so
//         the offscreen pixel-oracle can prove the task actually removed geometry.
//
//   MESH  (one workgroup per surviving meshlet) reads its meshlet index from the payload, keyed by
//         gl_WorkGroupID (a workgroup-grid builtin, valid in mesh shaders under VK_EXT_mesh_shader),
//         and emits one colored quad (4 vertices, 2 triangles) at that meshlet's grid cell. The
//         per-vertex color is a smooth varying; gl_PrimitiveID is written per primitive so the
//         fragment could key off it.
//
//   FRAG  outputs the interpolated meshlet color.
//
// The grid is 2 rows x 4 cols (MAX_MESHLETS = 8). meshlet i sits at cell (i%4, i/4).

module mesh_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind helpers
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public

let MAX_MESHLETS = 8

// ===== task -> mesh payload =====

//! Surviving-meshlet list handed from the task workgroup to the mesh workgroups it dispatches.
struct MeshletPayload {
    count     : uint            // how many meshlets survived (== the mesh-workgroup dispatch count)
    survivors : uint[8]         // their grid indices, packed [0 .. count)
}
var @task_payload payload : MeshletPayload

// ===== mesh -> fragment varyings =====

var @out @location = 0 mo_color : array<float3>   // per-vertex smooth color

// ===== TASK: cluster cull =====

[vulkan_task_shader(local_size_x=1, name="mesh_tut_task_spv")]
def mesh_tut_task {
    // Deterministic stand-in cull: keep every meshlet except 3 and 4 (a gap in the middle of row 0/1).
    var n = 0u
    for (i in range(MAX_MESHLETS)) {
        if (i != 3 && i != 4) {
            payload.survivors[n] = uint(i)
            n ++
        }
    }
    payload.count = n
    EmitMeshTasksEXT(n, 1u, 1u)         // dispatch `n` mesh workgroups, carry the payload
}

// ===== MESH: one colored quad per surviving meshlet =====

[vulkan_mesh_shader(local_size_x=1, max_vertices=4, max_primitives=2, name="mesh_tut_mesh_spv")]
def mesh_tut_mesh {
    SetMeshOutputsEXT(4u, 2u)
    let mid = payload.survivors[gl_WorkGroupID.x]   // which meshlet this workgroup draws
    let col = float(mid % 4u)
    let row = float(mid / 4u)
    // cell center in NDC: 4 columns across [-1,1], 2 rows down [-1,1]
    let cx = -1.0 + (col + 0.5) * 0.5
    let cy = -1.0 + (row + 0.5) * 1.0
    let hw = 0.18
    let hh = 0.36
    gl_MeshVerticesEXT[0].gl_Position = float4(cx - hw, cy - hh, 0.0, 1.0)
    gl_MeshVerticesEXT[1].gl_Position = float4(cx + hw, cy - hh, 0.0, 1.0)
    gl_MeshVerticesEXT[2].gl_Position = float4(cx + hw, cy + hh, 0.0, 1.0)
    gl_MeshVerticesEXT[3].gl_Position = float4(cx - hw, cy + hh, 0.0, 1.0)
    let color = float3((col + 1.0) / 4.0, (row + 1.0) / 2.0, 0.6)
    mo_color[0] = color
    mo_color[1] = color
    mo_color[2] = color
    mo_color[3] = color
    gl_PrimitiveTriangleIndicesEXT[0] = uint3(0u, 1u, 2u)
    gl_PrimitiveTriangleIndicesEXT[1] = uint3(0u, 2u, 3u)
    gl_MeshPrimitivesEXT[0].gl_PrimitiveID = int(mid)
    gl_MeshPrimitivesEXT[1].gl_PrimitiveID = int(mid)
}

// ===== FRAGMENT =====

var @in @location = 0 fi_color : float3
var @out @location = 0 fo_color : float4

[vulkan_fragment_shader(name="mesh_tut_frag_spv")]
def mesh_tut_frag {
    fo_color = float4(fi_color, 1.0)
}
