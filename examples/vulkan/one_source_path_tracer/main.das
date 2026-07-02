options gen2
options indenting = 4
options solid_context

// One source, four engines. The SAME pt_core path tracer — scene, camera, RNG, materials,
// integrator — runs on four execution tiers, live-switchable from an imgui_boost_v2 panel rendered
// by the 100%-daslang Vulkan backend:
//
//   CPU 1 thread   one background band thread (interpreter, or JIT when launched with -jit)
//   CPU N threads  band threads over the same rows (path_tracer_lab pattern)
//   GPU compute    the same functions lowered to a SPIR-V compute kernel by dasSpirv
//   GPU ray trace  hardware RT pipeline (VK_KHR_ray_tracing_pipeline): raygen/miss/closest-hit
//                  also lowered by dasSpirv; hardware traversal replaces the software intersector,
//                  the shading/NEE/RNG code is shared verbatim (harness_rt_supported-gated)
//
// The vulkan_imgui_app harness owns the window/device/swapchain and wires the live stack, so the
// app is fully drivable under daslang-live: `imgui_snapshot` dumps the widget state,
// `imgui_force_set {"target":"PT_WIN/ENGINE","value":3}` switches to hardware RT,
// `imgui_force_set {"target":"PT_WIN/THREADS","value":16}` moves the thread slider, and
// `screenshot {"file":"..."}` captures the frame.
//
// Run (after `daslang ../../../utils/daspkg/main.das -- install` in this directory):
//   daslang -project_root . main.das            # interpreter CPU tiers
//   daslang -jit -project_root . main.das       # JIT CPU tiers
// Live:
//   daslang-live -project_root . main.das
// Keys: P = screenshot. Flags: --engine=<Single|Threads|Compute|RayTrace>, --max-frames=N (writes
// one_source_path_tracer_proof.png before exiting; for CI-less smoke and the README shots).

require vulkan/vulkan_imgui_app
require vulkan/vulkan_reflect
require daslib/safe_addr
require daslib/jobque_boost
require daslib/clargs
require math
require strings

require pt_shaders

let {
    TRACE_W = 512
    TRACE_H = 512
    WIN_W = 960
    WIN_H = 640
    MAX_BANDS = 16
}

enum Engine {
    Single
    Threads
    Compute
    RayTrace
}

[CommandLineArgs]
struct Args {
    @clarg_doc = "Starting engine"
    engine : Engine
    @clarg_doc = "Exit after N presented frames, writing one_source_path_tracer_proof.png (0 = run until closed)"
    max_frames : int
    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// ===== CPU engines: band threads over the shared core (path_tracer_lab state machine) =====

var g_mode = int(Engine.Single)
var g_epoch = 0
var g_render_epoch = -1
var g_pass = -1                          // last completed pass (accumulated sample count - 1)
var g_max_threads = 4
var g_accum : array<float3>              // CPU radiance accumulation
var g_pixels : array<uint8>              // CPU tonemapped RGBA (upload source)
var g_cpu_dirty = false                  // a completed CPU pass awaits upload
var g_threads_status : JobStatus?
var g_threads_epoch = 0
var g_threads_t0 = 0l
var g_last_pass_us = 0
var g_smooth_pps = 0.0lf                 // smoothed Mpaths/s

// One band: trace rows [ymin, ymax) for pass `my_pass`, tonemap them into the shared pixel buffer,
// bail per-row if the epoch moves (engine switch). Runs in its own context — own pt_core globals.
def cpu_band(var pacc : array<float3>?; var ppix : array<uint8>?; var pepoch : int?;
             my_epoch, my_pass, ymin, ymax : int) {
    pt_init_scene()
    let inv = 1.0 / float(my_pass + 1)
    for (y in range(ymin, ymax)) {
        if (*pepoch != my_epoch) {
            break
        }
        unsafe {
            pt_render_rows(my_pass, TRACE_W, TRACE_H, y, y + 1, *pacc)
        }
        for (x in range(TRACE_W)) {
            let i = y * TRACE_W + x
            let c = pt_tonemap(unsafe((*pacc)[i]) * inv)
            unsafe {
                (*ppix)[i * 4 + 0] = uint8(int(clamp(c.x, 0.0, 1.0) * 255.0 + 0.5))
                (*ppix)[i * 4 + 1] = uint8(int(clamp(c.y, 0.0, 1.0) * 255.0 + 0.5))
                (*ppix)[i * 4 + 2] = uint8(int(clamp(c.z, 0.0, 1.0) * 255.0 + 0.5))
                (*ppix)[i * 4 + 3] = uint8(255)
            }
        }
    }
}

def start_cpu_pass(my_pass, bands : int) {
    var pacc : array<float3>?
    var ppix : array<uint8>?
    var pepoch : int?
    unsafe {
        pacc = addr(g_accum)
        ppix = addr(g_pixels)
        pepoch = addr(g_epoch)
    }
    g_threads_t0 = ref_time_ticks()
    g_threads_epoch = g_epoch
    var status = job_status_create()
    status |> append(bands)
    g_threads_status = status
    let chunk = (TRACE_H + bands - 1) / bands
    let my_epoch = g_epoch
    for (c in range(bands)) {
        let ymin = c * chunk
        let ymax = min((c + 1) * chunk, TRACE_H)
        var st = status
        new_thread() <| @capture(= pacc, = ppix, = pepoch, = my_epoch, = my_pass, = ymin, = ymax, = st) {
            cpu_band(pacc, ppix, pepoch, my_epoch, my_pass, ymin, ymax)
            st |> notify_and_release
        }
    }
}

def note_pass_done {
    g_last_pass_us = get_time_usec(g_threads_t0)
    let inst = double(TRACE_W * TRACE_H) / double(max(g_last_pass_us, 1))   // paths/us == Mpaths/s
    g_smooth_pps = g_smooth_pps == 0.0lf ? inst : g_smooth_pps * 0.85lf + inst * 0.15lf
}

// Drain an in-flight CPU pass and apply the epoch reset. Returns false while a pass from a dead
// epoch is still winding down — no engine may dispatch until the reset has actually happened.
def advance_ready : bool {
    if (g_threads_status != null) {
        if (!g_threads_status.isReady) {
            return false
        }
        if (g_threads_epoch == g_epoch) {
            g_pass++
            g_cpu_dirty = true
            note_pass_done()
        }
        var st = g_threads_status
        unsafe {
            job_status_remove(st)
        }
        g_threads_status = null
    }
    if (g_render_epoch != g_epoch) {
        g_render_epoch = g_epoch
        g_pass = -1
        for (i in range(TRACE_W * TRACE_H)) {
            g_accum[i] = float3(0.0)
        }
    }
    return true
}

// ===== GPU resources (globals — daslang-live reloads keep them; shutdown() deletes them) =====

struct PtImage {
    image : Image
    memory : DeviceMemory
    view : ImageView
}

def finalize(var pi : PtImage) {
    delete pi.view
    delete pi.image
    delete pi.memory
}

def make_storage_image(device : Device; phys : VkPhysicalDevice; w, h : int; fmt : VkFormat;
                       sampled, transfer_dst : bool) : PtImage {
    let dev = boost_value_to_vk(device)
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = 1u,
        arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(w)
    ici.extent.height = uint(h)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.storage = true
    ici.usage.sampled = sampled
    ici.usage.transfer_dst = transfer_dst
    var inscope image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope imem <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(image), boost_value_to_vk(imem), 0ul), null)
    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = fmt)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)
    return <- PtImage(image <- image, memory <- imem, view <- view)
}

//! layouts + pool + set + pipeline for one engine stack (compute or RT — same descriptor surface,
//! the RT stack adds the TLAS at binding 4 and builds its pipeline/SBT separately)
struct EngineStack {
    set_layouts : array<DescriptorSetLayout>
    pipe_layout : PipelineLayout
    desc_pool : DescriptorPool
    dsets : array<DescriptorSet>
    pipeline : Pipeline
}

def finalize(var s : EngineStack) {
    delete s.pipeline
    delete s.desc_pool
    delete s.pipe_layout
    delete s.set_layouts
    delete s.dsets
}

def write_common_bindings(device : Device; dset : DescriptorSet; accum, disp : PtImage;
                          vbuf, tbuf : HostBuffer) {
    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(dstSet = weak_copy(dset), dstBinding = 0u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    w0.pImageInfo |> push(DescriptorImageInfo(imageView = weak_copy(accum.view), imageLayout = VkImageLayout.GENERAL))
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(dstSet = weak_copy(dset), dstBinding = 1u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    w1.pImageInfo |> push(DescriptorImageInfo(imageView = weak_copy(disp.view), imageLayout = VkImageLayout.GENERAL))
    writes |> emplace(w1)
    var w2 = WriteDescriptorSet(dstSet = weak_copy(dset), dstBinding = 2u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    w2.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(vbuf.buffer), range_ = vbuf.size))
    writes |> emplace(w2)
    var w3 = WriteDescriptorSet(dstSet = weak_copy(dset), dstBinding = 3u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    w3.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(tbuf.buffer), range_ = tbuf.size))
    writes |> emplace(w3)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)
}

// ===== fullscreen display quad (letterboxed, samples the display image) =====

def build_quad_pipeline(device : Device; render_pass : RenderPass; layout : PipelineLayout;
                        vert, frag : ShaderModule) : Pipeline {
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

    var vinput = VkPipelineVertexInputStateCreateInfo()   // no vertex input: gl_VertexIndex drives it
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u
    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true
    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u
    var dyn_states : array<VkDynamicState>
    dyn_states |> push(VkDynamicState.VIEWPORT)
    dyn_states |> push(VkDynamicState.SCISSOR)
    var dynstate = VkPipelineDynamicStateCreateInfo()
    dynstate.dynamicStateCount = 2u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        dynstate.pDynamicStates = addr(dyn_states[0])
        gp.pDynamicState = addr(dynstate)
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    delete stages
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

// ===== app state (globals so daslang-live reloads keep them) =====

var g_rt_ok = false
var g_max_frames = 0
var g_vbuf : HostBuffer
var g_tbuf : HostBuffer
var g_staging : HostBuffer
var g_accum_img : PtImage
var g_disp_img : PtImage
var g_comp : EngineStack
var g_rt : EngineStack
var g_blas : AccelStructure
var g_tlas : AccelStructure
var g_sbt : ShaderBindingTable
var g_sampler : Sampler
var g_quad : EngineStack

[export]
def init() {
    var args_r <- parse_args(type<Args>)
    if (args_r |> is_err) {
        print("error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "one_source_path_tracer")
        request_exit()
        return
    }
    let args <- args_r |> move_unwrap
    if (args.help) {
        print_help(get_command_info(type<Args>), "one_source_path_tracer")
        request_exit()
        return
    }
    g_mode = int(args.engine)
    g_max_frames = args.max_frames

    harness_init("one source, four engines — daslang path tracer", WIN_W, WIN_H, true)
    let device = harness_device()
    let phys = harness_phys()
    let queue = harness_queue()
    let pool = harness_pool()
    g_rt_ok = harness_rt_supported()
    if (!g_rt_ok && g_mode == int(Engine.RayTrace)) {
        g_mode = int(Engine.Compute)
    }
    ENGINE.value = g_mode

    // ----- shared scene + images: CPU fills the globals, GPU binds the same arrays -----
    pt_init_scene()
    g_accum |> resize(TRACE_W * TRACE_H)
    g_pixels |> resize(TRACE_W * TRACE_H * 4)
    var buf_usage : VkBufferUsageFlags
    buf_usage.storage_buffer = true
    g_vbuf <- create_host_buffer_from_bytes(device, phys, buf_usage, pt_verts)
    g_tbuf <- create_host_buffer_from_bytes(device, phys, buf_usage, pt_tris)
    var stage_usage : VkBufferUsageFlags
    stage_usage.transfer_src = true
    g_staging <- create_host_buffer(device, phys, uint64(TRACE_W * TRACE_H * 4), stage_usage)
    g_accum_img <- make_storage_image(device, phys, TRACE_W, TRACE_H, VkFormat.R32G32B32A32_SFLOAT, false, false)
    g_disp_img <- make_storage_image(device, phys, TRACE_W, TRACE_H, VkFormat.R8G8B8A8_UNORM, true, true)

    // both storage images live in GENERAL forever: written by compute/RT, copied into from the CPU
    // staging buffer, sampled by the display quad — every engine submit is queue-synchronous
    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var shader_rw : VkAccessFlags
        shader_rw.shader_read = true
        shader_rw.shader_write = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var comp_stage : VkPipelineStageFlags
        comp_stage.compute_shader = true
        transition_image(cmd, g_accum_img.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL, none, shader_rw, top, comp_stage)
        transition_image(cmd, g_disp_img.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL, none, shader_rw, top, comp_stage)
    }

    // ----- compute engine stack -----
    {
        var reflections <- [decode_reflection(pt_compute_spv_reflect)]
        g_comp.set_layouts <- build_descriptor_set_layouts(device, reflections)
        g_comp.pipe_layout <- build_pipeline_layout(device, g_comp.set_layouts, reflections)
        delete reflections
        var dpci = DescriptorPoolCreateInfo(maxSets = 1u,
            pPoolSizes <- [
                DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 2u),
                DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 2u)
            ])
        g_comp.desc_pool <- create_descriptor_pool(device, dpci)
        var dsai : DescriptorSetAllocateInfo
        dsai.descriptorPool = weak_copy(g_comp.desc_pool)
        dsai.pSetLayouts |> push(weak_copy(g_comp.set_layouts[0]))
        g_comp.dsets <- allocate_descriptor_sets(device, dsai)
        write_common_bindings(device, g_comp.dsets[0], g_accum_img, g_disp_img, g_vbuf, g_tbuf)
        var inscope shader <- create_shader_module(device, pt_compute_spv)
        g_comp.pipeline <- create_compute_pipeline(device, g_comp.pipe_layout, shader)
    }

    // ----- hardware RT engine stack (only when the device has the pipeline) -----
    if (g_rt_ok) {
        var bverts : array<float3>
        for (pv in pt_verts) {
            bverts |> push(pv.xyz)
        }
        var bidx : array<uint>
        for (tr in pt_tris) {
            bidx |> push(tr.x)
            bidx |> push(tr.y)
            bidx |> push(tr.z)
        }
        g_blas <- build_blas(device, phys, pool, queue, bverts, bidx)
        delete bverts
        delete bidx
        var insts <- [make_accel_instance(g_blas.address)]
        g_tlas <- build_tlas(device, phys, pool, queue, insts)
        delete insts
        var reflections <- [decode_reflection(pt_rgen_spv_reflect),
            decode_reflection(pt_miss_spv_reflect),
            decode_reflection(pt_shadow_miss_spv_reflect),
            decode_reflection(pt_chit_spv_reflect)]
        g_rt.set_layouts <- build_descriptor_set_layouts(device, reflections)
        g_rt.pipe_layout <- build_pipeline_layout(device, g_rt.set_layouts, reflections)
        delete reflections
        var dpci = DescriptorPoolCreateInfo(maxSets = 1u,
            pPoolSizes <- [
                DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 2u),
                DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 2u),
                DescriptorPoolSize(type_ = VkDescriptorType.ACCELERATION_STRUCTURE_KHR, descriptorCount = 1u)
            ])
        g_rt.desc_pool <- create_descriptor_pool(device, dpci)
        var dsai : DescriptorSetAllocateInfo
        dsai.descriptorPool = weak_copy(g_rt.desc_pool)
        dsai.pSetLayouts |> push(weak_copy(g_rt.set_layouts[0]))
        g_rt.dsets <- allocate_descriptor_sets(device, dsai)
        write_common_bindings(device, g_rt.dsets[0], g_accum_img, g_disp_img, g_vbuf, g_tbuf)
        write_descriptor_acceleration_structure(device, g_rt.dsets[0], 4u, g_tlas)
        var inscope rgen <- create_shader_module(device, pt_rgen_spv)
        var inscope miss <- create_shader_module(device, pt_miss_spv)
        var inscope shadow_miss <- create_shader_module(device, pt_shadow_miss_spv)
        var inscope chit <- create_shader_module(device, pt_chit_spv)
        var miss_list <- [weak_copy(miss), weak_copy(shadow_miss)]
        g_rt.pipeline <- create_ray_tracing_pipeline(device, g_rt.pipe_layout, rgen, miss_list, chit, 1u)
        delete miss_list
        g_sbt <- build_shader_binding_table(device, phys, g_rt.pipeline, 2u, 1u)
    }

    // ----- display quad stack -----
    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE,
        maxLod = 1.0f)
    g_sampler <- create_sampler(device, sci)
    {
        var reflections <- [decode_reflection(pt_quad_vert_spv_reflect), decode_reflection(pt_quad_frag_spv_reflect)]
        g_quad.set_layouts <- build_descriptor_set_layouts(device, reflections)
        g_quad.pipe_layout <- build_pipeline_layout(device, g_quad.set_layouts, reflections)
        delete reflections
        var dpci = DescriptorPoolCreateInfo(maxSets = 1u,
            pPoolSizes <- [DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)])
        g_quad.desc_pool <- create_descriptor_pool(device, dpci)
        var dsai : DescriptorSetAllocateInfo
        dsai.descriptorPool = weak_copy(g_quad.desc_pool)
        dsai.pSetLayouts |> push(weak_copy(g_quad.set_layouts[0]))
        g_quad.dsets <- allocate_descriptor_sets(device, dsai)
        var write = WriteDescriptorSet(dstSet = weak_copy(g_quad.dsets[0]), dstBinding = 0u,
            descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
        write.pImageInfo |> push(DescriptorImageInfo(sampler = weak_copy(g_sampler),
            imageView = weak_copy(g_disp_img.view), imageLayout = VkImageLayout.GENERAL))
        var writes : array<WriteDescriptorSet>
        writes |> emplace(write)
        let no_copies : array<CopyDescriptorSet>
        update_descriptor_sets(device, writes, no_copies)
        var inscope qvert <- create_shader_module(device, pt_quad_vert_spv)
        var inscope qfrag <- create_shader_module(device, pt_quad_frag_spv)
        g_quad.pipeline <- build_quad_pipeline(device, harness_render_pass(), g_quad.pipe_layout, qvert, qfrag)
    }

    g_max_threads = clamp(get_total_hw_threads(), 4, MAX_BANDS)
    g_cpu_dirty = true   // first frame uploads the zeroed pixel buffer = a black display image
}

[export]
def update() {
    if (!harness_begin_frame()) {
        return
    }
    harness_new_frame()
    let device = harness_device()
    let queue = harness_queue()
    let pool = harness_pool()

    // ----- advance the active engine -----
    if (advance_ready()) {
        if (g_mode == int(Engine.Single)) {
            start_cpu_pass(g_pass + 1, 1)
        } elif (g_mode == int(Engine.Threads)) {
            start_cpu_pass(g_pass + 1, clamp(g_max_threads, 1, MAX_BANDS))
        } else {
            let f = g_pass + 1
            let t0 = ref_time_ticks()
            run_cmd_sync(device, pool, queue) $(cmd) {
                var shader_rw : VkAccessFlags
                shader_rw.shader_read = true
                shader_rw.shader_write = true
                var frag_read : VkAccessFlags
                frag_read.shader_read = true
                var comp_stage : VkPipelineStageFlags
                comp_stage.compute_shader = true
                var rt_stage : VkPipelineStageFlags
                rt_stage.ray_tracing_shader_khr = true
                var frag_stage : VkPipelineStageFlags
                frag_stage.fragment_shader = true
                var no_dyn : array<uint>
                pt_push.frame = uint(f)
                if (g_mode == int(Engine.Compute)) {
                    cmd_bind_pipeline(cmd, g_comp.pipeline, VkPipelineBindPoint.COMPUTE)
                    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, g_comp.pipe_layout, 0u, g_comp.dsets, no_dyn)
                    pt_compute_push_constants(cmd, g_comp.pipe_layout)
                    cmd_dispatch(cmd, uint(TRACE_W / 8), uint(TRACE_H / 8), 1u)
                    transition_image(cmd, g_disp_img.image, VkImageLayout.GENERAL, VkImageLayout.GENERAL,
                        shader_rw, frag_read, comp_stage, frag_stage)
                } else {
                    cmd_bind_pipeline(cmd, g_rt.pipeline, VkPipelineBindPoint.RAY_TRACING_KHR)
                    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.RAY_TRACING_KHR, g_rt.pipe_layout, 0u, g_rt.dsets, no_dyn)
                    pt_rgen_push_constants(cmd, g_rt.pipe_layout)
                    cmd_trace_rays(cmd, g_sbt, uint(TRACE_W), uint(TRACE_H))
                    transition_image(cmd, g_disp_img.image, VkImageLayout.GENERAL, VkImageLayout.GENERAL,
                        shader_rw, frag_read, rt_stage, frag_stage)
                }
            }
            g_pass = f
            g_threads_t0 = t0
            note_pass_done()
        }
    }

    // a completed CPU pass: copy its tonemapped pixels into the display image
    if (g_cpu_dirty) {
        g_cpu_dirty = false
        g_staging |> upload_bytes(device, g_pixels)
        run_cmd_sync(device, pool, queue) $(cmd) {
            let none : VkAccessFlags
            var to_dst : VkAccessFlags
            to_dst.transfer_write = true
            var frag_read : VkAccessFlags
            frag_read.shader_read = true
            var top : VkPipelineStageFlags
            top.top_of_pipe = true
            var xfer : VkPipelineStageFlags
            xfer.transfer = true
            var frag_stage : VkPipelineStageFlags
            frag_stage.fragment_shader = true
            transition_image(cmd, g_disp_img.image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_DST_OPTIMAL,
                none, to_dst, top, xfer)
            var region : BufferImageCopy
            region.imageSubresource.aspectMask.color = true
            region.imageSubresource.layerCount = 1u
            region.imageExtent.width = uint(TRACE_W)
            region.imageExtent.height = uint(TRACE_H)
            region.imageExtent.depth = 1u
            var regions : array<BufferImageCopy>
            regions |> emplace(region)
            cmd_copy_buffer_to_image(cmd, g_staging.buffer, g_disp_img.image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
            transition_image(cmd, g_disp_img.image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.GENERAL,
                to_dst, frag_read, xfer, frag_stage)
        }
    }

    // ----- panel (imgui_boost_v2: every widget live-drivable at PT_WIN/<id>) -----
    window(PT_WIN, (text = "One source, four engines", closable = false,
                    flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        text("The same daslang functions on every engine.")
        radio_button_int(ENGINE, (text = "CPU - 1 thread", v_button = int(Engine.Single)))
        radio_button_int(ENGINE, (text = "CPU - band threads", v_button = int(Engine.Threads)))
        radio_button_int(ENGINE, (text = "GPU - dasSpirv compute", v_button = int(Engine.Compute)))
        if (g_rt_ok) {
            radio_button_int(ENGINE, (text = "GPU - hardware ray tracing", v_button = int(Engine.RayTrace)))
        } else {
            text("GPU - hardware ray tracing: not supported here")
        }
        if (ENGINE.value != g_mode) {
            g_mode = ENGINE.value
            g_epoch++
            g_smooth_pps = 0.0lf
        }
        if (g_mode == int(Engine.Threads)) {
            edit_slider_int(safe_addr(g_max_threads), (id = "THREADS", text = "threads",
                                                       v_min = 1, v_max = MAX_BANDS))
        }
        separator(PT_SEP)
        text(STATS_SAMPLES, (text = "samples: {max(g_pass + 1, 0)}"))
        text(STATS_PPS, (text = "throughput: {fmt(":.2f", g_smooth_pps)} Mpaths/s"))
    }

    // ----- present: letterboxed quad + UI -----
    let sz = harness_size()
    let a = float(sz.x) / float(max(sz.y, 1))
    pt_qpush.scale = a > 1.0 ? float2(1.0 / a, 1.0) : float2(1.0, a)
    harness_end_frame() $(cmd : CommandBuffer) {
        cmd_bind_pipeline(cmd, g_quad.pipeline)
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, g_quad.pipe_layout, 0u, g_quad.dsets, no_dyn)
        pt_quad_vs_push_constants(cmd, g_quad.pipe_layout)
        // scissor to the letterboxed quad so the fullscreen triangle's hypotenuse can't smear
        // clamped edge texels outside it (imgui sets its own scissors after this)
        var sc : Rect2D
        sc.offset.x = int((1.0 - pt_qpush.scale.x) * 0.5 * float(sz.x))
        sc.offset.y = int((1.0 - pt_qpush.scale.y) * 0.5 * float(sz.y))
        sc.extent.width = uint(pt_qpush.scale.x * float(sz.x) + 0.5)
        sc.extent.height = uint(pt_qpush.scale.y * float(sz.y) + 0.5)
        var scis : array<Rect2D>
        scis |> push(sc)
        cmd_set_scissor(cmd, 0u, scis)
        delete scis
        cmd_draw(cmd, 3u, 1u)
    }

    if (g_max_frames > 0) {
        let fc = harness_frame_count()
        if (fc == g_max_frames - 1) {
            vk_live_save_screenshot("one_source_path_tracer_proof.png")
            print("saved one_source_path_tracer_proof.png\n")
        }
        if (fc >= g_max_frames) {
            request_exit()
        }
    }
}

[export]
def shutdown() {
    // wind down any in-flight CPU pass before the Vulkan objects go away
    g_epoch++
    if (g_threads_status != null) {
        g_threads_status |> join()
        var st = g_threads_status
        unsafe {
            job_status_remove(st)
        }
        g_threads_status = null
    }
    delete g_quad
    delete g_sampler
    delete g_sbt
    delete g_rt
    delete g_tlas
    delete g_blas
    delete g_comp
    delete g_disp_img
    delete g_accum_img
    delete g_staging
    delete g_tbuf
    delete g_vbuf
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
