options gen2
options indenting = 4

module _row_stats shared public

//! A measured row's statistic over its reps, with no engine module behind it: an instrument that only reads clocks
//! (`served_bench.das`) takes it without loading the engine, and the timing races take it through `_race_fixture`.

require math

//! `vals` sorted in place: the smallest, the median (the upper middle) and the population cv in percent
def spread_of(var vals : array<float>) : tuple<best : float; median : float; cv : float> {
    sort(vals)
    var mean = 0.0
    for (v in vals) {
        mean += v
    }
    mean /= float(length(vals))
    var sq = 0.0
    for (v in vals) {
        sq += (v - mean) * (v - mean)
    }
    return (best = vals[0], median = vals[length(vals) / 2], cv = sqrt(sq / float(length(vals))) / mean * 100.0)
}
