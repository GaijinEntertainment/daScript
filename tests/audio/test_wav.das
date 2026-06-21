options gen2
options indenting = 4

require dastest/testing_boost
require audio/audio_wav
require daslib/fio
require strings
require math

// Exercises read_wav's integer-PCM decode paths (16 / 24 / 32-bit). write_wav only
// emits 16-bit, so the wider depths can't round-trip through the module — instead we
// hand-build a minimal mono RIFF/fmt/data WAV in memory at each depth (no binary
// fixtures committed) and assert read_wav reconstructs the samples, rate, and channels.

def private push_le(var a : array<uint8>; v, nbytes : int) {
    for (i in range(nbytes)) {
        a |> push(uint8((v >> (i * 8)) & 0xFF))   // nolint: PERF006 — 2..4 bytes, reserve is noise
    }
}

def private push_tag(var a : array<uint8>; s : string) {
    peek_data(s) $(bytes) {
        for (b in bytes) {
            a |> push(b)   // nolint: PERF022 — peek_data yields a temp (const#); push_from rejects it
        }
    }
}

// Encode `samples` (-1..1) as signed little-endian PCM at `bits` depth, wrapped in a
// canonical mono WAV container.
def private build_wav(bits : int; samples : array<float>; sr : int) : array<uint8> {
    let bytes_per = bits / 8
    var data : array<uint8>
    for (s in samples) {
        var v = 0
        if (bits == 16) {
            v = clamp(int(s * 32768.0), -32768, 32767)
        } elif (bits == 24) {
            v = clamp(int(s * 8388608.0), -8388608, 8388607)
        } else {
            v = int(s * 2147483647.0)
        }
        push_le(data, v, bytes_per)
    }
    let data_size = length(data)
    var w : array<uint8>
    push_tag(w, "RIFF")
    push_le(w, 36 + data_size, 4)
    push_tag(w, "WAVE")
    push_tag(w, "fmt ")
    push_le(w, 16, 4)                // fmt chunk size
    push_le(w, 1, 2)                 // format = integer PCM
    push_le(w, 1, 2)                 // channels = mono
    push_le(w, sr, 4)
    push_le(w, sr * bytes_per, 4)    // byte rate (mono)
    push_le(w, bytes_per, 2)         // block align (mono)
    push_le(w, bits, 2)
    push_tag(w, "data")
    push_le(w, data_size, 4)
    w |> push_from(data)
    return <- w
}

[test]
def test_read_wav_bit_depths(t : T?) {
    t |> run("read_wav decodes 16 / 24 / 32-bit integer PCM (mono)") @(t : T?) {
        let sr = 44100
        let want = [0.0, 0.5, -0.5, 0.25, 0.999, -0.999]
        for (bits in [16, 24, 32]) {
            let tmp = create_temp_file_result("das_read_wav_test", ".wav")   // unique writable path (no cwd assumptions / collisions)
            if (!(tmp is value)) {
                t |> success(false, "temp file created ({bits}-bit)")
                continue
            }
            let path = tmp as value
            let bytes <- build_wav(bits, want, sr)
            var wrote = false
            fopen(path, "wb") $(f) {
                if (f != null) {
                    fwrite(f, bytes)
                    wrote = true
                }
            }
            t |> success(wrote, "wrote {bits}-bit wav")
            var got : array<float>
            var gsr = 0
            var gch = 0
            let ok = read_wav(path, got, gsr, gch)
            t |> success(ok, "read_wav ok ({bits}-bit)")
            t |> success(gsr == sr, "rate preserved ({bits}-bit)")
            t |> success(gch == 1, "mono ({bits}-bit)")
            t |> success(length(got) == length(want), "sample count ({bits}-bit)")
            let tol = bits == 16 ? 0.0001 : 0.000002   // 1 quantum at the depth + truncation slack
            for (i in range(min(length(got), length(want)))) {
                t |> success(abs(got[i] - want[i]) < tol, "sample {i} within tol ({bits}-bit)")
            }
            remove(path)
        }
    }
}
