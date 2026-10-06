options gen2
options _dasllama_internal = true   // this file spawns fixtures; its own requires reach the audio_io leaf directly

require dastest/testing_boost public
require dasllama/dasllama_audio_io
require dasllama/dasllama_gemma4a
require daslib/fio
require daslib/strings_boost
require strings
require _compares

// Model-free contract cells for two facade-boundary pieces the escape-hatched suite cannot see
// from inside: the lint's ALLOWED set (a facade-only program with NO escape compiles), and
// load_audio_16k_mono's empty-on-failure contract that every caller must guard.

// a one-function program requiring `requires` (one per line), no escape line
def fixture_source(requires : array<string>) : string =>
    join(requires, "\n") + "\n\n[export]\ndef main() " + "\{\n    print(\"ok\\n\")\n\}\n"

//! an engine-wide compile: the facade pulls every engine module, and the windows nightly runner compiles it in about two
//! minutes, so the cap sits well past that
let ENGINE_COMPILE_CAP_SEC = 480.0

def spawn_compile(requires : array<string>; tag : string) : tuple<rc : int; out : string> =>
    spawn_compile_only(fixture_source(requires), "lint_{tag}", ENGINE_COMPILE_CAP_SEC)

//! a module with a function global above its `[init, boot_restore]` function, and one below it where `below` is set: the annotation
//! runs as the parser reaches the function, so a global below it would get no guard - the macro refuses the module
def private spawn_boot_restore_fixture(below : bool; tag : string) : tuple<rc : int; out : string> {
    var lines <- [
        "options gen2",
        "options _dasllama_internal = true",
        "",
        "require dasllama/dasllama_lint public",
        "require dasllama/dasllama_boot_restore",
        "",
        "def private f_a : int => 1",
        "var private g_above = @@f_a",
        "",
        "[init, boot_restore]",
        "def private restore_probe \{\}",
        ""]
    if (below) {
        lines |> push("var private g_below = @@f_a")
    }
    lines |> push("[export]")
    lines |> push("def main() \{")
    lines |> push("    print(\"ok\\n\")")
    lines |> push("\}")
    return spawn_compile_only(join(lines, "\n"), "boot_restore_{tag}", 120.0)
}

[test]
def test_boot_restore_order_gate(t : T?) {
    t |> run("[boot_restore] refuses a function global declared below its restore function and compiles the module with every global above") @(t : T?) {
        let bad = spawn_boot_restore_fixture(true, "below")
        t |> success(bad.rc != 0, "a `= @@fn` global below the restore function must not compile")
        t |> success(find(bad.out, "is declared below the restore function") >= 0, "the refusal names the global and the order: {bad.out}")
        let good = spawn_boot_restore_fixture(false, "above")
        t |> equal(0, good.rc, "every global above the restore function compiles: {good.out}")
    }
}

[test]
def test_allowed_set_admits_the_entry_modules(t : T?) {
    t |> run("a program on the facade, scheduler, exchange and bench entries with no escape compiles") @(t : T?) {
        let r = spawn_compile(["require dasllama/dasllama", "require dasllama/dasllama_scheduler",
                               "require dasllama/dasllama_exchange_schema", "require dasllama/dasllama_bench"], "entries")
        t |> equal(0, r.rc, "facade + scheduler + exchange_schema + bench allowed: {r.out}")
        t |> success(find(r.out, "DASLLAMA001") < 0, "no lint finding on the entry modules")
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

def private put_le(var b : array<uint8>; off : int; v : uint; nbytes : int) {
    for (i in range(nbytes)) {
        b[off + i] = uint8((v >> uint(i * 8)) & 0xFFu)
    }
}

//! A mono 8-bit PCM WAV of `data_bytes` samples at `rate` Hz. A rate of 1 is the decompression
//! bomb — miniaudio resamples it to 16 kHz, so a tiny file inflates into hours of PCM.
def fill_wav(var b : array<uint8>; rate : uint; data_bytes : int) {
    b |> resize(44 + data_bytes)
    b[0] = uint8('R'); b[1] = uint8('I'); b[2] = uint8('F'); b[3] = uint8('F')
    put_le(b, 4, uint(36 + data_bytes), 4)
    b[8] = uint8('W'); b[9] = uint8('A'); b[10] = uint8('V'); b[11] = uint8('E')
    b[12] = uint8('f'); b[13] = uint8('m'); b[14] = uint8('t'); b[15] = uint8(' ')
    put_le(b, 16, 16u, 4)
    put_le(b, 20, 1u, 2)
    put_le(b, 22, 1u, 2)
    put_le(b, 24, rate, 4)
    put_le(b, 28, rate, 4)
    put_le(b, 32, 1u, 2)
    put_le(b, 34, 8u, 2)
    b[36] = uint8('d'); b[37] = uint8('a'); b[38] = uint8('t'); b[39] = uint8('a')
    put_le(b, 40, uint(data_bytes), 4)
}

[test]
def test_decode_audio_frame_cap(t : T?) {
    t |> run("a well-formed clip under the cap decodes; a bomb over the cap decodes to EMPTY") @(t : T?) {
        let cap = 16000 * 10
        var ok : array<uint8>
        fill_wav(ok, 8000u, 8000)
        let good <- decode_audio_16k_mono(ok, cap)
        t |> success(!empty(good), "a short clip decodes ({length(good)} frames)")
        t |> success(length(good) <= cap, "the decode never exceeds the cap")
        var bomb : array<uint8>
        fill_wav(bomb, 1u, 4096)
        let refused <- decode_audio_16k_mono(bomb, cap)
        t |> equal(0, length(refused), "the length-probe refuses the bomb before decoding it")
        let uncapped <- decode_audio_16k_mono(ok, 0)
        t |> success(!empty(uncapped), "cap 0 is unbounded (the trusted-local-file path)")
        delete ok
        delete bomb
    }
}

[test]
def test_gemma4a_probe_rejects_non_gguf(t : T?) {
    t |> run("the audio probe returns 0 (never panics) on a .dlim path and on non-GGUF bytes") @(t : T?) {
        t |> equal(0l, gemma4a_probe_proj_dim("some-prepared-image.dlim"), "a .dlim is not a gguf")
        t |> equal(0l, gemma4a_probe_proj_dim("this-file-does-not-exist.gguf"), "a missing file is 0")
        let dir_r = create_temp_directory_result("dasllama_probe")
        if (dir_r is value) {
            let dir = unsafe(dir_r.value)
            let junk = path_join(dir, "junk.gguf")
            fwrite(junk, "not a gguf at all")
            t |> equal(0l, gemma4a_probe_proj_dim(junk), "non-GGUF bytes are 0, not a panic")
            rmdir_rec_result(dir)
        }
    }
}
