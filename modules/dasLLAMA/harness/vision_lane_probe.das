// a vision tower's device encode timed on either home, either lane: `-- <gemma3v|gemma4v|qwen3v|qwen25v> <q8 0|1> <mmproj> <w> <h>`
// the tower staged and minted in memory under the lane pin (no .dlim), the family's encode called thirteen times, the median of the last ten in ms
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
require daslib/jobque_boost
require daslib/strings_convert

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
    var times : array<double>
    for (i in range(WARM + TIMED)) {
        let t0 = ref_time_ticks()
        invoke(encode)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        if (i >= WARM) {
            times |> push(ms)
        }
    }
    sort(times)
    print("{tag}: median {times[TIMED / 2]} ms, min {times[0]} ms, max {times[TIMED - 1]} ms\n")
}

[export]
def main() : int {
    let args <- get_command_line_arguments()
    let i0 = find_index(args, "--") + 1
    if (i0 <= 0 || i0 + 4 >= length(args)) {
        print("usage: vision_lane_probe.das -- <gemma3v|gemma4v|qwen3v|qwen25v> <q8 0|1> <mmproj> <w> <h>\n")
        return 1
    }
    let fam = args[i0]
    let q8 = args[i0 + 1] == "1"
    let mm = args[i0 + 2]
    let w = to_int64(args[i0 + 3], false)
    let h = to_int64(args[i0 + 4], false)
    let lane = q8 ? "q8" : "exact"
    if (fam != "gemma3v" && fam != "gemma4v" && fam != "qwen3v" && fam != "qwen25v") {
        print("unknown family {fam}\n")
        return 1
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(true)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(true)
    }
    let planes <- planes_of(w, h)
    var out : array<float>
    var ntok = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        if (fam == "gemma3v") {
            set_gemma3v_q8(q8)
            var inscope stg <- stage_gemma3v_tower(mm)
            var tw = Gemma3vTower()
            mint_gemma3v_tower(stg, tw)
            reset_gemma3v_q8()
            var st = Gemma3vState()
            timed("gemma3v {lane} {w}x{h}") {
                ntok = gemma3v_encode(tw, st, planes, w, h, out)
            }
        } elif (fam == "gemma4v") {
            set_gemma4v_q8(q8)
            var inscope stg <- stage_gemma4v_tower(mm)
            var tw = Gemma4vTower()
            mint_gemma4v_tower(stg, tw)
            reset_gemma4v_q8()
            var st = Gemma4vState()
            timed("gemma4v {lane} {w}x{h}") {
                ntok = gemma4v_encode(tw, st, planes, w, h, out)
            }
        } elif (fam == "qwen3v") {
            set_qwen3v_q8(q8)
            var inscope stg <- stage_qwen3v_tower(mm)
            var tw = Qwen3vTower()
            mint_qwen3v_tower(stg, tw)
            reset_qwen3v_q8()
            var st = Qwen3vState()
            timed("qwen3v {lane} {w}x{h}") {
                ntok = qwen3v_encode(tw, st, planes, w, h, out)
            }
        } else {
            var inscope stg <- stage_qwen25v_tower(mm)
            var tw = Qwen25vTower()
            mint_qwen25v_tower(stg, tw)
            var st = Qwen25vState()
            timed("qwen25v exact {w}x{h}") {
                ntok = qwen25v_encode(tw, st, planes, w, h, out)
            }
        }
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        print("tokens {ntok}; {metal_tower_stats()}\n")
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        print("tokens {ntok}; {vulkan_tower_stats()} {vulkan_tower_declines()}\n")
    }
    return 0
}
