options gen2
options indenting = 4

module strudel_pattern shared

require strudel/strudel_event
require strudel/strudel_time
require math

// A Pattern is a function from time span to events.
// Patterns are pure — no side effects, no audio, just data.
typedef Pattern = lambda<(span : TimeSpan) : array<Hap>>
typedef PatternTransform = lambda<(var pat : Pattern) : Pattern>

// ─── Constructors ───

// A single event spanning every cycle.
def pure(value : Event) : Pattern {
    return @capture(= value) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cyc = sameCycle(sub.start)
            let whole = TimeSpan(start = cyc, stop = cyc + 1.0lf)
            result |> push(Hap(whole = whole, part = sub, value = value)) // nolint:PERF006
        }
        return <- result
    }
}

// No events, ever.
def silence() : Pattern {
    return @(span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        return <- result
    }
}

// Shorthand: make a pattern from a sound name.
def atom(name : string) : Pattern {
    return pure(Event(s = name))
}

// ─── Time Manipulation ───

// Speed up by factor n. The pattern repeats n times per cycle.
def fast(var pat : Pattern; n : double) : Pattern {
    return @capture(= n, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        let innerSpan = TimeSpan(start = span.start * n, stop = span.stop * n)
        var haps <- pat(innerSpan)
        result |> reserve(length(haps))
        for (h in haps) {
            result |> push(Hap(
                whole = TimeSpan(start = h.whole.start / n, stop = h.whole.stop / n),
                part = TimeSpan(start = h.part.start / n, stop = h.part.stop / n),
                has_whole = h.has_whole,
                value = h.value
            ))
        }
        delete haps
        return <- result
    }
}

// Slow down by factor n.
def slow(var pat : Pattern; n : double) : Pattern {
    return fast(pat, 1.0lf / n)
}

// Shift pattern earlier in time by `offset` cycles.
def early(var pat : Pattern; offset : double) : Pattern {
    return @capture(= offset, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        let innerSpan = TimeSpan(start = span.start + offset, stop = span.stop + offset)
        var haps <- pat(innerSpan)
        result |> reserve(length(haps))
        for (h in haps) {
            result |> push(Hap(
                whole = TimeSpan(start = h.whole.start - offset, stop = h.whole.stop - offset),
                part = TimeSpan(start = h.part.start - offset, stop = h.part.stop - offset),
                has_whole = h.has_whole,
                value = h.value
            ))
        }
        delete haps
        return <- result
    }
}

// Shift pattern later in time.
def late(var pat : Pattern; offset : double) : Pattern {
    return early(pat, 0.0lf - offset)
}

// ─── Composition ───

// Layer patterns on top of each other (simultaneous).
def stack(var pats : array<Pattern>) : Pattern {
    return @capture(<- pats) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        for (pat in pats) {
            var haps <- pat(span)
            for (h in haps) {
                result |> push(h) // nolint:PERF006
            }
            delete haps
        }
        return <- result
    }
}

// Sequence patterns across cycles. With N patterns, pattern i plays
// during cycle i (mod N). Each pattern is time-stretched to fill one cycle.
def cat(var pats : array<Pattern>) : Pattern {
    let cn = length(pats)
    return @capture(= cn, <- pats) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = sameCycle(sub.start)
            var idx = int(cycle) % cn
            if (idx < 0) {
                idx += cn
            }
            let innerStart = cyclePos(sub.start)
            let innerStop = innerStart + (sub.stop - sub.start)
            let innerSpan = TimeSpan(start = innerStart, stop = innerStop)
            var haps <- invoke(pats[idx], innerSpan)
            for (h in haps) {
                result |> push(Hap( // nolint:PERF006
                    whole = TimeSpan(start = h.whole.start + cycle, stop = h.whole.stop + cycle),
                    part = TimeSpan(start = h.part.start + cycle, stop = h.part.stop + cycle),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
            delete haps
        }
        return <- result
    }
}

// Like cat but all patterns squeezed into one cycle.
def fastcat(var pats : array<Pattern>) : Pattern {
    let cn = double(length(pats))
    return fast(cat(pats), cn)
}

// Like fastcat but each element gets time proportional to its weight.
// weighted_fastcat([a, b], [3.0, 1.0]) → a gets 3/4 of cycle, b gets 1/4.
def weighted_fastcat(var pats : array<Pattern>; var weights : array<float>) : Pattern {
    var total_weight = 0.0
    for (w in weights) {
        total_weight += w
    }
    let tw = total_weight
    return @capture(<- pats, <- weights, = tw) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycleStart = floor(sub.start)
            let relStart = sub.start - cycleStart
            let relStop = sub.stop - cycleStart
            var offset = 0.0lf
            for (i in range(length(pats))) {
                let w = double(weights[i]) / double(tw)
                let elemStart = offset
                let elemStop = offset + w
                if (elemStop > relStart && elemStart < relStop) {
                    let qStart = min((max(relStart, elemStart) - elemStart) / w, 1.0lf)
                    let qStop = min((min(relStop, elemStop) - elemStart) / w, 1.0lf)
                    var haps <- invoke(pats[i], TimeSpan(start = qStart, stop = qStop))
                    for (h in haps) {
                        result |> push(Hap( // nolint:PERF006
                            whole = TimeSpan(start = h.whole.start * w + elemStart + cycleStart, stop = h.whole.stop * w + elemStart + cycleStart),
                            part = TimeSpan(start = h.part.start * w + elemStart + cycleStart, stop = h.part.stop * w + elemStart + cycleStart),
                            has_whole = h.has_whole,
                            value = h.value
                        ))
                    }
                    delete haps
                }
                offset += w
            }
        }
        return <- result
    }
}

// ─── Mapping ───

// Transform the value of every event.
def fmap(var pat : Pattern; var fn : lambda<(e : Event) : Event>) : Pattern {
    return @capture(<- fn, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        result |> reserve(length(haps))
        for (h in haps) {
            result |> push(Hap(
                whole = h.whole,
                part = h.part,
                has_whole = h.has_whole,
                value = fn(h.value)
            ))
        }
        delete haps
        return <- result
    }
}

// ─── Conditional ───

// Randomly remove events with given probability.
// Uses deterministic hash of cycle position — same cycle = same result.
def degrade(var pat : Pattern; prob : float) : Pattern {
    return @capture(= prob, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        for (h in haps) {
            let seed = uint(h.whole.start * 10000.0lf)
            let hash = (seed * 2654435761u) >> 16u
            let r = float(hash % 1000u) / 1000.0
            if (r >= prob) {
                result |> push(h)
            }
        }
        delete haps
        return <- result
    }
}

// Apply a different pattern every nth cycle.
// every(4, fast_version, normal_version) — use fast_version on cycles 0,4,8,...
def every(n : int; var pat_on : Pattern; var pat_off : Pattern) : Pattern {
    var cn = n
    return @capture(<- cn, <- pat_on, <- pat_off) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            var idx = cycle % cn
            if (idx < 0) {
                idx += cn
            }
            if (idx == 0) {
                var inscope patOn = pat_on(sub)
                for (h in patOn) {
                    result |> push(h) // nolint:PERF006
                }
            } else {
                var inscope patOff = pat_off(sub)
                for (h in patOff) {
                    result |> push(h) // nolint:PERF006
                }
            }
        }
        return <- result
    }
}

// ─── Euclidean Rhythms ───

// Bjorklund's algorithm: distribute k onsets across n slots.
// bjorklund(3, 8) → [x . . x . . x .]
def bjorklund(k, n : int) : array<bool> {
    var pattern : array<bool>
    pattern |> resize(n)
    if (k <= 0) {
        return <- pattern
    }
    if (k >= n) {
        for (i in range(n)) {
            pattern[i] = true
        }
        return <- pattern
    }
    var prev = -1
    for (i in range(n)) {
        let curr = (i * k) / n
        if (curr != prev) {
            pattern[i] = true
            prev = curr
        }
    }
    return <- pattern
}

// ─── Parameter Helpers ───
// Each sets a single Event field via fmap.

// pat |> set_gain(0.5) — set amplitude
def set_gain(var pat : Pattern; g : float) : Pattern {
    let fn <- @capture(= g) (e : Event) : Event {
        var r = e; r.gain = g; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_velocity(0.5) — set expression/velocity
def set_velocity(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.velocity = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_pan(0.0) — set stereo position (0=left, 0.5=center, 1=right)
def set_pan(var pat : Pattern; p : float) : Pattern {
    let fn <- @capture(= p) (e : Event) : Event {
        var r = e; r.pan = p; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_speed(2.0) — set sample playback speed
def set_speed(var pat : Pattern; spd : float) : Pattern {
    let fn <- @capture(= spd) (e : Event) : Event {
        var r = e; r.speed = spd; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_note(48.0) — set MIDI note number
def set_note(var pat : Pattern; n : float) : Pattern {
    let fn <- @capture(= n) (e : Event) : Event {
        var r = e; r.note = n; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sound("sawtooth") — change the sound name
def set_sound(var pat : Pattern; snd : string) : Pattern {
    let fn <- @capture(= snd) (e : Event) : Event {
        var r = e; r.s = snd; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sound(sound_pat) — sample sound name from a pattern at each event onset.
// Enables strudel-style: note("c3").s("<supersaw sawtooth>")
def set_sound(var pat : Pattern; var sound_pat : Pattern) : Pattern {
    return combineWithStr(pat, sound_pat, @(e : Event; val : string) : Event {
        var r = e; r.s = val; return r
    })
}

// pat |> set_cut(1) — set cut group (new voice kills previous in same group)
def set_cut(var pat : Pattern; c : int) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.cut = c; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_room(0.5) — set reverb send amount
def set_room(var pat : Pattern; r : float) : Pattern {
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.room = r; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_roomsize(0.8) — set reverb room size
def set_roomsize(var pat : Pattern; r : float) : Pattern {
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.roomsize = r; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delay(0.5) — set delay send amount
def set_delay(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delay_amount = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delaytime(0.25) — set delay time in seconds
def set_delaytime(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delaytime = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delayfeedback(0.7) — set delay feedback
def set_delayfeedback(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delayfeedback = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_orbit(1) — set effect bus routing
def set_orbit(var pat : Pattern; o : int) : Pattern {
    let fn <- @capture(= o) (e : Event) : Event {
        var ev = e; ev.orbit = o; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_fm(2.0) — set FM modulation depth
def set_fm(var pat : Pattern; f : float) : Pattern {
    let fn <- @capture(= f) (e : Event) : Event {
        var ev = e; ev.fm = f; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_fmh(1.5) — set FM harmonicity ratio
def set_fmh(var pat : Pattern; f : float) : Pattern {
    let fn <- @capture(= f) (e : Event) : Event {
        var ev = e; ev.fmh = f; return ev
    }
    return <- fmap(pat, fn)
}

// ─── SF2 SoundFont Helpers ───

// GM instrument name → program number mapping.
def gm_program_by_name(name : string) : int {
    // Piano family
    if (name == "piano" || name == "acoustic_grand_piano") { return 0; }
    if (name == "bright_piano") { return 1; }
    if (name == "epiano" || name == "electric_piano") { return 4; }
    if (name == "honkytonk") { return 3; }
    if (name == "rhodes") { return 4; }
    if (name == "chorus_piano") { return 5; }
    if (name == "harpsichord") { return 6; }
    if (name == "clavinet") { return 7; }
    // Chromatic Percussion
    if (name == "celesta") { return 8; }
    if (name == "glockenspiel") { return 9; }
    if (name == "music_box") { return 10; }
    if (name == "vibraphone") { return 11; }
    if (name == "marimba") { return 12; }
    if (name == "xylophone") { return 13; }
    if (name == "tubular_bells") { return 14; }
    // Organ
    if (name == "organ") { return 19; }
    if (name == "church_organ") { return 19; }
    if (name == "reed_organ") { return 20; }
    if (name == "accordion") { return 21; }
    if (name == "harmonica") { return 22; }
    // Guitar
    if (name == "guitar" || name == "nylon_guitar") { return 24; }
    if (name == "steel_guitar") { return 25; }
    if (name == "electric_guitar" || name == "clean_guitar") { return 27; }
    if (name == "overdriven_guitar") { return 29; }
    if (name == "distortion_guitar") { return 30; }
    // Bass
    if (name == "bass" || name == "acoustic_bass") { return 32; }
    if (name == "electric_bass" || name == "finger_bass") { return 33; }
    if (name == "pick_bass") { return 34; }
    if (name == "fretless_bass") { return 35; }
    if (name == "slap_bass") { return 36; }
    if (name == "synth_bass") { return 38; }
    // Strings
    if (name == "violin") { return 40; }
    if (name == "viola") { return 41; }
    if (name == "cello") { return 42; }
    if (name == "contrabass") { return 43; }
    if (name == "strings" || name == "tremolo_strings") { return 44; }
    if (name == "pizzicato") { return 45; }
    if (name == "harp") { return 46; }
    if (name == "timpani") { return 47; }
    // Ensemble
    if (name == "string_ensemble") { return 48; }
    if (name == "synth_strings") { return 50; }
    if (name == "choir") { return 52; }
    if (name == "voice_oohs") { return 53; }
    if (name == "orchestra_hit") { return 55; }
    // Brass
    if (name == "trumpet") { return 56; }
    if (name == "trombone") { return 57; }
    if (name == "tuba") { return 58; }
    if (name == "muted_trumpet") { return 59; }
    if (name == "french_horn") { return 60; }
    if (name == "brass" || name == "brass_section") { return 61; }
    // Reed
    if (name == "soprano_sax") { return 64; }
    if (name == "alto_sax") { return 65; }
    if (name == "tenor_sax") { return 66; }
    if (name == "baritone_sax") { return 67; }
    if (name == "oboe") { return 68; }
    if (name == "english_horn") { return 69; }
    if (name == "bassoon") { return 70; }
    if (name == "clarinet") { return 71; }
    // Pipe
    if (name == "piccolo") { return 72; }
    if (name == "flute") { return 73; }
    if (name == "recorder") { return 74; }
    if (name == "pan_flute") { return 75; }
    if (name == "shakuhachi") { return 77; }
    if (name == "whistle") { return 78; }
    if (name == "ocarina") { return 79; }
    // Synth Lead
    if (name == "synth_lead" || name == "square_lead") { return 80; }
    if (name == "saw_lead") { return 81; }
    // Synth Pad
    if (name == "synth_pad" || name == "new_age") { return 88; }
    if (name == "warm_pad") { return 89; }
    // Ethnic
    if (name == "sitar") { return 104; }
    if (name == "banjo") { return 105; }
    if (name == "shamisen") { return 106; }
    if (name == "koto") { return 107; }
    if (name == "kalimba") { return 108; }
    if (name == "bagpipe") { return 109; }
    if (name == "fiddle") { return 110; }
    if (name == "shanai") { return 111; }
    return -1
}

// pat |> set_sf2(0) — set SF2 program number (GM instrument)
def set_sf2(var pat : Pattern; program : int) : Pattern {
    let fn <- @capture(= program) (e : Event) : Event {
        var r = e; r.sf2_program = program; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2("piano") — set SF2 instrument by GM name
def set_sf2(var pat : Pattern; name : string) : Pattern {
    let program = gm_program_by_name(name)
    let fn <- @capture(= program) (e : Event) : Event {
        var r = e; r.sf2_program = program; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_bank(128) — set SF2 bank (0 = melodic, 128 = percussion)
def set_sf2_bank(var pat : Pattern; bank : int) : Pattern {
    let fn <- @capture(= bank) (e : Event) : Event {
        var r = e; r.sf2_bank = bank; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_expression(0.5) — set SF2 expression CC11 (0.0–1.0)
def set_sf2_expression(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.sf2_expression = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_mod_wheel(0.5) — set SF2 mod wheel CC1 (0.0–1.0, vibrato depth)
def set_sf2_mod_wheel(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.sf2_mod_wheel = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_pitch_bend(0.6) — set SF2 pitch bend (0.0–1.0, 0.5 = center)
def set_sf2_pitch_bend(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.sf2_pitch_bend = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_chorus(0.5) — set chorus send amount (0.0–1.0)
def set_chorus(var pat : Pattern; c : float) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.chorus = c; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_lpq(5.0) — set low-pass filter resonance
def set_lpq(var pat : Pattern; q : float) : Pattern {
    let fn <- @capture(= q) (e : Event) : Event {
        var r = e; r.lpq = q; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hpq(5.0) — set high-pass filter resonance
def set_hpq(var pat : Pattern; q : float) : Pattern {
    let fn <- @capture(= q) (e : Event) : Event {
        var r = e; r.hpq = q; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_lpf(1000.0) — set low-pass filter cutoff
def set_lpf(var pat : Pattern; freq : float) : Pattern {
    let fn <- @capture(= freq) (e : Event) : Event {
        var r = e; r.lpf = freq; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hpf(200.0) — set high-pass filter cutoff
def set_hpf(var pat : Pattern; freq : float) : Pattern {
    let fn <- @capture(= freq) (e : Event) : Event {
        var r = e; r.hpf = freq; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_attack(0.1) — set envelope attack time
def set_attack(var pat : Pattern; a : float) : Pattern {
    let fn <- @capture(= a) (e : Event) : Event {
        var r = e; r.attack = a; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_decay(0.3) — set envelope decay time
def set_decay(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var r = e; r.decay = d; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sustain(0.7) — set envelope sustain level
def set_sustain(var pat : Pattern; s : float) : Pattern {
    let fn <- @capture(= s) (e : Event) : Event {
        var r = e; r.sustain = s; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_release(0.5) — set envelope release time
def set_release(var pat : Pattern; r : float) : Pattern {
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.release = r; return ev
    }
    return <- fmap(pat, fn)
}

// ─── Phase 0.5: Combinators ───

// Reverse event positions within each cycle.
// Mirrors the query span within each cycle, queries the inner pattern at the
// mirrored position, then mirrors the results back.
def rev(var pat : Pattern) : Pattern {
    return @capture(<- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        // mirror the query span per-cycle, then flip results back
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = floor(sub.start)
            let relStart = sub.start - cycle
            let relStop = sub.stop - cycle
            // mirror: [0.0, 0.25) → [0.75, 1.0)
            let mirStart = 1.0lf - relStop
            let mirStop = 1.0lf - relStart
            let mirSpan = TimeSpan(start = mirStart + cycle, stop = mirStop + cycle)
            var haps <- pat(mirSpan)
            for (h in haps) {
                let hCycle = floor(h.whole.start)
                let wStart = h.whole.start - hCycle
                let wStop = h.whole.stop - hCycle
                let pStart = h.part.start - hCycle
                let pStop = h.part.stop - hCycle
                result |> push(Hap( // nolint:PERF006
                    whole = TimeSpan(start = 1.0lf - wStop + hCycle, stop = 1.0lf - wStart + hCycle),
                    part = TimeSpan(start = 1.0lf - pStop + hCycle, stop = 1.0lf - pStart + hCycle),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
            delete haps
        }
        return <- result
    }
}

// Stereo split: original panned left, transformed panned right.
// jux(s("bd sd"), @(x) => rev(x)) — classic stereo widening
def jux(var pat : Pattern; transform : PatternTransform) : Pattern {
    var transformed <- invoke(transform, pat) // nolint:LINT003
    var left <- pat |> set_pan(0.0)
    var right <- transformed |> set_pan(1.0)
    var layers : array<Pattern>
    layers |> emplace <| left
    layers |> emplace <| right
    return <- stack(layers)
}

// Overlay pattern with a time-shifted copy.
// off(s("bd sd"), 0.125, @(x) => transpose(x, 7.0)) — canon at the fifth
def off(var pat : Pattern; time : double; transform : PatternTransform) : Pattern {
    var shifted <- invoke(transform, pat) |> late(time)
    var layers : array<Pattern>
    layers |> emplace <| pat
    layers |> emplace <| shifted
    return <- stack(layers)
}

// Keep events where hash >= prob (original survivors).
def degrade_keep(var pat : Pattern; prob : float) : Pattern {
    let cn = prob
    return @capture(= cn, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope haps = pat(span)
        for (h in haps) {
            if (h.has_whole) {
                let seed = uint(h.whole.start * 10000.0lf)
                let hash = (seed * 2654435761u) >> 16u
                let r = float(hash % 1000u) / 1000.0
                if (r >= cn) {
                    result |> push(h) // nolint:PERF006
                }
            }
        }
        return <- result
    }
}

// Keep events where hash < prob (complement — selected for transform).
def degrade_drop(var pat : Pattern; prob : float) : Pattern {
    let cn = prob
    return @capture(= cn, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope haps = pat(span)
        for (h in haps) {
            if (h.has_whole) {
                let seed = uint(h.whole.start * 10000.0lf)
                let hash = (seed * 2654435761u) >> 16u
                let r = float(hash % 1000u) / 1000.0
                if (r < cn) {
                    result |> push(h) // nolint:PERF006
                }
            }
        }
        return <- result
    }
}

// Apply transform to random events with given probability.
// Matches strudel: stack(degrade(pat, prob), transform(undegrade(pat, prob)))
// Randomness is decided at original event positions, BEFORE transform reshuffles timing.
def sometimesby(var pat : Pattern; prob : float; transform : PatternTransform) : Pattern {
    var degraded <- degrade_keep(pat, prob)
    var undegraded <- degrade_drop(pat, prob) // nolint:LINT003
    var transformed <- invoke(transform, undegraded) // nolint:LINT003
    var layers : array<Pattern>
    layers |> emplace <| degraded
    layers |> emplace <| transformed
    return <- stack(layers)
}

// sometimes = 50% probability
def sometimes(var pat : Pattern; transform : PatternTransform) : Pattern {
    return <- sometimesby(pat, 0.5, transform)
}

// often = 75% probability
def often(var pat : Pattern; transform : PatternTransform) : Pattern {
    return <- sometimesby(pat, 0.75, transform)
}

// rarely = 25% probability
def rarely(var pat : Pattern; transform : PatternTransform) : Pattern {
    return <- sometimesby(pat, 0.25, transform)
}

// Stack multiple patterns on top of the base pattern.
// layer(s("bd"), [s("bd") |> transpose(7.0), s("bd") |> transpose(12.0)])
def layer(var pats : array<Pattern>; var base : Pattern) : Pattern {
    pats |> emplace <| base
    return <- stack(pats)
}

// Shift note by semitones. transpose(pat, 7) = up a fifth.
def transpose(var pat : Pattern; semitones : float) : Pattern {
    let fn <- @capture(= semitones) (e : Event) : Event {
        var r = e; r.note += semitones; return r
    }
    return <- fmap(pat, fn)
}

// Alias: add is the same as transpose (matches strudel naming).
def add(var pat : Pattern; semitones : float) : Pattern {
    return <- transpose(pat, semitones)
}

// ─── innerJoin (patternified parameters) ───

// Core mechanism for patternified parameters: for each hap in param_pat,
// create a pattern from its note value via fn, query that inner pattern,
// and collect results whose parts overlap. This is how slow("<2 16>") works.
def innerJoin(var param_pat : Pattern; var fn : lambda<(val : float) : Pattern>) : Pattern {
    return @capture(<- param_pat, <- fn) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var outer_haps <- param_pat(span)
        for (oh in outer_haps) {
            if (!oh.has_whole) {
                continue
            }
            var inner_pat <- fn(oh.value.note) // nolint:LINT003
            var inner_haps <- inner_pat(span)
            for (ih in inner_haps) {
                let pStart = max(ih.part.start, oh.part.start)
                let pStop = min(ih.part.stop, oh.part.stop)
                if (pStart < pStop) {
                    result |> push(Hap(
                        whole = ih.whole,
                        part = TimeSpan(start = pStart, stop = pStop),
                        has_whole = ih.has_whole,
                        value = ih.value
                    ))
                }
            }
            delete inner_haps
        }
        delete outer_haps
        return <- result
    }
}

// Patternified slow: factor varies per cycle based on factor_pat's note values.
// slow(pat, n("<2 16>")) — slow by 2 on even cycles, 16 on odd cycles
def slow(var pat : Pattern; var factor_pat : Pattern) : Pattern {
    return @capture(<- pat, <- factor_pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var outer_haps <- factor_pat(span)
        for (oh in outer_haps) {
            if (!oh.has_whole) {
                continue
            }
            let factor = double(oh.value.note)
            if (factor == 0.0lf) {
                continue
            }
            let n = 1.0lf / factor
            let innerSpan = TimeSpan(start = span.start * n, stop = span.stop * n)
            var haps <- pat(innerSpan)
            for (h in haps) {
                let p = TimeSpan(start = h.part.start / n, stop = h.part.stop / n)
                let pStart = max(p.start, oh.part.start)
                let pStop = min(p.stop, oh.part.stop)
                if (pStart < pStop) {
                    result |> push(Hap(
                        whole = TimeSpan(start = h.whole.start / n, stop = h.whole.stop / n),
                        part = TimeSpan(start = pStart, stop = pStop),
                        has_whole = h.has_whole,
                        value = h.value
                    ))
                }
            }
            delete haps
        }
        delete outer_haps
        return <- result
    }
}

// Patternified fast: factor varies per cycle based on factor_pat's note values.
def fast(var pat : Pattern; var factor_pat : Pattern) : Pattern {
    return @capture(<- pat, <- factor_pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var outer_haps <- factor_pat(span)
        for (oh in outer_haps) {
            if (!oh.has_whole) {
                continue
            }
            let n = double(oh.value.note)
            if (n == 0.0lf) {
                continue
            }
            let innerSpan = TimeSpan(start = span.start * n, stop = span.stop * n)
            var haps <- pat(innerSpan)
            for (h in haps) {
                let p = TimeSpan(start = h.part.start / n, stop = h.part.stop / n)
                let pStart = max(p.start, oh.part.start)
                let pStop = min(p.stop, oh.part.stop)
                if (pStart < pStop) {
                    result |> push(Hap(
                        whole = TimeSpan(start = h.whole.start / n, stop = h.whole.stop / n),
                        part = TimeSpan(start = pStart, stop = pStop),
                        has_whole = h.has_whole,
                        value = h.value
                    ))
                }
            }
            delete haps
        }
        delete outer_haps
        return <- result
    }
}

// ─── Signal Patterns ───
// Signals are continuous patterns that produce one Hap per query span
// with has_whole=false (no discrete onset). Value stored in value.note (0..1).

// Base signal constructor: wraps a function from cycle-time to value.
def signal(var fn : lambda<(t : double) : float>) : Pattern {
    return @capture(<- fn) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        result |> push(Hap(
            whole = span,
            part = span,
            has_whole = false,
            value = Event(note = fn(span.start))
        ))
        return <- result
    }
}

// Sine wave mapped to 0..1. At t=0 → 0.5, t=0.25 → 1.0, t=0.5 → 0.5, t=0.75 → 0.0.
def signal_sine() : Pattern {
    return signal(@(t : double) : float {
        return float(sin(3.14159265358979323846lf * 2.0lf * t) + 1.0lf) * 0.5
    })
}

// Cosine wave mapped to 0..1. Same as sine shifted left by 0.25 cycles.
def signal_cosine() : Pattern {
    return signal(@(t : double) : float {
        return float(cos(3.14159265358979323846lf * 2.0lf * t) + 1.0lf) * 0.5
    })
}

// Sawtooth ramp 0..1 within each cycle.
def signal_saw() : Pattern {
    return signal(@(t : double) : float {
        return float(t - floor(t))
    })
}

// Triangle wave 0..1. Rises 0→1 in first half, falls 1→0 in second half.
def signal_tri() : Pattern {
    return signal(@(t : double) : float {
        let pos = float(t - floor(t))
        return pos < 0.5 ? pos * 2.0 : (1.0 - pos) * 2.0
    })
}

// Deterministic Perlin-style noise mapped to 0..1.
// Uses smootherstep interpolation between pseudo-random values at integer cycles.
def signal_perlin() : Pattern {
    return signal(@(t : double) : float {
        let i = int(floor(t))
        let frac = float(t - floor(t))
        // hash integer cycle to pseudo-random value 0..1
        let h0 = float((uint(i) * 2654435761u) & 0xFFFFu) / 65535.0
        let h1 = float((uint(i + 1) * 2654435761u) & 0xFFFFu) / 65535.0
        // smootherstep: 6t^5 - 15t^4 + 10t^3
        let u = frac * frac * frac * (frac * (frac * 6.0 - 15.0) + 10.0)
        return h0 + (h1 - h0) * u
    })
}

// Scale a signal from 0..1 to lo..hi.
def signal_range(var pat : Pattern; lo, hi : float) : Pattern {
    let span = hi - lo
    return fmap(pat, @capture(= lo, = span) (e : Event) : Event {
        var r = e
        r.note = lo + r.note * span
        return r
    })
}

// Combine a base pattern with a value (signal) pattern.
// For each Hap in basePat, samples valuePat at the hap's onset time and
// applies fn(baseEvent, sampledValue) to produce the output event.
// The sampled value is extracted from the value pattern's hap value.note field.
def combineWith(var pat : Pattern; var val_pat : Pattern; var fn : lambda<(e : Event; val : float) : Event>) : Pattern {
    return @capture(<- pat, <- val_pat, <- fn) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        for (h in haps) {
            // sample the value pattern at a tiny window around the hap's onset
            let onset = h.has_whole ? h.whole.start : h.part.start
            let sampleSpan = TimeSpan(start = onset, stop = onset + 0.001lf)
            var val_haps <- val_pat(sampleSpan)
            if (length(val_haps) > 0) {
                let sampled = val_haps[0].value.note
                result |> push(Hap( // nolint:PERF006
                    whole = h.whole,
                    part = h.part,
                    has_whole = h.has_whole,
                    value = fn(h.value, sampled)
                ))
            } else {
                result |> push(h) // nolint:PERF006
            }
            delete val_haps
        }
        delete haps
        return <- result
    }
}

// ─── Pattern-valued parameter setters ───
// Each takes a Pattern for the modulation value, using combineWith internally.

// pat |> set_gain(signal_sine()) — modulate gain with a signal
def set_gain(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.gain = val; return r
    })
}

// pat |> set_note(signal_saw() |> signal_range(48.0, 72.0)) — modulate note
def set_note(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.note = val; return r
    })
}

// pat |> set_pan(signal_sine()) — modulate pan with a signal
def set_pan(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.pan = val; return r
    })
}

// pat |> set_lpf(signal_sine() |> signal_range(200.0, 4000.0)) — modulate lpf
def set_lpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.lpf = val; return r
    })
}

// pat |> set_hpf(signal_pattern) — modulate hpf with a signal
def set_hpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.hpf = val; return r
    })
}

// pat |> set_speed(signal_pattern) — modulate speed with a signal
def set_speed(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.speed = val; return r
    })
}

// pat |> set_attack(signal_pattern) — modulate attack with a signal
def set_attack(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.attack = val; return r
    })
}

// pat |> set_decay(signal_pattern) — modulate decay with a signal
def set_decay(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.decay = val; return r
    })
}

// pat |> set_sustain(signal_pattern) — modulate sustain with a signal
def set_sustain(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sustain = val; return r
    })
}

// pat |> set_release(signal_pattern) — modulate release with a signal
def set_release(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.release = val; return r
    })
}

// pat |> set_velocity(signal_pattern) — modulate velocity with a signal
def set_velocity(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.velocity = val; return r
    })
}

// pat |> set_lpq(signal_pattern) — modulate lpq with a signal
def set_lpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.lpq = val; return r
    })
}

// pat |> set_hpq(signal_pattern) — modulate hpq with a signal
def set_hpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.hpq = val; return r
    })
}

// pat |> set_room(signal_pattern) — modulate reverb send with a signal
def set_room(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.room = val; return r
    })
}

// pat |> set_roomsize(signal_pattern) — modulate room size with a signal
def set_roomsize(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.roomsize = val; return r
    })
}

// pat |> set_delay(signal_pattern) — modulate delay send with a signal
def set_delay(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.delay_amount = val; return r
    })
}

// pat |> set_delaytime(signal_pattern) — modulate delay time with a signal
def set_delaytime(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.delaytime = val; return r
    })
}

// pat |> set_delayfeedback(signal_pattern) — modulate delay feedback with a signal
def set_delayfeedback(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.delayfeedback = val; return r
    })
}

// pat |> set_fm(signal_pattern) — modulate FM depth with a signal
def set_fm(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.fm = val; return r
    })
}

// pat |> set_fmh(signal_pattern) — modulate FM harmonicity with a signal
def set_fmh(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.fmh = val; return r
    })
}

// pat |> set_chorus(signal_pattern) — modulate chorus send with a signal
def set_chorus(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.chorus = val; return r
    })
}

// pat |> set_sf2_expression(signal_pattern) — modulate SF2 expression with a signal
def set_sf2_expression(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sf2_expression = val; return r
    })
}

// pat |> set_sf2_mod_wheel(signal_pattern) — modulate SF2 mod wheel with a signal
def set_sf2_mod_wheel(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sf2_mod_wheel = val; return r
    })
}

// pat |> set_sf2_pitch_bend(signal_pattern) — modulate SF2 pitch bend with a signal
def set_sf2_pitch_bend(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sf2_pitch_bend = val; return r
    })
}

// ─── String-valued combineWith (for vowel, etc.) ───

// Like combineWith but samples a string value from the value pattern's event.s field.
// Used for string-typed parameters like vowel.
def combineWithStr(var pat : Pattern; var val_pat : Pattern; var fn : lambda<(e : Event; val : string) : Event>) : Pattern {
    return @capture(<- pat, <- val_pat, <- fn) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        for (h in haps) {
            let onset = h.has_whole ? h.whole.start : h.part.start
            let sampleSpan = TimeSpan(start = onset, stop = onset + 0.001lf)
            var val_haps <- val_pat(sampleSpan)
            if (length(val_haps) > 0) {
                let sampled = val_haps[0].value.s
                result |> push(Hap( // nolint:PERF006
                    whole = h.whole,
                    part = h.part,
                    has_whole = h.has_whole,
                    value = fn(h.value, sampled)
                ))
            } else {
                result |> push(h) // nolint:PERF006
            }
            delete val_haps
        }
        delete haps
        return <- result
    }
}

// ─── Vowel Parameter ───

// set_vowel is defined in strudel_mini.das (needs mini-notation parser).
// Handles both static "a" and pattern "<a e i o u>" syntax in a single function.

// ─── Euclidean Rhythms ───

// Euclidean rhythm: play pattern at k evenly-spaced onsets across n subdivisions.
// euclid(3, 8, atom("bd")) → kick on steps 0, 3, 6 of an 8-step cycle
def euclid(var pat : Pattern; k, n : int) : Pattern {
    var rhythm <- bjorklund(k, n)
    let nf = double(n)
    return @capture(<- rhythm, = nf, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        let steps = length(rhythm)
        let innerSpan = TimeSpan(start = span.start * nf, stop = span.stop * nf)
        var haps <- pat(innerSpan)
        for (h in haps) {
            let step = int(sameCycle(h.whole.start)) % steps
            if (rhythm[step]) {
                result |> push(Hap(
                    whole = TimeSpan(start = h.whole.start / nf, stop = h.whole.stop / nf),
                    part = TimeSpan(start = h.part.start / nf, stop = h.part.stop / nf),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
        }
        delete haps
        return <- result
    }
}
