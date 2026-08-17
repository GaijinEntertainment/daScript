options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 01 recording driver (manual, local-only). Sweeps the triangle through one full turn,
// captures an APNG, then muxes a strudel music bed into the committed ../triangle.mp4. CI never runs
// this -- the tutorials .das_test skips the `recording/` folder when stbimage/audio aren't loaded.
// The regression gate is the pixel oracle in ../test_triangle.das; this only produces the doc figure.
//
//   <daslang> -load_module <dasVulkan> tutorials/01_triangle/recording/record_triangle.das

require ../triangle_tut.das
require ../../recording/tutorial_record.das
require daslib/fio
require daslib/module_path
require math

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "triangle.apng")
    let out_mp4 = path_join(dir_name(here), "triangle.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/sf2_integration_full_ambient.das")

    let n_frames = 120
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    let ok = capture_apng(apng, TRI_W, TRI_H, n_frames, frame_ms) $(frame : int) {
        let angle = float(frame) / float(n_frames) * 2.0f * PI
        return <- render_spin_triangle(angle)
    }
    if (!ok) {
        panic("capture failed")
    }
    if (!convert_to_mp4(apng, music, daslang_exe, out_mp4, dur_s, fps, -13, 100)) {
        panic("convert failed")
    }
    to_log(LOG_INFO, "[record] tutorial 01 done\n")
}
