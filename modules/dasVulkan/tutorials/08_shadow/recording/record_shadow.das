options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 08 recording driver (manual, local-only). Renders the two-pass shadow scene headlessly
// over one full camera orbit + several sun rotations (30 fps * 30 s -> 900 frames), captures an
// APNG, and ffmpeg-muxes a daStrudel music bed + voice + captions into ../shadow.mp4. CI never
// runs this -- the tutorials .das_test skips the `recording/` folder when stbimage / audio / dasHV
// aren't loaded. Sibling of record_particles.das.
//
//   <daslang> -load_module <dasVulkan> tutorials/08_shadow/recording/record_shadow.das

require ../shadow_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path
require math

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "shadow.apng")
    let out_mp4 = path_join(dir_name(here), "shadow.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled phonetically per skills/recording.md.
    say(30,  "dasVulkan tutorial 8 -- shadow mapping",
             "das Vulkan tutorial eight. Shadow mapping.")
    say(330, "Two passes. One depth image",
             "Two passes. One depth image. Light view first. Camera view second.")
    say(660, "textureCompare. Hardware PCF",
             "Sampler 2 D shadow. Texture compare. Hardware P C F.")

    var inscope ctx <- build_shadow_context()

    let ok = capture_apng(apng, SCENE_W, SCENE_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        let camera_t = float(frame) / float(n_frames) * 2.0f * PI
        return <- render_shadow_frame(ctx, t, camera_t)
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
    to_log(LOG_INFO, "[record] tutorial 08 done\n")
}
