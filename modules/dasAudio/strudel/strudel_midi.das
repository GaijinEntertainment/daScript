options gen2
options indenting = 4

module strudel_midi shared public

require daslib/fio
require daslib/algorithm
require math

// ─── Data Structures ───

enum MidiEventKind {
    midi_note_off
    midi_note_on
    midi_control_change
    midi_program_change
    midi_pitch_bend
    midi_tempo
    midi_end_of_track
    midi_other
}

struct MidiEvent {
    tick    : int           // absolute tick position
    kind    : MidiEventKind
    channel : int           // 0-15
    data1   : int           // note number / controller number
    data2   : int           // velocity / controller value / program number
    tempo   : int           // microseconds per quarter note (for tempo events)
    seq     : int           // original order within track (for stable sort tiebreaking)
}

struct MidiTrack {
    events : array<MidiEvent>
}

struct MidiFile {
    format       : int      // 0, 1, or 2
    ticks_per_qn : int      // ticks per quarter note (division)
    tracks       : array<MidiTrack>
}

// ─── Binary Readers ───

def read_byte(data : array<uint8>; var cursor : int&) : int {
    if (cursor >= length(data)) {
        return -1
    }
    let b = int(data[cursor])
    cursor ++
    return b
}

def read_u16(data : array<uint8>; var cursor : int&) : int {
    let hi = read_byte(data, cursor)
    let lo = read_byte(data, cursor)
    if (hi < 0 || lo < 0) {
        return -1
    }
    return (hi << 8) | lo
}

def read_u32(data : array<uint8>; var cursor : int&) : int {
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

def parse_midi_track(data : array<uint8>; var cursor : int&) : MidiTrack {
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
