options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

[test]
def test_handles(t : T?) {
    t |> success(volkInitialize() == 0, "volkInitialize succeeds")
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))

    // create an owning image; weak_copy must be non-owning and must not disturb the owner
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = VkFormat.R8G8B8A8_UNORM,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = 32u
    ici.extent.height = 32u
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.color_attachment = true
    {
        var inscope img <- create_image(device, ici)
        t |> success(img._needs_delete, "created image owns its handle")
        t |> success(img._vk != 0ul, "created image has a valid handle")
        let w = weak_copy(img)
        t |> success(!w._needs_delete, "weak_copy is non-owning")
        t |> success(img._needs_delete, "weak_copy left the owner owning")
        t |> success(boost_value_to_vk(w) == boost_value_to_vk(img), "weak_copy refers to the same handle")
        // img finalizes here exactly once (vkDestroyImage); w does not
    }
    t |> success(true, "owning image created and finalized without crash")
}
