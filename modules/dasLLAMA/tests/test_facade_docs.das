options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
options _dasllama_internal = true   // the gate cells' child helper is a fixture of this folder (`_child_run.das`)

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost
require daslib/rtti
require _child_run   // with_child_log_info: a gate child's held lines
require dasllama/dasllama   // nolint:STYLE030 - the dependency cell reads this module's graph by name
require ../../../examples/dasLLAMA/library/dasllama_lib.das

// Drift detector for the dasllama facade (the documented public API). Three invariants:
//  1. every facade `def` carries a `//!` docstring INSIDE its body — the doc extractor
//     (daslib/rst_comment) attaches only body-leading `//!`; a `//!` above the def is silently
//     invisible, so that pattern is rejected outright;
//  2. facade stubs and the engine's trailing-underscore spellings stay 1:1 — adding `foo_` to
//     the engine without a documented `foo` stub (or vice versa) fails here;
//  3. the examples stay facade-only: their one dasllama require is
//     the facade, dasllama/dasllama_core (run.das and chat.das decode
//     no audio or image file, so they take the engine without the
//     file decoders dasllama/dasllama.das adds).

let FACADE = "modules/dasLLAMA/dasllama/dasllama_core.das"
let FACADE_REQUIRE = "dasllama/dasllama_core"
let ENGINE_FILES = [
    "modules/dasLLAMA/dasllama/dasllama_common.das",
    "modules/dasLLAMA/dasllama/dasllama_load.das",      // load_model_ (the common split moved it)
    "modules/dasLLAMA/dasllama/dasllama_blocks.das",    // eval_embd_ / embed_
    "modules/dasLLAMA/dasllama/dasllama_batch.das",     // eval_batch_ / create_batch_workspace_
    "modules/dasLLAMA/dasllama/dasllama_sampling.das",  // sample_ / generate_ / generate_embd_ / stats_ / set_seed_
    "modules/dasLLAMA/dasllama/dasllama_dflash.das",    // attach_block_drafter_ / detach_block_drafter_ / block_drafter_attached_
    "modules/dasLLAMA/dasllama/dasllama_prefix.das",
    "modules/dasLLAMA/dasllama/dasllama_chat.das",
    "modules/dasLLAMA/dasllama/dasllama_gemma4uv.das",   // encode_image_ (the vision splice's encode half)
    "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das",   // encode_audio_ (encode_image_'s audio twin)
    "modules/dasLLAMA/dasllama/dasllama_math.das",   // setup_dasllama_jobque_ + the dispatch knobs
    "modules/dasLLAMA/dasllama/dasllama_par.das",    // set_single_thread_ / get_single_thread_ (the dispatch macros' gate)
    "modules/dasLLAMA/dasllama/dasllama_image.das",  // dlim_inventory_ / dlim_clean_ / prepared_image_alignment_ (the image surface)
    "modules/dasLLAMA/dasllama/dasllama_tune.das",   // kernel_report_
    "modules/dasLLAMA/dasllama/dasllama_gpu_tier.das",     // gpu_tier_status_ / set_gpu_tier_want_ / moe_gpu_tier_arm_ / moe_gpu_weight_budget_
    "modules/dasLLAMA/dasllama/dasllama_gpu_resident.das"  // the slot ops + marks / hydrate / drop / prefill pin
]
let EXAMPLES = ["examples/dasLLAMA/run.das", "examples/dasLLAMA/chat.das"]

def private read_lines(path : string) : array<string> {
    let text = fread("{get_das_root()}/{path}")
    if (empty(text)) {
        panic("test_facade_docs: can't read {path}")
    }
    // Windows checkouts are CRLF (no *.das eol rule in .gitattributes); the parsing here is line-exact
    return <- split(replace(text, "\r\n", "\n"), "\n")
}

// "def name(" / "def name_(" at column 0 => "name"/"name_"; "" for anything else (incl. private)
def private toplevel_def_name(line : string) : string {
    if (!starts_with(line, "def ")) {
        return ""
    }
    let rest = slice(line, 4, length(line))
    let paren = find(rest, "(")
    if (paren <= 0) {
        return ""
    }
    let name = slice(rest, 0, paren)
    return find(name, " ") >= 0 ? "" : name   // "def private x(", "def operator ..." etc. drop out
}

[test(description = "Every public def in the facade `dasllama_core.das` opens its body with a //! docstring, and no //! line sits directly above a def where the doc extractor cannot see it")]
def test_facade_docstrings_inside_body(t : T?) {
    t |> run("every facade def opens with a body-leading //! docstring") @(t : T?) {
        let lines <- read_lines(FACADE)
        let n = length(lines)
        var checked = 0
        for (i in range(n)) {
            let name = toplevel_def_name(lines[i])
            continue if (empty(name))
            // the body opens at the first line of this def ending in "{" (signatures may wrap)
            var open = i
            while (open < n && !ends_with(lines[open], "\{")) {
                open++
            }
            let has_doc = open + 1 < n && starts_with(strip(lines[open + 1]), "//!")
            t |> success(has_doc, "facade `{name}` has a //! docstring as the first body line")
            checked++
        }
        t |> success(checked > 0, "facade defs found ({checked})")
    }
    t |> run("no inert above-def //! blocks in the facade") @(t : T?) {
        let lines <- read_lines(FACADE)
        for (i in range(length(lines) - 1)) {
            let inert = starts_with(strip(lines[i]), "//!") && starts_with(lines[i + 1], "def ")
            t |> success(!inert, "line {i + 1}: //! directly above a def is invisible to the doc extractor - move it inside the body")
        }
    }
}

[test(description = "The facade's public defs and the listed engine files' trailing-underscore defs map one to one: each facade name wraps an engine name_ and each engine name_ has a facade stub")]
def test_facade_engine_names_one_to_one(t : T?) {
    t |> run("facade stubs <-> engine underscore spellings are 1:1") @(t : T?) {
        var facade : table<string>
        for (line in read_lines(FACADE)) {
            let name = toplevel_def_name(line)
            if (!empty(name)) {
                facade |> insert(name)
            }
        }
        var engine : table<string>
        for (path in ENGINE_FILES) {
            for (line in read_lines(path)) {
                let name = toplevel_def_name(line)
                if (!empty(name) && ends_with(name, "_")) {
                    engine |> insert(slice(name, 0, length(name) - 1))
                }
            }
        }
        for (name in keys(facade)) {
            t |> success(key_exists(engine, name), "facade `{name}` wraps an engine `{name}_`")
        }
        for (name in keys(engine)) {
            t |> success(key_exists(facade, name), "engine `{name}_` has a documented facade `{name}`")
        }
    }
}

[test(description = "The run.das and chat.das examples each carry exactly one dasllama require, and it is the facade `dasllama/dasllama_core`")]
def test_examples_are_facade_only(t : T?) {
    t |> run("run.das / chat.das require only the facade from dasLLAMA") @(t : T?) {
        for (path in EXAMPLES) {
            var inscope dasllama_requires : array<string>
            for (line in read_lines(path)) {
                if (starts_with(line, "require dasllama/")) {
                    let rest = slice(line, length("require "), length(line))
                    let sp = find(rest, " ")
                    dasllama_requires |> push(sp < 0 ? rest : slice(rest, 0, sp))
                }
            }
            t |> equal(length(dasllama_requires), 1)
            t |> equal(dasllama_requires[0], FACADE_REQUIRE)
        }
    }
}

// dasllama/dasllama_core is the engine without the file decoders: no module it depends on is
// `audio` or `stbimage`, while dasllama/dasllama, core plus the decoders, depends on both.

def private collect_dependencies(mod : Module?; var seen : table<string; bool>) {
    module_for_each_dependency(mod) $(dep, _public) {
        let name = string(dep.name)
        if (!key_exists(seen, name)) {
            seen[name] = true
            collect_dependencies(dep, seen)
        }
    }
}

def private dependencies(name : string) : table<string; bool> {
    var seen : table<string; bool>
    let mod = get_module(name)
    if (mod != null) {
        collect_dependencies(mod, seen)
    }
    return <- seen
}

[test(description = "The transitive module dependencies of `dasllama_core` reach the engine but include neither `audio` nor `stbimage`, while the `dasllama` facade's include both decoders")]
def test_dasllama_core_requires_no_decoder(t : T?) {
    let core <- dependencies("dasllama_core")
    t |> success(key_exists(core, "dasllama_tts"), "the walk reaches the engine")
    t |> success(!key_exists(core, "audio") && !key_exists(core, "stbimage"), "dasllama_core needs no decoder module")
    let facade <- dependencies("dasllama")
    t |> success(key_exists(facade, "audio") && key_exists(facade, "stbimage"), "dasllama/dasllama brings both decoders")
}

// The library example streams PCM: with no model open its stream and memory calls refuse
// cleanly and say why, instead of panicking across the C ABI.

[test(description = "With no model open, the library example's `asr_feed`, `asr_listen` and `tts_speak` refuse with empty or zero results and a reason naming `asr_listen`, synthesizing nothing and never panicking")]
def test_stream_calls_refuse_without_a_model(t : T?) {
    var pcm = fixed_array(0.0, 0.0, 0.0, 0.0)
    t |> equal("", asr_feed(unsafe(addr(pcm[0])), 4, 16000), "asr_feed before asr_listen returns no text")
    t |> success(find(reason(), "asr_listen") >= 0, "and says to call asr_listen: {reason()}")
    t |> success(!asr_listen("no_such_vad.bin"), "asr_listen refuses with no ASR model open")
    t |> equal(0, tts_speak("hello"), "tts_speak refuses with no TTS model open")
    t |> success(tts_pcm() == null && tts_pcm_rate() == 0, "nothing was synthesized")
}

// Drift detector for dasLLAMA program roots (tutorials, examples, server tools). Two invariants
// this tree has broken before and repaired file by file:
//  1. a program root that requires dasllama/ and exports main - or, for a library root, carries
//     [export_c] entry points instead - declares `options stack = 524288` — a required module
//     cannot raise the program's stack, and the 16K default overflows;
//  2. a program root that loads a model declares its prefill intent — allow_cpu_prefill() or
//     set_metal_mode() — or every Metal-capable build panics at the first real prompt.

let ROOT_DIRS = ["tutorials/dasLLAMA", "examples/dasLLAMA", "utils/dasllama-server"]
let STACK_LINE = "options stack = 524288"

//! every .das under a root dir, subdirectories included: a program root may sit in its own folder
//! (examples/dasLLAMA/storyteller/main.das)
def private das_files_of(dir_path : string) : array<string> {
    var files : array<string>
    fio::dir("{get_das_root()}/{dir_path}") $(name) {
        return if (name == "." || name == "..")
        let rel = "{dir_path}/{name}"
        if (name |> ends_with(".das")) {
            files |> push(rel)
        } elif (stat("{get_das_root()}/{rel}").is_dir) {
            var sub <- das_files_of(rel)
            files |> push_from(sub)
        }
    }
    return <- files
}

// the scan reads CODE, so a tail comment can never stand in for a declaration
def private strip_comment(line : string) : string {
    let at = find(line, "//")
    return at < 0 ? line : slice(line, 0, at)
}

[test(description = "Every dasLLAMA program root under tutorials, examples and the server declares `options stack = 524288`, every model-loading root declares prefill intent in code, and more than ten roots are scanned so the census is not vacuous")]
def test_program_roots(t : T?) {
    t |> run("dasllama program roots declare the stack and their prefill intent") @(t : T?) {
        var checked = 0
        for (d in ROOT_DIRS) {
            for (path in das_files_of(d)) {
                var inscope lines <- read_lines(path)
                var requires_dasllama = false
                var has_main = false
                var has_export_c = false
                var has_stack = false
                var loads_model = false
                var has_intent = false
                for (line in lines) {
                    requires_dasllama ||= line |> starts_with("require dasllama/")
                    has_main ||= (line |> starts_with("def main")) || (line |> starts_with("def main("))
                    has_export_c ||= strip_comment(line) |> starts_with("[export_c")
                    has_stack ||= line |> starts_with(STACK_LINE)
                    // the loaders whose model prefills (LLM, ASR decoder, audio tower); a TTS
                    // carrier runs no prefill, so a TTS-only root owes no intent
                    loads_model ||= find(line, "load_model(") >= 0 || find(line, "load_asr_model(") >= 0 || find(line, "load_audio_tower(") >= 0
                    // CODE only: roots' allow_cpu_prefill() tail comments NAME set_metal_mode, so a
                    // raw-line scan would let a comment satisfy the check
                    let code = strip_comment(line)
                    // MetalMode.off leaves the CPU-prefill guard armed, so only a literal GPU mode counts as intent
                    let gpu_pin = (find(code, "set_metal_mode(") >= 0
                        && (find(code, "MetalMode.when_available") >= 0 || find(code, "MetalMode.required") >= 0))
                    // the server folder's shared pick (serving_knobs.das) lands the mode for both
                    // programs; a root that calls it has declared its intent through the one seam
                    has_intent ||= find(code, "allow_cpu_prefill(") >= 0 || gpu_pin || find(code, "apply_gpu_backend(") >= 0
                }
                continue if (!requires_dasllama || !(has_main || has_export_c))
                checked++
                t |> success(has_stack, "{path}: a dasllama program root declares `{STACK_LINE}`")
                if (loads_model) {
                    t |> success(has_intent, "{path}: a model-loading root declares prefill intent (allow_cpu_prefill(), or set_metal_mode with a literal GPU mode)")
                }
            }
        }
        t |> success(checked > 10, "the scan found the program roots ({checked} checked - a broken glob would pass vacuously)")
    }
}

// Two checks of modules/dasLLAMA/REVIEW.das over fixture files written under the test's temp dir - the HOW_TO check and
// the unlocked-queue-call check: one file a defect, each red on its own finding, and a clean one green. The gate runs as a
// child: REVIEW.das is a program root whose checks are private to it, and its `-- --howto <doc>...` and
// `-- --queue-calls <file>...` rails read only the named files.

struct private GateRun {
    rc : int
    output : string
}

def private run_gate(rail, file : string) : GateRun {
    let args <- get_command_line_arguments()
    let exe = empty(args) ? "" : args[0]
    var output = ""
    let rc = run_and_capture([exe, "-dasroot", get_das_root(), "{get_das_root()}/modules/dasLLAMA/REVIEW.das", "--", rail, file], output)
    return GateRun(rc = rc, output = output)
}

def private fixture(name, text : string) : string {
    let path = path_join(test_temp_dir(), name)
    fwrite(path, text)
    return path
}

let private CLEAN = "# Fixture\n\nThe preamble names no step.\n\n## 1. Read the charter\n\nOpen `ARCHITECTURE_MEDIA_ASR.md#gigaam-planes` and follow it.\n\n## Traps\n\nA trap section carries no number and needs no anchor.\n"

def private red_on(t : T?; name, text, fragment : string; rail : string = "--howto") {
    let run = run_gate(rail, fixture(name, text))
    t |> equal(run.rc, 1, "{name}: the gate's exit code\n{run.output}")
    t |> success(find(run.output, fragment) >= 0, "{name}: the finding names '{fragment}'\n{run.output}")
    t |> success(find(run.output, "1 finding(s)") >= 0, "{name}: the one defect is the one finding\n{run.output}")
}

[test(description = "The REVIEW.das HOW_TO gate passes a clean fixture and fails, with exactly one named finding each, a past-tense marker, a 20YY-MM date, a #NNN number, and a numbered step citing no architecture anchor", timeout = 1200)]
def test_howto_gate(t : T?) {
    t |> run("a clean HOW_TO passes") @(t : T?) {
        let run = run_gate("--howto", fixture("clean.md", CLEAN))
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a past-tense marker is a finding") @(t : T?) {
        red_on(t, "past.md", "{CLEAN}\nThe loader was slow.\n", "past-tense marker 'was'")
    }
    t |> run("a 20YY-MM date is a finding") @(t : T?) {
        red_on(t, "date.md", "{CLEAN}\nMeasured in 2026-10.\n", "a date - a HOW_TO is timeless")
    }
    t |> run("a #NNN number is a finding") @(t : T?) {
        red_on(t, "pr.md", "{CLEAN}\nSee #4370 for the reason.\n", "a PR or issue number")
    }
    t |> run("a numbered step that cites no architecture anchor is a finding") @(t : T?) {
        red_on(t, "uncited.md", "{CLEAN}\n## 2. Run the test\n\nRun it through the runner.\n", "a numbered step that cites no")
    }
}

//! the call's open paren, spliced into the fixtures' banned spellings so the folder walk over this file never reads a call
let private OPEN = "("

let private QUEUE_CLEAN = "def readback(raw : VkCommandBuffer) \{\n    cmd_one_shot() $(r; var _h) \{\n        copy_rows(r)\n    \}\n\}\n// run_cmd_sync{OPEN}device, pool, queue) in a comment is not a call\n"
[test(description = "The REVIEW.das queue-call gate passes a source using the locked one-shot (a commented call ignored) and fails with one finding each on run_cmd_sync, vkQueueSubmit, vkQueueWaitIdle and vkDeviceWaitIdle", timeout = 1200)]
def test_locked_queue_gate(t : T?) {
    t |> run("a source that submits through the locked one-shot passes") @(t : T?) {
        let run = run_gate("--queue-calls", fixture("locked.das", QUEUE_CLEAN))
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("run_cmd_sync is a finding") @(t : T?) {
        red_on(t, "sync.das", "{QUEUE_CLEAN}run_cmd_sync{OPEN}g_gpu.device, g_gpu.pool, g_gpu.queue) $(cmd) \{\n\}\n", "without the shared device's queue lock", "--queue-calls")
    }
    t |> run("a raw vkQueueSubmit is a finding") @(t : T?) {
        red_on(t, "submit.das", "{QUEUE_CLEAN}vk_check(vkQueueSubmit{OPEN}q, 1u, si, fence), null)\n", "without the shared device's queue lock", "--queue-calls")
    }
    t |> run("a raw vkQueueWaitIdle is a finding") @(t : T?) {
        red_on(t, "wait.das", "{QUEUE_CLEAN}vkQueueWaitIdle{OPEN}q)\n", "without the shared device's queue lock", "--queue-calls")
    }
    t |> run("a raw vkDeviceWaitIdle is a finding") @(t : T?) {
        red_on(t, "devwait.das", "{QUEUE_CLEAN}vkDeviceWaitIdle{OPEN}d)\n", "without the shared device's queue lock", "--queue-calls")
    }
}

//! the gate over several named files on one rail
def private run_gate_files(rail : string; files : array<string>) : GateRun {
    let args <- get_command_line_arguments()
    let exe = empty(args) ? "" : args[0]
    var output = ""
    var argv <- [exe, "-dasroot", get_das_root(), "{get_das_root()}/modules/dasLLAMA/REVIEW.das", "--", rail]
    argv |> push_from(files)
    let rc = run_and_capture(argv, output)
    delete argv
    return GateRun(rc = rc, output = output)
}

let private RR_KERNELS = "let KQ_N_RR = 2   // the row groups a lane group takes\n"
let private RR_STAMP = "class FxN4 : FxNCol \{\n    override RR = 2\n\}\n"

[test(description = "The REVIEW.das N-column row-group gate passes stamps whose `override RR` is the kernel file's KQ_N_RR and fails, with one named finding, a stamp of another RR and a stamp file with no `override RR`; a kernel file without KQ_N_RR is a finding of its own", timeout = 1200)]
def test_ncol_rr_gate(t : T?) {
    let kernels = fixture("rr_kernels.das", RR_KERNELS)
    t |> run("two stamp files at KQ_N_RR pass") @(t : T?) {
        let run = run_gate_files("--ncol-rr", [kernels, fixture("rr_a.das", RR_STAMP), fixture("rr_b.das", "{RR_STAMP}{RR_STAMP}")])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a stamp of another RR is a finding") @(t : T?) {
        let run = run_gate_files("--ncol-rr", [kernels, fixture("rr_off.das", "{RR_STAMP}class FxN8 : FxNCol \{\n    override RR = 4\n\}\n")])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "`override RR = 4` against KQ_N_RR = 2") >= 0, "the finding names the stamp's RR and the count\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one defect is the one finding\n{run.output}")
    }
    t |> run("a stamp file with no `override RR` is a finding") @(t : T?) {
        let run = run_gate_files("--ncol-rr", [kernels, fixture("rr_none.das", "class FxN4 : FxNCol \{\n\}\n")])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "no `override RR` stamp found") >= 0, "the finding says the rule walked nothing\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one defect is the one finding\n{run.output}")
    }
    t |> run("a kernel file without KQ_N_RR is a finding") @(t : T?) {
        let run = run_gate_files("--ncol-rr", [fixture("rr_nocount.das", "let KQ_N_ROWS = 2\n"), fixture("rr_c.das", RR_STAMP)])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "no `let KQ_N_RR` found") >= 0, "the finding says the count is gone\n{run.output}")
    }
}

//! the tests/ gate's load-once rail over the named fixture files alone, the findings of the files `unfolded` names held
//! (`-- --load-once <file>... --unfolded <name>...`)
def private run_load_once_gate(files : array<string>; unfolded : array<string> = array<string>()) : GateRun {
    var tail <- ["--load-once"]
    tail |> push_from(files)
    tail |> push("--unfolded")
    tail |> push_from(unfolded)
    return run_tests_gate(tail)
}

//! the tests/ gate with the command-line tail `tail` after its `--`: one of its fixture rails
def private run_tests_gate(tail : array<string>) : GateRun {
    let args <- get_command_line_arguments()
    let exe = empty(args) ? "" : args[0]
    var output = ""
    var argv <- [exe, "-dasroot", get_das_root(), "{get_das_root()}/modules/dasLLAMA/tests/REVIEW.das", "--"]
    argv |> push_from(tail)
    let rc = run_and_capture(argv, output)
    delete argv
    return GateRun(rc = rc, output = output)
}

//! `run_tests_gate` with the gate's info lines printed (`with_child_log_info`): its held lines
def private run_tests_gate_logged(tail : array<string>) : GateRun {
    var run = GateRun()
    with_child_log_info() {
        run = run_tests_gate(tail)
    }
    return run
}

//! a load-once fixture's block of `file`: the area's one load of it, the call's paren spliced so the folder walk reads no call
def private lo_block(file : string) : string => "def test_owner(t : T?) \{\n    with_cpu_and_gpu_model{OPEN}t, \"llm\", \"{file}\", 64l) $(m, form) \{\n    \}\n\}\n"

//! the gate's one finding on `files` names `want` at `at` (a `file:line`), each part a fragment of the same output
def private lo_red(t : T?; files : array<string>; at, want : string; count : int = 1) {
    let run = run_load_once_gate(files)
    t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
    t |> success(find(run.output, at) >= 0, "the finding names its site '{at}'\n{run.output}")
    t |> success(find(run.output, want) >= 0, "the finding names '{want}'\n{run.output}")
    t |> success(find(run.output, "{count} finding(s)") >= 0, "the defect is {count} finding(s)\n{run.output}")
}

let private LO_LOAD = "def test_raw(t : T?) \{\n    var m <- load_model_{OPEN}path_join(dir, \"fx-a.gguf\"), QuantMode.q8)\n\}\n"

[test(description = "The tests/ REVIEW.das load-once gate passes a block alone and a second load of its file whose function declares its own load with a reason, and fails, naming the site's file and line and the model file, a second loader of a blocked file, a load through a listed helper whose name a local or a top-level constant binds, two loads of a file no block loads, an own-load declaration with an empty reason, a function passing its parameter to a loader the list does not name, and a second block of one file; a block in a file whose findings are held owns its model file where no other block does, so a load in another file names it as its block, while a held file's second block of a file an unheld block owns is a held load site; it reads a name bound one local away or by a constant of another file, a load on a one-line function's def line, and no call whose name ends a longer name or follows a dot; a listed loader passing its parameter, or the block's home doing so, is no finding; and it fails a held file it finds clean, a block naming its area or file by a non-literal, an own-load reason that is not a literal, a listed loader no file defines, and an area whose runner lists no file - the area's files the runner's; a `with_model_once` call, and a `with_model_once_at` one, is a block like the chat models' - a second load of its file a finding, a second block of one area and file a finding and a block an area none, a held file's second block a held site its held line names by its own call, a file its form loader loads beside its own a load site of that file; two loads of a `.bin` pack are findings in an area that reads packs and none in the llm area; and a held file one area finds dirty is not a clean file of another", timeout = 1200)]
def test_load_once_gate(t : T?) {
    let owner = fixture("lo_owner.das", lo_block("fx-a.gguf"))
    t |> run("a block alone passes") @(t : T?) {
        let run = run_load_once_gate([owner])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a second loader of a blocked file is a finding at its line") @(t : T?) {
        lo_red(t, [owner, fixture("lo_raw.das", LO_LOAD)], "lo_raw.das:2:", "`load_model_` loads fx-a.gguf, its block is lo_owner.das:2")
    }
    t |> run("a second load whose function declares its own load with a reason passes") @(t : T?) {
        let own = fixture("lo_own.das", "def test_own(t : T?) \{\n    own_load{OPEN}\"the cell drops the model and loads it again\")\n    var m <- load_model_{OPEN}path_join(dir, \"fx-a.gguf\"), QuantMode.q8)\n\}\n")
        let run = run_load_once_gate([owner, own])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("an own-load declaration with an empty reason is a finding") @(t : T?) {
        let empty_why = fixture("lo_empty.das", "def test_own(t : T?) \{\n    own_load{OPEN}\"\")\n    var m <- load_model_{OPEN}path_join(dir, \"fx-a.gguf\"), QuantMode.q8)\n\}\n")
        lo_red(t, [owner, empty_why], "lo_empty.das:2:", "`own_load` names, as a non-empty string literal", 2)
    }
    t |> run("a name a local binds, passed through a listed helper, is read") @(t : T?) {
        let helper = fixture("lo_helper.das", "def test_helper(t : T?) \{\n    let path = path_join(dir, \"fx-a.gguf\")\n    var m <- load_capped{OPEN}path)\n\}\n")
        lo_red(t, [owner, helper], "lo_helper.das:3:", "`load_capped` loads fx-a.gguf")
    }
    t |> run("a name a top-level constant binds, passed through a listed helper, is read") @(t : T?) {
        let constant = fixture("lo_const.das", "let private FX = \"fx-a.gguf\"\n\ndef test_const(t : T?) \{\n    load_kq_native{OPEN}t, FX, true, false) $(tr; path) \{\n    \}\n\}\n")
        lo_red(t, [owner, constant], "lo_const.das:4:", "`load_kq_native` loads fx-a.gguf")
    }
    t |> run("two loads of a file no block loads are a finding each") @(t : T?) {
        let twice = fixture("lo_twice.das", "def test_one(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n\ndef test_two(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n")
        lo_red(t, [twice], "lo_twice.das:6:", "loads fx-b.gguf, loaded at 2 sites of the llm area and by no block", 2)
    }
    t |> run("a function passing its parameter to a loader the list does not name is a finding") @(t : T?) {
        let unlisted = fixture("lo_unlisted.das", "def private my_loader(path : string) : Model \{\n    return <- load_model_{OPEN}path, QuantMode.q8)\n\}\n")
        lo_red(t, [unlisted], "lo_unlisted.das:2:", "`my_loader` passes `path` to `load_model_`")
    }
    t |> run("a second block of one file is a finding") @(t : T?) {
        lo_red(t, [owner, fixture("lo_second.das", lo_block("fx-a.gguf"))], "lo_second.das:2:", "the first call is lo_owner.das:2")
    }
    t |> run("a block in a file UNFOLDED lists owns its file where no other block does: a load elsewhere names it") @(t : T?) {
        let d_twice = "\ndef test_one(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-d.gguf\"), QuantMode.q8)\n\}\n\ndef test_two(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-d.gguf\"), QuantMode.q8)\n\}\n"
        let held_owner = fixture("lo_held_owner.das", "{lo_block("fx-c.gguf")}{d_twice}")
        let raw = fixture("lo_raw_c.das", "def test_raw(t : T?) \{\n    var m <- load_model_{OPEN}path_join(dir, \"fx-c.gguf\"), QuantMode.q8)\n\}\n")
        let run = run_load_once_gate([held_owner, raw], ["lo_held_owner.das"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "lo_raw_c.das:2:") >= 0, "the finding names its site\n{run.output}")
        t |> success(find(run.output, "`load_model_` loads fx-c.gguf, its block is lo_held_owner.das:2") >= 0, "the held file's block owns the file\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one load is the one finding, the block itself none\n{run.output}")
        t |> success(find(run.output, "2 finding(s) held") >= 0, "the held file's two loads of a file no block loads are its held findings\n{run.output}")
    }
    lo_gate_reads(t, owner)
    lo_gate_lists(t, owner)
    lo_gate_once_blocks(t, owner)
}

//! a load-once fixture's `with_model_once` block of `file` in `area`, its form loader `fx_form`
def private lo_once_block(area, file : string) : string =>
    "def test_once(t : T?) \{\n    with_model_once{OPEN}t, \"{area}\", \"{file}\", forms, @@fx_form) $(var m; form) \{\n    \}\n\}\n"

//! a load-once fixture's two loads of the pack `file`, a function each
def private lo_pack_twice(file : string) : string =>
    "def test_one(t : T?) \{\n    var p <- load_g2p{OPEN}path_join(dir, \"{file}\"))\n\}\n\ndef test_two(t : T?) \{\n    var p <- load_g2p{OPEN}path_join(dir, \"{file}\"))\n\}\n"

//! the load-once gate's second block, `with_model_once`: its file's other loads, a second block, a file its form loader loads
//! beside its own, the packs an area reads, and the held list across areas
def private lo_gate_once_blocks(t : T?; owner : string) {
    let once = fixture("lo_once.das", lo_once_block("llm", "fx-m.gguf"))
    t |> run("a `with_model_once` block alone passes") @(t : T?) {
        let run = run_load_once_gate([once])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a second load of a file a `with_model_once` block loads is a finding naming the block") @(t : T?) {
        let raw = fixture("lo_raw_m.das", "def test_raw(t : T?) \{\n    var m <- load_model_{OPEN}path_join(dir, \"fx-m.gguf\"), QuantMode.q8)\n\}\n")
        lo_red(t, [once, raw], "lo_raw_m.das:2:", "`load_model_` loads fx-m.gguf, its block is lo_once.das:2")
    }
    t |> run("a `with_model_once` block of a file another block loads is a second block") @(t : T?) {
        lo_red(t, [owner, fixture("lo_once_a.das", lo_once_block("llm", "fx-a.gguf"))], "lo_once_a.das:2:", "the first call is lo_owner.das:2")
    }
    t |> run("a `with_model_once_at` call, its file under a root it names, is a block of that file") @(t : T?) {
        let at_root = fixture("lo_once_at.das", "def test_at(t : T?) \{\n    with_model_once_at{OPEN}t, \"llm\", \"fx-a.gguf\", forms, root, @@fx_form) $(var m; form) \{\n    \}\n\}\n")
        lo_red(t, [owner, at_root], "lo_once_at.das:2:", "the first call is lo_owner.das:2")
    }
    t |> run("a held file's `with_model_once` block of a file another block loads is a held load site named by its call") @(t : T?) {
        let held = fixture("lo_held_once.das", lo_once_block("llm", "fx-a.gguf"))
        let run = run_tests_gate_logged(["--load-once", held, owner, "--unfolded", "lo_held_once.das"])
        t |> equal(run.rc, 0, "the gate's exit code: the held block is held\n{run.output}")
        t |> success(find(run.output, "1 finding(s) held") >= 0, "the held block is the one held finding\n{run.output}")
        t |> success(find(run.output, "lo_held_once.das:2: `with_model_once` loads fx-a.gguf, its block is lo_owner.das:2") >= 0, "the held line names the block's own call\n{run.output}")
    }
    t |> run("one file's blocks in two areas are a block each") @(t : T?) {
        let run = run_load_once_gate([owner, fixture("lo_once_tts.das", lo_once_block("tts", "fx-a.gguf"))])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a file a block's form loader loads beside the block's own is a load site of that file") @(t : T?) {
        let loader = "def fx_form{OPEN}path, form : string) : FxPair \{\n    return <- FxPair{OPEN}pack <- load_g2p{OPEN}path), tagger <- load_pos_tagger{OPEN}path_join(dir, \"fx-tag.bin\")))\n\}\n\n"
        let pair = fixture("lo_pair.das", "{loader}{lo_once_block("tts", "fx-pack.bin")}")
        let alone = run_tests_gate(["--load-once", pair, "--loaders", "fx_form", "--area", "tts"])
        t |> equal(alone.rc, 0, "the pair's one load of each file passes\n{alone.output}")
        let tag = fixture("lo_tag.das", "def test_tag(t : T?) \{\n    var p <- load_pos_tagger{OPEN}path_join(dir, \"fx-tag.bin\"))\n\}\n")
        let run = run_tests_gate(["--load-once", pair, tag, "--loaders", "fx_form", "--area", "tts"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "loads fx-tag.bin, loaded at 2 sites of the tts area and by no block: lo_pair.das:2, lo_tag.das:2") >= 0, "the form loader's load and the second one are the two sites\n{run.output}")
        t |> success(find(run.output, "2 finding(s)") >= 0, "a finding a site\n{run.output}")
    }
    t |> run("two loads of a `.bin` pack are findings in an area that reads packs, and none in the llm area") @(t : T?) {
        let packs = fixture("lo_packs.das", lo_pack_twice("fx-pack.bin"))
        let tts = run_tests_gate(["--load-once", packs, "--area", "tts"])
        t |> equal(tts.rc, 1, "the gate's exit code in the tts area\n{tts.output}")
        t |> success(find(tts.output, "loads fx-pack.bin, loaded at 2 sites of the tts area and by no block") >= 0, "the findings name the pack\n{tts.output}")
        t |> success(find(tts.output, "2 finding(s)") >= 0, "a finding a site\n{tts.output}")
        let llm = run_load_once_gate([packs])
        t |> equal(llm.rc, 0, "the llm area reads a `.bin` as a checkpoint, not a model file\n{llm.output}")
    }
    t |> run("a held file one area finds dirty is not a clean file of the area that does not list it") @(t : T?) {
        let runner = fixture("lo_run_two.das", "def area_tests(area : string) : array<string> \{\n    if (area == \"llm\") \{\n        return <- [ \"test_lo_llm.das\" ]\n    \} elif (area == \"tts\") \{\n        return <- [ \"test_lo_tts.das\" ]\n    \}\n    var none : array<string>\n    return <- none\n\}\n")
        let dirty = fixture("test_lo_llm.das", "def test_one(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n\ndef test_two(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n")
        let clean = fixture("test_lo_tts.das", lo_once_block("tts", "fx-v.gguf"))
        let run = run_tests_gate(["--load-once", dirty, clean, "--unfolded", "test_lo_llm.das", "--area-run", runner, "--area", "llm", "tts"])
        t |> equal(run.rc, 0, "the gate's exit code: the held file's findings are held, and the tts pass does not call it clean\n{run.output}")
        t |> success(find(run.output, "2 finding(s) held") >= 0, "the llm area's two loads are the held findings\n{run.output}")
        let stale = run_tests_gate(["--load-once", dirty, clean, "--unfolded", "test_lo_tts.das", "--area-run", runner, "--area", "llm", "tts"])
        t |> equal(stale.rc, 1, "the control: a held file no area finds dirty is a finding\n{stale.output}")
        t |> success(find(stale.output, "UNFOLDED lists test_lo_tts.das, whose loads the check finds folded") >= 0, "the finding names the clean file\n{stale.output}")
    }
}

//! the load-once gate's lists on its rail: the loaders it reads (`--loaders`), the area's files off a runner
//! (`--area-run`), the block's home, and a one-line function's load
def private lo_gate_lists(t : T?; owner : string) {
    t |> run("a listed loader no file defines is a finding") @(t : T?) {
        let defs = fixture("lo_defs.das", "def fx_loader{OPEN}path : string) : Model \{\n    return <- load_model_{OPEN}path, QuantMode.q8)\n\}\n")
        let run = run_tests_gate(["--load-once", owner, defs, "--loaders", "fx_loader", "fx_stale"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "LOADERS lists `fx_stale`, which no file the load-once check reads defines") >= 0, "the finding names the stale loader\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the defined loader is no finding\n{run.output}")
    }
    let runner = "def area_tests(area : string) : array<string> \{\n    if (area == \"llm\") \{\n        return <- [ \"test_lo_area.das\" ]\n    \}\n    var none : array<string>\n    return <- none\n\}\n"
    t |> run("an area whose runner lists no file there is a finding") @(t : T?) {
        let run = run_tests_gate(["--load-once", owner, "--area-run", fixture("lo_run_none.das", runner)])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "the load-once check read no test file of the area llm off `area_tests`") >= 0, "the finding names the empty area\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the empty area is the one finding\n{run.output}")
    }
    t |> run("the area's files are the ones its runner lists: a load in a named file it does not list is not read") @(t : T?) {
        let listed = fixture("test_lo_area.das", LO_LOAD)
        let off_area = fixture("lo_off_area.das", LO_LOAD)
        let run = run_tests_gate(["--load-once", owner, listed, off_area, "--area-run", fixture("lo_run.das", runner)])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "test_lo_area.das:2:") >= 0 && find(run.output, "lo_off_area.das") < 0, "the listed file's load is the finding\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one listed load is the one finding\n{run.output}")
    }
    t |> run("the block's home passes its parameter to a loader with no listing") @(t : T?) {
        let home = fixture("_cpu_gpu_model.das", "def private fx_home_load{OPEN}path : string) : Model \{\n    return <- load_model_{OPEN}path, QuantMode.q8)\n\}\n")
        let run = run_load_once_gate([owner, home])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a one-line function's load on its def line is a load site") @(t : T?) {
        let one_line = fixture("lo_one_line.das", "def private fx_load() : Model => <- load_model_{OPEN}path_join(dir, \"fx-a.gguf\"), QuantMode.q8)\n")
        lo_red(t, [owner, one_line], "lo_one_line.das:1:", "`load_model_` loads fx-a.gguf, its block is lo_owner.das:2")
    }
}

//! the load-once gate's reading of a source: the held list, the block's literals, the own-load reason, the held file's second
//! block, a bound name one hop away or in another file, a listed loader's own body, the call-name guards
def private lo_gate_reads(t : T?; owner : string) {
    t |> run("a held file the check finds clean is a finding: drop it from the list") @(t : T?) {
        let run = run_load_once_gate([owner], ["lo_owner.das"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "UNFOLDED lists lo_owner.das, whose loads the check finds folded: drop it from the list") >= 0, "the finding names the file\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the listed file is the one finding\n{run.output}")
    }
    t |> run("a block naming its area or its file by a non-literal is a finding each") @(t : T?) {
        let blocks = fixture("lo_nonlit.das", "def test_area(t : T?) \{\n    with_cpu_and_gpu_model{OPEN}t, AREA, \"fx-a.gguf\", 64l) $(m, form) \{\n    \}\n\}\n\ndef test_file(t : T?) \{\n    with_cpu_and_gpu_model{OPEN}t, \"llm\", FX, 64l) $(m, form) \{\n    \}\n\}\n")
        let run = run_load_once_gate([blocks])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "lo_nonlit.das:2:") >= 0 && find(run.output, "lo_nonlit.das:7:") >= 0, "the findings name both blocks\n{run.output}")
        t |> success(find(run.output, "takes its run area and its model file as string literals") >= 0, "the finding states the rule\n{run.output}")
        t |> success(find(run.output, "2 finding(s)") >= 0, "each block is one finding\n{run.output}")
    }
    for (why in fixed_array("why", "lead + \"drops the model\"")) {
        t |> run("an own-load reason that is not a string literal is a finding: {why}") @(t : T?) {
            let own = fixture("lo_why.das", "def test_own(t : T?) \{\n    own_load{OPEN}{why})\n    var m <- load_model_{OPEN}path_join(dir, \"fx-a.gguf\"), QuantMode.q8)\n\}\n")
            lo_red(t, [owner, own], "lo_why.das:2:", "`own_load` names, as a non-empty string literal", 2)
        }
    }
    t |> run("a held file's second block of a file an unheld file blocks is a held load site, the unheld block its owner") @(t : T?) {
        let held = fixture("lo_held_second.das", lo_block("fx-a.gguf"))
        let run = run_load_once_gate([held, owner], ["lo_held_second.das"])
        t |> equal(run.rc, 0, "the gate's exit code: the unheld block owns the file, the held one is held, the held file is not clean\n{run.output}")
        t |> success(find(run.output, "1 finding(s) held") >= 0, "the held block is the one held finding\n{run.output}")
    }
    t |> run("a name a local binds one hop away, through another local, is read") @(t : T?) {
        let hop = fixture("lo_hop.das", "def test_hop(t : T?) \{\n    let NAME = \"fx-a.gguf\"\n    let p = path_join(dir, NAME)\n    var m <- load_capped{OPEN}p)\n\}\n")
        lo_red(t, [owner, hop], "lo_hop.das:4:", "`load_capped` loads fx-a.gguf")
    }
    t |> run("a name a top-level constant of another file binds is read") @(t : T?) {
        let names = fixture("lo_names.das", "let FX_SHARED = \"fx-a.gguf\"\n")
        let user = fixture("lo_user.das", "def test_user(t : T?) \{\n    load_kq_native{OPEN}t, FX_SHARED, true, false) $(tr; path) \{\n    \}\n\}\n")
        lo_red(t, [owner, names, user], "lo_user.das:2:", "`load_kq_native` loads fx-a.gguf")
    }
    t |> run("a listed loader passing its own parameter is no finding, nor is its definition a load site") @(t : T?) {
        let listed = fixture("lo_listed.das", "def load_capped{OPEN}path : string) : Model \{\n    return <- load_model_{OPEN}path, QuantMode.q8)\n\}\n")
        let run = run_load_once_gate([owner, listed])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a call whose name ends a longer name or follows a `.` is no load site") @(t : T?) {
        let sites = "    var a <- xload_gguf{OPEN}path_join(dir, \"fx-b.gguf\"))\n    var b <- m.load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"))\n"
        let guards = fixture("lo_guards.das", "def test_one(t : T?) \{\n{sites}\}\n\ndef test_two(t : T?) \{\n{sites}\}\n")
        let run = run_load_once_gate([guards])
        t |> equal(run.rc, 0, "the gate's exit code: no two loads of fx-b.gguf\n{run.output}")
    }
}

//! a cell-pin fixture's clean cell: it reads the pin and the form's knob, and a commented pin is no call
let private PIN_CLEAN = "def fx_cell(t : T?; var m : Model; form : ModelForm) \{\n    // pin_kernel_backend{OPEN}\"portable\") on the form, not here\n    t |> equal(kernel_backend_pin{OPEN}), form.backend)\n    t |> success(form.kq_native != KqNative.kept || m.kquant_native)\n\}\n"

[test(description = "The tests/ REVIEW.das cell-pin gate passes a fixture cell module that reads the backend pin and its form's K-quant knob, a commented pin ignored and a call whose name only ends in a pin's or follows a dot no pin, and fails with one named finding each a cell that pins the backend, one that clears the pin and one that sets the K-quant native knob", timeout = 1200)]
def test_cell_pins_gate(t : T?) {
    t |> run("a cell that reads the pin and its form's knobs passes") @(t : T?) {
        let run = run_tests_gate(["--cell-pins", fixture("_fx_clean_cells.das", PIN_CLEAN)])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a call whose name ends a longer name or follows a `.` is no pin") @(t : T?) {
        let near = fixture("_fx_near_cells.das", "{PIN_CLEAN}\ndef fx_near(t : T?) \{\n    my_pin_kernel_backend{OPEN}\"portable\")\n    rig.pin_kernel_backend{OPEN}\"portable\")\n\}\n")
        let run = run_tests_gate(["--cell-pins", near])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    for (bad in fixed_array((name = "_fx_pin_cells.das", call = "pin_kernel_backend{OPEN}\"portable\")", callee = "pin_kernel_backend"),
            (name = "_fx_clear_cells.das", call = "clear_kernel_backend_pin{OPEN})", callee = "clear_kernel_backend_pin"),
            (name = "_fx_kq_cells.das", call = "set_kquant_native{OPEN}true)", callee = "set_kquant_native"))) {
        t |> run("a cell calling `{bad.callee}` is a finding at its line") @(t : T?) {
            let run = run_tests_gate(["--cell-pins", fixture(bad.name, "{PIN_CLEAN}\ndef fx_bad(t : T?) \{\n    {bad.call}\n\}\n")])
            t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
            t |> success(find(run.output, "{bad.name}:8:") >= 0, "the finding names its site\n{run.output}")
            t |> success(find(run.output, "never pins, clears or re-pins the kernel backend") >= 0 && find(run.output, "`{bad.callee}`") >= 0, "the finding states the rule and names the call\n{run.output}")
            t |> success(find(run.output, "1 finding(s)") >= 0, "the one call is the one finding\n{run.output}")
        }
    }
}
