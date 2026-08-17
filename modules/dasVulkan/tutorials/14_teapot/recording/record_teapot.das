options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 14 recording driver (manual, local-only). Spins the GPU-tessellated teapot through two
// full turns over one 30 s take (30 fps * 30 s -> 900 frames) on its grounded stage -- env sky,
// reflective checkerboard floor (receiving the mesh-shader shadow), and the glazed-ceramic teapot.
// The per-patch debug tint auto-toggles every 3 seconds, so a viewer watches the smooth ceramic
// glaze dissolve into the rainbow "1 Bezier patch = 1 mesh workgroup" decomposition and back. Needs
// a GPU with VK_EXT_mesh_shader -- this is why CI never runs it (lavapipe lacks the extension; the
// tutorials .das_test also skips `recording/` when stbimage / audio / dasHV aren't loaded).
//
//   <daslang> -load_module <dasVulkan> tutorials/14_teapot/recording/record_teapot.das

require ../teapot_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path
require math

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "teapot.apng")
    let out_mp4 = path_join(dir_name(here), "teapot.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Two seamless spins over the take: teapot_model rotates by time*0.6, so spin_max = 4*pi/0.6 -> 2 turns.
    let spin_max = 4.0f * PI / 0.6f

    // Captions are canonical but short: this tutorial captures at 512 px wide (others are 768), so
    // terse keeps them inside the frame; the voice (respelled per skills/recording.md) carries the detail.
    say(30,  "dasVulkan tutorial 14 -- GPU teapot",
             "das Vulkan tutorial fourteen. The G P U teapot. Mesh shaders.")
    say(300, "Mesh shader tessellates 32 patches",
             "The mesh shader tessellates the thirty two Bezier patches. straight on the G P U. No vertex buffer.")
    say(540, "Debug tint: patch = workgroup",
             "The rainbow tint shows the structure. One Bezier patch. is one mesh workgroup.")
    say(780, "Grounded stage + mesh-shader shadow",
             "A grounded stage. env sky. reflective floor. and a shadow map. baked by a second mesh shader pass.")

    if (!mesh_shader_available()) {
        panic("VK_EXT_mesh_shader not available -- tutorial 14 needs a mesh-shader-capable GPU")
    }

    var inscope ctx <- build_teapot_context()

    let ok = capture_apng(apng, TEAPOT_W, TEAPOT_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        let phase = float(frame) / float(n_frames)
        let spin_t = phase * spin_max
        let dbg = (int(t / 3.0f) % 2) == 1      // per-patch tint toggles every 3 s
        return <- render_teapot_frame(ctx, spin_t, dbg)
    }
    if (!ok) {
        panic("capture failed")
    }
    if (!prepare_voiceover(apng)) {
        panic("voiceover failed -- is Kokoro running at :8880?")
    }
    if (!convert_to_mp4_voiced(apng, music, daslang_exe, out_mp4, dur_s, fps, -13, 100)) {
        panic("convert failed")
    }
    to_log(LOG_INFO, "[record] tutorial 14 done\n")
}
