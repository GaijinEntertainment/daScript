// a vision tower's device encode timed on either home, either lane: `-- <gemma3v|gemma4v|qwen3v|qwen25v> <q8 0|1> <mmproj> <w> <h>`
// the tower staged and minted in memory under the lane pin (no .dlim) in the serving home's flavor on the q8 lane (`#tower-dev-block`),
// the family's encode called thirteen times, the median of the last ten in ms
options gen2
options stack = 524288
options _dasllama_internal = true
require dasllama/dasllama_gemma3v
require dasllama/dasllama_gemma4v
require dasllama/dasllama_qwen3v
require dasllama/dasllama_qwen25v
require dasllama/dasllama_math
require ?das_metal dasllama/dasllama_metal_tower
require ?vulkan dasllama/dasllama_vulkan_tower
require ?cuda dasllama/dasllama_cuda_tower
require dasllama/dasllama_tower   // tower_dev_staged: the staging under the serving flavor
require ../tests/_gpu_knobs.das   // serving_tower_dev, set_every_gpu_tower: the serving home's flavor and knobs
require daslib/jobque_boost
require daslib/strings_convert
require daslib/strings_boost
require _row_stats

let WARM = 3
let TIMED = 10

def planes_of(w, h : int64) : array<float> {
    var planes : array<float>
    planes |> reserve_resize(w * h * 3l)
    for (y in range64(h)) {
        for (x in range64(w)) {
            let v = ((x / 16l + y / 16l) % 2l == 0l) ? 0.8 : 0.2
            for (c in range64(3l)) {
                planes[(y * w + x) * 3l + c] = v + 0.05 * float(c)
            }
        }
    }
    return <- planes
}

def timed(tag : string; encode : block<() : void>) {
    var times : array<float>
    for (i in range(WARM + TIMED)) {
        let t0 = ref_time_ticks()
        invoke(encode)
        let ms = float(get_time_usec(t0)) / 1000.0
        if (i >= WARM) {
            times |> push(ms)
        }
    }
    let samples : string = join([for (ms in times); "{ms}"], " ")
    let sp = spread_of(times)
    print("{tag}: median {sp.median} ms, min {sp.best} ms, max {times[TIMED - 1]} ms, cv {sp.cv}%; samples {samples}\n")
}

//! the probe's arguments: the family, the lane, the mmproj and the canvas; `ok` false after the usage line
def parse_args() : tuple<ok : bool; fam : string; q8 : bool; mm : string; w : int64; h : int64> {
    let args <- get_command_line_arguments()
    let i0 = find_index(args, "--") + 1
    if (i0 <= 0 || i0 + 4 >= length(args)) {
        print("usage: vision_lane_probe.das -- <gemma3v|gemma4v|qwen3v|qwen25v> <q8 0|1> <mmproj> <w> <h>\n")
        return (ok = false, fam = "", q8 = false, mm = "", w = 0l, h = 0l)
    }
    let fam = args[i0]
    if (fam != "gemma3v" && fam != "gemma4v" && fam != "qwen3v" && fam != "qwen25v") {
        print("unknown family {fam}\n")
        return (ok = false, fam = fam, q8 = false, mm = "", w = 0l, h = 0l)
    }
    return (ok = true, fam = fam, q8 = args[i0 + 1] == "1", mm = args[i0 + 2], w = to_int64(args[i0 + 3], false), h = to_int64(args[i0 + 4], false))
}

[export]
def main() : int {
    let a = parse_args()
    return 1 if (!a.ok)
    let lane = a.q8 ? "q8" : "exact"
    set_every_gpu_tower(true)
    let dev = a.q8 ? serving_tower_dev(TowerFamily.vision) : TowerDev.none
    let planes <- planes_of(a.w, a.h)
    var out : array<float>
    var ntok = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        if (a.fam == "gemma3v") {
            set_gemma3v_q8(a.q8)
            var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gemma3v_tower(a.mm) }
            var tw = Gemma3vTower()
            mint_gemma3v_tower(stg, tw, dev)
            reset_gemma3v_q8()
            var st = Gemma3vState()
            timed("gemma3v {lane} {a.w}x{a.h}") {
                ntok = gemma3v_encode(tw, st, planes, a.w, a.h, out)
            }
        } elif (a.fam == "gemma4v") {
            set_gemma4v_q8(a.q8)
            var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gemma4v_tower(a.mm) }
            var tw = Gemma4vTower()
            mint_gemma4v_tower(stg, tw, dev)
            reset_gemma4v_q8()
            var st = Gemma4vState()
            timed("gemma4v {lane} {a.w}x{a.h}") {
                ntok = gemma4v_encode(tw, st, planes, a.w, a.h, out)
            }
        } elif (a.fam == "qwen3v") {
            set_qwen3v_q8(a.q8)
            var inscope stg <- tower_dev_staged(dev) $ { return <- stage_qwen3v_tower(a.mm) }
            var tw = Qwen3vTower()
            mint_qwen3v_tower(stg, tw, dev)
            reset_qwen3v_q8()
            var st = Qwen3vState()
            timed("qwen3v {lane} {a.w}x{a.h}") {
                ntok = qwen3v_encode(tw, st, planes, a.w, a.h, out)
            }
        } else {
            var inscope stg <- stage_qwen25v_tower(a.mm)
            var tw = Qwen25vTower()
            mint_qwen25v_tower(stg, tw)
            var st = Qwen25vState()
            timed("qwen25v exact {a.w}x{a.h}") {
                ntok = qwen25v_encode(tw, st, planes, a.w, a.h, out)
            }
        }
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        print("tokens {ntok}; {metal_tower_stats()}\n")
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        print("tokens {ntok}; vulkan {vulkan_tower_stats()} {vulkan_tower_declines()}\n")
    }
    static_if (typeinfo builtin_module_exists(cuda)) {
        print("tokens {ntok}; cuda {cuda_tower_stats()} {cuda_tower_declines()}\n")
    }
    return 0
}
