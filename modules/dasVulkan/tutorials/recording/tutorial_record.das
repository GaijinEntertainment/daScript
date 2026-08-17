options gen2
options _comment_hygiene = true
options indenting = 4

// Shared tutorial recording harness (manual, local-only). Two steps reused by every silent driver:
//   capture_apng  -- stream a parametric frame(i) render into an APNG intermediate.
//   convert_to_mp4 -- render a strudel music bed to the clip length, then ffmpeg-mux APNG + faded
//                     bed into the committed .mp4 (only the .mp4 is tracked; APNG/wav are scratch).
//
// Voiced drivers (Kokoro voiceovers + ffmpeg drawtext captions) live in the sibling
// `tutorial_record_voiced.das` -- a separate file so this one's compile-check in CI stays clean
// even when dasHV / dasOPENAI aren't loaded.
//
// This is NOT a regression gate -- each tutorial's pixel oracle (its [test]) is. This folder is
// named `recording` so the tutorials .das_test skips it in CI (stbimage + audio aren't loaded there).

module tutorial_record

require stbimage
require daslib/fio
require math

// The APNG writer flips rows top-down for GL glReadPixels (bottom-up) data; Vulkan readback is
// already top-down, so we pre-flip each frame to cancel the writer's flip (apng_write_impl.h:
// "callers should flip in their pixel buffer instead").
def private flip_rows_inplace(var pixels : array<uint8>; w, h : int) {
    let row = w * 4
    for (y in range(h / 2)) {
        let top = y * row
        let bot = (h - 1 - y) * row
        for (i in range(row)) {
            let tmp = pixels[top + i]
            pixels[top + i] = pixels[bot + i]
            pixels[bot + i] = tmp
        }
    }
}

//! Stream RGBA8 frames into an APNG. `render(frame)` returns one w*h*4 RGBA8 buffer per frame index;
//! `frame_ms` is the per-frame delay stored in the APNG. Returns true on success.
def public capture_apng(apng_path : string; w, h, n_frames, frame_ms : int;
                        render : block<(frame : int) : array<uint8>>) : bool {
    var writer = stbi_apng_begin(apng_path, w, h, 4)
    if (writer == null) {
        to_log(LOG_ERROR, "[record] stbi_apng_begin failed for {apng_path}\n")
        return false
    }
    var ok = true
    for (f in range(n_frames)) {
        var pixels <- invoke(render, f)
        if (length(pixels) < w * h * 4) {
            to_log(LOG_ERROR, "[record] frame {f}: short buffer ({length(pixels)} < {w * h * 4})\n")
            ok = false
            delete pixels
            break
        }
        flip_rows_inplace(pixels, w, h)
        var rc = 0
        unsafe {
            rc = stbi_apng_frame(writer, addr(pixels[0]), w * 4, frame_ms)
        }
        delete pixels
        if (rc != 1) {
            to_log(LOG_ERROR, "[record] stbi_apng_frame {f} failed\n")
            ok = false
            break
        }
    }
    if (stbi_apng_end(writer) != 1) {
        to_log(LOG_ERROR, "[record] stbi_apng_end failed\n")
        ok = false
    }
    return ok
}

//! Locate the daslang executable under `das_root`, probing the common build layouts across platforms
//! (Windows MSVC multi-config, single-config, Unix install / build dirs). Falls back to bare "daslang"
//! (resolved via PATH) when none exist, so the recording driver runs on Windows, Linux, and macOS.
def public find_daslang_exe(das_root : string) : string {
    let candidates <- [
        path_join(das_root, "bin/Release/daslang.exe"),    // Windows MSVC multi-config
        path_join(das_root, "bin/daslang.exe"),            // Windows single-config
        path_join(das_root, "bin/daslang"),                // Unix install layout
        path_join(das_root, "build/daslang")]              // Unix Make/Ninja build dir
    for (c in candidates) {
        var st : FStat
        if (stat(c, st)) {
            return c
        }
    }
    return "daslang"
}

//! Render the strudel music bed to `dur_s` seconds, then ffmpeg-mux the APNG video + a faded,
//! low-volume bed into `out_mp4`. `daslang_exe` runs `music_feature` (a daStrudel feature) as a
//! subprocess to synthesize the bed. Returns true on success.
def public convert_to_mp4(apng_path, music_feature, daslang_exe, out_mp4 : string;
                          dur_s : float; fps, bed_db, fade_ms : int) : bool {
    let music_wav = path_join(dir_name(apng_path), "{stem(apng_path)}_bed.wav")
    let margs <- [daslang_exe, music_feature, "--", "--wav", music_wav, "--duration", "{dur_s}"]
    var mout : string
    if (run_and_capture(margs, mout) != 0) {
        to_log(LOG_ERROR, "[record] music bed render failed:\n{mout}\n")
        return false
    }
    let fade = float(fade_ms) / 1000.0f
    let fade_out_st = max(dur_s - fade, 0.0f)
    let bed = "[1:a]afade=t=in:st=0:d={fade},afade=t=out:st={fade_out_st}:d={fade},volume={bed_db}dB[aout]"
    let fargs <- [
        "ffmpeg", "-y", "-loglevel", "error", "-r", "{fps}", "-i", apng_path, "-i", music_wav,
        "-filter_complex", bed, "-map", "0:v", "-map", "[aout]",
        "-c:v", "libx264", "-preset", "medium", "-crf", "23", "-pix_fmt", "yuv420p",
        "-movflags", "+faststart", "-shortest", out_mp4]
    var fout : string
    if (run_and_capture(fargs, fout) != 0) {
        to_log(LOG_ERROR, "[record] ffmpeg mux failed:\n{fout}\n")
        return false
    }
    to_log(LOG_INFO, "[record] wrote {out_mp4}\n")
    return true
}
