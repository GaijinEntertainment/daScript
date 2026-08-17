options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02 render path: dispatch the Mandelbrot compute shader over a MAND_DIM x MAND_DIM grid and
// return the RGBA8 pixels. The whole compute-to-storage-image path (storage image, descriptor set,
// dispatch, readback) lives in the boost rail compute_to_storage_image -- so the tutorial is one call,
// and the interesting part is the shader. The CI gate asserts analytic properties of these pixels; the
// recording driver and the windowed viewer render the same image.

module mandelbrot_tut

require vulkan public
require vulkan/vulkan_boost public
require mandelbrot_tut_shaders

//! Render the Mandelbrot set on the GPU; returns MAND_DIM*MAND_DIM RGBA8 pixels (inside the set is
//! black, outside is an escape-count ramp). mandelbrot_spv is the SPIR-V compute blob dasSpirv emitted
//! from mandelbrot_tut_shaders.das.
def public render_mandelbrot() : array<uint8> {
    return <- compute_to_storage_image(mandelbrot_spv, MAND_DIM, MAND_DIM, true)
}

//! Sample the RGB of pixel (x,y) from a MAND_DIM*MAND_DIM RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * MAND_DIM + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
