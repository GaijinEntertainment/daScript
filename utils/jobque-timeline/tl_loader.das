options gen2
options indenting = 4

module tl_loader shared

require tl_model
require tl_lod
require strings
require daslib/fio
require daslib/json_boost

// Rigid-format fast path over traceSave output (job_que.cpp): one event per line, fixed key
// order, kind names unique by first letter. No JSON DOM for events — only the small
// jobqueProfile trailer goes through the generic parser. Old files (no header, no markers)
// load with the legacy palette.

def private parse_uint(data : array<uint8> const#; var pos : int&) : uint64 {
    var v = 0ul
    while (pos < length(data)) {
        let c = int(data[pos])
        if (!is_number(c)) {
            break
        }
        v = v * 10ul + uint64(c - '0')
        pos++
    }
    return v
}

// "%.3f" µs field parsed as exact integer nanoseconds
def private parse_us3(data : array<uint8> const#; var pos : int&) : double {
    var ns = parse_uint(data, pos) * 1000ul
    if (pos < length(data) && data[pos] == uint8('.')) {
        pos++
        var mul = 100ul
        while (pos < length(data) && uint(data[pos]) >= uint('0') && uint(data[pos]) <= uint('9')) {
            ns += uint64(uint(data[pos]) - uint('0')) * mul
            mul /= 10ul
            pos++
        }
    }
    return double(ns) / 1000.0lf
}

// advance past `key` (e.g. "\"tid\":") scanning from pos, up to line_end; -1 if absent
def private seek_past(data : array<uint8> const#; pos, line_end : int; key : string) : int {
    var at = pos
    var hit = -1
    peek_data(key) $(kb) {
        let kn = length(kb)
        while (at + kn <= line_end) {
            var m = 0
            while (m < kn && data[at + m] == kb[m]) {
                m++
            }
            if (m == kn) {
                hit = at + kn
                break
            }
            at++
        }
    }
    return hit
}

def private read_json_string(data : array<uint8> const#; var pos : int&; line_end : int) : string {
    var sb : array<uint8>
    while (pos < line_end && data[pos] != uint8('"')) {
        if (data[pos] == uint8('\\') && pos + 1 < line_end) {
            pos++   // minimal unescape: \" and \\ (our writer escapes nothing else in names)
        }
        sb |> push(data[pos])
        pos++
    }
    var out : string
    unsafe {
        out = string(sb)
    }
    delete sb
    return out
}

struct private LineEv {
    tid : int
    ts : double
    dur : double
    kind : int
    stage : uint
    arg : uint
    chain : uint
}

// parse one `{"ph":"X"...}` line; false when malformed
def private parse_x_line(data : array<uint8> const#; pos, line_end : int; var e : LineEv&) : bool {
    var at = seek_past(data, pos, line_end, "\"tid\":")
    if (at < 0) return false
    e.tid = int(parse_uint(data, at))
    at = seek_past(data, at, line_end, "\"ts\":")
    if (at < 0) return false
    e.ts = parse_us3(data, at)
    at = seek_past(data, at, line_end, "\"dur\":")
    if (at < 0) return false
    e.dur = parse_us3(data, at)
    at = seek_past(data, at, line_end, "\"name\":\"")
    if (at < 0) return false
    let c = int(data[at])
    if (c == 'p') {
        e.kind = EV_PUBLISH
    } elif (c == 'c') {
        e.kind = EV_CHUNK
    } elif (c == 's') {
        e.kind = EV_STAGE_WAIT
    } elif (c == 'w') {
        e.kind = EV_WAKE
    } else {
        e.kind = EV_FIFO
    }
    at = seek_past(data, at, line_end, "\"stage\":")
    if (at < 0) return false
    e.stage = uint(parse_uint(data, at))
    at = seek_past(data, at, line_end, "\"arg\":")
    if (at < 0) return false
    e.arg = uint(parse_uint(data, at))
    at = seek_past(data, at, line_end, "\"chain\":")
    if (at < 0) return false
    e.chain = uint(parse_uint(data, at))
    return true
}

def private cat_for_tag(var doc : TraceDoc; var tag2cat : table<int; int>; tag : int) : int {
    let known = tag2cat?[tag] ?? -1
    if (known >= 0) return known
    doc.cats |> push(legacy_tag_category(tag))
    let idx = length(doc.cats) - 1
    tag2cat[tag] = idx
    return idx
}

def private marker_kind_index(var doc : TraceDoc; name : string) : int {
    for (i in range(length(doc.marker_kinds))) {
        if (doc.marker_kinds[i].name == name) return i
    }
    var mk : MarkerKind
    mk.name = name
    doc.marker_kinds |> emplace(mk)
    return length(doc.marker_kinds) - 1
}

//! Load a traceSave JSON file into a TraceDoc. Synchronous (phases run back to back);
//! returns false on read/parse failure. Old-format files (no jobqueProfile) get the
//! legacy palette and an empty marker table.
def public load_trace(path : string; var doc : TraceDoc) : bool { // nolint:STYLE038 — two-pass trace parser; the passes share one byte cursor and the per-tid tallies
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    if (empty(text)) return false
    doc.path = path
    doc.title = base_name(path)
    doc.cats <- base_categories()
    var tag2cat : table<int; int>
    var chains : table<uint; uint>   // chain -> packed nstages|tag<<8 (from its publish)
    var ok = true
    peek_data(text) $(data) { // nolint:STYLE037 — both parser passes live in this block, sharing the byte cursor
        let n = length(data)
        // pass A: count events per tid, find max tid, register header categories
        var counts : array<int>
        var pos = 0
        while (pos < n) {
            var line_end = pos
            while (line_end < n && data[line_end] != uint8('\n')) {
                line_end++
            }
            if (pos + 8 < n && data[pos] == uint8('{') && data[pos + 2] == uint8('p') && data[pos + 4] == uint8('"')) {
                let ph = int(data[pos + 7])
                if (ph == 'X' || ph == 'i') {
                    var at = seek_past(data, pos, line_end, "\"tid\":")
                    if (at >= 0) {
                        let tid = int(parse_uint(data, at))
                        while (length(counts) <= tid) {
                            counts |> push(0)
                        }
                        if (ph == 'X') {
                            counts[tid]++
                        }
                    }
                }
            } elif (data[pos] == uint8(']')) {
                // trailer: parse the jobqueProfile object with the generic JSON parser
                let at = seek_past(data, pos, line_end, "\"jobqueProfile\":")
                if (at >= 0) {
                    var jtxt = slice(data, at, line_end)
                    if (ends_with(jtxt, "}}")) {
                        jtxt = slice(jtxt, 0, length(jtxt) - 1)
                    }
                    var jerr = ""
                    let prof = read_json(jtxt, jerr)
                    if (prof != null) {
                        doc.has_profile = true
                        if (prof?["categories"] != null) {
                            for (cat in prof?["categories"] as _array) {
                                let tag = cat?["id"] ?? 0
                                var col = 0xC39536u
                                let hexs = cat?["color"] ?? ""
                                if (length(hexs) == 7) {
                                    col = to_uint("0x" + slice(hexs, 1), true)
                                }
                                doc.cats |> push(Category(tag = tag, name = cat?["name"] ?? "", col = col))
                                tag2cat[tag] = length(doc.cats) - 1
                            }
                        }
                    }
                }
            }
            pos = line_end + 1
        }
        if (empty(counts)) {
            ok = false
            return
        }
        doc.lanes |> resize(length(counts))
        for (i in range(length(counts))) {
            doc.lanes[i].ev |> reserve(counts[i])
            doc.lanes[i].name = "lane {i}"
        }
        // pass B: fill events, lane names, markers; collect chain->packed off publishes
        pos = 0
        while (pos < n) {
            var line_end = pos
            while (line_end < n && data[line_end] != uint8('\n')) {
                line_end++
            }
            if (pos + 8 < n && data[pos] == uint8('{') && data[pos + 2] == uint8('p') && data[pos + 4] == uint8('"')) {
                let ph = int(data[pos + 7])
                if (ph == 'X') {
                    var le : LineEv
                    if (parse_x_line(data, pos, line_end, le) && le.tid < length(doc.lanes)) {
                        var stage = le.stage
                        if (le.kind == EV_PUBLISH) {
                            chains[le.chain] = le.stage   // packed nstages|tag<<8
                            stage = le.stage & 0xFFu      // trace_window.py semantics
                        }
                        doc.lanes[le.tid].ev |> push(Ev(
                            t0 = le.ts, dur = float(le.dur),
                            meta = ev_meta(le.kind, int(stage), 0, 0),
                            arg = le.arg, chain = le.chain))
                        doc.total_events++
                        let tend = le.ts + le.dur
                        if (tend > doc.span) {
                            doc.span = tend
                        }
                    }
                } elif (ph == 'i') {
                    var at = seek_past(data, pos, line_end, "\"tid\":")
                    if (at >= 0) {
                        let tid = int(parse_uint(data, at))
                        at = seek_past(data, at, line_end, "\"ts\":")
                        if (at >= 0) {
                            let ts = parse_us3(data, at)
                            at = seek_past(data, at, line_end, "\"name\":\"")
                            if (at >= 0) {
                                let mname = read_json_string(data, at, line_end)
                                var aat = seek_past(data, at, line_end, "\"arg\":")
                                let arg = aat >= 0 ? int(parse_uint(data, aat)) : 0
                                let mi = marker_kind_index(doc, mname)
                                doc.marker_kinds[mi].ts |> push(ts)
                                doc.marker_kinds[mi].args |> push(arg)
                                doc.marker_kinds[mi].lane |> push(int8(tid))
                                if (ts > doc.span) {
                                    doc.span = ts
                                }
                            }
                        }
                    }
                } elif (ph == 'M') {
                    var at = seek_past(data, pos, line_end, "\"tid\":")
                    if (at >= 0) {
                        let tid = int(parse_uint(data, at))
                        at = seek_past(data, at, line_end, "\"args\":\{\"name\":\"")
                        if (at >= 0 && tid < length(doc.lanes)) {
                            doc.lanes[tid].name = read_json_string(data, at, line_end)
                        }
                    }
                }
            }
            pos = line_end + 1
        }
    }
    if (!ok) return false
    // resolve: stamp nstages/cat from each event's chain, sort lanes, compute max_dur
    for (li in range(length(doc.lanes))) {
        var lane & = unsafe(doc.lanes[li])
        for (i in range(length(lane.ev))) {
            var e & = unsafe(lane.ev[i])
            let kind = ev_kind(e.meta)
            let packed = chains?[e.chain] ?? 0u
            let nstages = int(packed & 0xFFu)
            let tag = int(packed >> 8u)
            var cat = CAT_STD
            if (kind == EV_PUBLISH || kind == EV_CHUNK || kind == EV_STAGE_WAIT) {
                cat = tag != 0 ? cat_for_tag(doc, tag2cat, tag) : heuristic_cat(nstages)
            }
            e.meta = ev_meta(kind, ev_stage(e.meta), nstages, cat)
        }
        lane.ev |> sort() $(a, b : Ev) {
            return a.t0 < b.t0
        }
        var mx = 0.0lf
        for (e in lane.ev) {
            if (double(e.dur) > mx) {
                mx = double(e.dur)
            }
        }
        lane.max_dur = mx
    }
    build_mips(doc)
    doc.generation++
    return true
}
