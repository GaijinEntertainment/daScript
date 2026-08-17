options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 10 recording driver (manual, local-only). Renders the deferred-shading scene in AUTO
// mode for one full camera orbit (30 fps * 30 s -> 900 frames). The 16-row mode-indicator strip
// auto-cycles LIT -> ALBEDO -> NORMAL -> WORLD-POS -> SSAO -> LIGHTING-ONLY every 2.5 seconds, so
// a viewer literally watches the G-buffer + SSAO + lighting decompose themselves. CI never runs
// this -- the tutorials .das_test skips `recording/` when stbimage / audio / dasHV aren't loaded.
//
//   <daslang> -load_module <dasVulkan> tutorials/10_deferred/recording/record_deferred.das

require ../deferred_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "deferred.apng")
    let out_mp4 = path_join(dir_name(here), "deferred.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled phonetically per skills/recording.md.
    // Frames spaced so the last line finishes well before the 30 s video ends (no audio cut-off).
    say(30,  "dasVulkan tutorial 10 -- deferred shading",
             "das Vulkan tutorial ten. Deferred shading.")
    say(210, "G-buffer pass. MRT writes albedo. normal. world-pos",
             "G buffer pass. M R T writes the concrete cat's albedo. normal. and world position.")
    say(420, "SSAO pass samples the G-buffer. real hemisphere occlusion",
             "S S A O pass samples the G buffer textures. real hemisphere occlusion grounds the cat.")
    say(660, "Lighting pass. G-buffer + SSAO + shadow + HDR environment",
             "Lighting pass samples the G buffer. S S A O. the shadow map. and the H D R environment.")

    var inscope ctx <- build_deferred_context()

    let ok = capture_apng(apng, DEFERRED_W, DEFERRED_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        let camera_t = float(frame) / float(n_frames)
        let mode = deferred_active_mode(DeferredToggle.auto_toggle, t)
        return <- render_deferred_frame(ctx, t, camera_t, mode)
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
    to_log(LOG_INFO, "[record] tutorial 10 done\n")
}
