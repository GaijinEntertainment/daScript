options gen2
options _comment_hygiene = true
options indenting = 4

// Shared asset loaders for the tutorials: file textures (via dasStbImage) and OBJ models (via the
// daslang glsl/geom_gen OBJ parser). Both upload onto GPU resources with the same staging path the
// cube tutorial uses inline. The actual asset FILES live in tutorials/_assets/ (all CC0).
//
// NOTE: this requires dasStbImage, which is a HARD dependency -- the loaders panic on decode failure.
// The lavapipe CI daslang build now builds dasModuleStbImage (added to the tests.yml --target line),
// so the texture tutorials' oracle gates load real maps in CI. (The recording/ folders stay CI-skipped
// because they also need audio.)

// NOT shared: requires geometry/geom_gen, a core daslang module that isn't shared (a shared module
// can only require shared ones). No shared module depends on vulkan_assets, so this is harmless.
module vulkan_assets

require vulkan public
require vulkan/vulkan_boost public
require stbimage/stbimage_boost
require geometry/geom_gen
require daslib/module_path
require daslib/fio
require math

// ===== asset path resolution =====

//! Absolute path to a subdirectory of ``tutorials/_assets``. Resolved relative to THIS module file,
//! which the dasVulkan ``.das_module`` mounts by ABSOLUTE path (``{project_path}/daslib/...``), so
//! ``get_this_module_dir()`` bakes an absolute path here and the result is CWD-independent — unlike
//! calling it from a tutorial file reached via a relative ``require`` (whose baked path is relative).
def public tutorial_asset_dir(sub : string) : string {
    // vulkan_assets.das lives at <root>/modules/dasVulkan/daslib/ ; the shared asset tree is
    // at <root>/tutorials/_assets/<sub> (same three-levels-up shape in the repo and the SDK).
    let root = dir_name(dir_name(dir_name(get_this_module_dir())))
    return path_join(path_join(root, "tutorials/_assets"), sub)
}

// ===== textures =====

//! A GPU texture: device-local image + its memory + a 2D view. Bind through a combined image sampler.
struct public Texture {
    image  : vulkan_handles::Image
    memory : DeviceMemory
    view   : ImageView
    width  : int
    height : int
}

def public finalize(var t : Texture) {
    unsafe {
        delete t.view
        delete t.image
        delete t.memory
    }
}

// Isolate the dasStbImage `Image` (clashes with vulkan's `Image` name) to this one helper: load a
// file as 4-channel data and hand back the raw bytes + dimensions. hdr=true loads float (bpc=4).
def private load_pixels(path : string; hdr : bool; var w, h : int&; var bytes : array<uint8>) : bool {
    var simg : stbimage_boost::Image
    let r = hdr ? simg.load_hdr(path, 4) : simg.load(path, 4)
    if (!r._0) {
        return false
    }
    w = simg.width
    h = simg.height
    delete bytes
    bytes <- simg.bytes
    return true
}

// Per-mip-range color image barrier — transition_image only covers a single whole-image level, so
// the mip-generation blit chain needs this range-aware form.
def private mip_barrier(cmd : CommandBuffer; image : vulkan_handles::Image; base, count : uint;
                        old_layout, new_layout : VkImageLayout; src_access, dst_access : VkAccessFlags;
                        src_stage, dst_stage : VkPipelineStageFlags) {
    var barrier = ImageMemoryBarrier(srcAccessMask = src_access, dstAccessMask = dst_access,
        oldLayout = old_layout, newLayout = new_layout, srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, image = weak_copy(image))
    barrier.subresourceRange.aspectMask.color = true
    barrier.subresourceRange.baseMipLevel = base
    barrier.subresourceRange.levelCount = count
    barrier.subresourceRange.layerCount = 1u
    let no_mem : array<MemoryBarrier>
    let no_buf : array<BufferMemoryBarrier>
    var imgs : array<ImageMemoryBarrier>
    imgs |> emplace(barrier)
    let no_dep : VkDependencyFlags
    cmd_pipeline_barrier(cmd, src_stage, dst_stage, no_dep, no_mem, no_buf, imgs)
}

//! Upload already-decoded pixel `bytes` (w*h*`fmt`) as a sampled device-local image. When `gen_mips`
//! is true a full mip chain is built on the GPU via a linear blit cascade (kills minification moiré
//! on detailed textures); the caller's sampler should then use a high maxLod.
def public upload_texture(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool;   // nolint:STYLE038 - flat staging-upload sequence
                          w, h : int; fmt : VkFormat; var bytes : array<uint8>; gen_mips : bool = true) : Texture {
    let dev = boost_value_to_vk(device)
    let mip_levels = gen_mips ? uint(floor(log2(float(max(w, h))))) + 1u : 1u
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = mip_levels, arrayLayers = 1u,
        tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(w)
    ici.extent.height = uint(h)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    ici.usage.transfer_src = true       // mip 0 is the blit SOURCE for level 1, etc.
    var inscope image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(image), boost_value_to_vk(memory), 0ul), null)

    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = fmt)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = mip_levels
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)

    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer_from_bytes(device, phys, staging_usage, bytes)
    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var to_write : VkAccessFlags
        to_write.transfer_write = true
        var to_tread : VkAccessFlags
        to_tread.transfer_read = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag_stage : VkPipelineStageFlags
        frag_stage.fragment_shader = true

        // all mips UNDEFINED -> TRANSFER_DST, then fill mip 0 from the staging buffer
        mip_barrier(cmd, image, 0u, mip_levels, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_write, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = uint(w)
        region.imageExtent.height = uint(h)
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)

        // blit cascade: level i is half of level i-1; each source level ends in SHADER_READ_ONLY
        var mw = w
        var mh = h
        for (i in range(1, int(mip_levels))) {
            let nw = max(mw / 2, 1)
            let nh = max(mh / 2, 1)
            mip_barrier(cmd, image, uint(i - 1), 1u, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
                to_write, to_tread, xfer, xfer)
            var blit : VkImageBlit
            blit.srcSubresource.aspectMask.color = true
            blit.srcSubresource.mipLevel = uint(i - 1)
            blit.srcSubresource.layerCount = 1u
            blit.srcOffsets[1].x = mw
            blit.srcOffsets[1].y = mh
            blit.srcOffsets[1].z = 1
            blit.dstSubresource.aspectMask.color = true
            blit.dstSubresource.mipLevel = uint(i)
            blit.dstSubresource.layerCount = 1u
            blit.dstOffsets[1].x = nw
            blit.dstOffsets[1].y = nh
            blit.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd), boost_value_to_vk(image), VkImageLayout.TRANSFER_SRC_OPTIMAL,
                    boost_value_to_vk(image), VkImageLayout.TRANSFER_DST_OPTIMAL, 1u, addr(blit), VkFilter.LINEAR)
            }
            mip_barrier(cmd, image, uint(i - 1), 1u, VkImageLayout.TRANSFER_SRC_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
                to_tread, to_read, xfer, frag_stage)
            mw = nw
            mh = nh
        }
        // the last level was never a blit source — it is still TRANSFER_DST
        mip_barrier(cmd, image, mip_levels - 1u, 1u, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_write, to_read, xfer, frag_stage)
    }
    return <- Texture(image <- image, memory <- memory, view <- view, width = w, height = h)
}

//! Load an 8-bit file texture (PNG/JPG) as a sampled image. `srgb` picks R8G8B8A8_SRGB (albedo) vs
//! _UNORM (data maps: normal / roughness / AO). Panics if the file can't be decoded.
def public load_texture_2d(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool;
                           path : string; srgb : bool = false) : Texture {
    var w, h : int
    var bytes : array<uint8>
    if (!load_pixels(path, false, w, h, bytes)) {
        panic("load_texture_2d: cannot decode '{path}'")
    }
    let fmt = srgb ? VkFormat.R8G8B8A8_SRGB : VkFormat.R8G8B8A8_UNORM
    var tex <- upload_texture(device, phys, queue, pool, w, h, fmt, bytes)
    delete bytes
    return <- tex
}

//! Load a float (HDR) equirectangular image as an R32G32B32A32_SFLOAT sampled image (IBL / env).
def public load_texture_hdr(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool;
                            path : string) : Texture {
    var w, h : int
    var bytes : array<uint8>
    if (!load_pixels(path, true, w, h, bytes)) {
        panic("load_texture_hdr: cannot decode '{path}'")
    }
    var tex <- upload_texture(device, phys, queue, pool, w, h, VkFormat.R32G32B32A32_SFLOAT, bytes, false)
    delete bytes
    return <- tex
}

// ===== OBJ models =====

//! A GPU mesh: interleaved vertex buffer (pos float3 @0, normal float3 @12, uv float2 @24 -- 32 bytes)
//! + a uint32 index buffer, plus the model-space bounds for fitting.
struct public Mesh {
    vbuf        : HostBuffer
    ibuf        : HostBuffer
    index_count : int
    bmin        : float3
    bmax        : float3
}

def public finalize(var m : Mesh) {
    unsafe {
        delete m.ibuf
        delete m.vbuf
    }
}

//! Load an OBJ model into vertex + index buffers via the daslang glsl/geom_gen parser (positions +
//! normals + texcoords, interleaved 32-byte vertices). `dir` is the search directory for the file.
def public load_obj_model(device : Device; phys : VkPhysicalDevice; fname, dir : string) : Mesh {
    var frag <- load_obj_mesh(fname, dir, dir, false)
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var inscope vbuf <- create_host_buffer_from_bytes(device, phys, vb_usage, frag.vertices)
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var inscope ibuf <- create_host_buffer_from_bytes(device, phys, ib_usage, frag.indices)
    let n = length(frag.indices)
    let bmin = frag.bmin
    let bmax = frag.bmax
    delete frag
    return <- Mesh(vbuf <- vbuf, ibuf <- ibuf, index_count = n, bmin = bmin, bmax = bmax)
}
