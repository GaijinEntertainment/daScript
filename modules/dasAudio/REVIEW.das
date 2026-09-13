options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasAudio/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang modules/dasAudio/REVIEW.das - exit 0 clean, 1 with findings.

// Where programs that require live/audio_live live.
let ROOTS <- ["examples", "tutorials", "modules", "tests", "utils"]

let LIVE_MODULE = "live/audio_live"

// The calls audio_live.das makes on a live program's behalf (ARCHITECTURE.md, "audio_live owns
// the audio system across a reload"); audio_live.das itself does not require its own module.
let RAW_CALLS <- ["audio_system_create(", "audio_system_finalize(", "set_audio_thread_command_stream("]

def private walk_das(root : string; var out : array<string>) {
    dir(root) $(name) {
        return if (name |> starts_with("."))
        let p = path_join(root, name)
        let st = stat(p)
        return if (!st.is_valid)
        if (st.is_dir) {
            walk_das(p, out)
        } elif (name |> ends_with(".das")) {
            out |> push(p)
        }
    }
}

def private check_live_clients {
    var files : array<string>
    for (root in ROOTS) {
        walk_das(root, files)
    }
    files |> sort
    for (path in files) {
        continue if (find_index(das_requires(path), LIVE_MODULE) < 0)
        let text = strip_line_comments(fread(path))
        for (call in RAW_CALLS) {
            let line = find_line(text, call)
            continue if (line == 0)
            gate_finding(path, line, "a program that requires {LIVE_MODULE} calls {call}...) directly; under a reload that tears down or double-references the stream audio_live carries over - use audio_live_system() / audio_live_finalize(), or drop the require when this program's audio is meant to die with a reload")
        }
    }
}

[export]
def main : int {
    if (!fexist("modules/dasAudio/REVIEW.das")) {
        to_log(LOG_ERROR, "modules/dasAudio/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_live_clients()
    return gate_verdict("modules/dasAudio/REVIEW.das")
}
