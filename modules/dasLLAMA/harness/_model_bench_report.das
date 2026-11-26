options gen2
options indenting = 4

module _model_bench_report shared public

//! The model bench's page (`<entry>.md`): the rows two servers gave one model - ours and a reference - side by side, a
//! cell `ours / reference` and their ratio beside it, with no engine module behind it.

require _model_bench
require daslib/md_boost
require daslib/strings_convert
require daslib/cmp
require strings

//! one server's side of a page: what ran, and its rows joined over its run files; `engine` is "" for a side with no card
struct ReportSide {
    engine : string
    backend : string
    box : string
    date : string
    commit : string
    cpu : string
    gpu : string
    run : RunOut
}

//! a row's name read back (`pp4096@4`, `tg512+4096:dflash2`); `known` is false for a name of another shape
struct RowKey {
    known : bool
    decode : bool
    reply : int
    length : int
    width : int
    arm : string
    behind : int   //! a behind row's base length (`pp4096+131072`, `tg128+4096+131072`, `restore131072`, `save131072`, `cold131072`); 0 for any other row
    slot : bool    //! a row of the behind pass's base itself: a slot route's, or its cold prefill
}

enum private Section {
    prefill
    decode
    streams
    arms
    behind
    other
}

struct private PageRow {
    section : Section
    name : string
    key : RowKey
}

def private count_of(text : string) : int => try_to_int(text) ?? -1

def row_key(name : string) : RowKey {
    var key = RowKey(width = 1)
    var rest = name
    let colon = find(rest, ":")
    if (colon >= 0) {
        key.arm = slice(rest, colon + 1)
        rest = slice(rest, 0, colon)
    }
    let at = find(rest, "@")
    if (at >= 0) {
        key.width = count_of(slice(rest, at + 1))
        rest = slice(rest, 0, at)
    }
    for (route in ["restore", "save", "cold"]) {
        if (starts_with(rest, route)) {
            key.slot = true
            key.behind = count_of(slice(rest, length(route)))
            key.known = key.behind > 0 && key.width == 1 && empty(key.arm)
            return key
        }
    }
    let last = rfind(rest, "+")
    let plus = find(rest, "+")
    key.decode = starts_with(rest, "tg")
    if (plus >= 0 && (last > plus || !key.decode)) {   // a base after the length: a decode row's second `+`, a prefill row's only one
        key.behind = count_of(slice(rest, last + 1))
        rest = slice(rest, 0, last)
        if (key.behind <= 0) {
            return key
        }
    }
    let lead = key.decode ? find(rest, "+") : -1
    if (lead >= 0) {
        key.length = count_of(slice(rest, lead + 1))
        rest = slice(rest, 0, lead)
    }
    let count = length(rest) > 2 ? count_of(slice(rest, 2)) : -1
    if (key.decode) {
        key.reply = count
    } else {
        key.length = count
    }
    key.known = (key.decode || starts_with(rest, "pp")) && count >= 0 && key.length >= 0 && key.width > 0
    return key
}

def private section_of(key : RowKey) : Section {
    return Section.other if (!key.known)
    return Section.arms if (!empty(key.arm))
    return Section.behind if (key.behind > 0)
    return Section.streams if (key.width > 1)
    return key.decode ? Section.decode : Section.prefill
}

def private page_rows(ours, theirs : RunOut) : array<PageRow> {
    var names : table<string>
    for (name in keys(ours.tests)) {
        names |> insert(name)
    }
    for (name in keys(theirs.tests)) {
        names |> insert(name)
    }
    var rows : array<PageRow>
    for (name in keys(names)) {
        let key = row_key(name)
        rows |> push(PageRow(section = section_of(key), name = name, key = key))
    }
    sort(rows) <| cmp_by(int(_.section), _.key.behind, _.key.arm, _.key.width, _.key.length, _.key.reply, _.name)
    delete names
    return <- rows
}

def private rate_text(rate : double) : string => rate <= 0.0lf ? "-" : (rate >= 1000.0lf ? "{rate:.0f}" : "{rate:.1f}")

//! "" where neither side has a value
def private both_sides_cell(ours, theirs : string) : string => ours == "-" && theirs == "-" ? "" : "{ours} / {theirs}"

def private times_text(ours, theirs : double) : string => ours > 0.0lf && theirs > 0.0lf ? "{ours / theirs:.2f}" : ""

def private seconds_text(ms : double) : string => ms <= 0.0lf ? "-" : "{ms / 1000.0lf:.2f}"

def private share_text(part, whole : int64) : string => whole <= 0l ? "-" : "{double(part) * 100.0lf / double(whole):.0f}%"

def private over_text(rate, plain : double) : string => rate > 0.0lf && plain > 0.0lf ? "{rate / plain:.2f}x" : "-"

def private label_of(side : ReportSide; fallback : string) : string => empty(side.engine) ? fallback : side.engine

def private row_of(side : ReportSide; name : string) : TestRow => side.run.tests?[name] ?? TestRow()

def private flags_text(ours_label, ours_flags, theirs_label, theirs_flags : string) : string {
    return "" if (empty(ours_flags) && empty(theirs_flags))
    return "{theirs_label}: {theirs_flags}" if (empty(ours_flags))
    return "{ours_label}: {ours_flags}" if (empty(theirs_flags))
    return "{ours_label}: {ours_flags}; {theirs_label}: {theirs_flags}"
}

def private row_cells(row : PageRow; ours, theirs : ReportSide) : array<string> {
    let a = row_of(ours, row.name)
    let b = row_of(theirs, row.name)
    let rates = both_sides_cell(rate_text(a.tok_s), rate_text(b.tok_s))
    let times = times_text(a.tok_s, b.tok_s)
    let flags = flags_text(label_of(ours, "ours"), a.flags, label_of(theirs, "reference"), b.flags)
    let prompt = a.prompt_tokens > 0l ? a.prompt_tokens : b.prompt_tokens
    if (row.section == Section.prefill) {
        return <- [row.name, "{prompt}", rates, times, flags]
    } elif (row.section == Section.decode) {
        return <- [row.name, "{prompt}", rates, times, both_sides_cell(seconds_text(a.ttft_ms), seconds_text(b.ttft_ms)), flags]
    } elif (row.section == Section.streams) {
        let wa = wave_rate(row.key, a)
        let wb = wave_rate(row.key, b)
        return <- [row.name, "{row.key.width}", both_sides_cell(rate_text(wa), rate_text(wb)), times_text(wa, wb), both_sides_cell(rate_text(a.stream_tok_s), rate_text(b.stream_tok_s)),
            both_sides_cell(seconds_text(a.done_ms), seconds_text(b.done_ms)), flags]
    } elif (row.section == Section.behind) {
        return <- [row.name, "{prompt}", both_sides_cell(cached_text(a), cached_text(b)), rates, times, both_sides_cell(seconds_text(a.ttft_ms), seconds_text(b.ttft_ms)), flags]
    } elif (row.section == Section.arms) {
        let plain = slice(row.name, 0, find(row.name, ":"))
        return <- [row.name, rates, times,
            both_sides_cell(over_text(a.tok_s, row_of(ours, plain).tok_s), over_text(b.tok_s, row_of(theirs, plain).tok_s)),
            both_sides_cell(share_text(a.draft_n_accepted, a.draft_n), share_text(b.draft_n_accepted, b.draft_n)), flags]
    }
    return <- [row.name, rates, times, flags]
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench-runner")]
def private wave_rate(key : RowKey; row : TestRow) : double
    => key.decode && row.done_ms > 0.0lf ? double(key.width * key.reply) * 1000.0lf / row.done_ms : row.tok_s

def private cached_text(row : TestRow) : string => row.cached_tokens > 0l ? "{row.cached_tokens}" : "-"

def private section_title(section : Section) : string {
    if (section == Section.behind) {
        return "Behind a long context"
    } elif (section == Section.prefill) {
        return "Prefill, one stream"
    } elif (section == Section.decode) {
        return "Decode, one stream"
    } elif (section == Section.streams) {
        return "Streams at once"
    } elif (section == Section.arms) {
        return "Speculative arms"
    }
    return "Other rows"
}

def private section_headers(section : Section) : array<string> {
    if (section == Section.behind) {
        return <- ["row", "prompt tokens", "off the cache", "tok/s", "x", "first text, s", "flags"]
    } elif (section == Section.prefill) {
        return <- ["row", "prompt tokens", "tok/s", "x", "flags"]
    } elif (section == Section.decode) {
        return <- ["row", "prompt tokens", "tok/s", "x", "first text, s", "flags"]
    } elif (section == Section.streams) {
        return <- ["row", "streams", "tok/s over the wave", "x", "tok/s, one stream", "wave done, s", "flags"]
    } elif (section == Section.arms) {
        return <- ["row", "tok/s", "x", "over the plain row", "accepted of drafted", "flags"]
    }
    return <- ["row", "tok/s", "x", "flags"]
}

def private left_aligned(columns : int) : array<ColAlign> => [for (_c in range(columns)); ColAlign.left]

def private table_text(headers : array<string>; rows : array<array<string>>) : string {
    var aligns <- left_aligned(length(headers))
    let text = clone_string(render_md_table(headers, aligns, rows))
    delete aligns
    return text
}

def private section_text(section : Section; rows : array<PageRow>; ours, theirs : ReportSide) : string {
    var cells : array<array<string>>
    for (row in rows) {
        if (row.section == section) {
            cells |> emplace(row_cells(row, ours, theirs))
        }
    }
    var headers <- section_headers(section)
    let text = empty(cells) ? "" : "## {section_title(section)}\n\n{table_text(headers, cells)}\n\n"
    delete headers
    delete cells
    return text
}

def private wrong_replies(side : ReportSide) : int {
    var wrong = 0
    for (reply in side.run.eyeball) {
        wrong += empty(reply.why) ? 0 : 1
    }
    return wrong
}

def private sides_text(ours, theirs : ReportSide) : string {
    var headers <- ["", label_of(ours, "ours"), label_of(theirs, "reference")]
    var cells <- [["backend", ours.backend, theirs.backend],
        ["box", ours.box, theirs.box],
        ["date", ours.date, theirs.date],
        ["commit", ours.commit, theirs.commit],
        ["cpu", ours.cpu, theirs.cpu],
        ["gpu", ours.gpu, theirs.gpu],
        ["reps a cell", "{ours.run.bench.reps}", "{theirs.run.bench.reps}"],
        ["seconds a cell", "{ours.run.bench.cell_seconds}", "{theirs.run.bench.cell_seconds}"],
        ["rows", "{length(ours.run.tests)}", "{length(theirs.run.tests)}"],
        ["refused", "{length(ours.run.refused)}", "{length(theirs.run.refused)}"],
        ["eyeball replies, wrong", "{length(ours.run.eyeball)}, {wrong_replies(ours)}", "{length(theirs.run.eyeball)}, {wrong_replies(theirs)}"]]
    let text = table_text(headers, cells)
    delete headers
    delete cells
    return text
}

def private refused_cells(var cells : array<array<string>>; who : string; run : RunOut) {
    cells |> reserve(length(cells) + length(run.refused) + length(run.eyeball))
    for (refusal in run.refused) {
        cells |> emplace([who, refusal.cell, md_escape(refusal.why)])
    }
    for (reply in run.eyeball) {
        if (!empty(reply.why)) {
            cells |> emplace([who, "eyeball {reply.mode} {reply.ask}", md_escape(reply.why)])
        }
    }
}

def private refused_text(ours, theirs : ReportSide) : string {
    var cells : array<array<string>>
    refused_cells(cells, label_of(ours, "ours"), ours.run)
    refused_cells(cells, label_of(theirs, "reference"), theirs.run)
    var headers <- ["server", "cell", "why"]
    let text = empty(cells) ? "" : "## Refused\n\n{table_text(headers, cells)}\n\n"
    delete headers
    delete cells
    return text
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench-runner")]
def model_report(entry : string; ours, theirs : ReportSide) : string {
    let a = label_of(ours, "ours")
    let b = label_of(theirs, "reference")
    var rows <- page_rows(ours.run, theirs.run)
    let text = build_string() $(writer) {
        writer |> write("# {entry}\n\n{sides_text(ours, theirs)}\n\n")
        writer |> write("Every rate cell reads `{a} / {b}`, in tokens a second; `x` is {a} over {b}, and `-` is a row that side did not run.\n\n")
        for (section in [Section.prefill, Section.decode, Section.streams, Section.arms, Section.behind, Section.other]) {
            writer |> write(section_text(section, rows, ours, theirs))
        }
        writer |> write(refused_text(ours, theirs))
    }
    delete rows
    return text
}
