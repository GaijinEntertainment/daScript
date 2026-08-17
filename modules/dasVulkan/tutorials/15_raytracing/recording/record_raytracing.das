options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 15 recording driver (manual, local-only). One full camera orbit of the hardware
// ray-traced scene over a 30 s take (30 fps * 30 s -> 900 frames): the box, its traced shadow
// swinging around it, and the sky gradient -- every frame fired through vkCmdTraceRaysKHR.
// Needs an RT-capable GPU (VK_KHR_ray_tracing_pipeline) -- this is why CI never runs it
// (lavapipe has no RT; the tutorials .das_test also skips `recording/` when stbimage / audio /
// dasHV aren't loaded).
//
//   <daslang> -load_module <dasVulkan> tutorials/15_raytracing/recording/record_raytracing.das

require ../raytracing_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path
require math

let REC_DIM = 768

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "raytracing.apng")
    let out_mp4 = path_join(dir_name(here), "raytracing.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    say(30, "dasVulkan tutorial 15 -- hardware ray tracing",
        "das Vulkan tutorial fifteen. Hardware ray tracing.")
    say(270, "raygen / closest-hit / miss -- all daslang",
        "Ray generation. Closest hit. And miss shaders. All authored in daslang. Lowered to Spear V.")
    say(510, "one traced shadow ray per hit",
        "Each hit casts one shadow ray toward the light. A second miss shader flips it lit.")
    say(750, "VK_KHR_ray_tracing_pipeline",
        "Real hardware rays. Built. bound. and traced from daslang.")

    if (!rt_available()) {
        panic("VK_KHR_ray_tracing_pipeline not available -- tutorial 15 needs an RT-capable GPU")
    }

    var inscope ro <- build_rt_offscreen(REC_DIM, REC_DIM)

    let ok = capture_apng(apng, REC_DIM, REC_DIM, n_frames, frame_ms) $(frame : int) {
        let angle = float(frame) / float(n_frames) * 2.0f * PI
        return <- render_rt_frame(ro, angle)
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
    to_log(LOG_INFO, "[record] tutorial 15 done\n")
}
