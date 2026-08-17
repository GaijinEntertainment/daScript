options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 12 recording driver (manual, local-only). Renders the GPU-driven occlusion-culling scene
// while the occluder wall sweeps across the field (30 s at 30 fps -> 900 frames), captures an APNG,
// then ffmpeg-muxes a daStrudel music bed + voice + captions into the committed ../gpu_driven.mp4.
// CI never runs this -- the tutorials .das_test skips the `recording/` folder when stbimage / audio /
// dasHV aren't loaded.
//
//   <daslang> -load_module <dasVulkan> tutorials/12_gpu_driven/recording/record_gpu_driven.das

require ../gpu_driven_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path
require math

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "gpu_driven.apng")
    let out_mp4 = path_join(dir_name(here), "gpu_driven.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_signal_filter_reverb.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text respelled phonetically (ASCII only -- no em-dash / arrows).
    say(15,  "dasVulkan tutorial 12 -- GPU-driven rendering",
             "das Vulkan tutorial twelve. G P U driven rendering.")
    say(210, "Hi-Z occlusion culling: a compute shader tests every cube against a depth pyramid",
             "High Z occlusion culling. A compute shader tests every cube against a depth pyramid.")
    say(420, "survivors compact into an indirect-draw buffer with a GPU-written count",
             "Survivors compact into an indirect draw buffer, with a G P U written count. The C P U never learns how many survive.")
    say(620, "bindless materials: nonuniformEXT indexes a sampler array, keyed by gl_DrawID",
             "Bindless materials. non uniform E X T indexes a sampler array, keyed by G L draw I D.")
    say(800, "left: the camera sees only survivors. right: the god view reveals the culled ghosts",
             "Left, the camera sees only survivors. Right, the god view reveals the culled cubes as ghosts.")

    var inscope ctx <- build_gpu_driven_context()

    let ok = capture_apng(apng, OUT_W, OUT_H, n_frames, frame_ms) $(frame : int) {
        let progress = float(frame) / float(n_frames)
        let wall_x = 5.5f * sin(progress * 2.0f * PI * 1.5f)     // ~1.5 sweeps across the field
        return <- render_frame(ctx, wall_x)
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
    to_log(LOG_INFO, "[record] tutorial 12 done\n")
}
