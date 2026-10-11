options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 16 recording driver (manual, local-only): renders the three lit cubes headlessly over one
// camera orbit (32 seconds at 30 fps -> 960 frames), captures an APNG and ffmpeg-muxes a daStrudel bed,
// the Kokoro voiceover and captions into ../vulkan14.mp4 (gitignored; the deliverable ships on the
// docs-assets release as vulkan_vulkan14.mp4). CI never runs this - the tutorials .das_test skips the
// recording/ folder when stbimage / audio / dasHV are not loaded. Voiced sibling of record_cube.das.
//
//   <daslang> -project_root . modules/dasVulkan/tutorials/16_vulkan14/recording/record_vulkan14.das

require ../vulkan14_tut.das
require ../../recording/tutorial_record_voiced.das
require daslib/fio
require daslib/module_path

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "vulkan14.apng")
    let out_mp4 = path_join(dir_name(here), "vulkan14.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/integration_supersaw_pad.das")

    let fps = 30
    let n_frames = 32 * fps
    let dur_s = float(n_frames) / float(fps)

    // Captions stay canonical; voice text is respelled for the ear (dasVulkan -> "das Vulkan", digits ->
    // words) and uses periods, not commas, to make the TTS pause between beats. Each anchor sits past the
    // previous line's measured length (4.4 s, 7.9 s, 7.4 s, 8.5 s) plus half a second.
    say(30, "dasVulkan tutorial 16 -- Vulkan 1.4",
        "das Vulkan tutorial sixteen. Vulkan one point four.")
    say(180, "Host image copy: no staging buffer, no command buffer",
        "Host image copy. Each texture goes from host memory straight into the image. No staging buffer. No command buffer.")
    say(435, "Push descriptors: no descriptor pool",
        "Push descriptors. Every descriptor is written into the command buffer right before the draw. No descriptor pool.")
    say(675, "Local read: two passes, one rendering, no render pass",
        "Local read. The lighting pass reads the albedo and the normal at the same fragment. Two passes. One rendering. No render pass.")

    var inscope ctx <- build_vulkan14_context()
    let ok = capture_apng(apng, V14_W, V14_H, n_frames, 1000 / fps) $(frame : int) {
        return <- render_vulkan14_frame(ctx, float(frame) / float(fps))
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
    to_log(LOG_INFO, "[record] tutorial 16 done\n")
}
