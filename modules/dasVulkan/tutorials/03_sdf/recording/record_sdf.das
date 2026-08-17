options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 03 recording driver (manual, local-only). Renders the SDF raymarch headlessly at fixed
// dt = 1/fps (no glfw window, no swapchain), captures an APNG of the camera orbiting once around the
// scene, then ffmpeg-muxes a daStrudel music bed + voice + captions into the committed ../sdf.mp4.
// CI never runs this -- the tutorials .das_test skips the `recording/` folder when stbimage/audio/dasHV
// aren't loaded. Sibling of record_mandelbrot.das and record_cube.das.
//
//   <daslang> -load_module <dasVulkan> tutorials/03_sdf/recording/record_sdf.das

require ../sdf_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "sdf.apng")
    let out_mp4 = path_join(dir_name(here), "sdf.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/sf2_integration_full_ambient.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled phonetically per skills/recording.md
    // (dasVulkan -> "das Vulkan", SPIR-V -> "Spear V" per Khronos, digits -> words for the ear).
    // Periods (not commas) force the TTS to pause between beats.
    say( 30, "dasVulkan tutorial 3 -- SDF ray march",
            "das Vulkan tutorial three. The SDF ray march.")
    say(360, "No vertices. No textures. Just math",
            "No vertices. No textures. The whole scene is math.")
    say(720, "daslang to SPIR-V",
            "Authored in daslang. Lowered to Spear V.")

    var inscope r <- build_sdf_renderer()

    // animation orbits once over the 30-second recording. cam_ang in the shader is t * 0.4, so a full
    // 2*pi orbit lands at t = 2*pi / 0.4 ~ 15.7 -- we map [0, 1) over the recording to that span so
    // the loop closes seamlessly.
    let orbit_t_max = 15.70796326f       // 2 * pi / 0.4

    let ok = capture_apng(apng, SDF_DIM, SDF_DIM, n_frames, frame_ms) $(frame : int) {
        let phase = float(frame) / float(n_frames)        // [0, 1)
        let t = phase * orbit_t_max
        return <- render_sdf_frame(r, t)
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
    to_log(LOG_INFO, "[record] tutorial 03 done\n")
}
