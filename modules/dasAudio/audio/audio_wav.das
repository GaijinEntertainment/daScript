options gen2
options indenting = 4

module audio_wav shared private
//! WAV file reading and writing (16-bit PCM format).

require daslib/fio
require math
require strings

def public read_wav(fname : string; var samples : array<float>&; var sample_rate : int&; var channels : int&) : bool {
    //! Read a WAV file into float samples. Returns true on success. Sets sample_rate and channels.
    var raw : array<uint8>
    fopen(fname, "rb") $(f) {
        if (f != null) {
            let st = fstat(f)
            if (st.is_valid) {
                raw |> resize(int(st.size))
                fread(f, raw)
            }
        }
    }
    if (length(raw) < 44) return false
    var cursor = 0
    if (read_wav_tag(raw, cursor) != "RIFF") return false
    cursor += 4
    if (read_wav_tag(raw, cursor) != "WAVE") return false
    var fmt_channels = 0
    var fmt_rate = 0
    var fmt_bits = 0
    var data_offset = 0
    var data_size = 0
    while (cursor < length(raw) - 8) {
        let chunk_id = read_wav_tag(raw, cursor)
        let chunk_size = read_wav_u32(raw, cursor)
        if (chunk_id == "fmt ") {
            cursor += 2
            fmt_channels = read_wav_u16(raw, cursor)
            fmt_rate = int(read_wav_u32(raw, cursor))
            cursor += 4
            cursor += 2
            fmt_bits = read_wav_u16(raw, cursor)
            cursor += int(chunk_size) - 16
        } elif (chunk_id == "data") {
            data_offset = cursor
            data_size = int(chunk_size)
            cursor += data_size
        } else {
            cursor += int(chunk_size)
        }
        if (cursor % 2 != 0) {
            cursor ++
        }
    }
    if (data_offset == 0 || fmt_bits != 16) return false
    sample_rate = fmt_rate
    channels = fmt_channels
    let num_samples = data_size / 2
    samples |> resize(num_samples)
    var c = data_offset
    for (i in range(num_samples)) {
        let lo = int(raw[c])
        let hi = int(raw[c + 1])
        var v = lo | (hi << 8)
        if (v >= int(0x8000)) {
            v -= int(0x10000)
        }
        samples[i] = float(v) / 32768.0
        c += 2
    }
    return true
}

def read_wav_tag(data : array<uint8>; var cursor : int&) : string {
    if (cursor + 3 >= length(data)) return ""
    let r = "{to_char(int(data[cursor]))}{to_char(int(data[cursor+1]))}{to_char(int(data[cursor+2]))}{to_char(int(data[cursor+3]))}"
    cursor += 4
    return r
}

def read_wav_u16(data : array<uint8>; var cursor : int&) : int {
    let v = int(data[cursor]) | (int(data[cursor + 1]) << 8)
    cursor += 2
    return v
}

def read_wav_u32(data : array<uint8>; var cursor : int&) : uint {
    let v = uint(data[cursor]) | (uint(data[cursor + 1]) << 8u) | (uint(data[cursor + 2]) << 16u) | (uint(data[cursor + 3]) << 24u)
    cursor += 4
    return v
}

def public write_wav(fname : string; samples : array<float>; sample_rate : uint = 44100u; channels : int = 2) {
    //! Write float samples as a 16-bit PCM WAV file.
    fopen(fname, "wb") $(f) {
        if (f == null) return
        let numFrames = length(samples) / channels
        let dataSize = uint(numFrames * channels * 2)
        fwrite(f, "RIFF")
        fwrite(f, 36u + dataSize)
        fwrite(f, "WAVE")
        fwrite(f, "fmt ")
        fwrite(f, 16u)
        fwrite(f, uint16(1))
        fwrite(f, uint16(channels))
        fwrite(f, sample_rate)
        fwrite(f, sample_rate * uint(channels) * 2u)
        fwrite(f, uint16(channels * 2))
        fwrite(f, uint16(16))
        fwrite(f, "data")
        fwrite(f, dataSize)
        for (s in samples) {
            fwrite(f, int16(clamp(s, -1.0, 1.0) * 32767.0))
        }
    }
}
