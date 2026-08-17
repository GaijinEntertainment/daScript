options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02, the "sample-as-texture" present path. The compute shader (mandelbrot_tut_shaders.das)
// wrote the fractal into a storage image; here a trivial graphics pass draws a single full-screen
// triangle whose fragment shader SAMPLES that image as a texture, so the result lands on the swapchain
// through the ordinary render-pass path (draw_frame) -- no blit needed. Both stages are daslang lowered
// to SPIR-V by dasSpirv. The blit path (show_mandelbrot_blit.das) needs no extra shaders; this one trades
// two tiny shaders for reusing the whole graphics present pipeline.

module mandelbrot_view_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

// full-screen triangle: three vertices that cover the whole clip-space square. Indexing a constant
// array by gl_VertexIndex (the dasSpirv const-array + dynamic-index path) keeps it to one draw of 3
// verts, no vertex buffer. clip = (-1,-1),(3,-1),(-1,3); the half-plane outside [-1,1] is clipped away.
var @out @location = 0 v_uv : float2
[vertex_shader(name="fsq_vert_spv")]
def fsq_vert {
    let clip = fixed_array(float2(-1.0, -1.0), float2(3.0, -1.0), float2(-1.0, 3.0))
    let p = clip[gl_VertexIndex]
    gl_Position = float4(p, 0.0f, 1.0f)
    v_uv = p * 0.5f + float2(0.5, 0.5)      // clip -> [0,1] texture coordinates
}

// sample the compute result (a combined image sampler at binding 0) and write it to the framebuffer.
var @binding = 0 src : sampler2D
var @in @location = 0 fi_uv : float2
var @out @location = 0 frag_color : float4
[fragment_shader(name="fsq_frag_spv")]
def fsq_frag {
    frag_color = texture(src, fi_uv)
}
