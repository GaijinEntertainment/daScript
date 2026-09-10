options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/regex
require daslib/regex_boost
require dastest/review_gate

// The mechanical half of utils/watchdog/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang utils/watchdog/REVIEW.das - exit 0 clean, 1 with findings.

let private FOLDER = "utils/watchdog"
let private README = "utils/watchdog/README.md"

var private EMIT <- %regex~emit\("([a-z_]+)"%%
var private LISTED <- %regex~`([a-z_]+)`%%

//! every `emit("<name>"` in the folder's sources, name -> the file that writes it
def private emitted_events() : table<string; string> {
    var events : table<string; string>
    dir(FOLDER) $(file) {
        return if (!ends_with(file, ".das") || file == "REVIEW.das")
        let path = "{FOLDER}/{file}"
        let text = fread(path)
        var from = 0
        while (true) {
            let m = regex_search(EMIT, text, from)
            break if (m.x < 0)
            events[regex_group(EMIT, 1, text)] = path
            from = m.y
        }
    }
    return <- events
}

//! the names between `The events:` and `In-tree readers:` in the README's `## The log` section
def private listed_events(readme : string) : table<string> {
    var listed : table<string>
    let start = find(readme, "The events:")
    let stop = find(readme, "In-tree readers:")
    if (start < 0 || stop < start) {
        gate_finding(README, "the `## The log` section has no `The events:` list ending at `In-tree readers:`")
        return <- listed
    }
    let section = slice(readme, start, stop)
    var from = 0
    while (true) {
        let m = regex_search(LISTED, section, from)
        break if (m.x < 0)
        listed |> insert(regex_group(LISTED, 1, section))
        from = m.y
    }
    return <- listed
}

//! an `event` value the folder writes is in the README's list, and a listed one is written
def private check_events_listed {
    let readme = fread(README)
    var inscope listed <- listed_events(readme)
    var inscope emitted <- emitted_events()
    for (name, path in keys(emitted), values(emitted)) {
        if (!key_exists(listed, name)) {
            gate_finding(path, "emit(\"{name}\") is not in the `The events:` list of {README} - add it there, in this change")
        }
    }
    for (name in keys(listed)) {
        if (!key_exists(emitted, name)) {
            gate_finding(README, "`{name}` is in the `The events:` list but nothing under {FOLDER}/ emits it")
        }
    }
}

[export]
def main() : int {
    check_events_listed()
    return gate_verdict("watchdog")
}
