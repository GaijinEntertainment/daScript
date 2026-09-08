options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

[test]
def test_os_video_memory(t : T?) {
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    var idp = VkPhysicalDeviceIDProperties()
    var p2 = VkPhysicalDeviceProperties2()
    unsafe {
        p2.pNext = addr<void?>(idp)
    }
    vkGetPhysicalDeviceProperties2(phys, p2)
    let os = os_video_memory(phys)
    let answered = os.budget > 0ul
    if (idp.deviceLUIDValid == 0u) {
        t |> success(!answered && os.usage == 0ul && os.adapter_dedicated == 0ul && os.adapter_shared == 0ul, "a device with no LUID reads 0, 0, 0, 0 (no answer)")
        return
    }
    t |> success(os.usage <= os.budget, "the process's usage never passes its ceiling ({int64(os.usage)} <= {int64(os.budget)})")
    t |> success(!answered || os.adapter_dedicated >= os.usage, "the adapter's usage is at least the process's ({int64(os.adapter_dedicated)} >= {int64(os.usage)})")
    to_log(LOG_INFO, "os video memory: budget {int64(os.budget) / 1_000_000l} MB, usage {int64(os.usage) / 1_000_000l} MB, adapter {int64(os.adapter_dedicated) / 1_000_000l} MB dedicated + {int64(os.adapter_shared) / 1_000_000l} MB system{answered ? "" : " (no OS answer on this platform)"}\n")
}
