options gen2
require dastest/testing_boost public
require daslib/fio
require tl_model
require tl_loader

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

[test]
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

[test]
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

[test]
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
