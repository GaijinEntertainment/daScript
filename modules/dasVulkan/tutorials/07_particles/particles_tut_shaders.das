options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 07 -- a compute-driven particle swarm. The same storage buffer flows through both
// pipelines: a compute shader rewrites every particle's position + velocity each frame (gravity
// pull toward the origin, integrated explicit-Euler), then a single per-frame pipeline barrier
// hands the buffer over to the graphics pipeline, which reads it as a per-instance vertex
// attribute stream and billboards a small quad per particle. The headline rails:
//
//   1. ``@ssbo`` -- a SPIR-V StorageBuffer block holding an ``array<Particle>``. The compute shader
//      reads + writes through ``particles[i].field`` chained access chains; the host marks the
//      buffer USAGE_STORAGE_BUFFER | USAGE_VERTEX_BUFFER so the same VkBuffer can also bind as a
//      vertex input.
//
//   2. **A per-frame execution + memory barrier** (``vkCmdPipelineBarrier`` with
//      COMPUTE_SHADER -> VERTEX_INPUT, SHADER_WRITE -> VERTEX_ATTRIBUTE_READ) -- the canonical
//      compute->graphics handoff. Without it the graphics pipeline can read stale particle values
//      from before the compute dispatch.
//
//   3. **Two pipelines reading the same buffer two ways**: compute sees ``Particle`` structs in a
//      std430-layout array; graphics binds the SAME bytes as a per-instance vertex stream with
//      attribute offsets matching the struct layout (pos at +0, life at +12, vel at +16, stride 32).

module particles_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind_uniform helper
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== shared particle struct =====

//! std430 layout: pos@0, life@12, vel@16, pad@28; stride 32 bytes. The pad keeps the struct's vec3
//! alignment (16 bytes) honest -- without it the stride would round up to 32 anyway, but writing it
//! explicitly makes the host-side initial-state generator's offsets impossible to miss.
struct Particle {
    pos  : float3
    life : float
    vel  : float3
    pad  : float
}

// ===== compute shader =====

//! Time + dt feed the explicit-Euler integration. dt = wall-clock-per-frame so the swarm advances
//! at a constant rate regardless of GPU speed; time isn't used in the integrator but is kept for a
//! future re-spawn / noise driver.
struct ComputePush {
    time : float
    dt   : float
}
var @push_constant cpc : ComputePush

var @ssbo @binding = 0 particles : array<Particle>

[vulkan_compute_shader(local_size_x=64, name="particles_compute_spv")]
def particles_compute {
    let i = gl_GlobalInvocationID.x
    let p_pos = particles[i].pos
    let p_vel = particles[i].vel
    // central attractor: gravity-like 1/r^2 pull toward the origin, softened by 0.05 so particles
    // passing close to the origin don't blow up. The orbit isn't strictly Keplerian (explicit Euler
    // drifts) but the trajectories stay bounded and lively for any practical recording length.
    let r2 = dot(p_pos, p_pos)
    let r_mag = sqrt(r2 + 0.05)
    let inv_r3 = 1.0 / (r_mag * r_mag * r_mag)
    let accel = -p_pos * inv_r3 * 1.8
    let new_vel = p_vel + accel * cpc.dt
    let new_pos = p_pos + new_vel * cpc.dt
    particles[i].vel = new_vel
    particles[i].pos = new_pos
    // life is just a colour driver -- it stays untouched here, the host seeded it once.
}

// ===== vertex shader -- point sprite per particle =====

//! View + projection. POINT_LIST topology makes each particle one GL point, rasterised as a screen-space
//! splat ``PARTICLE_PX`` pixels wide. ``gl_PointSize`` is a vertex-stage Output builtin (BuiltIn PointSize)
//! read only under POINT_LIST; values > 1 need the ``largePoints`` feature, which the host enables.
struct Camera {
    view : float4x4
    proj : float4x4
}
var @uniform @set = 0 @binding = 0 cam : Camera

//! Screen-space point size in pixels. 10 px reads cleanly at 768x768 and is well under every
//! desktop GPU's pointSizeRange max (lavapipe advertises [1, 64]).
let public PARTICLE_PX = 10.0

// per-instance attributes (binding 0, INPUT_RATE_INSTANCE) -- the SAME bytes the compute shader
// wrote to the SSBO, re-interpreted as vertex inputs with stride 32 + matching offsets.
var @in @location = 0 a_pos  : float3
var @in @location = 1 a_life : float
var @in @location = 2 a_vel  : float3

var @out @location = 0 v_speed   : float           // |vel|, drives colour
var @out @location = 1 v_life    : float           // 0..1, drives intensity

[vulkan_vertex_shader(name="particles_vert_spv")]
def particles_vs {
    gl_Position = cam.proj * cam.view * float4(a_pos, 1.0)
    gl_PointSize = PARTICLE_PX                       // BuiltIn PointSize write -> rasteriser splat width
    v_speed = sqrt(dot(a_vel, a_vel))
    v_life = a_life
}

// ===== fragment shader =====

var @in @location = 0 f_speed : float
var @in @location = 1 f_life  : float
var @out @location = 0 frag_color : float4

[vulkan_fragment_shader(name="particles_frag_spv")]
def particles_fs {
    // gl_PointCoord is the UV inside the rasterised point primitive, [0,1]^2 with (0.5, 0.5) at the
    // centre. Distance from centre gives a circular mask + soft falloff -- the basis for a real
    // point-sprite look without uploading a texture.
    let from_center = gl_PointCoord - float2(0.5, 0.5)
    let d = sqrt(dot(from_center, from_center)) * 2.0     // 0 at centre, 1 at the edge of the inscribed circle
    if (d > 1.0) {
        discard()                                          // outside the disc: punch a hole through the square
    }
    // soft glow: bright core, falloff to zero alpha at the rim
    let core = 1.0 - clamp(d, 0.0, 1.0)
    let glow = core * core * core                          // cubic falloff reads as a hot core + halo
    // colour by speed: cool teal for slow particles, hot magenta for fast
    let cool = float3(0.20, 0.70, 1.00)
    let hot  = float3(1.00, 0.30, 0.55)
    let s = clamp(f_speed * 0.4, 0.0, 1.0)
    let s3 = float3(s, s, s)
    let base = lerp(cool, hot, s3)
    // Additive blending on the pipeline means premultiplied colour with alpha=glow gives a pure
    // additive glow with a per-pixel falloff -- the canonical "particle" look.
    let intensity = glow * f_life
    frag_color = float4(base * intensity, intensity)
}
