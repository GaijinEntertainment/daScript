options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost

// Drift detector for dasLLAMA program roots (tutorials, examples, server tools). Two invariants
// this tree has broken before and repaired file by file:
//  1. a program root that requires dasllama/ and exports main declares `options stack = 524288` —
//     a required module cannot raise the program's stack, and the 16K default overflows;
//  2. a program root that loads a model declares its prefill intent — allow_cpu_prefill() or
//     set_metal_mode() — or every Metal-capable build panics at the first real prompt.

let ROOT_DIRS = ["tutorials/dasLLAMA", "examples/dasLLAMA", "utils/dasllama-server"]
let STACK_LINE = "options stack = 524288"

def private read_lines(path : string) : array<string> {
    let text = fread("{get_das_root()}/{path}")
    if (empty(text)) {
        panic("test_program_roots: can't read {path}")
    }
    // Windows checkouts are CRLF (no *.das eol rule in .gitattributes); the parsing here is line-exact
    return <- split(replace(text, "\r\n", "\n"), "\n")
}

def private das_files_of(dir_path : string) : array<string> {
    var files : array<string>
    fio::dir("{get_das_root()}/{dir_path}") $(name) {
        if (name |> ends_with(".das")) {
            files |> push("{dir_path}/{name}")
        }
    }
    return <- files
}

// the scan reads CODE, so a tail comment can never stand in for a declaration
def private strip_comment(line : string) : string {
    let at = find(line, "//")
    return at < 0 ? line : slice(line, 0, at)
}

[test]
def test_program_roots(t : T?) {
    t |> run("dasllama program roots declare the stack and their prefill intent") @(t : T?) {
        var checked = 0
        for (d in ROOT_DIRS) {
            for (path in das_files_of(d)) {
                let lines <- read_lines(path)
                var requires_dasllama = false
                var has_main = false
                var has_stack = false
                var loads_model = false
                var has_intent = false
                for (line in lines) {
                    requires_dasllama ||= line |> starts_with("require dasllama/")
                    has_main ||= (line |> starts_with("def main")) || (line |> starts_with("def main("))
                    has_stack ||= line |> starts_with(STACK_LINE)
                    loads_model ||= find(line, "load_model(") >= 0 || find(line, "load_asr_model(") >= 0 || find(line, "load_audio_tower(") >= 0
                    let code = strip_comment(line)
                    // MetalMode.off leaves the CPU-prefill guard armed, so only a literal GPU mode counts as intent
                    let gpu_pin = (find(code, "set_metal_mode(") >= 0
                        && (find(code, "MetalMode.when_available") >= 0 || find(code, "MetalMode.required") >= 0))
                    has_intent ||= find(code, "allow_cpu_prefill(") >= 0 || gpu_pin
                }
                continue if (!requires_dasllama || !has_main)
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
