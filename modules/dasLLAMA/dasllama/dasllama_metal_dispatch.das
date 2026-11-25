options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_metal_dispatch shared private

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

// The dasLLAMA Metal rail-dispatch lens ([vk_dispatch]'s and [cuda_dispatch]'s sibling). [metal_rail_dispatch] reads a
// [metal_kernel] class's member annotations and generates the class's dispatch surface over the Metal rail
// (dasllama_metal_rail):
//   ensure_<name>()                   - device up, the kernel's pipeline compiled once
//   enc_<name>(rl, roles, pc, ...)    - the dispatch over a tower rail (MtRail): the planes at `roles`, on its encoder
// The read/write axis is DERIVED from the kernel body (shared classifier, field mode); declared @role is a
// cross-checked assertion. The @kargs struct binds past the class's highest binding, by value.

let private METAL_WORDS = LensWords(lens = "[metal_rail_dispatch]", kernel = "metal_kernel", grid_note = "",
    uniform_note = "a rail class takes its scalars in the @kargs struct", args_many = "more than one args struct member",
    args_word = "args", kargs = "uniform", kargs_bind = true)

def private is_ssbo(field : FieldDeclaration) : bool => find_arg(field.annotation, "ssbo") is tBool

//! An alias member is a second view of a binding's buffer (the rows as float4, the quants as 16-byte words). The MSL emitter
//! takes one member a binding, so each alias that shares one moves to a slot of its own from `first` up, and the dispatch
//! binds the base member's plane there too: `packed` holds a 16-bit entry a moved view, its slot above the role's index
def private alias_slots(var st : StructurePtr; scan : LensScan; first : int) : tuple<packed : uint64; n : int> {
    var packed = 0ul
    var n = 0
    for (fi in range(length(st.fields))) {
        continue if (!is_ssbo(st.fields[fi]) || (find_arg(st.fields[fi].annotation, "role") ?as tString ?? "") != "alias")
        let b = find_arg(st.fields[fi].annotation, "binding") ?as tInt ?? -1
        var twin = false
        for (oi in range(length(st.fields))) {
            continue if (oi == fi || !is_ssbo(st.fields[oi]))
            twin ||= (find_arg(st.fields[oi].annotation, "binding") ?as tInt ?? -1) == b
        }
        let ri = find_index(scan.blist, b)
        continue if (!twin || ri < 0 || n == 4)
        let slot = first + n
        for (arg in st.fields[fi].annotation) {
            if (arg.name == "binding") {
                arg.iValue = slot
            }
        }
        packed |= uint64((slot << 8) | ri) << uint64(16 * n)
        n++
    }
    return (packed = packed, n = n)
}

[structure_macro(name = metal_rail_dispatch)]
class MetalRailDispatch : AstStructureAnnotation {
    //! the instance persists across applies: arrival sets ordinals
    fam_count : table<string; int>   //! "{module}`{family}" -> next ordinal
    seen_ord : table<string; int>    //! "{module}`{name}" -> assigned ordinal

    def override apply(var st : StructurePtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        let at = st.at
        var scan <- lens_scan(st, args, METAL_WORDS, fam_count, seen_ord, errors)
        return false if (!scan.ok)
        if (scan.args_t == null) {
            errors := "[metal_rail_dispatch] {scan.base}: no @kargs struct member - the dispatch binds one past the buffers"
            return false
        }
        let tg = find_arg(args, "tg") ?as tInt ?? 0
        if (tg <= 0) {
            errors := "[metal_rail_dispatch] {scan.base}: 'tg' (the threads a threadgroup) is required"
            return false
        }
        let acc = lens_access(st, scan, METAL_WORDS, errors)
        return false if (acc.kfn == null)
        var bmask = 0u
        for (b in scan.blist) {
            bmask |= 1u << uint(b)
        }
        let kslot = scan.blist[length(scan.blist) - 1] + 1
        let views = alias_slots(st, scan, kslot + 1)

        let gname = "{scan.family}`mtl"
        if (scan.ord == 0) {
            if (!add_global_var(compiling_module(), gname, mk_alias_t(at, "MtlClass", false), at, false)) {
                errors := "[metal_rail_dispatch] {scan.base}: can't add class-state global {gname}"
                return false
            }
        }
        let text = kernel_global_name(acc.kfn, "metal_kernel", "_msl")

        {
            var ci = new ExprCall(at = at, name := "mtl_class_init")
            ci.arguments |> emplace_new <| mk_gvar(at, gname)
            var cf = new ExprCall(at = at, name := "mtl_class_fn")
            cf.arguments |> emplace_new <| mk_gvar(at, gname)
            cf.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            cf.arguments |> emplace_new <| mk_gvar(at, text)
            cf.arguments |> emplace_new <| mk_gvar(at, "{text}_entry")
            cf.arguments |> emplace_new <| mk_gvar(at, "{text}_fastmath")
            cf.arguments |> emplace_new <| mk_gvar(at, "{text}_tgmem")
            cf.arguments |> emplace_new <| new ExprConstString(at = at, value := scan.base)
            var made = new ExprCall(at = at, name := "mtl_fn_made")
            made.arguments |> emplace_new <| mk_gvar(at, gname)
            made.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            var made2 = new ExprCall(at = at, name := "mtl_fn_made")
            made2.arguments |> emplace_new <| mk_gvar(at, gname)
            made2.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            var body <- array<ExpressionPtr>(
                mk_if_ret(at, mk_not(at, new ExprCall(at = at, name := "metal_common_init")), false),
                mk_if_do(at, mk_not(at, new ExprField(at = at, value = mk_gvar(at, gname), name := "ready")), ci),
                mk_if_do(at, mk_not(at, made), cf),
                new ExprReturn(at = at, subexpr = made2))
            if (!add_generated(at, "ensure_{scan.base}", new TypeDecl(at = at, baseType = Type.tBool), body) $(var _fn : Function?) {}) {
                errors := "[metal_rail_dispatch] {scan.base}: failed to add ensure_{scan.base}"
                return false
            }
        }

        {
            var dp = new ExprCall(at = at, name := "mtl_rail_dispatch")
            dp.arguments |> emplace_new <| mk_gvar(at, gname)
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            dp.arguments |> emplace_new <| mk_gvar(at, "rl")
            dp.arguments |> emplace_new <| mk_gvar(at, "roles")
            dp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = bmask)
            dp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = acc.rmask)
            dp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = acc.wmask)
            dp.arguments |> emplace_new <| mk_gvar(at, "pc")
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = kslot)
            dp.arguments |> emplace_new <| new ExprConstUInt64(at = at, value = views.packed)
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = views.n)
            dp.arguments |> emplace_new <| mk_grid_dim(at, scan.grid)
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = tg)
            var body <- array<ExpressionPtr>(dp)
            return false if (!lens_rail_overload(at, args, scan, METAL_WORDS, "MtRail", true, body, errors))
        }
        return true
    }
}
