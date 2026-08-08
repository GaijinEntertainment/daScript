options gen2
options indenting = 4

// Shader function-macro annotations (mirrors dasGlsl's GlslShader / [compute_program]). The emitted
// SPIR-V words are captured into a module global `{name} : array<uint>`. The global is DECLARED in
// `apply` (so consumer code referencing it resolves during the first inference pass — declaring it
// later, in patch/fixup, deadlocks: the consumer fails inference, which gates patchAnnotations off).
// Its init is FILLED in `fixup` — types are final there, and the pipeline gives fixup-set global
// inits a dirty re-infer (ast_parse.cpp), so the `to_array_move(...)` call the array<uint> literal
// lowers to resolves like ordinary code. (Historically the fill lived in `patch` with
// astChanged=true to force that re-infer by hand — at the price of a full-program infer restart
// per shader-bearing module.) The backend body-fill hooks stay in `patch`: they populate function
// BODIES, which genuinely need the in-loop re-infer.
// Builtins come in via spirv_builtins. Host: feed `{name}` straight to vkCreateShaderModule.

module spirv_shader shared public

require spirv_emit
require spirv_reflect
require spirv_builtins public
require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require strings

// `array<uint>(w0, w1, ...)` — the gen2 array literal is ExprMakeArray(makeType=uint, gen2=true)
// wrapped in to_array_move (ds2_parser.ypp).
[macro_function]
def private build_uint_array_literal(words : array<uint>; at : LineInfo) : ExpressionPtr {
    var mkArr = new ExprMakeArray(at = at, makeType = new TypeDecl(at = at, baseType = Type.tUInt))
    mkArr.gen2 = true
    for (w in words) {
        mkArr.values |> emplace_new() <| new ExprConstUInt(at = at, value = w)
    }
    var toMove = new ExprCall(at = at, name := "to_array_move")
    toMove.arguments |> emplace(mkArr)
    return toMove
}

// Per-shader auxiliary function names. The dasGlsl/dasOpenGL split puts the body fill in a backend
// subclass; we mirror that. {shader}_bind_uniform writes @uniform-tagged module globals into mapped
// UBO memory; {shader}_push_constants pushes @push_constant-tagged globals via the backend's push API.
// [macro_function]: both helpers are called from the backend subclass during patch/apply (macro
// context), mirroring glsl_internal::bind_uniform_function_name.
[macro_function]
def bind_uniform_function_name(fnMain : FunctionPtr) {
    return "{fnMain.name}_bind_uniform"
}
[macro_function]
def push_constants_function_name(fnMain : FunctionPtr) {
    return "{fnMain.name}_push_constants"
}

class SpirvShader : AstFunctionAnnotation {
    // overridden per stage; drives the entry-point execution model + execution mode in generate_spirv
    def shader_stage() : ShaderStage => ShaderStage.Compute
    // overridden by [spirv_kernel]: the kernel class whose annotated members are the resources.
    // null = the module-globals frontend (every stage annotation below).
    def kernel_self_of(func : FunctionPtr) : Structure const? {
        return null
    }
    // Backend-specific hooks. Base = no-op (keeps dasSpirv usable without a host backend). A backend
    // subclass (e.g. dasVulkan's SpirvVulkanShader) overrides all four: the _dummy form reserves an
    // empty function during apply() so consumer code references resolve in the first inference pass;
    // the non-dummy form fills the body during patch() after generate_spirv has succeeded.
    def generate_bind_uniform_dummy(var func : FunctionPtr) {
        pass
    }
    def generate_bind_uniform(fnMain : FunctionPtr; var fn : FunctionPtr) {
        pass
    }
    def generate_push_constants_dummy(var func : FunctionPtr) {
        pass
    }
    def generate_push_constants(fnMain : FunctionPtr; var fn : FunctionPtr) {
        pass
    }
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        func.flags.exports = true   // temporary: keeps the function alive through dependency collection
        let argName = args |> find_arg("name") ?as tString ?? "{func.name}`spirv"
        var atype = new TypeDecl(at = func.at, baseType = Type.tArray)
        atype.firstType = new TypeDecl(at = func.at, baseType = Type.tUInt)
        if (!add_global_var(compiling_module(), argName, atype, func.at, false)) {
            errors := "can't add global variable {argName}"
            return false
        }
        // Companion reflection global ({name}_reflect : array<uint>) — the host decodes it via
        // spirv_reflect::decode_reflection to auto-build descriptor/pipeline layouts.
        var rtype = new TypeDecl(at = func.at, baseType = Type.tArray)
        rtype.firstType = new TypeDecl(at = func.at, baseType = Type.tUInt)
        if (!add_global_var(compiling_module(), "{argName}_reflect", rtype, func.at, false)) {
            errors := "can't add global variable {argName}_reflect"
            return false
        }
        generate_bind_uniform_dummy(func)
        generate_push_constants_dummy(func)
        return true
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup; args, progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        // GPU compilers do their own inlining — the CPU auto-inliner (patchInline, which shares
        // this patch slot and runs once annotation patches go quiet) must not reshape the shader
        // tree the emitter reads at fixup. Callee-side neverInline pins every reachable function;
        // flag writes need no re-infer, so astChanged stays untouched. SAME-MODULE only: that is
        // the inliner's whole candidate universe, and the flag folds into the semantic hash — a
        // cross-module mark desyncs that module's separately-compiled AOT registration (50101).
        collect_dependencies(func) $(vfun, _vvar) {
            for (dfn in vfun) {
                if (dfn._module == compiling_module()) {
                    dfn.moreFlags.neverInline = true
                }
            }
        }
        // Backend body-fill: the dummy bind_uniform / push_constants functions were reserved in apply();
        // their bodies stay empty (still `exports = true`) until the backend's generate_* override
        // populates them here. astChanged on the first fill triggers re-infer so the bodies resolve.
        for_each_function(compiling_module(), bind_uniform_function_name(func)) $(bfun) {
            if (bfun.flags.exports) {
                bfun.flags.exports = false
                generate_bind_uniform(func, bfun)
                astChanged = true
            }
        }
        for_each_function(compiling_module(), push_constants_function_name(func)) $(bfun) {
            if (bfun.flags.exports) {
                bfun.flags.exports = false
                generate_push_constants(func, bfun)
                astChanged = true
            }
        }
        return true
    }
    def override fixup(var func : FunctionPtr; var group : ModuleGroup; args, progArgs : AnnotationArgumentList; var errors : das_string) : bool {
        let argName = args |> find_arg("name") ?as tString ?? "{func.name}`spirv"
        var err : array<string>
        // local_size_{x,y,z} take an int literal OR the name of a module-scope int constant -- the same
        // rule the layout indices follow. A compute shader whose workgroup size is a platform config
        // value names it (`local_size_x = SUBGROUP_SIZE`) rather than hardcoding the number.
        var local_size : int3
        local_size.x = annotation_int(args, "local_size_x", 1, err)
        local_size.y = annotation_int(args, "local_size_y", 1, err)
        local_size.z = annotation_int(args, "local_size_z", 1, err)
        // local_size_*_id = N makes that workgroup axis a specialization constant (SpecId = N,
        // default = the local_size_* value); -1 (absent) keeps the axis a plain literal.
        var local_size_ids : int3
        local_size_ids.x = args |> find_arg("local_size_x_id") ?as tInt ?? -1
        local_size_ids.y = args |> find_arg("local_size_y_id") ?as tInt ?? -1
        local_size_ids.z = args |> find_arg("local_size_z_id") ?as tInt ?? -1
        // Mesh-shader output configuration (max_vertices / max_primitives / output_topology). Only the
        // mesh stage carries these; other stages keep the defaults, which generate_spirv ignores. The
        // topology string is validated here — the only place the annotation args are visible — and a bad
        // value pushes a clean error so the blob is never installed; the counts are validated downstream
        // at the emit gate in generate_spirv.
        var mv = 64
        var mp = 64
        var topology = MeshOutputTopology.triangles
        if (shader_stage() == ShaderStage.Mesh) {
            mv = args |> find_arg("max_vertices") ?as tInt ?? 64
            mp = args |> find_arg("max_primitives") ?as tInt ?? 64
            let topo = args |> find_arg("output_topology") ?as tString ?? "triangles"
            if (topo == "triangles") {
                topology = MeshOutputTopology.triangles
            } elif (topo == "lines") {
                topology = MeshOutputTopology.lines
            } elif (topo == "points") {
                topology = MeshOutputTopology.points
            } else {
                err |> push("mesh_shader: output_topology must be \"triangles\", \"lines\", or \"points\" (got \"{topo}\")")
            }
        }
        let mesh_cfg = MeshOutputConfig(max_vertices = mv, max_primitives = mp, topology = topology)
        // early_fragment_tests -> the EarlyFragmentTests execution mode; fragment-only, so reject it on
        // any other stage where the mode would be illegal.
        let early_frag = args |> find_arg("early_fragment_tests") ?as tBool ?? false
        if (early_frag && shader_stage() != ShaderStage.Fragment) {
            err |> push("early_fragment_tests is only valid on a fragment shader")
        }
        var glob <- find_variable(compiling_module(), argName)
        if (glob != null && glob.init == null && glob.name == argName) {
            var refl : SpirvReflection
            var words <- generate_spirv(func, err, shader_stage(), local_size, local_size_ids, mesh_cfg, early_frag, refl, kernel_self_of(func))
            if (err |> empty()) {
                glob.init = build_uint_array_literal(words, func.at)
                // Fill the companion reflection global in the same fixup (gated on the blob's
                // null init, so both fill exactly once).
                var rwords <- encode_reflection(refl)
                var rglob <- find_variable(compiling_module(), "{argName}_reflect")
                if (rglob != null && rglob.init == null) {
                    rglob.init = build_uint_array_literal(rwords, func.at)
                }
                rglob := null
                delete rwords
            }
            finalize(refl)
            delete words
        }
        glob := null
        if (!(err |> empty())) {
            errors := join(err, "\n")
            return false
        }
        return true
    }
}

[function_macro(name="compute_shader")]
class SpirvComputeShader : SpirvShader {
}

// [spirv_kernel] — the CLASS-KERNEL frontend: [compute_shader]'s sibling for a kernel authored as
// a class (resources = annotated members, kernels = methods — the dasMetal [metal_kernel] model).
// `self.member` accesses classify + resolve exactly like module globals (classify_kernel_members /
// member_var_of); the module-globals frontend is untouched. Same annotation args (name,
// local_size_*, local_size_*_id).
[function_macro(name="spirv_kernel")]
class SpirvKernelShader : SpirvShader {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        if (func.classParent == null) {
            errors := "[spirv_kernel] must annotate a class method - resources are the class's annotated members"
            return false
        }
        return SpirvShader`apply(self, func, group, args, errors)
    }
    def override kernel_self_of(func : FunctionPtr) : Structure const? {
        return func.classParent
    }
}

[function_macro(name="vertex_shader")]
class SpirvVertexShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.Vertex
}

[function_macro(name="fragment_shader")]
class SpirvFragmentShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.Fragment
}

// [mesh_shader] / [task_shader] (VK_EXT_mesh_shader). Execution model + capabilities + execution modes
// + SetMeshOutputsEXT / EmitMeshTasksEXT prologue intrinsics. The mesh output is configured via the
// annotation args max_vertices / max_primitives (default 64 each -> OutputVertices / OutputPrimitivesEXT)
// and output_topology ("triangles" | "lines" | "points", default "triangles" -> OutputTrianglesEXT /
// OutputLinesEXT / OutputPoints). The per-vertex / per-primitive output array surface (gl_MeshVerticesEXT,
// gl_PrimitiveTriangleIndicesEXT, ...) and the array-of-builtin classifier land in a follow-up slice.
[function_macro(name="mesh_shader")]
class SpirvMeshShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.Mesh
}

[function_macro(name="task_shader")]
class SpirvTaskShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.Task
}

// [raygen_shader] / [miss_shader] / [closest_hit_shader] (VK_KHR_ray_tracing_pipeline). Execution
// model + RayTracingKHR capability + SPV_KHR_ray_tracing extension (SPIR-V 1.4); no execution modes.
// Payloads / hit attributes are @ray_payload / @incoming_ray_payload / @hit_attribute globals; the
// acceleration structure is an accelerationStructureEXT descriptor; rays are traced via traceRayEXT.
// AnyHit / Intersection / Callable stages land with their first consumer.
[function_macro(name="raygen_shader")]
class SpirvRayGenShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.RayGen
}

[function_macro(name="miss_shader")]
class SpirvMissShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.Miss
}

[function_macro(name="closest_hit_shader")]
class SpirvClosestHitShader : SpirvShader {
    def override shader_stage() : ShaderStage => ShaderStage.ClosestHit
}

// [spirv_decode] — marks a daslang function as a cm2 DECODE function (SPV_NV_cooperative_matrix2):
// referenced via @@name from coopmatLoadTensorDecode and emitted as a real SPIR-V function with the
// rigid (PhysicalStorageBuffer block pointer, uint[2], uint[2]) signature. Marker only — signature
// validation happens at emission (register_decode_func); the body stays CPU-callable, so reference
// math can call the exact decode the GPU runs.
[function_macro(name="spirv_decode")]
class SpirvDecodeAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        // pin against the CPU auto-inliner: the emitter must see the body as authored
        func.moreFlags.neverInline = true
        return true
    }
}
