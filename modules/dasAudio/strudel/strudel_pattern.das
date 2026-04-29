options gen2
options indenting = 4

module strudel_pattern shared public
//! Core pattern algebra and fluent DSL for daStrudel.
//! A Pattern is a pure function from time span to events (haps). This module
//! provides constructors (`pure`, `silence`, `atom`), time operators (`fast`,
//! `slow`, `stack`, `cat`), combinators (`rev`, `jux`, `every`, `euclid`, ...),
//! signals (`sine`, `saw`, `perlin`), and the full per-hap parameter setter
//! surface (`gain`, `pan`, `note`, `lpf`, `room`, ADSR, phaser, ...).

require strudel/strudel_event
require strudel/strudel_time
require math

//! A Pattern is a pure function from a query `TimeSpan` to the `Hap`s
//! (events with timing info) that fall within it. No side effects, no audio —
//! just data. All combinators in this module take and return `Pattern`.
typedef Pattern = lambda<(span : TimeSpan) : array<Hap>>
//! A function pattern → pattern. Passed to combinators like ``jux`` and ``off``
//! to describe how the transformed copy of a pattern should differ from the
//! original. Use ``@(x) => rev(x)`` for a lambda literal at call sites.
typedef PatternTransform = lambda<(var pat : Pattern) : Pattern>

// ─── Constructors ───

// A single event spanning every cycle.
def pure(value : Event) : Pattern {
    //! Lifts an `Event` into a `Pattern` that emits that value once per cycle, spanning the entire cycle.
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
    //! The empty pattern — produces no haps for any query span.
    return @(span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        return <- result
    }
}

// Shorthand: make a pattern from a sound name.
def atom(name : string) : Pattern {
    //! Shorthand for `pure(Event(s=name))` — a pattern with one event per cycle using the named sound. Example: `atom("bd")`.
    return pure(Event(s = name))
}

// ─── Time Manipulation ───

// Speed up by factor n. The pattern repeats n times per cycle.
def fast(var pat : Pattern; n : double) : Pattern {
    //! Speeds up `pat` by factor `n` — the pattern repeats `n` times per cycle. Example: `s("bd") |> fast(2.0lf)`.
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
    //! Slows down `pat` by factor `n` — `pat` takes `n` cycles to complete once. Example: `s("bd sd") |> slow(2.0lf)`.
    return fast(pat, 1.0lf / n)
}

// Shift pattern earlier in time by `offset` cycles.
def private early(var pat : Pattern; offset : double) : Pattern {
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
def private late(var pat : Pattern; offset : double) : Pattern {
    return early(pat, 0.0lf - offset)
}

// ─── Composition ───

// Layer patterns on top of each other (simultaneous).
def stack(var pats : array<Pattern>) : Pattern {
    //! Layers all patterns simultaneously — every pattern plays at once. Example: `stack([s("bd"), s("hh"), s("sd")])`.
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
    //! Sequences patterns across cycles — with N patterns, pattern `i` plays during cycle `i mod N`, each stretched to one cycle. Example: `cat([s("bd"), s("sd")])`.
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
    //! Like `cat` but squeezes all patterns into a single cycle, side by side. Example: `fastcat([s("bd"), s("sd"), s("hh")])`.
    let cn = double(length(pats))
    return fast(cat(pats), cn)
}

// Like fastcat but each element gets time proportional to its weight.
// weighted_fastcat([a, b], [3.0, 1.0]) → a gets 3/4 of cycle, b gets 1/4.
def weighted_fastcat(var pats : array<Pattern>; var weights : array<float>) : Pattern {
    //! Like `fastcat` but each element's cycle slice is proportional to its weight. Example: `weighted_fastcat([a, b], [3.0, 1.0])` — `a` fills 3/4, `b` fills 1/4.
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
    //! Applies `fn` to the `Event` value of every hap produced by `pat`, preserving timing. The functor-map over patterns.
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
    //! Randomly drops events from `pat` with probability `prob` (0..1). Deterministic — same time yields same result. Example: `s("bd*8") |> degrade(0.3)`.
    return @capture(= prob, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        for (h in haps) {
            let r = strudel_hash01(h.whole.start, 0)
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
    //! Plays `pat_on` on every `n`-th cycle, `pat_off` otherwise. Example: `every(4, s("cp"), s("bd"))` — clap every 4th cycle.
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
    //! Bjorklund's algorithm — distributes `k` onsets as evenly as possible across `n` slots. Example: `bjorklund(3, 8)` gives `[x . . x . . x .]`.
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
    //! Sets the `gain` (amplitude, 0..1) field on every event in `pat`. Example: `pat |> set_gain(0.5)`.
    let fn <- @capture(= g) (e : Event) : Event {
        var r = e; r.gain = g; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_velocity(0.5) — set expression/velocity
def private set_velocity(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.velocity = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_pan(0.0) — set stereo position (0=left, 0.5=center, 1=right)
def set_pan(var pat : Pattern; p : float) : Pattern {
    //! Sets the `pan` (stereo position: 0=left, 0.5=center, 1=right) field on every event in `pat`.
    let fn <- @capture(= p) (e : Event) : Event {
        var r = e; r.pan = p; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hrtf_azimuth(-45.0) — set 3D HRTF azimuth (degrees, -180..180); pan still controls source width.
def set_hrtf_azimuth(var pat : Pattern; deg : float) : Pattern {
    //! Sets the HRTF azimuth (degrees, -180..180) on every event in `pat` and engages HRTF positioning.
    //! 0 = directly ahead, +90 = right, -90 = left. Under the binaural-stereo HRTF render `pan` still
    //! controls source width / L/R balance feeding the spatialiser; HRTF controls 3D position.
    let fn <- @capture(= deg) (e : Event) : Event {
        var r = e; r.hrtf_azimuth = deg; r.hrtf_active = true; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hrtf_elevation(15.0) — set 3D HRTF elevation (degrees, -90..90); pan still controls source width.
def set_hrtf_elevation(var pat : Pattern; deg : float) : Pattern {
    //! Sets the HRTF elevation (degrees, -90..90) on every event in `pat` and engages HRTF positioning.
    //! 0 = horizontal plane, positive = up. Under the binaural-stereo HRTF render `pan` still controls
    //! source width / L/R balance feeding the spatialiser; HRTF controls 3D position.
    let fn <- @capture(= deg) (e : Event) : Event {
        var r = e; r.hrtf_elevation = deg; r.hrtf_active = true; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hrtf(-45.0, 0.0) — set both HRTF azimuth and elevation in one call; pan still controls source width.
def set_hrtf(var pat : Pattern; az, el : float) : Pattern {
    //! Sets HRTF azimuth and elevation in one call and engages HRTF positioning.
    //! Azimuth (-180..180): 0 = ahead, +90 = right, -90 = left. Elevation (-90..90): 0 = horizontal.
    //! Under the binaural-stereo HRTF render `pan` still controls source width / L/R balance feeding
    //! the spatialiser; HRTF controls 3D position.
    let fn <- @capture(= az, = el) (e : Event) : Event {
        var r = e; r.hrtf_azimuth = az; r.hrtf_elevation = el; r.hrtf_active = true; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_speed(2.0) — set sample playback speed
def private set_speed(var pat : Pattern; spd : float) : Pattern {
    let fn <- @capture(= spd) (e : Event) : Event {
        var r = e; r.speed = spd; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_note(48.0) — set MIDI note number
def set_note(var pat : Pattern; n : float) : Pattern {
    //! Sets the MIDI `note` number on every event in `pat`. Example: `pat |> set_note(48.0)` (C3).
    let fn <- @capture(= n) (e : Event) : Event {
        var r = e; r.note = n; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sound("sawtooth") — change the sound name
def set_sound(var pat : Pattern; snd : string) : Pattern {
    //! Sets the sound name `s` on every event in `pat`. Example: `pat |> set_sound("sawtooth")`.
    let fn <- @capture(= snd) (e : Event) : Event {
        var r = e; r.s = snd; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sound(sound_pat) — sample sound name from a pattern at each event onset.
// Enables strudel-style: note("c3").s("<supersaw sawtooth>")
def set_sound(var pat : Pattern; var sound_pat : Pattern) : Pattern {
    //! Pattern-valued `set_sound`: samples the sound name from `sound_pat` at each event onset. Example: `note("c3") |> set_sound(s("<saw tri>"))`.
    return combineWithStr(pat, sound_pat, @(e : Event; val : string) : Event {
        var r = e; r.s = val; return r
    })
}

// pat |> set_cut(1) — set cut group (new voice kills previous in same group)
def private set_cut(var pat : Pattern; c : int) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.cut = c; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_room(0.5) — set reverb send amount
def set_room(var pat : Pattern; r : float) : Pattern {
    //! Sets the reverb send amount `room` (0..1) on every event in `pat`.
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.room = r; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_roomsize(0.8) — set reverb room size
def set_roomsize(var pat : Pattern; r : float) : Pattern {
    //! Sets the reverb room size `roomsize` (larger = more sustained reverb) on every event in `pat`.
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.roomsize = r; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delay(0.5) — set delay send amount
def set_delay(var pat : Pattern; d : float) : Pattern {
    //! Sets the delay send amount (`delay_amount`, 0..1) on every event in `pat`.
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delay_amount = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delaytime(0.25) — set delay time in seconds
def set_delaytime(var pat : Pattern; d : float) : Pattern {
    //! Sets the delay time `delaytime` (seconds) on every event in `pat`.
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delaytime = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delayfeedback(0.7) — set delay feedback
def set_delayfeedback(var pat : Pattern; d : float) : Pattern {
    //! Sets the delay feedback `delayfeedback` (0..<1) on every event in `pat`.
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delayfeedback = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_delaysync(0.1875) — set delay time in cycles (tempo-synced delay)
def private set_delaysync(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.delaysync = d; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_orbit(1) — set effect bus routing
def set_orbit(var pat : Pattern; o : int) : Pattern {
    //! Sets the effect-bus routing index `orbit` on every event in `pat`. Different orbits have independent FX chains.
    let fn <- @capture(= o) (e : Event) : Event {
        var ev = e; ev.orbit = o; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_fm(2.0) — set FM modulation depth
def set_fm(var pat : Pattern; f : float) : Pattern {
    //! Sets the FM modulation depth `fm` (modulator amplitude) on every event in `pat`.
    let fn <- @capture(= f) (e : Event) : Event {
        var ev = e; ev.fm = f; return ev
    }
    return <- fmap(pat, fn)
}

// pat |> set_fmh(1.5) — set FM harmonicity ratio
def set_fmh(var pat : Pattern; f : float) : Pattern {
    //! Sets the FM harmonicity ratio `fmh` (modulator frequency / carrier frequency) on every event in `pat`.
    let fn <- @capture(= f) (e : Event) : Event {
        var ev = e; ev.fmh = f; return ev
    }
    return <- fmap(pat, fn)
}

// ─── Sample Region & Audio Effects ───

def private set_begin(var pat : Pattern; b : float) : Pattern {
    let fn <- @capture(= b) (e : Event) : Event {
        var r = e; r.begin = b; return r
    }
    return <- fmap(pat, fn)
}

def private set_end(var pat : Pattern; e_val : float) : Pattern {
    let fn <- @capture(= e_val) (e : Event) : Event {
        var r = e; r.end_pos = e_val; return r
    }
    return <- fmap(pat, fn)
}

def private set_crush(var pat : Pattern; c : float) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.crush = c; return r
    }
    return <- fmap(pat, fn)
}

def private set_coarse(var pat : Pattern; c : int) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.coarse = c; return r
    }
    return <- fmap(pat, fn)
}

def private set_shape(var pat : Pattern; s : float) : Pattern {
    let fn <- @capture(= s) (e : Event) : Event {
        var r = e; r.shape = s; return r
    }
    return <- fmap(pat, fn)
}

def private set_djf(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var r = e; r.djf = d; return r
    }
    return <- fmap(pat, fn)
}

def private set_bpf(var pat : Pattern; freq : float) : Pattern {
    let fn <- @capture(= freq) (e : Event) : Event {
        var r = e; r.bpf = freq; return r
    }
    return <- fmap(pat, fn)
}

def private set_bpq(var pat : Pattern; q : float) : Pattern {
    let fn <- @capture(= q) (e : Event) : Event {
        var r = e; r.bpq = q; return r
    }
    return <- fmap(pat, fn)
}

// Phaser: rate in Hz (0 disables). Modulates a single notch biquad.
def private set_phaser(var pat : Pattern; r : float) : Pattern {
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.phaser = r; return ev
    }
    return <- fmap(pat, fn)
}

def private set_phaserdepth(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.phaserdepth = d; return ev
    }
    return <- fmap(pat, fn)
}

def private set_phasercenter(var pat : Pattern; c : float) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var ev = e; ev.phasercenter = c; return ev
    }
    return <- fmap(pat, fn)
}

def private set_phasersweep(var pat : Pattern; s : float) : Pattern {
    let fn <- @capture(= s) (e : Event) : Event {
        var ev = e; ev.phasersweep = s; return ev
    }
    return <- fmap(pat, fn)
}

// Tremolo: rate in Hz (0 disables), amplitude modulation.
def private set_tremolo(var pat : Pattern; r : float) : Pattern {
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.tremolo = r; return ev
    }
    return <- fmap(pat, fn)
}

def private set_tremolodepth(var pat : Pattern; d : float) : Pattern {
    let fn <- @capture(= d) (e : Event) : Event {
        var ev = e; ev.tremolodepth = d; return ev
    }
    return <- fmap(pat, fn)
}

// Compressor: threshold in dB (>=0 = bypass). Negative values activate.
def private set_compressor(var pat : Pattern; thr_db : float) : Pattern {
    let fn <- @capture(= thr_db) (e : Event) : Event {
        var ev = e; ev.compressor = thr_db; return ev
    }
    return <- fmap(pat, fn)
}

def private set_compressor_ratio(var pat : Pattern; ratio : float) : Pattern {
    let fn <- @capture(= ratio) (e : Event) : Event {
        var ev = e; ev.compressor_ratio = ratio; return ev
    }
    return <- fmap(pat, fn)
}

def private set_compressor_knee(var pat : Pattern; knee : float) : Pattern {
    let fn <- @capture(= knee) (e : Event) : Event {
        var ev = e; ev.compressor_knee = knee; return ev
    }
    return <- fmap(pat, fn)
}

def private set_compressor_attack(var pat : Pattern; a : float) : Pattern {
    let fn <- @capture(= a) (e : Event) : Event {
        var ev = e; ev.compressor_attack = a; return ev
    }
    return <- fmap(pat, fn)
}

def private set_compressor_release(var pat : Pattern; r : float) : Pattern {
    let fn <- @capture(= r) (e : Event) : Event {
        var ev = e; ev.compressor_release = r; return ev
    }
    return <- fmap(pat, fn)
}

// ─── SF2 SoundFont Helpers ───

// GM instrument name → program number mapping.
def private gm_program_by_name(name : string) : int {
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
def private set_sf2(var pat : Pattern; program : int) : Pattern {
    let fn <- @capture(= program) (e : Event) : Event {
        var r = e; r.sf2_program = program; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2("piano") — set SF2 instrument by GM name
def private set_sf2(var pat : Pattern; name : string) : Pattern {
    let program = gm_program_by_name(name)
    let fn <- @capture(= program) (e : Event) : Event {
        var r = e; r.sf2_program = program; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_bank(128) — set SF2 bank (0 = melodic, 128 = percussion)
def private set_sf2_bank(var pat : Pattern; bank : int) : Pattern {
    let fn <- @capture(= bank) (e : Event) : Event {
        var r = e; r.sf2_bank = bank; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_expression(0.5) — set SF2 expression CC11 (0.0–1.0)
def private set_sf2_expression(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.sf2_expression = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_mod_wheel(0.5) — set SF2 mod wheel CC1 (0.0–1.0, vibrato depth)
def private set_sf2_mod_wheel(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.sf2_mod_wheel = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sf2_pitch_bend(0.6) — set SF2 pitch bend (0.0–1.0, 0.5 = center)
def private set_sf2_pitch_bend(var pat : Pattern; v : float) : Pattern {
    let fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.sf2_pitch_bend = v; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_chorus(0.5) — set chorus send amount (0.0–1.0)
def private set_chorus(var pat : Pattern; c : float) : Pattern {
    let fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.chorus = c; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_lpq(5.0) — set low-pass filter resonance
def set_lpq(var pat : Pattern; q : float) : Pattern {
    //! Sets the low-pass filter resonance `lpq` (Q factor) on every event in `pat`.
    let fn <- @capture(= q) (e : Event) : Event {
        var r = e; r.lpq = q; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hpq(5.0) — set high-pass filter resonance
def set_hpq(var pat : Pattern; q : float) : Pattern {
    //! Sets the high-pass filter resonance `hpq` (Q factor) on every event in `pat`.
    let fn <- @capture(= q) (e : Event) : Event {
        var r = e; r.hpq = q; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_lpf(1000.0) — set low-pass filter cutoff
def set_lpf(var pat : Pattern; freq : float) : Pattern {
    //! Sets the low-pass filter cutoff `lpf` (Hz) on every event in `pat`.
    let fn <- @capture(= freq) (e : Event) : Event {
        var r = e; r.lpf = freq; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_hpf(200.0) — set high-pass filter cutoff
def set_hpf(var pat : Pattern; freq : float) : Pattern {
    //! Sets the high-pass filter cutoff `hpf` (Hz) on every event in `pat`.
    let fn <- @capture(= freq) (e : Event) : Event {
        var r = e; r.hpf = freq; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_attack(0.1) — set envelope attack time
def set_attack(var pat : Pattern; a : float) : Pattern {
    //! Sets the amp-envelope `attack` time (seconds) on every event in `pat`.
    let fn <- @capture(= a) (e : Event) : Event {
        var r = e; r.attack = a; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_decay(0.3) — set envelope decay time
def set_decay(var pat : Pattern; d : float) : Pattern {
    //! Sets the amp-envelope `decay` time (seconds) on every event in `pat`.
    let fn <- @capture(= d) (e : Event) : Event {
        var r = e; r.decay = d; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_sustain(0.7) — set envelope sustain level
def set_sustain(var pat : Pattern; s : float) : Pattern {
    //! Sets the amp-envelope `sustain` level (0..1) on every event in `pat`.
    let fn <- @capture(= s) (e : Event) : Event {
        var r = e; r.sustain = s; return r
    }
    return <- fmap(pat, fn)
}

// pat |> set_release(0.5) — set envelope release time
def set_release(var pat : Pattern; r : float) : Pattern {
    //! Sets the amp-envelope `release` time (seconds) on every event in `pat`.
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
    //! Reverses event positions within each cycle. Example: `s("bd sd cp") |> rev()` plays `cp sd bd`.
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
    //! Stereo split: original is panned left, `transform(pat)` is panned right. Example: `s("bd sd") |> jux(@@(x) => rev(x))`.
    var transformed <- invoke(transform, pat) // nolint:LINT003
    var left <- pat |> set_pan(0.0)
    var right <- transformed |> set_pan(1.0)
    var layers : array<Pattern> // nolint:STYLE012
    layers |> emplace <| left
    layers |> emplace <| right
    return <- stack(layers)
}

// Overlay pattern with a time-shifted copy.
// off(s("bd sd"), 0.125, @(x) => transpose(x, 7.0)) — canon at the fifth
def off(var pat : Pattern; time : double; transform : PatternTransform) : Pattern {
    //! Overlays `pat` with a time-shifted copy transformed by `transform`. Example: `s("bd sd") |> off(0.125, @@(x) => transpose(x, 7.0))` — canon at the fifth.
    var shifted <- invoke(transform, pat) |> late(time)
    var layers : array<Pattern> // nolint:STYLE012
    layers |> emplace <| pat
    layers |> emplace <| shifted
    return <- stack(layers)
}

// Keep events where hash >= prob (original survivors).
def private degrade_keep(var pat : Pattern; prob : float) : Pattern {
    let cn = prob
    return @capture(= cn, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope haps = pat(span)
        for (h in haps) {
            if (h.has_whole) {
                let r = strudel_hash01(h.whole.start, 0)
                if (r >= cn) {
                    result |> push(h) // nolint:PERF006
                }
            }
        }
        return <- result
    }
}

// Keep events where hash < prob (complement — selected for transform).
def private degrade_drop(var pat : Pattern; prob : float) : Pattern {
    let cn = prob
    return @capture(= cn, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope haps = pat(span)
        for (h in haps) {
            if (h.has_whole) {
                let r = strudel_hash01(h.whole.start, 0)
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
    //! Applies `transform` to each event with probability `prob` (0..1). Example: `s("bd*8") |> sometimesby(0.3, @@(x) => fast(x, 2.0lf))`.
    var degraded <- degrade_keep(pat, prob)
    var undegraded <- degrade_drop(pat, prob) // nolint:LINT003
    var transformed <- invoke(transform, undegraded) // nolint:LINT003
    var layers : array<Pattern> // nolint:STYLE012
    layers |> emplace <| degraded
    layers |> emplace <| transformed
    return <- stack(layers)
}

// sometimes = 50% probability
def sometimes(var pat : Pattern; transform : PatternTransform) : Pattern {
    //! Applies `transform` to each event with 50% probability. Shorthand for `sometimesby(pat, 0.5, transform)`.
    return <- sometimesby(pat, 0.5, transform)
}

// often = 75% probability
def often(var pat : Pattern; transform : PatternTransform) : Pattern {
    //! Applies `transform` to each event with 75% probability. Shorthand for `sometimesby(pat, 0.75, transform)`.
    return <- sometimesby(pat, 0.75, transform)
}

// rarely = 25% probability
def private rarely(var pat : Pattern; transform : PatternTransform) : Pattern {
    return <- sometimesby(pat, 0.25, transform)
}

// Stack multiple patterns on top of the base pattern.
// layer(s("bd"), [s("bd") |> transpose(7.0), s("bd") |> transpose(12.0)])
def layer(var pats : array<Pattern>; var base : Pattern) : Pattern {
    //! Stacks `pats` on top of `base`, all playing simultaneously. Example: `s("bd") |> layer([s("bd") |> transpose(7.0), s("bd") |> transpose(12.0)])`.
    pats |> emplace <| base
    return <- stack(pats)
}

// Shift note by semitones. transpose(pat, 7) = up a fifth.
def transpose(var pat : Pattern; semitones : float) : Pattern {
    //! Shifts every event's MIDI note by `semitones`. Example: `pat |> transpose(7.0)` transposes up a fifth.
    let fn <- @capture(= semitones) (e : Event) : Event {
        var r = e; r.note += semitones; return r
    }
    return <- fmap(pat, fn)
}

// Alias: add is the same as transpose (matches strudel naming).
def add(var pat : Pattern; semitones : float) : Pattern {
    //! Alias for `transpose` — adds `semitones` to each event's note. Matches strudel naming.
    return <- transpose(pat, semitones)
}

// ─── innerJoin (patternified parameters) ───

// Core mechanism for patternified parameters: for each hap in param_pat,
// create a pattern from its note value via fn, query that inner pattern,
// and collect results whose parts overlap. This is how slow("<2 16>") works.
def innerJoin(var param_pat : Pattern; var fn : lambda<(val : float) : Pattern>) : Pattern {
    //! Pattern monadic join — for each hap in `param_pat`, builds an inner pattern via `fn(hap.note)` and intersects its haps with the outer hap's part. Core mechanism for patternified parameters.
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
    //! Patternified `slow` — slow factor varies per cycle from `factor_pat`'s note values. Example: `slow(pat, n("<2 16>"))` alternates between 2x and 16x slowdown.
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
    //! Patternified `fast` — speed factor varies per cycle from `factor_pat`'s note values. Example: `fast(pat, n("<1 2 4>"))`.
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
    //! Builds a continuous-signal pattern from `fn(t) : float`. Produces one hap per query with `has_whole=false` and value stored in `note`. Foundation for `sine`, `saw`, `perlin`, etc.
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
    //! Unipolar sine signal, mapped to `0..1` over one cycle. At t=0 → 0.5, t=0.25 → 1.0, t=0.5 → 0.5, t=0.75 → 0.0.
    return signal(@(t : double) : float {
        return float(sin(3.14159265358979323846lf * 2.0lf * t) + 1.0lf) * 0.5
    })
}

// Cosine wave mapped to 0..1. Same as sine shifted left by 0.25 cycles.
def signal_cosine() : Pattern {
    //! Unipolar cosine signal, mapped to `0..1` over one cycle. Same as `signal_sine` shifted left by 0.25 cycles.
    return signal(@(t : double) : float {
        return float(cos(3.14159265358979323846lf * 2.0lf * t) + 1.0lf) * 0.5
    })
}

// Sawtooth ramp 0..1 within each cycle.
def signal_saw() : Pattern {
    //! Sawtooth signal — ramps `0..1` linearly within each cycle.
    return signal(@(t : double) : float {
        return float(t - floor(t))
    })
}

// Triangle wave 0..1. Rises 0→1 in first half, falls 1→0 in second half.
def signal_tri() : Pattern {
    //! Triangle signal — rises `0→1` in the first half of each cycle, falls `1→0` in the second half.
    return signal(@(t : double) : float {
        let pos = float(t - floor(t))
        return pos < 0.5 ? pos * 2.0 : (1.0 - pos) * 2.0
    })
}

// Deterministic Perlin-style noise mapped to 0..1.
// Uses smootherstep interpolation between pseudo-random values at integer cycles.
def signal_perlin() : Pattern {
    //! Deterministic Perlin-style noise mapped to `0..1`. Uses smootherstep interpolation between hashed random values at integer cycle boundaries.
    return signal(@(t : double) : float {
        let i = int(floor(t))
        let frac = float(t - floor(t))
        // hash integer cycle to pseudo-random value 0..1
        let h0 = strudel_hash01(double(i), 0)
        let h1 = strudel_hash01(double(i + 1), 0)
        // smootherstep: 6t^5 - 15t^4 + 10t^3
        let u = frac * frac * frac * (frac * (frac * 6.0 - 15.0) + 10.0)
        return h0 + (h1 - h0) * u
    })
}

// Scale a signal from 0..1 to lo..hi.
def signal_range(var pat : Pattern; lo, hi : float) : Pattern {
    //! Scales a signal pattern from `0..1` to `lo..hi`. Example: `sine() |> signal_range(200.0, 4000.0)` for a 200..4000 Hz filter sweep.
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
    //! For each hap in `pat`, samples `val_pat` at the hap's onset and applies `fn(event, sampledValue)` to produce the output event. Core primitive for pattern-modulated setters.
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
    //! Pattern-valued `set_gain` — samples `mod_pat` at each event onset to set `gain`. Example: `pat |> set_gain(sine())`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.gain = val; return r
    })
}

// pat |> set_note(signal_saw() |> signal_range(48.0, 72.0)) — modulate note
def set_note(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_note` — samples `mod_pat` at each event onset to set `note`. Example: `pat |> set_note(saw() |> range(48.0, 72.0))`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.note = val; return r
    })
}

// pat |> set_pan(signal_sine()) — modulate pan with a signal
def set_pan(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_pan` — samples `mod_pat` at each event onset to set `pan`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.pan = val; return r
    })
}

def set_hrtf_azimuth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_hrtf_azimuth` — samples `mod_pat` at each event onset to set the HRTF azimuth (degrees, -180..180).
    //! Engages HRTF positioning on every event; `pan` still controls source width / L/R balance feeding the spatialiser.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.hrtf_azimuth = val; r.hrtf_active = true; return r
    })
}

def set_hrtf_elevation(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_hrtf_elevation` — samples `mod_pat` at each event onset to set the HRTF elevation (degrees, -90..90).
    //! Engages HRTF positioning on every event; `pan` still controls source width / L/R balance feeding the spatialiser.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.hrtf_elevation = val; r.hrtf_active = true; return r
    })
}

// pat |> set_lpf(signal_sine() |> signal_range(200.0, 4000.0)) — modulate lpf
def set_lpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_lpf` — samples `mod_pat` at each event onset to set `lpf` cutoff. Example: `pat |> set_lpf(sine() |> range(200.0, 4000.0))`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.lpf = val; return r
    })
}

// pat |> set_hpf(signal_pattern) — modulate hpf with a signal
def set_hpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_hpf` — samples `mod_pat` at each event onset to set `hpf` cutoff.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.hpf = val; return r
    })
}

// pat |> set_speed(signal_pattern) — modulate speed with a signal
def set_speed(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_speed` — samples `mod_pat` at each event onset to set sample playback `speed`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.speed = val; return r
    })
}

// pat |> set_attack(signal_pattern) — modulate attack with a signal
def set_attack(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_attack` — samples `mod_pat` at each event onset to set envelope `attack`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.attack = val; return r
    })
}

// pat |> set_decay(signal_pattern) — modulate decay with a signal
def set_decay(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_decay` — samples `mod_pat` at each event onset to set envelope `decay`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.decay = val; return r
    })
}

// pat |> set_sustain(signal_pattern) — modulate sustain with a signal
def set_sustain(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_sustain` — samples `mod_pat` at each event onset to set envelope `sustain`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sustain = val; return r
    })
}

// pat |> set_release(signal_pattern) — modulate release with a signal
def set_release(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_release` — samples `mod_pat` at each event onset to set envelope `release`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.release = val; return r
    })
}

// pat |> set_velocity(signal_pattern) — modulate velocity with a signal
def set_velocity(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_velocity` — samples `mod_pat` at each event onset to set `velocity`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.velocity = val; return r
    })
}

// pat |> set_lpq(signal_pattern) — modulate lpq with a signal
def set_lpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_lpq` — samples `mod_pat` at each event onset to set low-pass resonance.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.lpq = val; return r
    })
}

// pat |> set_hpq(signal_pattern) — modulate hpq with a signal
def set_hpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_hpq` — samples `mod_pat` at each event onset to set high-pass resonance.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.hpq = val; return r
    })
}

// pat |> set_room(signal_pattern) — modulate reverb send with a signal
def set_room(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_room` — samples `mod_pat` at each event onset to set reverb send amount.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.room = val; return r
    })
}

// pat |> set_roomsize(signal_pattern) — modulate room size with a signal
def set_roomsize(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_roomsize` — samples `mod_pat` at each event onset to set reverb `roomsize`.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.roomsize = val; return r
    })
}

// pat |> set_delay(signal_pattern) — modulate delay send with a signal
def set_delay(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_delay` — samples `mod_pat` at each event onset to set delay send.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.delay_amount = val; return r
    })
}

// pat |> set_delaytime(signal_pattern) — modulate delay time with a signal
def set_delaytime(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_delaytime` — samples `mod_pat` at each event onset to set delay time.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.delaytime = val; return r
    })
}

// pat |> set_delayfeedback(signal_pattern) — modulate delay feedback with a signal
def set_delayfeedback(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_delayfeedback` — samples `mod_pat` at each event onset to set delay feedback.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.delayfeedback = val; return r
    })
}

// pat |> set_fm(signal_pattern) — modulate FM depth with a signal
def set_fm(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_fm` — samples `mod_pat` at each event onset to set FM modulation depth.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.fm = val; return r
    })
}

// pat |> set_fmh(signal_pattern) — modulate FM harmonicity with a signal
def set_fmh(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_fmh` — samples `mod_pat` at each event onset to set FM harmonicity ratio.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.fmh = val; return r
    })
}

// pat |> set_chorus(signal_pattern) — modulate chorus send with a signal
def set_chorus(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_chorus` — samples `mod_pat` at each event onset to set chorus send.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.chorus = val; return r
    })
}

// pat |> set_sf2_expression(signal_pattern) — modulate SF2 expression with a signal
def set_sf2_expression(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_sf2_expression` — samples `mod_pat` at each event onset to set SF2 expression CC11.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sf2_expression = val; return r
    })
}

// pat |> set_sf2_mod_wheel(signal_pattern) — modulate SF2 mod wheel with a signal
def set_sf2_mod_wheel(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_sf2_mod_wheel` — samples `mod_pat` at each event onset to set SF2 mod wheel CC1.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sf2_mod_wheel = val; return r
    })
}

// pat |> set_sf2_pitch_bend(signal_pattern) — modulate SF2 pitch bend with a signal
def set_sf2_pitch_bend(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_sf2_pitch_bend` — samples `mod_pat` at each event onset to set SF2 pitch bend.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.sf2_pitch_bend = val; return r
    })
}

// ─── Pattern-modulated sample region & effects setters ───

def set_begin(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_begin` — samples `mod_pat` at each event onset to set sample start position (0..1).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.begin = val; return r
    })
}

def set_end(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_end` — samples `mod_pat` at each event onset to set sample end position (0..1).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.end_pos = val; return r
    })
}

def set_crush(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_crush` — samples `mod_pat` at each event onset to set bit-crush amount.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.crush = val; return r
    })
}

def set_shape(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_shape` — samples `mod_pat` at each event onset to set waveshape/distortion amount.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.shape = val; return r
    })
}

def set_djf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_djf` — samples `mod_pat` at each event onset to set DJ-filter position (<0.5 = LP, >0.5 = HP).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.djf = val; return r
    })
}

def set_bpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_bpf` — samples `mod_pat` at each event onset to set band-pass filter cutoff (Hz).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.bpf = val; return r
    })
}

def set_bpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_bpq` — samples `mod_pat` at each event onset to set band-pass filter Q.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.bpq = val; return r
    })
}

def set_phaser(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_phaser` — samples `mod_pat` at each event onset to set phaser rate (Hz).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.phaser = val; return r
    })
}

def set_phaserdepth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_phaserdepth` — samples `mod_pat` at each event onset to set phaser depth.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.phaserdepth = val; return r
    })
}

def set_phasercenter(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_phasercenter` — samples `mod_pat` at each event onset to set phaser center frequency.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.phasercenter = val; return r
    })
}

def set_phasersweep(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_phasersweep` — samples `mod_pat` at each event onset to set phaser sweep range.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.phasersweep = val; return r
    })
}

def set_tremolo(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_tremolo` — samples `mod_pat` at each event onset to set tremolo rate (Hz).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.tremolo = val; return r
    })
}

def set_tremolodepth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_tremolodepth` — samples `mod_pat` at each event onset to set tremolo depth.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.tremolodepth = val; return r
    })
}

def set_compressor(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_compressor` — samples `mod_pat` at each event onset to set compressor threshold (dB).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.compressor = val; return r
    })
}

def set_compressor_ratio(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_compressor_ratio` — samples `mod_pat` at each event onset to set compressor ratio.
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.compressor_ratio = val; return r
    })
}

def set_compressor_knee(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_compressor_knee` — samples `mod_pat` at each event onset to set compressor knee (dB).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.compressor_knee = val; return r
    })
}

def set_compressor_attack(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_compressor_attack` — samples `mod_pat` at each event onset to set compressor attack (seconds).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.compressor_attack = val; return r
    })
}

def set_compressor_release(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-valued `set_compressor_release` — samples `mod_pat` at each event onset to set compressor release (seconds).
    return combineWith(pat, mod_pat, @(e : Event; val : float) : Event {
        var r = e; r.compressor_release = val; return r
    })
}

// ─── String-valued combineWith (for vowel, etc.) ───

// Like combineWith but samples a string value from the value pattern's event.s field.
// Used for string-typed parameters like vowel.
def combineWithStr(var pat : Pattern; var val_pat : Pattern; var fn : lambda<(e : Event; val : string) : Event>) : Pattern {
    //! Like `combineWith` but samples a string from `val_pat`'s `event.s` field. Used for string-typed parameters (e.g. `vowel`, `s`).
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
    //! Euclidean rhythm — `pat` plays at `k` evenly-spaced onsets over `n` subdivisions of each cycle. Example: `atom("bd") |> euclid(3, 8)` → hits on steps 0, 3, 6.
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

// ─── Additional Combinators ───

// Play forward on even cycles, reversed on odd cycles.
def palindrome(var pat : Pattern) : Pattern {
    //! Plays `pat` forward on even cycles, reversed on odd cycles. Example: `s("bd sd cp") |> palindrome()`.
    return @capture(<- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            if (cycle % 2 == 0) {
                var haps <- pat(sub)
                for (h in haps) {
                    result |> push(h) // nolint:PERF006
                }
                delete haps
            } else {
                // mirror within cycle
                let cyc = floor(sub.start)
                let relStart = sub.start - cyc
                let relStop = sub.stop - cyc
                let mirStart = 1.0lf - relStop
                let mirStop = 1.0lf - relStart
                let mirSpan = TimeSpan(start = mirStart + cyc, stop = mirStop + cyc)
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
        }
        return <- result
    }
}

// Stack original + transformed copy.
// superimpose(pat, @(x) => fast(x, 2.0lf)) — layer faster copy on top
def superimpose(var pat : Pattern; transform : PatternTransform) : Pattern {
    //! Stacks `pat` with `transform(pat)`, both playing simultaneously. Example: `pat |> superimpose(@@(x) => fast(x, 2.0lf))`.
    var transformed <- invoke(transform, pat)
    var layers : array<Pattern> // nolint:STYLE012
    layers |> emplace <| pat
    layers |> emplace <| transformed
    return <- stack(layers)
}

// Repeat events with time offset and gain decay.
// echo(pat, 3, 0.25, 0.5) — 3 echoes, 0.25 cycles apart, each 50% quieter
def echo(var pat : Pattern; times : int; time : double; feedback : float) : Pattern {
    //! Repeats every event `times` times, offset by `time` cycles each, with each repeat scaled by `feedback`. Example: `pat |> echo(3, 0.25lf, 0.5)`.
    // Build gain table and offsets, then query pat once per span and duplicate haps
    var gains : array<float>
    var offsets : array<double>
    gains |> reserve(times)
    offsets |> reserve(times)
    gains |> push(1.0)       // original at full gain
    offsets |> push(0.0lf)   // original at zero offset
    var current_gain = feedback
    for (i in range(1, times)) {
        gains |> push(current_gain)
        offsets |> push(time * double(i))
        current_gain *= feedback
    }
    let n = length(gains)
    return @capture(= n, <- gains, <- offsets, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        for (layer in range(n)) {
            let off = offsets[layer]
            let g = gains[layer]
            // Echoes play LATER than original: query pat at (span - off), emit at (hap + off)
            let innerSpan = TimeSpan(start = span.start - off, stop = span.stop - off)
            var haps <- pat(innerSpan)
            for (h in haps) {
                var ev = h.value
                ev.gain = ev.gain * g
                result |> push(Hap( // nolint:PERF006
                    whole = TimeSpan(start = h.whole.start + off, stop = h.whole.stop + off),
                    part = TimeSpan(start = h.part.start + off, stop = h.part.stop + off),
                    has_whole = h.has_whole,
                    value = ev
                ))
            }
            delete haps
        }
        return <- result
    }
}

// stut — alias for echo with reordered parameters (times, feedback, time).
def stut(var pat : Pattern; times : int; feedback : float; time : double) : Pattern {
    //! Alias for `echo` with argument order `(times, feedback, time)`. Example: `pat |> stut(3, 0.5, 0.125lf)`.
    return <- echo(pat, times, time, feedback)
}

// Repeat each event n times within its timespan.
def ply(var pat : Pattern; n : int) : Pattern {
    //! Repeats each event `n` times within its own timespan. Example: `s("bd sd") |> ply(3)` plays each hit as a triplet.
    let nf = double(n)
    return @capture(= n, = nf, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        for (h in haps) {
            if (!h.has_whole) {
                result |> push(h) // nolint:PERF006
                continue
            }
            let dur = (h.whole.stop - h.whole.start) / nf
            for (i in range(n)) {
                let subStart = h.whole.start + dur * double(i)
                let subStop = subStart + dur
                let pStart = max(subStart, h.part.start)
                let pStop = min(subStop, h.part.stop)
                if (pStart < pStop) {
                    result |> push(Hap(
                        whole = TimeSpan(start = subStart, stop = subStop),
                        part = TimeSpan(start = pStart, stop = pStop),
                        has_whole = true,
                        value = h.value
                    ))
                }
            }
        }
        delete haps
        return <- result
    }
}

// Shift pattern left by 1/n each cycle.
// iter(pat, 4) — cycle 0: original, cycle 1: shifted 1/4, cycle 2: shifted 2/4, etc.
def iter(var pat : Pattern; n : int) : Pattern {
    //! Rotates `pat` left by `1/n` cycle each cycle. Example: `pat |> iter(4)` — cycle 0 original, cycle 1 shifted 1/4, etc.
    let nf = double(n)
    return @capture(= n, = nf, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            var shift = double(cycle % n) / nf
            if (shift < 0.0lf) {
                shift += 1.0lf
            }
            let innerSpan = TimeSpan(start = sub.start + shift, stop = sub.stop + shift)
            var haps <- pat(innerSpan)
            for (h in haps) {
                result |> push(Hap( // nolint:PERF006
                    whole = TimeSpan(start = h.whole.start - shift, stop = h.whole.stop - shift),
                    part = TimeSpan(start = h.part.start - shift, stop = h.part.stop - shift),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
            delete haps
        }
        return <- result
    }
}

// Like iter but shifts right instead of left.
def iterBack(var pat : Pattern; n : int) : Pattern {
    //! Like `iter` but rotates `pat` right by `1/n` cycle each cycle. Example: `pat |> iterBack(4)`.
    let nf = double(n)
    return @capture(= n, = nf, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            var shift = 0.0lf - double(cycle % n) / nf
            if (shift > 0.0lf) {
                shift -= 1.0lf
            }
            let innerSpan = TimeSpan(start = sub.start + shift, stop = sub.stop + shift)
            var haps <- pat(innerSpan)
            for (h in haps) {
                result |> push(Hap( // nolint:PERF006
                    whole = TimeSpan(start = h.whole.start - shift, stop = h.whole.stop - shift),
                    part = TimeSpan(start = h.part.start - shift, stop = h.part.stop - shift),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
            delete haps
        }
        return <- result
    }
}

// Conditional transform based on cycle number.
// when_cycle(pat, @(c) => c % 4 == 0, @(x) => fast(x, 2.0lf))
def when_cycle(var pat : Pattern; var cond : lambda<(cycle : int) : bool>; transform : PatternTransform) : Pattern {
    //! Applies `transform` to `pat` on cycles where `cond(cycle)` is true, leaving others untouched. Example: `pat |> when_cycle(@(c) => c % 4 == 0, @@(x) => fast(x, 2.0lf))`.
    var transformed <- invoke(transform, pat)
    return @capture(<- pat, <- transformed, <- cond) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            if (cond(cycle)) {
                var haps <- transformed(sub)
                for (h in haps) {
                    result |> push(h) // nolint:PERF006
                }
                delete haps
            } else {
                var haps <- pat(sub)
                for (h in haps) {
                    result |> push(h) // nolint:PERF006
                }
                delete haps
            }
        }
        return <- result
    }
}

// Loop first fraction of pattern. linger(pat, 0.25) — first quarter looped to fill each cycle.
// Semantics: output(T) = pat(T mod t). Equivalent to pat.zoom(0, t).slow(t).
def linger(var pat : Pattern; t : double) : Pattern {
    //! Loops the first `t`-cycle fraction of `pat` to fill each cycle. Example: `pat |> linger(0.25lf)` loops the first quarter.
    return @capture(= t, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        return <- result if (t <= 0.0lf)
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = floor(sub.start)
            let relStart = sub.start - cycle
            let relStop = sub.stop - cycle
            // iterate each period-t chunk overlapping [relStart, relStop]
            var i = 0
            while (true) {
                let chunkStart = double(i) * t
                break if (chunkStart >= 1.0lf || chunkStart >= relStop)
                let chunkStop = chunkStart + t
                if (chunkStop > relStart) {
                    let overlapStart = max(chunkStart, relStart)
                    let overlapStop = min(min(chunkStop, 1.0lf), relStop)
                    // query pat at the same offset inside [0, t], in this cycle
                    let innerSpan = TimeSpan(
                        start = cycle + (overlapStart - chunkStart),
                        stop = cycle + (overlapStop - chunkStart)
                    )
                    var haps <- pat(innerSpan)
                    for (h in haps) {
                        // shift events by chunkStart so they land in the output iteration
                        result |> push(Hap( // nolint:PERF006
                            whole = TimeSpan(start = h.whole.start + chunkStart, stop = h.whole.stop + chunkStart),
                            part = TimeSpan(start = h.part.start + chunkStart, stop = h.part.stop + chunkStart),
                            has_whole = h.has_whole,
                            value = h.value
                        ))
                    }
                    delete haps
                }
                i ++
            }
        }
        return <- result
    }
}

// Combined fast + speed. hurry(pat, 2.0) speeds up pattern AND sample playback.
def hurry(var pat : Pattern; factor : double) : Pattern {
    //! Combined `fast` + `set_speed` — speeds up both pattern timing AND sample playback by `factor`. Example: `pat |> hurry(2.0lf)`.
    return <- fast(pat, factor) |> set_speed(float(factor))
}

// Squeeze pattern into time window [b,e] within each cycle.
// compress(pat, 0.25, 0.75) — pattern plays in middle half of each cycle
def compress(var pat : Pattern; b, e : double) : Pattern {
    //! Squeezes `pat` into the cycle-time window `[b, e]`. Example: `pat |> compress(0.25lf, 0.75lf)` plays the pattern in the middle half.
    let dur = e - b
    return @capture(= b, = dur, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = floor(sub.start)
            let winStart = cycle + b
            let winStop = cycle + b + dur
            let overStart = max(sub.start, winStart)
            let overStop = min(sub.stop, winStop)
            if (overStart >= overStop) {
                continue
            }
            // map [winStart,winStop] → [cycle,cycle+1]
            let innerStart = cycle + (overStart - winStart) / dur
            let innerStop = cycle + (overStop - winStart) / dur
            var haps <- pat(TimeSpan(start = innerStart, stop = innerStop))
            for (h in haps) {
                // map back: [cycle,cycle+1] → [winStart,winStop]
                result |> push(Hap( // nolint:PERF006
                    whole = TimeSpan(
                        start = (h.whole.start - cycle) * dur + winStart,
                        stop = (h.whole.stop - cycle) * dur + winStart
                    ),
                    part = TimeSpan(
                        start = (h.part.start - cycle) * dur + winStart,
                        stop = (h.part.stop - cycle) * dur + winStart
                    ),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
            delete haps
        }
        return <- result
    }
}

// Apply function to the nth chunk each cycle (rotating).
// chunk(pat, 4, @(x) => fast(x, 2.0lf)) — doubles speed of chunk 0, then 1, then 2, then 3
def chunk(var pat : Pattern; n : int; transform : PatternTransform) : Pattern {
    //! Divides each cycle into `n` chunks, applying `transform` to a different chunk each cycle (rotating). Example: `pat |> chunk(4, @@(x) => fast(x, 2.0lf))`.
    var transformed <- invoke(transform, pat)
    let nf = double(n)
    return @capture(= n, = nf, <- pat, <- transformed) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            var activeChunk = cycle % n
            if (activeChunk < 0) {
                activeChunk += n
            }
            let cyc = floor(sub.start)
            for (i in range(n)) {
                let chunkStart = cyc + double(i) / nf
                let chunkStop = cyc + double(i + 1) / nf
                let overStart = max(sub.start, chunkStart)
                let overStop = min(sub.stop, chunkStop)
                if (overStart >= overStop) {
                    continue
                }
                let querySpan = TimeSpan(start = overStart, stop = overStop)
                if (i == activeChunk) {
                    var haps <- transformed(querySpan)
                    for (h in haps) {
                        result |> push(h) // nolint:PERF006
                    }
                    delete haps
                } else {
                    var haps <- pat(querySpan)
                    for (h in haps) {
                        result |> push(h) // nolint:PERF006
                    }
                    delete haps
                }
            }
        }
        return <- result
    }
}

// Interleave n slices of the pattern across events.
// striate(pat, 4) — cuts each sample into n parts; slice i gets begin=i/n, end=(i+1)/n.
// Behavior: progressive slices reveal later parts of the sample envelope.
def striate(var pat : Pattern; n : int) : Pattern {
    //! Cuts each sample into `n` slices and plays them in sequence. Slice `i` gets `begin=i/n`, `end=(i+1)/n`. Example: `pat |> striate(4)`.
    let nf = double(n)
    let nfinv = 1.0 / float(n)
    return @capture(= n, = nf, = nfinv, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        // speed up pattern by n, then assign each event the begin/end for its slice
        let innerSpan = TimeSpan(start = span.start * nf, stop = span.stop * nf)
        var haps <- pat(innerSpan)
        result |> reserve(length(haps))
        for (h in haps) {
            let step = int(sameCycle(h.whole.start)) % n
            var ev = h.value
            ev.begin = float(step) * nfinv
            ev.end_pos = float(step + 1) * nfinv
            result |> push(Hap(
                whole = TimeSpan(start = h.whole.start / nf, stop = h.whole.stop / nf),
                part = TimeSpan(start = h.part.start / nf, stop = h.part.stop / nf),
                has_whole = h.has_whole,
                value = ev
            ))
        }
        delete haps
        return <- result
    }
}

// Restructure pattern using a boolean mask from mini-notation.
// struct_(pat, s("t f t t")) — play events only where mask is true
def struct_(var pat : Pattern; var bool_pat : Pattern) : Pattern {
    //! Restructures `pat` to the rhythmic positions of `bool_pat`'s truthy haps — values come from `pat`, timing from the mask. Example: `pat |> struct_(s("t f t t"))`.
    return @capture(<- pat, <- bool_pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var mask_haps <- bool_pat(span)
        for (mh in mask_haps) {
            if (!mh.has_whole) {
                continue
            }
            // query inner pattern at mask hap's whole
            var inner_haps <- pat(mh.whole)
            for (ih in inner_haps) {
                let pStart = max(ih.part.start, mh.part.start)
                let pStop = min(ih.part.stop, mh.part.stop)
                if (pStart < pStop) {
                    result |> push(Hap( // nolint:PERF006
                        whole = mh.whole,
                        part = TimeSpan(start = pStart, stop = pStop),
                        has_whole = true,
                        value = ih.value
                    ))
                }
            }
            delete inner_haps
        }
        delete mask_haps
        return <- result
    }
}

// Keep events where mask pattern is active (has events).
def mask(var pat : Pattern; var mask_pat : Pattern) : Pattern {
    //! Keeps only events from `pat` whose timespan overlaps an event in `mask_pat`. Example: `pat |> mask(s("t t f t"))`.
    return @capture(<- pat, <- mask_pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
        for (h in haps) {
            if (!h.has_whole) {
                continue
            }
            // check if mask has any event overlapping this hap
            var mask_haps <- mask_pat(h.whole)
            if (length(mask_haps) > 0) {
                result |> push(h) // nolint:PERF006
            }
            delete mask_haps
        }
        delete haps
        return <- result
    }
}

// Randomly pick one pattern per cycle.
def choose(var pats : array<Pattern>) : Pattern {
    //! Randomly (deterministically) picks one pattern from `pats` per cycle. Example: `choose([s("bd"), s("cp"), s("sd")])`.
    let cn = length(pats)
    return @capture(= cn, <- pats) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            let idx = int(strudel_hash32(double(cycle), 0) % uint(cn))
            var haps <- invoke(pats[idx], sub)
            for (h in haps) {
                result |> push(h) // nolint:PERF006
            }
            delete haps
        }
        return <- result
    }
}

// Weighted random pattern selection per cycle.
def wchoose(var pats : array<Pattern>; var weights : array<float>) : Pattern {
    //! Weighted random pick from `pats` per cycle using parallel `weights` array. Example: `wchoose([s("bd"), s("cp")], [3.0, 1.0])` — `bd` 75% of the time.
    var total_weight = 0.0
    for (w in weights) {
        total_weight += w
    }
    let tw = total_weight
    let cn = length(pats)
    return @capture(= cn, = tw, <- pats, <- weights) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            let r = strudel_hash01(double(cycle), 0) * tw
            var cumulative = 0.0
            var idx = 0
            for (i in range(cn)) {
                cumulative += weights[i]
                if (r < cumulative) {
                    idx = i
                    break
                }
            }
            var haps <- invoke(pats[idx], sub)
            for (h in haps) {
                result |> push(h) // nolint:PERF006
            }
            delete haps
        }
        return <- result
    }
}

// Alias for choose.
def randcat(var pats : array<Pattern>) : Pattern {
    //! Alias for `choose` — randomly picks one pattern per cycle.
    return <- choose(pats)
}

// Alias for choose — chooseCycles (one pick per cycle).
// daStrudel's `choose` already behaves per-cycle; this name exists for the
// `chooseCycles` convention used by some pattern libraries where `choose`
// is a continuous rand signal (glitchy for discrete note patterns).
def chooseCycles(var pats : array<Pattern>) : Pattern {
    //! Alias for `choose` — one random pick per cycle. Present for pattern libraries where `choose` is a continuous rand signal.
    return <- choose(pats)
}

// Deterministic 32-bit RNG for strudel. Given a query time and an index,
// returns a well-mixed uint. Approach:
//   T = floor(t * 2^29)  — ~1.86ns time resolution, overflow-safe for decades
//   decorrelate (T_low, T_high, slot) with Murmur mix constants
//   apply Murmur3 finalizer for full avalanche
// This replaces earlier LCG/single-multiply hashes whose middle bits correlated
// across consecutive slots (the "same-note-in-a-row" bug in scramble/shuffle).
def private strudel_hash32(t : double; slot : int) : uint {
    let Ti = int64(t * 536870912.0lf)
    let lo = uint(Ti)
    let hi = uint(Ti >> 32l)
    var key = lo ^ ((hi ^ 0x85EBCA6Bu) * 0xC2B2AE35u)
    key ^= (uint(slot) ^ 0x7F4A7C15u) * 0x9E3779B9u
    key ^= key >> 16u
    key *= 0x85EBCA6Bu
    key ^= key >> 13u
    key *= 0xC2B2AE35u
    key ^= key >> 16u
    return key
}

// Same hash, mapped to float in [0, 1).
def private strudel_hash01(t : double; slot : int) : float {
    return float(strudel_hash32(t, slot)) / 4294967296.0
}

// Random permutation of n subdivisions per cycle.
def shuffle(var pat : Pattern; n : int) : Pattern {
    //! Randomly permutes `n` equal subdivisions of each cycle (Fisher-Yates, deterministic per cycle). Example: `pat |> shuffle(4)`.
    let nf = double(n)
    return @capture(= n, = nf, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            // Fisher-Yates shuffle using cycle as seed
            var perm : array<int>
            perm |> resize(n)
            for (i in range(n)) {
                perm[i] = i
            }
            var fi = n - 1
            while (fi > 0) {
                let j = int(strudel_hash32(double(cycle), fi) % uint(fi + 1))
                let tmp = perm[fi]
                perm[fi] = perm[j]
                perm[j] = tmp
                fi --
            }
            let cyc = floor(sub.start)
            for (i in range(n)) {
                let slotStart = cyc + double(i) / nf
                let slotStop = cyc + double(i + 1) / nf
                let overStart = max(sub.start, slotStart)
                let overStop = min(sub.stop, slotStop)
                if (overStart >= overStop) {
                    continue
                }
                // query the perm[i]-th subdivision
                let srcStart = cyc + double(perm[i]) / nf
                let srcStop = cyc + double(perm[i] + 1) / nf
                var haps <- pat(TimeSpan(start = srcStart, stop = srcStop))
                for (h in haps) {
                    // remap to destination slot
                    let shift = slotStart - srcStart
                    result |> push(Hap( // nolint:PERF006
                        whole = TimeSpan(start = h.whole.start + shift, stop = h.whole.stop + shift),
                        part = TimeSpan(start = max(h.part.start + shift, overStart), stop = min(h.part.stop + shift, overStop)),
                        has_whole = h.has_whole,
                        value = h.value
                    ))
                }
                delete haps
            }
        }
        return <- result
    }
}

// Shuffle with replacement — each slot picks a random subdivision.
def scramble(var pat : Pattern; n : int) : Pattern {
    //! Like `shuffle` but with replacement — each of the `n` slots independently picks a random subdivision. Example: `pat |> scramble(8)`.
    let nf = double(n)
    return @capture(= n, = nf, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var inscope split = splitSpans(span)
        for (sub in split) {
            let cycle = int(sameCycle(sub.start))
            let cyc = floor(sub.start)
            for (i in range(n)) {
                let slotStart = cyc + double(i) / nf
                let slotStop = cyc + double(i + 1) / nf
                let overStart = max(sub.start, slotStart)
                let overStop = min(sub.stop, slotStop)
                if (overStart >= overStop) {
                    continue
                }
                let srcIdx = int(strudel_hash32(double(cycle), i) % uint(n))
                let srcStart = cyc + double(srcIdx) / nf
                let srcStop = cyc + double(srcIdx + 1) / nf
                var haps <- pat(TimeSpan(start = srcStart, stop = srcStop))
                for (h in haps) {
                    let shift = slotStart - srcStart
                    result |> push(Hap( // nolint:PERF006
                        whole = TimeSpan(start = h.whole.start + shift, stop = h.whole.stop + shift),
                        part = TimeSpan(start = max(h.part.start + shift, overStart), stop = min(h.part.stop + shift, overStop)),
                        has_whole = h.has_whole,
                        value = h.value
                    ))
                }
                delete haps
            }
        }
        return <- result
    }
}

// ─── Additional Signals ───

// Square wave 0..1. First half = 1.0, second half = 0.0.
def private signal_square() : Pattern {
    return signal(@(t : double) : float {
        let pos = t - floor(t)
        return pos < 0.5lf ? 1.0 : 0.0
    })
}

// Inverted sawtooth 0..1. Falls from 1.0 to 0.0 each cycle.
def private signal_isaw() : Pattern {
    return signal(@(t : double) : float {
        return 1.0 - float(t - floor(t))
    })
}

// Inverted triangle 0..1. Starts at 1.0, falls to 0.0 at midpoint, returns to 1.0.
def private signal_itri() : Pattern {
    return signal(@(t : double) : float {
        let pos = float(t - floor(t))
        return pos < 0.5 ? 1.0 - pos * 2.0 : (pos - 0.5) * 2.0
    })
}

// Bipolar sine -1..1
def private signal_sine2() : Pattern {
    return signal(@(t : double) : float {
        return float(sin(3.14159265358979323846lf * 2.0lf * t))
    })
}

// Bipolar cosine -1..1
def private signal_cosine2() : Pattern {
    return signal(@(t : double) : float {
        return float(cos(3.14159265358979323846lf * 2.0lf * t))
    })
}

// Bipolar sawtooth -1..1
def private signal_saw2() : Pattern {
    return signal(@(t : double) : float {
        return float(t - floor(t)) * 2.0 - 1.0
    })
}

// Bipolar triangle -1..1
def private signal_tri2() : Pattern {
    return signal(@(t : double) : float {
        let pos = float(t - floor(t))
        return pos < 0.5 ? pos * 4.0 - 1.0 : 3.0 - pos * 4.0
    })
}

// Bipolar square -1..1
def private signal_square2() : Pattern {
    return signal(@(t : double) : float {
        let pos = t - floor(t)
        return pos < 0.5lf ? 1.0 : -1.0
    })
}

// Bipolar inverted sawtooth -1..1
def private signal_isaw2() : Pattern {
    return signal(@(t : double) : float {
        return 1.0 - float(t - floor(t)) * 2.0
    })
}

// Bipolar inverted triangle -1..1
def private signal_itri2() : Pattern {
    return signal(@(t : double) : float {
        let pos = float(t - floor(t))
        return pos < 0.5 ? 1.0 - pos * 4.0 : pos * 4.0 - 3.0
    })
}

// Random 0..1 — deterministic per query time (hash-based).
def private signal_rand() : Pattern {
    return signal(@(t : double) : float {
        return strudel_hash01(t, 0)
    })
}

// Random -1..1
def private signal_rand2() : Pattern {
    return signal(@(t : double) : float {
        return strudel_hash01(t, 0) * 2.0 - 1.0
    })
}

// Random integer 0..n-1
def private signal_irand(n : int) : Pattern {
    return signal(@capture(= n) (t : double) : float {
        return float(strudel_hash32(t, 0) % uint(n))
    })
}

// Discrete ramp 0..n-1 within each cycle.
def private signal_run(n : int) : Pattern {
    let nf = double(n)
    return @capture(= n, = nf) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        let cycle = floor(span.start)
        for (i in range(n)) {
            let start = cycle + double(i) / nf
            let stop = cycle + double(i + 1) / nf
            if (stop > span.start && start < span.stop) {
                let pStart = max(start, span.start)
                let pStop = min(stop, span.stop)
                result |> push(Hap(
                    whole = TimeSpan(start = start, stop = stop),
                    part = TimeSpan(start = pStart, stop = pStop),
                    has_whole = true,
                    value = Event(note = float(i))
                ))
            }
        }
        return <- result
    }
}

// ─── Bare-Name Aliases ───
// Shorter names for chainable pattern combinators.
// Old set_* names remain valid.

// ── Scalar parameter aliases ──

def gain(var pat : Pattern; g : float) : Pattern {
    //! Fluent shorthand for `set_gain`. Sets amplitude (0..1) on every event.
    return <- set_gain(pat, g)
}
def velocity(var pat : Pattern; v : float) : Pattern {
    //! Fluent shorthand for `set_velocity`. Sets MIDI-like velocity (0..1) on every event.
    return <- set_velocity(pat, v)
}
def vel(var pat : Pattern; v : float) : Pattern {
    //! Alias for `velocity`.
    return <- set_velocity(pat, v)
}
def pan(var pat : Pattern; p : float) : Pattern {
    //! Fluent shorthand for `set_pan`. Sets stereo position (0=L, 0.5=C, 1=R).
    return <- set_pan(pat, p)
}
def hrtf_azimuth(var pat : Pattern; deg : float) : Pattern {
    //! Fluent shorthand for `set_hrtf_azimuth`. Engages HRTF positioning; `pan` still controls source width.
    return <- set_hrtf_azimuth(pat, deg)
}
def hrtf_elevation(var pat : Pattern; deg : float) : Pattern {
    //! Fluent shorthand for `set_hrtf_elevation`. Engages HRTF positioning; `pan` still controls source width.
    return <- set_hrtf_elevation(pat, deg)
}
def hrtf(var pat : Pattern; az, el : float) : Pattern {
    //! Fluent shorthand for `set_hrtf` — sets HRTF azimuth + elevation and engages 3D positioning;
    //! `pan` still controls source width.
    return <- set_hrtf(pat, az, el)
}
def speed(var pat : Pattern; spd : float) : Pattern {
    //! Fluent shorthand for `set_speed`. Sets sample playback speed.
    return <- set_speed(pat, spd)
}
def note(var pat : Pattern; n : float) : Pattern {
    //! Fluent shorthand for `set_note`. Sets MIDI note number on every event.
    return <- set_note(pat, n)
}
def sound(var pat : Pattern; snd : string) : Pattern {
    //! Fluent shorthand for `set_sound`. Sets the sound name.
    return <- set_sound(pat, snd)
}
def cut(var pat : Pattern; c : int) : Pattern {
    //! Fluent shorthand for `set_cut`. Sets the cut group (new voice kills previous in same group).
    return <- set_cut(pat, c)
}
def room(var pat : Pattern; r : float) : Pattern {
    //! Fluent shorthand for `set_room`. Sets reverb send amount (0..1).
    return <- set_room(pat, r)
}
def roomsize(var pat : Pattern; r : float) : Pattern {
    //! Fluent shorthand for `set_roomsize`. Sets reverb room size.
    return <- set_roomsize(pat, r)
}
def size(var pat : Pattern; r : float) : Pattern {
    //! Alias for `roomsize`.
    return <- set_roomsize(pat, r)
}
def delay(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_delay`. Sets delay send amount (0..1).
    return <- set_delay(pat, d)
}
def delaytime(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_delaytime`. Sets delay time in seconds.
    return <- set_delaytime(pat, d)
}
def dt(var pat : Pattern; d : float) : Pattern {
    //! Alias for `delaytime`.
    return <- set_delaytime(pat, d)
}
def delayfeedback(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_delayfeedback`. Sets delay feedback (0..<1).
    return <- set_delayfeedback(pat, d)
}
def dfb(var pat : Pattern; d : float) : Pattern {
    //! Alias for `delayfeedback`.
    return <- set_delayfeedback(pat, d)
}
def delaysync(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_delaysync`. Sets delay time in cycles (tempo-synced delay).
    return <- set_delaysync(pat, d)
}
def orbit(var pat : Pattern; o : int) : Pattern {
    //! Fluent shorthand for `set_orbit`. Sets the effect-bus routing index.
    return <- set_orbit(pat, o)
}
def fm(var pat : Pattern; f : float) : Pattern {
    //! Fluent shorthand for `set_fm`. Sets FM modulation depth.
    return <- set_fm(pat, f)
}
def fmh(var pat : Pattern; f : float) : Pattern {
    //! Fluent shorthand for `set_fmh`. Sets FM harmonicity ratio.
    return <- set_fmh(pat, f)
}
def lpf(var pat : Pattern; freq : float) : Pattern {
    //! Fluent shorthand for `set_lpf`. Sets low-pass cutoff (Hz).
    return <- set_lpf(pat, freq)
}
def lp(var pat : Pattern; freq : float) : Pattern {
    //! Alias for `lpf`.
    return <- set_lpf(pat, freq)
}
def hpf(var pat : Pattern; freq : float) : Pattern {
    //! Fluent shorthand for `set_hpf`. Sets high-pass cutoff (Hz).
    return <- set_hpf(pat, freq)
}
def hp(var pat : Pattern; freq : float) : Pattern {
    //! Alias for `hpf`.
    return <- set_hpf(pat, freq)
}
def lpq(var pat : Pattern; q : float) : Pattern {
    //! Fluent shorthand for `set_lpq`. Sets low-pass filter resonance.
    return <- set_lpq(pat, q)
}
def resonance(var pat : Pattern; q : float) : Pattern {
    //! Alias for `lpq`.
    return <- set_lpq(pat, q)
}
def hpq(var pat : Pattern; q : float) : Pattern {
    //! Fluent shorthand for `set_hpq`. Sets high-pass filter resonance.
    return <- set_hpq(pat, q)
}
def attack(var pat : Pattern; a : float) : Pattern {
    //! Fluent shorthand for `set_attack`. Sets envelope attack (seconds).
    return <- set_attack(pat, a)
}
def att(var pat : Pattern; a : float) : Pattern {
    //! Alias for `attack`.
    return <- set_attack(pat, a)
}
def decay(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_decay`. Sets envelope decay (seconds).
    return <- set_decay(pat, d)
}
def dec(var pat : Pattern; d : float) : Pattern {
    //! Alias for `decay`.
    return <- set_decay(pat, d)
}
def sustain(var pat : Pattern; s : float) : Pattern {
    //! Fluent shorthand for `set_sustain`. Sets envelope sustain level (0..1).
    return <- set_sustain(pat, s)
}
def sus(var pat : Pattern; s : float) : Pattern {
    //! Alias for `sustain`.
    return <- set_sustain(pat, s)
}
def release(var pat : Pattern; r : float) : Pattern {
    //! Fluent shorthand for `set_release`. Sets envelope release (seconds).
    return <- set_release(pat, r)
}
def rel(var pat : Pattern; r : float) : Pattern {
    //! Alias for `release`.
    return <- set_release(pat, r)
}
def chorus(var pat : Pattern; c : float) : Pattern {
    //! Fluent shorthand for `set_chorus`. Sets chorus send amount (0..1).
    return <- set_chorus(pat, c)
}

// ── Sample region & effects aliases ──

def begin(var pat : Pattern; b : float) : Pattern {
    //! Fluent shorthand for `set_begin`. Sets sample start position (0..1).
    return <- set_begin(pat, b)
}
def end_pos(var pat : Pattern; e_val : float) : Pattern {
    //! Fluent shorthand for `set_end`. Sets sample end position (0..1).
    return <- set_end(pat, e_val)
}
def crush(var pat : Pattern; c : float) : Pattern {
    //! Fluent shorthand for `set_crush`. Sets bit-crush amount.
    return <- set_crush(pat, c)
}
def coarse(var pat : Pattern; c : int) : Pattern {
    //! Fluent shorthand for `set_coarse`. Sets sample-rate reduction factor.
    return <- set_coarse(pat, c)
}
def shape(var pat : Pattern; s : float) : Pattern {
    //! Fluent shorthand for `set_shape`. Sets waveshape/distortion amount.
    return <- set_shape(pat, s)
}
def djf(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_djf`. Sets DJ-filter position (<0.5 = LP, >0.5 = HP).
    return <- set_djf(pat, d)
}
def bpf(var pat : Pattern; freq : float) : Pattern {
    //! Fluent shorthand for `set_bpf`. Sets band-pass filter cutoff (Hz).
    return <- set_bpf(pat, freq)
}
def bpq(var pat : Pattern; q : float) : Pattern {
    //! Fluent shorthand for `set_bpq`. Sets band-pass filter Q.
    return <- set_bpq(pat, q)
}

// ── Phaser / Tremolo / Compressor aliases ──

def phaser(var pat : Pattern; r : float) : Pattern {
    //! Fluent shorthand for `set_phaser`. Sets phaser rate in Hz (0 disables).
    return <- set_phaser(pat, r)
}
def ph(var pat : Pattern; r : float) : Pattern {
    //! Alias for `phaser`.
    return <- set_phaser(pat, r)
}
def phaserdepth(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_phaserdepth`. Sets phaser depth.
    return <- set_phaserdepth(pat, d)
}
def phd(var pat : Pattern; d : float) : Pattern {
    //! Alias for `phaserdepth`.
    return <- set_phaserdepth(pat, d)
}
def phasercenter(var pat : Pattern; c : float) : Pattern {
    //! Fluent shorthand for `set_phasercenter`. Sets phaser center frequency.
    return <- set_phasercenter(pat, c)
}
def phc(var pat : Pattern; c : float) : Pattern {
    //! Alias for `phasercenter`.
    return <- set_phasercenter(pat, c)
}
def phasersweep(var pat : Pattern; s : float) : Pattern {
    //! Fluent shorthand for `set_phasersweep`. Sets phaser sweep range.
    return <- set_phasersweep(pat, s)
}
def phs(var pat : Pattern; s : float) : Pattern {
    //! Alias for `phasersweep`.
    return <- set_phasersweep(pat, s)
}

def tremolo(var pat : Pattern; r : float) : Pattern {
    //! Fluent shorthand for `set_tremolo`. Sets tremolo rate in Hz (0 disables).
    return <- set_tremolo(pat, r)
}
def trem(var pat : Pattern; r : float) : Pattern {
    //! Alias for `tremolo`.
    return <- set_tremolo(pat, r)
}
def tremolodepth(var pat : Pattern; d : float) : Pattern {
    //! Fluent shorthand for `set_tremolodepth`. Sets tremolo depth.
    return <- set_tremolodepth(pat, d)
}
def tremdepth(var pat : Pattern; d : float) : Pattern {
    //! Alias for `tremolodepth`.
    return <- set_tremolodepth(pat, d)
}

def compressor(var pat : Pattern; thr_db : float) : Pattern {
    //! Fluent shorthand for `set_compressor`. Sets compressor threshold in dB (>=0 bypasses).
    return <- set_compressor(pat, thr_db)
}
// camelCase aliases; also expose snake_case for daScript-idiomatic callers.
def compressorRatio(var pat : Pattern; r : float) : Pattern {
    //! camelCase fluent shorthand for `set_compressor_ratio`. Sets compressor ratio.
    return <- set_compressor_ratio(pat, r)
}
def compressor_ratio(var pat : Pattern; r : float) : Pattern {
    //! snake_case fluent shorthand for `set_compressor_ratio`. Sets compressor ratio.
    return <- set_compressor_ratio(pat, r)
}
def compressorKnee(var pat : Pattern; k : float) : Pattern {
    //! camelCase fluent shorthand for `set_compressor_knee`. Sets compressor knee (dB).
    return <- set_compressor_knee(pat, k)
}
def compressor_knee(var pat : Pattern; k : float) : Pattern {
    //! snake_case fluent shorthand for `set_compressor_knee`. Sets compressor knee (dB).
    return <- set_compressor_knee(pat, k)
}
def compressorAttack(var pat : Pattern; a : float) : Pattern {
    //! camelCase fluent shorthand for `set_compressor_attack`. Sets compressor attack (seconds).
    return <- set_compressor_attack(pat, a)
}
def compressor_attack(var pat : Pattern; a : float) : Pattern {
    //! snake_case fluent shorthand for `set_compressor_attack`. Sets compressor attack (seconds).
    return <- set_compressor_attack(pat, a)
}
def compressorRelease(var pat : Pattern; r : float) : Pattern {
    //! camelCase fluent shorthand for `set_compressor_release`. Sets compressor release (seconds).
    return <- set_compressor_release(pat, r)
}
def compressor_release(var pat : Pattern; r : float) : Pattern {
    //! snake_case fluent shorthand for `set_compressor_release`. Sets compressor release (seconds).
    return <- set_compressor_release(pat, r)
}

// ── SF2 aliases ──

def sf2(var pat : Pattern; program : int) : Pattern {
    //! Fluent shorthand for `set_sf2`. Sets SF2 GM program number (0..127).
    return <- set_sf2(pat, program)
}
def sf2(var pat : Pattern; name : string) : Pattern {
    //! Fluent shorthand for `set_sf2` by GM instrument name. Example: `pat |> sf2("piano")`.
    return <- set_sf2(pat, name)
}
def sf2_bank(var pat : Pattern; bank : int) : Pattern {
    //! Fluent shorthand for `set_sf2_bank`. Sets SF2 bank (0 = melodic, 128 = percussion).
    return <- set_sf2_bank(pat, bank)
}
def sf2_expression(var pat : Pattern; v : float) : Pattern {
    //! Fluent shorthand for `set_sf2_expression`. Sets SF2 expression CC11 (0..1).
    return <- set_sf2_expression(pat, v)
}
def sf2_mod_wheel(var pat : Pattern; v : float) : Pattern {
    //! Fluent shorthand for `set_sf2_mod_wheel`. Sets SF2 mod wheel CC1 (0..1, vibrato depth).
    return <- set_sf2_mod_wheel(pat, v)
}
def sf2_pitch_bend(var pat : Pattern; v : float) : Pattern {
    //! Fluent shorthand for `set_sf2_pitch_bend`. Sets SF2 pitch bend (0..1, 0.5 = center).
    return <- set_sf2_pitch_bend(pat, v)
}

// ── Pattern-modulated parameter aliases ──

def gain(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `gain` — sampled from `mod_pat` per event.
    return <- set_gain(pat, mod_pat)
}
def velocity(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `velocity` — sampled from `mod_pat` per event.
    return <- set_velocity(pat, mod_pat)
}
def vel(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `velocity`.
    return <- set_velocity(pat, mod_pat)
}
def pan(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `pan` — sampled from `mod_pat` per event.
    return <- set_pan(pat, mod_pat)
}
def hrtf_azimuth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated HRTF azimuth — sampled from `mod_pat` per event. Engages HRTF positioning.
    return <- set_hrtf_azimuth(pat, mod_pat)
}
def hrtf_elevation(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated HRTF elevation — sampled from `mod_pat` per event. Engages HRTF positioning.
    return <- set_hrtf_elevation(pat, mod_pat)
}
def speed(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `speed` — sampled from `mod_pat` per event.
    return <- set_speed(pat, mod_pat)
}
def note(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `note` — sampled from `mod_pat` per event.
    return <- set_note(pat, mod_pat)
}
def sound(var pat : Pattern; var sound_pat : Pattern) : Pattern {
    //! Pattern-valued `sound` — sound name sampled from `sound_pat.s` per event. Example: `note("c3") |> sound(s("<saw tri>"))`.
    return <- set_sound(pat, sound_pat)
}
def room(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `room` — sampled from `mod_pat` per event.
    return <- set_room(pat, mod_pat)
}
def roomsize(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `roomsize` — sampled from `mod_pat` per event.
    return <- set_roomsize(pat, mod_pat)
}
def size(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `roomsize`.
    return <- set_roomsize(pat, mod_pat)
}
def delay(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `delay` send — sampled from `mod_pat` per event.
    return <- set_delay(pat, mod_pat)
}
def delaytime(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `delaytime` — sampled from `mod_pat` per event.
    return <- set_delaytime(pat, mod_pat)
}
def dt(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `delaytime`.
    return <- set_delaytime(pat, mod_pat)
}
def delayfeedback(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `delayfeedback` — sampled from `mod_pat` per event.
    return <- set_delayfeedback(pat, mod_pat)
}
def dfb(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `delayfeedback`.
    return <- set_delayfeedback(pat, mod_pat)
}
def fm(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated FM depth — sampled from `mod_pat` per event.
    return <- set_fm(pat, mod_pat)
}
def fmh(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated FM harmonicity — sampled from `mod_pat` per event.
    return <- set_fmh(pat, mod_pat)
}
def lpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `lpf` cutoff — sampled from `mod_pat` per event.
    return <- set_lpf(pat, mod_pat)
}
def lp(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `lpf`.
    return <- set_lpf(pat, mod_pat)
}
def hpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `hpf` cutoff — sampled from `mod_pat` per event.
    return <- set_hpf(pat, mod_pat)
}
def hp(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `hpf`.
    return <- set_hpf(pat, mod_pat)
}
def lpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `lpq` resonance — sampled from `mod_pat` per event.
    return <- set_lpq(pat, mod_pat)
}
def resonance(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `lpq`.
    return <- set_lpq(pat, mod_pat)
}
def hpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated `hpq` resonance — sampled from `mod_pat` per event.
    return <- set_hpq(pat, mod_pat)
}
def attack(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated envelope `attack` — sampled from `mod_pat` per event.
    return <- set_attack(pat, mod_pat)
}
def att(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `attack`.
    return <- set_attack(pat, mod_pat)
}
def decay(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated envelope `decay` — sampled from `mod_pat` per event.
    return <- set_decay(pat, mod_pat)
}
def dec(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `decay`.
    return <- set_decay(pat, mod_pat)
}
def sustain(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated envelope `sustain` — sampled from `mod_pat` per event.
    return <- set_sustain(pat, mod_pat)
}
def sus(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `sustain`.
    return <- set_sustain(pat, mod_pat)
}
def release(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated envelope `release` — sampled from `mod_pat` per event.
    return <- set_release(pat, mod_pat)
}
def rel(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `release`.
    return <- set_release(pat, mod_pat)
}
def chorus(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated chorus send — sampled from `mod_pat` per event.
    return <- set_chorus(pat, mod_pat)
}
def sf2_expression(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated SF2 expression (CC11) — sampled from `mod_pat` per event.
    return <- set_sf2_expression(pat, mod_pat)
}
def sf2_mod_wheel(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated SF2 mod wheel (CC1) — sampled from `mod_pat` per event.
    return <- set_sf2_mod_wheel(pat, mod_pat)
}
def sf2_pitch_bend(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated SF2 pitch bend — sampled from `mod_pat` per event.
    return <- set_sf2_pitch_bend(pat, mod_pat)
}
def begin(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated sample `begin` — sampled from `mod_pat` per event.
    return <- set_begin(pat, mod_pat)
}
def end_pos(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated sample `end_pos` — sampled from `mod_pat` per event.
    return <- set_end(pat, mod_pat)
}
def crush(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated bit-`crush` — sampled from `mod_pat` per event.
    return <- set_crush(pat, mod_pat)
}
def shape(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated wave-`shape` — sampled from `mod_pat` per event.
    return <- set_shape(pat, mod_pat)
}
def djf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated DJ-filter `djf` — sampled from `mod_pat` per event.
    return <- set_djf(pat, mod_pat)
}
def bpf(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated band-pass cutoff `bpf` — sampled from `mod_pat` per event.
    return <- set_bpf(pat, mod_pat)
}
def bpq(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated band-pass Q `bpq` — sampled from `mod_pat` per event.
    return <- set_bpq(pat, mod_pat)
}
def phaser(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated phaser rate — sampled from `mod_pat` per event.
    return <- set_phaser(pat, mod_pat)
}
def ph(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `phaser`.
    return <- set_phaser(pat, mod_pat)
}
def phaserdepth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated phaser depth — sampled from `mod_pat` per event.
    return <- set_phaserdepth(pat, mod_pat)
}
def phd(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `phaserdepth`.
    return <- set_phaserdepth(pat, mod_pat)
}
def phasercenter(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated phaser center frequency — sampled from `mod_pat` per event.
    return <- set_phasercenter(pat, mod_pat)
}
def phc(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `phasercenter`.
    return <- set_phasercenter(pat, mod_pat)
}
def phasersweep(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated phaser sweep — sampled from `mod_pat` per event.
    return <- set_phasersweep(pat, mod_pat)
}
def phs(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `phasersweep`.
    return <- set_phasersweep(pat, mod_pat)
}
def tremolo(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated tremolo rate — sampled from `mod_pat` per event.
    return <- set_tremolo(pat, mod_pat)
}
def trem(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `tremolo`.
    return <- set_tremolo(pat, mod_pat)
}
def tremolodepth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated tremolo depth — sampled from `mod_pat` per event.
    return <- set_tremolodepth(pat, mod_pat)
}
def tremdepth(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Alias for pattern-modulated `tremolodepth`.
    return <- set_tremolodepth(pat, mod_pat)
}
def compressor(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor threshold — sampled from `mod_pat` per event.
    return <- set_compressor(pat, mod_pat)
}
def compressorRatio(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor ratio (camelCase) — sampled from `mod_pat` per event.
    return <- set_compressor_ratio(pat, mod_pat)
}
def compressor_ratio(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor ratio (snake_case) — sampled from `mod_pat` per event.
    return <- set_compressor_ratio(pat, mod_pat)
}
def compressorKnee(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor knee (camelCase) — sampled from `mod_pat` per event.
    return <- set_compressor_knee(pat, mod_pat)
}
def compressor_knee(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor knee (snake_case) — sampled from `mod_pat` per event.
    return <- set_compressor_knee(pat, mod_pat)
}
def compressorAttack(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor attack (camelCase) — sampled from `mod_pat` per event.
    return <- set_compressor_attack(pat, mod_pat)
}
def compressor_attack(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor attack (snake_case) — sampled from `mod_pat` per event.
    return <- set_compressor_attack(pat, mod_pat)
}
def compressorRelease(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor release (camelCase) — sampled from `mod_pat` per event.
    return <- set_compressor_release(pat, mod_pat)
}
def compressor_release(var pat : Pattern; var mod_pat : Pattern) : Pattern {
    //! Pattern-modulated compressor release (snake_case) — sampled from `mod_pat` per event.
    return <- set_compressor_release(pat, mod_pat)
}

// ── Signal aliases ──

def sine() : Pattern {
    //! Unipolar sine signal `0..1`. Alias for `signal_sine`.
    return <- signal_sine()
}
def cosine() : Pattern {
    //! Unipolar cosine signal `0..1`. Alias for `signal_cosine`.
    return <- signal_cosine()
}
def saw() : Pattern {
    //! Unipolar sawtooth signal `0..1`. Alias for `signal_saw`.
    return <- signal_saw()
}
def tri() : Pattern {
    //! Unipolar triangle signal `0..1`. Alias for `signal_tri`.
    return <- signal_tri()
}
def perlin() : Pattern {
    //! Unipolar Perlin-noise signal `0..1`. Alias for `signal_perlin`.
    return <- signal_perlin()
}
def range(var pat : Pattern; lo, hi : float) : Pattern {
    //! Scales a signal pattern from `0..1` to `lo..hi`. Alias for `signal_range`. Example: `sine() |> range(200.0, 4000.0)`.
    return <- signal_range(pat, lo, hi)
}
def square() : Pattern {
    //! Unipolar square signal (1.0 first half, 0.0 second half).
    return <- signal_square()
}
def isaw() : Pattern {
    //! Inverted unipolar sawtooth signal — falls from 1.0 to 0.0 each cycle.
    return <- signal_isaw()
}
def itri() : Pattern {
    //! Inverted unipolar triangle signal — starts at 1.0, dips to 0.0 at midpoint.
    return <- signal_itri()
}
def sine2() : Pattern {
    //! Bipolar sine signal `-1..1`.
    return <- signal_sine2()
}
def cosine2() : Pattern {
    //! Bipolar cosine signal `-1..1`.
    return <- signal_cosine2()
}
def saw2() : Pattern {
    //! Bipolar sawtooth signal `-1..1`.
    return <- signal_saw2()
}
def tri2() : Pattern {
    //! Bipolar triangle signal `-1..1`.
    return <- signal_tri2()
}
def square2() : Pattern {
    //! Bipolar square signal `-1..1`.
    return <- signal_square2()
}
def isaw2() : Pattern {
    //! Bipolar inverted sawtooth signal `-1..1`.
    return <- signal_isaw2()
}
def itri2() : Pattern {
    //! Bipolar inverted triangle signal `-1..1`.
    return <- signal_itri2()
}
def rand() : Pattern {
    //! Unipolar deterministic random signal `0..1` — hash-based, same time yields same value.
    return <- signal_rand()
}
def rand2() : Pattern {
    //! Bipolar deterministic random signal `-1..1` — hash-based.
    return <- signal_rand2()
}
def irand(n : int) : Pattern {
    //! Random integer signal in `0..n-1`, deterministic.
    return <- signal_irand(n)
}
def run(n : int) : Pattern {
    //! Discrete ramp `0..n-1` within each cycle — `n` haps per cycle with values `0, 1, ..., n-1`.
    return <- signal_run(n)
}

// ── Additional aliases ──

def degradeBy(var pat : Pattern; prob : float) : Pattern {
    //! Alias for `degrade` with explicit probability. Randomly drops events with probability `prob`. Example: `pat |> degradeBy(0.3)`.
    return <- degrade(pat, prob)
}
