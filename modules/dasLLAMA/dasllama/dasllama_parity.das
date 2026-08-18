options gen2
options _dasllama_internal = true

module dasllama_parity shared public

require dasllama/dasllama_lint public
require daslib/fio
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require daslib/strings_boost
require daslib/strings_convert
require strings
require math

// CPU-reference caches for parity instruments (the metal test suites + batch_parity_probe).
// A CPU truth is deterministic in (model file, prompts, steps) but costs minutes at 12B on the
// portable backend — computing it ONCE per model and mapping it afterwards is the same move as
// the prepared .dlim image. Caches live NEXT TO the model (`<model>.ref.<key>.tsv`), the key
// names the cell + its parameters, and `DASLLAMA_TRUTH_REFRESH=1` forces recompute+resave
// (bump/refresh when a cell's prompts or the CPU engine's numerics change on purpose).

def parity_ref_path(model_path, key : string) : string {
    return "{model_path}.ref.{key}.tsv"
}

def parity_refresh : bool {
    return g_env_engine.truth_refresh
}

def save_ref_ids(path : string; ids : array<int64>) {
    fopen(path, "wb") $(f) {
        if (f == null) {
            to_log(LOG_ERROR, "cannot write ref cache {path}\n")
            return
        }
        fwrite(f, build_string() $(var w) {
            w |> write("ids")
            for (v in ids) {
                w |> write(" ")
                w |> write(v)
            }
            w |> write("\n")
        })
    }
}

def load_ref_ids(path : string; var ids : array<int64>) : bool {
    if (!stat(path).is_valid) {
        return false
    }
    var ok = false
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        for (line in split(fread(f), "\n")) {
            if (line |> starts_with("ids")) {
                for (tk in split_by_chars(slice(line, 4), " ")) {
                    let parsed = try_to_int64(tk)
                    if (is_err(parsed)) {   // corrupt/truncated cache — reject it whole, the caller recomputes+resaves
                        ids |> clear()
                        return
                    }
                    ids |> push(unwrap(parsed))
                }
            }
        }
        ok = !empty(ids)
    }
    return ok
}

def save_ref_vals(path : string; vals : array<float>) {
    fopen(path, "wb") $(f) {
        if (f == null) {
            to_log(LOG_ERROR, "cannot write ref cache {path}\n")
            return
        }
        var at = 0
        while (at < length(vals)) {
            let hi = min(at + 4096, length(vals))
            let lo = at
            fwrite(f, build_string() $(var w) {
                w |> write("vals")
                for (i in range(lo, hi)) {
                    w |> write(" ")
                    w |> write(fmt(":.9g", vals[i]))
                }
                w |> write("\n")
            })
            at = hi
        }
    }
}

def load_ref_vals(path : string; var vals : array<float>) : bool {
    if (!stat(path).is_valid) {
        return false
    }
    var ok = false
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        for (line in split(fread(f), "\n")) {
            if (line |> starts_with("vals")) {
                for (tk in split_by_chars(slice(line, 5), " ")) {
                    let parsed = try_to_float(tk)
                    if (is_err(parsed)) {   // corrupt/truncated cache — reject it whole, the caller recomputes+resaves
                        vals |> clear()
                        return
                    }
                    vals |> push(unwrap(parsed))
                }
            }
        }
        ok = !empty(vals)
    }
    return ok
}

//! Load the cached CPU reference for (model, key), or run `compute` once and cache its result.
def cached_ids(model_path, key : string; compute : block<() : array<int64>>) : array<int64> {
    let path = parity_ref_path(model_path, key)
    var ids : array<int64>
    if (!parity_refresh() && load_ref_ids(path, ids)) {
        return <- ids
    }
    ids |> clear()
    var fresh <- invoke(compute)
    save_ref_ids(path, fresh)
    return <- fresh
}

//! The float-vector twin (final-step logits references and similar).
def cached_vals(model_path, key : string; compute : block<() : array<float>>) : array<float> {
    let path = parity_ref_path(model_path, key)
    var vals : array<float>
    if (!parity_refresh() && load_ref_vals(path, vals)) {
        return <- vals
    }
    vals |> clear()
    var fresh <- invoke(compute)
    save_ref_vals(path, fresh)
    return <- fresh
}
