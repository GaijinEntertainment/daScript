options gen2
options indenting = 4

module strudel_pattern shared

require strudel/strudel_event
require strudel/strudel_time
require math

// A Pattern is a function from time span to events.
// Patterns are pure — no side effects, no audio, just data.
typedef Pattern = lambda<(span : TimeSpan) : array<Hap>>

// ─── Constructors ───

// A single event spanning every cycle.
def pure(value : Event) : Pattern {
    return @capture(= value) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        for (sub in splitSpans(span)) {
            let cyc = sameCycle(sub.start)
            let whole = TimeSpan(start = cyc, stop = cyc + 1.0lf)
            result |> push(Hap(whole = whole, part = sub, value = value))
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
def fast(n : double; var pat : Pattern) : Pattern {
    return @capture(= n, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        let innerSpan = TimeSpan(start = span.start * n, stop = span.stop * n)
        var haps <- pat(innerSpan)
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
def slow(n : double; var pat : Pattern) : Pattern {
    return fast(1.0lf / n, pat)
}

// Shift pattern earlier in time by `offset` cycles.
def early(offset : double; var pat : Pattern) : Pattern {
    return @capture(= offset, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        let innerSpan = TimeSpan(start = span.start + offset, stop = span.stop + offset)
        var haps <- pat(innerSpan)
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
def late(offset : double; var pat : Pattern) : Pattern {
    return early(0.0lf - offset, pat)
}

// ─── Composition ───

// Layer patterns on top of each other (simultaneous).
def stack(var pats : array<Pattern>) : Pattern {
    return @capture(<- pats) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        for (pat in pats) {
            var haps <- pat(span)
            for (h in haps) {
                result |> push(h)
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
        for (sub in splitSpans(span)) {
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
                result |> push(Hap(
                    whole = TimeSpan(start = h.whole.start + cycle, stop = h.whole.stop + cycle),
                    part = TimeSpan(start = h.part.start + cycle, stop = h.part.stop + cycle),
                    has_whole = h.has_whole,
                    value = h.value
                ))
            }
        }
        return <- result
    }
}

// Like cat but all patterns squeezed into one cycle.
def fastcat(var pats : array<Pattern>) : Pattern {
    let cn = double(length(pats))
    return fast(cn, cat(pats))
}

// ─── Mapping ───

// Transform the value of every event.
def fmap(var fn : lambda<(e : Event) : Event>; var pat : Pattern) : Pattern {
    return @capture(<- fn, <- pat) (span : TimeSpan) : array<Hap> {
        var result : array<Hap>
        var haps <- pat(span)
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
def degrade(prob : float; var pat : Pattern) : Pattern {
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
        for (sub in splitSpans(span)) {
            let cycle = int(sameCycle(sub.start))
            var idx = cycle % cn
            if (idx < 0) {
                idx += cn
            }
            if (idx == 0) {
                for (h in pat_on(sub)) {
                    result |> push(h)
                }
            } else {
                for (h in pat_off(sub)) {
                    result |> push(h)
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

// set_gain(0.5, pat) — set amplitude
def set_gain(g : float; var pat : Pattern) : Pattern {
    var fn <- @capture(= g) (e : Event) : Event {
        var r = e; r.gain = g; return r
    }
    return <- fmap(fn, pat)
}

// set_velocity(0.5, pat) — set expression/velocity
def set_velocity(v : float; var pat : Pattern) : Pattern {
    var fn <- @capture(= v) (e : Event) : Event {
        var r = e; r.velocity = v; return r
    }
    return <- fmap(fn, pat)
}

// set_pan(0.0, pat) — set stereo position (0=left, 0.5=center, 1=right)
def set_pan(p : float; var pat : Pattern) : Pattern {
    var fn <- @capture(= p) (e : Event) : Event {
        var r = e; r.pan = p; return r
    }
    return <- fmap(fn, pat)
}

// set_speed(2.0, pat) — set sample playback speed
def set_speed(spd : float; var pat : Pattern) : Pattern {
    var fn <- @capture(= spd) (e : Event) : Event {
        var r = e; r.speed = spd; return r
    }
    return <- fmap(fn, pat)
}

// set_note(48.0, pat) — set MIDI note number
def set_note(n : float; var pat : Pattern) : Pattern {
    var fn <- @capture(= n) (e : Event) : Event {
        var r = e; r.note = n; return r
    }
    return <- fmap(fn, pat)
}

// set_sound("sawtooth", pat) — change the sound name
def set_sound(snd : string; var pat : Pattern) : Pattern {
    var fn <- @capture(= snd) (e : Event) : Event {
        var r = e; r.s = snd; return r
    }
    return <- fmap(fn, pat)
}

// set_cut(1, pat) — set cut group (new voice kills previous in same group)
def set_cut(c : int; var pat : Pattern) : Pattern {
    var fn <- @capture(= c) (e : Event) : Event {
        var r = e; r.cut = c; return r
    }
    return <- fmap(fn, pat)
}

// ─── Euclidean Rhythms ───

// Euclidean rhythm: play pattern at k evenly-spaced onsets across n subdivisions.
// euclid(3, 8, atom("bd")) → kick on steps 0, 3, 6 of an 8-step cycle
def euclid(k, n : int; var pat : Pattern) : Pattern {
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
