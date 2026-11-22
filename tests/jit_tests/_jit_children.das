options gen2

module _jit_children public

require daslib/fio
require strings
require dastest/testing_boost
require daslib/strings_boost

//! what the jit_tests files that start daslang children share: where
//! they write, how they run a child and read its lines, and the names
//! of the artifacts a build leaves

let OUTPUT_DIR = test_temp_dir()

let FIXTURE_DIR = "{get_das_root()}/tests/jit_tests"

let WASM_LOWERING_FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_lowering_fixture.das"

let TWO_POW_112 = "0x46F0000000000000"

//! no DLL cache; module cache on, so the emitter compiles once a run
let NO_DLL_CACHE_JIT = "-dasroot \"{get_das_root()}\" -jit -jit-no-cache"

def spawn(cmd : string; var lines : array<string>) : int {
    let rc = unsafe(popen_timeout("{cmd} 2>&1", 600.0) $(f) {
        if (f == null) {
            return
        }
        while (!feof(f)) {
            let ln = strip(fgets(f))
            if (!(ln |> empty())) {
                lines |> push("{ln}")
            }
        }
    })
    return rc
}

def spawn_child(cmd : string; var lines : array<string>) : int {
    var rc : int
    var full = "{cmd} 2>&1"
    if (get_platform_name() == "windows") {
        // cmd.exe strips the first and last quote of a line that opens with
        // one
        full = "\"{full}\""
    }
    rc = unsafe(popen_timeout(full, 300.0) $(f) {
        if (f == null) {
            return
        }
        while (!feof(f)) {
            let ln = strip(fgets(f))
            if (!empty(ln)) {
                lines |> push("{ln}")
            }
        }
    })
    return rc
}

def has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) return true
    }
    return false
}

//! the lines holding `needle`, and `also` when one is named
def count_with(lines : array<string>; needle : string; also : string = "") : int {
    var n = 0
    for (ln in lines) {
        if (find(ln, needle) >= 0 && (empty(also) || find(ln, also) >= 0)) {
            n++
        }
    }
    return n
}

//! runs `cmd` in `OUTPUT_DIR`, where the child's module cache lands
def spawn_in_temp(cmd : string; var lines : array<string>) : int {
    mkdir_rec(OUTPUT_DIR)
    return spawn_child("{cd_to(OUTPUT_DIR)} && {cmd}", lines)
}

//! the shell prefix that makes `dir` the child's working directory;
//! on Windows a plain `cd` stays on the current drive
def cd_to(dir : string) : string =>
    get_platform_name() == "windows" ? "cd /d \"{dir}\"" : "cd \"{dir}\""

def shared_artifact(stem : string) : string {
    let plat = get_platform_name()
    if (plat == "windows") {
        return "{stem}.dll"
    }
    return plat == "darwin" ? "{stem}.dylib" : "{stem}.so"
}


def static_artifact(stem : string) : string =>
    get_platform_name() == "windows" ? "{stem}.lib" : "{stem}.a"
