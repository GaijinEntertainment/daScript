options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 05 recording driver (manual, local-only). Renders the instanced swarm headlessly over
// one full camera orbit (30 seconds at 30 fps -> 900 frames), captures an APNG, and ffmpeg-muxes a
// daStrudel music bed + voice + captions into the committed ../instancing.mp4. CI never runs this
// -- the tutorials .das_test skips the `recording/` folder when stbimage / audio / dasHV aren't
// loaded. Sibling of record_cube.das / record_sdf.das.
//
//   <daslang> -load_module <dasVulkan> tutorials/05_instancing/recording/record_instancing.das

require ../instancing_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "instancing.apng")
    let out_mp4 = path_join(dir_name(here), "instancing.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled phonetically per skills/recording.md
    // (dasVulkan -> "das Vulkan", SPIR-V -> "Spear V" per Khronos, digits -> words for the ear).
    say(30, "dasVulkan tutorial 5 -- instancing",
            "das Vulkan tutorial five. Instancing.")
    say(360, "One draw call. A thousand cubes",
            "One draw call. A thousand cubes.")
    say(720, "Per-instance vertex attributes",
            "Per instance vertex attributes. The headline rail.")

    var inscope ctx <- build_instancing_context()

    let ok = capture_apng(apng, INSTANCE_W, INSTANCE_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)              // wall-clock seconds (per-instance breathing + Y rotation)
        let camera_t = float(frame) / float(n_frames)  // [0, 1) over the recording (one camera orbit)
        return <- render_instancing_frame(ctx, t, camera_t)
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
    to_log(LOG_INFO, "[record] tutorial 05 done\n")
}
