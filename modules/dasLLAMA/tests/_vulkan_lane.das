// The vulkan lane cells' rig - test_vulkan_mint (the live tier) and test_model_image_vulkan (the dry tier): the q8 cached
// load inside the job queue, and the route and trim levers a lane cell pins for its loads and puts back as it found them.

options gen2
options _dasllama_internal = true

module _vulkan_lane shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_image   // load_model_cached
require dasllama/dasllama_gpu_tier

//! `path` loaded through the image cache in q8 inside the job queue
def load_cached_q8(path : string) : Model {
    var m = Model()
    with_dasllama_jobque_() {
        m <- load_model_cached(path, QuantMode.q8)   // nolint:PERF030,LINT019 -- the move must sit inside with_job_que; the target is fresh-empty, so nothing drops
    }
    return <- m
}

//! the levers a lane cell pins for its loads, as it found them
struct LaneLevers {
    route : bool
    trim_set : bool
    trim : bool
}

//! the levers captured, then the resident route pinned on (the trimmed lane's admission reads it) and the trim off
def pin_lane_levers() : LaneLevers {
    let l = LaneLevers(route = gpu_resident_route_lever(), trim_set = vulkan_trim_lever_set(), trim = vulkan_trim())
    set_gpu_resident_route(true)
    set_vulkan_trim(false)
    return l
}

//! the route and the trim put back as `pin_lane_levers` found them
def restore_lane_levers(l : LaneLevers) {
    set_gpu_resident_route(l.route)
    restore_vulkan_trim(l.trim_set, l.trim)
}
