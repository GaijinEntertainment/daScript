options gen2
options indenting = 4
options _dasllama_internal = true

// The fixture the CUDA-vs-Vulkan races share: one round of each arm, a race's rounds run in turn, each arm's best round, and
// the report lines.

module _cuda_probe_fixture shared public

require vulkan
require daslib/fio
require math
require daslib/strings_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_cuda_common
require _vk_probe_fixture

let VK_CLASS_REFUSED = "the Vulkan class did not arm (no Vulkan device, or its build failed)"
let CU_CLASS_REFUSED = "the CUDA class did not arm (no CUDA device with NVRTC, or its build failed)"

//! one round of a CUDA arm in microseconds a launch: `dispatches` launches closed by a sync on the host wall, then as many
//! between a CUDA event pair; `launch` takes its index within the round
def cuda_time_chain(dispatches : int; launch : block<(i : int) : void>) : tuple<wall : double; event : double> {
    let t0 = ref_time_ticks()
    for (i in range(dispatches)) {
        invoke(launch, i)
    }
    cud_sync()
    let wall = double(get_time_usec(t0)) / double(dispatches)
    let ms = cud_time_ms() {
        for (i in range(dispatches)) {
            invoke(launch, i)
        }
    }
    return (wall = wall, event = double(ms) * 1000.0lf / double(dispatches))
}

//! a Vulkan arm's command, recorded once; each of the arm's rounds is one submit of it
def record_arm(rec : block<(raw : VkCommandBuffer; var hz : VkHaz) : void>) : VkCommandBuffer {
    var raw = alloc_cmd()
    record_cmd(raw, false, rec)
    return raw
}

//! a race's arms: each arm's best round in microseconds a dispatch (the CUDA arm's on the host wall and on its events), or,
//! when its `why` is set, the reason the arm refused and no figure
struct RaceRow {
    vk : double
    cu_wall : double
    cu_event : double
    vk_why : string
    cu_why : string
}

//! `warmups` dropped rounds, then `rounds` timed ones, each one submit of the Vulkan arm's `raw` and then one round of the
//! CUDA arm's `launch`; an arm whose `why` is set does not run
def race(rounds, warmups, dispatches : int; vk_why : string; var raw : VkCommandBuffer; cu_why : string; launch : block<(i : int) : void>) : RaceRow {
    var row = RaceRow(vk_why = vk_why, cu_why = cu_why)
    for (r in range(warmups + rounds)) {
        let first = r == warmups
        if (vk_why == "") {
            let us = submit_avg_us(raw, 1, dispatches, 0)
            if (r >= warmups) {
                row.vk = first ? us : min(row.vk, us)
            }
        }
        if (cu_why == "") {
            let t = cuda_time_chain(dispatches, launch)
            if (r >= warmups) {
                row.cu_wall = first ? t.wall : min(row.cu_wall, t.wall)
                row.cu_event = first ? t.event : min(row.cu_event, t.event)
            }
        }
    }
    return row
}

//! a CUDA arm no Vulkan arm races: its rounds alone
def cuda_rounds(rounds, warmups, dispatches : int; launch : block<(i : int) : void>) : RaceRow {
    var raw : VkCommandBuffer
    return race(rounds, warmups, dispatches, "no Vulkan arm", raw, "", launch)
}

//! a figure in microseconds, and the rate `rate` makes of it when `unit` names one
def private us_text(us : double; unit : string; rate : block<(us : double) : double>) : string {
    return "{us:9.2f} us" if (unit == "")
    let r = invoke(rate, us)
    return "{us:9.2f} us ({r:8.2f} {unit})"
}

//! the CUDA arm's report line, its wall and event figures then `tail`, or its refusal and no figure; 1 when it measured
def report_cuda(shape : string; row : RaceRow; tail, unit : string; rate : block<(us : double) : double>) : int {
    if (row.cu_why != "") {
        print("{shape} cuda refused: {row.cu_why}\n")
        return 0
    }
    print("{shape} cuda wall {us_text(row.cu_wall, unit, rate)} event {us_text(row.cu_event, unit, rate)} {tail}\n")
    return 1
}

//! a race's report lines, one an arm, each `timing-only`, or the arm's refusal and no figure; the arms measured
def report_race(shape : string; row : RaceRow; unit : string; rate : block<(us : double) : double>) : int {
    var measured = 0
    if (row.vk_why != "") {
        print("{shape} vulkan refused: {row.vk_why}\n")
    } else {
        print("{shape} vulkan {us_text(row.vk, unit, rate)} timing-only\n")
        measured++
    }
    return measured + report_cuda(shape, row, "timing-only", unit, rate)
}

//! the same, the figures with no rate
def report_race(shape : string; row : RaceRow) : int => report_race(shape, row, "") $(us) => us

//! a run that measured no arm says so and exits 1
def require_measured(measured : int) {
    if (measured == 0) {
        print("no arm measured\n")
        unsafe(exit(1))
    }
}

//! `--only <name,name,...>` off the command line as a set: a race's arm filter (format names, a shape switch), empty = every arm
def race_only : table<string; void> {
    var only : table<string; void>
    var take = false
    for (a in get_command_line_arguments()) {
        if (take) {
            for (f in split(a, ",")) {
                only |> insert(f)
            }
            take = false
        }
        take = a == "--only"
    }
    return <- only
}
