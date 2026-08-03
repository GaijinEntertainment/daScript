options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_math
require ?vulkan _vkd_toy
require ?vulkan vulkan
require ?vulkan vulkan/vulkan_boost
require ?vulkan dasllama/dasllama_vulkan_common
require daslib/shader_lingua_franca   // gl_GlobalInvocationID — the CPU replay drives it per lane

// The model-less Vulkan kernel-unit suite ([vk_dispatch] bring-up): a class kernel dispatched
// through the GENERATED surface (ensure/set/enc — per-class layout, push constants, hazard rail)
// against the CPU run of the SAME method (the class body is ordinary daslang; the driver loop
// feeds gl_GlobalInvocationID). Without dasVulkan (or with no device) every check feints cleanly.

[test]
def test_vkd_class_dispatch(t0 : T?) {
    t0 |> run("class kernel through the generated [vk_dispatch] surface == CPU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_vkd_toy()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let n = 250   // deliberately not a multiple of 64 — the in-kernel guard covers the pad
            let bytes = int64(n) * 4l
            var xs : array<float>
            xs |> resize(n)
            for (i in range(n)) {
                xs[i] = float(i) * 0.25 - 7.0
            }
            // CPU oracle: the same method on an instance with plain arrays; the loop overruns the
            // logical range exactly like the padded GPU grid does, so the guard is under test too
            var k = new VkdToy()
            k.xs := xs
            k.ys |> resize(n)
            k.pa = VtArgs(n = uint(n), gain = 2.5)
            for (i in range(n + 6)) {
                gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
                k->run()
            }
            // device: SSBOs + the generated set/encoder, one submit, readback
            let xdev = make_device_buf(bytes)
            let ydev = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var gpu : array<float>
            gpu |> resize(n)
            unsafe {
                upload_region_at(xdev, 0l, addr<uint8 const?>(xs[0]), bytes)
                var s = set_vkd_toy(fixed_array(xdev, ydev), fixed_array(bytes, bytes), fixed_array(1u, 2u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = VtArgs(n = uint(n), gain = 2.5)
                enc_vkd_toy(raw, h, s, pc, int64(n))
                vhz_dep(raw, h, 2u, 0u, true)   // ys (bit 2) reads back over the copy engine
                cmd_copy_whole(raw, ydev, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(gpu[0]), host.mapped, bytes)
            }
            var bad = 0
            for (i in range(n)) {
                if (gpu[i] != k.ys[i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "GPU == CPU on all {n} lanes ({bad} diverge)")
            delete xs
            delete gpu
            unsafe {
                delete k
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}
