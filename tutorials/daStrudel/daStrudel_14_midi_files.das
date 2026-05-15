// Tutorial DASTRUDEL-14: MIDI Files
//
// This tutorial covers:
//   - Parsing MIDI files with load_midi (format 0 and format 1)
//   - Inspecting tracks, events, and tempo data
//   - merge_tracks: combining tracks into one sorted event list
//   - Playback with built-in sample banks via midi_play
//   - Switching to SF2 synthesis with midi_load_sf2
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_14_midi_files.das

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel_midi
require strudel/strudel_midi_player
require audio/audio_boost
require daslib/fio

let MEDIA = "{get_das_root()}/examples/media"

def find_sf2() : string {
    let dir = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"
    var st : FStat
    if (stat("{dir}/FluidR3_GM.sf2", st)) return "{dir}/FluidR3_GM.sf2"
    if (stat("{dir}/GeneralUser GS v1.471.sf2", st)) return "{dir}/GeneralUser GS v1.471.sf2"
    return ""
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Parse a MIDI file and inspect it
// ──────────────────────────────────────────────────────────────────────────
//
// load_midi returns a MidiFile with:
//   - format:        0 (single track) or 1 (multi-track synchronous)
//   - ticks_per_qn:  timing resolution — ticks per quarter note
//   - tracks:        array of MidiTrack, each with an array of MidiEvent
//
// Each MidiEvent has tick, kind (MidiEventKind), channel, data1, data2.
// Tempo events stash microseconds-per-quarter-note in the tempo field.

def example_parse() {
    print("=== Section 1: Parse Fur Elise ===\n")
    let path = "{MEDIA}/midi/fur_elise.mid"
    let midi = load_midi(path)
    if (empty(midi.tracks)) {
        print("  (file not found — skipping)\n")
        return
    }
    print("  Format:       {midi.format}\n")
    print("  Ticks per QN: {midi.ticks_per_qn}\n")
    print("  Tracks:       {length(midi.tracks)}\n")
    for (i in range(length(midi.tracks))) {
        print("    Track {i}: {length(midi.tracks[i].events)} events\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — merge_tracks and tempo inspection
// ──────────────────────────────────────────────────────────────────────────
//
// merge_tracks collapses all tracks into one stable-sorted stream — easier
// to iterate for analysis. Here we walk it looking for tempo change events
// (MIDI meta events) and convert microseconds-per-QN to BPM.

def example_tempo() {
    print("\n=== Section 2: Tempo events ===\n")
    let midi = load_midi("{MEDIA}/midi/fur_elise.mid")
    if (empty(midi.tracks)) {
        print("  (file not found — skipping)\n")
        return
    }
    var merged <- merge_tracks(midi)
    print("  Merged events: {length(merged)}\n")
    var found_tempo = false
    for (evt in merged) {
        if (evt.kind == MidiEventKind.midi_tempo && evt.tempo > 0) {
            let bpm = 60000000.0 / float(evt.tempo)
            print("    Tempo change at tick {evt.tick}: {bpm:.1f} BPM\n")
            found_tempo = true
        }
    }
    if (!found_tempo) {
        print("  (no tempo events — defaults to 120 BPM)\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Playback with built-in sample banks
// ──────────────────────────────────────────────────────────────────────────
//
// midi_play runs a MIDI file on the audio system's streaming thread. The
// lifecycle is:
//   1. midi_load_samples(media_path)  — load piano + drum banks
//   2. midi_init()                    — spin up the playback thread
//   3. midi_play(name, path, ...)     — schedule a track (named so we can
//                                       control it later)
//   4. midi_stop(name) / midi_shutdown() — stop and tear down
// Multiple tracks can play at once — each gets its own name.

def example_play_samples() {
    print("\n=== Section 3: Play with built-in piano samples ===\n")
    with_audio_system() {
        midi_load_samples(MEDIA)
        midi_init()
        if (midi_play("music", "{MEDIA}/midi/fur_elise.mid", [gain = 0.6, looping = false])) {
            print("  Playing 6 seconds of Fur Elise (piano samples)...\n")
            sleep(6000u)
        } else {
            print("  (file not found — skipping)\n")
        }
        midi_stop("music")
        midi_shutdown()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Play with an SF2 SoundFont backend
// ──────────────────────────────────────────────────────────────────────────
//
// midi_load_sf2 swaps the sample-based backend for a GM SoundFont — every
// channel's program change now picks a real instrument, and channel 10 uses
// the SF2 drum kit. This is the highest-quality playback path.

def example_play_sf2() {
    print("\n=== Section 4: Play with SF2 backend ===\n")
    let sf2 = find_sf2()
    if (empty(sf2)) {
        print("  (no SF2 found — skipping)\n")
        return
    }
    with_audio_system() {
        midi_load_sf2(sf2)
        midi_init()
        if (midi_play("music", "{MEDIA}/midi/Bach_Air_on_G_string_BWV1068.mid", [gain = 1.0, looping = false])) {
            print("  Playing 8 seconds of Bach Air (SF2)...\n")
            sleep(8000u)
        } else {
            print("  (file not found — skipping)\n")
        }
        midi_stop("music")
        midi_shutdown()
    }
}

[export]
def main() {
    print("DASTRUDEL-14: MIDI Files\n\n")
    example_parse()
    example_tempo()
    example_play_samples()
    example_play_sf2()
    print("\nDone!\n")
}
