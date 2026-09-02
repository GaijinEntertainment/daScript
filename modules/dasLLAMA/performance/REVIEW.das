options gen2

require strings
require daslib/fio
require daslib/defer
require daslib/json_boost
require dastest/review_gate

// The mechanical half of modules/dasLLAMA/performance/REVIEW.md (contract: REVIEW_COMMON.md
// at the repo root). Run from the repo root: bin/daslang modules/dasLLAMA/performance/REVIEW.das
// — exit 0 clean, 1 with findings.

let private SNAPSHOT = "modules/dasLLAMA/performance/last_known_good_sidecar.json"
let private SCHEMA = "modules/dasLLAMA/dasllama/dasllama_exchange_schema.das"
let private EXCHANGE_CLIENT = "modules/dasLLAMA/dasllama/dasllama_exchange.das"
let private MODULE_ROOT = "modules/dasLLAMA"

// the validator stays engine-free: its only dasllama/ require is the compile-time lint
// macro module - anything else compiles engine code into the dasllama.io ladder service
def private check_schema_engine_free {
    for (req in das_requires(SCHEMA)) {
        if (req |> starts_with("dasllama/") && req != "dasllama/dasllama_lint") {
            gate_finding(SCHEMA, "requires '{req}' - the exchange validator stays engine-free (dasllama_lint is the one sanctioned dasllama/ require)")
        }
    }
}

def private walk_das_files(folder : string; var out : array<string>) {
    dir(folder) $(name) {
        return if (name == "." || name == "..")
        let full = "{folder}/{name}"
        if (name |> ends_with(".das")) {
            out |> push(full)
        } elif (stat(full).is_dir) {
            walk_das_files(full, out)
        }
    }
}

// one exchange client: every HTTP call to the sidecar exchange goes through
// dasllama_exchange.das - a second dashv requirer under the module is a second HTTP path
def private check_single_exchange_client {
    var files : array<string>
    walk_das_files(MODULE_ROOT, files)
    for (f in files) {
        continue if (f == EXCHANGE_CLIENT)
        for (req in das_requires(f)) {
            if (req |> starts_with("dashv")) {
                gate_finding(f, "requires '{req}' - {EXCHANGE_CLIENT} is the single exchange client; a second HTTP path under the module is a defect")
            }
        }
    }
}

// last_known_good_sidecar.json invariants that hold at ANY age: a complete, honest mint.
// The refresh-only half (dasllama_version == current, from-the-box provenance) stays in
// REVIEW.md — it binds a refresh, and the standing snapshot may legitimately lag versions.
def private check_last_known_good {
    let text = fread(SNAPSHOT)
    if (empty(text)) {
        gate_finding(SNAPSHOT, "missing or unreadable - the exchange's seed snapshot must exist")
        return
    }
    var jerr = ""
    var doc = read_json(text, jerr)
    defer() {
        unsafe {
            delete doc
        }
    }
    if (doc == null) {
        gate_finding(SNAPSHOT, "does not parse as JSON: {jerr}")
        return
    }
    let noise = "{doc?["provenance"]?["noise"] ?? ""}"
    if (noise != "ok") {
        gate_finding(SNAPSHOT, "provenance.noise is '{noise}' - a last-known-GOOD snapshot carries a quiet mint (noise ok)")
    }
    let validation = "{doc?["provenance"]?["validation"] ?? ""}"
    if (validation != "ok") {
        gate_finding(SNAPSHOT, "provenance.validation is '{validation}' - a last-known-GOOD snapshot carries a validated mint")
    }
    let kernels = doc?["kernels"]
    let race = doc?["race"]
    if (kernels == null || !(kernels.value is _object) || race == null || !(race.value is _object)) {
        gate_finding(SNAPSHOT, "missing kernels/race sections - not a tune sidecar")
        return
    }
    for (name, entry in keys(race.value as _object), values(race.value as _object)) {
        let winner = "{entry?["winner"] ?? ""}"
        if (!key_exists(kernels.value as _object, name)) {
            gate_finding(SNAPSHOT, "race entry '{name}' has no kernels entry - a partial or hand-edited copy")
            continue
        }
        let stamped = "{kernels?[name] ?? ""}"
        if (winner != stamped) {
            gate_finding(SNAPSHOT, "race '{name}' winner '{winner}' != kernels value '{stamped}' - the two sections desynced (hand edit?)")
        }
    }
}

// the corpus half of REVIEW.md's write-time provenance rules that are decidable from tree
// state alone: every stored run measured with no remote-desktop session, every archived
// sidecar minted on a quiet box. (Ref-pin equality and stamp reachability bind the DIFF -
// historical rows legitimately carry older pins and shas - so they stay out of this sweep;
// reachability runs in make-pr's chain, where the orphan hazard appears.)
let private RECORDS_DIR = "modules/dasLLAMA/performance/records"
let private DEFAULTS_DIR = "modules/dasLLAMA/performance/defaults"

// the shipped defaults profiles: a quiet mint, a version pin, and NO commit stamp (a
// profile's validity is its pin + feature class; an engine_sha would be an orphan after
// every rebase and says nothing a reader can act on)
def private check_defaults_profiles {
    dir(DEFAULTS_DIR) $(name) {
        return if (!(name |> ends_with(".tune-defaults.json")))
        let full = "{DEFAULTS_DIR}/{name}"
        let text = fread(full)
        if (empty(text)) {
            gate_finding(full, "missing or unreadable")
            return
        }
        var jerr = ""
        var doc = read_json(text, jerr)
        defer() {
            unsafe {
                delete doc
            }
        }
        if (doc == null) {
            gate_finding(full, "does not parse as JSON: {jerr}")
            return
        }
        let noise = "{doc?["provenance"]?["noise"] ?? ""}"
        if (noise != "ok") {
            gate_finding(full, "provenance.noise is '{noise}' - a shipped profile carries a quiet mint (noise ok)")
        }
        if (empty("{doc?["provenance"]?["dasllama_version"] ?? ""}")) {
            gate_finding(full, "no provenance.dasllama_version - a profile with no version pin adopts on every build")
        }
        if (!empty("{doc?["provenance"]?["engine_sha"] ?? ""}")) {
            gate_finding(full, "carries provenance.engine_sha - a profile ships no commit stamp (tune_profile_export strips it)")
        }
        let kernels = doc?["kernels"]
        if (kernels == null || !(kernels.value is _object) || empty(kernels.value as _object)) {
            gate_finding(full, "no kernels section - not a profile")
        }
    }
}

def private check_records_corpus {
    dir(RECORDS_DIR) $(name) {
        return if (!(name |> ends_with(".json")) || name == "annotations.json")
        let full = "{RECORDS_DIR}/{name}"
        let text = fread(full)
        if (empty(text)) {
            gate_finding(full, "missing or unreadable")
            return
        }
        var jerr = ""
        var doc = read_json(text, jerr)
        defer() {
            unsafe {
                delete doc
            }
        }
        if (doc == null) {
            gate_finding(full, "does not parse as JSON: {jerr}")
            return
        }
        if (name |> find(".tune.") >= 0) {
            let noise = "{doc?["provenance"]?["noise"] ?? ""}"
            if (!empty(noise) && noise != "ok") {   // empty = archived before the field existed
                gate_finding(full, "provenance.noise is '{noise}' - an archived sidecar generation carries a quiet mint (noise ok)")
            }
            return
        }
        gate_home_paths(full)
        return if (!(doc.value is _array))
        for (model in doc.value as _array) {
            let runs = model?["runs"]
            continue if (runs == null || !(runs.value is _array))
            for (r in runs.value as _array) {
                let rd = "{r?["hardware"]?["remote_desktop"] ?? ""}"
                continue if (empty(rd))   // rows minted before the field existed
                if (rd != "off") {
                    let gguf = "{model?["gguf"] ?? "?"}"
                    gate_finding(full, "{gguf}: hardware.remote_desktop is '{rd}' - a stored run is measured with no remote-desktop session; re-mint")
                }
            }
        }
    }
}

[export]
def main() : int {
    if (!fexist(SNAPSHOT)) {
        to_log(LOG_ERROR, "modules/dasLLAMA/performance/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_last_known_good()
    check_defaults_profiles()
    check_schema_engine_free()
    check_single_exchange_client()
    check_records_corpus()
    return gate_verdict("dasllama-performance")
}
