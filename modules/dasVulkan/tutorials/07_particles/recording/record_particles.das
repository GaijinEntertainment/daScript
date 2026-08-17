options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 07 recording driver (manual, local-only). Renders the compute-driven swarm headlessly
// over one full camera orbit (30 seconds at 30 fps -> 900 frames), captures an APNG, and
// ffmpeg-muxes a daStrudel music bed + voice + captions into the committed ../particles.mp4. CI
// never runs this -- the tutorials .das_test skips the `recording/` folder when
// stbimage / audio / dasHV aren't loaded. Sibling of record_cube.das / record_skybox.das.
//
//   <daslang> -load_module <dasVulkan> tutorials/07_particles/recording/record_particles.das

require ../particles_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "particles.apng")
    let out_mp4 = path_join(dir_name(here), "particles.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled phonetically per skills/recording.md.
    say(30, "dasVulkan tutorial 7 -- compute particles",
            "das Vulkan tutorial seven. Compute particles.")
    say(360, "Compute writes. Graphics reads",
            "Compute writes the same buffer. Graphics reads it as vertex attribs.")
    say(720, "One barrier between them",
            "One pipeline barrier between them. The handoff.")

    var inscope ctx <- build_particles_context()

    let dt_per_frame = 1.0f / float(fps)
    let ok = capture_apng(apng, PART_W, PART_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        let camera_t = float(frame) / float(n_frames)
        return <- render_particles_frame(ctx, t, dt_per_frame, camera_t)
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
    to_log(LOG_INFO, "[record] tutorial 07 done\n")
}
