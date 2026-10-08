options gen2
options indenting = 4

require dastest/testing_boost public
require cache_probe_cycle   // the cycle under test (sibling)
require daslib/json_boost
require strings
require math

//! the cycle module is model-free: every request body it builds and every figure it reads off a table of answers
//! are checked here without a server

def private body_json(index : int; replies : array<string>) : JsonValue? {
    var err = ""
    return read_json(probe_body(index, replies, "probe-llm", 16), err)
}

//! the item count of the array under `key`, -1 when the key holds no array
def private count_at(js : JsonValue?; key : string) : int {
    let v = js?[key]
    return v != null && v.value is _array ? length(v.value as _array) : -1
}

//! message `i`'s `key` field of a request body, "" when absent
def private message_field(js : JsonValue?; i : int; key : string) : string => js?["messages"]?[i]?[key] ?? ""

def private role_at(js : JsonValue?; i : int) : string => message_field(js, i, "role")

def private content_at(js : JsonValue?; i : int) : string => message_field(js, i, "content")

[test]
def test_cache_probe_cycle_shape(t : T?) {
    t |> run("sixteen requests: eight first rounds on one opening, six later rounds replaying their first reply, one other system prompt, one repeat") @(t : T?) {
        let replies <- [for (i in range(PROBE_FIRST)); "reply {i + 1}"]
        var kinds : table<string; int>
        for (index in range(1, PROBE_REQUESTS + 1)) {
            kinds[probe_kind(index)] ++
        }
        t |> equal(kinds?["first"] ?? 0, PROBE_FIRST)
        t |> equal(kinds?["later"] ?? 0, PROBE_LATER)
        t |> equal(kinds?["other-system"] ?? 0, 1)
        t |> equal(kinds?["repeat"] ?? 0, 1)
        var first_system = ""
        var firsts : table<string; int>   // every first round asks its own question
        for (index in range(1, PROBE_FIRST + 1)) {
            var js = body_json(index, replies)
            t |> success(js != null, "request {index} is JSON")
            t |> equal(role_at(js, 0), "system")
            t |> equal(role_at(js, 1), "user")
            t |> equal(count_at(js, "messages"), 2, "a first round is system + user")
            t |> equal(count_at(js, "tools"), 3, "the tool set rides every request")
            t |> equal(js?["model"] ?? "", "probe-llm")
            t |> equal(int(js?["max_tokens"] ?? 0l), 16)
            if (index == 1) {
                first_system = content_at(js, 0)
            } else {
                t |> equal(content_at(js, 0), first_system, "one system prompt opens every house conversation")
            }
            firsts[content_at(js, 1)] ++
            delete_json(js)
        }
        t |> equal(length(firsts), PROBE_FIRST, "eight distinct questions")
        for (index in range(PROBE_FIRST + 1, PROBE_FIRST + PROBE_LATER + 1)) {
            let conv = probe_conversation(index)
            var js = body_json(index, replies)
            t |> equal(count_at(js, "messages"), 4, "a later round is system + user + assistant + user")
            t |> equal(content_at(js, 0), first_system)
            var opener = body_json(conv, replies)
            t |> equal(content_at(js, 1), content_at(opener, 1), "the later round reopens conversation {conv}'s question")
            t |> equal(role_at(js, 2), "assistant")
            t |> equal(content_at(js, 2), "reply {conv}", "... replays that conversation's first reply")
            t |> equal(role_at(js, 3), "user")
            t |> success(content_at(js, 3) != content_at(js, 1), "... and asks something new")
            delete_json(js)
            delete_json(opener)
        }
        var other = body_json(PROBE_REQUESTS - 1, replies)
        t |> success(content_at(other, 0) != first_system, "the other-system request opens on another system prompt")
        t |> equal(count_at(other, "tools"), 3, "... with the same tools")
        t |> equal(probe_conversation(PROBE_REQUESTS - 1), PROBE_FIRST + 1)
        delete_json(other)
        t |> equal(probe_body(PROBE_REQUESTS, replies, "probe-llm", 16), probe_body(1, replies, "probe-llm", 16), "the last request repeats the first byte for byte")
        t |> equal(probe_conversation(PROBE_REQUESTS), 1)
        let none : array<string>
        var bare = body_json(PROBE_FIRST + 1, none)
        t |> equal(content_at(bare, 2), "", "a later round with no recorded reply replays an empty assistant turn")
        delete_json(bare)
    }
}

[test]
def test_cache_probe_notes_and_kwargs(t : T?) {
    t |> run("every conversation's note is its own, long, and the same each time; a body builds the same twice; preserve_thinking rides the kwargs on request") @(t : T?) {
        let replies <- [for (i in range(PROBE_FIRST)); "reply {i + 1}"]
        var notes : table<string; int>
        for (conv in range(1, PROBE_FIRST + 2)) {
            let note = probe_note(conv)
            notes[note] ++
            var lines = 0
            peek_data(note) $(d) {
                for (b in d) {
                    lines += int(b) == '\n' ? 1 : 0
                }
            }
            t |> equal(lines, PROBE_NOTE_LINES + 1, "note {conv} carries its heading and {PROBE_NOTE_LINES} device lines")
            t |> success(length(note) > 1200, "note {conv} runs past a page of tokens ({length(note)} bytes)")
            t |> equal(probe_note(conv), note, "a note is the same text every time it is built")
        }
        t |> equal(length(notes), PROBE_FIRST + 1, "every conversation's note is its own")
        for (index in range(1, PROBE_REQUESTS + 1)) {
            t |> equal(probe_body(index, replies, "probe-llm", 16), probe_body(index, replies, "probe-llm", 16), "request {index} builds the same body twice - the second sweep repeats the first byte for byte")
        }
        var err = ""
        var kept = read_json(probe_body(PROBE_FIRST + 1, replies, "probe-llm", 16, true), err)
        t |> equal(kept?["chat_template_kwargs"]?["preserve_thinking"] ?? false, true, "the preserve_thinking form carries the kwarg")
        t |> equal(kept?["chat_template_kwargs"]?["enable_thinking"] ?? true, false, "... beside thinking off")
        delete_json(kept)
        t |> success(find(probe_body(PROBE_FIRST + 1, replies, "probe-llm", 16), "preserve_thinking") < 0, "the plain form sends no preserve_thinking")
    }
}

def private row(index : int; prompt, cached : int; ms : double; status : int = 200; sweep : int = 1) : ProbeRow =>
    ProbeRow(sweep = sweep, index = index, kind = probe_kind(index), conversation = probe_conversation(index), prompt = prompt, cached = cached, completion = 4, status = status, ms = ms, reply = "ok")

[test]
def test_cache_probe_summary(t : T?) {
    t |> run("the figures off a table of answers: shares by kind, the cold start, the openings attached, the other-system and repeat reads, the medians, the failures") @(t : T?) {
        var rows : array<ProbeRow>
        rows |> reserve(PROBE_REQUESTS)
        rows |> emplace(row(1, 200, 0, 900.0lf))    // the cold start
        for (index in range(2, PROBE_FIRST + 1)) {
            rows |> emplace(row(index, 200, 180, 100.0lf + double(index)))   // the opening attached
        }
        rows[4].cached = 0   // one first round that missed
        for (index in range(PROBE_FIRST + 1, PROBE_FIRST + PROBE_LATER + 1)) {
            rows |> emplace(row(index, 240, 230, 60.0lf))
        }
        rows |> emplace(row(PROBE_REQUESTS - 1, 190, 60, 300.0lf))
        rows |> emplace(row(PROBE_REQUESTS, 200, 199, 50.0lf))
        rows[9].cached = 180   // one later round that attached only the opening
        let s = probe_summary(rows, 64)
        t |> equal(s.cold_first, 2, "the cold start and the one miss")
        t |> equal(s.openings_attached, PROBE_FIRST - 2, "six of the seven first rounds after the cold start attached")
        t |> equal(s.tails_attached, PROBE_LATER - 1, "five later rounds attached past the 180-token opening")
        t |> equal(s.other_system_cached, 60)
        t |> equal(s.repeat_cached, 199)
        t |> equal(s.repeat_prompt, 200)
        t |> equal(s.failed, 0)
        // first rounds: 8 * 200 prompt, 6 * 180 cached
        t |> success(abs(s.cached_first - 1080.0lf / 1600.0lf) < 1e-9lf, "first-round share {s.cached_first}")
        t |> success(abs(s.cached_later - 1330.0lf / 1440.0lf) < 1e-9lf, "later-round share {s.cached_later}")
        let all_prompt = 1600.0lf + 1440.0lf + 190.0lf + 200.0lf
        let all_cached = 1080.0lf + 1330.0lf + 60.0lf + 199.0lf
        t |> success(abs(s.cached_all - all_cached / all_prompt) < 1e-9lf, "overall share {s.cached_all}")
        t |> success(abs(s.ms_first - 105.5lf) < 1e-9lf, "median first-round wall of 900,103..108: between 105 and 106 ({s.ms_first})")
        t |> success(abs(s.ms_later - 60.0lf) < 1e-9lf, "median later-round wall {s.ms_later}")
        t |> equal(s.warm_answered, 0, "no second sweep yet")
        t |> equal(s.warm_share, -1.0lf)
        let report = probe_report(rows, 64)
        t |> success(find(report, "first rounds with nothing cached: 2") >= 0, report)
        t |> success(find(report, "past the opening: 5 of 6") >= 0, report)
        t |> success(find(report, "199 of 200 cached") >= 0, report)
        t |> success(find(report, "not answered") < 0, "no failure line when every request answered")
        delete rows
    }
}

[test]
def test_cache_probe_warm(t : T?) {
    t |> run("the second sweep reads warm: its share, and every request that attached less than its prompt minus a page") @(t : T?) {
        var rows : array<ProbeRow>
        rows |> reserve(2 * PROBE_REQUESTS)
        for (index in range(1, PROBE_REQUESTS + 1)) {
            rows |> emplace(row(index, 1000, index == 1 ? 0 : 900, 100.0lf))
        }
        for (index in range(1, PROBE_REQUESTS + 1)) {
            rows |> emplace(row(index, 1000, 999, 30.0lf, 200, 2))   // warm: everything but the last token
        }
        rows[PROBE_REQUESTS + 2].cached = 960   // a dense model's page boundary: 40 short at 64-row pages is warm
        rows[PROBE_REQUESTS + 4].cached = 800   // the opening only: short of warm
        rows[PROBE_REQUESTS + 6].cached = 0     // a cold miss on the warm sweep
        let s = probe_summary(rows, 64)
        t |> equal(s.warm_answered, PROBE_REQUESTS)
        t |> equal(s.warm_short, 2, "the opening-only and the miss; the page-boundary read is warm")
        t |> success(abs(s.warm_share - (999.0lf * 13.0lf + 960.0lf + 800.0lf) / 16000.0lf) < 1e-9lf, "warm share {s.warm_share}")
        t |> success(abs(s.ms_warm - 30.0lf) < 1e-9lf)
        t |> success(abs(s.cached_first - (900.0lf * 7.0lf) / 8000.0lf) < 1e-9lf, "the first sweep's first-round share stays its own ({s.cached_first})")
        let wide = probe_summary(rows, 128)
        t |> equal(wide.warm_short, 2, "a wider page keeps both short reads short")
        let tight = probe_summary(rows, 8)
        t |> equal(tight.warm_short, 3, "an 8-row page makes the 40-short read short too")
        let report = probe_report(rows, 64)
        t |> success(find(report, "2 of 16 requests short of warm (under prompt - 64, the page)") >= 0, report)
        t |> success(find(probe_row_line(rows[PROBE_REQUESTS]), "2.01  first") >= 0, probe_row_line(rows[PROBE_REQUESTS]))
        delete rows
    }
    t |> run("a request the server refused counts as failed and leaves the shares to the answered ones") @(t : T?) {
        var rows <- [row(1, 100, 0, 500.0lf), row(2, -1, -1, 5.0lf, 500), row(3, 100, 90, 40.0lf)]
        let s = probe_summary(rows, 64)
        t |> equal(s.failed, 1)
        t |> equal(s.cold_first, 1, "the refused request is not a cold start")
        t |> equal(s.openings_attached, 1)
        t |> success(abs(s.cached_first - 0.45lf) < 1e-9lf, "share over the two answered ({s.cached_first})")
        t |> success(abs(s.ms_first - 270.0lf) < 1e-9lf, "median over the two answered ({s.ms_first})")
        t |> equal(s.cached_later, -1.0lf, "no later round answered")
        t |> success(find(probe_report(rows, 64), "requests not answered 200: 1") >= 0)
        t |> success(find(probe_row_line(rows[1]), "HTTP 500") >= 0, probe_row_line(rows[1]))
        t |> success(find(probe_row_line(rows[1]), "no usage") >= 0, "a reply with no usage says so: {probe_row_line(rows[1])}")
        t |> success(find(probe_row_line(rows[2]), "90 of 100 (90 %)") >= 0, probe_row_line(rows[2]))
        t |> success(starts_with(probe_row_line(rows[0]), "1.01  first"), "the index pads to two digits under its sweep: {probe_row_line(rows[0])}")
        delete rows
    }
    t |> run("the row line cuts a long reply at eight words; a median over an odd count is the middle value") @(t : T?) {
        var long_reply = row(1, 100, 50, 10.0lf)
        long_reply.reply = "one two three four five six seven eight nine ten"
        let line = probe_row_line(long_reply)
        t |> success(find(line, "one two three four five six seven eight ...") >= 0 && find(line, "nine") < 0, "eight words then an ellipsis: {line}")
        var rows <- [row(1, 100, 0, 30.0lf), row(2, 100, 90, 10.0lf), row(3, 100, 90, 200.0lf)]
        let s = probe_summary(rows, 64)
        t |> success(abs(s.ms_first - 30.0lf) < 1e-9lf, "the median of 30, 10, 200 is 30 ({s.ms_first})")
        delete rows
    }
}
