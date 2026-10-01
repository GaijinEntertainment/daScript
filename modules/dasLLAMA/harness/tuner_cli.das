options gen2
options _dasllama_internal = true

module tuner_cli shared public

//! The flag spec shared by the dasllama tuner and both of its halves, so one declaration is
//! the contract for all three. One race protocol: ``--tune-paranoid`` is accepted and runs
//! it; the debugging concession is `daspkg release --quick`, which accepts the existing
//! sidecar and races nothing. Beside it, the halves' shared mint plumbing: the incumbent
//! reader, the confirm-vehicle picker, the child-output readers and the mint's exit and
//! failure wording.

require dasllama/dasllama_lint public
require dasllama/dasllama_env
require daslib/clargs
require daslib/fio
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require strings
require math

[CommandLineArgs]
struct TunerArgs {
    @clarg_name = "tune-paranoid"
    @clarg_doc = "Accepted for compatibility; the tuner runs one margin-decided protocol and this flag no longer changes the budget"
    tune_paranoid : bool
}

//! Exit code of a `--tune-only` half whose filter matched nothing here: the wrapper fails the mint
//! only when BOTH halves report it (a filter naming one half's family is not a typo).
let TUNE_ONLY_NO_MATCH = 3

def tuner_paranoid_requested() : bool {
    //! True when ``--tune-paranoid`` is on this tuner's command line (a parse failure reads
    //! as not set). The flag changes nothing today; the halves only note it.
    var r <- parse_args(type<TunerArgs>, get_user_args())
    if (r |> is_err) {
        return false
    }
    let cfg <- r |> move_unwrap
    return cfg.tune_paranoid
}

//! The previous sidecar's "kernels" section at `path`, read raw - a stale sidecar still names the
//! seats; empty when the file is absent or unreadable.
def read_incumbents(path : string) : table<string; string> {
    var seats : table<string; string>
    let text = fread(path)
    return <- seats if (empty(text))
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc != null && doc is _object) {
        var ks = doc?["kernels"]
        if (ks != null && ks is _object) {
            for (k, v in keys(ks as _object), values(ks as _object)) {
                if (v != null && v is _string) {
                    seats[k] = clone_string(v as _string)
                }
            }
        }
    }
    delete_json(doc)
    return <- seats
}

//! The confirm vehicle: `env_value` when the operator set `env_name` (said as pinning the `what`),
//! else the first model by name in the models dir that `vehicle(dir, name)` accepts; "" when none.
def pick_vehicle(env_value, env_name, what : string; vehicle : block<(md, name : string) : bool>) : string {
    if (!empty(env_value)) {
        print("confirm: {env_name}={env_value} - the operator pins the {what}\n")
        return env_value
    }
    let md = models_dir_resolved()
    var best = ""
    dir(md) $(name : string) {
        return if (!invoke(vehicle, md, name))
        if (empty(best) || name < best) {
            best = name
        }
    }
    return empty(best) ? "" : "{md}{best}"
}

//! The first token after `tag` on the first line of `outp` carrying it (the rest of that line when
//! it starts with a space); "" when no line carries it.
def value_after(outp, tag : string) : string {
    for (ln in split(outp, "\n")) {
        let ix = find(ln, tag)
        if (ix >= 0) {
            let rest = slice(ln, ix + length(tag))
            let sp = find(rest, " ")
            return sp > 0 ? slice(rest, 0, sp) : rest
        }
    }
    return ""
}

//! The last `n` lines of a child's output, the non-empty ones, each as a `confirm:   | ` line.
def print_child_tail(outp : string; n : int) {
    var lns <- split(outp, "\n")
    let total = length(lns)
    for (i in range(max(0, total - n), total)) {
        if (!empty(lns[i])) {
            print("confirm:   | {lns[i]}\n")
        }
    }
    delete lns
}

//! The line a mint prints when the sidecar at `path` would not take its write.
def mint_write_failure(path : string) : string {
    return "FAILED to write {path} - if the app dir is read-only, set DAS_TUNE_MANIFEST to a writable path\n"
}
