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

// STRICT box-store lister: records/{box}.json only — a dotted stem is a tune sidecar
// (m1.tune.<sha>.json) and annotations.json is the hand-note overlay, not a store.
def list_asr_stores() : array<string> {
    var out : array<string>
    dir("{PERF_DIR}/records") $(fn) {
        if (ends_with(fn, ".json")) {
            let stem = slice(fn, 0, length(fn) - length(".json"))
            if (stem != "annotations" && find(stem, ".") < 0) {
                out |> push("{PERF_DIR}/records/{fn}")
            }
        }
    }
    sort(out)
    return <- out
}

// "modules/.../records/m1.json" -> "m1"
def store_box(path : string) : string {
    let b = base_name(path)
    return ends_with(b, ".json") ? slice(b, 0, length(b) - length(".json")) : b
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

def test_ms(r : BenchRun; key : string) : double {
    return r.tests?[key]?.ms ?? -1.0lf
}

def test_audio(r : BenchRun; key : string) : double {
    return r.tests?[key]?.audio_s ?? -1.0lf
}

def test_enc(r : BenchRun; key : string) : double {
    return r.tests?[key]?.encode_ms ?? -1.0lf
}

def is_asr_run(r : BenchRun) : bool {
    return r.workload == "asr" || r.workload == "audio-chat"
}

// the model's das ASR run index, or -1 — the row source every table hangs off
def das_asr_run(m : BenchModel) : int {
    for (i in range(length(m.runs))) {
        if (m.runs[i].engine == "das" && is_asr_run(m.runs[i]) && m.runs[i].backend == "cpu") {
            return i
        }
    }
    return -1
}

// reference runs for the table, catalog order first (spec.refs → engine names), then any
// other non-das ASR runs the store carries that the catalog does not name
def asr_ref_runs(m : BenchModel; spec : AsrModelSpec) : array<int> {
    var out : array<int>
    for (rf in spec.refs) {
        let want = asr_engine_name(rf.tool)
        for (i in range(length(m.runs))) {
            if (m.runs[i].engine == want && is_asr_run(m.runs[i])) {
                out |> push(i)
            }
        }
    }
    for (i in range(length(m.runs))) {
        if (m.runs[i].engine != "das" && is_asr_run(m.runs[i]) && find_index(out, i) < 0) {
            out |> push(i)
        }
    }
    return <- out
}

// a reference column's header stem: engine plus its precision tag when the run carries one
def ref_label(r : BenchRun) : string {
    return empty(r.exec_fmt) ? r.engine : "{r.engine} {r.exec_fmt}"
}

// Whether to render the encode-split columns — gated on the DAS split being present so a lone
// "ref enc / das -" pair never reads as incomplete coverage.
def has_encode(dr : BenchRun; wavs : array<string>) : bool {
    for (w in wavs) {
        if (test_enc(dr, "asr:{w}") > 0.0lf) {
            return true
        }
    }
    return false
}

// One model's uniform table: file | audio s | das ms | <ref ms>… | <das/ref>… | das xRT | (enc cols).
def render_asr_table(m : BenchModel; dr : BenchRun; refis : array<int>; wavs : array<string>) : string {
    let enc = has_encode(dr, wavs)
    let nr = length(refis)
    let ncols = 3 + nr + nr + 1 + (enc ? 1 + nr : 0)
    var headers : array<string>
    var aligns : array<ColAlign>
    headers |> reserve(ncols)
    aligns |> reserve(ncols)
    headers |> push("file")
    headers |> push("audio s")
    headers |> push("das ms")
    for (i in refis) {
        headers |> push("{ref_label(m.runs[i])} ms")
    }
    for (i in refis) {
        headers |> push("das/{m.runs[i].engine}")
    }
    headers |> push("das xRT")
    if (enc) {
        headers |> push("das enc ms")
        for (i in refis) {
            headers |> push("{m.runs[i].engine} enc ms")
        }
    }
    aligns |> push(ColAlign.left)
    for (_i in range(ncols - 1)) {
        aligns |> push(ColAlign.right)
    }
    var rows : array<array<string>>
    rows |> reserve(length(wavs))
    for (w in wavs) {
        let key = "asr:{w}"
        let das_ms = test_ms(dr, key)
        if (das_ms <= 0.0lf) {
            continue
        }
        let audio_s = test_audio(dr, key)
        var row : array<string>
        row |> reserve(ncols)
        row |> push(w)
        row |> push("{audio_s:.0f}")
        row |> push(cell_ms(das_ms))
        for (i in refis) {
            row |> push(cell_ms(test_ms(m.runs[i], key)))
        }
        for (i in refis) {
            row |> push(cell_ms_ratio(das_ms, test_ms(m.runs[i], key)))
        }
        row |> push(cell_xrt(audio_s, das_ms))
        if (enc) {
            row |> push(cell_ms(test_enc(dr, key)))
            for (i in refis) {
                row |> push(cell_ms(test_enc(m.runs[i], key)))
            }
        }
        rows |> emplace(row)
    }
    return render_md_table(headers, aligns, rows)
}

// The LibriSpeech dictation aggregate — TOTALS per side (the store carries one aggregate key,
// not per-clip rows; the old mean/p50/p95 died with the per-clip profile shape).
def render_libri(m : BenchModel; dr : BenchRun; refis : array<int>) : string {
    let key = libri_test_key()
    let headers <- ["side", "total ms", "xRT"]
    let aligns <- [ColAlign.left, ColAlign.right, ColAlign.right]
    let audio = test_audio(dr, key)
    var rows : array<array<string>>
    rows |> emplace([md_bold("das"), md_bold("{test_ms(dr, key):.0f}"), md_bold(cell_xrt(audio, test_ms(dr, key)))])
    for (i in refis) {
        let ms = test_ms(m.runs[i], key)
        if (ms > 0.0lf) {
            rows |> emplace([ref_label(m.runs[i]), "{ms:.0f}", cell_xrt(audio, ms)])
        }
    }
    return render_md_table(headers, aligns, rows)
}

// One generated table's italic platform sub-line (precision + date + rev live here so they
// auto-update with every sweep).
def asr_platform_line(dr : BenchRun) : string {
    let fmt = empty(dr.exec_fmt) ? "" : " — das {dr.exec_fmt}"
    return "_{dr.hardware.cpu}, {dr.threads} threads{fmt} — daslang {dr.version} {dr.sha}, {dr.date}._"
}

// Splice every box store's generated ASR tables into results.md (prose preserved). Returns
// regions replaced. Markers are "asr {display} {box}" — enrolment stays add-the-marker-pair.
def regen_asr(stores : array<string>) : int {
    var doc = fread(ASR_RESULTS_PATH)
    var nrepl = 0
    var inscope cat <- asr_catalog()
    for (path in stores) {
        var models : array<BenchModel>
        if (!read_bench_records(path, models)) {
            print("skip: {path} did not parse as a record store\n")
            continue
        }
        let box = store_box(path)
        for (m in models) {
            let di = das_asr_run(m)
            if (di < 0) {
                continue
            }
            var spec_i = -1
            for (si in range(length(cat))) {
                if (cat[si].das_model == m.gguf) {
                    spec_i = si
                }
            }
            if (spec_i < 0) {
                print("  no catalog spec for {m.gguf} ({box}) — not rendered\n")
                continue
            }
            let refis <- asr_ref_runs(m, cat[spec_i])
            var found = false
            let body = "{asr_platform_line(m.runs[di])}\n\n{render_asr_table(m, m.runs[di], refis, cat[spec_i].buckets)}"
            doc = replace_region(doc, "asr {cat[spec_i].display} {box}", body, found)
            if (found) {
                nrepl++
            } else {
                print("  no marker for: asr {cat[spec_i].display} {box}\n")
            }
            if (test_ms(m.runs[di], libri_test_key()) > 0.0lf) {
                var lfound = false
                doc = replace_region(doc, "asr librispeech {box}", render_libri(m, m.runs[di], refis), lfound)
                if (lfound) {
                    nrepl++
                } else {
                    print("  no marker for: asr librispeech {box}\n")
                }
            }
        }
        delete models
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

    // ---- ASR scoreboard (regenerated in place from the record store, prose preserved) ----
    let asr <- list_asr_stores()
    if (empty(asr)) {
        print("no records/<box>.json under {PERF_DIR}/records — skipping ASR scoreboard\n")
    } else {
        let n = regen_asr(asr)
        print("asr: replaced {n} region(s) in {ASR_RESULTS_PATH}\n")
    }
}
