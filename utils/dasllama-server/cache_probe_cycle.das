options gen2
options indenting = 4

require daslib/json_boost   // JV / write_json_compact: the one JSON string escaper
require daslib/strings_boost   // join
require math

//! The sixteen-request cycle `cache_probe.das` sends twice and the figures read off what came back - no server here, so a
//! test runs it with a table of answers. The cycle: eight conversations open on one system prompt and one tool set (the
//! first prefills cold, the rest attach the shared opening); six of them come back with their first reply and a follow-up
//! (the whole earlier exchange attaches); one conversation opens on another system prompt with the same tools (what
//! attaches is the tool block when the template renders it ahead of the text, nothing otherwise); and the first request
//! repeats verbatim. Every user turn carries a house note of `PROBE_NOTE_LINES` lines, so each conversation's own tail runs
//! well past a page. The second pass sends the sixteen bodies again, byte for byte: everything is warm, and a request is
//! short of warm when it attached less than its prompt minus one page - a dense model's cache ends at a page boundary.

//! one request of the cycle: its pass and place, and the counts and time the server answered with
struct ProbeRow {
    sweep : int = 1         // 1 the cycle, 2 the same bodies again
    index : int
    kind : string           // first | later | other-system | repeat
    conversation : int      // 1..8 on the house prompt, 9 on the boat prompt
    prompt : int = -1
    cached : int = -1
    completion : int = -1
    status : int
    ms : double
    reply : string
}

//! the cycle's shares and counts over pass 1, the warm read over pass 2; a share is cached tokens over prompt tokens, -1
//! where no request of the kind answered
struct ProbeSummary {
    cached_all : double = -1.0lf
    cached_first : double = -1.0lf
    cached_later : double = -1.0lf
    cold_first : int        // first rounds with nothing cached; one is the cold start
    openings_attached : int // first rounds after the cold start that attached something
    tails_attached : int    // later rounds that attached more than the opening the first rounds share
    other_system_cached : int = -1
    repeat_cached : int = -1
    repeat_prompt : int = -1
    ms_first : double = -1.0lf  // medians
    ms_later : double = -1.0lf
    warm_share : double = -1.0lf  // pass 2
    warm_short : int        // pass 2 requests that attached less than prompt - page_rows
    warm_answered : int
    ms_warm : double = -1.0lf
    failed : int            // requests the server did not answer 200, both passes
}

let PROBE_REQUESTS = 16
let PROBE_FIRST = 8
let PROBE_LATER = 6
let PROBE_NOTE_LINES = 40

let private ROOMS <- ["kitchen", "bedroom", "study", "garden", "hall", "garage", "attic", "cellar", "porch", "pantry"]
let private THINGS <- ["ceiling light", "wall lights", "desk lamp", "sprinkler", "heater", "window blind", "door lock", "fan", "radio", "thermostat"]
let private STATES <- ["on", "off", "half", "open", "closed", "locked", "unlocked", "warm", "cool", "quiet"]

//! conversation `conv`'s house note: `PROBE_NOTE_LINES` device lines no other conversation shares, a few hundred tokens
def probe_note(conv : int) : string {
    return build_string() $(var w) {
        w |> write("House note {conv}, the devices as the family app lists them this morning:\n")
        for (i in range(PROBE_NOTE_LINES)) {
            let k = conv * 7 + i * 3
            w |> write("- {ROOMS[(k + i) % length(ROOMS)]} {THINGS[(k + conv) % length(THINGS)]} {i + 1}: {STATES[(k * 5 + i) % length(STATES)]}, checked {(k % 12) + 1}:{(k * 17) % 60 < 10 ? "0" : ""}{(k * 17) % 60}\n")
        }
    }
}

let private SYSTEM_HOUSE = "You are the assistant of a house with a kitchen, a bedroom, a study and a garden. Devices: the kitchen ceiling light, the bedroom wall lights, the study desk lamp, the garden sprinkler. A request to do something is a tool call; plain talk is for questions. Answer in one short sentence and nothing else."
let private SYSTEM_BOAT = "You are the assistant of a boat with a cabin, a galley and a deck. Devices: the cabin lamp, the galley light, the bilge pump. A request to do something is a tool call; plain talk is for questions. Answer in one short sentence and nothing else."

let private PROBE_TOOLS <- [
    "\{\"type\":\"function\",\"function\":\{\"name\":\"control\",\"description\":\"Switch a device or set its level.\",\"parameters\":\{\"type\":\"object\",\"properties\":\{\"point\":\{\"type\":\"string\",\"description\":\"device name\"},\"value\":\{\"type\":\"string\",\"description\":\"on, off or a level\"}},\"required\":[\"point\",\"value\"]}}}",
    "\{\"type\":\"function\",\"function\":\{\"name\":\"house_status\",\"description\":\"The lights that are on, the open windows, the temperatures.\",\"parameters\":\{\"type\":\"object\",\"properties\":\{\"query\":\{\"type\":\"string\",\"description\":\"lights, windows or temperature\"}},\"required\":[\"query\"]}}}",
    "\{\"type\":\"function\",\"function\":\{\"name\":\"not_for_me\",\"description\":\"The phrase was not addressed to the assistant.\",\"parameters\":\{\"type\":\"object\",\"properties\":\{},\"required\":[]}}}"
]

let private QUESTIONS <- [
    "What is the capital of France? One word.",
    "What is two plus two? One word.",
    "Name a primary color. One word.",
    "Which is heavier, a kilogram of feathers or a kilogram of iron?",
    "How many legs does a spider have? One word.",
    "Name the largest ocean. One word.",
    "What color is the sky on a clear day? One word.",
    "Which month comes after March? One word."
]

let private FOLLOW_UPS <- [
    "And its population, roughly?",
    "And three plus three?",
    "Name another one.",
    "Why?",
    "And an insect?",
    "And the smallest?",
    "And at sunset?",
    "And before it?"
]

def private message(role, content : string) : string => "\{\"role\":{sprint_json(role, false)},\"content\":{sprint_json(content, false)}}"

//! the kind of request `index` (1-based) is in the cycle
def probe_kind(index : int) : string {
    if (index <= PROBE_FIRST) {
        return "first"
    } elif (index <= PROBE_FIRST + PROBE_LATER) {
        return "later"
    } elif (index == PROBE_REQUESTS - 1) {
        return "other-system"
    }
    return "repeat"
}

//! the conversation request `index` belongs to: 1..8 on the house prompt, 9 on the boat prompt
def probe_conversation(index : int) : int {
    let kind = probe_kind(index)
    if (kind == "first") {
        return index
    } elif (kind == "later") {
        return index - PROBE_FIRST
    } elif (kind == "other-system") {
        return PROBE_FIRST + 1
    }
    return 1
}

//! the request body of request `index`; `replies` holds each house conversation's first reply by conversation (1-based),
//! which a later round replays as the assistant turn - an empty slot replays an empty turn; `preserve_thinking` asks the
//! server to render a replayed assistant turn with its think block, the way the generation prompt rendered it
def probe_body(index : int; replies : array<string>; model : string; max_tokens : int; preserve_thinking : bool = false) : string {
    let kind = probe_kind(index)
    let conv = probe_conversation(index)
    var msgs : array<string>
    if (kind == "other-system") {
        msgs |> push(message("system", SYSTEM_BOAT))
        msgs |> push(message("user", "{probe_note(conv)}\nIs the cabin lamp on? One word."))
    } else {
        msgs |> push(message("system", SYSTEM_HOUSE))
        msgs |> push(message("user", "{probe_note(conv)}\n{QUESTIONS[conv - 1]}"))
        if (kind == "later") {
            msgs |> push(message("assistant", conv <= length(replies) ? replies[conv - 1] : ""))
            msgs |> push(message("user", FOLLOW_UPS[conv - 1]))
        }
    }
    let kwargs = preserve_thinking ? "\{\"enable_thinking\":false,\"preserve_thinking\":true}" : "\{\"enable_thinking\":false}"
    let body = "\{\"model\":{sprint_json(model, false)},\"messages\":[{join(msgs, ",")}],\"tools\":[{join(PROBE_TOOLS, ",")}],\"max_tokens\":{max_tokens},\"temperature\":0,\"chat_template_kwargs\":{kwargs}}"
    delete msgs
    return body
}

def private share(rows : array<ProbeRow>; sweep : int; kind : string) : double {
    var prompt = 0
    var cached = 0
    for (r in rows) {
        if (r.sweep == sweep && (empty(kind) || r.kind == kind) && r.prompt > 0) {
            prompt += r.prompt
            cached += max(r.cached, 0)
        }
    }
    return prompt > 0 ? double(cached) / double(prompt) : -1.0lf
}

def private median_ms(rows : array<ProbeRow>; sweep : int; kind : string) : double {
    var ms <- [for (r in rows); r.ms; where r.sweep == sweep && (empty(kind) || r.kind == kind) && r.status == 200]
    if (empty(ms)) {
        return -1.0lf
    }
    sort(ms)
    let n = length(ms)
    return n % 2 == 1 ? ms[n / 2] : (ms[n / 2 - 1] + ms[n / 2]) * 0.5lf
}

//! the cycle's figures over the rows the server answered; `page_rows` is the cache's page, the slack a warm request may
//! fall short of its prompt by
def probe_summary(rows : array<ProbeRow>; page_rows : int) : ProbeSummary {
    var s = ProbeSummary(cached_all = share(rows, 1, ""), cached_first = share(rows, 1, "first"), cached_later = share(rows, 1, "later"),
                         ms_first = median_ms(rows, 1, "first"), ms_later = median_ms(rows, 1, "later"),
                         warm_share = share(rows, 2, ""), ms_warm = median_ms(rows, 2, ""))
    var opening = 0   // the most the first rounds after the cold start attached: the shared opening's length
    for (r in rows) {
        if (r.sweep == 1 && r.kind == "first" && r.index > 1 && r.status == 200) {
            opening = max(opening, r.cached)
        }
    }
    for (r in rows) {
        if (r.status != 200) {
            s.failed ++
            continue
        }
        if (r.sweep == 2) {
            s.warm_answered ++
            if (r.prompt > 0 && r.cached < r.prompt - page_rows) {
                s.warm_short ++
            }
            continue
        }
        if (r.kind == "first") {
            if (r.cached <= 0) {
                s.cold_first ++
            } elif (r.index > 1) {
                s.openings_attached ++
            }
        } elif (r.kind == "later") {
            if (r.cached > opening) {
                s.tails_attached ++
            }
        } elif (r.kind == "other-system") {
            s.other_system_cached = r.cached
        } elif (r.kind == "repeat") {
            s.repeat_cached = r.cached
            s.repeat_prompt = r.prompt
        }
    }
    return s
}

def private pct(share : double) : string => share < 0.0lf ? "n/a" : "{int(share * 100.0lf + 0.5lf)} %"

def private first_words(s : string; n : int) : string {
    var words <- split(s, " ")
    var kept <- [for (i in range(min(n, length(words)))); words[i]]
    let short = length(words) > n
    let text = join(kept, " ")
    delete words
    delete kept
    return short ? "{text} ..." : text
}

//! one line per row as the probe prints them while it runs
def probe_row_line(r : ProbeRow) : string {
    let cached = r.prompt > 0 ? "{r.cached} of {r.prompt} ({pct(double(max(r.cached, 0)) / double(r.prompt))})" : "no usage"
    let status = r.status == 200 ? "" : " HTTP {r.status}"
    return "{r.sweep}.{r.index < 10 ? "0" : ""}{r.index}  {r.kind}{repeat(" ", 13 - length(r.kind))}conv {r.conversation}  cached {cached}  {int(r.ms)} ms{status}  {first_words(r.reply, 8)}"
}

//! the summary block printed after both passes
def probe_report(rows : array<ProbeRow>; page_rows : int) : string {
    let s = probe_summary(rows, page_rows)
    return build_string() $(var w) {
        w |> write("pass 1, cached prompt tokens: all {pct(s.cached_all)}, first rounds {pct(s.cached_first)}, later rounds {pct(s.cached_later)}\n")
        w |> write("  first rounds with nothing cached: {s.cold_first} (one is the cold start)\n")
        w |> write("  first rounds after it that attached the shared opening: {s.openings_attached} of {PROBE_FIRST - 1}\n")
        w |> write("  later rounds that attached their own earlier exchange past the opening: {s.tails_attached} of {PROBE_LATER}\n")
        w |> write("  another system prompt, same tools: {s.other_system_cached} cached (the tool block, where the template puts it ahead of the text; near zero otherwise)\n")
        w |> write("  the first request repeated: {s.repeat_cached} of {s.repeat_prompt} cached\n")
        w |> write("  wall per request, median: first rounds {int(s.ms_first)} ms, later rounds {int(s.ms_later)} ms\n")
        w |> write("pass 2, the same bodies warm: {pct(s.warm_share)} cached, {s.warm_short} of {s.warm_answered} requests short of warm (under prompt - {page_rows}, the page), median {int(s.ms_warm)} ms\n")
        if (s.failed > 0) {
            w |> write("requests not answered 200: {s.failed}\n")
        }
    }
}
