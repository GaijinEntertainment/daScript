options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_vulkan_dispatch shared private

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost   // make_expression_block / add_global_var / force_at
require daslib/rtti
require strings
require daslib/strings_boost   // split / strip — the grid & params micro-format tokenizers
require dasllama_kernel_access   // the shared body-walk classifier (field mode) — access is DERIVED

// The dasLLAMA Vulkan kernel-dispatch lens ([metal_dispatch]'s sibling). [vk_dispatch] reads a
// [spirv_kernel] class's member annotations and generates the class's dispatch surface over the
// vkd_ rail (dasllama_vulkan_common):
//   ensure_<name>()                      — lazy per-class layouts + pipeline, masks registered
//   set_<name>(bufs, sizes, gbits)      — a descriptor set over the CLASS layout + region bits
//   enc_<name>(raw, h, set_, pc, ...)   — push constants + hazard dispatch
// The read/write axis is DERIVED from the kernel body (shared classifier, field mode); declared
// @role is a cross-checked assertion, weight/alias stay declared. The hazard rail is untouched:
// the class pipeline registers binding masks, sets carry per-binding region bits.

struct private VkBufField {
    name : string
    binding : int
    role : string
}

def private mk_alias_t(at : LineInfo; nm : string; is_const : bool) : TypeDeclPtr {
    var t = new TypeDecl(at = at, baseType = Type.alias, alias := nm)
    t.flags.constant = is_const
    return t
}

def private mk_gvar(at : LineInfo; nm : string) : ExpressionPtr {
    return new ExprVar(at = at, name := nm)
}

// a [spirv_kernel] method of a lensed class + its captured blob global name. `want` names the
// method for multi-kernel classes; "" counts every kernel method and returns the first (the
// single-kernel form's exactly-one check rides on nmatch)
def private find_kernel_fn(st : StructurePtr; mod : Module?; want : string; var nmatch : int&; var blob : string&) : Function? {
    var kfn : Function?
    let wanted = empty(want) ? "" : "{st.name}`{want}"
    mod |> for_each_function("") $(fn) {
        if (!empty(fn.arguments) && fn.arguments[0]._type.structType == st) {
            for (ann in fn.annotations) {
                if (ann.annotation.name == "spirv_kernel") {
                    if (empty(wanted)) {
                        nmatch++
                        if (kfn == null) {
                            kfn = fn
                            blob = find_arg(ann.arguments, "name") ?as tString ?? "{fn.name}`spirv"
                        }
                    } elif (fn.name == wanted) {
                        nmatch++
                        kfn = fn
                        blob = find_arg(ann.arguments, "name") ?as tString ?? "{fn.name}`spirv"
                    }
                }
            }
        }
    }
    return kfn
}

def private mk_ret_bool(at : LineInfo; v : bool) : ExpressionPtr {
    return new ExprReturn(at = at, subexpr = new ExprConstBool(at = at, value = v))
}

def private mk_if_ret(at : LineInfo; var cond : Expression?; ret_val : bool) : ExpressionPtr {
    var blk <- [mk_ret_bool(at, ret_val)]
    return new ExprIfThenElse(at = at, cond = cond, if_true = make_expression_block(blk))
}

def private mk_if_do(at : LineInfo; var cond : Expression?; var stmt : Expression?) : ExpressionPtr {
    var blk <- [unsafe(reinterpret<ExpressionPtr>(stmt))]
    return new ExprIfThenElse(at = at, cond = cond, if_true = make_expression_block(blk))
}

def private mk_not(at : LineInfo; var e : Expression?) : ExpressionPtr {
    return new ExprOp1(at = at, op := "!", subexpr = e)
}

[structure_macro(name = vk_dispatch)]
class VkDispatch : AstStructureAnnotation {
    // multi-kernel ordinal state: st.annotations is EMPTY at apply time (probe-verified
    // 2026-08-04), but the macro INSTANCE persists across applies — arrival order assigns each
    // instance its pipe slot; seen_ord makes a re-fired instance idempotent (keyed by name=)
    fam_count : table<string; int>   // "{module}`{family}" -> next ordinal
    seen_ord : table<string; int>    // "{module}`{name}" -> assigned ordinal

    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {   // nolint:STYLE037,STYLE038 — flat per-argument validation + emit table
        let at = st.at
        let base = find_arg(args, "name") ?as tString ?? ""
        let grid = find_arg(args, "grid") ?as tString ?? ""
        if (empty(base)) {
            errors := "[vk_dispatch]: 'name' (the generated surface's base name) is required"
            return false
        }
        // multi-kernel form: N [vk_dispatch] instances on one class, each passing kernel= (its
        // [spirv_kernel] method) + the shared family= (the ensure_/set_ surface); instance N's
        // pipe slot = its arrival order within the family
        let kern = find_arg(args, "kernel") ?as tString ?? ""
        let famArg = find_arg(args, "family") ?as tString ?? ""
        let family = empty(famArg) ? base : famArg
        let multi = !empty(kern)
        if (!multi && !empty(famArg)) {
            errors := "[vk_dispatch] {base}: family= without kernel= - a family instance must name its [spirv_kernel] method"
            return false
        }
        // census/override display name: the vk_dispatch name= ("<base>_spv"), not the blob
        // global's mangled default — templated instances drop [spirv_kernel] name=, and a
        // backticked "{Class}`run`spirv" makes the SPV-override filename unusable
        let disp_name = "{base}_spv"
        let mname = "{compiling_module().name}"
        let bkey = "{mname}`{base}"
        let fkey = "{mname}`{family}"
        var ord = 0
        if (key_exists(seen_ord, bkey)) {
            ord = seen_ord[bkey]
        } else {
            if (key_exists(fam_count, fkey)) {
                ord = fam_count[fkey]
            }
            seen_ord[bkey] = ord
            fam_count[fkey] = ord + 1
        }
        if (empty(grid)) {
            errors := "[vk_dispatch] {base}: 'grid' is required (1D: int | param | param/coverage)"
            return false
        }
        if (find(grid, ",") >= 0) {
            errors := "[vk_dispatch] {base}: grid '{grid}' - only a 1D grid is supported (vkCmdDispatch x,1,1)"
            return false
        }
        let dparts <- split(grid, "/")
        if (length(dparts) > 2) {
            errors := "[vk_dispatch] {base}: grid '{strip(grid)}' has more than one '/' (int | param | param/coverage)"
            return false
        }

        // Collect @ssbo members (binding order; same-binding VIEWS legal — access unions into the
        // slot) + the single @push_constant params struct. @workgroup/@private_global/methods skip.
        var bufs : array<VkBufField>
        var push_t : TypeDeclPtr
        var push_fields = 0
        var wg_bytes = 0   // @workgroup footprint — ensure_ gates it against the device cap
        for (field in st.fields) {
            var is_ssbo = false
            var is_push = false
            var is_wg = false
            for (annDecl in field.annotation) {
                if (annDecl.name == "ssbo") {
                    is_ssbo = true
                } elif (annDecl.name == "push_constant") {
                    is_push = true
                } elif (annDecl.name == "workgroup") {
                    is_wg = true
                } elif (annDecl.name == "uniform") {
                    errors := "[vk_dispatch] {base}: member '{field.name}' is @uniform - UBO members are not lensed yet; params ride the @push_constant struct"
                    return false
                }
            }
            if (is_wg) {
                wg_bytes += field._type.sizeOf
                continue
            }
            if (is_push) {
                if (push_t != null) {
                    errors := "[vk_dispatch] {base}: more than one @push_constant member"
                    return false
                }
                if (field._type == null || field._type.baseType != Type.tStructure || field._type.structType == null) {
                    errors := "[vk_dispatch] {base}: @push_constant member '{field.name}' must be a struct"
                    return false
                }
                for (pf in field._type.structType.fields) {
                    let bt = pf._type != null ? pf._type.baseType : Type.none
                    if (bt != Type.tInt && bt != Type.tUInt && bt != Type.tFloat) {
                        errors := "[vk_dispatch] {base}: @push_constant field '{pf.name}' must be a 4-byte scalar (int/uint/float)"
                        return false
                    }
                    push_fields++
                }
                push_t = clone_type(unsafe(reinterpret<TypeDeclPtr>(field._type)))
                continue
            }
            if (!is_ssbo) {
                continue
            }
            let binding = find_arg(field.annotation, "binding") ?as tInt ?? -1
            if (binding < 0) {
                errors := "[vk_dispatch] {base}: @ssbo member '{field.name}' has no @binding"
                return false
            }
            let role = find_arg(field.annotation, "role") ?as tString ?? ""
            if (!role_ok(role)) {
                errors := "[vk_dispatch] {base}: member '{field.name}' has unknown @role '{role}' (read/write/readwrite/weight/alias)"
                return false
            }
            bufs |> push(VkBufField(name = "{field.name}", binding = binding, role = role))
        }
        if (empty(bufs)) {
            errors := "[vk_dispatch] {base}: no @ssbo members to bind"
            return false
        }
        // the class's distinct bindings, ascending (gaps legal — the layout lists exactly these;
        // same-binding views collapse to one slot)
        var covered : table<int>
        var blist : array<int>
        for (bf in bufs) {
            if (!key_exists(covered, bf.binding)) {
                covered |> insert(bf.binding)
                blist |> push(bf.binding)
            }
        }
        sort(blist)
        if (blist[length(blist) - 1] > 7) {
            errors := "[vk_dispatch] {base}: binding {blist[length(blist) - 1]} - the hazard rail's SetBits caps at binding 7"
            return false
        }
        let nb = length(blist)   // the set builder's arity: one buffer/size/bits entry per declared binding

        // the kernel method + derived access -> per-binding read/write masks
        var nmatch = 0
        var blob = ""
        let kfn = find_kernel_fn(st, compiling_module(), kern, nmatch, blob)
        if (empty(kern)) {
            if (kfn == null || nmatch != 1) {
                errors := "[vk_dispatch] {base}: expected exactly one [spirv_kernel] method on {st.name} (found {nmatch}) - multi-kernel classes pick per instance via kernel="
                return false
            }
        } elif (kfn == null) {
            errors := "[vk_dispatch] {base}: no [spirv_kernel] method '{kern}' on {st.name}"
            return false
        }
        var tracked : table<string>
        for (bf in bufs) {
            tracked |> insert(bf.name)
        }
        var res <- classify_field_access(kfn, compiling_module(), st, tracked)
        if (!empty(res.err)) {
            errors := "[vk_dispatch] {base}: {res.err}"
            delete res
            delete tracked
            return false
        }
        var rmask = 0u
        var wmask = 0u
        for (bf in bufs) {
            let dr = derived_role(res, bf.name)
            let decl = bf.role
            if (decl == "weight") {
                if (key_exists(res.writes, bf.name)) {
                    errors := "[vk_dispatch] {base}: member '{bf.name}' declares @role='weight' but the kernel body writes it"
                    delete res
                    delete tracked
                    return false
                }
            } elif (multi) {
                // per-kernel view of shared members: unused-here is fine (a sibling kernel uses
                // it), and the declared role must COVER this kernel's derived access
                let cov = (empty(dr) || decl == "" || decl == "alias" || decl == dr
                    || (decl == "readwrite" && (dr == "read" || dr == "write")))
                if (!cov) {
                    errors := "[vk_dispatch] {base}: member '{bf.name}' declares @role='{decl}' but kernel '{kern}' derives '{dr}' - the declared role must cover every kernel"
                    delete res
                    delete tracked
                    return false
                }
            } elif (decl != "" && decl != "alias" && decl != dr) {
                let drName = empty(dr) ? "unused" : dr
                errors := "[vk_dispatch] {base}: member '{bf.name}' declares @role='{decl}' but the body derives '{drName}' - fix whichever is wrong"
                delete res
                delete tracked
                return false
            } elif (decl == "" && empty(dr)) {
                errors := "[vk_dispatch] {base}: @ssbo member '{bf.name}' is never accessed by the kernel body - declare @role (weight/alias) or drop the binding"
                delete res
                delete tracked
                return false
            }
            let bit = 1u << uint(bf.binding)
            if (key_exists(res.reads, bf.name)) {
                rmask |= bit
            }
            if (key_exists(res.writes, bf.name)) {
                wmask |= bit
            }
        }
        delete res
        delete tracked

        // ===== the generated surface =====
        // one VkdClass global + one set builder per FAMILY (ordinal 0 generates them); every
        // instance generates its own ensure_<base> (lazy pipe slot) + enc_<base>
        let gname = "{family}`vkd"
        if (ord == 0) {
            if (!add_global_var(compiling_module(), gname, mk_alias_t(at, "VkdClass", false), at, false)) {
                errors := "[vk_dispatch] {base}: can't add class-state global {gname}"
                return false
            }
        }

        // ensure_<base>() : bool — device up, family layouts once, THIS kernel's pipe slot once
        {
            // `array<int>(b0, b1, ...)` — the gen2 array literal (ExprMakeArray wrapped in
            // to_array_move, the spirv_shader capture shape)
            var mkArr = new ExprMakeArray(at = at, makeType = new TypeDecl(at = at, baseType = Type.tInt))
            mkArr.gen2 = true
            for (bind in blist) {
                mkArr.values |> emplace_new <| new ExprConstInt(at = at, value = bind)
            }
            var bl = new ExprCall(at = at, name := "to_array_move")
            bl.arguments |> emplace(mkArr)
            var ci = new ExprCall(at = at, name := "vkd_class_init")
            ci.arguments |> emplace_new <| mk_gvar(at, gname)
            ci.arguments |> emplace_new <| bl
            ci.arguments |> emplace_new <| new ExprConstInt(at = at, value = push_fields * 4)
            var cp = new ExprCall(at = at, name := "vkd_class_pipe")
            cp.arguments |> emplace_new <| mk_gvar(at, gname)
            cp.arguments |> emplace_new <| new ExprConstInt(at = at, value = ord)
            cp.arguments |> emplace_new <| mk_gvar(at, blob)
            cp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = rmask)
            cp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = wmask)
            cp.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
            var pm = new ExprCall(at = at, name := "vkd_pipe_made")
            pm.arguments |> emplace_new <| mk_gvar(at, gname)
            pm.arguments |> emplace_new <| new ExprConstInt(at = at, value = ord)
            var body : array<ExpressionPtr>
            body |> push(mk_if_ret(at, mk_not(at, new ExprCall(at = at, name := "vk_moe_init")), false))
            if (wg_bytes > 0) {
                // over-cap classes decline by name BEFORE the pipeline build (MoltenVK's 32 KB
                // threadgroup cap fails the COMPILE otherwise — an opaque INITIALIZATION_FAILED)
                var wf = new ExprCall(at = at, name := "vkd_wg_fits")
                wf.arguments |> emplace_new <| new ExprConstInt(at = at, value = wg_bytes)
                wf.arguments |> emplace_new <| new ExprConstString(at = at, value := base)
                body |> push(mk_if_ret(at, mk_not(at, wf), false))
            }
            body |> push(mk_if_do(at, mk_not(at, new ExprField(at = at, value = mk_gvar(at, gname), name := "ready")), ci))
            body |> push(mk_if_do(at, mk_not(at, pm), cp))
            body |> push(mk_ret_bool(at, true))
            var fn = new Function(at = at, atDecl = at, name := "ensure_{base}",
                flags = FunctionFlags.generated,
                result = new TypeDecl(at = at, baseType = Type.tBool))
            fn.body = make_expression_block(body)
            fn.body |> force_at(at)
            if (!(compiling_module() |> add_function(fn))) {
                errors := "[vk_dispatch] {base}: failed to add ensure_{base}"
                return false
            }
        }

        // [init] census seed — the kernel joins the coverage universe at program start, device
        // or not, so a never-dispatched class kernel warns like the module-global kernels do
        {
            var cs = new ExprCall(at = at, name := "vkd_census_seed")
            cs.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
            cs.arguments |> emplace_new <| new ExprConstUInt(at = at, value = rmask)
            cs.arguments |> emplace_new <| new ExprConstUInt(at = at, value = wmask)
            var body <- array<ExpressionPtr>(cs)
            var fn = new Function(at = at, atDecl = at, name := "{base}`census",
                flags = FunctionFlags.generated,
                result = new TypeDecl(at = at, baseType = Type.tVoid))
            fn.flags.init = true
            fn.flags.privateFunction = true
            fn.body = make_expression_block(body)
            fn.body |> force_at(at)
            if (!(compiling_module() |> add_function(fn))) {
                errors := "[vk_dispatch] {base}: failed to add the census [init]"
                return false
            }
        }

        // set_<family>(bufs, sizes, gbits) : VkDescriptorSet — a set over the FAMILY layout
        if (ord == 0) {
            var body : array<ExpressionPtr>
            var sv = new Variable(at = at, name := "set_", _type = new TypeDecl(at = at, baseType = Type.autoinfer))
            sv.init = mk_call1(at, "vkd_alloc_set", gname)
            var letd = new ExprLet(at = at)
            letd.variables |> emplace(sv)
            body |> push(letd)
            var ws = new ExprCall(at = at, name := "vkd_set")
            ws.arguments |> emplace_new <| mk_gvar(at, gname)
            ws.arguments |> emplace_new <| mk_gvar(at, "set_")
            ws.arguments |> emplace_new <| mk_gvar(at, "bufs")
            ws.arguments |> emplace_new <| mk_gvar(at, "sizes")
            ws.arguments |> emplace_new <| mk_gvar(at, "gbits")
            body |> push(ws)
            body |> push(new ExprReturn(at = at, subexpr = mk_gvar(at, "set_")))
            var fn = new Function(at = at, atDecl = at, name := "set_{family}",
                flags = FunctionFlags.generated,
                result = mk_alias_t(at, "VkDescriptorSet", false))
            var bufs_t = make_fixed_array_type(nb, new TypeDecl(at = at, baseType = Type.tUInt64))
            bufs_t.flags.constant = true
            var sizes_t = make_fixed_array_type(nb, new TypeDecl(at = at, baseType = Type.tInt64))
            sizes_t.flags.constant = true
            var gbits_t = make_fixed_array_type(nb, new TypeDecl(at = at, baseType = Type.tUInt))
            gbits_t.flags.constant = true
            fn.arguments |> emplace_new <| new Variable(at = at, name := "bufs", _type = bufs_t)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "sizes", _type = sizes_t)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "gbits", _type = gbits_t)
            fn.body = make_expression_block(body)
            fn.body |> force_at(at)
            if (!(compiling_module() |> add_function(fn))) {
                errors := "[vk_dispatch] {base}: failed to add set_{family}"
                return false
            }
        }

        // enc_<base>(raw, h, set_, pc, params...) — push + hazard dispatch
        {
            var body : array<ExpressionPtr>
            if (push_t != null) {
                var pp = new ExprCall(at = at, name := "vkd_push")
                pp.arguments |> emplace_new <| mk_gvar(at, "raw")
                pp.arguments |> emplace_new <| mk_gvar(at, gname)
                pp.arguments |> emplace_new <| mk_gvar(at, "pc")
                body |> push(pp)
            }
            var dp = new ExprCall(at = at, name := "vkd_dispatch")
            dp.arguments |> emplace_new <| mk_gvar(at, "raw")
            dp.arguments |> emplace_new <| mk_gvar(at, "h")
            dp.arguments |> emplace_new <| mk_gvar(at, gname)
            dp.arguments |> emplace_new <| mk_gvar(at, "set_")
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = ord)
            dp.arguments |> emplace_new <| mk_grid_dim(at, grid)
            body |> push(dp)
            var fn = new Function(at = at, atDecl = at, name := "enc_{base}",
                flags = FunctionFlags.generated,
                result = new TypeDecl(at = at, baseType = Type.tVoid))
            fn.arguments |> emplace_new <| new Variable(at = at, name := "raw", _type = mk_alias_t(at, "VkCommandBuffer", true))
            var ht = mk_alias_t(at, "VkHaz", false)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "h", _type = ht)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "set_", _type = mk_alias_t(at, "VkDescriptorSet", false))
            if (push_t != null) {
                push_t.flags.ref = true
                fn.arguments |> emplace_new <| new Variable(at = at, name := "pc", _type = push_t)
            }
            let params = find_arg(args, "params") ?as tString ?? ""
            if (!empty(strip(params))) {
                for (p in split(params, ",")) {
                    let nt <- split(p, ":")
                    if (length(nt) != 2) {
                        errors := "[vk_dispatch] {base}: param '{strip(p)}' must be 'name : type'"
                        return false
                    }
                    var ok = true
                    var pt = param_type(at, strip(nt[1]), ok)
                    if (!ok) {
                        errors := "[vk_dispatch] {base}: param '{strip(p)}' has unsupported type '{strip(nt[1])}' (int/uint/int64/uint64)"
                        return false
                    }
                    pt.flags.constant = true
                    fn.arguments |> emplace_new <| new Variable(at = at, name := "{strip(nt[0])}", _type = pt)
                }
            }
            // the ceil-div grid builds with int64 literals and daslang has no promotion — any
            // name a divide-form grid references must be an int64 param (bare single-name grids
            // go through uint() and accept all four types)
            if (find(grid, "/") >= 0) {
                for (gtok in split(grid, "/")) {
                    let gs = strip(gtok)
                    if (is_digit_tok(gs)) {
                        continue
                    }
                    var good = false
                    for (p in split(params, ",")) {
                        let nt <- split(p, ":")
                        if (length(nt) == 2 && strip(nt[0]) == gs && strip(nt[1]) == "int64") {
                            good = true
                        }
                    }
                    if (!good) {
                        errors := "[vk_dispatch] {base}: grid '{grid}' references '{gs}', which must be declared an int64 param (the generated ceil-div math is int64)"
                        return false
                    }
                }
            }
            fn.body = make_expression_block(body)
            fn.body |> force_at(at)
            if (!(compiling_module() |> add_function(fn))) {
                errors := "[vk_dispatch] {base}: failed to add enc_{base}"
                return false
            }
        }
        return true
    }
}
