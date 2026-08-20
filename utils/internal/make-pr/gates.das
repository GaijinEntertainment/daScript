options gen2

// make-pr gate-selection logic, extracted from main.das so test_gates.das can
// reach it (the pr-babysit / review-md companion-module pattern).

require strings
require daslib/fio
require daslib/strings_boost

let public GATE_NAMES <- fixed_array("sync", "review-md", "dupes", "ast-verify", "jit-smoke")

def public want_gate(only : string; skip : array<string>; gate : string) : bool {
    if (!empty(only)) {
        return only == gate
    }
    for (s in skip) {
        if (s == gate) {
            return false
        }
    }
    return true
}

def public unknown_gate_names(only : string; skip : array<string>) : array<string> {
    var bad <- [for (g in skip); g; where GATE_NAMES |> find_index(g) < 0]
    if (!empty(only) && GATE_NAMES |> find_index(only) < 0) {
        bad |> push(only)
    }
    return <- bad
}

def public scopes_of(files : array<string>) : array<string> {
    var seen : table<string>
    var scopes : array<string>
    for (f in files) {
        let parts <- split(to_generic_path(f), "/")
        if (length(parts) < 2) {
            continue
        }
        let scope = length(parts) == 2 ? parts[0] : "{parts[0]}/{parts[1]}"
        if (!key_exists(seen, scope)) {
            seen |> insert(scope)
            scopes |> push(scope)
        }
    }
    return <- scopes
}

def public any_path_matches(files : array<string>; patterns : array<string>) : bool {
    for (f in files) {
        let g = to_generic_path(f)
        for (p in patterns) {
            if (find(g, p) >= 0) {
                return true
            }
        }
    }
    return false
}

def public pick_newest(candidates : array<string>) : string {
    var best = ""
    var best_mtime = 0l
    for (c in candidates) {
        if (fexist(c)) {
            let st = stat(c)
            if (st.is_valid && int64(st.mtime) > best_mtime) {
                best_mtime = int64(st.mtime)
                best = c
            }
        }
    }
    return best
}
