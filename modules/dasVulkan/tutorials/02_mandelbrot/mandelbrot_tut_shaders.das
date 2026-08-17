options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02 - the Mandelbrot set, computed entirely on the GPU. A [compute_shader] runs one
// invocation per pixel: it maps the pixel to a complex number c, iterates z = z*z + c until |z| > 2
// or it hits MAX_ITER, and writes a color derived from the escape count into a storage image. Every
// line is daslang, lowered to SPIR-V at compile time by dasSpirv -- the escape-time loop exercises
// the compute tier's control flow (while + break) and float arithmetic. The host reads the image back.

module mandelbrot_tut_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

// the output storage image (binding 0); the render path creates it Rgba8 and reads it back.
var @binding = 0 out_img : image2D

//! the square render resolution, used by the render path (image extent) and the test (oracle). The
//! shader sizes its pixel->complex mapping off the bound image via imageSize, so it does not reference
//! this -- the host just needs one number. Must be a multiple of 8 (the compute local size).
let public MAND_DIM = 512

// the view rectangle in the complex plane. Square and symmetric in the imaginary axis so the set's
// reflection symmetry (c and its conjugate escape identically) lands as exact pixel mirror symmetry --
// the test's float-robust oracle. Centered at real = -0.5 (inside the main cardioid).
let REAL_MIN = -2.0f
let REAL_MAX = 1.0f
let IMAG_MIN = -1.5f
let IMAG_MAX = 1.5f
let MAX_ITER = 100

[compute_shader(local_size_x=8, local_size_y=8, name="mandelbrot_spv")]
def mandelbrot {
    let gid = gl_GlobalInvocationID
    let dim = imageSize(out_img)
    let cx = REAL_MIN + (float(gid.x) + 0.5f) / float(dim.x) * (REAL_MAX - REAL_MIN)
    let cy = IMAG_MIN + (float(gid.y) + 0.5f) / float(dim.y) * (IMAG_MAX - IMAG_MIN)
    var zx = 0.0f
    var zy = 0.0f
    var n = 0
    while (n < MAX_ITER) {
        let x2 = zx * zx
        let y2 = zy * zy
        if (x2 + y2 > 4.0f) {
            break
        }
        let nzx = x2 - y2 + cx
        zy = 2.0f * zx * zy + cy
        zx = nzx
        n++
    }
    let coord = int2(int(gid.x), int(gid.y))
    if (n == MAX_ITER) {
        // inside the set: black
        imageStore(out_img, coord, float4(0.0f, 0.0f, 0.0f, 1.0f))
    } else {
        // outside: a simple arithmetic ramp on the escape count (no trig, so no math intrinsics)
        let t = float(n) / float(MAX_ITER)
        imageStore(out_img, coord, float4(t, t * 0.5f, 1.0f - t, 1.0f))
    }
}
