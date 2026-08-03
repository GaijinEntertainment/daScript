options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
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

[test]
def test_vkd_ar_class(t0 : T?) {
    t0 |> run("ar_add_rms class twin == the vk_add_rms seam (bit-exact, same reduce order)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let dim = 1024
            let bytes = int64(dim) * 4l
            let eps = 1e-5
            let ascale = 1.44
            var x : array<float>
            var a : array<float>
            var w : array<float>
            x |> resize(dim)
            a |> resize(dim)
            w |> resize(dim)
            for (i in range(dim)) {
                x[i] = float((i * 37) % 101) * 0.11 - 5.0
                a[i] = float((i * 13) % 89) * 0.07 - 3.0
                w[i] = 0.5 + float(i % 17) * 0.05
            }
            // the seam is the reference: the SHIPPED kernel on the same device, same inputs
            var x2 := x
            var y2 : array<float>
            y2 |> resize(dim)
            vk_add_rms(x2, a, w, y2, int64(dim), eps, ascale, true)
            // the class twin through the generated surface
            let xd = make_device_buf(bytes)
            let ad = make_device_buf(bytes)
            let wd = make_device_buf(bytes)
            let yd = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var yc : array<float>
            var xc : array<float>
            yc |> resize(dim)
            xc |> resize(dim)
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bytes)
                upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), bytes)
                upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), bytes)
                var s = set_cls_ar(fixed_array(xd, ad, wd, yd), fixed_array(bytes, bytes, bytes, bytes), fixed_array(1u, 2u, 4u, 8u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                enc_cls_ar(raw, h, s, pc, 1l)
                vhz_dep(raw, h, 8u | 1u, 0u, true)   // y + the updated x read back over the copy engine
                cmd_copy_whole(raw, yd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(yc[0]), host.mapped, bytes)
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                cmd_copy_whole(raw2, xd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(xc[0]), host.mapped, bytes)
            }
            var bad_y = 0
            var bad_x = 0
            for (i in range(dim)) {
                if (yc[i] != y2[i]) {
                    bad_y++
                }
                if (xc[i] != x2[i]) {
                    bad_x++
                }
            }
            t |> success(bad_y == 0, "normed output bit-exact vs the seam ({bad_y} of {dim} diverge)")
            t |> success(bad_x == 0, "residual stream bit-exact vs the seam ({bad_x} of {dim} diverge)")
            delete x
            delete a
            delete w
            delete x2
            delete y2
            delete yc
            delete xc
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}
