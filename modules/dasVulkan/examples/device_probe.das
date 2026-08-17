options gen2
options _comment_hygiene = true

require vulkan

[export]
def main() {   // nolint:STYLE038 - flat example scaffold
    if (volkInitialize() != 0) {
        print("no Vulkan\n")
        return
    }
    var ai : VkApplicationInfo
    ai.sType = VkStructureType.APPLICATION_INFO
    ai.apiVersion = (1u << 22u) | (3u << 12u)
    var ici : VkInstanceCreateInfo
    ici.sType = VkStructureType.INSTANCE_CREATE_INFO
    unsafe {
        ici.pApplicationInfo = addr(ai)
    }
    var instance : VkInstance
    if (vkCreateInstance(ici, null, instance) != VkResult.SUCCESS) {
        print("instance failed\n")
        return
    }
    volkLoadInstance(instance)

    // pick first physical device
    var count = 0u
    vkEnumeratePhysicalDevices(instance, count, null)
    if (count == 0u) {
        print("no devices\n")
        return
    }
    var devices : array<VkPhysicalDevice>
    devices |> resize(int(count))
    unsafe {
        vkEnumeratePhysicalDevices(instance, count, addr(devices[0]))
    }
    let phys = devices[0]

    // find a graphics queue family -- exercises bitfield field access
    var qcount = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, null)
    var qprops : array<VkQueueFamilyProperties>
    qprops |> resize(int(qcount))
    unsafe {
        vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, addr(qprops[0]))
    }
    var gfx_family = -1
    for (i in range(int(qcount))) {
        if (qprops[i].queueFlags.graphics) {
            gfx_family = i
            break
        }
    }
    print("graphics queue family: {gfx_family}\n")
    if (gfx_family < 0) {
        return
    }

    // create logical device + one graphics queue
    var priority = 1.0f
    var qci : VkDeviceQueueCreateInfo
    qci.sType = VkStructureType.DEVICE_QUEUE_CREATE_INFO
    qci.queueFamilyIndex = uint(gfx_family)
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(priority)
    }
    var dci : VkDeviceCreateInfo
    dci.sType = VkStructureType.DEVICE_CREATE_INFO
    dci.queueCreateInfoCount = 1u
    unsafe {
        dci.pQueueCreateInfos = addr(qci)
    }
    var device : VkDevice
    if (vkCreateDevice(phys, dci, null, device) != VkResult.SUCCESS) {
        print("device failed\n")
        return
    }
    volkLoadDevice(device)
    print("device created OK\n")

    var queue : VkQueue
    vkGetDeviceQueue(device, uint(gfx_family), 0u, queue)
    print("got queue (null? {queue == null})\n")

    vkDestroyDevice(device, null)
    vkDestroyInstance(instance, null)
    print("device probe OK\n")
}
