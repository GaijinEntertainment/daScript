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
require math
require daslib/fio
require daslib/strings_boost

// the recorded fleet: CPU brand substring -> the store tag its records already carry
def private box_from_cpu(cpu : string) : string {
    if (cpu |> find("Apple M1 Max") >= 0) {
        return "m1"
    }
    if (cpu == "Apple M3") {   // EXACT: the Air's base chip — "Apple M3 Pro/Max/Ultra" must slug, not inherit the Air's crowns and store
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
    if (cpu |> find("6975P") >= 0) {
        return "gnr"
    }
    if (cpu |> find("EPYC 9R14") >= 0) {   // AWS c7a / m7a (Zen4)
        return "epyc9r14"
    }
    if (cpu |> find("Neoverse-V2") >= 0) {   // AWS c8g / m8g (Graviton4)
        return "graviton4"
    }
    return ""
}

def private capture_line(cmd : string) : string {
    var out = ""
    unsafe(popen(cmd) $(f) {
        if (f != null) {
            let ln = fgets(f)
            out = strip(ln)
        }
    })
    return out
}

//! the first line of `path`, "" without the file
def private first_line(path : string) : string {
    var out = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            out = strip(fgets(f))
        }
    }
    return out
}

//! the box's RAM in GiB, 0 where no probe answers: the physical memory, or the container's limit where a cgroup caps
//! this process under it (a RunPod pod reads the host's 251 GB off /proc/meminfo and holds 62)
def public box_ram_gb() : int {
    let gib = 1024.0 * 1024.0 * 1024.0
    let plat = get_platform_name()
    if (plat == "windows") {
        // wmic emits "TotalPhysicalMemory=68632934400"; clean Windows 11 24H2+ installs ship without wmic (it survives
        // in-place upgrades), so CIM answers when it yields nothing - and 2>nul keeps the miss off the console
        let raw = capture_line("wmic ComputerSystem get TotalPhysicalMemory /value 2>nul | findstr TotalPhysicalMemory")
        let i = find(raw, "=")
        if (i >= 0) {
            return int(to_float(strip(slice(raw, i + 1))) / gib + 0.5)
        }
        let ps = capture_line("powershell -NoProfile -Command \"(Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory\"")
        return empty(ps) ? 0 : int(to_float(ps) / gib + 0.5)
    }
    if (plat == "darwin") {
        return int(to_float(capture_line("sysctl -n hw.memsize")) / gib + 0.5)
    }
    // linux /proc/meminfo: "MemTotal:       131842348 kB"; a cgroup v2 limit in bytes at memory.max ("max" = none)
    let ln = first_line("/proc/meminfo")
    let i = find(ln, ":")
    var gb = 0
    if (i >= 0) {
        let toks <- split(strip(slice(ln, i + 1)), " ")
        gb = empty(toks) ? 0 : int(to_float(toks[0]) / (1024.0 * 1024.0) + 0.5)
    }
    let cap = first_line("/sys/fs/cgroup/memory.max")
    if (!empty(cap) && cap != "max") {
        let cap_gb = int(to_float(cap) / gib + 0.5)
        gb = gb == 0 ? cap_gb : min(gb, cap_gb)
    }
    return gb
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
    // arm64 /proc/cpuinfo carries no "model name" line - lscpu names the core from its part id
    var ln = capture_line("grep -m1 \"model name\" /proc/cpuinfo")
    if (empty(ln)) {
        ln = capture_line("lscpu | grep -m1 \"Model name\"")
    }
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
