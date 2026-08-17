options gen2
options _comment_hygiene = true
options indenting = 4

// Vulkan-specific dasSpirv shader subclass + annotations. Mirrors the dasGlsl/dasOpenGL split:
// dasSpirv declares abstract bind hooks on SpirvShader; this subclass overrides them to emit
// Vulkan-flavored bind functions, and three function-macro annotations ([vulkan_vertex_shader],
// [vulkan_fragment_shader], [vulkan_compute_shader]) instantiate the subclass.
//
// generate_bind_uniform walks every @uniform-tagged module global reachable from the shader,
// computes its std140 layout via spirv_emit::compute_block_layout (the same offsets the SPIR-V
// emitter uses to lay out the interface block — single source of truth), and emits one write_field
// call per std140 field at its byte offset. The whole body is wrapped in with_mapped_memory so
// vkMapMemory/vkUnmapMemory bookend a single map cycle per call.

module spirv_vulkan_shader shared public

// vulkan_boost (where with_mapped_memory + write_field live) is NOT a shared module, so this
// shared macro module can't require it directly. It's not needed at macro time anyway — we just
// emit symbolic ExprCall("with_mapped_memory") / ExprCall("write_field") and rely on the consumer
// (tutorial / app) module's `require vulkan/vulkan_boost public` for resolution at codegen time.
// Same trick dasOpenGL's glsl_opengl uses for glUniformAny without requiring opengl_boost.
require spirv/spirv_shader public
require spirv/spirv_emit
require daslib/shader_block_layout   // std140_block_writes / Std140Write / compute_block_layout (explicit, not via spirv_emit's re-export)
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost

class SpirvVulkanShader : SpirvShader {
    def override generate_bind_uniform_dummy(var func : FunctionPtr) {
        spirv_vulkan_shader::generate_bind_uniform_dummy(func)
    }
    def override generate_bind_uniform(fnMain : FunctionPtr; var fn : FunctionPtr) {
        spirv_vulkan_shader::generate_bind_uniform(fnMain, fn)
    }
    def override generate_push_constants_dummy(var func : FunctionPtr) {
        spirv_vulkan_shader::generate_push_constants_dummy(func)
    }
    def override generate_push_constants(fnMain : FunctionPtr; var fn : FunctionPtr) {
        spirv_vulkan_shader::generate_push_constants(fnMain, fn, shader_stage())
    }
}

[function_macro(name="vulkan_compute_shader")]
class SpirvVulkanComputeShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.Compute
}

[function_macro(name="vulkan_vertex_shader")]
class SpirvVulkanVertexShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.Vertex
}

[function_macro(name="vulkan_fragment_shader")]
class SpirvVulkanFragmentShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.Fragment
}

// VK_EXT_mesh_shader stages. SpirvShader.apply reads the mesh output config (max_vertices /
// max_primitives / output_topology) from the annotation args when shader_stage() is Mesh, so these
// behave as [mesh_shader(...)] plus the Vulkan bind helpers. Pairs with create_device_mesh_shader.
[function_macro(name="vulkan_mesh_shader")]
class SpirvVulkanMeshShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.Mesh
}

[function_macro(name="vulkan_task_shader")]
class SpirvVulkanTaskShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.Task
}

// VK_KHR_ray_tracing_pipeline stages. SpirvShader handles @ray_payload / @incoming_ray_payload /
// @hit_attribute globals, the accelerationStructureEXT descriptor, and traceRayEXT; these add the
// Vulkan bind helpers. Pairs with create_device_ray_tracing + build_shader_binding_table.
[function_macro(name="vulkan_raygen_shader")]
class SpirvVulkanRayGenShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.RayGen
}

[function_macro(name="vulkan_miss_shader")]
class SpirvVulkanMissShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.Miss
}

[function_macro(name="vulkan_closest_hit_shader")]
class SpirvVulkanClosestHitShader : SpirvVulkanShader {
    def override shader_stage() : ShaderStage => ShaderStage.ClosestHit
}

// Reserve an empty bind_uniform_<shader>(device : Device; memory : DeviceMemory) function so
// consumer code referencing it resolves during the first inference pass. The body is filled in
// patch by generate_bind_uniform; until then `exports = true` flags the function as still dummy.
[macro_function]
def private generate_bind_uniform_dummy(fnMain : FunctionPtr) {
    var fn = new Function(at = fnMain.at, atDecl = fnMain.at, name := bind_uniform_function_name(fnMain))
    fn.flags.generated = true
    fn.flags.exports = true
    fn.result = new TypeDecl(at = fnMain.at, baseType = Type.tVoid)
    // const: caller's `ctx.device` / `ctx.ubo.memory` may be const in scope; this lets either form bind.
    var dev_type <- new TypeDecl(at = fnMain.at, baseType = Type.alias, alias := "Device")
    dev_type.flags.constant = true
    fn.arguments |> emplace_new() <| new Variable(at = fnMain.at, name := "device", _type <- dev_type)
    var mem_type <- new TypeDecl(at = fnMain.at, baseType = Type.alias, alias := "DeviceMemory")
    mem_type.flags.constant = true
    fn.arguments |> emplace_new() <| new Variable(at = fnMain.at, name := "memory", _type <- mem_type)
    fn.body = new ExprBlock(at = fnMain.at)
    if (!(compiling_module() |> add_function(fn))) {
        panic("can't reserve bind_uniform dummy function for {fnMain.name}")
    }
}

// Opaque resource markers (samplers, images, subpass input) bind via descriptor-set writes, not mapped-memory
// UBO uploads, so the @uniform / @push_constant walks MUST skip them. Keyed by NAME, not "zero fields" — shared
// sampler2D/image2D carry a GL-superset texture2D field. Keep in sync with dasSpirv's sampler_info/classify_global.
[macro_function]
def private is_opaque_resource_struct(st : Structure?) : bool {
    if (st == null) return false
    let n = "{st.name}"
    return (n == "sampler2D" || n == "sampler3D" || n == "samplerCube" || n == "sampler2DArray" ||
            n == "sampler2DShadow" || n == "image2D" || n == "image3D" ||
            n == "texture2D" || n == "sampler" || n == "subpassInput")
}

// Fill the dummy bind_uniform body: one write_field per std140 field of the FIRST @uniform block
// reachable from fnMain, wrapped in `with_mapped_memory(device, memory) <| $(ptr : void?)` — one
// map/unmap per call, host-visible memory ONLY. Samplers/images bind via descriptor writes, not here.
[macro_function]
def private generate_bind_uniform(fnMain : FunctionPtr; var fn : FunctionPtr) {
    var inner = new ExprBlock(at = fn.at)
    inner.blockFlags.isClosure = true
    inner.returnType = new TypeDecl(at = fn.at, baseType = Type.tVoid)
    var ptr_arg <- new Variable(at = fn.at, name := "ptr")
    ptr_arg._type = new TypeDecl(at = fn.at,
        baseType = Type.tPointer,
        firstType = new TypeDecl(at = fn.at, baseType = Type.tVoid)
    )
    // Macro-built Variables default to mutable; block params are const, so the sig must match.
    ptr_arg._type.flags.constant = true
    inner.arguments |> emplace(ptr_arg)

    // Collect the @uniform struct globals reachable from fnMain; only the first is auto-bound (see below).
    var ubo_globals : array<Variable?>
    collect_dependencies(fnMain) $(_vfun, vvar) {
        for (vv in vvar) {
            // Sampler/image markers are tStructure too but bind via descriptors; skip by name (empty() = belt-and-braces).
            if (!(vv.annotation |> find_arg("uniform") is tBool) ||
                vv._type == null || vv._type.baseType != Type.tStructure || vv._type.structType == null ||
                is_opaque_resource_struct(vv._type.structType) ||
                empty(vv._type.structType.fields)) continue
            ubo_globals |> push(vv)
        }
    }
    // nolint:STYLE015
    // Binds the FIRST reachable @uniform block only: one (device, memory) pair maps ONE allocation.
    // A shader may legitimately reach several blocks (a per-frame block plus a shared lookup table);
    // the rest are deliberately the host's. with_mapped_memory is valid ONLY for host-visible memory,
    // so host-coherent must never be conflated with device-local (staged, NOT mappable) — the host
    // uploads those through its own path.
    if (!empty(ubo_globals)) {
        let vv = ubo_globals[0]
        let st = vv._type.structType
        var layout_errors : array<string>
        var fields : array<BlockField>
        let layout = compute_block_layout(string(vv.name), st, fields, layout_errors)
        if (!layout.ok) {
            // Fail-closed: a no-op bind_uniform writes garbage at runtime. layout_errors holds exactly one entry here.
            let msg = empty(layout_errors) ? "(no diagnostic)" : layout_errors[0]
            macro_error(compiling_program(), vv.at,
                "[vulkan_*_shader]: @uniform '{vv.name}' std140 layout failed: {msg}")
            delete fields
            delete layout_errors
            unsafe { delete ubo_globals; }
            return
        }
        // Shared std140 walk — the SAME one the OpenGL binder drives, so both rails pack byte-identically.
        var writes : array<Std140Write>
        std140_block_writes(st, new ExprVar(at = vv.at, name := vv.name), 0, writes)
        for (w in writes) {
            var cll = new ExprCall(at = vv.at, name := "write_field")
            cll.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "ptr")
            cll.arguments |> emplace_new() <| new ExprConstUInt(at = vv.at, value = uint(w.offset))
            cll.arguments |> emplace(w.access)
            inner.list |> emplace(cll)
        }
        unsafe { delete writes; }
        delete fields
        delete layout_errors
    }
    unsafe { delete ubo_globals; }

    var mkblk = new ExprMakeBlock(at = fn.at, _block = inner)
    var wmm = new ExprCall(at = fn.at, name := "with_mapped_memory")
    wmm.arguments |> emplace_new() <| new ExprVar(at = fn.at, name := "device")
    wmm.arguments |> emplace_new() <| new ExprVar(at = fn.at, name := "memory")
    wmm.arguments |> emplace(mkblk)

    var body = new ExprBlock(at = fn.at)
    body.list |> emplace(wmm)
    fn.body = body
}

// Map ShaderStage to the underlying VkShaderStageFlagBits integer. The push-constants codegen emits
// VkShaderStageFlags(<value>) as a const bitfield-from-uint ctor because field-name assignment
// (`flags.vertex = true`) doesn't work through qmacro $i splicing. Values from VK_SHADER_STAGE_*_BIT.
def private stage_to_uint(stage : ShaderStage) : uint {
    if (stage == ShaderStage.Vertex) return 0x1u
    if (stage == ShaderStage.Fragment) return 0x10u
    if (stage == ShaderStage.Task) return 0x40u   // VK_SHADER_STAGE_TASK_BIT_EXT
    if (stage == ShaderStage.Mesh) return 0x80u   // VK_SHADER_STAGE_MESH_BIT_EXT
    if (stage == ShaderStage.RayGen) return 0x100u        // VK_SHADER_STAGE_RAYGEN_BIT_KHR
    if (stage == ShaderStage.ClosestHit) return 0x400u    // VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR
    if (stage == ShaderStage.Miss) return 0x800u          // VK_SHADER_STAGE_MISS_BIT_KHR
    return 0x20u  // compute
}

// Reserve an empty push_constants_<shader>(cb : CommandBuffer; layout : PipelineLayout) function.
// Mirrors generate_bind_uniform_dummy.
[macro_function]
def private generate_push_constants_dummy(fnMain : FunctionPtr) {
    var fn = new Function(at = fnMain.at, atDecl = fnMain.at, name := push_constants_function_name(fnMain))
    fn.flags.generated = true
    fn.flags.exports = true
    fn.result = new TypeDecl(at = fnMain.at, baseType = Type.tVoid)
    // const cb/layout: block-captured `cmd` is naturally const, and neither handle is ever mutated.
    var cb_type <- new TypeDecl(at = fnMain.at, baseType = Type.alias, alias := "CommandBuffer")
    cb_type.flags.constant = true
    fn.arguments |> emplace_new() <| new Variable(at = fnMain.at, name := "cb", _type <- cb_type)
    var layout_type <- new TypeDecl(at = fnMain.at, baseType = Type.alias, alias := "PipelineLayout")
    layout_type.flags.constant = true
    fn.arguments |> emplace_new() <| new Variable(at = fnMain.at, name := "layout", _type <- layout_type)
    fn.body = new ExprBlock(at = fnMain.at)
    if (!(compiling_module() |> add_function(fn))) {
        panic("can't reserve push_constants dummy function for {fnMain.name}")
    }
}

// Fill the push_constants body: one vkCmdPushConstants(cb, layout, stage_flags, 0u, size, ptr) per
// @push_constant struct global reachable from fnMain. stage_flags is this shader's own stage — if
// several stages read it, each stage's push function runs (Vulkan accepts any declared subset).
[macro_function]
def private generate_push_constants(fnMain : FunctionPtr; var fn : FunctionPtr; stage : ShaderStage) {   // nolint:STYLE038 - flat push-constant member walk
    let stage_uint = stage_to_uint(stage)
    var body = new ExprBlock(at = fn.at)
    // One push-constant block per stage in Vulkan, and offset is hard-coded 0 here, so >1 overlaps: fail closed.
    var pc_globals : array<Variable?>
    collect_dependencies(fnMain) $(_vfun, vvar) {
        for (vv in vvar) {
            // Same shape filter as the @uniform walk; the marker-name + empty() checks are defence-in-depth here.
            if (!(vv.annotation |> find_arg("push_constant") is tBool) ||
                vv._type == null || vv._type.baseType != Type.tStructure || vv._type.structType == null ||
                is_opaque_resource_struct(vv._type.structType) ||
                empty(vv._type.structType.fields)) continue
            pc_globals |> push(vv)
        }
    }
    if (length(pc_globals) > 1) {
        macro_error(compiling_program(), pc_globals[0].at,
            "[vulkan_*_shader]: shader reaches {length(pc_globals)} @push_constant struct globals; the generated function always pushes at offset=0 and would emit overlapping vkCmdPushConstants writes. Vulkan also caps push-constant blocks at one per stage -- merge the structs.")
        unsafe { delete pc_globals; }
        return
    }
    for (vv in pc_globals) {
        let st = vv._type.structType
        var errs : array<string>
        var fields : array<BlockField>
        let layout_info = compute_block_layout(string(vv.name), st, fields, errs)
        if (!layout_info.ok) {
            // Fail-closed: a silently skipped vkCmdPushConstants leaves stale push state and a corrupt frame.
            let msg = empty(errs) ? "(no diagnostic)" : errs[0]
            macro_error(compiling_program(), vv.at,
                "[vulkan_*_shader]: @push_constant '{vv.name}' std140 layout failed: {msg}")
            delete fields
            delete errs
            unsafe { delete pc_globals; }
            return
        }
        delete fields
        delete errs

        // nolint:STYLE015
        // Stage via the shared std140 walk, not a raw addr(pc_var) upload — daslang's struct image
        // != std140. Shapes the raw path mis-packed, i.e. what to suspect when push data arrives
        // garbled: float3x3, float3x4, nested composites, and a float3 immediately before a scalar.
        var inner = new ExprBlock(at = fn.at)
        inner.blockFlags.isClosure = true
        inner.returnType = new TypeDecl(at = fn.at, baseType = Type.tVoid)
        var ptr_arg <- new Variable(at = fn.at, name := "ptr")
        ptr_arg._type = new TypeDecl(at = fn.at,
            baseType = Type.tPointer,
            firstType = new TypeDecl(at = fn.at, baseType = Type.tVoid)
        )
        ptr_arg._type.flags.constant = true
        inner.arguments |> emplace(ptr_arg)

        var writes : array<Std140Write>
        std140_block_writes(st, new ExprVar(at = vv.at, name := vv.name), 0, writes)
        for (w in writes) {
            var cll = new ExprCall(at = vv.at, name := "write_field")
            cll.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "ptr")
            cll.arguments |> emplace_new() <| new ExprConstUInt(at = vv.at, value = uint(w.offset))
            cll.arguments |> emplace(w.access)
            inner.list |> emplace(cll)
        }
        unsafe { delete writes; }

        let size_expr <- new ExprConstUInt(at = vv.at, value = uint(layout_info.size))
        let stage_expr <- new ExprConstUInt(at = vv.at, value = stage_uint)
        // Upload while the staging pointer is live; no unsafe wrap needed since ptr is already void?.
        var push_stmt = qmacro(vkCmdPushConstants(boost_value_to_vk(cb), boost_value_to_vk(layout),
            VkShaderStageFlags($e(stage_expr)), 0u, $e(size_expr), $i("ptr")))
        inner.list |> emplace(push_stmt)

        var mkblk = new ExprMakeBlock(at = fn.at, _block = inner)
        var wps = new ExprCall(at = fn.at, name := "with_push_staging")
        wps.arguments |> emplace_new() <| new ExprConstUInt(at = fn.at, value = uint(layout_info.size))
        wps.arguments |> emplace(mkblk)
        body.list |> emplace(wps)
    }
    unsafe { delete pc_globals; }
    fn.body = body
}
