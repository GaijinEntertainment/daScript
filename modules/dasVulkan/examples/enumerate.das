options gen2
options _comment_hygiene = true

require vulkan
require vulkan/vulkan_boost

def device_name(var props : VkPhysicalDeviceProperties) : string {
    unsafe {
        return "{reinterpret<string>(addr(props.deviceName[0]))}"
    }
}

def version_str(v : uint) : string {
    return "{int((v >> 22u) & 0x7Fu)}.{int((v >> 12u) & 0x3FFu)}.{int(v & 0xFFFu)}"
}

[export]
def main() : int {
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return 1
    }
    var inscope instance <- create_instance("dasVulkan enumerate", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))

    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    print("Vulkan physical devices: {int(count)}\n")
    if (count == 0u) {
        return 1
    }
    var devices : array<VkPhysicalDevice>
    devices |> resize(int(count))
    unsafe {
        vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, addr(devices[0]))
    }

    for (d in devices) {
        var props : VkPhysicalDeviceProperties
        vkGetPhysicalDeviceProperties(d, props)
        print("\n== {device_name(props)} ==\n")
        print("  type:        {props.deviceType}\n")
        print("  api version: {version_str(props.apiVersion)}\n")
        print("  max 2D image dim: {int(props.limits.maxImageDimension2D)}\n")

        var qcount = 0u
        vkGetPhysicalDeviceQueueFamilyProperties(d, qcount, null)
        var qprops : array<VkQueueFamilyProperties>
        qprops |> resize(int(qcount))
        unsafe {
            vkGetPhysicalDeviceQueueFamilyProperties(d, qcount, addr(qprops[0]))
        }
        print("  queue families: {int(qcount)}\n")
        for (i in range(int(qcount))) {
            let f = qprops[i].queueFlags
            print("    [{i}] count={int(qprops[i].queueCount)} graphics={f.graphics} compute={f.compute} transfer={f.transfer}\n")
        }

        var mem : VkPhysicalDeviceMemoryProperties
        vkGetPhysicalDeviceMemoryProperties(d, mem)
        print("  memory heaps: {int(mem.memoryHeapCount)}\n")
        for (i in range(int(mem.memoryHeapCount))) {
            let mb = int(mem.memoryHeaps[i].size / 1048576ul)
            print("    [{i}] {mb} MB device_local={mem.memoryHeaps[i].flags.device_local}\n")
        }
    }
    print("\nenumerate OK\n")
    return 0
}
