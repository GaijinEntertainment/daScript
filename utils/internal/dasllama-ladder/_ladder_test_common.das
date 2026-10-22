options gen2
options indenting = 4

module _ladder_test_common public

require daslib/json_boost public

// Shared test fixtures (the leading _ keeps dastest's walker away). The
// records/official documents are %json~ literals — daScript#3690 collapses
// array-bearing literals under -jit, so these suites run interpreted (the
// extended_checks utils lane). url_encode comes from dashv_boost via ladder_server.

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
                "tests" : { "pp512" : { "tok_s" : 100.0, "stddev" : 1.0 }, "tg128" : { "tok_s" : 25.0, "stddev" : 0.5 }, "tg128@4" : { "tok_s" : 60.0, "stddev" : 2.0 } }
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

// an official asr + tts board: a das row, a reference row with no audio_s, and an mlx row per
// model; "encode" and "img:enc" are not audio cells, and the zero-ms clip is dropped
def audio_doc : string {
    var doc = %json~
    [ {
        "gguf" : "ggml-parakeet-tdt-0.6b-v3-f32.bin",
        "arch" : "Parakeet-TDT v3",
        "runs" : [
            {
                "engine" : "das", "backend" : "cpu", "flavor" : "tuned", "workload" : "asr",
                "box" : "m5", "threads" : 6, "date" : "2026-09-23",
                "cmd" : "dasllama-bench --asr", "sha" : "ea5747c91", "version" : "0.6.4", "dasllama_version" : 16,
                "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : {
                    "asr:jfk.wav" : { "ms" : 158.0, "audio_s" : 11, "text" : "And so, my fellow Americans" },
                    "asr:gb1.wav" : { "ms" : 4311.0, "audio_s" : 198.734375 },
                    "asr:void.wav" : { "ms" : 0.0, "audio_s" : 3 },
                    "encode" : { "ms" : 12.0 },
                    "img:enc" : { "ms" : 5.0 }
                }
            },
            {
                "engine" : "parakeet-cli", "backend" : "cpu", "flavor" : "stock", "workload" : "asr",
                "box" : "m5", "threads" : 6, "date" : "2026-09-23",
                "cmd" : "parakeet-cli -m m.bin", "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 159.5 } }
            },
            {
                "engine" : "mlx-audio", "backend" : "metal", "flavor" : "stock", "workload" : "asr",
                "box" : "m5", "threads" : 6, "date" : "2026-10-21",
                "cmd" : "python -m mlx_audio.stt.generate", "versions" : "mlx-audio 0.2.5",
                "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 90.0, "audio_s" : 11 } }
            }
        ]
    }, {
        "gguf" : "kokoro-82m.gguf",
        "arch" : "Kokoro 82M",
        "runs" : [
            {
                "engine" : "das", "backend" : "cpu", "flavor" : "tuned", "workload" : "tts",
                "box" : "m5", "threads" : 6, "date" : "2026-10-21",
                "cmd" : "lcpp_bench --tts", "sha" : "ea5747c91", "version" : "0.6.5", "dasllama_version" : 17,
                "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "tts:s01" : { "ms" : 120.0, "audio_s" : 2.5 }, "tts:s02" : { "ms" : 200.0, "audio_s" : 4.0 } }
            },
            {
                "engine" : "mlx-audio", "backend" : "metal", "flavor" : "stock", "workload" : "tts",
                "box" : "m5", "threads" : 6, "date" : "2026-10-21",
                "cmd" : "python -m mlx_audio.tts.generate", "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "tts:s01" : { "ms" : 80.0, "audio_s" : 2.4 } }
            }
        ]
    } ]
    %%
    return write_json(doc)
}

//! an audio-chat model's transcription runs: its clips ride the asr: keys, and a tts: key beside them is no cell
def audio_chat_doc : string {
    var doc = %json~
    [ {
        "gguf" : "gemma-4-E2B-it-Q8_0.gguf",
        "arch" : "Gemma-4 E2B audio",
        "runs" : [
            {
                "engine" : "das", "backend" : "metal", "flavor" : "tuned", "workload" : "audio-chat",
                "box" : "m5", "threads" : 6, "date" : "2026-10-21",
                "cmd" : "lcpp_bench --asr", "sha" : "ea5747c91", "version" : "0.6.5", "dasllama_version" : 17,
                "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 600.0, "audio_s" : 11 }, "tts:s01" : { "ms" : 5.0 } }
            },
            {
                "engine" : "llama-mtmd-cli", "backend" : "metal", "flavor" : "stock", "workload" : "audio-chat",
                "box" : "m5", "threads" : 6, "date" : "2026-10-21",
                "cmd" : "llama-mtmd-cli", "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 900.0 } }
            }
        ]
    } ]
    %%
    return write_json(doc)
}

//! an audio-chat model whose reference read one clip at a cold start: the das run carries every
//! clip's length, the reference none, and jfk.wav runs ninety times the reference's median rate
//! while the das run and the reference's second row are even; the third row names a clip in its
//! own `void_clips` beside the rule's
def audio_outlier_doc : string {
    var doc = %json~
    [ {
        "gguf" : "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf",
        "arch" : "Qwen3-Omni 30B",
        "runs" : [
            {
                "engine" : "das", "backend" : "metal", "flavor" : "tuned", "workload" : "audio-chat",
                "box" : "m5", "threads" : 8, "date" : "2026-10-21",
                "cmd" : "lcpp_bench --asr", "sha" : "ea5747c91", "version" : "0.6.5", "dasllama_version" : 17,
                "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 357.6, "audio_s" : 11 }, "asr:jfk3.wav" : { "ms" : 464.8, "audio_s" : 33 }, "asr:gb1.wav" : { "ms" : 4729.2, "audio_s" : 198.734375 } }
            },
            {
                "engine" : "llama-mtmd-cli", "backend" : "metal", "flavor" : "stock", "workload" : "audio-chat",
                "box" : "m5", "threads" : 8, "date" : "2026-10-21",
                "cmd" : "llama-mtmd-cli", "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 34960.4 }, "asr:jfk3.wav" : { "ms" : 553.2 }, "asr:gb1.wav" : { "ms" : 7075.4 } }
            },
            {
                "engine" : "llama-mtmd-cli", "backend" : "cpu", "flavor" : "stock", "workload" : "audio-chat",
                "box" : "m5", "threads" : 8, "date" : "2026-10-21",
                "cmd" : "llama-mtmd-cli", "hardware" : { "cpu" : "Apple M5 Max" },
                "tests" : { "asr:jfk.wav" : { "ms" : 9365.4 }, "asr:jfk3.wav" : { "ms" : 25961.6 }, "asr:gb1.wav" : { "ms" : 158455.9 } }
            },
            {
                "engine" : "llama-mtmd-cli", "backend" : "vulkan", "flavor" : "stock", "workload" : "audio-chat",
                "box" : "m5", "threads" : 8, "date" : "2026-10-21",
                "cmd" : "llama-mtmd-cli", "hardware" : { "cpu" : "Apple M5 Max" },
                "void_clips" : "gb1.wav",
                "tests" : { "asr:jfk.wav" : { "ms" : 9365.4 }, "asr:jfk3.wav" : { "ms" : 25961.6 }, "asr:gb1.wav" : { "ms" : 158455.9 } }
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
