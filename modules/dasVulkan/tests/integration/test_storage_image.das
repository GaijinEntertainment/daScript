options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7 storage-image GPU gate: a daslang [compute_shader] (lowered by dasSpirv) writes a
// coordinate-derived gradient into a read-write image2D, and we read it back and assert EVERY pixel
// exactly. This is the real-driver proof of imageStore + the coordinate mapping + the Rgba8 storage
// format -- none of which spirv-val can check. pixel (x,y) = (x, y, 0, 255): R = gl_GlobalInvocationID.x,
// G = .y, B = 0, A = 255. The image is 64x64 so x/255 round-trips exactly through the unorm8 store.

require dastest/testing_boost public
require scene_helpers
require storage_image_shaders

[test]
def test_storage_image_gradient(t : T?) {
    var pixels <- compute_image_rgba8(grad_spv, 64, 64)
    assert_pixels_exact(t, pixels, 64, 64, "storage-image gradient") $(x, y : int) {
        return int4(x, y, 0, 255)
    }
    delete pixels
}
