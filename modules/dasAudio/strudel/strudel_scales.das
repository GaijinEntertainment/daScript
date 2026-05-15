options gen2
options indenting = 4

module strudel_scales shared public
//! Music-theory helpers: scale interval tables (major, minor, modes, pentatonic, blues, ...)
//! and the `set_scale` combinator that reinterprets `event.note` as a scale degree.
//! Scale definitions use the `Root:Type` string syntax, e.g. `"C4:major"` or `"D:pentatonic"`.

require strudel/strudel_event
require strudel/strudel_time
require strudel/strudel_pattern
require strudel/strudel_mini
require math
require strings

// ─── Scale Interval Tables ───
// Each scale is defined as an array of semitone offsets from the root.
// Standard music theory interval definitions.

def private get_scale_intervals(scale_type : string) : array<int> {
    if (scale_type == "major" || scale_type == "ionian") {
        return <- [0, 2, 4, 5, 7, 9, 11]
    } elif (scale_type == "minor" || scale_type == "aeolian") {
        return <- [0, 2, 3, 5, 7, 8, 10]
    } elif (scale_type == "dorian") {
        return <- [0, 2, 3, 5, 7, 9, 10]
    } elif (scale_type == "phrygian") {
        return <- [0, 1, 3, 5, 7, 8, 10]
    } elif (scale_type == "lydian") {
        return <- [0, 2, 4, 6, 7, 9, 11]
    } elif (scale_type == "mixolydian") {
        return <- [0, 2, 4, 5, 7, 9, 10]
    } elif (scale_type == "locrian") {
        return <- [0, 1, 3, 5, 6, 8, 10]
    } elif (scale_type == "pentatonic" || scale_type == "minor_pentatonic") {
        return <- [0, 3, 5, 7, 10]
    } elif (scale_type == "major_pentatonic") {
        return <- [0, 2, 4, 7, 9]
    } elif (scale_type == "blues") {
        return <- [0, 3, 5, 6, 7, 10]
    } elif (scale_type == "chromatic") {
        return <- [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
    } elif (scale_type == "whole_tone" || scale_type == "wholetone") {
        return <- [0, 2, 4, 6, 8, 10]
    } elif (scale_type == "harmonic_minor") {
        return <- [0, 2, 3, 5, 7, 8, 11]
    } elif (scale_type == "melodic_minor") return <- [0, 2, 3, 5, 7, 9, 11]
    // default to chromatic if unknown
    return <- [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
}

// Parse root note from scale string prefix.
// Supports: "C", "C3", "Eb", "Eb4", "Fs", "Fs3", etc.
// Returns MIDI note number. Default octave is 3.
def private parse_scale_root(root_str : string) : int {
    let n = length(root_str)
    if (n == 0) return 48  // default C3
    var semitone = -1
    peek_data(root_str) $(bytes) {
        let ch = int(bytes[0])
        if (ch == 'C' || ch == 'c') { semitone = 0; }
        elif (ch == 'D' || ch == 'd') { semitone = 2; }
        elif (ch == 'E' || ch == 'e') { semitone = 4; }
        elif (ch == 'F' || ch == 'f') { semitone = 5; }
        elif (ch == 'G' || ch == 'g') { semitone = 7; }
        elif (ch == 'A' || ch == 'a') { semitone = 9; }
        elif (ch == 'B' || ch == 'b') { semitone = 11; }
    }
    if (semitone < 0) return 48  // fallback C3
    var octave = 3  // default octave
    var idx = 1
    if (n > 1) {
        peek_data(root_str) $(bytes) {
            let mod = int(bytes[1])
            if (mod == 's' || mod == '#') {
                semitone ++
                idx = 2
            } elif (mod == 'b') {
                semitone --
                idx = 2
            }
            if (idx < n) {
                let octCh = int(bytes[idx])
                if (is_number(octCh)) {
                    octave = octCh - int('0')
                }
            }
        }
    }
    return (octave + 1) * 12 + semitone
}

// Normalize scale type: replace ":" with "_" so "minor:pentatonic" matches "minor_pentatonic".
// This lets users write "C4:minor:pentatonic" matching mini-notation syntax.
def private normalize_scale_type(raw : string) : string {
    var result = raw
    while (true) {
        let pos = find(result, ":")
        if (pos < 0) break
        result = slice(result, 0, pos) + "_" + slice(result, pos + 1)
    }
    return result
}

// Parse a scale definition string "Root:Type" or "Root:Mod:Type" → (root_midi, scale_type).
// Examples: "C4:major" → (60, "major"), "D:pentatonic" → (50, "pentatonic") using default oct=3.
// If no colon, treats entire string as type with C3 root.
def private parse_scale_def(scale_def : string) : tuple<int; string> {
    let colon_pos = find(scale_def, ":")
    if (colon_pos < 0) return tuple(48, scale_def)
    let root_str = slice(scale_def, 0, colon_pos)
    let type_str = normalize_scale_type(slice(scale_def, colon_pos + 1))
    return tuple(parse_scale_root(root_str), type_str)
}

// Convert a scale degree to a MIDI note number.
// degree 0 = root, degree 1 = second, etc.
// Wraps at octave boundaries: degree 7 in major = root + 12.
// Handles negative degrees (wraps backwards).
def degree_to_note(degree : int; root_midi : int; intervals : array<int> const) : float {
    //! Convert a scale degree to a MIDI note number. Degree 0 = root, 1 = second, etc.;
    //! wraps at octave boundaries so degree 7 in a major scale = root + 12. Negative
    //! degrees wrap backwards: degree -1 in a 7-note scale is the 7th one octave down.
    let n = length(intervals)
    if (n == 0) return float(root_midi)
    var oct : int
    var step : int
    if (degree >= 0) {
        oct = degree / n
        step = degree % n
    } else {
        // negative wrap: degree -1 in 7-note scale → octave -1, step 6
        oct = (degree + 1) / n - 1
        step = ((degree % n) + n) % n
    }
    return float(root_midi + oct * 12 + intervals[step])
}

// ─── Scale Setter ───
// Converts event.note (treated as a scale degree) into a MIDI note number.
// pat |> set_scale("C4:major") — degree 0→60, 1→62, 2→64, 3→65, 4→67...

def set_scale(var pat : Pattern; scale_def : string) : Pattern {
    //! Treat each event's `note` field as a scale degree and map it to a MIDI note
    //! using `scale_def` (format `"Root:Type"`, e.g. `"C4:major"`, `"D:pentatonic"`).
    //! Example: `n("0 2 4 7") |> set_scale("C4:major")` plays C, E, G, C (one octave up).
    let parsed = parse_scale_def(scale_def)
    let root_midi = parsed._0
    let scale_type = parsed._1
    var inscope intervals <- get_scale_intervals(scale_type)
    let fn <- @capture(= root_midi, <- intervals) (e : Event) : Event {
        var r = e
        r.note = degree_to_note(int(r.note), root_midi, intervals)
        return r
    }
    return <- fmap(pat, fn)
}

// ─── Convenience Constructors ───

def scale_pattern(notation : string; scale_def : string; sound : string = "sine") : Pattern {
    //! Shorthand for `n(notation) |> set_scale(scale_def) |> set_sound(sound)`.
    //! Example: `scale_pattern("0 2 4 7", "C4:pentatonic")` plays a pentatonic arpeggio.
    return <- n(notation) |> set_scale(scale_def) |> set_sound(sound)
}

// ─── Query Helpers (for tests) ───

def get_scale_intervals_by_name(scale_type : string) : array<int> {
    //! Return the semitone-interval table for a named scale (`"major"`, `"minor"`,
    //! `"dorian"`, `"pentatonic"`, `"blues"`, `"chromatic"`, ...). Used by tests.
    return <- get_scale_intervals(scale_type)
}

def parse_root(root_str : string) : int {
    //! Parse a root-note string (`"C"`, `"Eb4"`, `"Fs3"`, ...) into a MIDI note number.
    //! Default octave is 3. Exposed for tests.
    return parse_scale_root(root_str)
}

// ─── Alias ───

def scale(var pat : Pattern; scale_def : string) : Pattern {
    //! Alias for `set_scale`. Example: `n("0 2 4") |> scale("C4:major")`.
    return <- set_scale(pat, scale_def);
}
