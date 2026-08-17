options gen2
options _comment_hygiene = true
options indenting = 4

// #52: the default debug-utils callback binding. Install vk_debug_callback()
// through VkDebugUtilsMessengerCreateInfoEXT (sType-prefilled ctor), create and
// destroy the messenger. Gated on VK_EXT_debug_utils -- the loader implements it,
// so it is present on CI lavapipe, but a missing extension soft-skips rather than
// crashing a bare environment.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

[test]
def test_debug_messenger(t : T?) {
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
    if (!instance_extension_available("VK_EXT_debug_utils")) {
        t |> success(true, "skipped: VK_EXT_debug_utils not available")
        return
    }
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u),
        ["VK_EXT_debug_utils"])
    volkLoadInstance(boost_value_to_vk(instance))

    var ci = VkDebugUtilsMessengerCreateInfoEXT()
    ci.messageSeverity.warning_ext = true
    ci.messageSeverity.error_ext = true
    ci.messageType.general_ext = true
    ci.messageType.validation_ext = true
    ci.messageType.performance_ext = true
    ci.pfnUserCallback = vk_debug_callback()
    t |> success(ci.pfnUserCallback != null, "vk_debug_callback yields a non-null C callback")

    var messenger : VkDebugUtilsMessengerEXT
    let rc = vkCreateDebugUtilsMessengerEXT(boost_value_to_vk(instance), ci, null, messenger)
    t |> success(rc == VkResult.SUCCESS, "debug-utils messenger created with the default callback")
    if (rc == VkResult.SUCCESS) {
        vkDestroyDebugUtilsMessengerEXT(boost_value_to_vk(instance), messenger, null)
    }
}
