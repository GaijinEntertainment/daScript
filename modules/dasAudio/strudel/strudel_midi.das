options gen2
options indenting = 4

module strudel_midi shared public
//! MIDI Standard File (.mid) parser.
//! Reads format 0 and format 1 MIDI files, extracts note events, control changes,
//! program changes, pitch bend, and tempo information.

require daslib/fio
require daslib/algorithm
require math

// ─── Data Structures ───

//! MIDI event type discriminator.
enum MidiEventKind {
    midi_note_off
        //! Note released.
    midi_note_on
        //! Note pressed (velocity > 0) or released (velocity == 0).
    midi_control_change
        //! Controller value change (CC). data1 = controller number, data2 = value.
    midi_program_change
        //! Instrument/program change. data1 = program number.
    midi_pitch_bend
        //! Pitch bend. data1 = LSB, data2 = MSB (14-bit value, 8192 = center).
    midi_tempo
        //! Tempo change (meta event). tempo field = microseconds per quarter note.
    midi_end_of_track
        //! End of track marker.
    midi_other
        //! Unrecognized or unsupported MIDI event.
}

//! A single MIDI event with absolute tick position.
struct MidiEvent {
    tick    : int           // absolute tick position
        //! Absolute tick position within the track.
    kind    : MidiEventKind
        //! Event type.
    channel : int           // 0-15
        //! MIDI channel (0-15).
    data1   : int           // note number / controller number
        //! First data byte (note number, controller number, or program number).
    data2   : int           // velocity / controller value / program number
        //! Second data byte (velocity, controller value, or pitch bend MSB).
    tempo   : int           // microseconds per quarter note (for tempo events)
        //! Microseconds per quarter note (only valid for midi_tempo events).
    seq     : int           // original order within track (for stable sort tiebreaking)
        //! Sequence number for stable sort ordering.
}

//! A sequence of MIDI events forming one track.
struct MidiTrack {
    events : array<MidiEvent>
        //! Events in this track, in order of absolute tick position.
}

//! Parsed MIDI file containing header info and tracks.
struct MidiFile {
    format       : int      // 0, 1, or 2
        //! MIDI format (0 = single track, 1 = multi-track synchronous, 2 = multi-track asynchronous).
    ticks_per_qn : int      // ticks per quarter note (division)
        //! Ticks per quarter note (time resolution).
    tracks       : array<MidiTrack>
        //! Array of tracks.
}

// ─── Binary Readers ───

def read_byte(data : array<uint8>; var cursor : int&) : int {
    //! Read a single byte from data at cursor, advancing cursor. Returns -1 on EOF.
    if (cursor >= length(data)) {
        return -1
    }
    let b = int(data[cursor])
    cursor ++
    return b
}

def read_u16(data : array<uint8>; var cursor : int&) : int {
    //! Read a big-endian 16-bit unsigned integer. Returns -1 on EOF.
    let hi = read_byte(data, cursor)
    let lo = read_byte(data, cursor)
    if (hi < 0 || lo < 0) {
        return -1
    }
    return (hi << 8) | lo
}

def read_u32(data : array<uint8>; var cursor : int&) : int {
    //! Read a big-endian 32-bit unsigned integer. Returns -1 on EOF.
    let b0 = read_byte(data, cursor)
    let b1 = read_byte(data, cursor)
    let b2 = read_byte(data, cursor)
    let b3 = read_byte(data, cursor)
    if (b0 < 0 || b1 < 0 || b2 < 0 || b3 < 0) {
        return -1
    }
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3
}

// ─── Variable-Length Quantity ───

def read_vlq(data : array<uint8>; var cursor : int&) : int {
    //! Read a MIDI variable-length quantity (up to 4 bytes). Returns -1 on EOF.
    var result = 0
    for (_ in range(4)) {
        let b = read_byte(data, cursor)
        if (b < 0) {
            return -1
        }
        result = (result << 7) | (b & int(0x7F))
        if ((b & int(0x80)) == 0) {
            return result
        }
    }
    return result
}

// ─── Track Parser ───

def private parse_midi_track(data : array<uint8>; var cursor : int&) : MidiTrack {
    //! Parse a single MTrk chunk starting at cursor, returning the track with all events.
    var track : MidiTrack
    // expect "MTrk"
    if (cursor + 4 > length(data)) {
        return <- track
    }
    let m = int(data[cursor])
    let t = int(data[cursor + 1])
    let r = int(data[cursor + 2])
    let kk = int(data[cursor + 3])
    cursor += 4
    if (m != 'M' || t != 'T' || r != 'r' || kk != 'k') {
        return <- track
    }
    let trackLen = read_u32(data, cursor)
    if (trackLen < 0) {
        return <- track
    }
    let trackEnd = cursor + trackLen
    var absTick = 0
    var runningStatus = 0
    while (cursor < trackEnd && cursor < length(data)) {
        let delta = read_vlq(data, cursor)
        if (delta < 0) {
            break
        }
        absTick += delta
        let firstByte = read_byte(data, cursor)
        if (firstByte < 0) {
            break
        }
        var status = firstByte
        if (status < int(0x80)) {
            // running status — reuse previous status byte
            status = runningStatus
        }
        let msgType = status & int(0xF0)
        let ch = int(status & int(0x0F))
        if (msgType == int(0x80)) {
            // Note Off
            let note = (status == firstByte) ? read_byte(data, cursor) : firstByte
            let vel = read_byte(data, cursor)
            track.events |> push(MidiEvent(
                tick = absTick,
                kind = MidiEventKind.midi_note_off,
                channel = ch,
                data1 = note,
                data2 = vel
            ))
            runningStatus = status
        } elif (msgType == int(0x90)) {
            // Note On (velocity 0 = note off)
            let note = (status == firstByte) ? read_byte(data, cursor) : firstByte
            let vel = read_byte(data, cursor)
            if (vel == 0) {
                track.events |> push(MidiEvent(
                    tick = absTick,
                    kind = MidiEventKind.midi_note_off,
                    channel = ch,
                    data1 = note,
                    data2 = 0
                ))
            } else {
                track.events |> push(MidiEvent(
                    tick = absTick,
                    kind = MidiEventKind.midi_note_on,
                    channel = ch,
                    data1 = note,
                    data2 = vel
                ))
            }
            runningStatus = status
        } elif (msgType == int(0xA0)) {
            // Aftertouch — skip 2 data bytes
            if (status == firstByte) { cursor += 2; } else { cursor ++; }
            runningStatus = status
        } elif (msgType == int(0xB0)) {
            // Control Change
            let cc = (status == firstByte) ? read_byte(data, cursor) : firstByte
            let val = read_byte(data, cursor)
            track.events |> push(MidiEvent(
                tick = absTick,
                kind = MidiEventKind.midi_control_change,
                channel = ch,
                data1 = cc,
                data2 = val
            ))
            runningStatus = status
        } elif (msgType == int(0xC0)) {
            // Program Change — 1 data byte
            let prog = (status == firstByte) ? read_byte(data, cursor) : firstByte
            track.events |> push(MidiEvent(
                tick = absTick,
                kind = MidiEventKind.midi_program_change,
                channel = ch,
                data1 = prog
            ))
            runningStatus = status
        } elif (msgType == int(0xD0)) {
            // Channel Pressure — skip 1 data byte
            if (status == firstByte) { cursor ++; }
            runningStatus = status
        } elif (msgType == int(0xE0)) {
            // Pitch Bend — 2 data bytes: LSB, MSB → 14-bit value (0..16383, center 8192)
            let lsb = status == firstByte ? read_byte(data, cursor) : firstByte
            let msb = read_byte(data, cursor)
            let value14 = (msb << 7) | lsb
            track.events |> push(MidiEvent(
                tick = absTick,
                kind = MidiEventKind.midi_pitch_bend,
                channel = ch,
                data1 = value14
            ))
            runningStatus = status
        } elif (status == int(0xFF)) {
            // Meta event
            let metaType = (status == firstByte) ? read_byte(data, cursor) : firstByte
            let metaLen = read_vlq(data, cursor)
            if (metaType == int(0x51) && metaLen == 3) {
                // Tempo: 3 bytes = microseconds per quarter note
                let tb0 = read_byte(data, cursor)
                let tb1 = read_byte(data, cursor)
                let tb2 = read_byte(data, cursor)
                let tempoUs = (tb0 << 16) | (tb1 << 8) | tb2
                track.events |> push(MidiEvent(
                    tick = absTick,
                    kind = MidiEventKind.midi_tempo,
                    tempo = tempoUs
                ))
            } elif (metaType == int(0x2F)) {
                // End of Track
                track.events |> push(MidiEvent(
                    tick = absTick,
                    kind = MidiEventKind.midi_end_of_track
                ))
                break
            } else {
                // skip other meta events
                if (metaLen > 0) {
                    cursor += metaLen
                }
            }
            runningStatus = 0
        } elif (status == int(0xF0) || status == int(0xF7)) {
            // SysEx — read length, skip data
            let sysexLen = read_vlq(data, cursor)
            if (sysexLen > 0) {
                cursor += sysexLen
            }
            runningStatus = 0
        } else {
            runningStatus = 0
        }
    }
    cursor = min(trackEnd, length(data))
    return <- track
}

// ─── File Parser ───

def parse_midi(data : array<uint8>) : MidiFile {
    //! Parse MIDI data from a byte array into a MidiFile structure.
    var result : MidiFile
    if (length(data) < 14) {
        return <- result
    }
    var cursor = 0
    // "MThd"
    if (int(data[0]) != 'M' || int(data[1]) != 'T' || int(data[2]) != 'h' || int(data[3]) != 'd') {
        return <- result
    }
    cursor = 4
    let headerLen = read_u32(data, cursor)
    if (headerLen < 6) {
        return <- result
    }
    result.format = read_u16(data, cursor)
    let nTracks = read_u16(data, cursor)
    result.ticks_per_qn = read_u16(data, cursor)
    if (headerLen > 6) {
        cursor += headerLen - 6
    }
    for (_ in range(nTracks)) {
        if (cursor >= length(data)) {
            break
        }
        var trk <- parse_midi_track(data, cursor)
        result.tracks |> emplace <| trk
    }
    return <- result
}

// ─── File Loading ───

def load_midi(path : string) : MidiFile {
    //! Load and parse a MIDI file from disk.
    var data : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            let st = fstat(f)
            if (st.is_valid) {
                data |> resize(int(st.size))
                fread(f, data)
            }
        }
    }
    return <- parse_midi(data)
}

// ─── Track Merging ───

def merge_tracks(midi : MidiFile) : array<MidiEvent> {
    //! Merge all tracks into a single sorted event list for sequential playback.
    var merged : array<MidiEvent>
    var total = 0
    for (track in midi.tracks) {
        total += length(track.events)
    }
    merged |> reserve(total)
    // assign global sequence numbers for stable sort
    var seqNum = 0
    for (track in midi.tracks) {
        for (ev in track.events) {
            var e = ev
            e.seq = seqNum
            seqNum ++
            merged |> push(e)
        }
    }
    sort(merged) $(a, b) {
        if (a.tick != b.tick) {
            return a.tick < b.tick
        }
        return a.seq < b.seq
    }
    return <- merged
}
