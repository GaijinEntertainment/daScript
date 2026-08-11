options gen2
options indenting = 4

module _ladder_test_common public

require daslib/json_boost public
require strings   // peek_data + to_char — the url encoder

// Shared test fixtures (the leading _ keeps dastest's walker away). The
// records/official documents are %json~ literals — daScript#3690 collapses
// array-bearing literals under -jit, so these suites run interpreted (the
// extended_checks utils lane).

//! Percent-encode a query-string value (RFC 3986 unreserved set kept verbatim) —
//! box strings carry '|', ',' and spaces, which are not URL-legal raw.
def url_encode(s : string) : string {
    return build_string() $(w) {
        peek_data(s) $(arr) {
            for (b in arr) {
                let c = int(b)
                if (is_alpha(c) || is_number(c)
                        || c == '-' || c == '_' || c == '.' || c == '~') {
                    w |> write(to_char(c))
                } else {
                    let hi = (c >> 4) & 15
                    let lo = c & 15
                    w |> write("%")
                    w |> write(to_char(hi < 10 ? '0' + hi : 'A' + (hi - 10)))
                    w |> write(to_char(lo < 10 ? '0' + lo : 'A' + (lo - 10)))
                }
            }
        }
    }
}

let BOX_A = "darwin|arm64|MacBookPro18,2|25F84|Apple M1 Max"
let BOX_B = "darwin|arm64|Macmini9,1|25F84|Apple M1 Max"      // same cpu, another machine
let BOX_C = "darwin|arm64|MacBookAir10,1|25F84|Apple M2"      // same arch, another cpu

def sidecar_doc(box : string; version : string; winner : string) : string {
    return write_json(JV((
        kernels = (dot = winner, softmax = "vec8_u2"),
        runtime = (jobque_spin_us = 30000, metal_tensor = ""),
        provenance = (
            box = box,
            engine_sha = "8a538d795",
            written = "2026-08-10T00:00:00Z",
            noise = "ok",
            dasllama_version = version))))
}

// a stamped community submission: one model, a das row with full receipts + a reference row
def records_doc : string {
    var doc = %json~
    [ {
        "gguf" : "test-model-Q8_0.gguf",
        "arch" : "Test 1B",
        "quant" : "Q8_0",
        "size_bytes" : 1234567,
        "runs" : [
            {
                "engine" : "das", "backend" : "cpu", "flavor" : "tuned",
                "box" : "testbox", "threads" : 8, "date" : "2026-08-10",
                "cmd" : "daslang -jit bench.das", "env" : "DASLLAMA_BOX=test",
                "sha" : "abc123def", "version" : "0.6.4", "dasllama_version" : 3,
                "tune" : "k4q8_tile_gen=mr8 (manifest)", "tune_sha" : "cafe0123beef",
                "exec_fmt" : "q8 blob",
                "hardware" : { "cpu" : "Test CPU 9000" },
                "tests" : { "pp512" : { "tok_s" : 100.0, "stddev" : 1.0 } }
            },
            {
                "engine" : "llama.cpp", "backend" : "cpu", "flavor" : "clean-cpu",
                "box" : "testbox", "threads" : 8, "date" : "2026-08-10",
                "cmd" : "llama-bench -m m.gguf", "sha" : "ebd048fc5",
                "hardware" : { "cpu" : "Test CPU 9000" },
                "tests" : { "pp512" : { "tok_s" : 80.0, "stddev" : 1.0 } }
            }
        ]
    } ]
    %%
    return write_json(doc)
}

// official history predates the release counter: shape-valid, no stamps
def official_doc : string {
    var doc = %json~
    [ {
        "gguf" : "gemma-3-1b-it-Q8_0.gguf",
        "runs" : [
            {
                "engine" : "das", "backend" : "cpu", "flavor" : "tuned",
                "box" : "m1", "threads" : 8, "date" : "2026-08-01",
                "cmd" : "daslang -jit bench.das", "sha" : "8a538d795", "version" : "0.6.4",
                "source" : "official",
                "hardware" : { "cpu" : "Apple M1 Max" },
                "tests" : { "pp512" : { "tok_s" : 259.6 } }
            },
            {
                "engine" : "llama.cpp", "backend" : "cpu", "flavor" : "clean-cpu",
                "box" : "m1", "threads" : 8, "date" : "2026-08-01",
                "cmd" : "llama-bench", "sha" : "ebd048fc5",
                "hardware" : { "cpu" : "Apple M1 Max" },
                "tests" : { "pp512" : { "tok_s" : 145.0 } }
            }
        ]
    } ]
    %%
    return write_json(doc)
}
