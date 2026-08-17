options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 12 host -- GPU-driven rendering: the GPU decides what to draw.
//
// The frame is a five-pass chain, all recorded into one command buffer:
//   1. occluder depth pre-pass  -- render the moving wall into a D32 depth image (camera view)
//   2. HZB build (compute)      -- copy depth into HZB mip0, then max-downsample the pyramid
//   3. cull (compute)           -- per instance: frustum + Hi-Z occlusion test; survivors append an
//                                  indirect draw command + a GPU-written count; all get a cull_reason
//   4. main view (left panel)   -- draw the occluder + survivors via cmd_draw_indexed_indirect_count
//   5. god view (right panel)   -- draw ALL N instances; cull_reason tints the skipped ones as ghosts
//
// The barrier chain between these passes is the real correctness risk -- verify it in RenderDoc, not
// by trusting a green fence-per-frame test (the sync-blindspot lesson). Both views render into one
// wide offscreen target via dynamic viewport + scissor.

module gpu_driven_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require gpu_driven_tut_shaders public
require math

// ===== dimensions =====
let public PANEL   = 512                 // each view is a square panel
let public OUT_W   = PANEL * 2           // side-by-side: camera | god
let public OUT_H   = PANEL
let HZB_BASE       = 512                 // power-of-two HZB base (matches the camera panel)
let MIP_COUNT      = 7                   // 512,256,128,64,32,16,8 -- every level is a multiple of 8
let OUT_FMT        = VkFormat.R8G8B8A8_UNORM
let DEPTH_FMT      = VkFormat.D32_SFLOAT
let HZB_FMT        = VkFormat.R32_SFLOAT

let public GRID_N      = 12
let public N_INSTANCES = GRID_N * GRID_N // 144 cubes
// N_MAT (= 6) comes from gpu_driven_tut_shaders (the bindless sampler2D[N_MAT] array size).
let N_INDICES          = 36
let VERT_STRIDE        = 32u             // pos(12) + normal(12) + uv(8)
let INST_STRIDE        = 32u             // pos_scale(16) + mat_idx(4) + pad(12)
let DRAWCMD_STRIDE     = 20u             // VkDrawIndexedIndirectCommand
let SCENE_BYTES        = 160ul          // 2*float4x4 + 2*float4

// the wall's pinned X for the deterministic oracle; the animated viewers sweep it.
let public WALL_PIN_X = 0.0f

// ===== cube geometry (24 verts: pos + faceted normal + uv; 36 indices) =====

let private cube_vertices : array<float> <- [
    // +Z face
    -0.5, -0.5,  0.5,  0.0, 0.0, 1.0,  0.0, 0.0,
     0.5, -0.5,  0.5,  0.0, 0.0, 1.0,  1.0, 0.0,
     0.5,  0.5,  0.5,  0.0, 0.0, 1.0,  1.0, 1.0,
    -0.5,  0.5,  0.5,  0.0, 0.0, 1.0,  0.0, 1.0,
    // -Z face
     0.5, -0.5, -0.5,  0.0, 0.0, -1.0,  0.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 0.0, -1.0,  1.0, 0.0,
    -0.5,  0.5, -0.5,  0.0, 0.0, -1.0,  1.0, 1.0,
     0.5,  0.5, -0.5,  0.0, 0.0, -1.0,  0.0, 1.0,
    // +X face
     0.5, -0.5,  0.5,  1.0, 0.0, 0.0,  0.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 0.0, 0.0,  1.0, 0.0,
     0.5,  0.5, -0.5,  1.0, 0.0, 0.0,  1.0, 1.0,
     0.5,  0.5,  0.5,  1.0, 0.0, 0.0,  0.0, 1.0,
    // -X face
    -0.5, -0.5, -0.5, -1.0, 0.0, 0.0,  0.0, 0.0,
    -0.5, -0.5,  0.5, -1.0, 0.0, 0.0,  1.0, 0.0,
    -0.5,  0.5,  0.5, -1.0, 0.0, 0.0,  1.0, 1.0,
    -0.5,  0.5, -0.5, -1.0, 0.0, 0.0,  0.0, 1.0,
    // +Y face
    -0.5,  0.5,  0.5,  0.0, 1.0, 0.0,  0.0, 0.0,
     0.5,  0.5,  0.5,  0.0, 1.0, 0.0,  1.0, 0.0,
     0.5,  0.5, -0.5,  0.0, 1.0, 0.0,  1.0, 1.0,
    -0.5,  0.5, -0.5,  0.0, 1.0, 0.0,  0.0, 1.0,
    // -Y face
    -0.5, -0.5, -0.5,  0.0, -1.0, 0.0,  0.0, 0.0,
     0.5, -0.5, -0.5,  0.0, -1.0, 0.0,  1.0, 0.0,
     0.5, -0.5,  0.5,  0.0, -1.0, 0.0,  1.0, 1.0,
    -0.5, -0.5,  0.5,  0.0, -1.0, 0.0,  0.0, 1.0
]

let private cube_indices : array<uint16> <- [
    uint16(0),  uint16(1),  uint16(2),   uint16(0),  uint16(2),  uint16(3),
    uint16(4),  uint16(5),  uint16(6),   uint16(4),  uint16(6),  uint16(7),
    uint16(8),  uint16(9),  uint16(10),  uint16(8),  uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14),  uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18),  uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22),  uint16(20), uint16(22), uint16(23)
]

// ===== matrices (column-major, Vulkan Z[0,1] + Y flip) =====

def private look_at_m4(eye, target, up : float3) : float4x4 {
    let f = normalize(target - eye)
    let r = normalize(cross(f, up))
    let u = cross(r, f)
    var m : float4x4
    m[0] = float4(r.x,  u.x, -f.x, 0.0)
    m[1] = float4(r.y,  u.y, -f.y, 0.0)
    m[2] = float4(r.z,  u.z, -f.z, 0.0)
    m[3] = float4(-dot(r, eye), -dot(u, eye), dot(f, eye), 1.0)
    return m
}

def private perspective_m4(fovy, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0 / tan(fovy * 0.5)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0, 0.0, 0.0)
    m[1] = float4(0.0, -f, 0.0, 0.0)
    m[2] = float4(0.0, 0.0, zfar / (znear - zfar), -1.0)
    m[3] = float4(0.0, 0.0, (zfar * znear) / (znear - zfar), 0.0)
    return m
}

//! translate * scale (no rotation): the axis-aligned model matrix for one cube / the wall slab.
def private model_ts(tx, ty, tz, sx, sy, sz : float) : float4x4 {
    var m : float4x4
    m[0] = float4(sx, 0.0, 0.0, 0.0)
    m[1] = float4(0.0, sy, 0.0, 0.0)
    m[2] = float4(0.0, 0.0, sz, 0.0)
    m[3] = float4(tx, ty, tz, 1.0)
    return m
}

// ===== camera / wall placement (shared by the host, viewers, and oracle) =====

def public camera_view_proj() : float4x4 {
    let view = look_at_m4(float3(0.0, 0.0, 9.0), float3(0.0, 0.0, 0.0), float3(0.0, 1.0, 0.0))
    let proj = perspective_m4(0.9, 1.0, 0.1, 40.0)
    return proj * view
}

def public god_view_proj() : float4x4 {
    let view = look_at_m4(float3(13.0, 6.0, 11.0), float3(0.0, 0.0, -1.0), float3(0.0, 1.0, 0.0))
    let proj = perspective_m4(0.9, 1.0, 0.1, 60.0)
    return proj * view
}

//! the wall slab model matrix for a given X (animated viewers sweep `wall_x`; the oracle pins it). Wide
//! enough (half-width 2) that whole columns of the field sit fully behind it: the conservative max-
//! downsampled HZB culls only a cube whose ENTIRE footprint is covered, so an edge-thin wall never would.
def public wall_model(wall_x : float) : float4x4 {
    return model_ts(wall_x, 0.0, 2.5, 3.2, 16.0, 0.5)
}

//! World center of instance `i` -- the single source of truth shared by the instance-buffer builder
//! and the oracle (so the test projects the exact cubes the GPU placed).
def public instance_center(i : int) : float3 {
    let gx = i % GRID_N
    let gy = i / GRID_N
    let x = (float(gx) / float(GRID_N - 1) - 0.5f) * 14.0f
    let y = (float(gy) / float(GRID_N - 1) - 0.5f) * 14.0f
    let z = -3.0f + 0.6f * sin(float(gx) * 1.3f + float(gy) * 0.7f)
    return float3(x, y, z)
}

//! Project a world point through `vp` to a pixel in panel `p` (0 = camera/left, 1 = god/right).
def public project_panel(vp : float4x4; world : float3; p : int) : int2 {
    let clip = vp * float4(world, 1.0f)
    let ndc = clip.xyz / clip.w
    let x = int((ndc.x * 0.5f + 0.5f) * float(PANEL)) + p * PANEL
    let y = int((ndc.y * 0.5f + 0.5f) * float(PANEL))
    return int2(x, y)
}

// ===== instance scene =====

//! float bit pattern as uint, for packing the std430 instance buffer.
def private f2u(f : float) : uint {
    return unsafe(reinterpret<uint>(f))
}

//! Build the N_INSTANCES x 8-word (32-byte) std430 instance buffer: a GRID_N x GRID_N field of cubes
//! spread in X/Y at a depth behind the wall, with a gentle per-cube Z ripple so the field is not a
//! flat plane. Each cube picks one of N_MAT bindless materials.
def private gen_instance_words() : array<uint> {
    var w : array<uint>
    w |> resize(N_INSTANCES * 8)
    for (i in range(N_INSTANCES)) {
        let c = instance_center(i)
        let base = i * 8
        w[base + 0] = f2u(c.x)
        w[base + 1] = f2u(c.y)
        w[base + 2] = f2u(c.z)
        w[base + 3] = f2u(0.45f)                            // uniform scale
        w[base + 4] = uint(i % N_MAT)                       // mat_idx
        w[base + 5] = 0u
        w[base + 6] = 0u
        w[base + 7] = 0u
    }
    return <- w
}

// ===== device: GPU-driven feature set =====

// multiDrawIndirect (base) + drawIndirectCount (1.2) + shaderDrawParameters (1.1, for gl_DrawID) +
// descriptorIndexing + shaderSampledImageArrayNonUniformIndexing (1.2, for the bindless sampler2D[]).
// Models create_device_descriptor_indexing / create_device_draw_parameters: 1.1 + 1.2 + base, one pNext.
def public create_device_gpu_driven(phys : VkPhysicalDevice; queue_family : uint) : Device {
    let no_ext : array<string>
    return <- create_device_gpu_driven(phys, queue_family, no_ext)
}

//! Probe for ``drawIndirectCount``, the one feature here that the bindless / draw-parameter tests don't
//! already exercise on CI's lavapipe (they prove descriptorIndexing + multiDrawIndirect + shaderDrawParameters).
//! Gates the indirect-count path so the oracle soft-skips rather than panicking. Mirrors ``mesh_shader_supported``.
def public draw_indirect_count_available() : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("dasVulkan tutorial 12 probe", make_api_version(1u, 2u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        return false
    }
    let phys = select_physical_device(instance)
    var f12 = VkPhysicalDeviceVulkan12Features()
    f12.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_2_FEATURES
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f12)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return f12.drawIndirectCount != 0u
}

//! Extensions overload -- the windowed viewer passes `["VK_KHR_swapchain"]`.
def public create_device_gpu_driven(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    var base : VkPhysicalDeviceFeatures
    base.multiDrawIndirect = 1u
    var f11 : VkPhysicalDeviceVulkan11Features
    f11.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_1_FEATURES
    f11.shaderDrawParameters = 1u
    var f12 : VkPhysicalDeviceVulkan12Features
    f12.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_2_FEATURES
    f12.drawIndirectCount = 1u
    f12.descriptorIndexing = 1u
    f12.shaderSampledImageArrayNonUniformIndexing = 1u
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    var h : VkDevice
    unsafe {
        qci.pQueuePriorities = addr(prio)
        f12.pNext = addr<void?>(f11)           // chain: ci -> f12 -> f11
        ci.pNext = addr<void?>(f12)
        ci.pEnabledFeatures = addr(base)
        ci.pQueueCreateInfos = addr(qci)
        if (!empty(extensions)) {
            ci.enabledExtensionCount = uint(length(extensions))
            ci.ppEnabledExtensionNames = addr<void?>(extensions[0])
        }
        vk_check(vkCreateDevice(phys, ci, null, h), null)
    }
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

// ===== bindless material textures =====

struct private Tex {
    image  : Image
    memory : DeviceMemory
    view   : ImageView
}

def private finalize(var t : Tex) {
    delete t.view
    delete t.image
    delete t.memory
}

//! An 8x8 procedural material texture: a base hue with a 2x2 checker modulation so the cube UVs read
//! as textured (not flat) in the recording. `m` in [0, N_MAT) selects the hue.
def private gen_material_bytes(m : int) : array<uint8> {
    let hue = float(m) / float(N_MAT) * 6.2831f
    let br = uint8(clamp(int((0.5f + 0.5f * cos(hue)) * 255.0f), 0, 255))
    let bg = uint8(clamp(int((0.5f + 0.5f * cos(hue + 2.094f)) * 255.0f), 0, 255))
    let bb = uint8(clamp(int((0.5f + 0.5f * cos(hue + 4.188f)) * 255.0f), 0, 255))
    var b : array<uint8>
    b |> resize(8 * 8 * 4)
    for (y in range(8)) {
        for (x in range(8)) {
            let checker = ((x / 4) + (y / 4)) % 2 == 0
            let k = checker ? 255 : 150
            let p = (y * 8 + x) * 4
            b[p + 0] = uint8(int(br) * k / 255)
            b[p + 1] = uint8(int(bg) * k / 255)
            b[p + 2] = uint8(int(bb) * k / 255)
            b[p + 3] = 255u8
        }
    }
    return <- b
}

def private create_material(device : Device; phys : VkPhysicalDevice; queue : VkQueue;
                            pool : CommandPool; m : int) : Tex {
    let dev = boost_value_to_vk(device)
    let fmt = VkFormat.R8G8B8A8_UNORM
    var bytes <- gen_material_bytes(m)
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer(device, phys, uint64(long_length(bytes)), staging_usage)
    upload_bytes(staging, device, bytes)

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = 8u
    ici.extent.height = 8u
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var img <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(img), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var mem <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(img), boost_value_to_vk(mem), 0ul), null)

    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag : VkPipelineStageFlags
        frag.fragment_shader = true
        transition_image(cmd, img, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = 8u
        region.imageExtent.height = 8u
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, img, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, img, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag)
    }
    delete bytes

    var tvci = ImageViewCreateInfo(image = weak_copy(img), viewType = VkImageViewType._2D, format = fmt)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var view <- create_image_view(device, tvci)
    return <- Tex(image <- img, memory <- mem, view <- view)
}

// ===== HZB image: R32f, MIP_COUNT levels, per-mip storage views + one full-chain sampled view =====

struct private Hzb {
    image       : Image
    memory      : DeviceMemory
    sampled     : ImageView            // full chain (levelCount = MIP_COUNT), read by cull + downsample src
    mip_views   : array<ImageView>     // one STORAGE view per mip level (the imageStore target)
}

def private finalize(var h : Hzb) {
    delete h.mip_views
    delete h.sampled
    delete h.image
    delete h.memory
}

def private build_hzb(device : Device; phys : VkPhysicalDevice) : Hzb {
    let dev = boost_value_to_vk(device)
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = HZB_FMT,
        mipLevels = uint(MIP_COUNT), arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(HZB_BASE)
    ici.extent.height = uint(HZB_BASE)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.storage = true
    ici.usage.sampled = true
    var img <- create_image(device, ici)
    var req : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(img), req)
    var want : VkMemoryPropertyFlags
    want.device_local = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var mem <- allocate_memory(device, mai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(img), boost_value_to_vk(mem), 0ul), null)

    var svci = ImageViewCreateInfo(image = weak_copy(img), viewType = VkImageViewType._2D, format = HZB_FMT)
    svci.subresourceRange.aspectMask.color = true
    svci.subresourceRange.levelCount = uint(MIP_COUNT)
    svci.subresourceRange.layerCount = 1u
    var sampled <- create_image_view(device, svci)

    var mip_views : array<ImageView>
    mip_views |> reserve(MIP_COUNT)
    for (l in range(MIP_COUNT)) {
        var mvci = ImageViewCreateInfo(image = weak_copy(img), viewType = VkImageViewType._2D, format = HZB_FMT)
        mvci.subresourceRange.aspectMask.color = true
        mvci.subresourceRange.baseMipLevel = uint(l)
        mvci.subresourceRange.levelCount = 1u
        mvci.subresourceRange.layerCount = 1u
        var v <- create_image_view(device, mvci)
        mip_views |> emplace(v)
    }
    return <- Hzb(image <- img, memory <- mem, sampled <- sampled, mip_views <- mip_views)
}

// ===== descriptor helpers =====

def private alloc_one_set(device : Device; pool : DescriptorPool; layout : DescriptorSetLayout) : VkDescriptorSet {
    let dev = boost_value_to_vk(device)
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(pool)
    dsai.descriptorSetCount = 1u
    var raw_layout = boost_value_to_vk(layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }
    return raw_set
}

def private write_ubo(set : VkDescriptorSet; binding : uint; buf : Buffer; size : uint64) : WriteDescriptorSet {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set), dstBinding = binding,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    let bi = DescriptorBufferInfo(buffer = weak_copy(buf), range_ = size)
    w.pBufferInfo |> push(bi)
    return <- w
}

def private write_ssbo(set : VkDescriptorSet; binding : uint; buf : Buffer; size : uint64) : WriteDescriptorSet {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set), dstBinding = binding,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    let bi = DescriptorBufferInfo(buffer = weak_copy(buf), range_ = size)
    w.pBufferInfo |> push(bi)
    return <- w
}

def private write_sampler(set : VkDescriptorSet; binding : uint; view : ImageView; sampler : Sampler;
                          layout : VkImageLayout) : WriteDescriptorSet {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set), dstBinding = binding,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let ii = DescriptorImageInfo(sampler = weak_copy(sampler), imageView = weak_copy(view), imageLayout = layout)
    w.pImageInfo |> push(ii)
    return <- w
}

def private write_storage_image(set : VkDescriptorSet; binding : uint; view : ImageView) : WriteDescriptorSet {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set), dstBinding = binding,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    let ii = DescriptorImageInfo(imageView = weak_copy(view), imageLayout = VkImageLayout.GENERAL)
    w.pImageInfo |> push(ii)
    return <- w
}

//! the bindless sampler2D[N_MAT] write: one binding, descriptorCount = N_MAT, N image-infos sharing
//! one sampler (the canonical bindless-array write -- cf. tests/integration/test_bindless_*.das).
def private write_sampler_array(set : VkDescriptorSet; binding : uint; var views : array<ImageView>;
                                sampler : Sampler) : WriteDescriptorSet {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set), dstBinding = binding,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = uint(length(views)))
    w.pImageInfo |> reserve(length(views))
    for (v in views) {
        let ii = DescriptorImageInfo(sampler = weak_copy(sampler), imageView = weak_copy(v),
            imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
        w.pImageInfo |> push(ii)
    }
    return <- w
}

// ===== pipeline builders =====

//! depth-only pipeline (vertex stage only, no color attachment) for the occluder pre-pass. Reads the
//! cube vertex buffer's position attribute (location 0); normal/uv are ignored.
def private build_depth_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;
                                 vert : ShaderModule) : Pipeline {
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.vertex = true
    stage.module_ = boost_value_to_vk(vert)
    stage.pName = "main"

    var binding = VkVertexInputBindingDescription(binding = 0u, stride = VERT_STRIDE, inputRate = VkVertexInputRate.VERTEX)
    var a_pos = VkVertexInputAttributeDescription(location = 0u, binding = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 0u)
    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = 1u
    vinput.vertexAttributeDescriptionCount = 1u
    var ia = VkPipelineInputAssemblyStateCreateInfo()
    ia.topology = VkPrimitiveTopology.TRIANGLE_LIST
    var viewport = VkViewport(width = float(HZB_BASE), height = float(HZB_BASE), maxDepth = 1.0f)
    var scissor : VkRect2D
    scissor.extent.width = uint(HZB_BASE)
    scissor.extent.height = uint(HZB_BASE)
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u
    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL
    rast.cullMode.back = true
    rast.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    rast.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true
    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = VK_TRUE
    depthstate.depthCompareOp = VkCompareOp.LESS
    var blend = VkPipelineColorBlendStateCreateInfo()       // attachmentCount = 0
    var h : VkPipeline
    unsafe {
        var gp = VkGraphicsPipelineCreateInfo()
        gp.stageCount = 1u
        gp.pStages = addr(stage)
        vinput.pVertexBindingDescriptions = addr(binding)
        vinput.pVertexAttributeDescriptions = addr(a_pos)
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(ia)
        vp.pViewports = addr(viewport)
        vp.pScissors = addr(scissor)
        gp.pViewportState = addr(vp)
        gp.pRasterizationState = addr(rast)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! full graphics pipeline (vertex+fragment, color+depth, dynamic viewport) with caller-chosen vertex
//! input, depth-write and alpha-blend. Covers all four color-pass pipelines: main + occluder-solid (blend
//! off), god + occluder-ghost (blend on; ghost skips depth-write so the wall doesn't stamp over cubes).
def private build_color_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                                 vert, frag : ShaderModule;
                                 var vbindings : array<VkVertexInputBindingDescription>;
                                 var vattrs : array<VkVertexInputAttributeDescription>;
                                 depth_write, blend_enable : bool) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = uint(length(vbindings))
    vinput.vertexAttributeDescriptionCount = uint(length(vattrs))
    var ia = VkPipelineInputAssemblyStateCreateInfo()
    ia.topology = VkPrimitiveTopology.TRIANGLE_LIST
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u
    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL
    rast.cullMode.back = true
    rast.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    rast.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true
    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = depth_write ? VK_TRUE : VK_FALSE
    depthstate.depthCompareOp = VkCompareOp.LESS_OR_EQUAL
    depthstate.maxDepthBounds = 1.0f

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.blendEnable = blend_enable ? VK_TRUE : VK_FALSE
    blend_att.srcColorBlendFactor = VkBlendFactor.SRC_ALPHA
    blend_att.dstColorBlendFactor = VkBlendFactor.ONE_MINUS_SRC_ALPHA
    blend_att.colorBlendOp = VkBlendOp.ADD
    blend_att.srcAlphaBlendFactor = VkBlendFactor.ONE
    blend_att.dstAlphaBlendFactor = VkBlendFactor.ZERO
    blend_att.alphaBlendOp = VkBlendOp.ADD
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states <- [VkDynamicState.VIEWPORT, VkDynamicState.SCISSOR]
    var dynstate = VkPipelineDynamicStateCreateInfo()
    dynstate.dynamicStateCount = 2u

    var h : VkPipeline
    unsafe {
        if (!empty(vbindings)) {
            vinput.pVertexBindingDescriptions = addr(vbindings[0])
        }
        if (!empty(vattrs)) {
            vinput.pVertexAttributeDescriptions = addr(vattrs[0])
        }
        dynstate.pDynamicStates = addr(dyn_states[0])
        blend.pAttachments = addr(blend_att)
        var gp = VkGraphicsPipelineCreateInfo()
        gp.stageCount = 2u
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(ia)
        gp.pViewportState = addr(vp)
        gp.pRasterizationState = addr(rast)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        gp.pColorBlendState = addr(blend)
        gp.pDynamicState = addr(dynstate)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete stages
    delete dyn_states
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! cube vertex-input (pos + normal + uv) shared by main + god pipelines.
def private cube_vbindings() : array<VkVertexInputBindingDescription> {
    return <- [VkVertexInputBindingDescription(binding = 0u, stride = VERT_STRIDE, inputRate = VkVertexInputRate.VERTEX)]
}
def private cube_vattrs() : array<VkVertexInputAttributeDescription> {
    return <- [
        VkVertexInputAttributeDescription(location = 0u, binding = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 0u),
        VkVertexInputAttributeDescription(location = 1u, binding = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 12u),
        VkVertexInputAttributeDescription(location = 2u, binding = 0u, format = VkFormat.R32G32_SFLOAT,    offset = 24u)
    ]
}
//! occluder vertex-input (position only; occluder_vs ignores normal/uv).
def private occ_vbindings() : array<VkVertexInputBindingDescription> {
    return <- [VkVertexInputBindingDescription(binding = 0u, stride = VERT_STRIDE, inputRate = VkVertexInputRate.VERTEX)]
}
def private occ_vattrs() : array<VkVertexInputAttributeDescription> {
    return <- [VkVertexInputAttributeDescription(location = 0u, binding = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 0u)]
}

// ===== resources =====

struct public GpuDrivenResources {
    out_color : OffscreenTarget        // 1024x512 side-by-side target (transfer_src for readback)
    out_depth : OffscreenDepth         // shared depth for both viewports
    occ_depth : OffscreenDepth         // occluder depth pre-pass (sampled by HZB mip0)
    hzb       : Hzb

    rp_depth  : RenderPass             // depth-only pre-pass
    rp_color  : RenderPass             // color+depth, both viewports
    fb_depth  : Framebuffer
    fb_color  : Framebuffer

    depth_sampler : Sampler            // samples occ_depth + the HZB
    mat_sampler   : Sampler            // bindless material sampler
    materials     : array<Tex>         // N_MAT textures

    cube_vb   : HostBuffer
    cube_ib   : HostBuffer
    ubo       : HostBuffer
    instances : HostBuffer
    draw_cmds : HostBuffer
    draw_count : HostBuffer
    draw_inst : HostBuffer
    cull_reason : HostBuffer

    // cull (compute)
    cull_layouts : array<DescriptorSetLayout>
    cull_pl      : PipelineLayout
    cull_pipe    : Pipeline
    cull_set     : VkDescriptorSet
    // hzb build (compute)
    hzb_layouts  : array<DescriptorSetLayout>
    hzb_pl       : PipelineLayout
    mip0_pipe    : Pipeline
    down_pipe    : Pipeline
    hzb_sets     : array<VkDescriptorSet>   // MIP_COUNT
    // occluder (graphics, push-only)
    occ_layouts  : array<DescriptorSetLayout>
    occ_pl       : PipelineLayout
    depth_pipe   : Pipeline
    occ_solid_pipe : Pipeline
    occ_ghost_pipe : Pipeline
    // main view (graphics)
    main_layouts : array<DescriptorSetLayout>
    main_pl      : PipelineLayout
    main_pipe    : Pipeline
    main_set     : VkDescriptorSet
    // god view (graphics)
    god_layouts  : array<DescriptorSetLayout>
    god_pl       : PipelineLayout
    god_pipe     : Pipeline
    god_set      : VkDescriptorSet

    desc_pool : DescriptorPool
    readback  : HostBuffer
    buf_size  : uint64
}

def public finalize(var r : GpuDrivenResources) {
    unsafe {
        delete r.readback
        delete r.desc_pool
        delete r.god_pipe
        delete r.god_pl
        delete r.god_layouts
        delete r.main_pipe
        delete r.main_pl
        delete r.main_layouts
        delete r.occ_ghost_pipe
        delete r.occ_solid_pipe
        delete r.depth_pipe
        delete r.occ_pl
        delete r.occ_layouts
        delete r.down_pipe
        delete r.mip0_pipe
        delete r.hzb_pl
        delete r.hzb_layouts
        delete r.cull_pipe
        delete r.cull_pl
        delete r.cull_layouts
        delete r.cull_reason
        delete r.draw_inst
        delete r.draw_count
        delete r.draw_cmds
        delete r.instances
        delete r.ubo
        delete r.cube_ib
        delete r.cube_vb
        delete r.materials
        delete r.mat_sampler
        delete r.depth_sampler
        delete r.fb_color
        delete r.fb_depth
        delete r.rp_color
        delete r.rp_depth
        delete r.hzb
        delete r.occ_depth
        delete r.out_depth
        delete r.out_color
    }
}

struct public GpuDrivenContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : GpuDrivenResources
}

def public finalize(var ctx : GpuDrivenContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

// ===== build =====

def public build_gpu_driven_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : GpuDrivenResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    // ----- offscreen targets -----
    var samples1 : VkSampleCountFlags
    samples1._1 = true
    var color_extra : VkImageUsageFlags
    color_extra.transfer_src = true
    var inscope out_color <- build_offscreen_target(device, phys, OUT_W, OUT_H, OUT_FMT, samples1, color_extra)
    let depth_extra : VkImageUsageFlags
    var inscope out_depth <- build_offscreen_depth(device, phys, OUT_W, OUT_H, DEPTH_FMT, depth_extra)
    var occ_extra : VkImageUsageFlags
    occ_extra.sampled = true                         // HZB mip0 samples the occluder depth
    var inscope occ_depth <- build_offscreen_depth(device, phys, HZB_BASE, HZB_BASE, DEPTH_FMT, occ_extra)
    var inscope hzb <- build_hzb(device, phys)

    // ----- render passes -----
    var inscope rp_depth <- create_render_pass_depth_only(device, DEPTH_FMT, VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL)
    var inscope rp_color <- create_render_pass_color_depth(device, OUT_FMT, DEPTH_FMT, VkImageLayout.TRANSFER_SRC_OPTIMAL)

    // ----- framebuffers -----
    var depth_attach : array<ImageView>
    depth_attach |> push(weak_copy(occ_depth.view))
    var inscope fb_depth <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(rp_depth), pAttachments <- depth_attach,
        width = uint(HZB_BASE), height = uint(HZB_BASE), layers = 1u))
    var color_attach <- [weak_copy(out_color.view), weak_copy(out_depth.view)]
    var inscope fb_color <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(rp_color), pAttachments <- color_attach,
        width = uint(OUT_W), height = uint(OUT_H), layers = 1u))

    // ----- samplers -----
    let dsci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST, addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE, addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE,
        maxLod = float(MIP_COUNT))
    var inscope depth_sampler <- create_sampler(device, dsci)
    let msci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST, addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT, maxLod = 1.0f)
    var inscope mat_sampler <- create_sampler(device, msci)

    var inscope materials : array<Tex>
    materials |> reserve(N_MAT)
    for (m in range(N_MAT)) {
        var inscope tex <- create_material(device, phys, queue, pool, m)
        materials |> emplace(tex)
    }

    // ----- geometry + buffers -----
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var ssbo_usage : VkBufferUsageFlags
    ssbo_usage.storage_buffer = true
    var cmds_usage : VkBufferUsageFlags
    cmds_usage.storage_buffer = true
    cmds_usage.indirect_buffer = true
    var count_usage : VkBufferUsageFlags
    count_usage.storage_buffer = true
    count_usage.indirect_buffer = true
    count_usage.transfer_dst = true

    var cv_copy := cube_vertices
    var inscope cube_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, cv_copy)
    delete cv_copy
    var ci_copy := cube_indices
    var inscope cube_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, ci_copy)
    delete ci_copy
    var inscope ubo <- create_host_buffer(device, phys, SCENE_BYTES, ubo_usage)
    var inst_words <- gen_instance_words()
    var inscope instances <- create_host_buffer_from_bytes(device, phys, ssbo_usage, inst_words)
    delete inst_words
    var inscope draw_cmds <- create_host_buffer(device, phys, uint64(N_INSTANCES) * uint64(DRAWCMD_STRIDE), cmds_usage)
    var inscope draw_count <- create_host_buffer(device, phys, 4ul, count_usage)
    var inscope draw_inst <- create_host_buffer(device, phys, uint64(N_INSTANCES) * 4ul, ssbo_usage)
    var inscope cull_reason <- create_host_buffer(device, phys, uint64(N_INSTANCES) * 4ul, ssbo_usage)

    // ----- reflection-driven layouts -----
    var cull_refl <- [decode_reflection(cull_spv_reflect)]
    var inscope cull_layouts <- build_descriptor_set_layouts(device, cull_refl)
    var inscope cull_pl <- build_pipeline_layout(device, cull_layouts, cull_refl)
    delete cull_refl
    var hzb_refl <- [decode_reflection(hzb_down_spv_reflect)]
    var inscope hzb_layouts <- build_descriptor_set_layouts(device, hzb_refl)
    var inscope hzb_pl <- build_pipeline_layout(device, hzb_layouts, hzb_refl)
    delete hzb_refl
    var occ_refl <- [decode_reflection(occluder_vert_spv_reflect), decode_reflection(occluder_solid_frag_spv_reflect)]
    var inscope occ_layouts <- build_descriptor_set_layouts(device, occ_refl)
    var inscope occ_pl <- build_pipeline_layout(device, occ_layouts, occ_refl)
    delete occ_refl
    var main_refl <- [decode_reflection(main_vert_spv_reflect), decode_reflection(main_frag_spv_reflect)]
    var inscope main_layouts <- build_descriptor_set_layouts(device, main_refl)
    var inscope main_pl <- build_pipeline_layout(device, main_layouts, main_refl)
    delete main_refl
    var god_refl <- [decode_reflection(god_vert_spv_reflect), decode_reflection(god_frag_spv_reflect)]
    var inscope god_layouts <- build_descriptor_set_layouts(device, god_refl)
    var inscope god_pl <- build_pipeline_layout(device, god_layouts, god_refl)
    delete god_refl

    // ----- descriptor pool -----
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = uint(3 + MIP_COUNT)
    // cull + main + god
    let p_ubo = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 3u)
    dpci.pPoolSizes |> push(p_ubo)
    // cull(5) + main(2) + god(2)
    let p_ssbo = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = uint(5 + 2 + 2))
    dpci.pPoolSizes |> push(p_ssbo)
    // cull hzb(1) + main(N) + god(N) + hzb_src(MIP)
    let p_smp = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = uint(1 + N_MAT + N_MAT + MIP_COUNT))
    dpci.pPoolSizes |> push(p_smp)
    let p_simg = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = uint(MIP_COUNT))
    dpci.pPoolSizes |> push(p_simg)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    // ----- allocate + write sets -----
    var cull_set = alloc_one_set(device, desc_pool, cull_layouts[0])
    var main_set = alloc_one_set(device, desc_pool, main_layouts[0])
    var god_set  = alloc_one_set(device, desc_pool, god_layouts[0])
    var hzb_sets <- [for (_l in range(MIP_COUNT)); alloc_one_set(device, desc_pool, hzb_layouts[0])]
    var mat_views <- [for (mt in materials); weak_copy(mt.view)]

    // cull set: scene UBO + 5 SSBOs + HZB sampler; main/god sets: scene + instances + bindless samplers
    var writes <- [
        write_ubo(cull_set, 0u, ubo.buffer, SCENE_BYTES),
        write_ssbo(cull_set, 1u, instances.buffer, uint64(N_INSTANCES) * uint64(INST_STRIDE)),
        write_ssbo(cull_set, 2u, draw_cmds.buffer, uint64(N_INSTANCES) * uint64(DRAWCMD_STRIDE)),
        write_ssbo(cull_set, 3u, draw_count.buffer, 4ul),
        write_ssbo(cull_set, 4u, draw_inst.buffer, uint64(N_INSTANCES) * 4ul),
        write_ssbo(cull_set, 5u, cull_reason.buffer, uint64(N_INSTANCES) * 4ul),
        write_sampler(cull_set, 6u, hzb.sampled, depth_sampler, VkImageLayout.GENERAL),
        write_ubo(main_set, 0u, ubo.buffer, SCENE_BYTES),
        write_ssbo(main_set, 1u, instances.buffer, uint64(N_INSTANCES) * uint64(INST_STRIDE)),
        write_ssbo(main_set, 4u, draw_inst.buffer, uint64(N_INSTANCES) * 4ul),
        write_sampler_array(main_set, 7u, mat_views, mat_sampler),
        write_ubo(god_set, 0u, ubo.buffer, SCENE_BYTES),
        write_ssbo(god_set, 1u, instances.buffer, uint64(N_INSTANCES) * uint64(INST_STRIDE)),
        write_ssbo(god_set, 5u, cull_reason.buffer, uint64(N_INSTANCES) * 4ul),
        write_sampler_array(god_set, 7u, mat_views, mat_sampler)
    ]
    // hzb sets: binding 0 = src sampler, binding 1 = dst storage view (per mip)
    //   mip 0 reads the occluder depth; mips 1..MIP-1 read the HZB itself (full-chain sampled view).
    writes |> reserve(length(writes) + 2 * MIP_COUNT)
    for (l in range(MIP_COUNT)) {
        let src_view = l == 0 ? weak_copy(occ_depth.view) : weak_copy(hzb.sampled)
        let src_layout = l == 0 ? VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL : VkImageLayout.GENERAL
        writes |> emplace(write_sampler(hzb_sets[l], 0u, src_view, depth_sampler, src_layout))
        writes |> emplace(write_storage_image(hzb_sets[l], 1u, hzb.mip_views[l]))
    }
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)
    delete mat_views

    // ----- shaders + pipelines -----
    var inscope sh_cull <- create_shader_module(device, cull_spv)
    var inscope sh_mip0 <- create_shader_module(device, hzb_mip0_spv)
    var inscope sh_down <- create_shader_module(device, hzb_down_spv)
    var inscope sh_occ_v <- create_shader_module(device, occluder_vert_spv)
    var inscope sh_occ_solid <- create_shader_module(device, occluder_solid_frag_spv)
    var inscope sh_occ_ghost <- create_shader_module(device, occluder_ghost_frag_spv)
    var inscope sh_main_v <- create_shader_module(device, main_vert_spv)
    var inscope sh_main_f <- create_shader_module(device, main_frag_spv)
    var inscope sh_god_v <- create_shader_module(device, god_vert_spv)
    var inscope sh_god_f <- create_shader_module(device, god_frag_spv)

    var inscope cull_pipe <- create_compute_pipeline(device, cull_pl, sh_cull)
    var inscope mip0_pipe <- create_compute_pipeline(device, hzb_pl, sh_mip0)
    var inscope down_pipe <- create_compute_pipeline(device, hzb_pl, sh_down)
    var inscope depth_pipe <- build_depth_pipeline(device, rp_depth, occ_pl, sh_occ_v)

    var mvb <- cube_vbindings()
    var mva <- cube_vattrs()
    var inscope main_pipe <- build_color_pipeline(device, rp_color, main_pl, sh_main_v, sh_main_f, mvb, mva, true, false)
    delete mvb
    delete mva
    var gvb <- cube_vbindings()
    var gva <- cube_vattrs()
    var inscope god_pipe <- build_color_pipeline(device, rp_color, god_pl, sh_god_v, sh_god_f, gvb, gva, true, true)
    delete gvb
    delete gva
    var ovb <- occ_vbindings()
    var ova <- occ_vattrs()
    var inscope occ_solid_pipe <- build_color_pipeline(device, rp_color, occ_pl, sh_occ_v, sh_occ_solid, ovb, ova, true, false)
    delete ovb
    delete ova
    var ovb2 <- occ_vbindings()
    var ova2 <- occ_vattrs()
    var inscope occ_ghost_pipe <- build_color_pipeline(device, rp_color, occ_pl, sh_occ_v, sh_occ_ghost, ovb2, ova2, false, true)
    delete ovb2
    delete ova2

    let buf_size = uint64(OUT_W * OUT_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- GpuDrivenResources(
        out_color <- out_color, out_depth <- out_depth, occ_depth <- occ_depth, hzb <- hzb,
        rp_depth <- rp_depth, rp_color <- rp_color, fb_depth <- fb_depth, fb_color <- fb_color,
        depth_sampler <- depth_sampler, mat_sampler <- mat_sampler, materials <- materials,
        cube_vb <- cube_vb, cube_ib <- cube_ib, ubo <- ubo, instances <- instances,
        draw_cmds <- draw_cmds, draw_count <- draw_count, draw_inst <- draw_inst, cull_reason <- cull_reason,
        cull_layouts <- cull_layouts, cull_pl <- cull_pl, cull_pipe <- cull_pipe, cull_set = cull_set,
        hzb_layouts <- hzb_layouts, hzb_pl <- hzb_pl, mip0_pipe <- mip0_pipe, down_pipe <- down_pipe,
        hzb_sets <- hzb_sets, occ_layouts <- occ_layouts, occ_pl <- occ_pl, depth_pipe <- depth_pipe,
        occ_solid_pipe <- occ_solid_pipe, occ_ghost_pipe <- occ_ghost_pipe,
        main_layouts <- main_layouts, main_pl <- main_pl, main_pipe <- main_pipe, main_set = main_set,
        god_layouts <- god_layouts, god_pl <- god_pl, god_pipe <- god_pipe, god_set = god_set,
        desc_pool <- desc_pool, readback <- readback, buf_size = buf_size)
}

def public build_gpu_driven_context() : GpuDrivenContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 12", make_api_version(1u, 2u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_gpu_driven(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var inscope pool <- create_command_pool(device, CommandPoolCreateInfo(queueFamilyIndex = gfx))
    var inscope res <- build_gpu_driven_resources(device, phys, queue, pool)
    return <- GpuDrivenContext(instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

// ===== per-frame =====

//! Push the camera + god matrices + HZB dims + per-cube index count into the shared scene UBO.
def public update_uniforms(res : GpuDrivenResources; device : Device) {
    scene.view_proj = camera_view_proj()
    scene.god_view_proj = god_view_proj()
    scene.hzb_dims = float4(float(HZB_BASE), float(HZB_BASE), float(MIP_COUNT), float(N_INSTANCES))
    scene.params = float4(float(N_INDICES), 0.0f, 0.0f, 0.0f)
    cull_cs_bind_uniform(device, res.ubo.memory)
}

//! viewport + scissor for panel `p` (0 = left/camera, 1 = right/god) in the wide target.
def private set_panel(cmd : CommandBuffer; p : int) {
    let raw = boost_value_to_vk(cmd)
    var vp : VkViewport
    vp.x = float(p * PANEL)
    vp.width = float(PANEL)
    vp.height = float(PANEL)
    vp.maxDepth = 1.0f
    var sc : VkRect2D
    sc.offset.x = p * PANEL
    sc.extent.width = uint(PANEL)
    sc.extent.height = uint(PANEL)
    unsafe {
        vkCmdSetViewport(raw, 0u, 1u, addr(vp))
        vkCmdSetScissor(raw, 0u, 1u, addr(sc))
    }
}

//! Full memory barrier between two compute stages (all shader writes visible to all shader reads).
def private compute_barrier(cmd : CommandBuffer) {
    var mb : MemoryBarrier
    mb.srcAccessMask.shader_write = true
    mb.dstAccessMask.shader_read = true
    var mems : array<MemoryBarrier>
    mems |> emplace(mb)
    let no_buf : array<BufferMemoryBarrier>
    let no_img : array<ImageMemoryBarrier>
    var src : VkPipelineStageFlags
    src.compute_shader = true
    var dst : VkPipelineStageFlags
    dst.compute_shader = true
    let no_dep : VkDependencyFlags
    cmd_pipeline_barrier(cmd, src, dst, no_dep, mems, no_buf, no_img)
}

//! Record the whole GPU-driven frame for a given wall X. One command buffer, five passes, the barrier
//! chain between them. Leaves out_color in TRANSFER_SRC_OPTIMAL for the caller's readback / present.
def public record_frame(res : GpuDrivenResources; cmd : CommandBuffer; wall_x : float) {   // nolint:STYLE038 - flat command-record run
    let raw = boost_value_to_vk(cmd)

    // ===== pass 1: occluder depth pre-pass (camera view) =====
    occ_pc.mvp = camera_view_proj() * wall_model(wall_x)
    var depth_clear <- [clear_depth(1.0f)]
    record_render_pass(cmd, res.rp_depth, res.fb_depth, full_area(HZB_BASE, HZB_BASE), depth_clear) {
        cmd_bind_pipeline(cmd, res.depth_pipe)
        occluder_vs_push_constants(cmd, res.occ_pl)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), 1u, 0u, 0, 0u)
    }
    delete depth_clear

    // barrier: depth write -> compute sampled read (occ_depth is in DEPTH_STENCIL_READ_ONLY already)
    {
        var ib : ImageMemoryBarrier
        ib.srcAccessMask.depth_stencil_attachment_write = true
        ib.dstAccessMask.shader_read = true
        ib.oldLayout = VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL
        ib.newLayout = VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL
        ib.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        ib.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        ib.image = weak_copy(res.occ_depth.image)
        ib.subresourceRange.aspectMask.depth = true
        ib.subresourceRange.levelCount = 1u
        ib.subresourceRange.layerCount = 1u
        var imgs : array<ImageMemoryBarrier>
        imgs |> emplace(ib)
        let no_mem : array<MemoryBarrier>
        let no_buf : array<BufferMemoryBarrier>
        var src : VkPipelineStageFlags
        src.early_fragment_tests = true       // the depth-only occluder pipeline has no fragment
        src.late_fragment_tests = true        // shader, so its depth write lands in EARLY tests; cover both
        var dst : VkPipelineStageFlags
        dst.compute_shader = true
        let no_dep : VkDependencyFlags
        cmd_pipeline_barrier(cmd, src, dst, no_dep, no_mem, no_buf, imgs)
    }

    // ===== pass 2: HZB build =====
    // transition the whole HZB chain UNDEFINED -> GENERAL for storage writes
    {
        var ib : ImageMemoryBarrier
        ib.dstAccessMask.shader_write = true
        ib.oldLayout = VkImageLayout.UNDEFINED
        ib.newLayout = VkImageLayout.GENERAL
        ib.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        ib.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        ib.image = weak_copy(res.hzb.image)
        ib.subresourceRange.aspectMask.color = true
        ib.subresourceRange.levelCount = uint(MIP_COUNT)
        ib.subresourceRange.layerCount = 1u
        var imgs : array<ImageMemoryBarrier>
        imgs |> emplace(ib)
        let no_mem : array<MemoryBarrier>
        let no_buf : array<BufferMemoryBarrier>
        var src : VkPipelineStageFlags
        src.top_of_pipe = true
        var dst : VkPipelineStageFlags
        dst.compute_shader = true
        let no_dep : VkDependencyFlags
        cmd_pipeline_barrier(cmd, src, dst, no_dep, no_mem, no_buf, imgs)
    }
    // mip 0: copy occluder depth -> HZB[0]
    cmd_bind_pipeline(cmd, res.mip0_pipe, VkPipelineBindPoint.COMPUTE)
    bind_compute_set(cmd, res.hzb_pl, res.hzb_sets[0])
    cmd_dispatch(cmd, uint(HZB_BASE / 8), uint(HZB_BASE / 8), 1u)
    compute_barrier(cmd)
    // downsample: mip L = max 2x2 of mip L-1
    cmd_bind_pipeline(cmd, res.down_pipe, VkPipelineBindPoint.COMPUTE)
    for (l in range(MIP_COUNT - 1)) {
        let dst_mip = l + 1
        let dim = HZB_BASE >> dst_mip
        hzb_pc.src_lod = l
        bind_compute_set(cmd, res.hzb_pl, res.hzb_sets[dst_mip])
        hzb_down_push_constants(cmd, res.hzb_pl)
        cmd_dispatch(cmd, uint(max(dim / 8, 1)), uint(max(dim / 8, 1)), 1u)
        compute_barrier(cmd)
    }

    // ===== pass 3: cull =====
    // zero the draw count, then make it visible to the cull shader
    unsafe {
        vkCmdFillBuffer(raw, boost_value_to_vk(res.draw_count.buffer), 0ul, 4ul, 0u)
    }
    {
        var bb : BufferMemoryBarrier
        bb.srcAccessMask.transfer_write = true
        bb.dstAccessMask.shader_read = true
        bb.dstAccessMask.shader_write = true
        bb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb.buffer = weak_copy(res.draw_count.buffer)
        bb.offset = 0ul
        bb.size = 4ul
        var bufs : array<BufferMemoryBarrier>
        bufs |> emplace(bb)
        let no_mem : array<MemoryBarrier>
        let no_img : array<ImageMemoryBarrier>
        var src : VkPipelineStageFlags
        src.transfer = true
        var dst : VkPipelineStageFlags
        dst.compute_shader = true
        let no_dep : VkDependencyFlags
        cmd_pipeline_barrier(cmd, src, dst, no_dep, no_mem, bufs, no_img)
    }
    cmd_bind_pipeline(cmd, res.cull_pipe, VkPipelineBindPoint.COMPUTE)
    bind_compute_set(cmd, res.cull_pl, res.cull_set)
    cmd_dispatch(cmd, uint((N_INSTANCES + 63) / 64), 1u, 1u)

    // barrier: cull writes -> indirect draw read (draw_cmds/draw_count) + vertex read (draw_inst/cull_reason)
    {
        var bb_ind : BufferMemoryBarrier
        bb_ind.srcAccessMask.shader_write = true
        bb_ind.dstAccessMask.indirect_command_read = true
        bb_ind.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_ind.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_ind.buffer = weak_copy(res.draw_cmds.buffer)
        bb_ind.size = VK_WHOLE_SIZE
        var bb_cnt : BufferMemoryBarrier
        bb_cnt.srcAccessMask.shader_write = true
        bb_cnt.dstAccessMask.indirect_command_read = true
        bb_cnt.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_cnt.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_cnt.buffer = weak_copy(res.draw_count.buffer)
        bb_cnt.size = VK_WHOLE_SIZE
        var bb_vtx : BufferMemoryBarrier
        bb_vtx.srcAccessMask.shader_write = true
        bb_vtx.dstAccessMask.shader_read = true
        bb_vtx.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_vtx.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_vtx.buffer = weak_copy(res.draw_inst.buffer)
        bb_vtx.size = VK_WHOLE_SIZE
        var bb_rsn : BufferMemoryBarrier
        bb_rsn.srcAccessMask.shader_write = true
        bb_rsn.dstAccessMask.shader_read = true
        bb_rsn.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_rsn.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
        bb_rsn.buffer = weak_copy(res.cull_reason.buffer)
        bb_rsn.size = VK_WHOLE_SIZE
        let bufs <- [ <- bb_ind, <- bb_cnt, <- bb_vtx, <- bb_rsn]
        let no_mem : array<MemoryBarrier>
        let no_img : array<ImageMemoryBarrier>
        var src : VkPipelineStageFlags
        src.compute_shader = true
        var dst : VkPipelineStageFlags
        dst.draw_indirect = true
        dst.vertex_shader = true
        let no_dep : VkDependencyFlags
        cmd_pipeline_barrier(cmd, src, dst, no_dep, no_mem, bufs, no_img)
    }

    // ===== passes 4 + 5: main view (left) + god view (right) into one wide target =====
    var clears <- [clear_color(0.04f, 0.05f, 0.08f, 1.0f), clear_depth(1.0f)]
    record_render_pass(cmd, res.rp_color, res.fb_color, full_area(OUT_W, OUT_H), clears) {
        // ---- left panel: camera view, occluder + survivors via indirect count ----
        set_panel(cmd, 0)
        occ_pc.mvp = camera_view_proj() * wall_model(wall_x)
        cmd_bind_pipeline(cmd, res.occ_solid_pipe)
        occluder_vs_push_constants(cmd, res.occ_pl)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), 1u, 0u, 0, 0u)

        cmd_bind_pipeline(cmd, res.main_pipe)
        bind_graphics_set(cmd, res.main_pl, res.main_set)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed_indirect_count(cmd, res.draw_cmds.buffer, 0ul, res.draw_count.buffer, 0ul,
            uint(N_INSTANCES), DRAWCMD_STRIDE)

        // ---- right panel: god view, ALL N instances + translucent wall ----
        set_panel(cmd, 1)
        cmd_bind_pipeline(cmd, res.god_pipe)
        bind_graphics_set(cmd, res.god_pl, res.god_set)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), uint(N_INSTANCES), 0u, 0, 0u)

        occ_pc.mvp = god_view_proj() * wall_model(wall_x)
        cmd_bind_pipeline(cmd, res.occ_ghost_pipe)
        occluder_vs_push_constants(cmd, res.occ_pl)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), 1u, 0u, 0, 0u)
    }
    delete clears
}

//! bind a single descriptor set for a compute pipeline.
def private bind_compute_set(cmd : CommandBuffer; layout : PipelineLayout; set : VkDescriptorSet) {
    let sets <- [vk_value_to_boost(set)]
    var no_dyn : array<uint>
    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, layout, 0u, sets, no_dyn)
}
//! bind a single descriptor set for a graphics pipeline.
def private bind_graphics_set(cmd : CommandBuffer; layout : PipelineLayout; set : VkDescriptorSet) {
    let sets <- [vk_value_to_boost(set)]
    var no_dyn : array<uint>
    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, layout, 0u, sets, no_dyn)
}

//! Render one frame at the given wall X and copy the wide target to an RGBA8 host array.
def public render_frame(var ctx : GpuDrivenContext; wall_x : float) : array<uint8> {
    update_uniforms(ctx.res, ctx.device)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_frame(ctx.res, cmd, wall_x)
        copy_image_to_buffer(cmd, ctx.res.out_color.image, ctx.res.readback, OUT_W, OUT_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! Self-contained one-shot for the test + recording.
def public render_gpu_driven(wall_x : float) : array<uint8> {
    var inscope ctx <- build_gpu_driven_context()
    return <- render_frame(ctx, wall_x)
}

//! RGB of pixel (x,y) in the OUT_W x OUT_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * OUT_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

//! Read the GPU-written per-instance cull reasons (0 = visible, 1 = frustum, 2 = occluded).
def public read_cull_reasons(ctx : GpuDrivenContext) : array<uint> {
    var out : array<uint>
    out |> resize(N_INSTANCES)
    map_memory_to_array(ctx.device, ctx.res.cull_reason.memory, uint64(N_INSTANCES) * 4ul) $(m) {
        for (i in range(N_INSTANCES)) {
            out[i] = uint(m[i * 4]) | (uint(m[i * 4 + 1]) << 8u) | (uint(m[i * 4 + 2]) << 16u) | (uint(m[i * 4 + 3]) << 24u)
        }
    }
    return <- out
}

//! Read the GPU-written survivor count (what cmd_draw_indexed_indirect_count consumed).
def public read_draw_count(ctx : GpuDrivenContext) : uint {
    var c = 0u
    map_memory_to_array(ctx.device, ctx.res.draw_count.memory, 4ul) $(m) {
        c = uint(m[0]) | (uint(m[1]) << 8u) | (uint(m[2]) << 16u) | (uint(m[3]) << 24u)
    }
    return c
}
