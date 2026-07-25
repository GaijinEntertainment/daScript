options gen2
options indenting = 4

// scratch probe: OUR dasSpirv port of llama.cpp's matmul_q8_0_f32_f16acc_aligned_l config vs THEIR
// compiled spv, same device buffers, one shape (the 24-TF headline: m=9728 k=2560 n=512).
// Their exact geometry: BM=128 BN=128 BK=32, 4 warps in 2x2, each warp 64x64 = 4x4 f16acc fragments
// (16 MMAs per TK step, A frag cached per row, B frag loaded per MMA), shared f16 pairs SST=BK/2+4.
// A is the gguf block_q8_0 byte stream (d f16 + 32 qs = 17 uint16s), dequantized during staging;
// B is f32, converted to f16 during staging. Success = matching their TFLOPS on the same harness.

require vulkan public
require vulkan/vulkan_boost public
require spirv/spirv_shader
require spirv/spirv_builtins public
require daslib/fio
require math

let SPV_PATH = "D:/Work/llama.cpp/build-vulkan/ggml/src/ggml-vulkan/vulkan-shaders.spv/matmul_q8_0_f32_f16acc_cm1.spv"

let N = 512              // tokens (all in-graph prefill GEMMs run n=512)
let DISPATCHES = 32

let SST = 20             // shared row stride in uints (f16 pairs): BK/2/2 uints data (8) ... 16 f16 + 4 pad v2 = 20 v2 units == 20 uints? (v2=2 f16=1 uint)

// their l_warptile_mmq spec for the reference pipeline
let SPEC = fixed_array<uint>(128u, 128u, 128u, 32u, 64u, 64u, 2u, 16u, 16u, 16u, 32u, 1u)

// ===== OUR kernel =====
// bindings: 0 = A as uint16 view of gguf q8_0 blocks (17 u16/block), 1 = B f32, 2 = D f32 (ours),
// 3 = meta [0]=M [1]=K [2]=NBK (runtime shape — the integration-realistic form)
var @ssbo @binding = 0 vk_aq : array<uint16>
var @ssbo @binding = 1 vk_bf : array<float>
var @ssbo @binding = 2 vk_y : array<float>
var @ssbo @binding = 3 vk_meta : array<uint>

var @workgroup buf_a : uint[2560]   // 128 rows x SST uints (f16 pairs, K contiguous)
var @workgroup buf_b : uint[2560]   // 128 cols x SST

[compute_shader(local_size_x=128, name="ourmm_spv")]
def ourmm_kernel {
    let mm = int(vk_meta[0])
    let kk = int(vk_meta[1])
    let nbk = int(vk_meta[2])
    let ir = int(gl_WorkGroupID.x)            // M tile
    let ic = int(gl_WorkGroupID.y)            // N tile
    let gm0 = ir * 128
    let gn0 = ic * 128
    let tid = int(gl_LocalInvocationID.x)
    let warp_i = int(gl_SubgroupID)
    let warp_r = warp_i % 2                   // BM/WM = 2
    let warp_c = warp_i / 2
    // staging map: 8 uint-pair slots per 32-K block row; 16 rows per pass
    let lw = tid % 8                          // which pair-of-u16 (4 qs) in the row's block
    let lr0 = tid / 8                         // row base 0..15
    var acc00 : coopmatAcc_f16_16x16
    var acc01 : coopmatAcc_f16_16x16
    var acc02 : coopmatAcc_f16_16x16
    var acc03 : coopmatAcc_f16_16x16
    var acc10 : coopmatAcc_f16_16x16
    var acc11 : coopmatAcc_f16_16x16
    var acc12 : coopmatAcc_f16_16x16
    var acc13 : coopmatAcc_f16_16x16
    var acc20 : coopmatAcc_f16_16x16
    var acc21 : coopmatAcc_f16_16x16
    var acc22 : coopmatAcc_f16_16x16
    var acc23 : coopmatAcc_f16_16x16
    var acc30 : coopmatAcc_f16_16x16
    var acc31 : coopmatAcc_f16_16x16
    var acc32 : coopmatAcc_f16_16x16
    var acc33 : coopmatAcc_f16_16x16
    var kb = 0
    while (kb < nbk) {
        // stage A: 128 rows x one q8_0 block each — d = u16[ib*17], qs pairs at u16[ib*17+1..16]
        for [unroll] (p in range(8)) {
            let row = lr0 + p * 16
            let ib = (gm0 + row) * nbk + kb
            let d = unpackHalf2x16(uint(vk_aq[ib * 17])).x
            let q0 = uint(vk_aq[ib * 17 + 1 + lw * 2])
            let q1 = uint(vk_aq[ib * 17 + 2 + lw * 2])
            let v = float4(int4(unpack8(int(q0 | (q1 << 16u))))) * d
            let o = row * SST + lw * 2
            buf_a[o] = packHalf2x16(v.xy)
            buf_a[o + 1] = packHalf2x16(v.zw)
        }
        // stage B: 128 cols x 32 K f32 -> f16 pairs
        for [unroll] (p in range(8)) {
            let col = lr0 + p * 16
            let base = (gn0 + col) * kk + kb * 32 + lw * 4
            let v = float4(vk_bf[base], vk_bf[base + 1], vk_bf[base + 2], vk_bf[base + 3])
            let o = col * SST + lw * 2
            buf_b[o] = packHalf2x16(v.xy)
            buf_b[o + 1] = packHalf2x16(v.zw)
        }
        barrier()
        // 2 TK steps x (4 A rows x 4 B cols): A cached per row, B loaded per MMA — their order
        for [unroll] (ks in range(2)) {
            var ca : coopmatA_f16_16x16
            var cb : coopmatB_f16_16x16
            coopmatLoad(ca, buf_a, (warp_r * 64) * SST + ks * 8, SST, 0)
            coopmatLoad(cb, buf_b, (warp_c * 64) * SST + ks * 8, SST, 1)
            acc00 = coopmatMulAdd(ca, cb, acc00)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 16) * SST + ks * 8, SST, 1)
            acc01 = coopmatMulAdd(ca, cb, acc01)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 32) * SST + ks * 8, SST, 1)
            acc02 = coopmatMulAdd(ca, cb, acc02)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 48) * SST + ks * 8, SST, 1)
            acc03 = coopmatMulAdd(ca, cb, acc03)
            coopmatLoad(ca, buf_a, (warp_r * 64 + 16) * SST + ks * 8, SST, 0)
            coopmatLoad(cb, buf_b, (warp_c * 64) * SST + ks * 8, SST, 1)
            acc10 = coopmatMulAdd(ca, cb, acc10)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 16) * SST + ks * 8, SST, 1)
            acc11 = coopmatMulAdd(ca, cb, acc11)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 32) * SST + ks * 8, SST, 1)
            acc12 = coopmatMulAdd(ca, cb, acc12)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 48) * SST + ks * 8, SST, 1)
            acc13 = coopmatMulAdd(ca, cb, acc13)
            coopmatLoad(ca, buf_a, (warp_r * 64 + 32) * SST + ks * 8, SST, 0)
            coopmatLoad(cb, buf_b, (warp_c * 64) * SST + ks * 8, SST, 1)
            acc20 = coopmatMulAdd(ca, cb, acc20)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 16) * SST + ks * 8, SST, 1)
            acc21 = coopmatMulAdd(ca, cb, acc21)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 32) * SST + ks * 8, SST, 1)
            acc22 = coopmatMulAdd(ca, cb, acc22)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 48) * SST + ks * 8, SST, 1)
            acc23 = coopmatMulAdd(ca, cb, acc23)
            coopmatLoad(ca, buf_a, (warp_r * 64 + 48) * SST + ks * 8, SST, 0)
            coopmatLoad(cb, buf_b, (warp_c * 64) * SST + ks * 8, SST, 1)
            acc30 = coopmatMulAdd(ca, cb, acc30)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 16) * SST + ks * 8, SST, 1)
            acc31 = coopmatMulAdd(ca, cb, acc31)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 32) * SST + ks * 8, SST, 1)
            acc32 = coopmatMulAdd(ca, cb, acc32)
            coopmatLoad(cb, buf_b, (warp_c * 64 + 48) * SST + ks * 8, SST, 1)
            acc33 = coopmatMulAdd(ca, cb, acc33)
        }
        barrier()
        kb++
    }
    // stores: D[n][m] (stride M), fragment M-dim contiguous = column-major
    let dr = gm0 + warp_r * 64
    let dc = gn0 + warp_c * 64
    var accw : coopmatAcc_f32_16x16
    coopmatConvert(accw, acc00)
    coopmatStore(accw, vk_y, dc * mm + dr, mm, 1)
    coopmatConvert(accw, acc01)
    coopmatStore(accw, vk_y, (dc + 16) * mm + dr, mm, 1)
    coopmatConvert(accw, acc02)
    coopmatStore(accw, vk_y, (dc + 32) * mm + dr, mm, 1)
    coopmatConvert(accw, acc03)
    coopmatStore(accw, vk_y, (dc + 48) * mm + dr, mm, 1)
    coopmatConvert(accw, acc10)
    coopmatStore(accw, vk_y, dc * mm + dr + 16, mm, 1)
    coopmatConvert(accw, acc11)
    coopmatStore(accw, vk_y, (dc + 16) * mm + dr + 16, mm, 1)
    coopmatConvert(accw, acc12)
    coopmatStore(accw, vk_y, (dc + 32) * mm + dr + 16, mm, 1)
    coopmatConvert(accw, acc13)
    coopmatStore(accw, vk_y, (dc + 48) * mm + dr + 16, mm, 1)
    coopmatConvert(accw, acc20)
    coopmatStore(accw, vk_y, dc * mm + dr + 32, mm, 1)
    coopmatConvert(accw, acc21)
    coopmatStore(accw, vk_y, (dc + 16) * mm + dr + 32, mm, 1)
    coopmatConvert(accw, acc22)
    coopmatStore(accw, vk_y, (dc + 32) * mm + dr + 32, mm, 1)
    coopmatConvert(accw, acc23)
    coopmatStore(accw, vk_y, (dc + 48) * mm + dr + 32, mm, 1)
    coopmatConvert(accw, acc30)
    coopmatStore(accw, vk_y, dc * mm + dr + 48, mm, 1)
    coopmatConvert(accw, acc31)
    coopmatStore(accw, vk_y, (dc + 16) * mm + dr + 48, mm, 1)
    coopmatConvert(accw, acc32)
    coopmatStore(accw, vk_y, (dc + 32) * mm + dr + 48, mm, 1)
    coopmatConvert(accw, acc33)
    coopmatStore(accw, vk_y, (dc + 48) * mm + dr + 48, mm, 1)
}

// ===== harness =====

def private s8b(v : int) : uint8 => uint8(v & 0xFF)

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

def private put_half_at(var buf : array<uint8>; byte_off : int; v : float) {
    let bits = packHalf2x16(float2(v, 0.0)) & 0xFFFFu
    buf[byte_off] = uint8(bits & 0xFFu)
    buf[byte_off + 1] = uint8((bits >> 8u) & 0xFFu)
}

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

def private record_chain(rawcmd : VkCommandBuffer; rpipe : VkPipeline; rlayout : VkPipelineLayout;
                         var rset : VkDescriptorSet; pc : uint[17]; use_pc : bool; gx, gy : uint; count : int) {
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
        if (use_pc) {
            vkCmdPushConstants(rawcmd, rlayout, flags, 0u, 68u, addr<void?>(pcl[0]))
        }
        for (i in range(count)) {
            vkCmdDispatch(rawcmd, gx, gy, 1u)
            if (i + 1 < count) {
                let df : VkDependencyFlags
                vkCmdPipelineBarrier(rawcmd, stg, stg, df, 1u, addr(mbar), 0u, null, 0u, null)
            }
        }
    }
}

def private write_set4(var device : Device; set_ : VkDescriptorSet; b0, b1, b2, b3 : DevBuf) {
    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(set_), dstBinding = 0u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(b0.buf), range_ = uint64(b0.bytes)))
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(set_), dstBinding = 1u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    w1.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(b1.buf), range_ = uint64(b1.bytes)))
    writes |> emplace(w1)
    var w2 = WriteDescriptorSet(dstSet = vk_value_to_boost(set_), dstBinding = 2u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    w2.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(b2.buf), range_ = uint64(b2.bytes)))
    writes |> emplace(w2)
    var w3 = WriteDescriptorSet(dstSet = vk_value_to_boost(set_), dstBinding = 3u, descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    w3.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(b3.buf), range_ = uint64(b3.bytes)))
    writes |> emplace(w3)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)
}

// one warmup then DISPATCHES-chain reps; returns best us/dispatch
def private bench_pipe(var device : Device; var pool : CommandPool; queue : VkQueue;
                       rpipe : VkPipeline; rlayout : VkPipelineLayout; var set_ : VkDescriptorSet;
                       pc : uint[17]; use_pc : bool; gx, gy : uint) : double {
    run_cmd_sync(device, pool, queue) $(cmd) {
        record_chain(boost_value_to_vk(cmd), rpipe, rlayout, set_, pc, use_pc, gx, gy, 1)
    }
    var best_us = 1.0e30lf
    for (_rep in range(5)) {
        let t0 = ref_time_ticks()
        run_cmd_sync(device, pool, queue) $(cmd) {
            record_chain(boost_value_to_vk(cmd), rpipe, rlayout, set_, pc, use_pc, gx, gy, DISPATCHES)
        }
        let us = double(get_time_usec(t0)) / double(DISPATCHES)
        if (us < best_us) { best_us = us }
    }
    return best_us
}

def private make_raw_pipeline(dev : VkDevice; shader_words : array<uint>; var rlayout : VkPipelineLayout;
                              use_spec : bool) : VkPipeline {
    var words := shader_words
    var smci = VkShaderModuleCreateInfo()
    smci.codeSize = uint64(length(words) * 4)
    var raw_shader : VkShaderModule
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
    stage.pName = "main"
    var cp = VkComputePipelineCreateInfo()
    cp.layout = rlayout
    cp.flags.capture_statistics_khr = true
    var raw_pipe : VkPipeline
    unsafe {
        smci.pCode = addr(words[0])
        vk_check(vkCreateShaderModule(dev, smci, null, raw_shader), null)
        stage.module_ = raw_shader
        if (use_spec) {
            spec_info.pMapEntries = addr(entries[0])
            spec_info.pData = addr<void?>(spec_vals[0])
            stage.pSpecializationInfo = addr(spec_info)
        }
        cp.stage = stage
        vk_check(vkCreateComputePipelines(dev, null, 1u, addr(cp), null, addr(raw_pipe)), null)
    }
    delete words
    return raw_pipe
}

[export]
def main {
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return
    }
    var their_words : array<uint>
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
    their_words |> resize(length(spv_bytes) / 4)
    for (i in range(length(their_words))) {
        their_words[i] = uint(spv_bytes[i * 4]) | (uint(spv_bytes[i * 4 + 1]) << 8u) | (uint(spv_bytes[i * 4 + 2]) << 16u) | (uint(spv_bytes[i * 4 + 3]) << 24u)
    }

    var inscope instance <- create_instance("coopmat mulmm port", make_api_version(1u, 3u, 0u))
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
    var cpci : CommandPoolCreateInfo
    cpci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, cpci)

    // shared descriptor layout: 4 storage buffers (theirs uses 0-2, ours also binds meta at 3)
    var dslci : DescriptorSetLayoutCreateInfo
    dslci.pBindings |> reserve(4)
    for (bi in range(4)) {
        var b0 = DescriptorSetLayoutBinding(binding = uint(bi), descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
        b0.stageFlags.compute = true
        dslci.pBindings |> emplace(b0)
    }
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 32u
    dpci.pPoolSizes |> push(DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 128u))
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    // pipeline layouts: theirs (push constants), ours (none)
    var plci_t : PipelineLayoutCreateInfo
    plci_t.pSetLayouts <- [weak_copy(set_layout)]
    var pcr : PushConstantRange
    pcr.stageFlags.compute = true
    pcr.size = 17u * 4u
    plci_t.pPushConstantRanges |> push(pcr)
    var inscope layout_t <- create_pipeline_layout(device, plci_t)
    var plci_o : PipelineLayoutCreateInfo
    plci_o.pSetLayouts <- [weak_copy(set_layout)]
    var inscope layout_o <- create_pipeline_layout(device, plci_o)

    let pipe_theirs = make_raw_pipeline(dev, their_words, boost_value_to_vk(layout_t), true)
    var our_words <- clone_to_move(ourmm_spv)
    let pipe_ours = make_raw_pipeline(dev, our_words, boost_value_to_vk(layout_o), false)

    // stats for ours (theirs already dumped by _lcpp_mulmm_isolated)
    var inscope pw <- vk_value_to_boost(pipe_ours)
    pw._needs_delete = true
    pw._device = unsafe(reinterpret<uint64>(dev))
    if (device_extension_available(phys, "VK_KHR_pipeline_executable_properties")) {
        let report = dump_pipeline_executables(device, pw)
        fwrite("D:/Work/daScript/_ourmm_stats.txt", report)
    }

    var ms : array<int> <- [1024, 4096, 9728, 2560, 3072, 4096]
    var ks : array<int> <- [2560, 2560, 2560, 9728, 3072, 14336]
    for (m, k in ms, ks) {
        let nbk = k / 32
        // data: biased (non-cancelling) synthetic, f16-exact values
        var a_bytes : array<uint8>
        a_bytes |> resize(m * nbk * 34)
        for (r in range(m)) {
            for (b in range(nbk)) {
                let off = (r * nbk + b) * 34
                put_half_at(a_bytes, off, float((r + b) % 4 + 1) * 0.03125)
                for (j in range(32)) {
                    a_bytes[off + 2 + j] = s8b((r + b * 32 + j) % 5 - 1)
                }
            }
        }
        var b_bytes : array<uint8>
        b_bytes |> resize(N * k * 4)
        for (r in range(N)) {
            for (j in range(k)) {
                put_f32(b_bytes, r * k + j, float((r * 2 + j) % 7 - 2) * 0.03125)
            }
        }
        var meta_bytes : array<uint8>
        meta_bytes |> resize(16)
        unsafe {
            var mp = addr<uint?>(meta_bytes[0])
            mp[0] = uint(m); mp[1] = uint(k); mp[2] = uint(nbk); mp[3] = 0u
        }
        var d_theirs : array<uint8>
        d_theirs |> resize(m * N * 4)
        var d_ours : array<uint8>
        d_ours |> resize(m * N * 4)

        var da <- make_dev_buf(device, phys, length(a_bytes))
        var db <- make_dev_buf(device, phys, length(b_bytes))
        var dmeta <- make_dev_buf(device, phys, length(meta_bytes))
        var dt <- make_dev_buf(device, phys, length(d_theirs))
        var douts <- make_dev_buf(device, phys, length(d_ours))
        staging_roundtrip(device, phys, pool, queue, da, a_bytes, true)
        staging_roundtrip(device, phys, pool, queue, db, b_bytes, true)
        staging_roundtrip(device, phys, pool, queue, dmeta, meta_bytes, true)

        var set_theirs : VkDescriptorSet
        var set_ours : VkDescriptorSet
        var dsai = VkDescriptorSetAllocateInfo()
        dsai.descriptorPool = boost_value_to_vk(desc_pool)
        dsai.descriptorSetCount = 1u
        var rsl = boost_value_to_vk(set_layout)
        unsafe {
            dsai.pSetLayouts = addr(rsl)
            vk_check(vkAllocateDescriptorSets(dev, dsai, addr(set_theirs)), null)
            vk_check(vkAllocateDescriptorSets(dev, dsai, addr(set_ours)), null)
        }
        write_set4(device, set_theirs, da, db, dt, dmeta)
        write_set4(device, set_ours, da, db, douts, dmeta)

        var pc : uint[17]
        pc[0] = uint(m); pc[1] = uint(N); pc[2] = uint(k)
        pc[3] = uint(k); pc[4] = uint(k); pc[5] = uint(m)
        pc[6] = uint(k * m); pc[7] = uint(k * N); pc[8] = uint(m * N)
        pc[9] = 0u; pc[10] = 1u
        pc[11] = uint(k)
        pc[12] = 1u; pc[13] = 1u; pc[14] = 1u; pc[15] = 1u
        pc[16] = uint(N)
        let gx = uint(m / 128)
        let gy = uint(N / 128)
        let us_theirs = bench_pipe(device, pool, queue, pipe_theirs, boost_value_to_vk(layout_t), set_theirs, pc, true, gx, gy)
        let us_ours = bench_pipe(device, pool, queue, pipe_ours, boost_value_to_vk(layout_o), set_ours, pc, false, gx, gy)
        let ops = 2.0lf * double(m) * double(N) * double(k)
        let tf_theirs = float(ops / (us_theirs * 1.0e6lf))
        let tf_ours = float(ops / (us_ours * 1.0e6lf))

        // outputs: D[n][m] stride m both — direct elementwise diff
        staging_roundtrip(device, phys, pool, queue, dt, d_theirs, false)
        staging_roundtrip(device, phys, pool, queue, douts, d_ours, false)
        var bad = 0
        var maxrel = 0.0
        for (s in range(4096)) {
            let i = (s * 121639) % (m * N)
            let a = f32_of(d_theirs, i)
            let b = f32_of(d_ours, i)
            let rel = abs(a - b) / max(max(abs(a), abs(b)), 0.5)
            if (rel > maxrel) { maxrel = rel }
            if (rel > 0.02) { bad++ }
        }
        let verdict = bad == 0 ? "PASS" : "FAIL({bad}/4096, maxrel {maxrel})"
        print("m={m} k={k} n={N}: theirs {us_theirs:.0f} us = {tf_theirs:.2f} TF | OURS {us_ours:.0f} us = {tf_ours:.2f} TF ({100.0 * tf_ours / tf_theirs:.1f}%) | diff {verdict}\n")

        delete a_bytes
        delete b_bytes
        delete meta_bytes
        delete d_theirs
        delete d_ours
        delete da
        delete db
        delete dmeta
        delete dt
        delete douts
    }
    delete ms
    delete ks
    delete their_words
    delete spv_bytes
}
