options gen2
options _dasllama_internal = true   // this file spawns fixtures; its own requires reach the audio_io leaf directly

require dastest/testing_boost public
require dasllama/dasllama_audio_io
require daslib/fio
require daslib/strings_boost
require strings

// Model-free contract cells for two facade-boundary pieces the escape-hatched suite cannot see
// from inside: the lint's ALLOWED set (a facade-only program with NO escape compiles), and
// load_audio_16k_mono's empty-on-failure contract that every caller must guard.

// a one-function program requiring `requires` (one per line), no escape line
def fixture_source(requires : array<string>) : string {
    return join(requires, "\n") + "\n\n[export]\ndef main() " + "\{\n    print(\"ok\\n\")\n\}\n"
}

def spawn_compile(requires : array<string>; tag : string) : tuple<rc : int; out : string> {
    let dir_r = create_temp_directory_result("dasllama_lint_{tag}")
    if (!(dir_r is value)) {
        return (rc = -1, out = "temp dir failed")
    }
    let dir = unsafe(dir_r.value)
    let path = path_join(dir, "prog.das")
    if (!fwrite(path, fixture_source(requires))) {
        return (rc = -1, out = "fwrite failed")
    }
    let argv <- get_command_line_arguments()
    let self = find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
    var out : string
    let rc = run_and_capture([self, "-compile-only", path], out, 120.0)
    rmdir_rec_result(dir)
    return (rc = rc, out = out)
}

[test]
def test_allowed_set_admits_the_entry_modules(t : T?) {
    t |> run("a facade-only program with no escape compiles") @(t : T?) {
        let r = spawn_compile(["require dasllama/dasllama"], "facade")
        t |> equal(0, r.rc, "facade require is allowed: {r.out}")
        t |> success(find(r.out, "DASLLAMA001") < 0, "no lint finding on the facade")
    }
    t |> run("the scheduler and exchange entries are allowed too") @(t : T?) {
        let r = spawn_compile(["require dasllama/dasllama", "require dasllama/dasllama_scheduler",
                               "require dasllama/dasllama_exchange_schema"], "entries")
        t |> equal(0, r.rc, "scheduler + exchange_schema allowed: {r.out}")
    }
    t |> run("an engine internal with no escape does not compile") @(t : T?) {
        let r = spawn_compile(["require dasllama/dasllama_gguf"], "internal")
        t |> success(r.rc != 0, "internal require refused")
        t |> success(find(r.out, "DASLLAMA001") >= 0, "the finding names the rule: {r.out}")
    }
}

[test]
def test_load_audio_16k_mono_contract(t : T?) {
    t |> run("a missing or unreadable file decodes to an EMPTY array, never a panic") @(t : T?) {
        let samples <- load_audio_16k_mono("this-file-does-not-exist.wav")
        t |> equal(0, length(samples), "empty on a missing file")
    }
}
