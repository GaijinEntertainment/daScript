options gen2
options indenting = 4

module strudel_time shared

require strudel/strudel_event
require math

// A time span in cycle-space. Cycles are continuous floats:
// 0.0–1.0 is the first cycle, 1.0–2.0 the second, etc.
struct TimeSpan {
    start : double
    stop  : double
}

// A single event produced by querying a pattern.
//
// `whole` is the "ideal" time span — the full note duration.
// `part` is the portion visible in the query window.
struct Hap {
    whole : TimeSpan
    part  : TimeSpan
    has_whole : bool = true
    @safe_when_uninitialized value : Event
}

// Split a time span at integer cycle boundaries.
// splitSpans(TimeSpan(0.75, 2.25)) → [(0.75,1.0), (1.0,2.0), (2.0,2.25)]
def splitSpans(span : TimeSpan) : array<TimeSpan> {
    var result : array<TimeSpan>
    var current = span.start
    while (current < span.stop) {
        let next = min(floor(current) + 1.0lf, span.stop)
        if (next > current) {
            result |> push(TimeSpan(start = current, stop = next))
        }
        current = next
    }
    return <- result
}

// Which cycle number does this time position fall in?
def sameCycle(t : double) : double {
    return floor(t)
}

// Fraction within the current cycle (0.0–1.0)
def cyclePos(t : double) : double {
    return t - floor(t)
}
