options gen2
options indenting = 4
options no_unused_function_arguments = false

// The box tag names a records store (records/<box>.json), a tune manifest
// (<box>.tune.json) and every per-box artifact - so it is DERIVED FROM HARDWARE, never
// defaulted: a wrong tag silently serves another box's crowns and poisons its store.
// Engine-free on purpose: tests/run.das requires this file without paying the engine
// compile, and profile_common re-exports box_name() through it.

module box_ident shared private

require strings
require daslib/fio

// the recorded fleet: CPU brand substring -> the store tag its records already carry
def private box_from_cpu(cpu : string) : string {
    if (cpu |> find("Apple M1 Max") >= 0) {
        return "m1"
    }
    if (cpu |> find("Apple M3") >= 0) {
        return "m3air"
    }
    if (cpu |> find("Apple M4 Pro") >= 0) {
        return "m4"
    }
    if (cpu |> find("Apple M5 Max") >= 0) {
        return "m5"
    }
    if (cpu |> find("3990X") >= 0) {
        return "zen2"
    }
    if (cpu |> find("8700GE") >= 0) {
        return "zen4"
    }
    return ""
}

def private capture_line(cmd : string) : string {
    var out = ""
    unsafe {
        popen(cmd) $(f) {
            if (f != null) {
                let ln = fgets(f)
                out = strip(ln)
            }
        }
    }
    return out
}

def public probe_cpu_brand : string {
    let plat = get_platform_name()   // "darwin" / "windows" / "linux" (probe-verified 2026-08-25)
    if (plat == "darwin") {
        return capture_line("sysctl -n machdep.cpu.brand_string")
    }
    if (plat == "windows") {
        // one REG_SZ line; the tail after the type column is the brand
        let ln = capture_line("reg query \"HKLM\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\" /v ProcessorNameString | findstr REG_SZ")
        let i = find(ln, "REG_SZ")
        return i >= 0 ? strip(slice(ln, i + length("REG_SZ"))) : ""
    }
    let ln = capture_line("grep -m1 \"model name\" /proc/cpuinfo")
    let i = find(ln, ":")
    return i >= 0 ? strip(slice(ln, i + 1)) : ""
}

// unknown hardware gets its own hardware-keyed tag, never another box's store
def private slug_of(cpu : string) : string {
    var s = build_string() $(var w) {
        var dash = true   // suppress a leading dash
        for (ch in cpu) {
            if ((ch >= 'a' && ch <= 'z') || is_number(ch)) {
                w |> write(to_char(ch))
                dash = false
            } elif (ch >= 'A' && ch <= 'Z') {
                w |> write(to_char(ch - 'A' + 'a'))
                dash = false
            } elif (!dash) {
                w |> write("-")
                dash = true
            }
        }
    }
    if (s |> ends_with("-")) {   // dash runs are suppressed above, so at most one survives
        s = slice(s, 0, length(s) - 1)
    }
    return empty(s) ? "unknown-box" : s
}

var private g_box_cached = ""

//! The pure half: CPU brand -> box tag (the fleet table, else the brand's own slug). The
//! probe and cache stay out so tests drive it directly.
def public box_tag_for_cpu(cpu : string) : string {
    let tag = box_from_cpu(cpu)
    return empty(tag) ? slug_of(cpu) : tag
}

//! The box tag: `DASLLAMA_BOX` (the explicit override) -> the CPU-brand fleet table -> a
//! slug of the brand itself (logged LOUDLY - a fresh box mints its own store either way).
def public derive_box_name(env_box : string) : string {
    if (!empty(env_box)) {
        return env_box
    }
    if (!empty(g_box_cached)) {
        return g_box_cached
    }
    let cpu = probe_cpu_brand()
    let tag = box_tag_for_cpu(cpu)
    if (empty(box_from_cpu(cpu))) {
        to_log(LOG_WARNING, "dasLLAMA box_ident: unrecognized hardware '{cpu}' - box tag derived as '{tag}'; set DASLLAMA_BOX or add this box to box_from_cpu\n")
    }
    g_box_cached = tag
    return tag
}
