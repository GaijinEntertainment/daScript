options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require strings
require math
require _model_tier   // models_dir() + model_available() gate

// Tier-1 gemma4uv embedder parity vs llama-mtmd-debug -p encode (CPU, --no-mmproj-offload):
// pre-resized [0,1] float fixtures skip preprocessing entirely, so a diff here isolates the
// embedder math. Oracle per-token lines live beside the models in gemma4-vision-oracle/
// (mint.sh regenerates); both the mmproj and the dumps gate this test with explicit skips.

struct OracleEncode {
    npos : int
    means : array<float>
    v0 : array<float>; v1 : array<float>; v2 : array<float>; v3 : array<float>
}

def private parse_field(part : string) : float {
    let eq = find(part, "=")
    return eq < 0 ? 0.0 : to_float(slice(part, eq + 1))
}

def private load_oracle_encode(path : string; var o : OracleEncode) : bool {
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
def private make_planes(kind : string; w, h : int) : array<float> {
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

// the tier-1 gate: encode a fixture, compare per-token mean/v0..v3 against the oracle dump.
// %.6f print rounding is ±5e-7 and the measured accumulation-order noise is ~2e-5 (logged per
// fixture); 2e-4 leaves 10x headroom while staying far under any structural failure
let TIER1_TOL = 2e-4

def private gate_encode(t : T?; emb : Gemma4uvEmbedder; var st : Gemma4uvState;
                        kind : string; w, h : int; logname : string) {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "gemma4-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via gemma4-vision-oracle/mint.sh")
        return
    }
    let planes <- make_planes(kind, w, h)
    var out : array<float>
    var npos = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        npos = gemma4uv_encode(emb, st, planes, int64(w), int64(h), out)
    }
    t |> equal(int(npos), o.npos, "{kind} {w}x{h} token count")
    if (int(npos) != o.npos) {
        return
    }
    var maxdiff = 0.0
    var max_tok = -1
    let d = int(emb.proj_dim)
    for (p in range(o.npos)) {
        var mean = 0.0
        for (i in range(d)) {
            mean += out[p * d + i]
        }
        mean /= float(d)
        for (diff in [abs(mean - o.means[p]), abs(out[p * d + 0] - o.v0[p]), abs(out[p * d + 1] - o.v1[p]),
                      abs(out[p * d + 2] - o.v2[p]), abs(out[p * d + 3] - o.v3[p])]) {
            if (diff > maxdiff) {
                maxdiff = diff
                max_tok = p
            }
        }
    }
    to_log(LOG_INFO, "gemma4uv tier-1 {kind} {w}x{h}: {o.npos} tokens, maxdiff {maxdiff} at tok {max_tok}\n")
    if (maxdiff > TIER1_TOL) {
        t |> failure("{kind} {w}x{h}: maxdiff {maxdiff} at tok {max_tok} exceeds {TIER1_TOL} - got ({out[max_tok * d]}, {out[max_tok * d + 1]}, ...) want ({o.v0[max_tok]}, {o.v1[max_tok]}, ...)")
    }
}

[test]
def test_gemma4uv_tier1(t : T?) {
    t |> run("tier-1 embedder parity vs the mtmd encode dumps") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        var emb <- load_gemma4uv_embedder(mm)
        var st = Gemma4uvState()
        tt |> equal(int(emb.patch), 48, "folded superpatch")
        tt |> equal(int(emb.patch_dim), 6912, "im2col row")
        tt |> equal(int(emb.n_embd), 3840, "embedding width")
        tt |> equal(int(emb.proj_dim), 3840, "soft-token width")
        tt |> equal(int(emb.pos_entries), 1120, "pos-table rows per axis")
        tt |> gate_encode(emb, st, "gray", 480, 480, "encode.gray480.log")
        tt |> gate_encode(emb, st, "cb", 336, 336, "encode.cb336.log")
        tt |> gate_encode(emb, st, "red", 336, 336, "encode.red336.log")
        tt |> gate_encode(emb, st, "green", 336, 336, "encode.green336.log")
        tt |> gate_encode(emb, st, "blue", 336, 336, "encode.blue336.log")
        // the non-square orientation / transposed-grid gate: identical cb patches make the
        // per-token values a pure fingerprint of pos_x[col] + pos_y[row]
        tt |> gate_encode(emb, st, "cb", 672, 336, "encode.cb672x336.log")
        // a grid wider than the pos table panics by name instead of reading past the plane
        var tripped = false
        try {
            let w = (emb.pos_entries + 1l) * emb.patch
            var planes : array<float>
            planes |> reserve(w * emb.patch * 3l)
            planes |> resize(w * emb.patch * 3l)
            var wide : array<float>
            gemma4uv_encode(emb, st, planes, w, emb.patch, wide)
            delete wide
            delete planes
        } recover {
            tripped = true
        }
        tt |> success(tripped, "a canvas past pos_entries columns panics at the grid check")
        delete st
        delete emb
    }
}
