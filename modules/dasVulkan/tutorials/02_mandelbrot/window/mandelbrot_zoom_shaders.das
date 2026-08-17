options gen2
options _comment_hygiene = true
options indenting = 4

// The animated viewer shader (live windows only -- the headless render + CI oracle use the static
// mandelbrot_tut_shaders.das). A port of Inigo Quilez's smooth-coloured Mandelbrot zoom
// (https://www.shadertoy.com/view/ldf3DN): the whole animation is driven by one float -- a
// @push_constant time -- from which the shader derives an oscillating zoom and a slow rotation about
// the seahorse-valley point (-0.745, 0.186). Continuous (smooth) iteration count kills the banding;
// an iq cosine palette colours it. Every line is daslang, lowered to SPIR-V by dasSpirv.
//
// Now that dasSpirv supports user-defined functions (OpFunctionCall + ref params), the shader is
// authored like ordinary code: cmul / is_interior / mandel_smooth / palette / sample_at. mandel_smooth
// returns the iteration count and writes the exit magnitude back through a `var float&` ref param --
// the canonical dasSpirv ref-param case (Function-storage copy-in / copy-out around OpFunctionCall).

module mandelbrot_zoom_shaders public

require vulkan/vulkan_boost public  // CommandBuffer + PipelineLayout for the generated push_constants function
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

var @binding = 0 out_img : image2D

//! the square render resolution (multiple of 8). The host needs one number for the image extent.
let public ZOOM_DIM = 768
let MAX_ITER = 512
let ESCAPE2 = 65536.0f      // B*B with the iq bailout radius B = 256
let AA = 4                  // 4x4 supersampling: AA*AA = 16 colour samples per pixel, averaged

// the single animation parameter, pushed each frame from wall-clock time
struct Push {
    time : float
}
var @push_constant pc : Push

// complex multiply -- (a.x + i a.y) * (b.x + i b.y)
def private cmul(a, b : float2) : float2 {
    return float2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)
}

// iq's cheap interior tests: skip the iteration inside the main cardioid and the period-2 bulb.
def private is_interior(c : float2) : bool {
    let c2 = c.x * c.x + c.y * c.y
    let in_cardioid = 256.0f * c2 * c2 - 96.0f * c2 + 32.0f * c.x - 3.0f < 0.0f
    let in_bulb = 16.0f * (c2 + 2.0f * c.x + 1.0f) - 1.0f < 0.0f
    return in_cardioid || in_bulb
}

// Escape-time iteration with a large bailout radius (needed for smooth colouring). Returns the
// iteration count `n`; writes the exit magnitude |z|^2 back through `d_out`. n == MAX_ITER together
// with d_out <= ESCAPE2 means the point did not escape (treat as interior).
def private mandel_smooth(c : float2; var d_out : float&) : int {
    var z = float2(0.0f, 0.0f)
    var n = 0
    while (n < MAX_ITER) {
        z = cmul(z, z) + c
        if (z.x * z.x + z.y * z.y > ESCAPE2) {
            break
        }
        n++
    }
    d_out = z.x * z.x + z.y * z.y
    return n
}

// iq's cosine palette evaluated at a continuous-iteration phase t.
def private palette(t : float) : float3 {
    let phase = 3.0f + t * 0.15f
    return float3(0.5f + 0.5f * cos(phase),
                  0.5f + 0.5f * cos(phase + 0.6f),
                  0.5f + 0.5f * cos(phase + 1.0f))
}

// One supersample at complex point c: interior -> black, else continuous iteration count -> palette.
def private sample_at(c : float2) : float3 {
    if (is_interior(c)) {
        return float3(0.0f, 0.0f, 0.0f)
    }
    var d = 0.0f
    let n = mandel_smooth(c, d)
    if (d > ESCAPE2) {
        let sn = float(n) - log2(log2(d)) + 4.0f
        return palette(sn)
    }
    return float3(0.0f, 0.0f, 0.0f)
}

[vulkan_compute_shader(local_size_x=8, local_size_y=8, name="mandelbrot_zoom_spv")]
def mandelbrot_zoom {
    let gid = gl_GlobalInvocationID
    let dim = imageSize(out_img)
    // animated zoom (oscillates) + slow rotation, both derived from time, shared by all subsamples
    let t = pc.time
    let zoo0 = 0.62f + 0.38f * cos(0.07f * t)
    let ang = 0.15f * (1.0f - zoo0) * t
    let coa = cos(ang)
    let sia = sin(ang)
    let zoo = pow(zoo0, 8.0f)
    // average AA*AA subpixel colour samples (supersampling antialiasing). Interior samples are zero
    // and contribute nothing, so set-boundary pixels soften correctly.
    var col = float3(0.0f, 0.0f, 0.0f)
    for (s in range(AA * AA)) {
        // subpixel offset within the pixel: a centred AAxAA grid
        let fx = float(gid.x) + (float(s % AA) + 0.5f) / float(AA)
        let fy = float(gid.y) + (float(s / AA) + 0.5f) / float(AA)
        // screen-normalised coords, y in [-1,1] (iq's p = (-res + 2*frag)/res.y), then zoom + rotate
        let px = (2.0f * fx - float(dim.x)) / float(dim.y)
        let py = (2.0f * fy - float(dim.y)) / float(dim.y)
        let xr = px * coa - py * sia
        let yr = px * sia + py * coa
        let c = float2(-0.745f + xr * zoo, 0.186f + yr * zoo)
        col = col + sample_at(c)
    }
    let coord = int2(int(gid.x), int(gid.y))
    let avg = col * (1.0f / float(AA * AA))
    imageStore(out_img, coord, float4(avg, 1.0f))
}
