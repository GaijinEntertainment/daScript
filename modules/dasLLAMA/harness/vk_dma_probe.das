options gen2
options stack = 524288
options _dasllama_internal = true

//! The device-to-host copy ruler: the logits' trip to host memory (`cmd_copy_whole` into a cached
//! host buffer) timed on the compute queue by device timestamps and on the transfer queue by the
//! host clock, at the sizes a token command moves (one row's logits to a 64-row batch's), so a box
//! whose copy path is slow shows it as a number per page rather than as a fence gap.
//!
//!   bin/daslang -jit modules/dasLLAMA/harness/vk_dma_probe.das

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common

let private REPS = 8

//! one copy of `bytes` on the compute queue, the device clock around it, averaged over REPS submits
def private compute_copy_us(src, dst : uint64; bytes : int64) : double {
    var raw = alloc_cmd()
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    ensure_pfq()
    g_pfq_on = true
    g_pfq_n = 0u
    cmd_reset_query_pool(vk_value_to_boost(raw), g_pfq_pool, 0u, PFQ_CAP)
    pfq_ts(raw)
    cmd_copy_whole(raw, src, dst, bytes)
    pfq_ts(raw)
    pfq_end(raw)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(raw)
    var us = 0.0lf
    for (_i in range(REPS)) {
        submit_wait(raw)
        us += pfq_us(1u)
    }
    return us / double(REPS)
}

//! the same copy on the transfer queue, the host clock from submit to the timeline's signal
def private transfer_copy_us(src, dst : uint64; bytes : int64) : double {
    var raw = g_gpu.xfer_cmd
    var us = 0.0lf
    for (i in range(REPS + 1)) {
        xfer_host_wait(g_gpu.xfer_value)   //! the shared transfer command re-records only after its last submit retired
        let rf : VkCommandBufferResetFlags
        vk_check(vkResetCommandBuffer(raw, rf), null)
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        cmd_copy_whole(raw, src, dst, bytes)
        vk_check(vkEndCommandBuffer(raw), null)
        g_gpu.xfer_value++
        var sig = g_gpu.xfer_value
        var tss = VkTimelineSemaphoreSubmitInfo()
        var sem = g_gpu.xfer_sem
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        submit.signalSemaphoreCount = 1u
        let nofence : VkFence
        let t0 = ref_time_ticks()
        unsafe {
            tss.signalSemaphoreValueCount = 1u
            tss.pSignalSemaphoreValues = addr(sig)
            submit.pNext = addr<void?>(tss)
            submit.pCommandBuffers = addr(raw)
            submit.pSignalSemaphores = addr(sem)
            vk_check(vkQueueSubmit(g_gpu.xfer_queue, 1u, addr(submit), nofence), null)
        }
        xfer_host_wait(g_gpu.xfer_value)
        if (i > 0) {   //! the first submit is the warm-up
            us += double(get_time_usec(t0))
        }
    }
    return us / double(REPS)
}

[export]
def main : int {
    if (!vk_moe_init()) {
        print("vk_dma_probe: no Vulkan device\n")
        return 1
    }
    let xfer = g_gpu.xfer_fam >= 0
    let xq = xfer ? "family {g_gpu.xfer_fam}" : "absent"
    print("vk_dma_probe: device -> cached host copies, {REPS} reps after a warm-up; compute queue by the device clock, transfer queue ({xq}) by the host clock including its submit and wait\n")
    print("|     bytes | compute us | compute GB/s | us/4K page | transfer us | transfer GB/s |\n")
    print("| --------: | ---------: | -----------: | ---------: | ----------: | ------------: |\n")
    for (kb in [512, 2048, 8192, 32768]) {
        let bytes = int64(kb) * 1024l
        let src = make_device_buf(bytes)
        let dst = make_host_buf(bytes, false, [cached = true])
        let cus = compute_copy_us(src, dst.buf, bytes)
        let pages = double(bytes) / 4096.0lf
        let tus = xfer ? transfer_copy_us(src, dst.buf, bytes) : 0.0lf
        print("| {bytes:9d} | {cus:10.1f} | {double(bytes) / (cus * 1000.0lf):12.2f} | {cus / pages:10.3f} | {tus:11.1f} | {xfer ? double(bytes) / (tus * 1000.0lf) : 0.0lf:13.2f} |\n")
    }
    return 0
}
