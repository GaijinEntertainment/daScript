options gen2
require dastest/testing_boost public
require daslib/fio
require tl_launch
require tl_model
require tl_loader
require math
require tl_stats

// Launch-rail helpers that run without a subprocess: quote-aware argv split and the
// mtime/size debounced file watcher.

[test(description = "the argv splitter respects quotes")]
def test_split_args(t : T?) {
    t |> run("quote-aware argv split") @(t : T?) {
        let a <- split_args("-m \"a b.gguf\" -p  512")
        t |> equal(4, length(a))
        t |> equal("-m", a[0])
        t |> equal("a b.gguf", a[1])
        t |> equal("-p", a[2])
        t |> equal("512", a[3])
        let b <- split_args("")
        t |> equal(0, length(b))
        let c <- split_args("single")
        t |> equal(1, length(c))
    }
}

[test(description = "the launcher finds the .json token in an output line")]
def test_extract_json_path(t : T?) {
    t |> run("finds the .json token in an output line") @(t : T?) {
        t |> equal("C:/tmp/a.trace.json", extract_json_path("jobque trace saved: C:/tmp/a.trace.json"))
        t |> equal("", extract_json_path("decode: 8 tokens in 118494us"))
        t |> equal("b.json", extract_json_path("wrote b.json ok"))
    }
}

[test(description = "the file watch fires on a change only after a stable poll")]
def test_file_watch(t : T?) {
    t |> run("watch fires on change only after a stable poll") @(t : T?) {
        var terr = ""
        let dir = temp_directory(terr)
        let path = "{dir}/tl_watch_probe.txt"
        fopen(path, "wb") $(f) {
            if (f != null) {
                f |> fwrite("one")
            }
        }
        var w = FileWatch()
        watch_prime(w, path)
        t |> success(!watch_poll(w, path), "no change -> no fire")
        fopen(path, "wb") $(f) {
            if (f != null) {
                f |> fwrite("two-longer")
            }
        }
        t |> success(!watch_poll(w, path), "first poll after change arms the debounce")
        t |> success(watch_poll(w, path), "second (stable) poll fires")
        t |> success(!watch_poll(w, path), "and it rearms quietly")
    }
}

// Loader round-trip on synthetic traceSave text: new-format (jobqueProfile + markers),
// old-format fallback (legacy palette, no markers), sorting, chain resolution.

def write_temp(t : T?; name, text : string) : string {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = "{dir}/{name}"
    fopen(path, "wb") $(f) {
        if (f != null) {
            f |> fwrite(text)
        }
    }
    return path
}

def new_format_trace() : string {
    // one worker + caller; a 2-stage tagged chain, a wake recorded AFTER later events
    // (ring order is completion order — the loader must sort), a marker, a header
    var s = "\{\"traceEvents\":[\n"
    s += "\{\"ph\":\"M\",\"pid\":0,\"tid\":0,\"name\":\"thread_name\",\"args\":\{\"name\":\"worker 0\"\}\},\n"
    s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":10.000,\"dur\":5.000,\"name\":\"chunk\",\"args\":\{\"stage\":0,\"arg\":0,\"chain\":7\}\},\n"
    s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":16.000,\"dur\":4.000,\"name\":\"chunk\",\"args\":\{\"stage\":1,\"arg\":1,\"chain\":7\}\},\n"
    s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":2.500,\"dur\":30.000,\"name\":\"wake\",\"args\":\{\"stage\":0,\"arg\":1,\"chain\":0\}\},\n"
    s += "\{\"ph\":\"M\",\"pid\":0,\"tid\":1,\"name\":\"thread_name\",\"args\":\{\"name\":\"caller 1\"\}\},\n"
    s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":1,\"ts\":9.000,\"dur\":1.500,\"name\":\"publish\",\"args\":\{\"stage\":1794,\"arg\":4,\"chain\":7\}\},\n"
    s += "\{\"ph\":\"i\",\"s\":\"g\",\"pid\":0,\"tid\":1,\"ts\":8.000,\"name\":\"token\",\"args\":\{\"arg\":41\}\},\n"
    s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":1,\"ts\":20.000,\"dur\":2.000,\"name\":\"stage_wait\",\"args\":\{\"stage\":1,\"arg\":0,\"chain\":7\}\}\n"
    s += "],\"jobqueProfile\":\{\"version\":1,\"lanes\":2,"
    s += "\"categories\":[\{\"id\":7,\"name\":\"my op\",\"color\":\"#112233\"\}],"
    s += "\"markers\":[\{\"id\":0,\"name\":\"token\"\}]\}\}\n"
    return s
}

[test(description = "a new-format trace loads with its header, markers and chains")]
def test_loader_new_format(t : T?) {
    t |> run("new-format trace loads with header, markers, chains") @(t : T?) {
        let path = write_temp(t, "tl_test_new.json", new_format_trace())
        var doc : TraceDoc
        t |> success(load_trace(path, doc), "load_trace succeeds")
        t |> equal(2, length(doc.lanes))
        t |> equal(5, int(doc.total_events))
        t |> success(doc.has_profile, "jobqueProfile parsed")
        t |> equal("worker 0", doc.lanes[0].name)
        t |> equal("caller 1", doc.lanes[1].name)
        // header category "my op" (tag 7) appended after the 4 base classes
        t |> equal(5, length(doc.cats))
        t |> equal("my op", doc.cats[4].name)
        t |> equal(0x112233u, doc.cats[4].col)
        // markers
        t |> equal(1, length(doc.marker_kinds))
        t |> equal("token", doc.marker_kinds[0].name)
        t |> equal(41, doc.marker_kinds[0].args[0])
        t |> equal(8.0lf, doc.marker_kinds[0].ts[0])
        // wake recorded out of t0 order must sort first on lane 0
        t |> equal(EV_WAKE, ev_kind(doc.lanes[0].ev[0].meta))
        t |> equal(2.5lf, doc.lanes[0].ev[0].t0)
        t |> equal(30.0lf, doc.lanes[0].max_dur)
        // chain resolution: packed 1794 = nstages 2 | tag 7<<8 -> both chunks tagged "my op"
        let c0 = doc.lanes[0].ev[1]
        t |> equal(EV_CHUNK, ev_kind(c0.meta))
        t |> equal(2, ev_nstages(c0.meta))
        t |> equal(4, ev_cat(c0.meta))
        // publish stage field unpacked to the low byte (sorted first on the caller lane)
        let pub = doc.lanes[1].ev[0]
        t |> equal(EV_PUBLISH, ev_kind(pub.meta))
        t |> equal(2, ev_stage(pub.meta))
        // span covers the longest event end
        t |> equal(32.5lf, doc.span)
        // mips built (level 0 at least)
        t |> success(!empty(doc.lanes[0].mips), "mips built")
    }
}

[test(description = "a headerless trace falls back to the legacy palette")]
def test_loader_old_format(t : T?) {
    t |> run("headerless trace falls back to the legacy palette") @(t : T?) {
        var s = "\{\"traceEvents\":[\n"
        s += "\{\"ph\":\"M\",\"pid\":0,\"tid\":0,\"name\":\"thread_name\",\"args\":\{\"name\":\"caller 0\"\}\},\n"
        s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":0.000,\"dur\":3.000,\"name\":\"publish\",\"args\":\{\"stage\":1281,\"arg\":2,\"chain\":3\}\},\n"
        s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":4.000,\"dur\":6.000,\"name\":\"chunk\",\"args\":\{\"stage\":0,\"arg\":0,\"chain\":3\}\}\n"
        s += "]\}\n"
        let path = write_temp(t, "tl_test_old.json", s)
        var doc : TraceDoc
        t |> success(load_trace(path, doc), "load_trace succeeds")
        t |> success(!doc.has_profile, "no header")
        t |> equal(0, length(doc.marker_kinds))
        // packed 1281 = nstages 1 | tag 5<<8 -> legacy tag 5 = classifier appended
        let c = doc.lanes[0].ev[1]
        t |> equal(1, ev_nstages(c.meta))
        t |> equal("classifier", doc.cats[ev_cat(c.meta)].name)
    }
}

[test(description = "untagged chains classify by their stage count")]
def test_loader_untagged_heuristic(t : T?) {
    t |> run("untagged chains classify by stage count") @(t : T?) {
        var s = "\{\"traceEvents\":[\n"
        s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":0.000,\"dur\":1.000,\"name\":\"publish\",\"args\":\{\"stage\":2,\"arg\":2,\"chain\":9\}\},\n"
        s += "\{\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":2.000,\"dur\":1.000,\"name\":\"chunk\",\"args\":\{\"stage\":0,\"arg\":0,\"chain\":9\}\}\n"
        s += "]\}\n"
        let path = write_temp(t, "tl_test_untagged.json", s)
        var doc : TraceDoc
        t |> success(load_trace(path, doc), "load_trace succeeds")
        t |> equal(CAT_FFN, ev_cat(doc.lanes[0].ev[1].meta))
    }
}

// RangeStats on a hand-built doc: clipping at range edges, idle/parallelism math,
// per-category shares, cache freshness.

def make_doc() : TraceDoc {
    var doc = TraceDoc(cats <- base_categories(), span = 100.0lf)
    doc.lanes |> resize(2)
    // lane 0: one 10µs attn chunk [10,20], one publish [5,6]
    doc.lanes[0].ev <- [
        Ev(t0 = 5.0lf, dur = 1.0f, meta = ev_meta(EV_PUBLISH, 2, 2, CAT_ATTN), arg = 0u, chain = 1u),
        Ev(t0 = 10.0lf, dur = 10.0f, meta = ev_meta(EV_CHUNK, 0, 2, CAT_ATTN), arg = 0u, chain = 1u)
    ]
    doc.lanes[0].max_dur = 10.0lf
    // lane 1: a 20µs ffn chunk [15,35] (clips at range edge), a 5µs stage_wait [40,45]
    doc.lanes[1].ev <- [
        Ev(t0 = 15.0lf, dur = 20.0f, meta = ev_meta(EV_CHUNK, 1, 2, CAT_FFN), arg = 1u, chain = 1u),
        Ev(t0 = 40.0lf, dur = 5.0f, meta = ev_meta(EV_STAGE_WAIT, 1, 2, CAT_FFN), arg = 0u, chain = 1u)
    ]
    doc.lanes[1].max_dur = 20.0lf
    doc.generation = 1
    return <- doc
}

[test(description = "range stats compute busy, idle and parallelism with edge clipping")]
def test_range_stats(t : T?) {
    t |> run("busy/idle/parallelism with edge clipping") @(t : T?) {
        var doc <- make_doc()
        // range [10,25]: lane0 chunk fully in (10), lane1 chunk clipped to [15,25] (10);
        // publish [5,6] outside; wait outside
        let st <- compute_range_stats(doc, 10.0lf, 25.0lf)
        t |> equal(2, st.events)
        t |> equal(20.0lf, st.busy_us)
        t |> equal(0.0lf, st.wait_us)
        t |> equal(0, st.publish_count)
        // idle = 1 - 20/(15*2) = 33.33%
        t |> success(abs(st.idle_pct - 33.333f) < 0.1f, "idle pct: {st.idle_pct}")
        t |> success(abs(st.parallelism - 20.0f / 15.0f) < 0.01f, "parallelism: {st.parallelism}")
        t |> equal(2, length(st.cat))
        t |> equal(st.cat[0].busy_us, st.cat[1].busy_us)   // 10 attn + 10 ffn
    }
}

[test(description = "a full-span range counts publish and wait time")]
def test_range_stats_full(t : T?) {
    t |> run("full span counts publish and wait") @(t : T?) {
        var doc <- make_doc()
        let st <- compute_range_stats(doc, 0.0lf, 100.0lf)
        t |> equal(4, st.events)
        t |> equal(1, st.publish_count)
        t |> equal(31.0lf, st.busy_us)   // 10 + 20 + 1 (publish counts as work)
        t |> equal(5.0lf, st.wait_us)
    }
}

[test(description = "the stats cache recomputes only when its key changes")]
def test_stats_cache(t : T?) {
    t |> run("cache recomputes only on key change") @(t : T?) {
        var doc <- make_doc()
        var cache = StatsCache()
        cached_stats(cache, doc, 10.0lf, 25.0lf)
        t |> equal(2, cache.stats.events)
        // same key -> stays; poke the stats to detect a recompute
        cache.stats.events = 999
        cached_stats(cache, doc, 10.0lf, 25.0lf)
        t |> equal(999, cache.stats.events)
        // generation bump -> recompute
        doc.generation++
        cached_stats(cache, doc, 10.0lf, 25.0lf)
        t |> equal(2, cache.stats.events)
    }
}
