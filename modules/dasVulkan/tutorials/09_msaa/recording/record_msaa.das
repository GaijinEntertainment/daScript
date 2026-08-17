options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 09 recording driver (manual, local-only). Renders the spiky AA-test ball in AUTO toggle mode for one
// full camera orbit (30 fps * 30 s -> 900 frames). The 16-row mode-indicator strip flips between
// bright green (4x MSAA) and red (1x rasterization) every 2 seconds, so a viewer literally watches
// the edges go from jaggy to smooth and back. CI never runs this -- the tutorials .das_test skips
// `recording/` when stbimage / audio / dasHV aren't loaded.
//
//   <daslang> -load_module <dasVulkan> tutorials/09_msaa/recording/record_msaa.das

require ../msaa_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "msaa.apng")
    let out_mp4 = path_join(dir_name(here), "msaa.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled phonetically per skills/recording.md.
    say(30,  "dasVulkan tutorial 9 -- MSAA + dynamic rendering",
             "das Vulkan tutorial nine. M S A A. plus. Dynamic rendering.")
    say(330, "Spiky silhouette. Green = 4x MSAA. Red = 1x",
             "A hundred fifty spikes. Every edge a jaggy. Green is four times M S A A. Red is one times. No anti aliasing.")
    say(660, "VkPipelineRenderingCreateInfo. No render pass",
             "Pipeline carries the format info in p Next. No V K Render Pass. Vulkan one point three core.")

    var inscope ctx <- build_msaa_context()

    let ok = capture_apng(apng, MSAA_W, MSAA_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        let camera_t = float(frame) / float(n_frames)
        // AUTO mode resolves to bool via msaa_active() -- alternates every AUTO_PERIOD seconds.
        let use_msaa = msaa_active(MsaaToggle.auto_toggle, t)
        return <- render_msaa_frame(ctx, t, camera_t, use_msaa)
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
    to_log(LOG_INFO, "[record] tutorial 09 done\n")
}
