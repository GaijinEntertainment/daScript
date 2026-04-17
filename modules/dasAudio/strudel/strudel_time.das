options gen2
options indenting = 4

module strudel_time shared public
//! Cycle-space time primitives for pattern queries.
//! Defines `TimeSpan` (a [start, stop) interval in cycles) and `Hap` (a single
//! event with its ideal whole span and the visible part within a query window).

require strudel/strudel_event
require math

//! A time span in cycle-space. Cycles are continuous floats: 0.0–1.0 is the first cycle,
//! 1.0–2.0 the second, etc.
struct TimeSpan {
    start : double
        //! Inclusive start position in cycles.
    stop  : double
        //! Exclusive end position in cycles.
}

//! A single event produced by querying a pattern. `whole` is the ideal note duration;
//! `part` is the portion visible in the query window (may be a subset of `whole`).
struct Hap {
    whole : TimeSpan
        //! Ideal full span of the event (full note duration).
    part  : TimeSpan
        //! Portion of `whole` visible in the current query window.
    has_whole : bool = true
        //! False for continuous events that have no discrete onset (e.g. signals).
    @safe_when_uninitialized value : Event
        //! The event payload (what to play and how).
}

def splitSpans(span : TimeSpan) : array<TimeSpan> {
    //! Split a time span at integer cycle boundaries. Example: `splitSpans(TimeSpan(0.75, 2.25))`
    //! returns `[(0.75,1.0), (1.0,2.0), (2.0,2.25)]`.
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

def sameCycle(t : double) : double {
    //! Cycle number containing time position `t` (integer part as double).
    return floor(t)
}

def cyclePos(t : double) : double {
    //! Fractional position within the current cycle (0.0–1.0).
    return t - floor(t)
}
