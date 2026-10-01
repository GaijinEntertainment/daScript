options gen2
options indenting = 4
options _dasllama_internal = true

module _race_fixture shared public

//! The timing-race scaffold the harness races share: the interleaved round loop, the unwritten-output
//! poison, the held-to-a-reference compare and its wording, the per-arm statistics, and the refusal
//! tally that decides the exit code.

require math
require ../tests/_compares.das   // nan_fill: the suites' NaN sentinel, an element the walk leaves reading outside every bound

typedef Held = tuple<diff : float; bound : float; ok : bool>

//! the rows a race reported and the arms or shapes it refused
struct RaceTally {
    rows : int
    bad : int
}

//! `y` refilled with the NaN sentinel before an arm's walk
def poison(var y : array<float>) {
    nan_fill(y)
}

//! `rounds + warmup` rounds of every arm in turn, `before(i)` untimed ahead of each timed `arm(i)`;
//! the first `warmup` rounds are dropped. Answers each arm's round times in microseconds.
def time_rounds(narms, rounds, warmup : int; before : block<(i : int) : void>; arm : block<(i : int) : void>) : array<array<int>> {
    var times : array<array<int>>
    times |> resize(narms)
    for (round in range(rounds + warmup)) {
        for (i in range(narms)) {
            invoke(before, i)
            let t0 = ref_time_ticks()
            invoke(arm, i)
            let us = get_time_usec(t0)
            if (round >= warmup) {
                times[i] |> push(us)
            }
        }
    }
    return <- times
}

def time_rounds(narms, rounds, warmup : int; arm : block<(i : int) : void>) : array<array<int>> {
    return <- time_rounds(narms, rounds, warmup, $(_i : int) {}, arm)
}

//! the largest difference of `got` from `want`, and the bound it answers to
def held(got : array<float>; want : array<float>; rel : float) : Held {
    var diff = 0.0
    var mag = 0.0
    var finite = true
    for (g, w in got, want) {
        finite &&= is_finite(g)
        diff = max(diff, abs(g - w))
        mag = max(mag, abs(w))
    }
    return (diff = diff, bound = rel * mag, ok = finite && diff <= rel * mag)
}

//! the compare as a row reads it; `region` names the elements held ("512 sampled elements")
def compare_words(h : Held; against : string; region : string) : string {
    if (h.diff == 0.0 && h.ok) {
        return "bit-exact vs {against} on {region}"
    }
    return "{h.ok ? "within" : "OUTSIDE"} {h.bound} of {against} on {region} (largest difference {h.diff})"
}

//! an arm's best, median (the upper middle) and population cv of its round times, in ms
def stats(us : array<int>) : tuple<best : float; median : float; cv : float> {
    var ms <- [for (u in us); float(u) / 1000.0]
    sort(ms)
    var mean = 0.0
    for (v in ms) {
        mean += v
    }
    mean /= float(length(ms))
    var sq = 0.0
    for (v in ms) {
        sq += (v - mean) * (v - mean)
    }
    return (best = ms[0], median = ms[length(ms) / 2], cv = sqrt(sq / float(length(ms))) / mean * 100.0)
}

//! one shape's answer into the tally: its row count, or a refusal when it answered -1
def tally(var t : RaceTally; rows : int) {
    if (rows < 0) {
        t.bad++
    } else {
        t.rows += rows
    }
}

//! the race's exit code: 1, with the closing line, when it reported no row or refused anything
def race_exit(t : RaceTally; tool : string) : int {
    if (t.rows == 0 || t.bad > 0) {
        print("{tool}: {t.rows} rows, {t.bad} refusals - the run fails\n")
        return 1
    }
    return 0
}
