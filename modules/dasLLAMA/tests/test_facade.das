options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require dasllama/dasllama_tokenizer
require dasllama/dasllama_bpe
require dasllama/dasllama_gpu_tier   // the K/V room seam a cell doubles
require ?das_metal dasllama/dasllama_metal_common   // the mirror ceiling behind that seam on a Metal box
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_bench
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require daslib/strings_boost
require strings
require daslib/env_registry
require _model_tier   // llama2c_dir
require ../../dasLLVM/tests/_exe_test_common.das   // lto_driver_missing - the LTO driver gate the dasLLVM exe tests share
require ../../dasLLVM/tests/_spawn_common.das      // with_child_log_info - the build announces its form at LOG_INFO

// The unified tokenizer facade (encode/decode/piece over auto-selected SPM/BPE) must produce
// exactly what a caller writes today against the raw backend — so migrating onto it is a no-op.
// GGUF models are gitignored multi-GB files; each case skips cleanly when its model is absent.

// Regression (Copilot #3340): SPM encode/decode must use the per-model bos_id/eos_id loaded from
// GGUF, not the struct defaults 1/2 — Gemma has bos=2 / eos=1. Empty text isolates the BOS/EOS
// framing (no vocab lookup), so this needs no model file.
[test(description = "SPM encode of empty text frames it with the tokenizer's own bos_id and eos_id (Gemma's 2 and 1), while a default SpmTokenizer keeps the Llama-2 ids 1 and 2")]
def test_spm_encode_respects_model_bos_eos(t : T?) {
    let tk = SpmTokenizer(bos_id = 2l, eos_id = 1l, add_space_prefix = false)
    let ids <- encode(tk, "", true, true)
    t |> equal(length(ids), 2, "empty text with bos+eos -> [bos, eos]")
    t |> equal(ids[0], 2l, "encode uses per-model bos_id (Gemma=2), not literal 1")
    t |> equal(ids[1], 1l, "encode uses per-model eos_id (Gemma=1), not literal 2")
    let def2 = SpmTokenizer()   // defaults (Llama-2) stay bit-identical
    let ids2 <- encode(def2, "", true, true)
    t |> equal(ids2[0], 1l, "default bos_id=1 unchanged")
    t |> equal(ids2[1], 2l, "default eos_id=2 unchanged")
}

[test(description = "The facade's jobque spin knobs default to 30 ms and 500 us and their setters reach the engine, while with no driver armed the CPU window stays in force whatever the GPU knob reads")]
def test_facade_jobque_spin_knob(t : T?) {
    t |> run("the spin window reads the default before any set, and the facade setter reaches the engine") @(t : T?) {
        t |> equal(get_jobque_spin_us(), 30000l, "the default is the 30 ms window, on every platform including a browser")
        set_jobque_spin_us(1234l)
        t |> equal(get_jobque_spin_us(), 1234l, "the facade setter lands on the engine knob")
        set_jobque_spin_us(30000l)
    }
    t |> run("the GPU-served window: its own knob, in force only while the resident driver reports the model served") @(t : T?) {
        t |> equal(get_jobque_spin_gpu_us(), 500l, "the default GPU-served window is 500 us")
        t |> equal(get_jobque_spin_in_force(), get_jobque_spin_us(), "no driver armed: the CPU window is in force")
        set_jobque_spin_gpu_us(777l)
        t |> equal(get_jobque_spin_gpu_us(), 777l, "the facade setter lands on the engine knob")
        t |> equal(get_jobque_spin_in_force(), get_jobque_spin_us(), "the GPU knob alone moves nothing: the CPU window stays in force")
        set_jobque_spin_gpu_us(500l)
    }
}

[test(description = "`load_tokenizer_auto` on tinyllama selects SPM and encodes ids equal to the raw SentencePiece tokenizer's, with and without BOS")]
def test_facade_spm(t : T?) {
    t |> run("SPM facade auto-dispatch matches raw SentencePiece") @(t : T?) {
        let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var v <- load_tokenizer_auto(path)
        var raw <- load_tokenizer_gguf(path)
        let text = "Once upon a time"
        let ok = (v.kind == TokKind.spm &&
            ids_equal(encode(v, text, true, false), encode(raw, text, true, false)) &&
            ids_equal(encode(v, text, false, false), encode(raw, text, false, false)))
        t |> success(ok, "spm dispatch + encode parity (with/without BOS)")
        delete raw
        delete v
    }
}

[test(description = "`load_tokenizer_auto` on Qwen2.5 selects BPE, encodes ids equal to raw `bpe_encode` with BOS only where the model asks, and decodes ASCII back exactly")]
def test_facade_bpe(t : T?) {
    t |> run("BPE facade auto-dispatch matches raw byte-level BPE") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var v <- load_tokenizer_auto(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let text = "Once upon a time"
        // add_special=false is exactly bpe_encode; add_special=true prepends bos_id only when the
        // model wants one (tokenizer.ggml.add_bos_token / per-pre default — Qwen ships false)
        var want : array<int64>
        if (raw.bos_id >= 0l && raw.add_bos) {
            want |> push(raw.bos_id)
        }
        want |> push_from(bpe_encode(raw, text))
        let ids <- encode(v, text, false, false)
        let ok = (v.kind == TokKind.bpe &&
            ids_equal(encode(v, text, false, false), bpe_encode(raw, text)) &&
            ids_equal(encode(v, text, true, false), want) &&
            decode(v, ids) == text)   // byte-level BPE round-trips ASCII exactly
        t |> success(ok, "bpe dispatch + encode/decode parity")
        delete raw
        delete v
    }
}

[test(description = "`load_model_` populates the model's BPE tokenizer, and the model-level encode and piece return exactly what the tokenizer's own calls return")]
def test_facade_model(t : T?) {
    t |> run("load_model_ owns the tokenizer; model facade delegates") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_model_(path, QuantMode.q8)
        let text = "Hello, world"
        let via_model <- encode(m, text, true, false)
        let via_tok <- encode(m.tok, text, true, false)
        let last = via_model[length(via_model) - 1]
        let ok = (m.tok.kind == TokKind.bpe &&
            ids_equal(via_model, via_tok) &&
            piece(m, last) == piece(m.tok, last))
        t |> success(ok, "model.tok populated + encode/piece delegate")
        delete m
    }
}

[test(description = "`eval_segments` on a bare model returns without panicking for no segments, and refuses by name a chunk with no session and a null session before any read")]
def test_facade_eval_segments_empty(t : T?) {
    t |> run("no segments is a no-op on a bare model shell: nothing is dispatched, nothing read") @(t : T?) {
        var m = Model()
        var sessions : array<Session?>
        var toks : array<array<int64>>
        let why = panic_text() $ {
            eval_segments(m, sessions, toks)
        }
        t |> equal(why, "", "an empty segment list returns before any backend is asked")
        var one : array<array<int64>>
        one |> resize(1)
        let why1 = panic_text() $ {
            eval_segments(m, sessions, one)
        }
        t |> success(find(why1, "0 sessions and 1 chunks") >= 0, "one chunk and no session is refused by name ({why1})")
        var nulls : array<Session?>
        nulls |> push(null)
        one[0] |> push(1l)
        let why2 = panic_text() $ {
            eval_segments(m, nulls, one)
        }
        t |> success(find(why2, "segment 0 names no session") >= 0, "a null session is refused by name before any read ({why2})")
    }
}

def private room_of_a_megabyte(row_bytes : int64) : int64 => 1000000l / row_bytes

//! The rows a device holds a session's K/V for: the installed room seam over the model's K/V bytes a row, both planes of every
//! layer that keeps rows of its own, never past the model's context; no seam bounds nothing.
[test(description = "`device_kv_max_rows` divides the installed room seam by K/V bytes a row over the layers owning rows (488 f16, 244 f32 in a megabyte), caps at the model context, and on Metal ignores the idle mirror cap")]
def test_facade_device_kv_rows(t : T?) {
    var m = Model()
    m.config.n_layers = 3l
    m.config.head_size = 128l
    m.config.n_kv_heads = 2l
    m.config.seq_len = 1000l
    m.kv_src <- [0l, 1l, 1l]   //! the third layer reads the second's rows
    let prior = gpu_kv_room_hook()
    set_gpu_kv_room_hook(@@room_of_a_megabyte)
    t |> equal(device_kv_max_rows(m, KVDtype.f16, KVDtype.f16), 488l, "f16 K and V over two owning layers of 256: 2048 bytes a row, 488 rows in the megabyte")
    t |> equal(device_kv_max_rows(m, KVDtype.f32, KVDtype.f32), 244l, "f32 doubles a row and halves the rows")
    m.config.seq_len = 300l
    t |> equal(device_kv_max_rows(m, KVDtype.f16, KVDtype.f16), 300l, "a context under the room is the bound")
    set_gpu_kv_room_hook(prior)
    m.config.seq_len = 1000l
    t |> success(device_kv_max_rows(m, KVDtype.f16, KVDtype.f16) <= 1000l, "the installed seam bounds a model by its own context at most")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        //! the Metal seam itself: the device's largest buffer over the row's bytes - the idle cap bounds no room, so 64 MiB and
        //! 128 MiB read the same rows, past what 128 MiB of 2048-byte rows would hold
        let prev_cap = get_metal_decode_mirror_cap_mb()
        m.config.seq_len = 1000000l
        set_metal_decode_mirror_cap_mb(64)
        let rows64 = device_kv_max_rows(m, KVDtype.f16, KVDtype.f16)
        set_metal_decode_mirror_cap_mb(128)
        t |> equal(device_kv_max_rows(m, KVDtype.f16, KVDtype.f16), rows64, "the idle cap bounds no room: 64 and 128 MiB read the same rows")
        t |> success(rows64 > 65536l, "the room is the device's largest buffer over 2048-byte rows (unbounded before a device is up), past 65536 ({rows64})")
        set_metal_decode_mirror_cap_mb(prev_cap)
    }
    delete m
}


// The bench rows' contracts on a real model (stories15M, the llama2.c checkpoint): what the
// released exe and the server's /bench run per row, held on the smallest model that runs them.

let private STORIES_MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

// a pp row on a session sized to `seq_len`: under the prompt it must refuse before it writes,
// at or over it the row runs
def private pp_row_panics(var tr : Model; seq_len, plen : int64; warm : bool) : bool {
    tr.config.seq_len = seq_len
    var s <- make_run_state(tr.config)
    var tripped = false
    try {
        if (warm) {
            bench_pp_warm(tr, s, plen)
        } else {
            bench_pp_rep(tr, s, plen, 1l)
        }
    } recover {
        tripped = true
    }
    delete s
    return tripped
}

[test(description = "On stories15M the pp bench warmup and a timed pp rep panic on a session whose seq_len is under the prompt length before writing, and the warmup runs on a session that fits")]
def test_pp_rows_capacity(t : T?) {
    t |> run("the pp rows refuse a session sized under their prompt (needs stories15M)") @(t : T?) {
        if (!stat(STORIES_MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present at {STORIES_MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(STORIES_MODEL_PATH)
        with_dasllama_jobque_() {
            t |> success(!pp_row_panics(tr, 16l, 8l, true), "a session that fits runs the warmup")
            t |> success(pp_row_panics(tr, 4l, 8l, true), "the warmup prefill checks the cap before it writes")
            t |> success(pp_row_panics(tr, 4l, 8l, false), "a timed rep checks the cap before it writes")
        }
    }
}


// The standalone-exe context gate: build `_exe_smoke_root.das` with `-jit -exe` and run the
// artifact - the one rail where globals restore as DATA, so a function-typed global with no
// boot-restore [init] dies on its first invoke while every -jit suite stays green. One tiny
// model, one 4-token generate, ~90 s; skips honestly without the model. The split-plus-LTO arm
// builds the root again with --jit-split-modules=-1 --jit-lto and also skips on a POSIX box
// without clang++ on PATH.

def private self_daslang() : string {
    let argv <- get_command_line_arguments()
    return find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
}

//! `build_says` is the line the build must announce, `build_never` the one it must not (the form a dropped or a leaked flag would take)
def private smoke_build_and_run(t : T?; tag : string; jit_args : array<string>; build_says, build_never : string) {
    let model = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
    if (!model_available(t, model)) {
        return
    }
    let dir_r = create_temp_directory_result("dasllama_exe_smoke")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    let exe = path_join(dir, "smoke.exe")
    var bout = ""
    var argv <- [self_daslang(), "-jit", "-exe", "-output", exe,
                 "modules/dasLLAMA/tests/_exe_smoke_root.das"]
    if (!empty(jit_args)) {
        argv |> push("--")
        argv |> push_from(jit_args)
    }
    var brc = 0
    with_child_log_info() {
        brc = run_and_capture(argv, bout, 600.0)
    }
    t |> equal(0, brc, "the -exe build ({tag}) succeeds: {slice(bout, max(0, length(bout) - 400))}")
    t |> success(find(bout, build_says) >= 0, "the build ({tag}) announces its form `{build_says}`: {slice(bout, max(0, length(bout) - 400))}")
    t |> success(find(bout, build_never) < 0, "the build ({tag}) never announces `{build_never}`: {slice(bout, max(0, length(bout) - 400))}")
    if (brc != 0 || !stat(exe).is_valid) {
        t |> success(stat(exe).is_valid, "the exe artifact exists at -output ({tag})")
        rmdir_rec_result(dir)
        return
    }
    var out = ""
    let prev_untuned = env_value_of("DASLLAMA_ALLOW_UNTUNED")
    set_env_variable("DASLLAMA_ALLOW_UNTUNED", "1")   //! inherited by the child; /usr/bin/env does not exist on Windows
    //! the artifact links the producing daslang's runtime DLLs, which resolve through PATH and never from the
    //! temp dir - so the run prepends that daslang's dir through the shell; the command opens with `set` or an
    //! assignment, never a quote, which cmd.exe would strip
    let bin_dir = dir_name(self_daslang())
    var cmd = "PATH=\"{bin_dir}:$PATH\" \"{exe}\" -- \"{model}\" 2>&1"
    if (get_platform_name() == "windows") {
        cmd = "set \"PATH={bin_dir};%PATH%\"&& \"{exe}\" -- \"{model}\" 2>&1"
    }
    let rc = unsafe(popen_timeout(cmd, 300.0, $(f) {
        if (f != null) {
            out = fread(f)
        }
    }))
    set_env_variable("DASLLAMA_ALLOW_UNTUNED", prev_untuned)
    t |> equal(0, rc, "the exe ({tag}) runs clean: {slice(out, max(0, length(out) - 400))}")
    t |> success(find(out, "EXE_SMOKE_OK") >= 0, "the generate completed in the exe ({tag}): {slice(out, max(0, length(out) - 400))}")
    rmdir_rec_result(dir)
}

[test(description = "A standalone `-jit -exe` build of the facade root, as one unit and split with LTO, announces its build form and the artifact completes a 4-token SmolLM2 generate on the serialized-globals rail, so every function-typed global is boot-restored", timeout = 1200)]
def test_exe_smoke(t : T?) {
    t |> run("a -jit -exe build of the facade generates on the serialized-globals rail") @(t : T?) {
        smoke_build_and_run(t, "one unit", [], "standalone exe entry point generated", "split codegen (--jit-split-modules)")
    }
    t |> run("the facade built split with LTO (--jit-split-modules --jit-lto) generates the same way") @(t : T?) {
        let why_not = lto_driver_missing()
        if (!empty(why_not)) {
            t |> skip(why_not)
            return
        }
        smoke_build_and_run(t, "split + LTO", ["--jit-split-modules=-1", "--jit-lto"], "bitcode partitions, the link runs LTO", "ignored for this run")
    }
}
