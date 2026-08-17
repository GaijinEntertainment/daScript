options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 09 render path: re-renders the synthwave cube from tutorial 04, but with two new rails
// stacked on top -- 4x MSAA color + dynamic rendering. Concretely:
//
//   * Color attachment is created with samples = 4, depth attachment is also 4 (the spec requires
//     the attachments in one render pass to agree on sample count). A separate 1x "resolve" color
//     image receives the MSAA-averaged pixels via the rendering attachment's resolveImageView,
//     and is the one that gets blitted into the readback buffer.
//   * No VkRenderPass, no VkFramebuffer. The pipeline carries a VkPipelineRenderingCreateInfo in
//     its pNext chain (color format, depth format, sample count); the per-frame work calls
//     cmd_begin_rendering / cmd_end_rendering with an inline VkRenderingInfo that points at the
//     three image views. Vulkan 1.3 core.
//
// Both rails ship as new vulkan_boost helpers (build_offscreen_target/depth with samples,
// create_graphics_pipeline_dyn, record_rendering, create_device with Vulkan 1.3 features). The
// shaders are unchanged from the cube tutorial -- MSAA is a pipeline + attachment concern, the
// fragment program doesn't know it's being sampled four times.

module msaa_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require msaa_tut_shaders
require math

let public MSAA_W = 768
let public MSAA_H = 768
let MSAA_DEPTH_FMT = VkFormat.D32_SFLOAT
let MSAA_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)
let TEX_DIM = 256

// ===== procedural spiky ball (the AA-test mesh) =====

// MSAA is subtle on a cube (12 edges) and dramatic on a dense thin silhouette, so this builds a UV-sphere
// core studded with SPIKE_COUNT flat-shaded pyramidal spikes in Fibonacci directions -- a sea-urchin whose
// every spike crawls with jaggies at 1x and goes silky at 4x. Layout: pos.xyz(0), uv(12), normal.xyz(20), stride 32.

let SPIKE_COUNT    = 150
let CORE_R         = 0.5f
let SPIKE_TIP      = 0.95f    // spike tip radius (sized so the ball ~fills the cube's old frame)
let SPIKE_BASE_AT  = 0.46f    // base ring sits just inside the core so spikes emerge seamlessly
let SPIKE_BASE_R   = 0.05f    // base ring radius in the tangent plane (thin = sharp)
let SPHERE_STACKS  = 18
let SPHERE_SECTORS = 26

def private push_vert(var verts : array<float>; p : float3; uv : float2; nrm : float3) {
    verts |> push(p.x); verts |> push(p.y); verts |> push(p.z)
    verts |> push(uv.x); verts |> push(uv.y)
    verts |> push(nrm.x); verts |> push(nrm.y); verts |> push(nrm.z)
}

//! Fill `verts` (interleaved pos/uv/normal floats) + `indices` (uint16) with the spiky ball. The core
//! sphere is smooth (per-vertex normal = direction) and indexed; the spikes are flat-shaded (per-face
//! normal), with the winding + normal forced outward so back-face culling keeps a clean silhouette.
def build_spiky_geometry(var verts : array<float>; var indices : array<uint16>) {
    verts |> reserve(20000)
    indices |> reserve(5400)
    var nv = 0

    // ---- core UV sphere (smooth, indexed) ----
    let sector_step = 2.0f * PI / float(SPHERE_SECTORS)
    let stack_step = PI / float(SPHERE_STACKS)
    for (i in range(SPHERE_STACKS + 1)) {
        let stack_a = PI * 0.5f - float(i) * stack_step
        let xy = cos(stack_a)
        let zz = sin(stack_a)
        for (j in range(SPHERE_SECTORS + 1)) {
            let sector_a = float(j) * sector_step
            let dir = float3(xy * cos(sector_a), zz, xy * sin(sector_a))
            let uv = float2(float(j) / float(SPHERE_SECTORS), float(i) / float(SPHERE_STACKS))
            push_vert(verts, dir * CORE_R, uv, dir)
            nv ++
        }
    }
    for (i in range(SPHERE_STACKS)) {
        var k1 = i * (SPHERE_SECTORS + 1)
        var k2 = k1 + SPHERE_SECTORS + 1
        for (_j in range(SPHERE_SECTORS)) {
            if (i != 0) {
                indices |> push(uint16(k1)); indices |> push(uint16(k2)); indices |> push(uint16(k1 + 1))
            }
            if (i != SPHERE_STACKS - 1) {
                indices |> push(uint16(k1 + 1)); indices |> push(uint16(k2)); indices |> push(uint16(k2 + 1))
            }
            k1 ++
            k2 ++
        }
    }

    // ---- spikes (flat-shaded square pyramids in Fibonacci directions) ----
    let golden = PI * (3.0f - sqrt(5.0f))
    for (s in range(SPIKE_COUNT)) {
        let yy = 1.0f - (float(s) + 0.5f) / float(SPIKE_COUNT) * 2.0f
        let rr = sqrt(max(1.0f - yy * yy, 0.0f))
        let theta = float(s) * golden
        let dir = float3(rr * cos(theta), yy, rr * sin(theta))
        let up = abs(dir.y) > 0.95f ? float3(1.0f, 0.0f, 0.0f) : float3(0.0f, 1.0f, 0.0f)
        let tan0 = vec3_normalize(vec3_cross(up, dir))
        let bit0 = vec3_cross(dir, tan0)
        let tip = dir * SPIKE_TIP
        var ring : float3[4]
        for (k in range(4)) {
            let a = float(k) * (PI * 0.5f)
            ring[k] = dir * SPIKE_BASE_AT + (tan0 * cos(a) + bit0 * sin(a)) * SPIKE_BASE_R
        }
        for (k in range(4)) {
            var v0 = ring[k]
            var v1 = ring[(k + 1) % 4]
            var fn = vec3_normalize(vec3_cross(vec3_sub(v1, v0), vec3_sub(tip, v0)))
            if (vec3_dot(fn, dir) < 0.0f) {       // force outward normal + matching CCW winding
                let tmp = v0; v0 = v1; v1 = tmp
                fn = float3(-fn.x, -fn.y, -fn.z)
            }
            push_vert(verts, v0, float2(0.5f, 0.92f), fn); nv ++
            push_vert(verts, v1, float2(0.5f, 0.92f), fn); nv ++
            push_vert(verts, tip, float2(0.5f, 0.06f), fn); nv ++
            indices |> push(uint16(nv - 3)); indices |> push(uint16(nv - 2)); indices |> push(uint16(nv - 1))
        }
    }
}

// ===== matrix builders (column-major, 16 floats each) =====

def perspective_vk(fov_y, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0f / tan(fov_y * 0.5f)
    let nf = 1.0f / (znear - zfar)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, -f, 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, zfar * nf, -1.0f)
    m[3] = float4(0.0f, 0.0f, znear * zfar * nf, 0.0f)
    return m
}

def vec3_sub(a, b : float3) : float3 => float3(a.x - b.x, a.y - b.y, a.z - b.z)
def vec3_cross(a, b : float3) : float3 => float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x)
def vec3_dot(a, b : float3) : float => a.x * b.x + a.y * b.y + a.z * b.z
def vec3_normalize(v : float3) : float3 {
    let len = sqrt(vec3_dot(v, v))
    return float3(v.x / len, v.y / len, v.z / len)
}

def look_at_rh(eye, center, up : float3) : float4x4 {
    let f = vec3_normalize(vec3_sub(center, eye))
    let s = vec3_normalize(vec3_cross(f, up))
    let u = vec3_cross(s, f)
    var m : float4x4
    m[0] = float4(s.x, u.x, -f.x, 0.0f)
    m[1] = float4(s.y, u.y, -f.y, 0.0f)
    m[2] = float4(s.z, u.z, -f.z, 0.0f)
    m[3] = float4(-vec3_dot(s, eye), -vec3_dot(u, eye), vec3_dot(f, eye), 1.0f)
    return m
}

def model_matrix(angle, scale : float) : float4x4 {
    let c = cos(angle) * scale
    let s = sin(angle) * scale
    var m : float4x4
    m[0] = float4(c, 0.0f, -s, 0.0f)
    m[1] = float4(0.0f, scale, 0.0f, 0.0f)
    m[2] = float4(s, 0.0f, c, 0.0f)
    m[3] = float4(0.0f, 0.0f, 0.0f, 1.0f)
    return m
}

// ===== procedural synthwave texture =====

def gen_synthwave_texture(w, h : int) : array<uint8> {
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    let horizon = h / 2 - 8
    let sun_cx = w / 2
    let sun_cy = horizon - 26
    let sun_r = 42
    for (y in range(h)) {
        for (x in range(w)) {
            var r = 0
            var g = 0
            var b = 0
            if (y < horizon) {
                let t = float(y) / float(horizon)
                r = int(40.0f + t * 215.0f)
                g = int(6.0f  + t * 64.0f)
                b = int(80.0f + t * 175.0f)
                let dx = x - sun_cx
                let dy = y - sun_cy
                if (dx * dx + dy * dy < sun_r * sun_r) {
                    let band = (y / 4) % 2
                    if (band == 0) {
                        let glow = clamp(1.0f - float(dy + sun_r) / float(2 * sun_r), 0.0f, 1.0f)
                        r = 255
                        g = int(120.0f + glow * 100.0f)
                        b = int(60.0f  + glow * 80.0f)
                    }
                }
            } elif (y < horizon + 4) {
                r = 255
                g = 90
                b = 210
            } else {
                r = 6
                g = 6
                b = 26
                let dist = y - horizon - 4
                let max_dist = h - horizon - 4
                let scale = float(dist) / float(max_dist)
                let spacing = max(2, int(2.0f + scale * 18.0f))
                if (dist % spacing < 1) {
                    r = 0
                    g = 220
                    b = 255
                }
                let dx = abs(x - w / 2)
                let v_spacing = max(2, int(4.0f + scale * 40.0f))
                if (dx % v_spacing < 2 && scale > 0.02f) {
                    r = (r + 0) / 2
                    g = (g + 200) / 2
                    b = (b + 255) / 2
                }
            }
            let idx = (y * w + x) * 4
            pixels[idx + 0] = uint8(clamp(r, 0, 255))
            pixels[idx + 1] = uint8(clamp(g, 0, 255))
            pixels[idx + 2] = uint8(clamp(b, 0, 255))
            pixels[idx + 3] = 255u8
        }
    }
    return <- pixels
}

// ===== reusable per-frame context =====

//! All MSAA cube state: BOTH the 4x path (color_msaa + color_resolve + depth_msaa + pipeline_msaa) AND a
//! 1x path (color_1x + depth_1x + pipeline_1x), so window/recording toggle at runtime with no rebuild. No
//! `render_pass` / `framebuffer` -- dynamic rendering replaces both with an inline VkRenderingInfo.
struct public MsaaResources {
    color_msaa    : OffscreenTarget //!< 4x MSAA color image; pipeline_msaa writes here
    color_resolve : OffscreenTarget //!< 1x image receiving the resolved (averaged-over-4) pixels at end-of-pass, via resolveImageView
    depth_msaa    : OffscreenDepth  //!< 4x depth; sample count must match the 4x color
    color_1x      : OffscreenTarget //!< 1x color for the no-MSAA path; the final image, blittable
    depth_1x      : OffscreenDepth  //!< 1x depth, matching color_1x
    vb            : HostBuffer
    ib            : HostBuffer
    ubo           : HostBuffer
    tex_image     : Image
    tex_memory    : DeviceMemory
    tex_view      : ImageView
    sampler       : Sampler
    set_layouts   : array<DescriptorSetLayout>
    desc_pool     : DescriptorPool
    desc_set      : VkDescriptorSet
    pipe_layout   : PipelineLayout
    pipeline_msaa : Pipeline        //!< same shader modules as pipeline_1x, rasterizationSamples = 4x
    pipeline_1x   : Pipeline        //!< ...and = 1x; the per-frame caller picks one via `use_msaa : bool`
    readback      : HostBuffer
    buf_size      : uint64
    push_size     : uint
    index_count   : uint               // spiky-ball index count (geometry is generated, not fixed)
}

def public finalize(var r : MsaaResources) {
    unsafe {
        delete r.readback
        delete r.pipeline_1x
        delete r.pipeline_msaa
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.sampler
        delete r.tex_view
        delete r.tex_image
        delete r.tex_memory
        delete r.ubo
        delete r.ib
        delete r.vb
        delete r.depth_1x
        delete r.color_1x
        delete r.depth_msaa
        delete r.color_resolve
        delete r.color_msaa
    }
}

struct public MsaaContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : MsaaResources
}

def public finalize(var ctx : MsaaContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

//! Build the MSAA cube's render resources. The pool's queue family must equal `queue`'s family --
//! the texture upload is recorded into pool and submitted on queue.
def public build_msaa_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : MsaaResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    let dev = boost_value_to_vk(device)

    // 4x MSAA color target: rendered into, never blitted from (cannot carry transfer_src).
    var samples4 : VkSampleCountFlags
    samples4._4 = true
    let msaa_color_extra : VkImageUsageFlags    // no extras; just color_attachment
    var inscope color_msaa <- build_offscreen_target(device, phys, MSAA_W, MSAA_H, MSAA_COLOR_FMT, samples4, msaa_color_extra)

    // 1x resolve target: the rendering attachment resolves MSAA into this image at end-of-pass.
    // Marked transfer_src so the same image flows into the readback buffer (or swapchain blit).
    var samples1 : VkSampleCountFlags
    samples1._1 = true
    var resolve_extra : VkImageUsageFlags
    resolve_extra.transfer_src = true
    var inscope color_resolve <- build_offscreen_target(device, phys, MSAA_W, MSAA_H, MSAA_COLOR_FMT, samples1, resolve_extra)

    // 4x MSAA depth: must match color sample count (VUID-VkSubpassDescription-pDepthStencilAttachment-01418).
    let depth_extra : VkImageUsageFlags
    var inscope depth_msaa <- build_offscreen_depth(device, phys, MSAA_W, MSAA_H, MSAA_DEPTH_FMT, depth_extra, samples4)

    // 1x rasterization path (no MSAA): standalone color target (blittable) + matching depth.
    // Used by the runtime toggle in the window viewer / recording so the same scene can be
    // compared with and without MSAA without a rebuild.
    var color_1x_extra : VkImageUsageFlags
    color_1x_extra.transfer_src = true
    var inscope color_1x <- build_offscreen_target(device, phys, MSAA_W, MSAA_H, MSAA_COLOR_FMT, samples1, color_1x_extra)
    var inscope depth_1x <- build_offscreen_depth(device, phys, MSAA_W, MSAA_H, MSAA_DEPTH_FMT, depth_extra, samples1)

    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var sverts : array<float>
    var sindices : array<uint16>
    build_spiky_geometry(sverts, sindices)
    let spiky_index_count = uint(length(sindices))
    var inscope vb <- create_host_buffer_from_bytes(device, phys, vb_usage, sverts)
    delete sverts
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var inscope ib <- create_host_buffer_from_bytes(device, phys, ib_usage, sindices)
    delete sindices

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 144ul, ubo_usage)

    var tex_pixels <- gen_synthwave_texture(TEX_DIM, TEX_DIM)
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer_from_bytes(device, phys, staging_usage, tex_pixels)
    delete tex_pixels

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = MSAA_COLOR_FMT)
    ici.extent.width = uint(TEX_DIM)
    ici.extent.height = uint(TEX_DIM)
    ici.extent.depth = 1u
    ici.mipLevels = 1u
    ici.arrayLayers = 1u
    ici.samples._1 = true
    ici.tiling = VkImageTiling.OPTIMAL
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    ici.initialLayout = VkImageLayout.UNDEFINED
    var inscope tex_image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(tex_image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope tex_memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(tex_image), boost_value_to_vk(tex_memory), 0ul), null)

    var tvci = ImageViewCreateInfo(image = weak_copy(tex_image),
        viewType = VkImageViewType._2D, format = MSAA_COLOR_FMT)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var inscope tex_view <- create_image_view(device, tvci)

    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT,
        addressModeW = VkSamplerAddressMode.REPEAT,
        maxLod = 1.0f)
    var inscope sampler <- create_sampler(device, sci)

    var reflections <- [decode_reflection(msaa_vert_spv_reflect), decode_reflection(msaa_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    let ps0 = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    let ps1 = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    dpci.pPoolSizes <- [ps0, ps1]
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layouts[0])
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    let binfo = DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 144ul)
    w0.pBufferInfo |> push(binfo)
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let iinfo = DescriptorImageInfo(sampler = weak_copy(sampler),
        imageView = weak_copy(tex_view), imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w1.pImageInfo |> push(iinfo)
    writes |> emplace(w1)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    var push_size = 0u
    for (refl in reflections) {
        for (p in refl.push_constants) {
            push_size = uint(p.size)
        }
    }
    assert(push_size == 64u, "expected reflected push constant size 64 (model float4x4)")
    delete reflections

    var inscope vert <- create_shader_module(device, msaa_vert_spv)
    var inscope frag <- create_shader_module(device, msaa_frag_spv)

    var vbindings : array<VkVertexInputBindingDescription>
    var vb_desc : VkVertexInputBindingDescription
    vb_desc.binding = 0u
    vb_desc.stride = 32u
    vb_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb_desc)
    var vattrs : array<VkVertexInputAttributeDescription>
    var attr_pos : VkVertexInputAttributeDescription
    attr_pos.location = 0u
    attr_pos.binding = 0u
    attr_pos.format = VkFormat.R32G32B32_SFLOAT
    attr_pos.offset = 0u
    vattrs |> push(attr_pos)
    var attr_uv : VkVertexInputAttributeDescription
    attr_uv.location = 1u
    attr_uv.binding = 0u
    attr_uv.format = VkFormat.R32G32_SFLOAT
    attr_uv.offset = 12u
    vattrs |> push(attr_uv)
    var attr_n : VkVertexInputAttributeDescription
    attr_n.location = 2u
    attr_n.binding = 0u
    attr_n.format = VkFormat.R32G32B32_SFLOAT
    attr_n.offset = 20u
    vattrs |> push(attr_n)
    // Two pipelines from the same shader modules, differing only in rasterizationSamples. The format +
    // samples go into VkPipelineRenderingCreateInfo on pNext (no VkRenderPass). The bindings/attrs arrays
    // are cloned for the second build because create_graphics_pipeline_dyn takes them as `var array<>`.
    var vbindings_copy := vbindings
    var vattrs_copy := vattrs
    var inscope pipeline_msaa <- create_graphics_pipeline_dyn(device,
        MSAA_COLOR_FMT, MSAA_DEPTH_FMT, samples4,
        pipe_layout, vert, frag, MSAA_W, MSAA_H, vbindings, vattrs)
    var inscope pipeline_1x <- create_graphics_pipeline_dyn(device,
        MSAA_COLOR_FMT, MSAA_DEPTH_FMT, samples1,
        pipe_layout, vert, frag, MSAA_W, MSAA_H, vbindings_copy, vattrs_copy)
    delete vbindings
    delete vattrs
    delete vbindings_copy
    delete vattrs_copy

    let buf_size = uint64(MSAA_W * MSAA_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    // one-shot texture upload: staging -> tex_image, UNDEFINED -> TRANSFER_DST -> SHADER_READ_ONLY
    run_cmd_sync(device, pool, queue) $(cmd) {
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        let none : VkAccessFlags
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag_stage : VkPipelineStageFlags
        frag_stage.fragment_shader = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        transition_image(cmd, tex_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = uint(TEX_DIM)
        region.imageExtent.height = uint(TEX_DIM)
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
    }

    return <- MsaaResources(
        color_msaa <- color_msaa, color_resolve <- color_resolve, depth_msaa <- depth_msaa,
        color_1x <- color_1x, depth_1x <- depth_1x,
        vb <- vb, ib <- ib, ubo <- ubo,
        tex_image <- tex_image, tex_memory <- tex_memory, tex_view <- tex_view, sampler <- sampler,
        set_layouts <- set_layouts, desc_pool <- desc_pool, desc_set = raw_set,
        pipe_layout <- pipe_layout,
        pipeline_msaa <- pipeline_msaa, pipeline_1x <- pipeline_1x,
        readback <- readback,
        buf_size = buf_size, push_size = push_size, index_count = spiky_index_count)
}

//! Build the offscreen-self-contained context. The instance asks for Vulkan 1.3 API; the device
//! opts in to the dynamicRendering feature via VkPhysicalDeviceVulkan13Features.
def public build_msaa_context() : MsaaContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 09", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    let no_ext : array<string>
    var features13 = VkPhysicalDeviceVulkan13Features()
    features13.dynamicRendering = 1u
    var inscope device <- create_device(phys, gfx, no_ext, features13)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_msaa_resources(device, phys, queue, pool)
    return <- MsaaContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

//! Host-side per-frame prep -- same as tutorial 04, just renamed for clarity.
def public update_msaa_uniforms(res : MsaaResources; device : Device; time, camera_t : float) {
    let camera_angle = camera_t * 2.0f * PI
    let camera_r = 3.5f
    let cam_x = camera_r * cos(camera_angle)
    let cam_z = camera_r * sin(camera_angle)
    let cam_y = sin(camera_angle * 2.0f) * 0.4f
    let cam_pos = float3(cam_x, cam_y, cam_z)
    cam.view = look_at_rh(cam_pos, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.proj = perspective_vk(60.0f * PI / 180.0f, float(MSAA_W) / float(MSAA_H), 0.1f, 10.0f)
    cam.cam_time = float4(cam_pos.x, cam_pos.y, cam_pos.z, time)
    msaa_vs_bind_uniform(device, res.ubo.memory)

    let breathe = 1.0f + 0.05f * sin(time * 2.0f * PI / 5.0f)
    let spin = time * 0.5f
    pc.model = model_matrix(spin, breathe)
}

//! Strip height for the mode indicator drawn at the top of the final image (a thin colored band:
//! green = 4x MSAA, red = 1x). Drawn as a second dynamic-rendering pass with renderArea narrowed
//! to the top of the target -- the begin/end CLEAR loadOp does all the work, no pipeline bound.
let INDICATOR_H = 16

//! Pick the "final" image -- the one with the actual rendered pixels after resolve. show_msaa.das
//! and the recording driver both call this to know which image to blit / read back.
def public final_image(res : MsaaResources; use_msaa : bool) : Image {
    return use_msaa ? res.color_resolve.image : res.color_1x.image
}

//! Record the cube draw with dynamic rendering. `use_msaa` picks 4x MSAA + resolve or 1x raster; each
//! path has its own pipeline + color/depth attachments built up front, so toggling needs no rebuild.
//! Ends with the chosen final image (resolve or 1x color) in TRANSFER_SRC_OPTIMAL, ready for blit.
def public record_msaa_render(res : MsaaResources; cmd : CommandBuffer; use_msaa : bool = true) {   // nolint:STYLE038 - flat command-record run
    var to_color : VkAccessFlags
    to_color.color_attachment_write = true
    var to_depth : VkAccessFlags
    to_depth.depth_stencil_attachment_write = true
    let none : VkAccessFlags
    var top : VkPipelineStageFlags
    top.top_of_pipe = true
    var color_out : VkPipelineStageFlags
    color_out.color_attachment_output = true
    var depth_tests : VkPipelineStageFlags
    depth_tests.early_fragment_tests = true

    var color_att = RenderingAttachmentInfo(imageLayout = VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
        loadOp = VkAttachmentLoadOp.CLEAR)
    color_att.clearValue.color.float32[0] = 0.02f
    color_att.clearValue.color.float32[1] = 0.0f
    color_att.clearValue.color.float32[2] = 0.06f
    color_att.clearValue.color.float32[3] = 1.0f
    var depth_att = RenderingAttachmentInfo(loadOp = VkAttachmentLoadOp.CLEAR,
        storeOp = VkAttachmentStoreOp.DONT_CARE)
    depth_att.clearValue.depthStencil.depth = 1.0f
    depth_att.imageLayout = VkImageLayout.DEPTH_ATTACHMENT_OPTIMAL

    // The "final image" -- the one that ends up holding the rendered pixels.
    let final_img = final_image(res, use_msaa)

    if (use_msaa) {
        transition_image(cmd, res.color_msaa.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
            none, to_color, top, color_out)
        transition_image(cmd, res.color_resolve.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
            none, to_color, top, color_out)
        transition_depth_image(cmd, res.depth_msaa.image, VkImageLayout.UNDEFINED, VkImageLayout.DEPTH_ATTACHMENT_OPTIMAL,
            none, to_depth, top, depth_tests)
        // Color attachment: 4x MSAA source + 1x resolve target. resolveMode=AVERAGE is the
        // standard box filter; SAMPLE_ZERO would be the no-AA fast path.
        color_att.imageView = weak_copy(res.color_msaa.view)
        color_att.storeOp = VkAttachmentStoreOp.DONT_CARE   // MSAA buffer discarded after resolve
        color_att.resolveMode.average = true
        color_att.resolveImageView = weak_copy(res.color_resolve.view)
        color_att.resolveImageLayout = VkImageLayout.COLOR_ATTACHMENT_OPTIMAL
        depth_att.imageView = weak_copy(res.depth_msaa.view)
    } else {
        transition_image(cmd, res.color_1x.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
            none, to_color, top, color_out)
        transition_depth_image(cmd, res.depth_1x.image, VkImageLayout.UNDEFINED, VkImageLayout.DEPTH_ATTACHMENT_OPTIMAL,
            none, to_depth, top, depth_tests)
        // 1x path: no resolve attachment, store the rasterized pixels directly.
        color_att.imageView = weak_copy(res.color_1x.view)
        color_att.storeOp = VkAttachmentStoreOp.STORE
        depth_att.imageView = weak_copy(res.depth_1x.view)
    }

    var color_atts : array<RenderingAttachmentInfo>
    color_atts |> emplace(color_att)
    let pipeline = use_msaa ? res.pipeline_msaa : res.pipeline_1x
    record_rendering(cmd, full_area(MSAA_W, MSAA_H), 1u, color_atts, depth_att) {
        cmd_bind_pipeline(cmd, pipeline)
        let sets <- [vk_value_to_boost(res.desc_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        msaa_vs_push_constants(cmd, res.pipe_layout)
        let vbufs <- [weak_copy(res.vb.buffer)]
        var voffs : array<uint64>
        voffs |> push(0ul)
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, res.index_count, 1u, 0u, 0, 0u)
    }

    // Mode indicator strip: a second dynamic-rendering "pass" with renderArea narrowed to the top
    // INDICATOR_H rows, loadOp=CLEAR with a mode-coloured value. No draw calls -- the clear at begin
    // time IS the indicator. Green = 4x, red = 1x; works in window and recording with no font texture.
    var indicator_att = RenderingAttachmentInfo(
        imageView = use_msaa ? weak_copy(res.color_resolve.view) : weak_copy(res.color_1x.view),
        imageLayout = VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
        loadOp = VkAttachmentLoadOp.CLEAR,
        storeOp = VkAttachmentStoreOp.STORE)
    indicator_att.clearValue.color.float32[0] = use_msaa ? 0.10f : 0.95f
    indicator_att.clearValue.color.float32[1] = use_msaa ? 0.85f : 0.18f
    indicator_att.clearValue.color.float32[2] = use_msaa ? 0.32f : 0.18f
    indicator_att.clearValue.color.float32[3] = 1.0f
    var indicator_atts : array<RenderingAttachmentInfo>
    indicator_atts |> emplace(indicator_att)
    var indicator_area : VkRect2D
    indicator_area.extent.width = uint(MSAA_W)
    indicator_area.extent.height = uint(INDICATOR_H)
    record_rendering(cmd, indicator_area, 1u, indicator_atts) {
        pass    // clear-only -- no draws needed for the indicator
    }

    // Final image -> TRANSFER_SRC for the readback / blit-out.
    var to_xfer : VkAccessFlags
    to_xfer.transfer_read = true
    var xfer : VkPipelineStageFlags
    xfer.transfer = true
    transition_image(cmd, final_img, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
        to_color, to_xfer, color_out, xfer)
}

//! Per-frame work: update UBO + push constant, record the dyn-rendering pass, copy the final
//! 1x color (resolved or rasterized) into the readback buffer, clone the bytes out as RGBA8.
def public render_msaa_frame(var ctx : MsaaContext; time, camera_t : float; use_msaa : bool = true) : array<uint8> {
    update_msaa_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_msaa_render(ctx.res, cmd, use_msaa)
        copy_image_to_buffer(cmd, final_image(ctx.res, use_msaa), ctx.res.readback, MSAA_W, MSAA_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame.
def public render_msaa_cube(time, camera_t : float; use_msaa : bool = true) : array<uint8> {
    var inscope ctx <- build_msaa_context()
    return <- render_msaa_frame(ctx, time, camera_t, use_msaa)
}

// ===== mode toggle (for the windowed viewer + recording driver) =====

//! Three-state toggle: pressing M cycles AUTO -> FORCE_OFF -> FORCE_ON -> AUTO. AUTO flips every
//! AUTO_PERIOD seconds so a recording session captures the comparison automatically.
let public AUTO_PERIOD : float = 2.0f

enum public MsaaToggle {
    auto_toggle  = 0
    force_off    = 1
    force_on     = 2
}

//! Derive the actual MSAA-on-or-off bool for `time` given the current toggle state.
def public msaa_active(t : MsaaToggle; time : float) : bool {
    if (t == MsaaToggle.auto_toggle) {
        let p = time / AUTO_PERIOD
        return (int(p) % 2) == 0
    } elif (t == MsaaToggle.force_on) {
        return true
    }
    return false
}

//! Window-title label for the current toggle state.
def public msaa_toggle_label(t : MsaaToggle) : string {
    if (t == MsaaToggle.auto_toggle) {
        return "AUTO"
    } elif (t == MsaaToggle.force_on) {
        return "ON"
    }
    return "OFF"
}

//! Advance the cycle on a key press: AUTO -> FORCE_OFF -> FORCE_ON -> AUTO ...
def public next_msaa_toggle(t : MsaaToggle) : MsaaToggle {
    if (t == MsaaToggle.auto_toggle) {
        return MsaaToggle.force_off
    } elif (t == MsaaToggle.force_off) {
        return MsaaToggle.force_on
    }
    return MsaaToggle.auto_toggle
}

def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * MSAA_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
