options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options indenting = 4

// The renderer half of the profiling suite: reads every committed profile_llm_<box>.json and emits the
// LLM CPU scoreboard (results_llm.md) via daslib/md_boost. One section per box (platform header + a
// per-model table of das vs llama.cpp prefill/emission tok/s, winners bolded). results_llm.md is
// GENERATED — never hand-edited; the JSON is the single source of truth.
//   bin/daslang modules/dasLLAMA/performance/gen_results.das

require profile_common
require daslib/md_boost
require daslib/fio
require strings
require llvm/daslib/llvm_tune   // [tune_policy]: the renderer reads JSON only — never auto-tune

let PERF_DIR = "modules/dasLLAMA/performance"
let RESULTS_PATH = "modules/dasLLAMA/performance/results_llm.md"

// All committed per-box LLM profiles, sorted so the .md ordering is stable across runs.
def list_llm_profiles() : array<string> {
    var out : array<string>
    dir(PERF_DIR) $(fn) {
        if (starts_with(fn, "profile_llm_") && ends_with(fn, ".json")) {
            out |> push("{PERF_DIR}/{fn}")
        }
    }
    sort(out)
    return <- out
}

// tok/s cell (1 decimal), or "-" when the value is absent (0 = unmeasured reference). ASCII, not an
// em dash, so the raw .md stays byte-aligned (render_md_table sizes columns by byte length).
def cell_tps(v : double) : string {
    return v > 0.0lf ? "{v:.1f}" : "-"
}

// das / lcpp ratio cell (higher das = faster => >1 wins), bolded on a das win. "-" if no reference.
def cell_ratio(das, lcpp : double) : string {
    if (lcpp <= 0.0lf || das <= 0.0lf) {
        return "-"
    }
    let r = das / lcpp
    let s = "{r:.2f}x"
    return r >= 1.0lf ? md_bold(s) : s
}

// One box's section: the platform header line + the per-model table.
def render_section(p : LlmProfile) : string {
    let pl = p.platform
    var rows : array<array<string>>
    // stable: ascending by active size
    var tests := p.tests
    sort(tests) $(a, b) => a.active_b < b.active_b
    var pctx = 512
    var ectx = 128
    if (!empty(tests)) {
        pctx = tests[0].prefill.context
        ectx = tests[0].emission.context
    }
    rows |> reserve(length(tests))
    for (t in tests) {
        rows |> emplace([
            t.model,
            "{t.active_b:.2f}",
            t.quant,
            cell_tps(t.prefill.das_tok_s),
            cell_tps(t.prefill.llama_cpp_tok_s),
            cell_ratio(t.prefill.das_tok_s, t.prefill.llama_cpp_tok_s),
            cell_tps(t.emission.das_tok_s),
            cell_tps(t.emission.llama_cpp_tok_s),
            cell_ratio(t.emission.das_tok_s, t.emission.llama_cpp_tok_s)
        ])
    }
    let headers <- [
        "model", "active B", "quant",
        "pp{pctx} das", "pp{pctx} lcpp", "das/lcpp",
        "tg{ectx} das", "tg{ectx} lcpp", "das/lcpp"
    ]
    let aligns <- [
        ColAlign.left, ColAlign.right, ColAlign.left,
        ColAlign.right, ColAlign.right, ColAlign.right,
        ColAlign.right, ColAlign.right, ColAlign.right
    ]
    let parsec = pl.parsec_off ? "Parsec off" : "Parsec ON"
    let aff = empty(pl.affinity) ? "" : " Affinity: {pl.affinity}."
    return build_string() $(w) {
        w |> write("## {pl.cpu}, {pl.threads} threads — daslang {pl.daslang}, llama.cpp {pl.llama_cpp}, {pl.date} ({parsec})\n\n")
        w |> write("{pl.os}; box profile `{pl.box_profile}`.{aff}\n\n")
        w |> write(render_md_table(headers, aligns, rows))
        w |> write("\n")
    }
}

def render_doc(profiles : array<LlmProfile>) : string {
    return build_string() $(w) {
        w |> write("# LLM CPU scoreboard (living doc — CURRENT numbers only; history in git)\n\n")
        w |> write("GENERATED from `profile_llm_<box>.json` by `gen_results.das` — do not hand-edit. ")
        w |> write("das = dasLLAMA (JIT; quant column = the GGUF's format, q4_k_m loads kq-native); llama.cpp = `llama-bench` CPU (`-ngl 0`, no Accelerate/AMX BLAS). ")
        w |> write("pp*N* = prefill throughput over *N* prompt tokens; tg*N* = greedy generation of *N* tokens. ")
        w |> write("Ratio = das / llama.cpp (>1 = das faster); winners bold.\n\n")
        for (p in profiles) {
            w |> write(render_section(p))
            w |> write("\n")
        }
    }
}

// ===== ASR scoreboard: regenerated IN PLACE in benchmarks/asr/results.md, prose PRESERVED =====
//
// results.md keeps its hand-authored prose (method preamble, per-model narrative, Correctness,
// Changelog). Each generated table sits between a "<!-- GEN:asr <model> <box> -->" / "<!-- /GEN:asr
// ... -->" marker pair; this renderer swaps ONLY the marked spans, leaving everything else verbatim.
// A section with no marker (a model/box not yet enrolled) is left untouched — enrol it by adding the
// marker pair around its table. The das/ref cell bolds a das WIN; ASR ms is lower=faster, so a das
// win is a ratio BELOW 1 (the opposite of the LLM tok/s scoreboard).

let ASR_RESULTS_PATH = "modules/dasLLAMA/benchmarks/asr/results.md"

def list_asr_profiles() : array<string> {
    var out : array<string>
    dir(PERF_DIR) $(fn) {
        if (starts_with(fn, "profile_asr_") && ends_with(fn, ".json")) {
            out |> push("{PERF_DIR}/{fn}")
        }
    }
    sort(out)
    return <- out
}

// "modules/.../profile_asr_m1.json" -> "m1"
def box_from_path(path : string) : string {
    var b = base_name(path)
    if (starts_with(b, "profile_asr_")) {
        b = slice(b, length("profile_asr_"))
    }
    if (ends_with(b, ".json")) {
        b = slice(b, 0, length(b) - length(".json"))
    }
    return b
}

// Replace the text between "<!-- GEN:{key} -->" and "<!-- /GEN:{key} -->" with `body`; `found` reports
// whether the marker pair existed (doc returned unchanged if not).
def replace_region(doc, key, body : string; var found : bool&) : string {
    let open = "<!-- GEN:{key} -->"
    let close = "<!-- /GEN:{key} -->"
    let a = find(doc, open)
    let b = find(doc, close)
    found = a >= 0 && b > a
    if (!found) {
        return doc
    }
    let head = slice(doc, 0, a + length(open))
    let tail = slice(doc, b)
    return "{head}\n{body}\n{tail}"
}

def cell_ms(v : double) : string {
    return v > 0.0lf ? "{v:.0f}" : "-"
}

// das/ref ms ratio (lower das = faster => <1 wins), bold on a das win. "-" if no reference.
def cell_ms_ratio(das, ref : double) : string {
    if (das <= 0.0lf || ref <= 0.0lf) {
        return "-"
    }
    let s = "{das / ref:.2f}x"
    return das < ref ? md_bold(s) : s
}

def cell_xrt(audio_s, ms : double) : string {
    return (ms > 0.0lf && audio_s > 0.0lf) ? "{audio_s * 1000.0lf / ms:.1f}" : "-"
}

def bucket_ref_ms(b : AsrBucket; tool : string) : double {
    for (r in b.refs) {
        if (r.tool == tool) {
            return r.ms
        }
    }
    return -1.0lf
}

def bucket_ref_enc(b : AsrBucket; tool : string) : double {
    for (r in b.refs) {
        if (r.tool == tool) {
            return r.encode_ms
        }
    }
    return -1.0lf
}

// Whether to render the encode-split columns. Gated on the DAS split being present: the JSON retains
// the mtmd reference encode_ms regardless, but until the das-side encode is wired (asr_prof, a TODO)
// a lone "ref enc / das -" column pair reads as incomplete — so the encode story stays in prose.
def has_encode(m : AsrRec) : bool {
    for (b in m.buckets) {
        if (b.das_encode_ms > 0.0lf) {
            return true
        }
    }
    return false
}

// One model's uniform table: file | audio s | das ms | <ref ms>… | <das/ref>… | das xRT | (enc cols).
def render_asr_table(m : AsrRec) : string {
    let enc = has_encode(m)
    let nr = length(m.references)
    let ncols = 3 + nr + nr + 1 + (enc ? 1 + nr : 0)
    var headers : array<string>
    var aligns : array<ColAlign>
    headers |> reserve(ncols)
    aligns |> reserve(ncols)
    headers |> push("file")
    headers |> push("audio s")
    headers |> push("das ms")
    for (t in m.references) {
        headers |> push("{t} ms")
    }
    for (t in m.references) {
        headers |> push("das/{t}")
    }
    headers |> push("das xRT")
    if (enc) {
        headers |> push("das enc ms")
        for (t in m.references) {
            headers |> push("{t} enc ms")
        }
    }
    aligns |> push(ColAlign.left)
    for (_i in range(ncols - 1)) {
        aligns |> push(ColAlign.right)
    }
    var rows : array<array<string>>
    rows |> reserve(length(m.buckets))
    for (b in m.buckets) {
        var row : array<string>
        row |> reserve(ncols)
        row |> push(b.wav)
        row |> push("{b.audio_s:.0f}")
        row |> push(cell_ms(b.das_ms))
        for (t in m.references) {
            row |> push(cell_ms(bucket_ref_ms(b, t)))
        }
        for (t in m.references) {
            row |> push(cell_ms_ratio(b.das_ms, bucket_ref_ms(b, t)))
        }
        row |> push(cell_xrt(b.audio_s, b.das_ms))
        if (enc) {
            row |> push(cell_ms(b.das_encode_ms))
            for (t in m.references) {
                row |> push(cell_ms(bucket_ref_enc(b, t)))
            }
        }
        rows |> emplace(row)
    }
    return render_md_table(headers, aligns, rows)
}

// The LibriSpeech dictation aggregate (mean / p50 / p95 per side); das rows bolded.
def render_libri(l : LibriAggregate) : string {
    let headers <- ["side", "mean ms", "p50", "p95"]
    let aligns <- [ColAlign.left, ColAlign.right, ColAlign.right, ColAlign.right]
    var rows : array<array<string>>
    rows |> reserve(length(l.sides))
    for (s in l.sides) {
        let win = starts_with(s.side, "das ")
        rows |> emplace([
            s.side,
            win ? md_bold("{s.mean_ms:.0f}") : "{s.mean_ms:.0f}",
            win ? md_bold("{s.p50_ms:.0f}") : "{s.p50_ms:.0f}",
            win ? md_bold("{s.p95_ms:.0f}") : "{s.p95_ms:.0f}"
        ])
    }
    return render_md_table(headers, aligns, rows)
}

// One generated table's italic platform sub-line (date + rev live here so they auto-update).
def asr_platform_line(pl : PlatformInfo) : string {
    let parsec = pl.parsec_off ? "Parsec off" : "Parsec ON"
    return "_{pl.cpu}, {pl.threads} threads — daslang {pl.daslang}, {pl.date} ({parsec})._"
}

// Splice every box's generated ASR tables into results.md (prose preserved). Returns regions replaced.
def regen_asr(profiles : array<string>) : int {
    var doc = fread(ASR_RESULTS_PATH)
    var nrepl = 0
    for (path in profiles) {
        var p : AsrProfile
        if (!sscan_json(fread(path), p)) {
            print("skip: {path} did not parse as an AsrProfile\n")
            continue
        }
        let box = box_from_path(path)
        for (m in p.tests) {
            var found = false
            let body = "{asr_platform_line(p.platform)}\n\n{render_asr_table(m)}"
            doc = replace_region(doc, "asr {m.model} {box}", body, found)
            if (found) {
                nrepl++
            } else {
                print("  no marker for: asr {m.model} {box}\n")
            }
        }
        if (!empty(p.librispeech.sides)) {
            var found = false
            doc = replace_region(doc, "asr librispeech {box}", render_libri(p.librispeech), found)
            if (found) {
                nrepl++
            } else {
                print("  no marker for: asr librispeech {box}\n")
            }
        }
    }
    fopen(ASR_RESULTS_PATH, "wb") $(f) {
        if (f != null) {
            fwrite(f, doc)
        }
    }
    return nrepl
}

// fallback: the renderer reads committed JSON — das gen-kernels never execute, so an
// untuned box must NOT trigger the (minutes-long) auto-tune the measuring tools get
[export, tune_policy(missing = "fallback")]
def main() {
    // ---- LLM scoreboard (fully generated file) ----
    var profiles : array<LlmProfile>
    for (path in list_llm_profiles()) {
        var p : LlmProfile
        if (sscan_json(fread(path), p)) {
            profiles |> emplace(p)
        } else {
            print("skip: {path} did not parse as an LlmProfile\n")
        }
    }
    if (empty(profiles)) {
        print("no profile_llm_*.json under {PERF_DIR} — skipping LLM scoreboard\n")
    } else {
        let doc = render_doc(profiles)
        fopen(RESULTS_PATH, "wb") $(f) {
            if (f != null) {
                fwrite(f, doc)
            }
        }
        print("wrote {length(profiles)} box section(s) -> {RESULTS_PATH}\n")
    }

    // ---- ASR scoreboard (regenerated in place, prose preserved) ----
    let asr <- list_asr_profiles()
    if (empty(asr)) {
        print("no profile_asr_*.json under {PERF_DIR} — skipping ASR scoreboard\n")
    } else {
        let n = regen_asr(asr)
        print("asr: replaced {n} region(s) in {ASR_RESULTS_PATH}\n")
    }
}
