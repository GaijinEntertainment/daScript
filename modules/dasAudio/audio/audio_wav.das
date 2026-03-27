options gen2
options indenting = 4

module audio_wav shared private

require daslib/fio
require math

def public write_wav(fname : string; samples : array<float>; sample_rate : uint = 44100u; channels : int = 2) {
    fopen(fname, "wb") <| $(f) {
        if (f == null) {
            return
        }
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
