options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// dasllama-cli end to end, one child process per cell: every subcommand on the fastest small
// models, a second column on the next size up, and the tower cell on gemma-4-E2B. Model-gated
// and JIT-only: skips by name where a model is absent, so it runs on a stocked box at release
// time and compiles everywhere else. cli.das is spawned exactly as a user runs it.

require dastest/testing_boost public
require _server_rig                                     // models_dir / model_missing (sibling)
require daslib/clargs                                   // get_host_binary
require daslib/module_path                              // get_this_module_dir: cli.das sits beside this test
require daslib/json_boost
require daslib/fio
require daslib/strings_boost                              // join / split
require strings

let CELL_TIMEOUT_SEC = 300.0   // a cold cell parses, JITs, loads its models and answers
let JFK_PATH = "modules/dasLLAMA/models/jfk_ask_not.wav"   // in-tree, 4 s, 16 kHz mono
let CATS = "coco-val2017-000000039769.jpg"

// the fastest column
let SMOL = "SmolLM2-135M-Instruct-Q8_0.gguf"
let KITTEN = "kitten-nano.gguf"
let QWEN_ASR = "Qwen3-ASR-0.6B-Q8_0.gguf"
let QWEN_ASR_MMPROJ = "mmproj-Qwen3-ASR-0.6B-bf16.gguf"
// the second column
let QWEN_08 = "Qwen3.5-0.8B-Q8_0.gguf"
let QWEN_4B = "Qwen3.5-4B-Q8_0.gguf"   // thinks by default, and loops inside its reasoning under greedy decoding
let KOKORO = "kokoro-82m.gguf"
// the tower column: one mmproj, both towers
let E2B = "gemma-4-E2B-it-Q4_K_M.gguf"
let E2B_MMPROJ = "mmproj-gemma-4-E2B-it-bf16.gguf"
let POCKET = "pocket-tts-en-q8.gguf"

def private model(name : string) : string => path_join(models_dir(), name)

def private scratch(name : string) : string {
    var terr : string
    let tmp = temp_directory(terr)
    return path_join(empty(tmp) ? "." : tmp, "dasllama_cli_{name}_{ref_time_ticks()}")
}

//! The cell's argv: the daslang this test runs under, -jit, cli.das, then the subcommand and its flags.
def private cli_argv(args : array<string>) : array<string> {
    var full <- ["--"]
    full |> push_from(args)
    return <- child_daslang_argv(path_join(get_this_module_dir(), "cli.das"), full)
}

struct private CellResult {
    rc : int
    out : string
}

//! One dasllama-cli invocation as a user runs it; a [tune] bootstrap exit relaunches once.
def private run_cli(args : array<string>) : CellResult {
    let argv <- cli_argv(args)
    var out = ""
    var rc = run_and_capture(argv, out, CELL_TIMEOUT_SEC)
    if (rc == 3 && find(out, "restart to apply") >= 0) {
        out = ""   // the child adopted this box's kernels and exited with the restart code
        rc = run_and_capture(argv, out, CELL_TIMEOUT_SEC)
    }
    return CellResult(rc = rc, out = out)
}


//! A documented example as written: bare model names, resolved under `--models-dir`.
def private run_doc(args : array<string>) : CellResult {
    var full <- [args[0], "--models-dir", models_dir()]   // the subcommand leads; the shared flag rides behind it
    full |> reserve(length(args) + 2)
    for (i in range(1, length(args))) {
        full |> push(args[i])
    }
    return run_cli(full)
}

//! The lines of a run that are neither a prompt echo, a progress line nor a JIT notice.
def private answer_lines(out : string) : array<string> {
    var lines : array<string>
    for (ln in split(out, "\n")) {
        continue if (empty(strip(ln)) || starts_with(ln, "dasllama-cli:") || starts_with(ln, "llvm_tune:")
                     || starts_with(ln, "[W]") || starts_with(ln, "remark:") || starts_with(ln, "warning:")
                     || starts_with(ln, "> "))
        lines |> push(ln)
    }
    return <- lines
}

//! The answer line of a `--quiet` run: the last of `answer_lines`.
def private answer_of(out : string) : string {
    let lines <- answer_lines(out)
    return empty(lines) ? "" : lines[length(lines) - 1]
}

def private pass_cell(t : T?; r : CellResult; what : string) : bool {
    t |> equal(r.rc, 0, "{what} exits 0 - tail: {tail_of(r.out)}")
    return r.rc == 0
}

//! A WAV the cell wrote: RIFF/WAVE, 16-bit PCM, and its length in seconds.
def private wav_seconds(t : T?; path : string) : double {
    var bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            bytes |> resize(44)
            let got = fread(f, bytes)
            if (got < 44) {
                bytes |> clear()
            }
        }
    }
    if (length(bytes) < 44) {
        t |> failure("{path} is not a WAV file (no 44-byte header)")
        return 0.0lf
    }
    let riff = length(bytes) >= 4 && bytes[0] == uint8('R') && bytes[1] == uint8('I') && bytes[2] == uint8('F') && bytes[3] == uint8('F')
    let wave = bytes[8] == uint8('W') && bytes[9] == uint8('A') && bytes[10] == uint8('V') && bytes[11] == uint8('E')
    t |> success(riff && wave, "{path} carries a RIFF/WAVE header")
    let rate = int(bytes[24]) | (int(bytes[25]) << 8) | (int(bytes[26]) << 16) | (int(bytes[27]) << 24)
    let bits = int(bytes[34]) | (int(bytes[35]) << 8)
    t |> equal(bits, 16, "16-bit PCM")
    var st : FStat
    if (!stat(path, st) || rate <= 0) {
        return 0.0lf
    }
    return double(st.size - 44ul) / double(rate * 2)
}


// ===== the fastest column: every subcommand =====

[test]
def test_complete(t : T?) {
    let llm = model(SMOL)
    t |> run("complete: greedy, deterministic, the answer alone on stdout under --quiet") @(t : T?) {
        return if (model_missing(t, llm))
        let args <- ["complete", "-m", llm, "-n", "12", "--quiet", "The capital of France is"]
        let a = run_cli(args)
        return if (!pass_cell(t, a, "complete"))
        t |> success(find(a.out, "Paris") >= 0, "the tiny model knows Paris: {tail_of(a.out)}")
        let b = run_cli(args)
        t |> equal(answer_of(b.out), answer_of(a.out), "greedy twice is the same text")
    }
    t |> run("complete: the sampler flags survive the shared flags around them (the greedy-fallback shape)") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["complete", "-m", llm, "-n", "4", "--quiet", "--gpu", "off", "--temp", "2.0", "--top-k", "20", "hi"])
        return if (!pass_cell(t, r, "complete --gpu off --temp 2.0"))
        t |> success(find(r.out, "sampler: temp 2") >= 0, "the run reports the temperature it was given, not greedy - tail: {tail_of(r.out)}")
        t |> success(find(r.out, "top-k 20") >= 0, "and the top-k")
    }
    t |> run("complete: a missing model is a refusal with the name in it") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["complete", "-m", "no-such-model.gguf", "hi"])
        t |> success(r.rc != 0, "non-zero exit")
        t |> success(find(r.out, "no-such-model.gguf") >= 0, "names the file: {tail_of(r.out)}")
    }
}

[test]
def test_tokenize_embed(t : T?) {
    let llm = model(SMOL)
    t |> run("tokenize: one id and piece per line, ids alone under --ids") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["tokenize", "-m", llm, "Hello world"])
        return if (!pass_cell(t, r, "tokenize"))
        t |> success(find(r.out, "\tHello") >= 0, "a piece column: {tail_of(r.out)}")
        let ids = run_cli(["tokenize", "-m", llm, "--ids", "Hello world"])
        return if (!pass_cell(t, ids, "tokenize --ids"))
        t |> success(find(ids.out, "\t") < 0 || find(ids.out, "\tHello") < 0, "no piece column")
    }
    t |> run("embed --json: a JSON array of floats") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["embed", "-m", llm, "--json", "Hello world"])
        return if (!pass_cell(t, r, "embed"))
        var line = ""
        for (ln in split(r.out, "\n")) {
            if (starts_with(ln, "[")) {
                line = ln
            }
        }
        var err = ""
        var doc = read_json(line, err)
        t |> success(doc != null && (doc.value is _array), "parses as an array: {err}")
        if (doc != null) {
            t |> success(length(doc.value as _array) > 100, "a real vector")
            unsafe {
                delete doc
            }
        }
    }
}

[test]
def test_transcribe(t : T?) {
    t |> run("transcribe: the JFK clip through Qwen3-ASR") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)))
        let r = run_cli(["transcribe", "--asr", model(QWEN_ASR), "--mmproj", model(QWEN_ASR_MMPROJ), JFK_PATH])
        return if (!pass_cell(t, r, "transcribe"))
        t |> success(find(to_lower(r.out), "fellow americans") >= 0, "the transcript: {tail_of(r.out)}")
    }
    t |> run("transcribe: into a file in a directory that did not exist") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)))
        let out = path_join(scratch("transcript_out"), "jfk.txt")   // the directory is absent: the CLI creates it
        let r = run_cli(["transcribe", "--asr", model(QWEN_ASR), "--mmproj", model(QWEN_ASR_MMPROJ), "-f", JFK_PATH, "-o", out])
        return if (!pass_cell(t, r, "transcribe -o"))
        let text = fread(out)
        t |> success(find(to_lower(text), "fellow americans") >= 0, "the transcript file: {text}")
        remove(out)
        rmdir(dir_name(out))
    }
}

[test]
def test_speak(t : T?) {
    t |> run("speak: kitten-nano writes a WAV of speech length, into a directory that did not exist") @(t : T?) {
        return if (model_missing(t, model(KITTEN)))
        let out = path_join(scratch("speak_out"), "hello.wav")   // the directory is absent: the CLI creates it
        let r = run_cli(["speak", "--tts", model(KITTEN), "-o", out, "Hello from the command line."])
        return if (!pass_cell(t, r, "speak"))
        t |> success(fexist(out), "the WAV landed")
        let s = wav_seconds(t, out)
        t |> success(s > 0.8lf && s < 8.0lf, "{s} s of speech for one short sentence")
        remove(out)
        rmdir(dir_name(out))
    }
    t |> run("speak: an unknown lane spelling still speaks (warned, served q8)") @(t : T?) {
        return if (model_missing(t, model(KITTEN)))
        let out = scratch("lane.wav")
        let r = run_cli(["speak", "--tts", model(KITTEN), "--tts-lane", "q3", "-o", out, "Lane."])
        return if (!pass_cell(t, r, "speak lane"))
        t |> success(fexist(out), "the WAV landed")
        remove(out)
    }
}

[test]
def test_chat_script(t : T?) {
    let llm = model(SMOL)
    t |> run("chat --script: messages, the block, the transcript file, the commands") @(t : T?) {
        return if (model_missing(t, llm))
        let script = scratch("script.txt")
        let saved = scratch("conv.json")
        let lines <- ["Answer in one short sentence: what is 2+2?", "/stats", "/save {saved}", "/regen", "/clear",
                      "\"\"\"", "Name one color.", "\"\"\"", "/bogus", "/system Be terse.", "/exit", "never sent"]
        t |> success(fwrite(script, "{join(lines, "\n")}\n"), "the script is written")
        let r = run_cli(["chat", "-m", llm, "--script", script, "-n", "24", "--quiet"])
        return if (!pass_cell(t, r, "chat --script"))
        t |> success(find(r.out, "prompt ") >= 0 && find(r.out, "gen ") >= 0, "/stats prints the counters")
        t |> success(find(r.out, "saved {saved}") >= 0, "/save answers")
        t |> success(find(r.out, "conversation cleared") >= 0, "/clear answers")
        t |> success(find(r.out, "unknown command") >= 0, "an unknown command is named")
        t |> success(find(r.out, "system prompt set") >= 0, "/system answers")
        t |> success(find(r.out, "never sent") < 0, "/exit stops the script")
        let text = fread(saved)
        var err = ""
        var doc = read_json(text, err)
        t |> success(doc != null, "the transcript parses: {err}")
        if (doc != null) {
            let turns = doc?["turns"]
            t |> success(turns != null && (turns.value is _array) && length(turns.value as _array) == 1, "one turn saved before /clear")
            unsafe {
                delete doc
            }
        }
        remove(script)
        remove(saved)
    }
}

[test]
def test_chat_thinking(t : T?) {
    let llm = model(QWEN_4B)
    t |> run("chat on a thinking model: the reasoning streams by default, the answer follows it within the budget") @(t : T?) {
        return if (model_missing(t, llm))
        let script = scratch("think.txt")
        t |> success(fwrite(script, "hello\n/exit\n"), "the script is written")
        // the 4B under GREEDY decoding rewrites its own draft inside the reasoning span until the
        // budget runs out - the sampled default is what lets it close; a hidden reasoning stream
        // would leave nothing on the screen until then
        let r = run_cli(["chat", "-m", llm, "--script", script, "-n", "1500"])
        return if (!pass_cell(t, r, "chat thinking"))
        var gen = -1
        var reasoning_lines = 0
        var seen_prompt = false
        for (ln in split(r.out, "\n")) {
            if (starts_with(ln, "> hello")) {
                seen_prompt = true
                continue
            }
            let at = find(ln, "| gen ")
            if (starts_with(ln, "dasllama-cli: prompt ") && at >= 0) {
                gen = to_int(slice(ln, at + 6))
                break
            }
            if (seen_prompt && !empty(strip(ln)) && !starts_with(ln, "dasllama-cli:") && !starts_with(ln, "[W]")) {
                reasoning_lines++
            }
        }
        t |> success(gen > 0 && gen < 1500, "the reply closed before the budget (gen {gen}) - greedy decoding loops the reasoning to the cap")
        t |> success(reasoning_lines >= 4, "the reasoning streamed to the screen ({reasoning_lines} lines) - a hidden stream shows the answer alone")
        remove(script)
    }
}

[test]
def test_talk_chain(t : T?) {
    t |> run("talk: JFK -> Qwen3-ASR -> SmolLM2 -> kitten, one process") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)) || model_missing(t, model(SMOL)) || model_missing(t, model(KITTEN)))
        let out = scratch("reply.wav")
        let r = run_cli(["talk", "--asr", model(QWEN_ASR), "--mmproj", model(QWEN_ASR_MMPROJ), "-m", model(SMOL),
                         "--tts", model(KITTEN), "--in", JFK_PATH, "-o", out, "-n", "32"])
        return if (!pass_cell(t, r, "talk"))
        t |> success(find(r.out, "you: ") >= 0, "the transcript is echoed")
        t |> success(find(to_lower(r.out), "fellow americans") >= 0, "and it is the clip's words")
        t |> success(fexist(out), "the spoken reply landed")
        t |> success(wav_seconds(t, out) > 0.5lf, "with speech in it")
        remove(out)
    }
    t |> run("talk: a text prompt -> Qwen3.5-0.8B -> kokoro") @(t : T?) {
        return if (model_missing(t, model(QWEN_08)) || model_missing(t, model(KOKORO)))
        let out = scratch("reply2.wav")
        let r = run_cli(["talk", "-m", model(QWEN_08), "--tts", model(KOKORO), "-p", "Say hello in five words.", "-o", out, "-n", "32", "--no-think"])
        return if (!pass_cell(t, r, "talk text"))
        t |> success(fexist(out), "the spoken reply landed")
        t |> success(wav_seconds(t, out) > 0.5lf, "with speech in it")
        remove(out)
    }
}

[test]
def test_bench(t : T?) {
    let llm = model(SMOL)
    t |> run("bench: the pp and tg rows as a markdown table") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["bench", "-m", llm, "-p", "64", "-n", "16", "-r", "2", "-o", "md"])
        return if (!pass_cell(t, r, "bench"))
        t |> success(find(r.out, "| pp64 |") >= 0, "the pp row: {tail_of(r.out)}")
        t |> success(find(r.out, "| tg16 |") >= 0, "the tg row")
    }
}



// ===== before any model loads: refusals and the exit codes, no model gate =====

[test]
def test_preload(t : T?) {
    t |> run("a bad command exits 2 with the usage; a missing prompt, config or format exits 1 with the reason") @(t : T?) {
        let bad = run_cli(["frobnicate"])
        t |> equal(bad.rc, 2, "exit 2")
        t |> success(find(bad.out, "unknown command") >= 0 && find(bad.out, "usage: dasllama-cli") >= 0, "names it and prints the usage")
        let badval = run_cli(["complete", "-n", "twelve", "hi"])
        t |> equal(badval.rc, 2, "a value that does not parse exits 2")
        t |> success(find(badval.out, "--prompt") >= 0, "and prints the command's help")
        let nomodel = run_cli(["complete", "hi"])
        t |> equal(nomodel.rc, 1, "no model exits 1")
        t |> success(find(nomodel.out, "--model") >= 0, "and names the flag: {tail_of(nomodel.out)}")
        let cases <- [
            (args = ["complete", "-m", "x.gguf"], wants = "nothing to complete"),
            (args = ["complete", "-m", "x.gguf", "--config", "/no/such.toml", "hi"], wants = "/no/such.toml"),
            (args = ["bench", "-m", "x.gguf", "-o", "csv"], wants = "--output is txt | md"),
            (args = ["talk", "--tts", "t.gguf"], wants = "nothing to answer"),
            (args = ["complete", "-m", "x.gguf", "--file", "/no/such.txt"], wants = "could not read"),
            (args = ["complete", "-m", "no-such-model.gguf", "hi"], wants = "no-such-model.gguf"),
            (args = ["speak", "--tts", "no-such-tts.gguf", "hi"], wants = "TTS model not found"),
            (args = ["transcribe", "--asr", "no-such-asr.bin", JFK_PATH], wants = "ASR model not found")
        ]
        for (c in cases) {
            let r = run_cli(c.args)
            t |> equal(r.rc, 1, "{join(c.args, " ")} exits 1")
            t |> success(find(r.out, c.wants) >= 0, "{join(c.args, " ")} names '{c.wants}': {tail_of(r.out)}")
        }
    }
}

[test]
def test_preload_hosts(t : T?) {
    t |> run("without the JIT the CLI refuses rather than runs the kernels interpreted") @(t : T?) {
        var argv <- cli_argv(["complete", "-m", "x.gguf", "hi"])
        argv |> erase(1)   // drop the -jit
        var out = ""
        let rc = run_and_capture(argv, out, CELL_TIMEOUT_SEC)
        t |> equal(rc, 1, "exit 1: {tail_of(out)}")
        t |> success(find(out, "run under the JIT") >= 0, "the refusal names the JIT")
    }
}

[test]
def test_flags_take_effect(t : T?) {
    let llm = model(SMOL)
    t |> run("-n caps the reply; --seed is reported; --ctx caps the context") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["complete", "-m", llm, "-n", "3", "--seed", "7", "--temp", "0.5", "--ctx", "64", "Tell me a long story about"])
        return if (!pass_cell(t, r, "flags"))
        t |> success(find(r.out, "gen 3 tok") >= 0, "-n 3 generated 3: {tail_of(r.out)}")
        t |> success(find(r.out, "seed 7") >= 0, "--seed 7 reported")
        t |> success(find(r.out, "gpu backend auto-detected") >= 0, "the auto pick is announced")
    }
    t |> run("--ctx caps the session: a prompt past it is refused through the recover, with the log named") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["complete", "-m", llm, "--ctx", "16", "-n", "4", "one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen twenty"])
        t |> equal(r.rc, 1, "exit 1")
        t |> success(find(r.out, "exceeds the session's seq_len 16") >= 0, "the engine's reason reaches the user: {tail_of(r.out)}")
        t |> success(find(r.out, "logs/dasllama-cli.log") >= 0, "and the log is named")
    }
    t |> run("--seed: the same seed repeats a sampled reply, another seed changes it") @(t : T?) {
        return if (model_missing(t, llm))
        let a = run_cli(["complete", "-m", llm, "-n", "24", "--temp", "0.9", "--seed", "7", "--quiet", "Write a long story about a lighthouse keeper."])
        let b = run_cli(["complete", "-m", llm, "-n", "24", "--temp", "0.9", "--seed", "7", "--quiet", "Write a long story about a lighthouse keeper."])
        let c = run_cli(["complete", "-m", llm, "-n", "24", "--temp", "0.9", "--seed", "8", "--quiet", "Write a long story about a lighthouse keeper."])
        return if (!pass_cell(t, a, "seed 7") || !pass_cell(t, b, "seed 7 again") || !pass_cell(t, c, "seed 8"))
        t |> equal(answer_of(a.out), answer_of(b.out), "seed 7 twice")
        t |> success(answer_of(a.out) != answer_of(c.out), "seed 8 differs: {answer_of(c.out)}")
    }
    t |> run("--seed reaches a chat: the same seed repeats a sampled reply, another seed changes it") @(t : T?) {
        return if (model_missing(t, llm))
        let script = scratch("seed.txt")
        t |> success(fwrite(script, "Write a long story about a lighthouse keeper.\n/exit\n"), "script")
        let a = run_cli(["chat", "-m", llm, "--script", script, "-n", "24", "--temp", "0.9", "--seed", "7", "--quiet"])
        let b = run_cli(["chat", "-m", llm, "--script", script, "-n", "24", "--temp", "0.9", "--seed", "7", "--quiet"])
        let c = run_cli(["chat", "-m", llm, "--script", script, "-n", "24", "--temp", "0.9", "--seed", "8", "--quiet"])
        return if (!pass_cell(t, a, "seed 7") || !pass_cell(t, b, "seed 7 again") || !pass_cell(t, c, "seed 8"))
        let la <- answer_lines(a.out)
        let lb <- answer_lines(b.out)
        let lc <- answer_lines(c.out)
        t |> equal(join(la, "|"), join(lb, "|"), "seed 7 twice")
        t |> success(join(la, "|") != join(lc, "|"), "seed 8 differs: {join(lc, "|")}")
        remove(script)
    }
    t |> run("--gpu off serves from the CPU: the bench row says so") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["bench", "-m", llm, "--gpu", "off", "-p", "32", "-n", "8", "-r", "2", "-o", "md"])
        return if (!pass_cell(t, r, "bench --gpu off"))
        t |> success(find(r.out, "| cpu |") >= 0, "the backend column reads cpu: {tail_of(r.out)}")
    }
    t |> run("tokenize --no-special drops a BOS the model adds") @(t : T?) {
        let llama = model("Llama-3.2-1B-Instruct-Q8_0.gguf")
        return if (model_missing(t, llama))
        let with_bos = run_cli(["tokenize", "-m", llama, "--ids", "Hello world"])
        let bare = run_cli(["tokenize", "-m", llama, "--ids", "--no-special", "Hello world"])
        return if (!pass_cell(t, with_bos, "tokenize") || !pass_cell(t, bare, "tokenize --no-special"))
        let a <- split(answer_of(with_bos.out), " ")
        let b <- split(answer_of(bare.out), " ")
        t |> success(length(b) < length(a), "strictly fewer ids without the BOS: {length(a)} vs {length(b)}")
    }
    t |> run("chat --no-think on the 4B: the answer with no reasoning lines") @(t : T?) {
        return if (model_missing(t, model(QWEN_4B)))
        let script = scratch("nothink.txt")
        t |> success(fwrite(script, "hello\n/exit\n"), "script")
        let r = run_cli(["chat", "-m", model(QWEN_4B), "--script", script, "-n", "60", "--no-think", "--quiet"])
        return if (!pass_cell(t, r, "no-think"))
        let lines <- answer_lines(r.out)
        t |> success(!empty(lines) && length(lines) <= 3, "the answer alone ({length(lines)} lines): {join(lines, " / ")}")
        remove(script)
    }
}

[test]
def test_chat_edges(t : T?) {
    let llm = model(SMOL)
    t |> run("/regen answers again and keeps one turn; the commands with nothing to act on say so") @(t : T?) {
        return if (model_missing(t, llm))
        let script = scratch("edges.txt")
        let saved = scratch("edges.json")
        let lines <- ["/stats", "/regen", "/system", "/read /no/such.txt", "/load /no/such.json", "/speak on", "/image a.jpg", "/audio a.wav",
                      "Say the word pear.", "/regen", "/save {saved}", "/exit"]
        t |> success(fwrite(script, "{join(lines, "\n")}\n"), "script")
        let r = run_cli(["chat", "-m", llm, "--script", script, "-n", "12", "--quiet"])
        return if (!pass_cell(t, r, "chat edges"))
        t |> success(find(r.out, "no reply yet") >= 0, "/stats before a reply")
        t |> success(find(r.out, "nothing to regenerate yet") >= 0, "/regen before a message")
        t |> success(find(r.out, "no system prompt") >= 0, "/system with no argument shows")
        t |> success(find(r.out, "could not read /no/such.txt") >= 0, "/read on a missing file")
        t |> success(find(r.out, "could not load /no/such.json") >= 0, "/load on a missing file")
        t |> success(find(r.out, "no speech model") >= 0, "/speak on without --tts")
        t |> success(find(r.out, "the queued image is dropped") >= 0, "/audio after /image replaces it")
        t |> success(find(r.out, "no audio mmproj") >= 0, "the queued audio is refused at the message without --audio-mmproj")
        let text = fread(saved)
        var err = ""
        var doc = read_json(text, err)
        t |> success(doc != null, "the transcript parses: {err}")
        if (doc != null) {
            let turns = doc?["turns"]
            t |> success(turns != null && (turns.value is _array) && length(turns.value as _array) == 1, "/regen replaced the turn rather than adding one")
            unsafe {
                delete doc
            }
        }
        remove(script)
        remove(saved)
    }
    t |> run("/image without --image-mmproj is refused at the message") @(t : T?) {
        return if (model_missing(t, llm))
        let script = scratch("noimg.txt")
        t |> success(fwrite(script, "/image {model(CATS)}\nWhat is this?\n/exit\n"), "script")
        let r = run_cli(["chat", "-m", llm, "--script", script, "-n", "8", "--quiet"])
        return if (!pass_cell(t, r, "no vision"))
        t |> success(find(r.out, "no vision mmproj") >= 0, "refused with the flag to pass: {tail_of(r.out)}")
        remove(script)
    }
    t |> run("--audio-mmproj on a file with no audio tower is refused by name") @(t : T?) {
        return if (model_missing(t, llm) || model_missing(t, model(QWEN_ASR_MMPROJ)))
        let r = run_cli(["chat", "-m", llm, "--audio-mmproj", model(QWEN_ASR_MMPROJ), "--script", JFK_PATH])
        t |> success(r.rc != 0, "non-zero exit")
        t |> success(find(r.out, "carries no audio tower") >= 0, "the reason: {tail_of(r.out)}")
    }
}

// ===== the documented examples, as written (README + the RST page) =====

[test]
def test_doc_examples(t : T?) {
    t |> run("README: complete -m SmolLM2-135M-Instruct-Q8_0.gguf --quiet \"The capital of France is\"") @(t : T?) {
        return if (model_missing(t, model(SMOL)))
        let r = run_doc(["complete", "-m", SMOL, "--quiet", "The capital of France is"])
        return if (!pass_cell(t, r, "doc complete"))
        t |> success(find(r.out, "Paris") >= 0, "Paris: {tail_of(r.out)}")
        t |> success(find(r.out, "| gen ") < 0, "--quiet drops the counters")
    }
    t |> run("README: speak --tts kitten-nano.gguf -o hello.wav \"Hello from the command line.\"") @(t : T?) {
        return if (model_missing(t, model(KITTEN)))
        let out = scratch("hello.wav")
        let r = run_doc(["speak", "--tts", KITTEN, "-o", out, "Hello from the command line."])
        return if (!pass_cell(t, r, "doc speak"))
        t |> success(wav_seconds(t, out) > 0.8lf, "speech landed")
        remove(out)
    }
    t |> run("README: transcribe --asr Qwen3-ASR-0.6B-Q8_0.gguf --mmproj mmproj-Qwen3-ASR-0.6B-bf16.gguf question.wav") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)))
        let r = run_doc(["transcribe", "--asr", QWEN_ASR, "--mmproj", QWEN_ASR_MMPROJ, JFK_PATH])
        return if (!pass_cell(t, r, "doc transcribe"))
        t |> success(find(to_lower(r.out), "fellow americans") >= 0, "the transcript: {tail_of(r.out)}")
    }
    t |> run("README: talk --asr ... --model Qwen3.5-0.8B-Q8_0.gguf --tts kokoro-82m.gguf --in question.wav --out answer.wav") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)) || model_missing(t, model(QWEN_08)) || model_missing(t, model(KOKORO)))
        let out = scratch("answer.wav")
        let r = run_doc(["talk", "--asr", QWEN_ASR, "--mmproj", QWEN_ASR_MMPROJ, "--model", QWEN_08, "--tts", KOKORO,
                         "--in", JFK_PATH, "--out", out, "-n", "64", "--no-think"])
        return if (!pass_cell(t, r, "doc talk"))
        t |> success(find(r.out, "you: ") >= 0, "the transcript is echoed")
        t |> success(wav_seconds(t, out) > 0.5lf, "the spoken reply landed")
        remove(out)
    }
    t |> run("README: chat -m gemma-4-E2B ... --image-mmproj X --audio-mmproj X (the same file twice)") @(t : T?) {
        return if (model_missing(t, model(E2B)) || model_missing(t, model(E2B_MMPROJ)))
        let script = scratch("e2b.txt")
        t |> success(fwrite(script, "/audio {JFK_PATH}\nWhat did the speaker say? Quote it.\n/exit\n"), "script")
        let r = run_doc(["chat", "-m", E2B, "--image-mmproj", E2B_MMPROJ, "--audio-mmproj", E2B_MMPROJ, "--script", script, "-n", "160", "--no-think", "--quiet"])
        return if (!pass_cell(t, r, "doc chat E2B"))
        t |> success(find(to_lower(r.out), "americans") >= 0, "it heard the clip: {tail_of(r.out)}")
        remove(script)
    }
    t |> run("README: bench -m SmolLM2-135M-Instruct-Q8_0.gguf --npl 2 -o md") @(t : T?) {
        return if (model_missing(t, model(SMOL)))
        let r = run_doc(["bench", "-m", SMOL, "--npl", "2", "-o", "md", "-p", "64", "-n", "16", "-r", "2"])
        return if (!pass_cell(t, r, "doc bench"))
        t |> success(find(r.out, "| pp64 |") >= 0 && find(r.out, "| tg16 |") >= 0, "the flat rows")
        t |> success(find(r.out, "| tg16@2 |") >= 0, "the batched row: {tail_of(r.out)}")
    }
    t |> run("module README: complete --model <decoder> --image-mmproj <mmproj> --image <picture> \"describe this image\"") @(t : T?) {
        return if (model_missing(t, model(E2B)) || model_missing(t, model(E2B_MMPROJ)) || model_missing(t, model(CATS)))
        let r = run_doc(["complete", "--model", E2B, "--image-mmproj", E2B_MMPROJ, "--image", model(CATS), "-n", "160", "--quiet", "describe this image"])
        return if (!pass_cell(t, r, "doc complete --image"))
        t |> success(find(to_lower(r.out), "cat") >= 0, "it describes cats: {tail_of(r.out)}")
    }
}

// ===== the documented scenarios =====

[test]
def test_doc_scenarios(t : T?) {
    let llm = model(SMOL)
    t |> run("<command> --show-help lists the command's flags and the shared ones, exit 0") @(t : T?) {
        let r = run_cli(["talk", "--show-help"])
        t |> equal(r.rc, 0, "exit 0: {tail_of(r.out)}")
        t |> success(find(r.out, "--tts") >= 0 && find(r.out, "--gpu") >= 0, "own flag and a shared flag")
        let usage = run_cli(["--show-help"])
        t |> equal(usage.rc, 0, "usage exits 0")
        t |> success(find(usage.out, "usage: dasllama-cli") >= 0, "the usage")
    }
    t |> run("the answer alone on stdout, progress and counters on stderr") @(t : T?) {
        return if (model_missing(t, llm))
        if (get_platform_name() == "windows") {
            t |> skip("the stderr redirect below is POSIX")
            return
        }
        let argv <- cli_argv(["complete", "-m", llm, "-n", "6", "The capital of France is"])
        let quoted <- [for (a in argv); "'{a}'"]
        var out = ""
        let rc = run_and_capture(["/bin/sh", "-c", "{join(quoted, " ")} 2>/dev/null"], out, CELL_TIMEOUT_SEC)
        t |> equal(rc, 0, "exit 0: {tail_of(out)}")
        var non_empty = 0
        for (ln in split(out, "\n")) {
            // the tune policy guard speaks before main from the source tree; the shipped exe has no guard
            non_empty += (empty(strip(ln)) || starts_with(ln, "llvm_tune:")) ? 0 : 1
        }
        t |> equal(non_empty, 1, "stdout carries one line, the answer: {out}")
        t |> success(find(out, "dasllama-cli:") < 0 && find(out, "[W]") < 0, "no progress line and no engine notice on stdout")
    }
    t |> run("the shared knobs: --gpu off --ctx --threads --quant --kv-dtype all take on one run") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["complete", "-m", llm, "-n", "6", "--quiet", "--gpu", "off", "--ctx", "256", "--threads", "4", "--quant", "q8", "--kv-dtype", "q8_0", "The capital of France is"])
        return if (!pass_cell(t, r, "shared knobs"))
        t |> success(find(r.out, "Paris") >= 0, "still Paris: {tail_of(r.out)}")
    }
    t |> run("--config names the file; the model, the ASR pair and the speech model come from it") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)) || model_missing(t, llm) || model_missing(t, model(KITTEN)))
        let cfg = scratch("cli.toml")
        let body = "model = \"{model(SMOL)}\"\nasr = \"{model(QWEN_ASR)}\"\nmmproj = \"{model(QWEN_ASR_MMPROJ)}\"\ntts = \"{model(KITTEN)}\"\nthreads = 4\n"
        t |> success(fwrite(cfg, body), "the config is written")
        let out = scratch("cfg_reply.wav")
        let r = run_cli(["talk", "--config", cfg, "--in", JFK_PATH, "-o", out, "-n", "24"])
        return if (!pass_cell(t, r, "talk from the config alone"))
        t |> success(find(r.out, "using config {cfg}") >= 0, "the config is named")
        t |> success(wav_seconds(t, out) > 0.3lf, "the spoken reply landed")
        remove(out)
        remove(cfg)
    }
}

[test]
def test_doc_chat_commands(t : T?) {
    let llm = model(SMOL)
    t |> run("chat: /help, /read, /load, /speak off then on - the spoken replies are numbered") @(t : T?) {
        return if (model_missing(t, llm) || model_missing(t, model(KITTEN)))
        let read_me = scratch("read_me.txt")
        let saved = scratch("saved.json")
        let script = scratch("cmds.txt")
        let out_dir = scratch("spoken")
        t |> success(fwrite(read_me, "Say the word apple."), "the read file")
        t |> success(fwrite(saved, "\{\"system\": \"\", \"turns\": [\{\"user\": \"hi\", \"assistant\": \"hello\"}]}"), "the saved transcript")
        let lines <- ["/help", "/speak off", "/read {read_me}", "/load {saved}", "/speak on", "Say the word pear.", "/stats", "/exit"]
        t |> success(fwrite(script, "{join(lines, "\n")}\n"), "the script")
        let r = run_cli(["chat", "-m", llm, "--tts", model(KITTEN), "--script", script, "--out-dir", out_dir, "-n", "16", "--quiet"])
        return if (!pass_cell(t, r, "chat commands"))
        t |> success(find(r.out, "/regen") >= 0, "/help printed the list")
        t |> success(find(r.out, "replies will not be spoken") >= 0 && find(r.out, "replies will be spoken") >= 0, "/speak off and on answered")
        t |> success(find(r.out, "loaded 1 turn(s)") >= 0, "/load replaced the conversation")
        t |> success(find(r.out, "prompt ") >= 0, "/stats after a reply")
        t |> success(!fexist(path_join(out_dir, "reply_1.wav")), "the /read turn was not spoken (/speak was off)")
        t |> success(fexist(path_join(out_dir, "reply_2.wav")), "the turn after /load is reply_2 (one loaded turn before it), spoken")
        remove(path_join(out_dir, "reply_2.wav"))
        rmdir(out_dir)
        remove(read_me)
        remove(saved)
        remove(script)
    }
    t |> run("chat --hide-thinking: the answer alone reaches the screen") @(t : T?) {
        return if (model_missing(t, model(QWEN_08)))
        let script = scratch("hide.txt")
        t |> success(fwrite(script, "Reply with the single word OK and nothing else.\n/exit\n"), "script")
        let r = run_cli(["chat", "-m", model(QWEN_08), "--script", script, "-n", "400", "--hide-thinking", "--quiet"])
        return if (!pass_cell(t, r, "hide-thinking"))
        let lines <- answer_lines(r.out)
        t |> equal(length(lines), 1, "one line, the answer: {join(lines, " / ")}")
        remove(script)
    }
    t |> run("embed: one float per line by default") @(t : T?) {
        return if (model_missing(t, llm))
        let r = run_cli(["embed", "-m", llm, "Hello world"])
        return if (!pass_cell(t, r, "embed plain"))
        let lines <- answer_lines(r.out)
        t |> success(length(lines) > 100, "a float per line ({length(lines)} lines)")
        t |> success(find(lines[0], "[") < 0 && find(lines[0], ",") < 0, "not JSON: {lines[0]}")
    }
    t |> run("tokenize --no-special drops the leading specials") @(t : T?) {
        return if (model_missing(t, llm))
        let with_bos = run_cli(["tokenize", "-m", llm, "--ids", "Hello world"])
        let bare = run_cli(["tokenize", "-m", llm, "--ids", "--no-special", "Hello world"])
        return if (!pass_cell(t, with_bos, "tokenize") || !pass_cell(t, bare, "tokenize --no-special"))
        let a <- answer_lines(with_bos.out)
        let b <- answer_lines(bare.out)
        t |> success(length(a) == 1 && length(b) == 1, "one id line each")
        if (length(a) == 1 && length(b) == 1) {
            t |> success(length(split(b[0], " ")) <= length(split(a[0], " ")), "no more ids without the specials: {a[0]} vs {b[0]}")
        }
    }
}

[test]
def test_doc_refusals(t : T?) {
    t |> run("transcribe --lang on a model that detects the language itself is refused, not a crash") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)))
        let r = run_cli(["transcribe", "--asr", model(QWEN_ASR), "--mmproj", model(QWEN_ASR_MMPROJ), "--lang", "en", JFK_PATH])
        t |> success(r.rc != 0, "non-zero exit")
        t |> success(find(r.out, "detects the language itself") >= 0, "names the reason: {tail_of(r.out)}")
        t |> success(find(r.out, "EXCEPTION") < 0, "no panic")
    }
    t |> run("a tower file that does not exist is refused by name, for the vision mmproj and an image") @(t : T?) {
        return if (model_missing(t, model(SMOL)))
        let r = run_cli(["complete", "-m", model(SMOL), "--image-mmproj", "no-such-mmproj.gguf", "--image", model(CATS), "hi"])
        t |> success(r.rc != 0, "non-zero exit")
        t |> success(find(r.out, "vision mmproj not found") >= 0, "names the tower: {tail_of(r.out)}")
        return if (model_missing(t, model(E2B)) || model_missing(t, model(E2B_MMPROJ)))
        let img = run_cli(["complete", "-m", model(E2B), "--image-mmproj", model(E2B_MMPROJ), "--image", "/no/such.jpg", "hi"])
        t |> success(img.rc != 0, "non-zero exit")
        t |> success(find(img.out, "could not decode image") >= 0, "an undecodable image is named: {tail_of(img.out)}")
    }
    t |> run("transcribe --lang auto is the self-detecting model's own setting") @(t : T?) {
        return if (model_missing(t, model(QWEN_ASR)))
        let r = run_cli(["transcribe", "--asr", model(QWEN_ASR), "--mmproj", model(QWEN_ASR_MMPROJ), "--lang", "auto", JFK_PATH])
        return if (!pass_cell(t, r, "lang auto"))
        t |> success(find(to_lower(r.out), "fellow americans") >= 0, "transcribed")
    }
    t |> run("speak --voice that the model does not carry is refused with the voices it does") @(t : T?) {
        return if (model_missing(t, model(KITTEN)))
        let out = scratch("novoice.wav")
        let r = run_cli(["speak", "--tts", model(KITTEN), "--voice", "no-such-voice", "-o", out, "Hello."])
        t |> success(r.rc != 0, "non-zero exit")
        t |> success(find(r.out, "no-such-voice") >= 0 && find(r.out, "expr-voice") >= 0, "names the voice and lists real ones: {tail_of(r.out)}")
        t |> success(!fexist(out), "nothing written")
    }
    t |> run("speak --voice and --speed that the model takes are honoured") @(t : T?) {
        return if (model_missing(t, model(KITTEN)))
        let slow = scratch("slow.wav")
        let fast = scratch("fast.wav")
        let a = run_cli(["speak", "--tts", model(KITTEN), "--voice", "expr-voice-2-f", "--speed", "0.8", "-o", slow, "Hello from the command line."])
        let b = run_cli(["speak", "--tts", model(KITTEN), "--voice", "expr-voice-2-f", "--speed", "1.3", "-o", fast, "Hello from the command line."])
        return if (!pass_cell(t, a, "speak slow") || !pass_cell(t, b, "speak fast"))
        t |> success(find(a.out, "expr-voice-2-f") >= 0, "the voice is reported")
        t |> success(wav_seconds(t, slow) > wav_seconds(t, fast), "a lower speed speaks longer")
        remove(slow)
        remove(fast)
    }
    t |> run("speak --speed on a model that takes none is refused") @(t : T?) {
        return if (model_missing(t, model(POCKET)))
        let out = scratch("pocketspeed.wav")
        let r = run_cli(["speak", "--tts", model(POCKET), "--speed", "1.5", "-o", out, "Hello."])
        t |> success(r.rc != 0, "non-zero exit")
        t |> success(find(r.out, "takes no speed") >= 0, "names the reason: {tail_of(r.out)}")
    }
}

// ===== the tower column: gemma-4-E2B, one mmproj, both towers =====

[test]
def test_towers(t : T?) {
    let llm = model(E2B)
    let mm = model(E2B_MMPROJ)
    t |> run("complete --image: the cats picture through the vision tower") @(t : T?) {
        return if (model_missing(t, llm) || model_missing(t, mm) || model_missing(t, model(CATS)))
        // 160, not 24: the completion streams the family's reasoning before its answer, and the word
        // the cell looks for sits in the answer
        let r = run_cli(["complete", "-m", llm, "--image-mmproj", mm, "--image", model(CATS), "-n", "160", "--quiet", "What animals are in this picture? One word."])
        return if (!pass_cell(t, r, "complete --image"))
        t |> success(find(to_lower(r.out), "cat") >= 0, "it sees cats: {tail_of(r.out)}")
    }
    t |> run("chat --script with /audio: the JFK clip through the audio tower, spoken back by pocket") @(t : T?) {
        return if (model_missing(t, llm) || model_missing(t, mm))
        let script = scratch("audio_script.txt")
        let out_dir = path_join(scratch("spoken"), "nested")   // absent on purpose: the CLI creates it
        let with_tts = !model_missing(t, model(POCKET))
        let cats = model(CATS)
        let lines <- ["/audio {JFK_PATH}", "Transcribe the audio clip exactly, nothing else.",
                      "/image {cats}", "What animals are in this picture? One word.", "/exit"]
        t |> success(fwrite(script, "{join(lines, "\n")}\n"), "the script is written")
        var args <- ["chat", "-m", llm, "--image-mmproj", mm, "--audio-mmproj", mm, "--script", script, "-n", "200", "--quiet", "--no-think"]
        if (with_tts) {
            args |> push("--tts")
            args |> push(model(POCKET))
            args |> push("--out-dir")
            args |> push(out_dir)
        }
        let r = run_cli(args)
        return if (!pass_cell(t, r, "chat /audio"))
        t |> success(find(to_lower(r.out), "americans") >= 0, "it heard the clip: {tail_of(r.out)}")
        if (stat(cats).is_valid) {
            t |> success(find(to_lower(r.out), "cat") >= 0, "and /image on the next turn saw the cats")
        }
        if (with_tts) {
            let spoken = path_join(out_dir, "reply_1.wav")
            t |> success(fexist(spoken), "the reply was spoken")
            if (fexist(spoken)) {
                t |> success(wav_seconds(t, spoken) > 0.5lf, "with speech in it")
                remove(spoken)
            }
        }
        remove(script)
        rmdir(out_dir)
        rmdir(dir_name(out_dir))
    }
}
