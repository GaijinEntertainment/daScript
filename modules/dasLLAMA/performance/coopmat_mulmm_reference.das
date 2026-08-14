options gen2
options indenting = 4

// scratch probe: run llama.cpp's OWN matmul_q8_0_f32_f16acc_aligned_l pipeline (the cm1/KHR-coopmat
// mul_mm that clocks 13-27 TFLOPS in-graph) in OUR harness, on the exact in-graph shapes, isolated.
// SPV straight from their build dir; spec constants = l_warptile_mmq + ALIGNED; push constants =
// vk_mat_mat_push_constants exactly as the GGML_VULKAN_DEBUG dispatch lines showed. The question this
// answers: does 17 TFLOPS exist OUTSIDE their graph, or is their own test-backend-ops' 1.78 the truth?

require vulkan public
require vulkan/vulkan_boost public
require spirv/spirv_shader
require spirv/spirv_builtins public
require daslib/fio
require math

let SPV_PATH = "D:/Work/llama.cpp/build-vulkan/ggml/src/ggml-vulkan/vulkan-shaders.spv/matmul_q8_0_f32_f16acc_cm1.spv"
let N_TOK = 512          // n (batch/tokens) — all in-graph prefill GEMMs run n=512
let DISPATCHES = 32      // per timed submit, compute-barrier separated (the in-graph pattern)

// l_warptile_mmq on this device (subgroup 32, KHR coopmat 16x16x16) + ALIGNED=1 at id 11:
// BLOCK_SIZE BM BN BK WM WN WMITER TM TN TK WARP ALIGNED
let SPEC = fixed_array<uint>(128u, 128u, 128u, 32u, 64u, 64u, 2u, 16u, 16u, 16u, 32u, 1u)

struct Shape {
    m : int      // output features (weight rows)
    k : int      // contraction
    graph_tf : float   // what the perf logger showed in-graph (qwen3-4B/llama-3B runs)
    tbo_tf : float     // what their test-backend-ops showed for the same shape
}

def private s8b(v : int) : uint8 => uint8(v & 0xFF)

def private s8_ref(v : uint8) : int {
    var x = int(v)
    if (x > 127) { x -= 256 }
    return x
}

def private put_half_at(var buf : array<uint8>; byte_off : int; v : float) {
    let bits = packHalf2x16(float2(v, 0.0)) & 0xFFFFu
    buf[byte_off] = uint8(bits & 0xFFu)
    buf[byte_off + 1] = uint8((bits >> 8u) & 0xFFu)
}

def private f32_of(buf : array<uint8>; i : int) : float {
    let w = uint(buf[i * 4]) | (uint(buf[i * 4 + 1]) << 8u) | (uint(buf[i * 4 + 2]) << 16u) | (uint(buf[i * 4 + 3]) << 24u)
    return unsafe(reinterpret<float>(w))
}

def private put_f32(var buf : array<uint8>; i : int; v : float) {
    let w = unsafe(reinterpret<uint>(v))
    buf[i * 4] = uint8(w & 0xFFu)
    buf[i * 4 + 1] = uint8((w >> 8u) & 0xFFu)
    buf[i * 4 + 2] = uint8((w >> 16u) & 0xFFu)
    buf[i * 4 + 3] = uint8((w >> 24u) & 0xFFu)
}

// device-local buffer + staged upload (matches the graph: A/B/D live in DEVICE_LOCAL memory)
struct DevBuf {
    buf : Buffer
    mem : DeviceMemory
    bytes : int
}

def private make_dev_buf(var device : Device; phys : VkPhysicalDevice; bytes : int) : DevBuf {
    var r : DevBuf
    r.bytes = bytes
    var bci : BufferCreateInfo
    bci.size = uint64(bytes)
    bci.usage.storage_buffer = true
    bci.usage.transfer_dst = true
    bci.usage.transfer_src = true
    r.buf <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(r.buf), req)
    var want : VkMemoryPropertyFlags
    want.device_local = true
    let mai = MemoryAllocateInfo(allocationSize = req.size, memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    r.mem <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(r.buf), boost_value_to_vk(r.mem), 0ul), null)
    return <- r
}

def private staging_roundtrip(var device : Device; phys : VkPhysicalDevice; var pool : CommandPool;
                              queue : VkQueue; var db : DevBuf; var host : array<uint8>; upload : bool) {
    var bci : BufferCreateInfo
    bci.size = uint64(db.bytes)
    bci.usage.transfer_src = true
    bci.usage.transfer_dst = true
    var inscope stage <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(stage), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size, memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope smem <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(stage), boost_value_to_vk(smem), 0ul), null)
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    if (upload) {
        unsafe {
            vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(smem), 0ul, uint64(db.bytes), mf, addr(mapped)), null)
            memcpy(mapped, addr<void?>(host[0]), db.bytes)
        }
        vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(smem))
        run_cmd_sync(device, pool, queue) $(cmd) {
            var region : VkBufferCopy
            region.size = uint64(db.bytes)
            unsafe {
                vkCmdCopyBuffer(boost_value_to_vk(cmd), boost_value_to_vk(stage), boost_value_to_vk(db.buf), 1u, addr(region))
            }
        }
    } else {
        run_cmd_sync(device, pool, queue) $(cmd) {
            var region : VkBufferCopy
            region.size = uint64(db.bytes)
            unsafe {
                vkCmdCopyBuffer(boost_value_to_vk(cmd), boost_value_to_vk(db.buf), boost_value_to_vk(stage), 1u, addr(region))
            }
        }
        unsafe {
            vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(smem), 0ul, uint64(db.bytes), mf, addr(mapped)), null)
            memcpy(addr<void?>(host[0]), mapped, db.bytes)
        }
        vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(smem))
    }
}

// all-raw recording (no boost-wrapper captures): bind, push constants, count dispatches with
// compute-compute barriers between (the in-graph dependent-chain pattern)
def private record_gemm(rawcmd : VkCommandBuffer; rpipe : VkPipeline; rlayout : VkPipelineLayout;
                        var rset : VkDescriptorSet; pc : uint[17]; gx, gy : uint; count : int) {
    var flags : VkShaderStageFlags
    flags.compute = true
    var mbar = VkMemoryBarrier()
    mbar.srcAccessMask.shader_write = true
    mbar.dstAccessMask.shader_read = true
    mbar.dstAccessMask.shader_write = true
    var stg : VkPipelineStageFlags
    stg.compute_shader = true
    var pcl = pc
    unsafe {
        vkCmdBindPipeline(rawcmd, VkPipelineBindPoint.COMPUTE, rpipe)
        vkCmdBindDescriptorSets(rawcmd, VkPipelineBindPoint.COMPUTE, rlayout, 0u, 1u, addr(rset), 0u, null)
        vkCmdPushConstants(rawcmd, rlayout, flags, 0u, 68u, addr<void?>(pcl[0]))
        for (i in range(count)) {
            vkCmdDispatch(rawcmd, gx, gy, 1u)
            if (i + 1 < count) {
                let df : VkDependencyFlags
                vkCmdPipelineBarrier(rawcmd, stg, stg, df, 1u, addr(mbar), 0u, null, 0u, null)
            }
        }
    }
}

[export]
def main {   // nolint:STYLE037,STYLE038 - linear Vulkan probe; every `var inscope` handle must outlive the shape loop, so the setup cannot move into helpers
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return
    }
    var spv_bytes : array<uint8>
    fopen(SPV_PATH, "rb") $(f) {
        if (f != null) {
            fmap(f) $(data) {
                spv_bytes |> resize(length(data))
                unsafe {
                    memcpy(addr<void?>(spv_bytes[0]), addr<void? const?>(data[0]), length(data))
                }
            }
        }
    }
    if (empty(spv_bytes)) {
        print("FAILED to read {SPV_PATH}\n")
        return
    }
    var words : array<uint>
    words |> resize(length(spv_bytes) / 4)
    for (i in range(length(words))) {
        words[i] = uint(spv_bytes[i * 4]) | (uint(spv_bytes[i * 4 + 1]) << 8u) | (uint(spv_bytes[i * 4 + 2]) << 16u) | (uint(spv_bytes[i * 4 + 3]) << 24u)
    }
    print("loaded {SPV_PATH}: {length(spv_bytes)} bytes\n")

    var inscope instance <- create_instance("lcpp mulmm isolated", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    if (!cooperative_matrix_supported(phys)) {
        to_log(LOG_WARNING, "device lacks VK_KHR_cooperative_matrix; skipping\n")
        return
    }
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_coopmat_full_subgroups(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)

    // descriptor layout: 3 storage buffers (A q8_0 blocks, B f32, D f32) — their binding order
    var dslci : DescriptorSetLayoutCreateInfo
    dslci.pBindings |> reserve(3)
    for (bi in range(3)) {
        var b0 = DescriptorSetLayoutBinding(binding = uint(bi), descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
        b0.stageFlags.compute = true
        dslci.pBindings |> emplace(b0)
    }
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 8u
    dpci.pPoolSizes |> push(DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 24u))
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    // pipeline layout: set 0 + the 17-word vk_mat_mat_push_constants block
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var pcr : PushConstantRange
    pcr.stageFlags.compute = true
    pcr.size = 17u * 4u
    plci.pPushConstantRanges |> push(pcr)
    var inscope pipe_layout <- create_pipeline_layout(device, plci)

    // pipeline: their spv + their spec constants; require_full_subgroups like ggml_vk_create_pipeline
    var inscope shader <- create_shader_module(device, words)
    var entries : VkSpecializationMapEntry[12]
    for (i in range(12)) {
        entries[i].constantID = uint(i)
        entries[i].offset = uint(i * 4)
        entries[i].size = uint64(4)
    }
    var spec_vals = SPEC
    var spec_info : VkSpecializationInfo
    spec_info.mapEntryCount = 12u
    spec_info.dataSize = uint64(48)
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.flags.require_full_subgroups = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = "main"
    var cp = VkComputePipelineCreateInfo()
    cp.layout = boost_value_to_vk(pipe_layout)
    cp.flags.capture_statistics_khr = true
    var raw_pipe : VkPipeline
    unsafe {
        spec_info.pMapEntries = addr(entries[0])
        spec_info.pData = addr<void?>(spec_vals[0])
        stage.pSpecializationInfo = addr(spec_info)
        cp.stage = stage
        vk_check(vkCreateComputePipelines(dev, null, 1u, addr(cp), null, addr(raw_pipe)), null)
    }
    var inscope pipeline <- vk_value_to_boost(raw_pipe)
    pipeline._needs_delete = true
    pipeline._device = unsafe(reinterpret<uint64>(dev))
    if (device_extension_available(phys, "VK_KHR_pipeline_executable_properties")) {
        let report = dump_pipeline_executables(device, pipeline)
        if (fwrite("D:/Work/daScript/_lcpp_mulmm_stats.txt", report)) {
            print("their pipeline stats -> D:/Work/daScript/_lcpp_mulmm_stats.txt\n")
        }
    }

    var cpci : CommandPoolCreateInfo
    cpci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, cpci)

    // the shapes: in-graph rate vs their-test rate, from the perf-logger and test-backend-ops sessions
    var shapes <- [
        Shape(m = 1024, k = 2560, graph_tf = 13.2, tbo_tf = 1.66),
        Shape(m = 4096, k = 2560, graph_tf = 15.1, tbo_tf = 1.70),
        Shape(m = 9728, k = 2560, graph_tf = 17.2, tbo_tf = 1.38),
        Shape(m = 2560, k = 9728, graph_tf = 18.0, tbo_tf = 3.70),
        Shape(m = 3072, k = 3072, graph_tf = 23.9, tbo_tf = 1.97),
        Shape(m = 4096, k = 14336, graph_tf = 0.0, tbo_tf = 1.78)]

    for (sh in shapes) {
        let m = sh.m
        let k = sh.k
        let n = N_TOK
        let nbk = k / 32
        // A: gguf block_q8_0 rows — d(f16) + 32 qs, 34B/block; crib's exact-in-f16 synthetic pattern
        var a_bytes : array<uint8>
        a_bytes |> resize(m * nbk * 34)
        for (r in range(m)) {
            for (b in range(nbk)) {
                let off = (r * nbk + b) * 34
                put_half_at(a_bytes, off, float((r + b) % 4 + 1) * 0.03125)
                for (j in range(32)) {
                    a_bytes[off + 2 + j] = s8b((r + b * 32 + j) % 5 - 1)   // mean +0.2: sums must NOT cancel (vacuous-validation guard)
                }
            }
        }
        // B: n rows x k f32, values exact in f16 (ints/32)
        var b_bytes : array<uint8>
        b_bytes |> resize(n * k * 4)
        for (r in range(n)) {
            for (j in range(k)) {
                put_f32(b_bytes, r * k + j, float((r * 2 + j) % 7 - 2) * 0.03125)   // mean +0.14, same guard
            }
        }
        var d_bytes : array<uint8>
        d_bytes |> resize(m * n * 4)

        var da <- make_dev_buf(device, phys, length(a_bytes))
        var db <- make_dev_buf(device, phys, length(b_bytes))
        var dd <- make_dev_buf(device, phys, length(d_bytes))
        staging_roundtrip(device, phys, pool, queue, da, a_bytes, true)
        staging_roundtrip(device, phys, pool, queue, db, b_bytes, true)

        var dsai = VkDescriptorSetAllocateInfo()
        dsai.descriptorPool = boost_value_to_vk(desc_pool)
        dsai.descriptorSetCount = 1u
        var raw_set_layout = boost_value_to_vk(set_layout)
        var raw_set : VkDescriptorSet
        unsafe {
            dsai.pSetLayouts = addr(raw_set_layout)
            vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
        }
        var writes : array<WriteDescriptorSet>
        var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
        w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(da.buf), range_ = uint64(da.bytes)))
        writes |> emplace(w0)
        var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
        w1.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(db.buf), range_ = uint64(db.bytes)))
        writes |> emplace(w1)
        var w2 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 2u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
        w2.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(dd.buf), range_ = uint64(dd.bytes)))
        writes |> emplace(w2)
        let no_copies : array<CopyDescriptorSet>
        update_descriptor_sets(device, writes, no_copies)

        // push constants exactly as the in-graph debug line: strides in elements, k_split 1, padded_N = n
        var pc : uint[17]
        pc[0] = uint(m); pc[1] = uint(n); pc[2] = uint(k)
        pc[3] = uint(k); pc[4] = uint(k); pc[5] = uint(m)
        pc[6] = uint(k * m); pc[7] = uint(k * n); pc[8] = uint(m * n)
        pc[9] = 0u; pc[10] = 1u
        pc[11] = uint(k)   // k_split is the per-partition K SIZE — full K when unsplit (ggml_vk_matmul:8280)
        pc[12] = 1u; pc[13] = 1u; pc[14] = 1u; pc[15] = 1u
        pc[16] = uint(n)
        let gx = uint((m + 127) / 128)
        let gy = uint((n + 127) / 128)

        // warmup (also produces D for validation)
        run_cmd_sync(device, pool, queue) $(cmd) {
            record_gemm(boost_value_to_vk(cmd), raw_pipe, boost_value_to_vk(pipe_layout), raw_set, pc, gx, gy, 1)
        }
        // did the kernel actually write? nonzero count over D after ONE dispatch
        staging_roundtrip(device, phys, pool, queue, dd, d_bytes, false)
        var nonzero = 0
        for (i in range(m * n)) {
            if (f32_of(d_bytes, i) != 0.0) { nonzero++ }
        }
        var one_us = 0.0lf
        for (rep in range(3)) {
            let t1 = ref_time_ticks()
            run_cmd_sync(device, pool, queue) $(cmd) {
                record_gemm(boost_value_to_vk(cmd), raw_pipe, boost_value_to_vk(pipe_layout), raw_set, pc, gx, gy, 1)
            }
            let us1 = double(get_time_usec(t1))
            if (rep == 0 || us1 < one_us) { one_us = us1 }
        }
        var best_us = 1.0e30lf
        for (_rep in range(5)) {
            let t0 = ref_time_ticks()
            run_cmd_sync(device, pool, queue) $(cmd) {
                record_gemm(boost_value_to_vk(cmd), raw_pipe, boost_value_to_vk(pipe_layout), raw_set, pc, gx, gy, DISPATCHES)
            }
            let us = double(get_time_usec(t0)) / double(DISPATCHES)
            if (us < best_us) { best_us = us }
        }
        print("  [nonzero {nonzero}/{m * n} after 1 dispatch; single-dispatch wall {one_us:.0f} us]\n")
        let ops = 2.0lf * double(m) * double(n) * double(k)
        let tflops = float(ops / (best_us * 1.0e6lf))

        // validate a sample vs the f16-dequant reference (their math: A,B -> f16, f16acc MMA)
        staging_roundtrip(device, phys, pool, queue, dd, d_bytes, false)
        var bad = 0
        var maxrel = 0.0
        for (s in range(64)) {
            let rm = (s * 37) % m
            let rn = (s * 101) % n
            // f16acc semantics: 16-wide chunks accumulate in higher precision inside one MMA, the
            // running total is f16 between MMAs
            var running = 0.0
            var chunk = 0.0
            var cn = 0
            for (b in range(nbk)) {
                let off = (rm * nbk + b) * 34
                let d = unpackHalf2x16(uint(a_bytes[off]) | (uint(a_bytes[off + 1]) << 8u)).x
                for (j in range(32)) {
                    let aq = s8_ref(a_bytes[off + 2 + j])
                    let bv = float((rn * 2 + (b * 32 + j)) % 7 - 2) * 0.03125
                    chunk += float(float16(float(aq) * d)) * float(float16(bv))
                    cn++
                    if (cn == 16) {
                        running = float(float16(running + chunk))
                        chunk = 0.0
                        cn = 0
                    }
                }
            }
            let ref = running
            let got = f32_of(d_bytes, rn * m + rm)
            let rel = abs(got - ref) / max(abs(ref), 0.5)
            if (rel > maxrel) { maxrel = rel }
            if (rel > 0.03) { bad++ }
        }
        let verdict = bad == 0 ? "PASS" : "FAIL({bad}/64, maxrel {maxrel})"
        print("m={m} k={k} n={n}: {best_us:.0f} us/dispatch -> {tflops:.2f} TFLOPS  (in-graph {sh.graph_tf}, their-test {sh.tbo_tf})  {verdict}\n")

        delete a_bytes
        delete b_bytes
        delete d_bytes
        delete da
        delete db
        delete dd
    }
    delete shapes
    delete words
    delete spv_bytes
}
