// Shared tier-1 vision oracle pieces for the gemma4uv / gemma4v encoder tests:
//   - the per-token line parser for llama-mtmd-debug's MTMD_DEBUG_EMBEDDINGS block
//   - the exact-value fixture generators (float-for-float from mtmd-debug.cpp)
//   - the per-token compare, logging the maxdiff against an absolute + scale-relative bar

options gen2
options _dasllama_internal = true

module _vision_oracle shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost
require math

struct OracleEncode {
    npos : int
    means : array<float>
    v0 : array<float>; v1 : array<float>; v2 : array<float>; v3 : array<float>
}

def private parse_field(part : string) : float {
    let eq = find(part, "=")
    return eq < 0 ? 0.0 : to_float(slice(part, eq + 1))
}

def load_oracle_encode(path : string; var o : OracleEncode) : bool {
    let text = fread(path)
    if (empty(text)) {
        return false
    }
    for (line in split(text, "\n")) {
        let sh = find(line, "Shape: [")
        if (sh >= 0) {
            let inner = slice(line, sh + length("Shape: ["))
            let comma = find(inner, ",")
            o.npos = to_int(strip(slice(inner, comma + 1, find(inner, "]"))))
            continue
        }
        let tk = find(line, " tok ")
        if (tk < 0 || find(line, "mean=") < 0) {
            continue
        }
        for (part in split(slice(line, tk + 1), " ")) {
            if (part |> starts_with("mean=")) {
                o.means |> push(parse_field(part))
            } elif (part |> starts_with("v0=")) {
                o.v0 |> push(parse_field(part))
            } elif (part |> starts_with("v1=")) {
                o.v1 |> push(parse_field(part))
            } elif (part |> starts_with("v2=")) {
                o.v2 |> push(parse_field(part))
            } elif (part |> starts_with("v3=")) {
                o.v3 |> push(parse_field(part))
            }
        }
    }
    return o.npos > 0 && length(o.means) == o.npos
}

// the -p encode fixture generators, float-for-float from mtmd-debug.cpp (interleaved RGB rows)
def make_planes(kind : string; w, h : int) : array<float> {
    var planes : array<float>
    planes |> resize(w * h * 3)
    if (kind == "gray") {
        for (v in planes) {
            v = 0.5
        }
    } elif (kind == "cb") {
        for (y in range(h)) {
            for (x in range(w)) {
                let v = ((x + y) % 2) != 0 ? 0.0 : 1.0
                let p = (y * w + x) * 3
                planes[p] = v
                planes[p + 1] = v
                planes[p + 2] = v
            }
        }
    } elif (kind == "red" || kind == "green" || kind == "blue") {
        let c = kind == "red" ? 0 : (kind == "green" ? 1 : 2)
        for (y in range(h)) {
            for (x in range(w)) {
                planes[(y * w + x) * 3 + c] = 1.0
            }
        }
    }
    // no rainbow arm: its generator runs atan2f, which is not float-portable across libms -
    // tier-1 fixtures must have exact-value generators (see mint.sh)
    return <- planes
}


// the per-token compare: mean and v0..v3 of every token against the dump, on the bar
// abs_tol + rel_tol * token_rms (token_rms = sqrt(mean(out[p]^2)) per token); logs the maxdiff under tag
def compare_encode(t : T?; tag : string; out : array<float>; d : int; o : OracleEncode; abs_tol, rel_tol : float) : float {
    var maxdiff = 0.0
    var max_tok = -1
    var max_excess = 0.0
    var nonfinite = 0
    for (p in range(o.npos)) {
        var mean = 0.0
        var sq = 0.0
        for (i in range(d)) {
            // NaN compares false against every bar, so a poisoned encode would PASS silently
            if (!is_finite(out[p * d + i])) {
                nonfinite++
            }
            mean += out[p * d + i]
            sq += out[p * d + i] * out[p * d + i]
        }
        mean /= float(d)
        let bound = abs_tol + rel_tol * sqrt(sq / float(d))
        for (diff in [abs(mean - o.means[p]), abs(out[p * d + 0] - o.v0[p]), abs(out[p * d + 1] - o.v1[p]),
                      abs(out[p * d + 2] - o.v2[p]), abs(out[p * d + 3] - o.v3[p])]) {
            if (diff > maxdiff) {
                maxdiff = diff
                max_tok = p
            }
            if (diff - bound > max_excess) {
                max_excess = diff - bound
            }
        }
    }
    to_log(LOG_INFO, "{tag}: {o.npos} tokens, maxdiff {maxdiff} at tok {max_tok} (bar {abs_tol} + {rel_tol} x rms)\n")
    if (nonfinite > 0) {
        t |> failure("{tag}: {nonfinite} non-finite output element(s) - the encode is poisoned, no bar applies")
    }
    if (max_excess > 0.0) {
        t |> failure("{tag}: worst diff exceeds {abs_tol} + {rel_tol}*rms by {max_excess} (maxdiff {maxdiff} at tok {max_tok}) - got ({out[max_tok * d]}, {out[max_tok * d + 1]}, ...) want ({o.v0[max_tok]}, {o.v1[max_tok]}, ...)")
    }
    return maxdiff
}
