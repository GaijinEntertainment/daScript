options gen2
options persistent_heap

// dasllama-cli's model-free half: the plan a command line parses to, the bare-token walk, the
// help surfaces, the config file's keys and roster, the model-path resolution, and the chat
// loop's line logic (commands, the """ block, the saved transcript). No model, no process.

require dastest/testing_boost public
require cli_args
require cli_repl
require daslib/json_boost
require daslib/fio
require daslib/strings_boost   // join
require strings
require math

[test]
def test_plan_dispatch(t : T?) {
    t |> run("no arguments asks for the usage") @(t : T?) {
        let none : array<string>
        let plan <- parse_cli(none)
        t |> success(plan.help, "help")
        t |> equal(plan.cmd, CliCommand.none)
    }
    t |> run("--help and help ask for the usage") @(t : T?) {
        for (word in ["--help", "-?", "help"]) {
            let plan <- parse_cli([word])
            t |> success(plan.help, "{word} is help")
        }
    }
    t |> run("an unknown command is refused by name") @(t : T?) {
        let plan <- parse_cli(["frobnicate", "-m", "x"])
        t |> success(!empty(plan.error), "refused")
        t |> success(find(plan.error, "frobnicate") >= 0, "the refusal names it: {plan.error}")
        t |> success(find(plan.error, "talk") >= 0, "the refusal lists the commands")
    }
    t |> run("every command name dispatches") @(t : T?) {
        for (name in CLI_COMMANDS) {
            let plan <- parse_cli([name])
            t |> success(plan.cmd != CliCommand.none && empty(plan.error), "{name} parses")
        }
    }
    t |> run("<command> --help asks for that command's help, in every spelling and any position") @(t : T?) {
        for (a in [["speak", "--show-help"], ["talk", "--help"], ["complete", "-h"], ["bench", "-m", "x.gguf", "--help"], ["chat", "-?"]]) {
            let plan <- parse_cli(a)
            t |> success(plan.help, "{join(a, " ")} is help")
            t |> equal(plan.error, "", "{join(a, " ")} is not refused")
            t |> equal(plan.cmd, command_of(a[0]))
        }
    }
}

[test]
def test_plan_flags(t : T?) {
    t |> run("complete: the shared, the sampler and its own flags land on one plan") @(t : T?) {
        let plan <- parse_cli(["complete", "-m", "x.gguf", "--gpu", "off", "-t", "4", "--temp", "0.7", "-n", "12", "-p", "hi", "--quiet"])
        t |> equal(plan.error, "")
        t |> equal(plan.common.model, "x.gguf")
        t |> equal(plan.common.gpu, "off")
        t |> equal(plan.common.threads, 4)
        t |> success(abs(plan.sampler.temp - 0.7) < 1.0e-6, "temp")
        t |> equal(plan.sampler.max_tokens, 12)
        t |> equal(plan.complete.prompt, "hi")
        t |> success(plan.complete.quiet, "quiet")
        t |> equal(length(plan.bare), 0)
    }
    t |> run("the first bare token is the prompt, a value flag's value is not") @(t : T?) {
        let plan <- parse_cli(["complete", "-m", "x.gguf", "-t", "8", "hello there", "--top-k=40"])
        t |> equal(plan.error, "")
        t |> equal(length(plan.bare), 1)
        t |> equal(plan.bare[0], "hello there")
        t |> equal(plan.sampler.top_k, 40)
    }
    t |> run("transcribe: the file rides bare, the hint is a flag") @(t : T?) {
        let plan <- parse_cli(["transcribe", "--asr", "a.bin", "q.wav", "-l", "de"])
        t |> equal(plan.error, "")
        t |> equal(plan.transcribe.asr, "a.bin")
        t |> equal(plan.transcribe.lang, "de")
        t |> equal(length(plan.bare), 1)
        t |> equal(plan.bare[0], "q.wav")
    }
    t |> run("talk: the three models and the clip") @(t : T?) {
        let plan <- parse_cli(["talk", "-a", "a.gguf", "--mmproj", "mm.gguf", "-m", "l.gguf", "--tts", "t.gguf", "--in", "q.wav", "-o", "r.wav", "--no-think"])
        t |> equal(plan.error, "")
        t |> equal(length(plan.bare), 0, "--in took its value; nothing fell through as a bare token")
        t |> equal(plan.talk.asr, "a.gguf")
        t |> equal(plan.talk.mmproj, "mm.gguf")
        t |> equal(plan.common.model, "l.gguf")
        t |> equal(plan.talk.tts, "t.gguf")
        t |> equal(plan.talk.in_file, "q.wav")
        t |> equal(plan.talk.out, "r.wav")
        t |> success(plan.talk.no_think, "no_think")
    }
    t |> run("chat: the towers, the speech model and the script") @(t : T?) {
        let plan <- parse_cli(["chat", "-m", "l.gguf", "--image-mmproj", "v.gguf", "--audio-mmproj", "v.gguf", "--tts", "t.gguf", "--script", "s.txt", "--hide-thinking"])
        t |> equal(plan.error, "")
        t |> equal(plan.chat.image_mmproj, "v.gguf")
        t |> equal(plan.chat.audio_mmproj, "v.gguf")
        t |> equal(plan.chat.tts, "t.gguf")
        t |> equal(plan.chat.script, "s.txt")
        t |> success(plan.chat.hide_thinking, "hide_thinking")
    }
    t |> run("bench: the llama-bench spellings and their defaults") @(t : T?) {
        let plan <- parse_cli(["bench", "-m", "l.gguf", "-r", "3", "--npl", "4", "-o", "md"])
        t |> equal(plan.error, "")
        t |> equal(plan.bench.plen, 512)
        t |> equal(plan.bench.ngen, 128)
        t |> equal(plan.bench.reps, 3)
        t |> equal(plan.bench.npl, 4)
        t |> equal(plan.bench.output, "md")
    }
    t |> run("tokenize and embed take their text bare or by flag") @(t : T?) {
        let a <- parse_cli(["tokenize", "-m", "l.gguf", "--ids", "a b c"])
        t |> equal(a.error, "")
        t |> success(a.tokenize.ids, "ids")
        t |> equal(a.bare[0], "a b c")
        let b <- parse_cli(["embed", "-m", "l.gguf", "--text", "x", "--json"])
        t |> equal(b.error, "")
        t |> equal(b.embed.text, "x")
        t |> success(b.embed.json, "json")
    }
}

[test]
def test_plan_flag_neighbours(t : T?) {
    t |> run("another struct's value flag never shadows this one's: --gpu off then --temp 2.0") @(t : T?) {
        let plan <- parse_cli(["complete", "-m", "x.gguf", "--gpu", "off", "--temp", "2.0", "hi"])
        t |> equal(plan.error, "")
        t |> equal(plan.common.gpu, "off")
        t |> success(abs(plan.sampler.temp - 2.0) < 1.0e-6, "temp parsed after --gpu off: {plan.sampler.temp}")
        t |> equal(plan.bare[0], "hi")
        let chat <- parse_cli(["chat", "--quiet", "--gpu", "off", "-n", "12", "--temp", "0.5", "--script", "s.txt", "-m", "l.gguf"])
        t |> equal(chat.error, "")
        t |> success(chat.chat.quiet, "quiet")
        t |> equal(chat.sampler.max_tokens, 12)
        t |> success(abs(chat.sampler.temp - 0.5) < 1.0e-6, "temp")
        t |> equal(chat.chat.script, "s.txt")
        t |> equal(chat.common.model, "l.gguf")
        t |> equal(length(chat.bare), 0)
    }
    t |> run("the shape that silently decoded greedy: a model path, -n, --temp, then a prompt with spaces") @(t : T?) {
        let plan <- parse_cli(["complete", "-m", "/Users/x/models/Qwen3.5-4B-Q8_0.gguf", "-n", "128", "--temp", "0.7", "--top-k", "20", "--top-p", "0.95", "Write a long story about a lighthouse keeper."])
        t |> equal(plan.error, "")
        t |> success(abs(plan.sampler.temp - 0.7) < 1.0e-6, "temp: {plan.sampler.temp}")
        t |> equal(plan.sampler.top_k, 20)
        t |> equal(plan.sampler.max_tokens, 128)
        t |> equal(plan.bare[0], "Write a long story about a lighthouse keeper.")
    }
    t |> run("a value that does not parse is a refusal, not a zero") @(t : T?) {
        let plan <- parse_cli(["complete", "-m", "l.gguf", "-n", "twelve"])
        t |> success(!empty(plan.error), "refused: {plan.error}")
    }
}

[test]
def test_sampler_defaults(t : T?) {
    t |> run("unset knobs: greedy for a completion, the chat preset for a conversation") @(t : T?) {
        let unset <- parse_cli(["chat", "-m", "l.gguf"])
        let chat = resolve_sampler(unset.sampler, true)
        t |> success(chat.temp > 0.0, "a conversation samples by default - greedy loops a thinking model on its own draft")
        t |> equal(chat.top_k, 20)
        t |> success(chat.top_p < 1.0, "nucleus on")
        let greedy = resolve_sampler(unset.sampler, false)
        t |> success(greedy.temp == 0.0, "a completion is greedy by default")
        t |> equal(greedy.top_k, 0)
        t |> success(greedy.top_p == 1.0, "nucleus off")
    }
    t |> run("an explicit 0 is greedy even in chat; explicit values win") @(t : T?) {
        let plan <- parse_cli(["chat", "-m", "l.gguf", "--temp", "0", "--top-k", "5", "--top-p", "0.5"])
        let pick = resolve_sampler(plan.sampler, true)
        t |> success(pick.temp == 0.0, "temp 0 stays greedy")
        t |> equal(pick.top_k, 5)
        t |> success(abs(pick.top_p - 0.5) < 1.0e-6, "top_p")
    }
}

[test]
def test_think_print(t : T?) {
    t |> run("reasoning streams when shown, one blank line before the answer, then the answer") @(t : T?) {
        var thought = false
        t |> equal(think_print(true, "let me ", "", thought), "let me ")
        t |> equal(think_print(true, "think", "", thought), "think")
        t |> success(thought, "in a reasoning span")
        t |> equal(think_print(true, "", "Hello", thought), "\n\nHello")
        t |> success(!thought, "the span closed")
        t |> equal(think_print(true, "", "!", thought), "!")
    }
    t |> run("hidden reasoning prints nothing and adds no blank line") @(t : T?) {
        var thought = false
        t |> equal(think_print(false, "let me think", "", thought), "")
        t |> equal(think_print(false, "", "Hello", thought), "Hello")
    }
    t |> run("a reply with no reasoning is the answer alone") @(t : T?) {
        var thought = false
        t |> equal(think_print(true, "", "Hello", thought), "Hello")
    }
}

[test]
def test_help_surfaces(t : T?) {
    t |> run("the usage names every command") @(t : T?) {
        let usage = cli_usage()
        for (name in CLI_COMMANDS) {
            t |> success(find(usage, name) >= 0, "usage names {name}")
        }
    }
    t |> run("a command's help carries its own flags, the sampler where it samples, and the shared flags") @(t : T?) {
        let complete = command_help(CliCommand.complete)
        t |> success(find(complete, "--prompt") >= 0, "own flag")
        t |> success(find(complete, "--temp") >= 0, "sampler")
        t |> success(find(complete, "--gpu") >= 0, "shared")
        let speak = command_help(CliCommand.speak)
        t |> success(find(speak, "--voice") >= 0, "own flag")
        t |> success(find(speak, "--temp") < 0, "speak does not sample")
        t |> success(find(speak, "--gpu") >= 0, "shared")
    }
}

[test]
def test_text_input(t : T?) {
    t |> run("the flag wins, then the file, then the bare token") @(t : T?) {
        var err = ""
        var terr = ""
        let tmp = temp_directory(terr)
        let file = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_text_{ref_time_ticks()}.txt")
        t |> success(fwrite(file, "from the file"), "wrote the file")
        t |> equal(text_input("flag", file, ["bare"], err), "flag")
        t |> equal(text_input("", file, ["bare"], err), "from the file")
        t |> equal(text_input("", "", ["bare"], err), "bare")
        t |> equal(text_input("", "", [], err), "")
        t |> equal(err, "")
        remove(file)
        t |> equal(text_input("", file, [], err), "")
        t |> success(!empty(err), "an unreadable file is an error")
    }
}

[test]
def test_config_file(t : T?) {
    let body = "model = \"a.gguf\"\ngpu = \"vulkan\"\ngpu_layers = 3\nthreads = 6\nasr = \"asr.bin\"\ntts = \"t.gguf\"\nimage_mmproj = \"v.gguf\"\nmodels_dir = \"/m\"\n"
    t |> run("the flat keys land") @(t : T?) {
        var cfg = CliConfig()
        t |> success(parse_cli_config(body, cfg), "parses")
        t |> equal(cfg.model, "a.gguf")
        t |> equal(cfg.gpu, "vulkan")
        t |> equal(cfg.gpu_layers, 3)
        t |> success(cfg.gpu_layers_set, "layers set")
        t |> success(!cfg.gpu_stream_set, "stream unset")
        t |> equal(cfg.threads, 6)
        t |> success(cfg.threads_set, "threads set")
        t |> equal(cfg.asr, "asr.bin")
        t |> equal(cfg.tts, "t.gguf")
        t |> equal(cfg.image_mmproj, "v.gguf")
        t |> equal(cfg.models_dir, "/m")
    }
    t |> run("a roster's default entry supplies the model and its overrides") @(t : T?) {
        let roster = "gpu = \"off\"\n[[models]]\npath = \"first.gguf\"\n[[models]]\npath = \"second.gguf\"\ndefault = true\nimage_mmproj = \"v2.gguf\"\nctx = 2048\nquant = \"q4_k\"\n"
        var cfg = CliConfig()
        t |> success(parse_cli_config(roster, cfg), "parses")
        t |> equal(cfg.model, "second.gguf")
        t |> equal(cfg.image_mmproj, "v2.gguf")
        t |> equal(cfg.ctx, 2048)
        t |> equal(cfg.quant, "q4_k")
    }
    t |> run("a roster with no default takes the first entry; a flat model key wins over the roster") @(t : T?) {
        var cfg = CliConfig()
        t |> success(parse_cli_config("[[models]]\npath = \"first.gguf\"\n[[models]]\npath = \"second.gguf\"\n", cfg), "parses")
        t |> equal(cfg.model, "first.gguf")
        var flat = CliConfig()
        t |> success(parse_cli_config("model = \"flat.gguf\"\n[[models]]\npath = \"first.gguf\"\n", flat), "parses")
        t |> equal(flat.model, "flat.gguf")
    }
    t |> run("bad TOML is the error, not a silent empty config") @(t : T?) {
        var cfg = CliConfig()
        t |> success(!parse_cli_config("model = [unclosed\n", cfg), "refused")
        t |> success(!empty(cfg.error), "with a reason")
    }
    t |> run("the config fills only what the flags left empty") @(t : T?) {
        var cfg = CliConfig()
        parse_cli_config(body, cfg)
        let plan <- parse_cli(["complete", "-m", "cli.gguf", "-t", "2"])
        var c := plan.common
        fill_common(c, cfg)
        t |> equal(c.model, "cli.gguf")
        t |> equal(c.threads, 2)
        t |> equal(c.gpu, "vulkan")
        t |> equal(c.models_dir, "/m")
    }
    t |> run("an explicit --config that is missing is the error; a discovered one is optional") @(t : T?) {
        var terr = ""
        let tmp = temp_directory(terr)
        let dir = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_cfg_{ref_time_ticks()}")
        mkdir_rec(dir)
        let missing = load_cli_config(path_join(dir, "nope.toml"), dir)
        t |> success(!empty(missing.error), "missing explicit config refused")
        let none = load_cli_config("", dir)
        t |> equal(none.path, "")
        t |> equal(none.error, "")
        let beside = path_join(dir, "dasllama-server.toml")
        t |> success(fwrite(beside, "model = \"beside.gguf\"\n"), "planted")
        let found = load_cli_config("", dir)
        t |> equal(found.path, beside)
        t |> equal(found.model, "beside.gguf")
        remove(beside)
        rmdir(dir)
    }
}

[test]
def test_model_path(t : T?) {
    t |> run("a path as given, a bare name under the models dir, else nothing") @(t : T?) {
        var terr = ""
        let tmp = temp_directory(terr)
        let dir = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_models_{ref_time_ticks()}")
        mkdir_rec(dir)
        let under = path_join(dir, "tiny.gguf")
        t |> success(fwrite(under, "GGUF"), "planted")
        t |> equal(resolve_model_path(under, ""), under)
        t |> equal(resolve_model_path("tiny.gguf", dir), under)
        t |> equal(resolve_model_path("absent.gguf", dir), "")
        t |> equal(resolve_model_path("", dir), "")
        t |> equal(resolve_model_path("sub/tiny.gguf", dir), "", "a relative path is never re-rooted")
        remove(under)
        rmdir(dir)
    }
}

[test]
def test_repl_lines(t : T?) {
    t |> run("a plain line is a message, a slash line a command") @(t : T?) {
        t |> equal(parse_repl_line("hello there").op, ReplOp.say)
        t |> equal(parse_repl_line("hello there").arg, "hello there")
        t |> equal(parse_repl_line("   ").op, ReplOp.nothing)
        t |> equal(parse_repl_line("/exit").op, ReplOp.exit)
        t |> equal(parse_repl_line("/quit").op, ReplOp.exit)
        t |> equal(parse_repl_line("/image  /tmp/a.jpg ").op, ReplOp.image)
        t |> equal(parse_repl_line("/image  /tmp/a.jpg ").arg, "/tmp/a.jpg")
        t |> equal(parse_repl_line("/system You are terse.").arg, "You are terse.")
        t |> equal(parse_repl_line("/speak on").op, ReplOp.speak)
        t |> equal(parse_repl_line("/nope").op, ReplOp.unknown)
        t |> equal(parse_repl_line("/regen").arg, "")
    }
    t |> run("a triple-quote block collects until it closes") @(t : T?) {
        var st = ReplState()
        t |> equal(repl_feed(st, "\"\"\"").op, ReplOp.nothing)
        t |> success(st.in_block, "open")
        t |> equal(repl_feed(st, "line one").op, ReplOp.nothing)
        t |> equal(repl_feed(st, "/exit").op, ReplOp.nothing, "a command inside a block is text")
        let closed = repl_feed(st, "\"\"\"")
        t |> equal(closed.op, ReplOp.say)
        t |> equal(closed.arg, "line one\n/exit")
        t |> success(!st.in_block, "closed")
        t |> equal(repl_feed(st, "\"\"\"").op, ReplOp.nothing)
        t |> equal(repl_feed(st, "\"\"\"").op, ReplOp.nothing, "an empty block sends nothing")
    }
    t |> run("the transcript saves and loads") @(t : T?) {
        let st <- ReplState(system = "be brief",
                            turns <- [ReplTurn(user = "hi", assistant = "hello"), ReplTurn(user = "2+2?", assistant = "4")],
                            images <- ["stale.jpg"])
        let text = repl_save_json(st)
        var back = ReplState()
        var err = ""
        t |> success(repl_load_json(text, back, err), "loads: {err}")
        t |> equal(back.system, "be brief")
        t |> equal(length(back.turns), 2)
        t |> equal(back.turns[1].user, "2+2?")
        t |> equal(back.turns[1].assistant, "4")
        t |> equal(length(back.images), 0)
    }
    t |> run("a file that is not a transcript is refused with a reason") @(t : T?) {
        var st = ReplState()
        var err = ""
        t |> success(!repl_load_json("\{\"system\": 1}", st, err), "refused")
        t |> success(!empty(err), "with a reason")
        t |> success(!repl_load_json("not json", st, err), "refused")
        t |> success(!empty(err), "with a reason")
    }
    t |> run("the help lists every command") @(t : T?) {
        let help = repl_help()
        for (word in ["/clear", "/regen", "/system", "/image", "/audio", "/read", "/save", "/load", "/stats", "/speak", "/exit"]) {
            t |> success(find(help, word) >= 0, "help names {word}")
        }
    }
}

[test]
def test_validate_plan(t : T?) {
    let none = CliConfig()
    t |> run("every pre-load refusal, by command") @(t : T?) {
        let cases <- [
            (args = ["complete", "-m", "x.gguf"], wants = "nothing to complete"),
            (args = ["complete", "-m", "x.gguf", "--image", "a.jpg", "hi"], wants = "--image needs --image-mmproj"),
            (args = ["complete", "-m", "x.gguf", "--file", "/no/such/file.txt"], wants = "could not read"),
            (args = ["speak", "--tts", "t.gguf", "--file", "/no/such/file.txt"], wants = "could not read"),
            (args = ["embed", "-m", "x.gguf", "--file", "/no/such/file.txt"], wants = "could not read"),
            (args = ["tokenize", "-m", "x.gguf", "--file", "/no/such/file.txt"], wants = "could not read"),
            (args = ["chat", "-m", "x.gguf", "--script", "/no/such/script.txt"], wants = "could not read --script"),
            (args = ["transcribe", "--asr", "a.bin"], wants = "nothing to transcribe"),
            (args = ["transcribe", "q.wav"], wants = "no ASR model"),
            (args = ["speak", "--tts", "t.gguf"], wants = "nothing to say"),
            (args = ["speak", "hello"], wants = "no TTS model"),
            (args = ["talk", "--tts", "t.gguf"], wants = "nothing to answer"),
            (args = ["talk", "--tts", "t.gguf", "--in", "q.wav"], wants = "no ASR model to hear"),
            (args = ["talk", "--prompt", "hi"], wants = "no TTS model to speak"),
            (args = ["embed", "-m", "x.gguf"], wants = "nothing to embed"),
            (args = ["tokenize", "-m", "x.gguf"], wants = "nothing to tokenize"),
            (args = ["bench", "-m", "x.gguf", "-o", "csv"], wants = "--output is txt | md"),
            (args = ["bench", "-m", "x.gguf", "-p", "0", "-n", "0"], wants = "nothing to measure")
        ]
        for (c in cases) {
            let plan <- parse_cli(c.args)
            t |> equal(plan.error, "", "{join(c.args, " ")} parses")
            let why = validate_plan(plan, none)
            t |> success(find(why, c.wants) >= 0, "{join(c.args, " ")} -> '{why}' names '{c.wants}'")
        }
    }
    t |> run("the config supplies what a refusal would ask for") @(t : T?) {
        let cfg = CliConfig(asr = "a.bin", tts = "t.gguf", image_mmproj = "v.gguf")
        t |> equal(validate_plan(parse_cli(["transcribe", "q.wav"]), cfg), "")
        t |> equal(validate_plan(parse_cli(["speak", "hello"]), cfg), "")
        t |> equal(validate_plan(parse_cli(["talk", "--in", "q.wav"]), cfg), "")
        t |> equal(validate_plan(parse_cli(["complete", "-m", "x", "--image", "a.jpg", "hi"]), cfg), "")
    }
    t |> run("a plan that holds is not refused") @(t : T?) {
        for (a in [["complete", "-m", "x", "hi"], ["chat", "-m", "x"], ["transcribe", "--asr", "a", "q.wav"], ["speak", "--tts", "t", "hi"],
                   ["talk", "--asr", "a", "--tts", "t", "--in", "q.wav"], ["talk", "--tts", "t", "--prompt", "hi"],
                   ["talk", "--asr", "a", "--tts", "t", "q.wav"], ["embed", "-m", "x", "hi"], ["tokenize", "-m", "x", "hi"],
                   ["bench", "-m", "x"], ["bench", "-m", "x", "-p", "0"], ["bench", "-m", "x", "-o", "txt"]]) {
            t |> equal(validate_plan(parse_cli(a), none), "", "{join(a, " ")} holds")
        }
    }
}

[test]
def test_plan_parse_errors(t : T?) {
    t |> run("--show-help as the first word asks for the usage") @(t : T?) {
        t |> success(parse_cli(["--show-help"]).help, "help")
    }
    t |> run("a shared flag that does not parse is a refusal") @(t : T?) {
        let plan <- parse_cli(["complete", "-t", "abc", "hi"])
        t |> success(!empty(plan.error), "refused: {plan.error}")
    }
    t |> run("one bad value per command is a refusal") @(t : T?) {
        for (a in [["complete", "-n", "twelve"], ["chat", "--speed", "fast"], ["transcribe", "--asr", "a", "--lang"], ["speak", "--speed", "slow"],
                   ["talk", "--speed", "slow"], ["embed", "--json=maybe"], ["tokenize", "--ids=maybe"], ["bench", "-r", "three"]]) {
            let plan <- parse_cli(a)
            t |> success(!empty(plan.error), "{join(a, " ")} refused: {plan.error}")
        }
    }
    t |> run("talk samples: --temp lands on its plan") @(t : T?) {
        let plan <- parse_cli(["talk", "-p", "x", "--temp", "0.3"])
        t |> success(abs(plan.sampler.temp - 0.3) < 1.0e-6, "temp")
    }
    t |> run("a lone dash is a bare token") @(t : T?) {
        let plan <- parse_cli(["complete", "-m", "x", "-"])
        t |> equal(length(plan.bare), 1)
        t |> equal(plan.bare[0], "-")
    }
    t |> run("speak, embed, tokenize: a value flag before the bare text never eats it") @(t : T?) {
        t |> equal(parse_cli(["speak", "--tts", "t.gguf", "-o", "o.wav", "Hi"]).bare[0], "Hi")
        t |> equal(parse_cli(["embed", "-m", "x", "--file", "f.txt", "hello"]).bare[0], "hello")
        t |> equal(parse_cli(["tokenize", "-m", "x", "--file", "f.txt", "hello"]).bare[0], "hello")
    }
    t |> run("every command's help carries one of its own flags") @(t : T?) {
        t |> success(find(command_help(CliCommand.chat), "--script") >= 0, "chat")
        t |> success(find(command_help(CliCommand.transcribe), "--asr") >= 0, "transcribe")
        t |> success(find(command_help(CliCommand.talk), "--in") >= 0, "talk")
        t |> success(find(command_help(CliCommand.embed), "--json") >= 0, "embed")
        t |> success(find(command_help(CliCommand.tokenize), "--ids") >= 0, "tokenize")
        t |> success(find(command_help(CliCommand.bench), "--npl") >= 0, "bench")
    }
}

[test]
def test_config_edges(t : T?) {
    t |> run("an empty roster leaves the model empty") @(t : T?) {
        var cfg = CliConfig()
        t |> success(parse_cli_config("models = []\n", cfg), "parses")
        t |> equal(cfg.model, "")
    }
    t |> run("the roster fills kv_dtype too") @(t : T?) {
        var cfg = CliConfig()
        t |> success(parse_cli_config("[[models]]\npath = \"a.gguf\"\nkv_dtype = \"q8_0\"\n", cfg), "parses")
        t |> equal(cfg.kv_dtype, "q8_0")
    }
    t |> run("fill_common lands all six keys on an empty plan") @(t : T?) {
        var cfg = CliConfig()
        t |> success(parse_cli_config("model = \"a.gguf\"\nquant = \"q4\"\nkv_dtype = \"f32\"\nmetal = \"auto\"\nctx = 512\nthreads = 3\n", cfg), "parses")
        var c := parse_cli(["complete", "hi"]).common
        fill_common(c, cfg)
        t |> equal(c.model, "a.gguf")
        t |> equal(c.quant, "q4")
        t |> equal(c.kv_dtype, "f32")
        t |> equal(c.metal, "auto")
        t |> equal(c.ctx, 512)
        t |> equal(c.threads, 3)
    }
    t |> run("a relative path with a separator is never re-rooted under the models dir") @(t : T?) {
        var terr = ""
        let tmp = temp_directory(terr)
        let dir = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_reroot_{ref_time_ticks()}")
        mkdir_rec(path_join(dir, "sub"))
        let planted = path_join(path_join(dir, "sub"), "tiny.gguf")
        t |> success(fwrite(planted, "GGUF"), "planted")
        t |> equal(resolve_model_path("sub/tiny.gguf", dir), "", "exists under the dir, still not re-rooted")
        remove(planted)
        rmdir(path_join(dir, "sub"))
        rmdir(dir)
    }
}

[test]
def test_repl_names(t : T?) {
    t |> run("every command spelling maps") @(t : T?) {
        t |> equal(parse_repl_line("/help").op, ReplOp.help)
        t |> equal(parse_repl_line("/?").op, ReplOp.help)
        t |> equal(parse_repl_line("/q").op, ReplOp.exit)
        t |> equal(parse_repl_line("/read f.txt").op, ReplOp.read)
        t |> equal(parse_repl_line("/load f.json").op, ReplOp.load)
        t |> equal(parse_repl_line("/regen").op, ReplOp.regen)
        t |> equal(parse_repl_line("/save f.json").op, ReplOp.save)
        t |> equal(parse_repl_line("/clear").op, ReplOp.clear)
        t |> equal(parse_repl_line("/stats").op, ReplOp.stats)
        t |> equal(parse_repl_line("/audio a.wav").op, ReplOp.audio)
    }
    t |> run("a transcript whose turns are not an array is refused") @(t : T?) {
        var st = ReplState()
        var err = ""
        t |> success(!repl_load_json("\{\"turns\": 1}", st, err), "refused")
        t |> success(find(err, "array") >= 0, "the reason: {err}")
    }
}

// ===== the chat loop over a fake engine =====

//! A model that echoes: every message becomes the turn `echo:<text>` tagged with the attachments
//! it consumed; every call is recorded, so a cell asserts what the loop asked for.
class FakeEngine : ReplEngine {
    lines : array<string>
    at : int
    out : string
    said : array<string>
    reopened : int
    stats : string
    speak_ok : bool

    def override read_line(var line : string&) : bool {
        if (at >= length(lines)) {
            return false
        }
        line = lines[at]
        at++
        return true
    }
    def override emit(text : string) : void {
        out = "{out}{text}"
    }
    def override say(var st : ReplState; text : string) : void {
        let tag = "img={length(st.images)} aud={length(st.audio)} speak={st.speak}"
        st.images |> clear()
        st.audio |> clear()
        said |> push("{text} [{tag}]")
        st.turns |> push(ReplTurn(user = text, assistant = "echo:{text}"))
        stats = "gen {length(said)}"
    }
    def override reopen(var st : ReplState) : void {
        reopened++
    }
    def override last_stats : string {
        return stats
    }
    def override can_speak : bool {
        return speak_ok
    }
}

def private drive(lines : array<string>; speak_ok : bool = false) : tuple<st : ReplState; eng : FakeEngine?> {
    var eng = new FakeEngine()
    eng.lines := lines
    eng.speak_ok = speak_ok
    var st = ReplState(speak = speak_ok)
    repl_run(st, eng)
    return <- (st = st, eng = eng)
}

[test]
def test_repl_loop(t : T?) {
    t |> run("messages reach the model in order, the block as one, and /exit stops the input") @(t : T?) {
        var r <- drive(["hello", "\"\"\"", "line one", "line two", "\"\"\"", "/exit", "never"])
        t |> equal(length(r.eng.said), 2)
        t |> equal(r.st.turns[0].user, "hello")
        t |> equal(r.st.turns[1].user, "line one\nline two")
        t |> equal(r.st.turns[1].assistant, "echo:line one\nline two")
        unsafe { delete r.eng }
    }
    t |> run("/regen pops the last turn, reopens, and asks again") @(t : T?) {
        var r <- drive(["/regen", "hi", "/regen"])
        t |> success(find(r.eng.out, "nothing to regenerate yet") >= 0, "before a message")
        t |> equal(length(r.eng.said), 2)
        t |> equal(r.eng.said[1], "hi [img=0 aud=0 speak=false]")
        t |> equal(length(r.st.turns), 1, "one turn after the redo")
        t |> equal(r.eng.reopened, 1)
        unsafe { delete r.eng }
    }
    t |> run("/clear forgets the turns and reopens; /system sets, shows, and reopens") @(t : T?) {
        var r <- drive(["a", "b", "/clear", "/system", "/system be terse", "/system"])
        t |> equal(length(r.st.turns), 0)
        t |> success(find(r.eng.out, "conversation cleared") >= 0, "cleared")
        t |> success(find(r.eng.out, "no system prompt") >= 0, "shown empty")
        t |> success(find(r.eng.out, "system prompt set") >= 0, "set")
        t |> success(find(r.eng.out, "system: be terse") >= 0, "shown")
        t |> equal(r.st.system, "be terse")
        t |> equal(r.eng.reopened, 2)
        unsafe { delete r.eng }
    }
    t |> run("/image and /audio queue for the next message; the later kind replaces the other; a second /image replaces the first") @(t : T?) {
        var r <- drive(["/image a.jpg", "/audio a.wav", "what?", "/audio b.wav", "/image b.jpg", "/image c.jpg", "and?", "/audio c.wav", "/audio d.wav", "both"])
        t |> success(find(r.eng.out, "the queued image is dropped") >= 0, "/audio after /image")
        t |> success(find(r.eng.out, "the queued audio is dropped") >= 0, "/image after /audio")
        t |> success(find(r.eng.out, "replaces the queued image") >= 0, "a second /image says it replaced the first - one image per message")
        t |> equal(r.eng.said[0], "what? [img=0 aud=1 speak=false]")
        t |> equal(r.eng.said[1], "and? [img=1 aud=0 speak=false]")
        t |> equal(r.eng.said[2], "both [img=0 aud=2 speak=false]", "two clips ride one message")
        t |> equal(r.st.images |> length, 0, "consumed")
        unsafe { delete r.eng }
    }
    t |> run("/read sends a file's text, and names a file it cannot read") @(t : T?) {
        var terr = ""
        let tmp = temp_directory(terr)
        let file = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_read_{ref_time_ticks()}.txt")
        t |> success(fwrite(file, "from the file"), "wrote")
        var r <- drive(["/read {file}", "/read /no/such/file.txt"])
        t |> equal(length(r.eng.said), 1)
        t |> equal(r.st.turns[0].user, "from the file")
        t |> success(find(r.eng.out, "could not read /no/such/file.txt") >= 0, "the missing file is named")
        remove(file)
        unsafe { delete r.eng }
    }
    t |> run("/save writes the transcript, /load replaces it and reopens; a bad file is named") @(t : T?) {
        var terr = ""
        let tmp = temp_directory(terr)
        let file = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_conv_{ref_time_ticks()}.json")
        var r <- drive(["one", "two", "/save {file}", "/clear", "/load {file}", "/load /no/such.json", "three"])
        t |> success(find(r.eng.out, "saved {file}") >= 0, "saved")
        t |> success(find(r.eng.out, "loaded 2 turn(s)") >= 0, "loaded: {r.eng.out}")
        t |> success(find(r.eng.out, "could not load /no/such.json") >= 0, "the missing file is named")
        t |> equal(length(r.st.turns), 3, "two loaded and one more")
        t |> equal(r.eng.reopened, 2, "/clear and /load each reopen")
        remove(file)
        unsafe { delete r.eng }
    }
}

[test]
def test_repl_loop_toggles(t : T?) {
    t |> run("/load names why a file is not a transcript; /save names a directory it cannot write into") @(t : T?) {
        var terr = ""
        let tmp = temp_directory(terr)
        let junk = path_join(empty(tmp) ? "." : tmp, "dasllama_cli_junk_{ref_time_ticks()}.json")
        t |> success(fwrite(junk, "\{\"system\": 1}"), "wrote")
        var r <- drive(["/load {junk}", "/save /no/such/dir/x.json"])
        t |> success(find(r.eng.out, "could not load {junk}: not a saved conversation") >= 0, "the reason rides the message: {r.eng.out}")
        t |> success(find(r.eng.out, "could not write /no/such/dir/x.json") >= 0, "the write failure is named")
        remove(junk)
        unsafe { delete r.eng }
    }
    t |> run("/stats before and after a reply; /speak needs a speech model") @(t : T?) {
        var r <- drive(["/stats", "hi", "/stats", "/speak on", "/speak off"])
        t |> success(find(r.eng.out, "no reply yet") >= 0, "before")
        t |> success(find(r.eng.out, "gen 1\n") >= 0, "after")
        t |> success(find(r.eng.out, "no speech model") >= 0, "/speak on refused without a model")
        t |> success(!r.st.speak, "stays off")
        unsafe { delete r.eng }
        var s <- drive(["/speak off", "quiet", "/speak on", "loud"], true)
        t |> success(find(s.eng.out, "replies will not be spoken") >= 0 && find(s.eng.out, "replies will be spoken") >= 0, "both toggles answered")
        t |> equal(s.eng.said[0], "quiet [img=0 aud=0 speak=false]")
        t |> equal(s.eng.said[1], "loud [img=0 aud=0 speak=true]")
        unsafe { delete s.eng }
    }
    t |> run("/help prints the list; an unknown command is named") @(t : T?) {
        var r <- drive(["/help", "/bogus"])
        t |> success(find(r.eng.out, "/regen") >= 0, "the list")
        t |> success(find(r.eng.out, "unknown command") >= 0, "unknown")
        t |> equal(length(r.eng.said), 0)
        unsafe { delete r.eng }
    }
}
