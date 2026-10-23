options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_dispatch shared private

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/rtti
require daslib/shader_emit_common
require strings
require daslib/strings_boost
require dasllama_kernel_access
require dasllama_dispatch_scan

// The dasLLAMA CUDA kernel-dispatch lens ([vk_dispatch]'s sibling). [cuda_dispatch] reads a
// [cuda_kernel] class's member annotations and generates the class's dispatch surface over the
// cud_ rail (dasllama_cuda_common):
//   ensure_<name>()                        - device up, the kernel's function loaded once
//   enc_<name>(stream, bufs, pc, ...)      - the launch over the buffer addresses in binding order
// The read/write axis is DERIVED from the kernel body (shared classifier, field mode); declared
// @role is a cross-checked assertion, weight/alias stay declared. A CUDA kernel has no sets and
// no hazard rail: the buffer tuple IS the parameter blob, built a launch.

let private CUDA_WORDS = LensWords(lens = "[cuda_dispatch]", kernel = "cuda_kernel", grid_note = "",
    uniform_note = "params ride the args struct", args_many = "more than one args struct member", args_word = "args")

[structure_macro(name = cuda_dispatch)]
class CudaDispatch : AstStructureAnnotation {
    //! the instance persists across applies: arrival sets ordinals
    fam_count : table<string; int>   //! "{module}`{family}" -> next ordinal
    seen_ord : table<string; int>    //! "{module}`{name}" -> assigned ordinal

    def override apply(var st : StructurePtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {   // nolint:STYLE038 - flat emit table
        let at = st.at
        var scan <- lens_scan(st, args, CUDA_WORDS, fam_count, seen_ord, errors)
        return false if (!scan.ok)
        let disp_name = "{scan.base}_cuda"
        if (scan.args_t == null) {
            errors := "[cuda_dispatch] {scan.base}: no args struct member (@kargs / @push_constant) - the launch's parameter blob ends on one"
            return false
        }
        let kfn = lens_access(st, scan, CUDA_WORDS, errors).kfn
        return false if (kfn == null)

        let gname = "{scan.family}`cud"
        if (scan.ord == 0) {
            if (!add_global_var(compiling_module(), gname, mk_alias_t(at, "CudClass", false), at, false)) {
                errors := "[cuda_dispatch] {scan.base}: can't add class-state global {gname}"
                return false
            }
        }
        let text = kernel_global_name(kfn, "cuda_kernel", "_cuda")

        {
            var ci = new ExprCall(at = at, name := "cud_class_init")
            ci.arguments |> emplace_new <| mk_gvar(at, gname)
            var cf = new ExprCall(at = at, name := "cud_class_fn")
            cf.arguments |> emplace_new <| mk_gvar(at, gname)
            cf.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            cf.arguments |> emplace_new <| mk_gvar(at, text)
            cf.arguments |> emplace_new <| mk_gvar(at, "{text}_entry")
            cf.arguments |> emplace_new <| mk_gvar(at, "{text}_smem")
            cf.arguments |> emplace_new <| mk_gvar(at, "{text}_block")
            cf.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
            var made = new ExprCall(at = at, name := "cud_fn_made")
            made.arguments |> emplace_new <| mk_gvar(at, gname)
            made.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            var made2 = new ExprCall(at = at, name := "cud_fn_made")
            made2.arguments |> emplace_new <| mk_gvar(at, gname)
            made2.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            var body <- array<ExpressionPtr>(
                mk_if_ret(at, mk_not(at, new ExprCall(at = at, name := "cud_init")), false),
                mk_if_do(at, mk_not(at, new ExprField(at = at, value = mk_gvar(at, gname), name := "ready")), ci),
                mk_if_do(at, mk_not(at, made), cf),
                new ExprReturn(at = at, subexpr = made2))
            if (!add_generated(at, "ensure_{scan.base}", new TypeDecl(at = at, baseType = Type.tBool), body) $(var _fn : Function?) {}) {
                errors := "[cuda_dispatch] {scan.base}: failed to add ensure_{scan.base}"
                return false
            }
        }

        {
            var cs = new ExprCall(at = at, name := "cud_census_seed")
            cs.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
            cs.arguments |> emplace_new <| new ExprAddr(at = at, target := "ensure_{scan.base}")
            var body <- array<ExpressionPtr>(cs)
            if (!add_generated(at, "{scan.base}`census", new TypeDecl(at = at, baseType = Type.tVoid), body) $(var fn : Function?) {
                    fn.flags.init = true
                    fn.flags.privateFunction = true
                }) {
                errors := "[cuda_dispatch] {scan.base}: failed to add the census [init]"
                return false
            }
        }

        {
            var dp = new ExprCall(at = at, name := "cud_launch")
            dp.arguments |> emplace_new <| mk_gvar(at, gname)
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            dp.arguments |> emplace_new <| mk_gvar(at, "stream")
            dp.arguments |> emplace_new <| mk_gvar(at, "bufs")
            dp.arguments |> emplace_new <| mk_gvar(at, "pc")
            dp.arguments |> emplace_new <| mk_grid_dim(at, scan.grid)
            var body <- array<ExpressionPtr>(dp)
            var fn = new Function(at = at, atDecl = at, name := "enc_{scan.base}",
                flags = FunctionFlags.generated,
                result = new TypeDecl(at = at, baseType = Type.tVoid))
            var st_t = new TypeDecl(at = at, baseType = Type.tPointer)
            st_t.firstType = new TypeDecl(at = at, baseType = Type.tVoid)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "stream", _type = st_t)
            var bufs_t = make_fixed_array_type(length(scan.blist), new TypeDecl(at = at, baseType = Type.tUInt64))
            bufs_t.flags.constant = true
            fn.arguments |> emplace_new <| new Variable(at = at, name := "bufs", _type = bufs_t)
            scan.args_t.flags.ref = true
            fn.arguments |> emplace_new <| new Variable(at = at, name := "pc", _type = scan.args_t)
            return false if (!lens_params(fn, at, args, scan, CUDA_WORDS, errors))
            fn.body = make_expression_block(body)
            fn.body |> force_at(at)
            if (!(compiling_module() |> add_function(fn))) {
                errors := "[cuda_dispatch] {scan.base}: failed to add enc_{scan.base}"
                return false
            }
        }
        return true
    }
}
