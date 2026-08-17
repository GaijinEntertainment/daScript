options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 11 recording driver (manual, local-only). Renders the HDR + bloom scene over one full
// camera orbit (30 s at 30 fps -> 900 frames), captures an APNG, then ffmpeg-muxes the daStrudel
// music bed + voice + captions into the committed ../hdr.mp4. CI never runs this -- the tutorials
// .das_test skips the `recording/` folder when stbimage / audio / dasHV aren't loaded.
//
//   <daslang> -load_module <dasVulkan> tutorials/11_hdr/recording/record_hdr.das

require ../hdr_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "hdr.apng")
    let out_mp4 = path_join(dir_name(here), "hdr.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text respelled phonetically per skills/recording.md.
    say(30,  "dasVulkan tutorial 11 -- HDR + bloom",
             "das Vulkan tutorial eleven. H D R plus bloom.")
    say(330, "HDR scene. R 16 G 16 B 16 A 16 SFLOAT. emissive cubes",
             "H D R scene. R sixteen G sixteen B sixteen A sixteen S float. Emissive cubes punch above one.")
    say(570, "Karis 5-level bloom pyramid. downsample plus tent upsample",
             "Karis five level bloom pyramid. Downsample plus tent upsample. Additive blend.")
    say(810, "ACES tonemap. composite to sRGB",
             "A C E S tone map. Composite to s R G B.")

    var inscope ctx <- build_hdr_context()

    let ok = capture_apng(apng, HDR_W, HDR_H, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        let camera_t = float(frame) / float(n_frames)
        return <- render_hdr_frame(ctx, t, camera_t)
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
    to_log(LOG_INFO, "[record] tutorial 11 done\n")
}
