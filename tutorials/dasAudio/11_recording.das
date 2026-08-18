// Tutorial AUDIO-11: Recording from the Microphone
//
// This tutorial covers:
//   - Enumerating capture (input) devices with sound_record_list_devices
//   - Recording to a WAV file with record_to_wav
//   - The low-level capture loop: sound_record_start / sound_record_read / sound_record_stop
//   - Reading the recording back with read_wav to confirm the round-trip
//
// Capture is independent of playback — you do NOT need with_audio_system here. The audio
// thread fills a lock-free ring buffer; your script drains it on the main thread. Samples are
// interleaved 32-bit float at the rate/channels you ask for.
//
// Run: daslang.exe tutorials/dasAudio/11_recording.das

options gen2
options persistent_heap
options indenting = 4

require audio/audio_record
require audio
require audio/audio_wav
require daslib/fio
require math

let SAMPLE_RATE = 44100
let CHANNELS = 1
let OUT_FILE = "recording.wav"

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Enumerate capture (microphone) devices
// ──────────────────────────────────────────────────────────────────────────
//
// Each AudioDeviceInfo carries a display name, whether it is the system default,
// and an index. Pass that index to record_to_wav / sound_record_start as
// device_index to target a specific microphone; -1 always means "system default".

def list_devices() {
    print("=== Section 1: Capture devices ===\n")
    let devices <- sound_record_list_devices()
    if (empty(devices)) {
        print("  no capture devices found\n")
        return
    }
    for (d in devices) {
        let tag = d.is_default ? " (default)" : ""
        print("  [{d.index}] {d.name}{tag}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Record two seconds to a WAV file (the easy way)
// ──────────────────────────────────────────────────────────────────────────
//
// record_to_wav opens the default capture device, records for the requested
// duration, writes a 16-bit PCM WAV, and returns false if the device could not
// be opened (e.g. no microphone, or the OS denied access). Pass a device_index
// as the last argument to record from a specific device instead of the default.

def record_clip() : bool {
    print("\n=== Section 2: Record {2.0} seconds to {OUT_FILE} ===\n")
    print("  speak into the microphone...\n")
    let ok = record_to_wav(OUT_FILE, 2.0, SAMPLE_RATE, CHANNELS)
    if (!ok) {
        print("  recording failed (no microphone, or access denied)\n")
        return false
    }
    print("  wrote {OUT_FILE}\n")
    return true
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Read the recording back
// ──────────────────────────────────────────────────────────────────────────
//
// read_wav (from audio/audio_wav) decodes the file we just wrote, so we can
// confirm the round-trip: same channel count, and a sample count that matches
// the duration we asked for.

def verify_clip() {
    print("\n=== Section 3: Read {OUT_FILE} back ===\n")
    var samples : array<float>
    var rate = 0
    var channels = 0
    if (!read_wav(OUT_FILE, samples, rate, channels)) {
        print("  could not read {OUT_FILE} (was it recorded?)\n")
        return
    }
    let seconds = float(length(samples) / channels) / float(rate)
    print("  {length(samples)} samples, {channels}ch @ {rate}Hz = {seconds} seconds\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — The manual capture loop (what record_to_wav does inside)
// ──────────────────────────────────────────────────────────────────────────
//
// For live processing you drive the loop yourself: start the device, then poll
// sound_record_read into a scratch buffer as fast as you consume it. Here we run
// for ~1 second and report the peak amplitude and any dropped frames (overflow
// means your drain loop fell behind the audio thread and the ring filled up).

def manual_capture() {
    print("\n=== Section 4: Manual capture loop (~1s) ===\n")
    if (!sound_record_start(SAMPLE_RATE, CHANNELS, SAMPLE_RATE, -1)) {
        print("  could not open capture device\n")
        return
    }
    var scratch : array<float>
    scratch |> resize(SAMPLE_RATE * CHANNELS)
    var frames = 0
    var peak = 0.0
    while (frames < SAMPLE_RATE) {
        let n = sound_record_read(scratch)
        if (n > 0) {
            for (i in range(n * CHANNELS)) {
                peak = max(peak, abs(scratch[i]))
            }
            frames += n
        } else {
            sleep(5u)
        }
    }
    sound_record_stop()
    print("  captured {frames} frames, peak amplitude {peak}, dropped {sound_record_overflow_frames()} frames\n")
}

[export]
def main() {
    print("AUDIO-11: Recording from the Microphone\n\n")
    list_devices()
    if (record_clip()) {
        verify_clip()
        remove(OUT_FILE)
        print("  removed {OUT_FILE} (drop the remove() to keep the clip)\n")
    }
    manual_capture()
    print("\nDone!\n")
}
