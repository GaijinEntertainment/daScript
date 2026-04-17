.. _tutorial_dastrudel_midi_files:

==========================
STRUDEL-14 — MIDI Files
==========================

.. index::
    single: Tutorial; Strudel; MIDI
    single: Tutorial; Standard MIDI File
    single: Tutorial; Tempo map

daStrudel parses and plays Standard MIDI Files (.mid) — the universal
sheet-music format.  You get a structured view of tracks, events, and
tempo, plus a high-level ``midi_play`` entry point that runs the file
through either built-in samples or an SF2 SoundFont.

Part A: Parsing
===============

``load_midi`` reads a ``.mid`` file and returns a ``MidiFile`` struct:

.. code-block:: das

    require strudel/strudel_midi

    let midi = load_midi("fur_elise.mid")
    print("Format: {midi.format}\n")         // 0 = single track, 1 = multi-track
    print("Ticks/QN: {midi.ticks_per_qn}\n") // timing resolution
    print("Tracks: {length(midi.tracks)}\n")

Each track is a ``MidiTrack`` containing an array of ``MidiEvent``.
Events carry a ``tick`` timestamp, a ``MidiEventKind`` (note-on, note-off,
control change, tempo, etc.), a ``channel`` number, and two data bytes.

Format 0 packs everything into one track; format 1 uses parallel tracks
— daStrudel handles both.  ``ticks_per_qn`` is the time resolution:
higher values mean finer timing subdivisions.

Part B: Merging and Tempo
=========================

``merge_tracks`` collapses all tracks into one stable-sorted event
stream, which is easier to walk for analysis:

.. code-block:: das

    var merged <- merge_tracks(midi)
    for (evt in merged) {
        if (evt.kind == MidiEventKind.midi_tempo && evt.tempo > 0) {
            let bpm = 60000000.0 / float(evt.tempo)
            print("Tempo change at tick {evt.tick}: {bpm:.1f} BPM\n")
        }
    }

Tempo events store microseconds-per-quarter-note in the ``tempo`` field;
convert to BPM with ``60_000_000 / tempo``.  Files with no tempo events
default to 120 BPM.

Part C: Playback with Sample Banks
==================================

The ``midi_play`` path uses the audio system's streaming infrastructure
and a built-in piano + drum sample bank.  The lifecycle:

.. code-block:: das

    require strudel/strudel_midi_player

    midi_load_samples(media_path)   // load built-in banks from media folder
    midi_init()                     // start the playback thread
    midi_play("music", "fur_elise.mid", [gain = 0.6, looping = false])
    sleep(6000u)
    midi_stop("music")
    midi_shutdown()

The first argument to ``midi_play`` is an arbitrary **track name** — you
can play multiple MIDI files at once and control them independently
(cross-fade, stop, set volume) via that name.

Part D: Playback with SF2
=========================

For higher-quality rendering, swap the sample backend for a General MIDI
SoundFont.  ``midi_load_sf2`` activates it; subsequent ``midi_play``
calls dispatch notes through the SoundFont presets (channel 10 uses the
GM drum kit automatically):

.. code-block:: das

    midi_load_sf2("FluidR3_GM.sf2")
    midi_init()
    midi_play("music", "Bach_Air_on_G_string.mid", [gain = 1.0])
    sleep(8000u)
    midi_shutdown()

The SF2 backend respects program change events — if the MIDI file
switches instruments mid-song, the SoundFont follows.  Pair with
:ref:`tutorial_dastrudel_sf2_soundfont` for per-instrument control from
live patterns.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_14_midi_files.das <../../../../tutorials/daStrudel/daStrudel_14_midi_files.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_sf2_soundfont`

   Next tutorial: :ref:`tutorial_dastrudel_live_reloading`
