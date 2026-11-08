options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel
require math

[test(description = "splitSpans leaves a span inside one cycle as a single span")]
def test_splitSpans(t : T?) {
    let spans <- splitSpans(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(spans), 1)
    t |> success(abs(spans[0].start - 0.0lf) < 0.001lf)
    t |> success(abs(spans[0].stop - 1.0lf) < 0.001lf)
    let spans2 <- splitSpans(TimeSpan(start = 0.75lf, stop = 1.25lf))
    t |> equal(length(spans2), 2)
    t |> success(abs(spans2[0].start - 0.75lf) < 0.001lf)
    t |> success(abs(spans2[0].stop - 1.0lf) < 0.001lf)
    t |> success(abs(spans2[1].start - 1.0lf) < 0.001lf)
    t |> success(abs(spans2[1].stop - 1.25lf) < 0.001lf)
    let spans3 <- splitSpans(TimeSpan(start = 0.75lf, stop = 2.25lf))
    t |> equal(length(spans3), 3)
}

[test(description = "pure yields one event holding the value")]
def test_pure(t : T?) {
    let pat <- pure(Event(s = "bd"))
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    t |> equal(haps[0].value.s, "bd")
    t |> success(abs(haps[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(haps[0].whole.stop - 1.0lf) < 0.001lf)
    t |> success(abs(haps[0].part.start - 0.0lf) < 0.001lf)
    t |> success(abs(haps[0].part.stop - 1.0lf) < 0.001lf)
}

[test(description = "pure queried over a partial cycle keeps the whole cycle as the event's whole")]
def test_pure_partial(t : T?) {
    let pat <- pure(Event(s = "bd"))
    let haps <- pat(TimeSpan(start = 0.25lf, stop = 0.75lf))
    t |> equal(length(haps), 1)
    t |> success(abs(haps[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(haps[0].whole.stop - 1.0lf) < 0.001lf)
    t |> success(abs(haps[0].part.start - 0.25lf) < 0.001lf)
    t |> success(abs(haps[0].part.stop - 0.75lf) < 0.001lf)
}

[test(description = "silence yields no events")]
def test_silence(t : T?) {
    let pat <- silence()
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 10.0lf))
    t |> equal(length(haps), 0)
}

[test(description = "atom yields one event with that sound")]
def test_atom(t : T?) {
    let pat <- atom("sd")
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    t |> equal(haps[0].value.s, "sd")
}

[test(description = "fast(2) doubles the events per cycle")]
def test_fast(t : T?) {
    let pat <- atom("bd") |> fast(2.0lf)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 2)
    t |> success(abs(haps[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(haps[0].whole.stop - 0.5lf) < 0.001lf)
    t |> success(abs(haps[1].whole.start - 0.5lf) < 0.001lf)
    t |> success(abs(haps[1].whole.stop - 1.0lf) < 0.001lf)
}

[test(description = "slow stretches an event over more than one cycle")]
def test_slow(t : T?) {
    let pat <- atom("bd") |> slow(2.0lf)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    t |> success(abs(haps[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(haps[0].whole.stop - 2.0lf) < 0.001lf)
}

[test(description = "stack plays its patterns at the same time")]
def test_stack(t : T?) {
    var pats : array<Pattern>
    var p0 <- atom("bd")
    var p1 <- atom("hh")
    pats |> emplace <| p0
    pats |> emplace <| p1
    let pat <- stack(pats)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 2)
    t |> equal(haps[0].value.s, "bd")
    t |> equal(haps[1].value.s, "hh")
}

[test(description = "cat plays one pattern per cycle")]
def test_cat(t : T?) {
    var pats : array<Pattern>
    var p0 <- atom("bd")
    var p1 <- atom("sd")
    pats |> emplace <| p0
    pats |> emplace <| p1
    let pat <- cat(pats)
    let haps0 <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps0), 1)
    t |> equal(haps0[0].value.s, "bd")
    let haps1 <- pat(TimeSpan(start = 1.0lf, stop = 2.0lf))
    t |> equal(length(haps1), 1)
    t |> equal(haps1[0].value.s, "sd")
    let haps2 <- pat(TimeSpan(start = 2.0lf, stop = 3.0lf))
    t |> equal(length(haps2), 1)
    t |> equal(haps2[0].value.s, "bd")
}

[test(description = "fastcat squeezes its patterns into one cycle")]
def test_fastcat(t : T?) {
    var pats : array<Pattern>
    var p0 <- atom("bd")
    var p1 <- atom("sd")
    var p2 <- atom("hh")
    var p3 <- atom("cp")
    pats |> emplace <| p0
    pats |> emplace <| p1
    pats |> emplace <| p2
    pats |> emplace <| p3
    let pat <- fastcat(pats)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 4)
    t |> equal(haps[0].value.s, "bd")
    t |> equal(haps[1].value.s, "sd")
    t |> equal(haps[2].value.s, "hh")
    t |> equal(haps[3].value.s, "cp")
    t |> success(abs(haps[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(haps[0].whole.stop - 0.25lf) < 0.001lf)
    t |> success(abs(haps[1].whole.start - 0.25lf) < 0.001lf)
    t |> success(abs(haps[1].whole.stop - 0.5lf) < 0.001lf)
}

[test(description = "fmap transforms each event's value")]
def test_fmap(t : T?) {
    let fn <- @(e : Event) : Event { return Event(s = e.s, gain = 0.5); }
    let pat <- atom("bd") |> fmap(fn)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    t |> equal(haps[0].value.s, "bd")
    t |> success(abs(haps[0].value.gain - 0.5) < 0.001)
}

[test(description = "degrade drops events deterministically at the extremes")]
def test_degrade(t : T?) {
    let pat <- atom("bd") |> degrade(1.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 0)
    let pat2 <- atom("bd") |> degrade(0.0)
    let haps2 <- pat2(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps2), 1)
}

[test(description = "the Bjorklund algorithm spreads 3 onsets over 8 steps")]
def test_bjorklund(t : T?) {
    var rhythm <- bjorklund(3, 8)
    t |> equal(length(rhythm), 8)
    var count = 0
    for (b in rhythm) {
        if (b) {
            count++
        }
    }
    t |> equal(count, 3)
}

[test(description = "euclid yields one event per onset of the rhythm")]
def test_euclid(t : T?) {
    let pat <- atom("bd") |> euclid(3, 8)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 3)
}

[test(description = "a query spanning two cycles returns the events of both")]
def test_cross_cycle_query(t : T?) {
    var pats : array<Pattern>
    var p0 <- atom("bd")
    var p1 <- atom("sd")
    pats |> emplace <| p0
    pats |> emplace <| p1
    let pat <- cat(pats)
    let haps <- pat(TimeSpan(start = 0.5lf, stop = 1.5lf))
    t |> equal(length(haps), 2)
    t |> equal(haps[0].value.s, "bd")
    t |> equal(haps[1].value.s, "sd")
}
