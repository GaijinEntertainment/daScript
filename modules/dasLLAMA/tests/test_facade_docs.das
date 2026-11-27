options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost
require daslib/rtti
require daslib/command_line   // get_das_exe: the binary a gate child runs on
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

// Checks of modules/dasLLAMA/REVIEW.das over fixture files written under the test's temp dir: one file a defect, each red on its own finding,
// and a clean one green. The gate runs as a child: REVIEW.das is a program root whose checks are private to it, and each rail reads only the files it names.

struct private GateRun {
    rc : int
    output : string
}

//! the gate `gate` (a path under the das root) in a child on the arguments `tail` after its `--`
def private run_gate_at(gate : string; tail : array<string>) : GateRun {
    var output = ""
    var argv <- [get_das_exe(), "-dasroot", get_das_root(), "{get_das_root()}/{gate}", "--"]
    argv |> push_from(tail)
    let rc = run_and_capture(argv, output)
    delete argv
    return GateRun(rc = rc, output = output)
}

def private run_gate(rail, file : string) : GateRun => run_gate_at("modules/dasLLAMA/REVIEW.das", [rail, file])

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
    var inscope tail <- [rail]
    tail |> push_from(files)
    return run_gate_at("modules/dasLLAMA/REVIEW.das", tail)
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

let private PSO_CLEAN = "var g_fx_count : int\ndef fx_pick(p : MetalComputePipeline?) : MetalComputePipeline? => p\nvar private g_fx_pso_a : MetalComputePipeline?\nvar g_fx_pso_b, g_fx_pso_e : MetalComputePipeline?\ndef fx_shutdown \{\n    release_handles(fx_pick(g_fx_pso_a),\n        g_fx_pso_b)\n    metal_release(g_fx_pso_e)\n\}\n"

[test(description = "The REVIEW.das pipeline-release gate passes a source whose Metal pipeline globals are each named in a release call - one call across two lines after a nested call, one line declaring two - and fails with one finding naming a global whose only release sits in a comment, or the first of two a line declares", timeout = 1200)]
def test_pso_release_gate(t : T?) {
    t |> run("every pipeline global named in a release call passes") @(t : T?) {
        let run = run_gate("--pso-release", fixture("pso_clean.das", PSO_CLEAN))
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a pipeline global no release call names is a finding") @(t : T?) {
        red_on(t, "pso_leak.das", "{PSO_CLEAN}var private g_fx_pso_c : MetalComputePipeline?\n// release_handle(g_fx_pso_c) in a comment releases nothing\n",
            "Metal pipeline `g_fx_pso_c` is named in no release call", "--pso-release")
    }
    t |> run("the first of two pipeline globals a line declares, unreleased, is a finding") @(t : T?) {
        red_on(t, "pso_pair.das", "{PSO_CLEAN}var g_fx_pso_f, g_fx_pso_a2 : MetalComputePipeline?\ndef fx_drop \{\n    release_handle(g_fx_pso_a2)\n\}\n",
            "Metal pipeline `g_fx_pso_f` is named in no release call", "--pso-release")
    }
}

//! the tests/ gate's load-once rail over the named fixture files alone (`-- --load-once <file>...`)
def private run_load_once_gate(files : array<string>) : GateRun {
    var tail <- ["--load-once"]
    tail |> push_from(files)
    return run_tests_gate(tail)
}

//! the tests/ gate with the command-line tail `tail` after its `--`: one of its fixture rails
def private run_tests_gate(tail : array<string>) : GateRun => run_gate_at("modules/dasLLAMA/tests/REVIEW.das", tail)

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

[test(description = "The tests/ REVIEW.das load-once gate passes a block alone and a second load of its file whose function declares its own load with a reason, and fails, naming the site's file and line and the model file, a second loader of a blocked file, a load through a listed helper whose name a local or a top-level constant binds, two loads of a file no block loads, an own-load declaration with an empty reason, a function passing its parameter to a loader the list does not name, and a second block of one file - of two files' blocks the first file read owns it; a block in a file whose other loads are findings owns its model file, so a load in another file names it as its block, and no finding is ever held - the gate keeps no list of exempt files; it reads a name bound one local away or by a constant of another file, a load on a one-line function's def line, and no call whose name ends a longer name or follows a dot; a listed loader passing its parameter, or the block's home doing so, is no finding; and it fails a block naming its area or file by a non-literal, an own-load reason that is not a literal, a listed loader no file defines, and an area whose runner lists no file - the area's files the runner's, one area's loads its findings and another area's block none; a `with_model_once` call, and a `with_model_once_at` one, is a block like the chat models' - a second load of its file a finding, a second block of one area and file a finding whichever kind is read first and a block an area none, a file its form loader loads beside its own a load site of that file; two loads of a `.bin` pack are findings in an area that reads packs and none in the llm area; a `let` at column 0 inside a function binds no top-level name; a block passing a table literal's row that names another file is a finding; it reads a name through a local bound to a top-level or fixture constant, one field of a struct constant, each value of a loop over an array literal or a table, and a snapshot's repo, counts a fixture function's load at its caller, and fails a name it cannot read outside an own-load function; a `with_cpu_and_gpu_snapshot` call is the block of the snapshot its repo names, alone passing and a load of that snapshot elsewhere a finding naming it; a name four local hops past a load's own binding is read and one five hops past it is not (the hop cap)", timeout = 1200)]
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
        let constant = fixture("lo_const.das", "let private FX = \"fx-a.gguf\"\n\ndef test_const(t : T?) \{\n    var m <- load_capped{OPEN}FX)\n\}\n")
        lo_red(t, [owner, constant], "lo_const.das:4:", "`load_capped` loads fx-a.gguf")
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
    t |> run("a block in a file whose other loads are findings still owns its file: a load elsewhere names it, and no finding is held") @(t : T?) {
        let d_twice = "\ndef test_one(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-d.gguf\"), QuantMode.q8)\n\}\n\ndef test_two(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-d.gguf\"), QuantMode.q8)\n\}\n"
        let dirty_owner = fixture("lo_dirty_owner.das", "{lo_block("fx-c.gguf")}{d_twice}")
        let raw = fixture("lo_raw_c.das", "def test_raw(t : T?) \{\n    var m <- load_model_{OPEN}path_join(dir, \"fx-c.gguf\"), QuantMode.q8)\n\}\n")
        let run = run_load_once_gate([dirty_owner, raw])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "lo_raw_c.das:2:") >= 0, "the finding names its site\n{run.output}")
        t |> success(find(run.output, "`load_model_` loads fx-c.gguf, its block is lo_dirty_owner.das:2") >= 0, "the dirty file's block owns the file\n{run.output}")
        t |> success(find(run.output, "lo_dirty_owner.das:7:") >= 0 && find(run.output, "lo_dirty_owner.das:11:") >= 0, "the owner's two loads of a file no block loads are findings at their lines\n{run.output}")
        t |> success(find(run.output, "3 finding(s)") >= 0 && find(run.output, "held") < 0, "the three loads are three findings, none held\n{run.output}")
    }
    let snap_call = "with_cpu_and_gpu_snapshot{OPEN}t, \"llm\", \"org/fx-snapb\", 64l, forms)"
    let snap = fixture("lo_snap_block.das",
        "def test_snap(t : T?) \{\n    {snap_call} $(m, form) \{\n    \}\n\}\n")
    t |> run("a snapshot block alone passes") @(t : T?) {
        let run = run_load_once_gate([snap])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a second load of a snapshot a snapshot block loads is a finding") @(t : T?) {
        let load = "load_model_{OPEN}hf_snapshot_dir{OPEN}\"org/fx-snapb\"), QuantMode.q8)"
        let raw = fixture("lo_snap_raw.das", "def test_raw(t : T?) \{\n    var m <- {load}\n\}\n")
        let want = "`load_model_` loads org/fx-snapb, its block is lo_snap_block.das:2"
        lo_red(t, [snap, raw], "lo_snap_raw.das:2:", want)
    }
    lo_gate_reads(t, owner)
    lo_gate_lists(t, owner)
    lo_gate_once_blocks(t, owner)
    lo_gate_bindings(t, owner)
}

//! a load-once fixture's `with_model_once` block of `file` in `area`, its form loader `fx_form`
def private lo_once_block(area, file : string) : string =>
    "def test_once(t : T?) \{\n    with_model_once{OPEN}t, \"{area}\", \"{file}\", forms, @@fx_form) $(var m; form) \{\n    \}\n\}\n"

//! a fixture folder `name` of one test file, `body` under one `[test]` annotation: what the gate's `--floor` rail walks
def private floor_folder(name, body : string) : string {
    let dir = path_join(test_temp_dir(), name)
    mkdir(dir)
    fwrite(path_join(dir, "test_fx.das"), "[test{OPEN}description = \"a fixture's one test\")]\n{body}")
    return dir
}

[test(description = "The tests/ gate's file-shape rail lifts the three-tests floor from a file whose one test holds a load-once block and keeps it for a file that holds none")]
def test_block_file_floor(t : T?) {
    t |> run("a file whose one `[test]` holds a block passes the floor") @(t : T?) {
        let run = run_tests_gate(["--floor", floor_folder("floor_block", lo_once_block("llm", "fx-m.gguf"))])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a file whose one `[test]` holds no block is under the floor") @(t : T?) {
        let run = run_tests_gate(["--floor", floor_folder("floor_plain", "def test_once(t : T?) \{\n\}\n")])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "test_fx.das") >= 0, "the finding names the file\n{run.output}")
        t |> success(find(run.output, "a category file holds three or more") >= 0, "the finding is the floor's\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one file is the one finding\n{run.output}")
    }
}

//! a load-once fixture's two loads of the pack `file`, a function each
def private lo_pack_twice(file : string) : string =>
    "def test_one(t : T?) \{\n    var p <- load_g2p{OPEN}path_join(dir, \"{file}\"))\n\}\n\ndef test_two(t : T?) \{\n    var p <- load_g2p{OPEN}path_join(dir, \"{file}\"))\n\}\n"

//! the load-once gate's second block, `with_model_once`: its file's other loads, a second block, a file its form loader loads
//! beside its own, the packs an area reads, and each area's own files
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
    t |> run("a `with_model_once` block read first owns its file: the chat models' block of the file after it is the second block") @(t : T?) {
        let first = fixture("lo_first_once.das", lo_once_block("llm", "fx-a.gguf"))
        lo_red(t, [first, owner], "lo_owner.das:2:", "the first call is lo_first_once.das:2")
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
    t |> run("each area reads its runner's files: one area's two loads of a file are its findings, another area's block none") @(t : T?) {
        let runner = fixture("lo_run_two.das", "def area_tests(area : string) : array<string> \{\n    if (area == \"llm\") \{\n        return <- [ \"test_lo_llm.das\" ]\n    \} elif (area == \"tts\") \{\n        return <- [ \"test_lo_tts.das\" ]\n    \}\n    var none : array<string>\n    return <- none\n\}\n")
        let dirty = fixture("test_lo_llm.das", "def test_one(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n\ndef test_two(t : T?) \{\n    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n")
        let clean = fixture("test_lo_tts.das", lo_once_block("tts", "fx-v.gguf"))
        let run = run_tests_gate(["--load-once", dirty, clean, "--area-run", runner, "--area", "llm", "tts"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "loads fx-b.gguf, loaded at 2 sites of the llm area and by no block") >= 0, "the llm area's two loads are its findings\n{run.output}")
        t |> success(find(run.output, "2 finding(s)") >= 0 && find(run.output, "test_lo_tts.das") < 0, "the tts area's block is no finding\n{run.output}")
        let llm_only = run_tests_gate(["--load-once", dirty, clean, "--area-run", runner, "--area", "tts"])
        t |> equal(llm_only.rc, 0, "the control: the tts area alone does not read the llm area's file\n{llm_only.output}")
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

struct private LoCase {
    cell : string
    name : string
    text : string
    aux : string = ""
    aux_text : string = ""
    listed : string = ""
    at : string = ""
    want : string = ""
    count : int = 1
}

let private LO_PATH_LOAD = "    var m <- load_model_{OPEN}path, QuantMode.q8)\n"
let private LO_PARAM_CELL = ("def fx_param_cell(t : T?; fx : Fx) \{\n"
    + "    let path = path_join(dir, fx.file)\n{LO_PATH_LOAD}\}\n")
let private LO_HELD_CELL = ("def fx_cell(t : T?) \{\n"
    + "    var m <- load_model_{OPEN}path_join(dir, \"fx-a.gguf\"), QuantMode.q8)\n\}\n")
let private LO_PARAM_USER = ("let private FXP = Fx(file = \"fx-a.gguf\")\n\n"
    + "def test_param(t : T?) \{\n    fx_param_cell{OPEN}t, FXP)\n\}\n")
let private LO_FIELD_FN = "    let path = path_join(dir, FXS.file)\n{LO_PATH_LOAD}\}\n"
let private LO_UNREAD = "    var m <- load_model_{OPEN}make_path(t), QuantMode.q8)\n"
let private LO_BLOCKED = "its block is lo_owner.das:2"
let private LO_HOPS_TAIL = ("    let h3 = h4\n    let h2 = h3\n    let h1 = h2\n"
    + "    let path = path_join(dir, h1)\n{LO_PATH_LOAD}\}\n")
//! a load whose path reaches its literal four local hops past its
//! own binding
let private LO_HOPS = ("def test_hops(t : T?) \{\n    let h4 = \"fx-a.gguf\"\n"
    + LO_HOPS_TAIL)
//! the same load one hop past the cap (LOAD_HOPS)
let private LO_HOPS_PAST = ("def test_hops(t : T?) \{\n    let h5 = \"fx-a.gguf\"\n"
    + "    let h4 = h5\n{LO_HOPS_TAIL}")

let private LO_CASES = fixed_array(
    LoCase(cell = "a local bound to a top-level constant is read through the hop",
        name = "lo_top_hop.das",
        text = ("let private FX = \"fx-a.gguf\"\n\ndef test_top(t : T?) \{\n"
            + "    let path = path_join(dir, FX)\n{LO_PATH_LOAD}\}\n"),
        at = "lo_top_hop.das:5:", want = "`load_model_` loads fx-a.gguf, {LO_BLOCKED}"),
    LoCase(cell = "a struct constant's field reads that field's literal alone",
        name = "lo_field.das",
        text = ("let private FXS = Fx(file = \"fx-b.gguf\", drafter = \"fx-a.gguf\")\n\n"
            + "def test_one(t : T?) \{\n{LO_FIELD_FN}\ndef test_two(t : T?) \{\n{LO_FIELD_FN}"),
        at = "lo_field.das:5:", want = "loads fx-b.gguf, loaded at 2 sites", count = 2),
    LoCase(cell = "a local bound to a fixture's global is read through the hop",
        name = "lo_glob_user.das",
        text = "def test_glob(t : T?) \{\n    let path = path_join(dir, FX_G)\n{LO_PATH_LOAD}\}\n",
        aux = "_lo_glob.das", aux_text = "let FX_G = \"fx-a.gguf\"\n",
        at = "lo_glob_user.das:3:", want = "`load_model_` loads fx-a.gguf, {LO_BLOCKED}"),
    LoCase(cell = "a loop over an array literal loads each of its files",
        name = "lo_loop.das",
        text = ("def test_loop(t : T?) \{\n    for (name in [\"fx-a.gguf\",\n"
            + "                  \"fx-b.gguf\"]) \{\n"
            + "        var m <- load_gguf{OPEN}path_join(dir, name), QuantMode.q8)\n    \}\n\}\n\n"
            + "def test_b(t : T?) \{\n"
            + "    var m <- load_gguf{OPEN}path_join(dir, \"fx-b.gguf\"), QuantMode.q8)\n\}\n"),
        at = "lo_loop.das:4:", want = "`load_gguf` loads fx-b.gguf, loaded at 2 sites", count = 3),
    LoCase(cell = "a loop over a top-level table loads each row's file field",
        name = "lo_rows.das",
        text = ("let private FX_ROWS = fixed_array((file = \"fx-c.gguf\", bar = 0.1),\n"
            + "    (file = \"fx-a.gguf\", bar = 0.2))\n\ndef test_rows(t : T?) \{\n"
            + "    for (spec in FX_ROWS) \{\n        let path = path_join(dir, spec.file)\n"
            + "    {LO_PATH_LOAD}    \}\n\}\n"),
        at = "lo_rows.das:7:", want = "`load_model_` loads fx-a.gguf, {LO_BLOCKED}"),
    LoCase(cell = "an unreadable name in a function that declares its own load passes",
        name = "lo_own_unread.das",
        text = ("def test_own(t : T?) \{\n"
            + "    own_load{OPEN}\"asserts on the load itself: its refusal\")\n{LO_UNREAD}\}\n"),
        count = 0),
    LoCase(cell = "an unreadable name elsewhere is a finding",
        name = "lo_unread.das",
        text = "def test_unread(t : T?) \{\n{LO_UNREAD}\}\n",
        at = "lo_unread.das:2:",
        want = "`load_model_` names it through a value the check cannot read"),
    LoCase(cell = "a name four hops past the load's own binding is read: the hop cap",
        name = "lo_hops.das", text = LO_HOPS,
        at = "lo_hops.das:7:", want = "`load_model_` loads fx-a.gguf, {LO_BLOCKED}"),
    LoCase(cell = "a name five hops past it is past the cap: unread",
        name = "lo_hops_past.das", text = LO_HOPS_PAST,
        at = "lo_hops_past.das:8:",
        want = "`load_model_` names it through a value the check cannot read"),
    LoCase(cell = "a snapshot named by a literal and through a local is one model, its repo",
        name = "lo_snap.das",
        text = ("let private REPO = \"org/fx-snap\"\n\ndef test_a(t : T?) \{\n"
            + "    var m <- load_model_{OPEN}hf_snapshot_dir{OPEN}\"org/fx-snap\"), QuantMode.q8)\n"
            + "\}\n\ndef test_b(t : T?) \{\n"
            + "    let path = hf_snapshot_dir{OPEN}REPO)\n{LO_PATH_LOAD}\}\n"),
        at = "lo_snap.das:4:", want = "loads org/fx-snap, loaded at 2 sites", count = 2),
    LoCase(cell = "a fixture function's load counts at its caller",
        name = "lo_held_user.das",
        text = "def test_fix(t : T?) \{\n    fx_cell{OPEN}t)\n\}\n",
        aux = "_lo_cells.das", aux_text = LO_HELD_CELL,
        at = "lo_held_user.das:2:", want = "`fx_cell` loads fx-a.gguf, {LO_BLOCKED}"),
    LoCase(cell = "a fixture function's load no test file calls counts nowhere",
        name = "_lo_cells.das", text = LO_HELD_CELL, count = 0),
    LoCase(cell = "a fixture passing its parameter through a local is an unlisted loader",
        name = "lo_param_user.das", text = LO_PARAM_USER,
        aux = "_lo_param.das", aux_text = LO_PARAM_CELL,
        at = "_lo_param.das:3:", want = "`fx_param_cell` passes `fx` to `load_model_`"),
    LoCase(cell = "a listed fixture loader loads its caller's file",
        name = "lo_param_user.das", text = LO_PARAM_USER,
        aux = "_lo_param.das", aux_text = LO_PARAM_CELL, listed = "fx_param_cell",
        at = "lo_param_user.das:4:", want = "`fx_param_cell` loads fx-a.gguf, {LO_BLOCKED}"),
    LoCase(cell = "a function declaring its own load passes its parameter with no listing",
        name = "lo_own_param.das",
        text = ("def private own_param(name : string) : string \{\n"
            + "    own_load{OPEN}\"asserts on the load itself - its refusal\")\n"
            + "    let path = path_join(scratch, \"\{name\}.gguf\")\n{LO_PATH_LOAD}\}\n"),
        count = 0))

def private lo_gate_bindings(t : T?; owner : string) {
    for (c in LO_CASES) {
        t |> run(c.cell) @(t : T?) {
            var inscope files <- [owner]
            if (!empty(c.aux)) {
                files |> push(fixture(c.aux, c.aux_text))
            }
            files |> push(fixture(c.name, c.text))
            if (!empty(c.listed)) {
                files |> push_from(["--loaders", c.listed])
            }
            if (c.count == 0) {
                let run = run_load_once_gate(files)
                t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
            } else {
                lo_red(t, files, c.at, c.want, c.count)
            }
        }
    }
}

//! the load-once gate's reading of a source: a clean file alone, the block's literals, the own-load reason, the order two
//! files' blocks of one file take, a bound name one hop away or in another file, a listed loader's own body, the call-name guards
def private lo_gate_reads(t : T?; owner : string) {
    t |> run("the gate holds no list of files exempt from the check: `--unfolded` names no file it reads") @(t : T?) {
        let raw = fixture("lo_raw_list.das", LO_LOAD)
        let run = run_tests_gate(["--load-once", owner, raw, "--unfolded", "lo_raw_list.das"])
        t |> equal(run.rc, 1, "the gate's exit code: the named file's load is a finding\n{run.output}")
        t |> success(find(run.output, "lo_raw_list.das:2:") >= 0, "the finding names the load's site\n{run.output}")
        t |> success(find(run.output, "held") < 0, "no finding is held\n{run.output}")
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
    t |> run("of two files' blocks of one file the first file read owns it, the other's block the finding") @(t : T?) {
        let first = fixture("lo_first.das", lo_block("fx-a.gguf"))
        lo_red(t, [first, owner], "lo_owner.das:2:", "the first call is lo_first.das:2")
    }
    t |> run("a name a local binds one hop away, through another local, is read") @(t : T?) {
        let hop = fixture("lo_hop.das", "def test_hop(t : T?) \{\n    let NAME = \"fx-a.gguf\"\n    let p = path_join(dir, NAME)\n    var m <- load_capped{OPEN}p)\n\}\n")
        lo_red(t, [owner, hop], "lo_hop.das:4:", "`load_capped` loads fx-a.gguf")
    }
    t |> run("a name a top-level constant of another file binds is read") @(t : T?) {
        let names = fixture("lo_names.das", "let FX_SHARED = \"fx-a.gguf\"\n")
        let user = fixture("lo_user.das", "def test_user(t : T?) \{\n    var m <- load_capped{OPEN}FX_SHARED)\n\}\n")
        lo_red(t, [owner, names, user], "lo_user.das:2:", "`load_capped` loads fx-a.gguf")
    }
    t |> run("a block passing another file's table row is a finding; its own file's row passes") @(t : T?) {
        let rows = "let private ROWS <- [Fx(code = \"de\"), Fx(code = \"es\")]\n\n"
        let row_block = ("def test_rows(t : T?) \{\n"
            + "    with_model_once{OPEN}t, \"llm\", \"fx-de.gguf\", forms, @@fx_form)"
            + " $(var m; form) \{\n"
            + "        cells(t, m, ROWS[IX])\n    \}\n\}\n")
        let own_text = "{rows}{replace(row_block, "IX", "0")}"
        let own = run_load_once_gate([fixture("lo_row_own.das", own_text)])
        t |> equal(own.rc, 0, "the gate's exit code, the block's own row\n{own.output}")
        let other_text = "{rows}{replace(row_block, "IX", "1")}"
        lo_red(t, [fixture("lo_row_other.das", other_text)], "lo_row_other.das:4:",
            "the block of fx-de.gguf passes `ROWS[1]` (es)")
    }
    t |> run("a `let` at column 0 inside a function binds no top-level name, in its own file or another") @(t : T?) {
        let body = "def test_col0(t : T?) \{\nlet path = \"fx-a.gguf\"\n    t |> success(true)\n\}\n"
        let loader = "def load_capped{OPEN}path : string) : Model \{\n    return <- load_model_{OPEN}path, QuantMode.q8)\n\}\n"
        let own_file = run_load_once_gate([owner, fixture("lo_col0.das", "{body}\n{loader}")])
        t |> equal(own_file.rc, 0, "the gate's exit code, one file\n{own_file.output}")
        let other = run_load_once_gate([owner, fixture("lo_col0_body.das", body), fixture("lo_col0_loader.das", loader)])
        t |> equal(other.rc, 0, "the gate's exit code, two files\n{other.output}")
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

//! a family-tag fixture's function `fn` holding the block of `file`: `lead` its statements ahead of the block, `tail` after
def private fam_block(fn, file : string; lead : string = ""; tail : string = "") : string =>
    ("def {fn}(t : T?) \{\n{lead}    with_model_once{OPEN}t, \"tts\", \"{file}\", [\"image\"], @@fx_form) "
        + "$(var m; form) \{\n    \}\n{tail}\}\n")

let private FAM_TAG = "    return if (!family_on(t, \"fx\"))\n"   //! a fixture function's family tag

[test(description = "The tests/ REVIEW.das family-tag gate passes a block whose file a family row holds, a block behind a `family_on` call in its own function and a block whose function's every caller calls `family_on` first, and an untagged block of a fixture the gate lists as no family's (stories15M.bin), and fails with one named finding each a block of a file of no row with no call, one whose only call follows the block, and one whose function an untagged function calls too", timeout = 1200)]
def test_family_tags_gate(t : T?) {
    let caller = "def test_caller(t : T?) \{\n{FAM_TAG}    fx_held(t)\n\}\n"
    t |> run("a row's file, a call ahead of the block and a tagged caller pass") @(t : T?) {
        let text = (fam_block("test_row", "kitten-nano.gguf") + fam_block("test_tagged", "fx-pack.bin", FAM_TAG)
            + fam_block("fx_held", "fx-held.bin") + caller)
        let run = run_tests_gate(["--family-tags", fixture("test_fx_family_ok.das", text)])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a fixture of no family (stories15M.bin) passes with no call, where the same block of fx-pack.bin does not") @(t : T?) {
        let run = run_tests_gate(["--family-tags", fixture("test_fx_family_free.das", fam_block("test_bare", "stories15M.bin"))])
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "carries no family") < 0, "no finding names the fixture\n{run.output}")
    }
    for (bad in fixed_array(
            (name = "test_fx_family_bare.das", file = "fx-pack.bin", text = fam_block("test_bare", "fx-pack.bin")),
            (name = "test_fx_family_late.das", file = "fx-pack.bin", text = fam_block("test_late", "fx-pack.bin", "", FAM_TAG)),
            (name = "test_fx_family_caller.das", file = "fx-held.bin",
                text = fam_block("fx_held", "fx-held.bin") + caller + "def test_other(t : T?) \{\n    fx_held(t)\n\}\n"))) {
        t |> run("{bad.name}: a block of a file of no row that no `family_on` call stands ahead of is a finding") @(t : T?) {
            let run = run_tests_gate(["--family-tags", fixture(bad.name, bad.text)])
            t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
            t |> success(find(run.output, "{bad.name}:2:") >= 0, "the finding names its site\n{run.output}")
            t |> success(find(run.output, "the block of {bad.file} carries no family") >= 0, "the finding names the file and the rule\n{run.output}")
            t |> success(find(run.output, "1 finding(s)") >= 0, "the one block is the one finding\n{run.output}")
        }
    }
}

//! a cell-pin fixture's clean cell: it reads the pin and the form's knob, and a commented pin is no call
let private PIN_CLEAN = "def fx_cell(t : T?; var m : Model; form : ModelForm) \{\n    // pin_kernel_backend{OPEN}\"portable\") on the form, not here\n    t |> equal(kernel_backend_pin{OPEN}), form.backend)\n    t |> success(form.kq_native != KqNative.kept || m.kquant_native)\n\}\n"

[test(description = "The tests/ REVIEW.das cell-pin gate passes a fixture cell module that reads the backend pin and its form's K-quant knob, a commented pin ignored and a call whose name only ends in a pin's or follows a dot no pin, and fails with one named finding each a cell that pins the backend, one that clears the pin and one that sets the K-quant native knob; its census-reset twin fails a direct engine census reset, Metal's or Vulkan's, and passes the census module's reset and `_kernel_census.das` itself", timeout = 1200)]
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
    t |> run("a direct census reset is a finding at its line; the census module's reset and `_kernel_census.das` itself pass") @(t : T?) {
        let reset = "{PIN_CLEAN}\ndef fx_reset() \{\n    metal_kernel_coverage_reset{OPEN})\n\}\n"
        let bad = run_tests_gate(["--census-resets", fixture("_fx_reset_cells.das", reset)])
        t |> equal(bad.rc, 1, "the gate's exit code\n{bad.output}")
        t |> success(find(bad.output, "_fx_reset_cells.das:8:") >= 0, "the finding names its site\n{bad.output}")
        t |> success(find(bad.output, "a direct reset loses them") >= 0 && find(bad.output, "`metal_kernel_coverage_reset`") >= 0, "the finding states the rule and names the call\n{bad.output}")
        t |> success(find(bad.output, "1 finding(s)") >= 0, "the one call is the one finding\n{bad.output}")
        let vk = "{PIN_CLEAN}\ndef fx_reset() \{\n    vk_kernel_coverage_reset{OPEN})\n\}\n"
        let vk_bad = run_tests_gate(["--census-resets", fixture("_fx_vk_reset_cells.das", vk)])
        t |> equal(vk_bad.rc, 1, "the Vulkan reset: the gate's exit code\n{vk_bad.output}")
        t |> success(find(vk_bad.output, "_fx_vk_reset_cells.das:8:") >= 0
            && find(vk_bad.output, "`vk_kernel_coverage_reset`") >= 0,
            "the Vulkan reset is a finding naming it\n{vk_bad.output}")
        let folded = "{PIN_CLEAN}\ndef fx_reset() \{\n    kernel_census_reset_metal{OPEN})\n\}\n"
        let ok = run_tests_gate(["--census-resets", fixture("_fx_folded_cells.das", folded), fixture("_kernel_census.das", reset)])
        t |> equal(ok.rc, 0, "the gate's exit code\n{ok.output}")
    }
}

//! a spec table of five rows: a parity row the walk loads, one whose
//! parity is copied in, a row with no parity fixture, and a parity
//! row each skip table names
let private WALK_SPECS = ("def model_specs() : array<ModelSpec> \{\n    return <- [\n"
    + "        ModelSpec{OPEN}file = \"fx-w.gguf\", parity <- ev_fx{OPEN})),\n"
    + "        ModelSpec{OPEN}file = \"fx-e.gguf\", parity = ev_fx{OPEN})),\n"
    + "        ModelSpec{OPEN}file = \"fx-n.gguf\"),\n"
    + "        ModelSpec{OPEN}file = \"fx-o.gguf\", parity <- ev_fx{OPEN})),\n"
    + "        ModelSpec{OPEN}file = \"fx-x.gguf\", parity <- ev_fx{OPEN}))\n    ]\n\}\n")

//! the load-once rail over `files` with the spec walk reading the
//! table `specs`
def private walk_gate(files : array<string>; specs : string) : GateRun {
    var inscope tail <- ["--load-once"]
    tail |> push_from(files, ["--spec-table", specs])
    return run_tests_gate(tail)
}

[test(description = "The tests/ REVIEW.das load-once gate reads the parity spec walk's loads off the spec table and the skip tables: the walk alone passes, a block of a walked row's file is a finding naming the walk, a row with no parity fixture is not walked, a row whose parity is copied in (`parity =`) is, a row either skip table (PARITY_BLOCK_ROWS, PARITY_OWN_ROWS) names is not walked - the same block a finding without the tables -, and a spec table with no row leaves the walk's load unread; a listed engine loader no file defines is a finding and a defined one none; the folder run's stale-list check over the files' definitions names each listed helper and engine loader no file defines, a defined one - a private one too - none", timeout = 1200)]
def test_spec_walk_gate(t : T?) {
    let specs = fixture("lo_specs.das", WALK_SPECS)
    let walk = fixture("test_lo_walk.das", "def test_walk(t : T?) \{\n    parity_spec_arms{OPEN}t, spec, path)\n\}\n")
    let skips = fixture("_lo_parity.das", (
        "let PARITY_BLOCK_ROWS = fixed_array{OPEN}\"fx-x.gguf\")\n"
        + "let PARITY_OWN_ROWS = fixed_array{OPEN}\"fx-o.gguf\")\n"))
    t |> run("the walk alone passes") @(t : T?) {
        let run = walk_gate([walk, skips], specs)
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a block of a walked row's file is a finding at the walk's call") @(t : T?) {
        let run = walk_gate([walk, skips, fixture("lo_owner_w.das", lo_block("fx-w.gguf"))], specs)
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "test_lo_walk.das:2: `parity_spec_arms` loads fx-w.gguf, its block is lo_owner_w.das:2") >= 0, "the finding names the walk and the block\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one walked row is the one finding\n{run.output}")
    }
    t |> run("a row whose parity fixture is copied in (`parity =`) is walked") @(t : T?) {
        let run = walk_gate([walk, skips, fixture("lo_owner_e.das", lo_block("fx-e.gguf"))], specs)
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        let want = "`parity_spec_arms` loads fx-e.gguf, its block is lo_owner_e.das:2"
        t |> success(find(run.output, want) >= 0,
            "the finding names the walk and the block\n{run.output}")
    }
    t |> run("a row with no parity fixture is not walked") @(t : T?) {
        let run = walk_gate([walk, skips, fixture("lo_owner_n.das", lo_block("fx-n.gguf"))], specs)
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    for (row in fixed_array("x", "o")) {
        let cell = "a row a skip table names is not walked, without the tables walked: fx-{row}"
        t |> run(cell) @(t : T?) {
            let owner = fixture("lo_owner_{row}.das", lo_block("fx-{row}.gguf"))
            let skipped = walk_gate([walk, skips, owner], specs)
            t |> equal(skipped.rc, 0, "the gate's exit code\n{skipped.output}")
            let walked = walk_gate([walk, owner], specs)
            t |> equal(walked.rc, 1, "the control's exit code\n{walked.output}")
            let want = "`parity_spec_arms` loads fx-{row}.gguf, its block is lo_owner_{row}.das:2"
            t |> success(find(walked.output, want) >= 0,
                "the control: the row is walked\n{walked.output}")
        }
    }
    t |> run("a spec table with no row leaves the walk's load unread") @(t : T?) {
        let run = walk_gate([walk, skips], fixture("lo_specs_none.das", "def model_specs() \{\n\}\n"))
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "`parity_spec_arms` names it through a value the check cannot read") >= 0, "the walk's load is unread\n{run.output}")
    }
    t |> run("a listed engine loader no file defines is a finding, a defined one none") @(t : T?) {
        let defs = fixture("lo_engine.das", "def fx_engine_load{OPEN}path : string) : int => 0\n")
        let run = run_tests_gate(["--load-once", defs, "--engine", "fx_engine_load", "fx_engine_gone"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "ENGINE_LOADERS lists `fx_engine_gone`, which no file the load-once check reads defines") >= 0, "the finding names the stale name\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the defined name is no finding\n{run.output}")
    }
    t |> run("the folder run's stale-list check names each listed name no file defines") @(t : T?) {
        let defs = fixture("lo_defs.das", ("def fx_engine_load{OPEN}path : string) : int => 0\n\n"
            + "def private fx_helper_load{OPEN}path : string) : int => 0\n"))
        let run = run_tests_gate(["--stale-loaders", defs, "--loaders", "fx_helper_load",
            "fx_helper_gone", "--engine", "fx_engine_load", "fx_engine_gone"])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        let gone = fixed_array("fx_helper_gone", "fx_engine_gone")
        for (list, name in fixed_array("LOADERS", "ENGINE_LOADERS"), gone) {
            let want = "{list} lists `{name}`, which no file under tests/ or the engine defines"
            t |> success(find(run.output, want) >= 0, "the finding names {name}\n{run.output}")
        }
        t |> success(find(run.output, "2 finding(s)") >= 0,
            "the defined names, a private one too, are none\n{run.output}")
    }
}

//! a runner of one area, a model-free suite, a kernel suite, a device
//! kernel suite, a stocked suite and a census verdict, each naming
//! one file
let private PLACE_RUN = ("let AREAS <- [\"llm\"]\n\ndef area_tests(area : string) : array<string> \{\n"
    + "    if (area == \"llm\") \{\n        return <- [ \"test_pl_area.das\" ]\n    \}\n    var none : array<string>\n"
    + "    return <- none\n\}\n\ndef suite_files(name : string) : array<string> \{\n"
    + "    if (name == \"model-free\") \{\n        return <- [ \"modules/dasLLAMA/tests/test_pl_free.das\" ]\n"
    + "    \} elif (name == \"kernels\") \{\n        return <- [ \"modules/dasLLAMA/tests/test_pl_kern.das\" ]\n"
    + "    \} elif (name == \"gpu-kernels\") \{\n"
    + "        return <- [ \"modules/dasLLAMA/tests/test_pl_gpu.das\" ]\n"
    + "    \} elif (name == \"stocked\") \{\n        return <- [ \"modules/dasLLAMA/tests/test_pl_stocked.das\" ]\n"
    + "    \}\n    var none : array<string>\n    return <- none\n\}\n\n"
    + "let CENSUS_VERDICT = \"modules/dasLLAMA/tests/test_pl_verdict.das\"\n")

//! the placement rail over the runner `runner`: a placed fixture file
//! of each kind it names, then `more`
def private place_gate(runner : string; more : array<string>) : GateRun {
    var inscope tail <- ["--placed", runner]
    let kinds = fixed_array("area", "free", "kern", "gpu", "verdict")
    tail |> push_from([for (n in kinds); fixture("test_pl_{n}.das", "")], more)
    return run_tests_gate(tail)
}

[test(description = "The tests/ REVIEW.das placement gate passes a test file an area, a model-free, kernel or device kernel suite or the census verdict names, fails with one named finding a file only a stocked suite names, and fails a runner it reads no list off", timeout = 1200)]
def test_placement_gate(t : T?) {
    let runner = fixture("lo_place_run.das", PLACE_RUN)
    let placed = "a file an area, a model-free or (device) kernel suite or the verdict names passes"
    t |> run(placed) @(t : T?) {
        let run = place_gate(runner, array<string>())
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a file in no area that only a stocked suite names is a finding") @(t : T?) {
        let run = place_gate(runner, [fixture("test_pl_stocked.das", "")])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "test_pl_stocked.das: a test file under tests/ sits in an area") >= 0, "the finding names the file\n{run.output}")
        t |> success(find(run.output, "1 finding(s)") >= 0, "the one unplaced file is the one finding\n{run.output}")
    }
    t |> run("a runner with no run list is a finding") @(t : T?) {
        let run = run_tests_gate(["--placed", fixture("lo_place_none.das", "def main() \{\n\}\n"), fixture("test_pl_area.das", "")])
        t |> equal(run.rc, 1, "the gate's exit code\n{run.output}")
        t |> success(find(run.output, "the placement check read no run list off the runner") >= 0, "the finding names the empty runner\n{run.output}")
    }
}
