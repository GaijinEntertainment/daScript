options gen2
options _comment_hygiene = true

require vulkan

[export]
def main() {
    let rc = volkInitialize()
    if (rc != 0) {
        print("volkInitialize failed: {rc} (no Vulkan loader / driver on this machine)\n")
        return
    }
    let ver = volkGetInstanceVersion()
    print("Vulkan instance version: {int((ver >> 22u) & 0x7Fu)}.{int((ver >> 12u) & 0x3FFu)}.{int(ver & 0xFFFu)}\n")

    var app_info : VkApplicationInfo
    app_info.sType = VkStructureType.APPLICATION_INFO
    app_info.apiVersion = (1u << 22u) | (0u << 12u)
    app_info.pApplicationName = "dasVulkan smoke"

    var ci : VkInstanceCreateInfo
    ci.sType = VkStructureType.INSTANCE_CREATE_INFO
    unsafe {
        ci.pApplicationInfo = addr(app_info)
    }

    var instance : VkInstance
    let crc = vkCreateInstance(ci, null, instance)
    if (crc != VkResult.SUCCESS) {
        print("vkCreateInstance failed: {crc}\n")
        return
    }
    print("instance created OK\n")
    volkLoadInstance(instance)

    var count = 0u
    vkEnumeratePhysicalDevices(instance, count, null)
    print("physical devices: {int(count)}\n")

    if (count > 0u) {
        var devices : array<VkPhysicalDevice>
        devices |> resize(int(count))
        unsafe {
            vkEnumeratePhysicalDevices(instance, count, addr(devices[0]))
        }
        for (d in devices) {
            var props : VkPhysicalDeviceProperties
            vkGetPhysicalDeviceProperties(d, props)
            var name = ""
            unsafe {
                name = "{reinterpret<string>(addr(props.deviceName[0]))}"
            }
            print("  device: {name} (api {int((props.apiVersion >> 22u) & 0x7Fu)}.{int((props.apiVersion >> 12u) & 0x3FFu)})\n")
        }
    }

    vkDestroyInstance(instance, null)
    print("smoke OK\n")
}
