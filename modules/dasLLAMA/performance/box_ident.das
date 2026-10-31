options gen2
options indenting = 4
options no_unused_function_arguments = false

// The box tag names a records store (records/<box>.json), a tune manifest
// (<box>.tune.json) and every per-box artifact - so it is DERIVED FROM HARDWARE, never
// defaulted: a wrong tag silently serves another box's crowns and poisons its store.
// Engine-free on purpose: tests/run.das requires this file without paying the engine
// compile, and profile_common re-exports box_name() through it. It is also the one home of
// the box probes (CPU brand, RAM, WMI, the argv capture they run on), which the server
// requires by path for the same reason.

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

//! argv spawn (no shell), stdout+stderr merged by popen_argv, the whole output trimmed; "" where the program is missing
[arch(at="../ARCHITECTURE_ENGINE.md#instrumentation-and-support")]
def public run_capture_argv(args : array<string>) : string {
    var acc = ""
    unsafe(popen_argv(args, 0.0, $(f) {
        if (f == null) {   // popen_argv hands a null handle on spawn failure (a missing program)
            return
        }
        while (true) {   // fgets, not fread: the loop predates fread's pipe-safe EOF fallback and has never dropped output
            let line = fgets(f)
            if (empty(line)) {   // fgets keeps the newline, so "" only at EOF
                break
            }
            acc = "{acc}{line}"
        }
    }))
    return strip(acc)
}

//! the after-colon tail of the first line of `raw` containing `key`, "" when none does
def public line_value(raw, key : string) : string {
    for (ln in split(raw, "\n")) {
        let k = find(ln, key)
        if (k >= 0) {
            let c = find(ln, ":", k)
            if (c >= 0) {
                return strip(slice(ln, c + 1))
            }
        }
    }
    return ""
}

//! the `Key=value` lines of a WMI answer, each stripped; every line without '=' dropped
def public wmi_lines_of(raw : string) : array<string> =>
    <- [for (ln in split(raw, "\n")); strip(ln); where find(ln, "=") >= 0]

//! the Get-CimInstance script answering `props` ("Capacity,Speed") of `cls` in wmic's own `Key=value` lines
def public cim_script(cls : string; props : string) : string {
    let quoted = "'" + replace(props, ",", "','") + "'"
    return "Get-CimInstance {cls} | ForEach-Object \{ foreach ($p in {quoted}) \{ $p + '=' + $_.$p \} \}"
}

//! a Windows WMI class's `Key=value` lines for `props`, one block per instance; empty where nothing answers
[arch(at="../ARCHITECTURE_ENGINE.md#instrumentation-and-support")]
def public wmi_lines(cls : string; props : string) : array<string> {
    var lines <- wmi_lines_of(run_capture_argv(["wmic", "path", cls, "get", props, "/value"]))
    if (empty(lines)) {
        lines <- wmi_lines_of(run_capture_argv(["powershell", "-NoProfile", "-Command", cim_script(cls, props)]))
    }
    return <- lines
}

//! the value of `key` on one `Key=value` line, "" when the line carries another key
def public kv_value(ln : string; key : string) : string {
    let eq = find(ln, "=")
    return (eq >= 0 && strip(slice(ln, 0, eq)) == key) ? strip(slice(ln, eq + 1)) : ""
}

//! every non-empty value of `key` across `Key=value` lines, in line order
def public kv_values(lines : array<string>; key : string) : array<string> {
    var vals : array<string>
    for (ln in lines) {
        let v = kv_value(ln, key)
        if (!empty(v)) {
            vals |> push(v)
        }
    }
    return <- vals
}

//! every instance's non-empty `prop` of a Windows WMI class, in answer order
def public wmi_prop(cls : string; prop : string) : array<string> =>
    <- kv_values(wmi_lines(cls, prop), prop)

//! the RAM modules a `Win32_PhysicalMemory` `Capacity,Speed` answer lists: their count, one module's GiB, its MT/s
def public ram_modules_of(lines : array<string>) : tuple<count : int; cap_gb : int; speed : int> {
    var count = 0
    var cap_gb = 0
    var speed = 0
    for (ln in lines) {
        let cap = kv_value(ln, "Capacity")
        if (!empty(cap)) {
            count++
            cap_gb = int(to_float(cap) / (1024.0 * 1024.0 * 1024.0) + 0.5)
            continue
        }
        let spd = kv_value(ln, "Speed")
        if (!empty(spd)) {
            speed = to_int(spd)
        }
    }
    return (count = count, cap_gb = cap_gb, speed = speed)
}

//! /proc/meminfo's MemTotal ("131842348 kB") in whole GiB, 0 without the line
def private ram_gb_from_meminfo(text : string) : int {
    var inscope toks <- split(line_value(text, "MemTotal"), " ")
    let kb = empty(toks) ? 0.0 : to_float(toks[0])
    return int(kb / (1024.0 * 1024.0) + 0.5)
}

def public ram_gb_from_cgroup(text : string) : int {
    //! a cgroup memory-limit file's text in whole GiB, rounded up so a finite cap under a GiB still reads as one;
    //! 0 for no limit (`max`, or v1's page-rounded count past a petabyte) and for text that is no byte count
    let t = strip(text)
    return 0 if (empty(t) || !is_number(first_character(t)))
    let bytes = to_float(t)
    let gib = 1024.0 * 1024.0 * 1024.0
    return bytes <= 0.0 || bytes >= 1024.0 * 1024.0 * gib ? 0 : int(ceil(bytes / gib))
}

def public linux_ram_gb(meminfo, cgroup_v2, cgroup_v1 : string) : int {
    //! a Linux box's RAM in whole GiB from its three files' texts: the
    //! cgroup v2 limit, else the v1 one, where under the meminfo total
    let total = ram_gb_from_meminfo(meminfo)
    var cap = ram_gb_from_cgroup(cgroup_v2)
    if (cap == 0) {
        cap = ram_gb_from_cgroup(cgroup_v1)
    }
    return cap > 0 && (total == 0 || cap < total) ? cap : total
}

[arch(at="../ARCHITECTURE_ENGINE.md#instrumentation-and-support")]
def public box_ram_gb() : int {
    //! the box's RAM in GiB, 0 where no probe answers: the physical memory, or the container's limit where a cgroup
    //! caps this process under it (a RunPod pod reads the host's 251 GB off /proc/meminfo and holds 62)
    let gib = 1024.0 * 1024.0 * 1024.0
    let plat = get_platform_name()
    if (plat == "windows") {
        var inscope bytes <- wmi_prop("Win32_ComputerSystem", "TotalPhysicalMemory")
        return empty(bytes) ? 0 : int(to_float(bytes[0]) / gib + 0.5)
    }
    if (plat == "darwin") {
        return int(to_float(run_capture_argv(["sysctl", "-n", "hw.memsize"])) / gib + 0.5)
    }
    return linux_ram_gb(fread("/proc/meminfo"), fread("/sys/fs/cgroup/memory.max"),
        fread("/sys/fs/cgroup/memory/memory.limit_in_bytes"))
}

//! the CPU marketing name, "" where no probe answers
def public probe_cpu_brand : string {
    let plat = get_platform_name()
    if (plat == "darwin") {
        return run_capture_argv(["sysctl", "-n", "machdep.cpu.brand_string"])
    }
    if (plat == "windows") {
        let listing = run_capture_argv(["reg", "query", "HKLM\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            "/v", "ProcessorNameString"])
        for (ln in split(listing, "\n")) {
            let i = find(ln, "REG_SZ")
            if (i >= 0 && find(ln, "ProcessorNameString") >= 0) {
                return strip(slice(ln, i + length("REG_SZ")))
            }
        }
        return ""
    }
    let brand = line_value(fread("/proc/cpuinfo"), "model name")
    // arm64's /proc/cpuinfo carries no "model name" line; lscpu names the core from its part id
    return empty(brand) ? line_value(run_capture_argv(["lscpu"]), "Model name") : brand
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
