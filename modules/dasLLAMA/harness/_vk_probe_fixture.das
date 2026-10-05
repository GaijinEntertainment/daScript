options gen2
options indenting = 4
options _dasllama_internal = true

// The fixture the Vulkan probes share: the synthetic planes (a word hash for quant bytes and a packed
// f16-pair scale word, the same shapes the kernel suite's fixtures use) and the device-timestamp
// scaffold around a recorded command.

module _vk_probe_fixture shared public

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require ../tests/_vk_kq_fixtures.das public

//! arms the device timestamp rail on the command `raw` is recording: the probe's stamps land from query 0
def pfq_arm(raw : VkCommandBuffer) {
    ensure_pfq()
    g_pfq_on = true
    g_pfq_n = 0u
    cmd_reset_query_pool(vk_value_to_boost(raw), g_pfq_pool, 0u, PFQ_CAP)
}

//! a recorded command's mean device time per stamp interval: one warm-up submit (pipeline compile, first-touch), then
//! `submits` submits, each summing its `stamps` intervals (query i to i - 1), over submits x stamps
def pfq_avg_us(var raw : VkCommandBuffer; submits, stamps : int) : double {
    submit_wait(raw)
    var us = 0.0lf
    for (_i in range(submits)) {
        submit_wait(raw)
        for (i in range(stamps)) {
            us += pfq_us(uint(i + 1))
        }
    }
    return us / double(submits * stamps)
}

//! a recorded command's host wall per dispatch: `warmups` warm-up submits (pipeline compile, first-touch), then `submits`
//! timed submits over `submits` x `dispatches`
def submit_avg_us(var raw : VkCommandBuffer; submits, dispatches, warmups : int) : double {
    for (_w in range(warmups)) {
        submit_wait(raw)
    }
    let t0 = ref_time_ticks()
    for (_s in range(submits)) {
        submit_wait(raw)
    }
    return double(get_time_usec(t0)) / double(submits * dispatches)
}

//! a fresh command `rec` records on one hazard rail, then its `submit_avg_us`
def time_chain(submits, dispatches, warmups : int; rec : block<(raw : VkCommandBuffer; var hz : VkHaz) : void>) : double {
    var raw = alloc_cmd()
    record_cmd(raw, false, rec)
    return submit_avg_us(raw, submits, dispatches, warmups)
}

//! two packed f16 scales per word, small positive values (the kernel-suite fixture shape)
def cool_f16_pair(i : uint) : uint {
    let h = 0x2c00u + (i % 64u)
    return h | (h << 16u)
}
