options gen2
options _comment_hygiene = true
options _dasllama_internal = true

//! Mono f32 PCM to WAV / raw PCM16 bytes and files - the encoders TTS output needs. Pure daslang,
//! so dasllama_core carries them; the decoders, which need the ``audio`` module, stay in
//! ``dasllama_audio_io``.

module dasllama_wav shared public

require dasllama/dasllama_lint public
require daslib/fio
require math

def private put_u16(var b : array<uint8>; v : int) {
    b |> push(uint8(v & 0xFF))
    b |> push(uint8((v >> 8) & 0xFF))
}

def private put_u32(var b : array<uint8>; v : uint) {
    b |> push(uint8(v & 0xFFu))
    b |> push(uint8((v >> 8u) & 0xFFu))
    b |> push(uint8((v >> 16u) & 0xFFu))
    b |> push(uint8((v >> 24u) & 0xFFu))
}

def private put_tag(var b : array<uint8>; tag : string) {
    b |> reserve(long_length(b) + long_length(tag))
    for (c in tag) {
        b |> push(uint8(c))
    }
}

//! Mono f32 PCM as a 16-bit RIFF/WAVE file's bytes at `sample_rate` - the container every
//! player and the OpenAI speech clients expect; samples clamp to [-1, 1].
def wav_pcm16_bytes(samples : array<float>; sample_rate : int) : array<uint8> {
    var b : array<uint8>
    let data_size = long_length(samples) * 2l   // the buffer is 64-bit; the RIFF header fields are the format's own 32 bits
    b |> reserve(44l + data_size)
    put_tag(b, "RIFF")
    put_u32(b, uint(36l + data_size))
    put_tag(b, "WAVE")
    put_tag(b, "fmt ")
    put_u32(b, 16u)
    put_u16(b, 1)
    put_u16(b, 1)
    put_u32(b, uint(sample_rate))
    put_u32(b, uint(sample_rate) * 2u)
    put_u16(b, 2)
    put_u16(b, 16)
    put_tag(b, "data")
    put_u32(b, uint(data_size))
    for (v in samples) {
        put_u16(b, roundi(clamp(v, -1.0, 1.0) * 32767.0))
    }
    return <- b
}

//! Write mono f32 PCM as a 16-bit WAV file; false when the path cannot be opened.
def write_wav_pcm16(path : string; samples : array<float>; sample_rate : int) : bool {
    let f = fopen(path, "wb")
    return false if (f == null)
    let bytes <- wav_pcm16_bytes(samples, sample_rate)
    fwrite(f, bytes)
    fclose(f)
    return true
}

//! Mono f32 PCM as raw 16-bit little-endian samples - the `pcm` response format.
def pcm16_bytes(samples : array<float>) : array<uint8> {
    var b : array<uint8>
    b |> reserve(long_length(samples) * 2l)
    for (v in samples) {
        put_u16(b, roundi(clamp(v, -1.0, 1.0) * 32767.0))
    }
    return <- b
}
