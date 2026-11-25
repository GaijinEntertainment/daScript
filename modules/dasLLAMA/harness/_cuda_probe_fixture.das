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
require _cuda_time public   // cuda_time_chain: a CUDA arm's round

let VK_CLASS_REFUSED = "the Vulkan class did not arm (no Vulkan device, or its build failed)"
let CU_CLASS_REFUSED = "the CUDA class did not arm (no CUDA device with NVRTC, or its build failed)"

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

//! a race of several arms: each arm's best round, and every timed round's microseconds a dispatch a home ([arm][round])
struct RaceArms {
    rows : array<RaceRow>
    vk_rounds : array<array<double>>
    cu_wall_rounds : array<array<double>>
}

//! `warmups` dropped rounds, then `rounds` timed ones; a round is, an arm in turn, one submit of the arm's Vulkan command
//! (`raws`) and then one round of its CUDA `launch`; a home whose `why` is set does not run
def race_arms(rounds, warmups, dispatches : int; vk_why : string; var raws : array<VkCommandBuffer>; cu_why : string; launch : block<(arm, i : int) : void>) : RaceArms {
    let arms = length(raws)
    var res <- RaceArms(rows <- [for (_a in range(arms)); RaceRow(vk = 1.0e30lf, cu_wall = 1.0e30lf, cu_event = 1.0e30lf, vk_why = vk_why, cu_why = cu_why)],
        vk_rounds <- [for (_a in range(arms)); [for (_r in range(rounds)); 0.0lf]],
        cu_wall_rounds <- [for (_a in range(arms)); [for (_r in range(rounds)); 0.0lf]])
    for (r in range(warmups + rounds)) {
        for (row, arm in res.rows, count()) {
            let us = vk_why == "" ? submit_avg_us(raws[arm], 1, dispatches, 0) : 0.0lf
            var wall = 0.0lf
            var event = 0.0lf
            if (cu_why == "") {
                let t = cuda_time_chain(dispatches) $(i) {
                    invoke(launch, arm, i)
                }
                wall = t.wall
                event = t.event
            }
            continue if (r < warmups)
            row.vk = min(row.vk, us)
            row.cu_wall = min(row.cu_wall, wall)
            row.cu_event = min(row.cu_event, event)
            res.vk_rounds[arm][r - warmups] = us
            res.cu_wall_rounds[arm][r - warmups] = wall
        }
    }
    return <- res
}

//! one arm's race: each round one submit of the Vulkan arm's `raw` and then one round of the CUDA arm's `launch`
def race(rounds, warmups, dispatches : int; vk_why : string; var raw : VkCommandBuffer; cu_why : string; launch : block<(i : int) : void>) : RaceRow {
    var raws <- [raw]
    var res <- race_arms(rounds, warmups, dispatches, vk_why, raws, cu_why) $(_arm, i) {
        invoke(launch, i)
    }
    let row = res.rows[0]
    raws |> clear()
    unsafe {
        delete raws
    }
    delete res
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
def only_arms : table<string; void> {
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
