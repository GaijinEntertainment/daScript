options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost
require daslib/rtti
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
