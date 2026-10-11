options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 16 recording driver (manual, local-only): renders the three lit cubes headlessly over one
// camera orbit (24 seconds at 30 fps -> 720 frames), captures an APNG and ffmpeg-muxes a daStrudel bed
// into ../vulkan14.mp4 (gitignored; the deliverable ships on the docs-assets release as
// vulkan_vulkan14.mp4). CI never runs this - the tutorials .das_test skips the recording/ folder when
// stbimage / audio / dasHV are not loaded. Silent sibling of record_triangle.das.
//
//   <daslang> -project_root . modules/dasVulkan/tutorials/16_vulkan14/recording/record_vulkan14.das

require ../vulkan14_tut.das
require ../../recording/tutorial_record.das
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
    let n_frames = 24 * fps
    let dur_s = float(n_frames) / float(fps)

    var inscope ctx <- build_vulkan14_context()
    let ok = capture_apng(apng, V14_W, V14_H, n_frames, 1000 / fps) $(frame : int) {
        return <- render_vulkan14_frame(ctx, float(frame) / float(fps))
    }
    if (!ok) {
        panic("capture failed")
    }
    if (!convert_to_mp4(apng, music, daslang_exe, out_mp4, dur_s, fps, -13, 100)) {
        panic("convert failed")
    }
    to_log(LOG_INFO, "[record] tutorial 16 done\n")
}
