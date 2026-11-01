options gen2
options indenting = 4

// [metal_kernel] — lowers a class method to MSL text (mirrors dasGlsl's shader annotations; the
// module-global surface mirrors dasSpirv's). `apply` DECLARES the companion globals (consumer code
// referencing them must resolve during the first inference pass) and keeps the method alive with
// exports=true (an uncalled method is otherwise culled before fixup ever runs — probe-verified).
// `fixup` FILLS them: every init is a constant node (ExprConstString/ExprConstBool), so the
// patch/astChanged dance dasSpirv needs for its array<uint> call-init does not apply here.
//
// Companion globals, for annotation arg name=N. Default (no name=): the mangled method name with
// backticks flattened — `Class`m` gets N = Class_m_msl, spellable from source, unique per class;
// a [template_struct_instance]-stamped method therefore names its globals per instance for free.
// The derived form also sets the MSL entry point to Class_m (explicit name= keeps the bare method
// name), so f16/f32 stamp twins stay distinguishable in GPU captures.
//   N          : string  — the MSL source
//   N_entry    : string  — the kernel entry-point name (feed to metal_new_function)
//   N_fastmath : bool    — the [metal_kernel(fastmath=...)] property, default true; feed to
//                          metal_new_library_from_source
//   N_tgmem    : uint64  — total dynamic [[threadgroup(0)]] bytes the kernel's @workgroup
//                          members occupy (0 = none); the dispatch MUST pass it to
//                          setThreadgroupMemoryLength (run_compute_1d takes it directly)
//   N_census   : string  — newline-joined census kinds (gate B; text has no disassembler, so the
//                          census is recorded at emit time and published for the meta-test)

module msl_shader shared public

require msl_emit public
require metal_builtins public
require daslib/ast
require daslib/ast_boost
require daslib/shader_emit_common
require daslib/templates_boost
require daslib/rtti
require daslib/strings_boost

[function_macro(name="metal_kernel")]
class MetalKernel : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        func.flags.exports = true   // keeps the (typically uncalled) method alive through dependency collection
        let argName = kernel_global_name(func, args, "_msl")
        return (add_typed_global(argName, Type.tString, func.at, errors) &&
            add_typed_global("{argName}_entry", Type.tString, func.at, errors) &&
            add_typed_global("{argName}_fastmath", Type.tBool, func.at, errors) &&
            add_typed_global("{argName}_census", Type.tString, func.at, errors) &&
            add_typed_global("{argName}_tgmem", Type.tUInt64, func.at, errors))
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        pin_reachable_never_inline(func)
        return true
    }
    def override fixup(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; progArgs : AnnotationArgumentList; var errors : das_string) : bool {
        let argName = kernel_global_name(func, args, "_msl")
        let fastmath = args |> find_arg("fastmath") ?as tBool ?? true
        let glob = find_variable(compiling_module(), argName)
        if (glob == null || glob.init != null || glob.name != argName) {
            return true
        }
        var err : array<string>
        var census : table<string>
        var cfg = MslEmitConfig(float_a_ok = args |> find_arg("float_a_ok") ?as tBool ?? false,
            max_threads = args |> find_arg("max_threads") ?as tInt ?? 0)
        if (empty(args |> find_arg("name") ?as tString ?? "")) {
            cfg.kernel_name = kernel_derived_name(func)
        }
        var tgmem = 0ul
        let text = generate_msl(func, err, cfg, census, tgmem)
        if (!empty(err)) {
            errors := join(err, "\n")
            return false
        }
        set_const_string_init(argName, text, func.at)
        set_const_string_init("{argName}_entry", kernel_entry_name(func, cfg), func.at)
        var sortedCensus <- [for (kind in keys(census)); kind]
        sortedCensus |> sort()
        set_const_string_init("{argName}_census", join(sortedCensus, "\n"), func.at)
        var fmGlob = find_variable(compiling_module(), "{argName}_fastmath")
        if (fmGlob != null && fmGlob.init == null) {
            fmGlob.init = new ExprConstBool(at = func.at, value = fastmath,
                _type = new TypeDecl(at = func.at, baseType = Type.tBool))
            fmGlob._type.flags.constant = true
        }
        var tgGlob = find_variable(compiling_module(), "{argName}_tgmem")
        if (tgGlob != null && tgGlob.init == null) {
            tgGlob.init = new ExprConstUInt64(at = func.at, value = tgmem,
                _type = new TypeDecl(at = func.at, baseType = Type.tUInt64))
            tgGlob._type.flags.constant = true
        }
        return true
    }
}
