options gen2
options indenting = 4
options stack = 1_048_576   // the emitter runs on this module's macro context: a kernel body's nesting recurses through emit_stmt, a frame of string locals a level

module cuda_kernel shared public

require cuda_emit public
require cuda_builtins public
require daslib/ast
require daslib/ast_boost
require daslib/shader_emit_common
require daslib/templates_boost
require daslib/rtti
require daslib/strings_boost

//! [cuda_kernel] - lowers a class method to CUDA C++ text (the [metal_kernel] shape: `apply`
//! DECLARES the companion globals and keeps the method alive, `fixup` FILLS them with constant
//! nodes). Companion globals, for annotation arg name=N (default: the mangled method name with
//! backticks flattened, `Class_m_cuda`). The entry point is `Class_m` (entry=E overrides), so
//! kernels of one family keep distinct names when their texts share one NVRTC program:
//!   N          : string  - the CUDA C++ source
//!   N_entry    : string  - the kernel entry-point name (feed to cuda_get_function)
//!   N_census   : string  - newline-joined census kinds (the emitter's coverage proxy)
//!   N_smem     : uint64  - the end of the dynamic shared carve, in bytes: the launch's dynamic shared size
//!   N_block    : uint3   - the block the annotation names (local_size_x/y/z, default 1) - the launch's block,
//!                          and the kernel's __launch_bounds__ (`min_blocks = N` adds its second argument: the
//!                          blocks an SM the register allocation must leave room for)
[function_macro(name="cuda_kernel")]
class CudaKernel : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        func.flags.exports = true
        let argName = kernel_global_name(func, args, "_cuda")
        return (add_typed_global(argName, Type.tString, func.at, errors) &&
            add_typed_global("{argName}_entry", Type.tString, func.at, errors) &&
            add_typed_global("{argName}_census", Type.tString, func.at, errors) &&
            add_typed_global("{argName}_smem", Type.tUInt64, func.at, errors) &&
            add_typed_global("{argName}_block", Type.tUInt3, func.at, errors))
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        pin_reachable_never_inline(func)
        return true
    }
    def override fixup(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; progArgs : AnnotationArgumentList; var errors : das_string) : bool {
        let argName = kernel_global_name(func, args, "_cuda")
        let glob = find_variable(compiling_module(), argName)
        if (glob == null || glob.init != null || glob.name != argName) {
            return true
        }
        var err : array<string>
        var census : table<string>
        let cfg = CudaEmitConfig(kernel_name = args |> find_arg("entry") ?as tString ?? kernel_derived_name(func),
            block_size = uint3(uint(args |> find_arg("local_size_x") ?as tInt ?? 1),
                uint(args |> find_arg("local_size_y") ?as tInt ?? 1),
                uint(args |> find_arg("local_size_z") ?as tInt ?? 1)),
            min_blocks = args |> find_arg("min_blocks") ?as tInt ?? 0,
            precise = args |> find_arg("precise") ?as tBool ?? false)
        var smem = 0ul
        let text = generate_cuda(func, err, cfg, census, smem)
        if (!empty(err)) {
            errors := join(err, "\n")
            return false
        }
        set_const_string_init(argName, text, func.at)
        set_const_string_init("{argName}_entry", cuda_kernel_entry_name(func, cfg), func.at)
        var sortedCensus <- [for (kind in keys(census)); kind]
        sortedCensus |> sort()
        set_const_string_init("{argName}_census", join(sortedCensus, "\n"), func.at)
        var smemGlob = find_variable(compiling_module(), "{argName}_smem")
        if (smemGlob != null && smemGlob.init == null) {
            smemGlob.init = new ExprConstUInt64(at = func.at, value = smem,
                _type = new TypeDecl(at = func.at, baseType = Type.tUInt64))
            smemGlob._type.flags.constant = true
        }
        var blockGlob = find_variable(compiling_module(), "{argName}_block")
        if (blockGlob != null && blockGlob.init == null) {
            blockGlob.init = new ExprConstUInt3(at = func.at, value = cfg.block_size,
                _type = new TypeDecl(at = func.at, baseType = Type.tUInt3))
            blockGlob._type.flags.constant = true
        }
        return true
    }
}
