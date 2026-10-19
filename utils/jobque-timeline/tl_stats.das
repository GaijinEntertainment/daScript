options gen2
options indenting = 4

module tl_stats shared

require tl_model
require math
require daslib/cmp

// Computed statistics for a time range of a TraceDoc — the chips the HTML artifact
// hand-wrote. Cached per (range, generation); recomputed only when the key changes.

struct public CatStat {
    name : string
    busy_us : double
    pct : float            // share of total busy
}

struct public RangeStats {
    t0, t1 : double
    events : int
    busy_us : double       // chunk + fifo + publish work, clipped to the range, all lanes
    wait_us : double       // stage_wait clipped
    idle_pct : float       // 1 - busy/(range × lanes)
    parallelism : float    // busy/range = average busy lanes
    publish_count : int
    cat : array<CatStat>   // busy>0 categories, descending
}

def public compute_range_stats(doc : TraceDoc; t0, t1 : double) : RangeStats {
    var st = RangeStats(t0 = t0, t1 = t1)
    let range_us = max(t1 - t0, 1.0e-9lf)
    var cat_busy : array<double>
    cat_busy |> resize(length(doc.cats))
    for (lane in doc.lanes) {
        var j = lane_lower_bound(lane, t0 - lane.max_dur)
        while (j < length(lane.ev)) {
            let e & = unsafe(lane.ev[j])
            if (e.t0 > t1) {
                break
            }
            j++
            let te = e.t0 + double(e.dur)
            if (te < t0) {
                continue
            }
            st.events++
            let clipped = min(te, t1) - max(e.t0, t0)
            let kind = ev_kind(e.meta)
            if (kind == EV_CHUNK || kind == EV_FIFO || kind == EV_PUBLISH) {
                st.busy_us += clipped
                cat_busy[ev_cat(e.meta)] += clipped
                if (kind == EV_PUBLISH) {
                    st.publish_count++
                }
            } elif (kind == EV_STAGE_WAIT) {
                st.wait_us += clipped
            }
        }
    }
    let nlanes = max(length(doc.lanes), 1)
    st.idle_pct = float(1.0lf - st.busy_us / (range_us * double(nlanes))) * 100.0f
    st.parallelism = float(st.busy_us / range_us)
    for (c in range(length(doc.cats))) {
        if (cat_busy[c] > 0.0lf) {
            st.cat |> push(CatStat(name = doc.cats[c].name, busy_us = cat_busy[c],
                pct = float(cat_busy[c] / max(st.busy_us, 1.0e-9lf)) * 100.0f))
        }
    }
    st.cat |> sort() <| cmp_by(-_.busy_us)
    delete cat_busy
    return <- st
}

struct public StatsCache {
    t0, t1 : double
    generation : int = -1
    stats : RangeStats
}

//! Freshen a stats cache: recompute only when the range or the doc generation changed.
//! Read the result from `cache.stats`.
def public cached_stats(var cache : StatsCache; doc : TraceDoc; t0, t1 : double) {
    if (cache.generation != doc.generation || cache.t0 != t0 || cache.t1 != t1) {
        cache.stats <- compute_range_stats(doc, t0, t1)
        cache.t0 = t0
        cache.t1 = t1
        cache.generation = doc.generation
    }
}

//! One-line chip text: "idle 23%  par 12.4x  pubs 321  |  ffn per-op 61%  attn std 34%"
def public stats_line(stats : RangeStats) : string {
    let deci = int(stats.parallelism * 10.0f + 0.5f)
    var line = "idle {int(stats.idle_pct + 0.5f)}%  par {deci / 10}.{deci % 10}x  pubs {stats.publish_count}"
    var shown = 0
    for (c in stats.cat) {
        if (shown >= 4) {
            break
        }
        line += "  |  {c.name} {int(c.pct + 0.5f)}%"   // nolint:PERF001 — at most 4 chips
        shown++
    }
    return line
}
