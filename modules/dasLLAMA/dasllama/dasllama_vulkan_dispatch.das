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
require dasllama_dispatch_scan
require dasllama_gpu_math   // VKD_SET_BINDINGS: the hazard record's binding cap

// The dasLLAMA Vulkan kernel-dispatch lens ([metal_dispatch]'s sibling). [vk_dispatch] reads a
// [spirv_kernel] class's member annotations and generates the class's dispatch surface over the
// vkd_ rail (dasllama_vulkan_common):
//   ensure_<name>()                      — lazy per-class layouts + pipeline, masks registered
//   set_<name>(bufs, sizes, gbits)      — a descriptor set over the CLASS layout + region bits
//   enc_<name>(raw, h, set_, pc, ...)   — push constants + hazard dispatch
//   enc_<name>(rl, roles, pc, ...)      — the same over a tower rail (VtRail): the set over the rail's planes at `roles`
// The read/write axis is DERIVED from the kernel body (shared classifier, field mode); declared
// @role is a cross-checked assertion, weight/alias stay declared. The hazard rail is untouched:
// the class pipeline registers binding masks, sets carry per-binding region bits.

let private VK_WORDS = LensWords(lens = "[vk_dispatch]", kernel = "spirv_kernel", grid_note = " (vkCmdDispatch x,1,1)",
    uniform_note = "UBO members are not lensed yet; params ride the @push_constant struct",
    args_many = "more than one @push_constant member", args_word = "@push_constant")

//! the captured blob global of a [spirv_kernel] method: its `name =` argument, else `<method>`spirv`
def private spirv_blob_of(kfn : Function?) : string {
    for (ann in kfn.annotations) {
        if (ann.annotation.name == "spirv_kernel") {
            return find_arg(ann.arguments, "name") ?as tString ?? "{kfn.name}`spirv"
        }
    }
    return "{kfn.name}`spirv"
}

//! the declared workgroup width of a [spirv_kernel] method whose `local_size_x_id` is spec constant 0 - the width a
//! `subgroup = "own"` class's pipeline specializes - or 0 where the method declares no such axis
def private spirv_own_wg_x(kfn : Function?) : int {
    for (ann in kfn.annotations) {
        if (ann.annotation.name == "spirv_kernel" && (find_arg(ann.arguments, "local_size_x_id") ?as tInt ?? -1) == 0) {
            return find_arg(ann.arguments, "local_size_x") ?as tInt ?? 0
        }
    }
    return 0
}

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-readonly-lens")]
def private family_write_mask(st : StructurePtr; mod : Module?; bufs : array<LensBuf>; tracked : table<string>) : uint {
    var wmask = 0u
    mod |> for_each_function("") $(fn) {
        if (!empty(fn.arguments) && fn.arguments[0]._type.structType == st) {
            var is_kernel = false
            for (ann in fn.annotations) {
                if (ann.annotation.name == "spirv_kernel") {
                    is_kernel = true
                    break
                }
            }
            if (is_kernel) {
                var res <- classify_field_access(fn, mod, st, tracked)
                if (!empty(res.err)) {
                    wmask = ~0u
                }
                for (bf in bufs) {
                    if (key_exists(res.writes, bf.name)) {
                        wmask |= 1u << uint(bf.binding)
                    }
                }
                delete res
            }
        }
    }
    return wmask
}

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-readonly-lens")]
def private stamp_readonly(var st : StructurePtr; base : string; write_mask : uint; var errors : das_string) : bool {
    for (field in st.fields) {
        continue if (!(find_arg(field.annotation, "ssbo") is tBool))
        let fb = find_arg(field.annotation, "binding") ?as tInt ?? -1
        continue if (fb < 0)
        let written = (write_mask & (1u << uint(fb))) != 0u
        let declared = find_arg(field.annotation, "readonly") is tBool
        if (written && declared) {
            errors := "[vk_dispatch] {base}: member '{field.name}' is @readonly but a kernel of {st.name} writes binding {fb}"
            return false
        }
        if (!written && !declared && !(find_arg(field.annotation, "writeonly") is tBool) && !(find_arg(field.annotation, "maybe_readonly") is tBool)) {
            field.annotation |> add_annotation_argument("readonly", true)
        }
    }
    return true
}


//! what a `wg_twin_of` class must repeat of the class it names: the bindings, the push struct and the derived access
struct private LensPrimary {
    wg_bytes : int
    blist : string
    args_name : string
    rmask : uint
    wmask : uint
}

def private lens_primary_of(scan : LensScan; rmask, wmask : uint) : LensPrimary
    => LensPrimary(wg_bytes = scan.wg_bytes, blist = join([for (bind in scan.blist); "{bind}"], ","),
        args_name = scan.args_t != null && scan.args_t.structType != null ? "{scan.args_t.structType.name}" : "", rmask = rmask, wmask = wmask)

//! the refusal of a stand-in `si` whose words build into the slot of its primary `pr`, "" where the two share a dispatch surface
def private stand_in_refusal(si, pr : LensPrimary) : string {
    return "it binds [{si.blist}], the primary [{pr.blist}]" if (si.blist != pr.blist)
    return "its push struct is '{si.args_name}', the primary's '{pr.args_name}'" if (si.args_name != pr.args_name)
    return "its kernel body's access differs from the primary's" if (si.rmask != pr.rmask || si.wmask != pr.wmask)
    return ""
}

//! the refusal of the twin `tw` against its primary `pr`, "" where the twin holds
def private wg_twin_refusal(tw, pr : LensPrimary) : string {
    return "its @workgroup footprint ({tw.wg_bytes} B) is not under the primary's ({pr.wg_bytes} B)" if (tw.wg_bytes >= pr.wg_bytes)
    return stand_in_refusal(tw, pr)
}

//! the refusal of the alternate `alt` against its primary `pr`: the primary's footprint gate is the one the slot passes
def private alt_refusal(alt, pr : LensPrimary) : string {
    return "its @workgroup footprint ({alt.wg_bytes} B) passes the primary's ({pr.wg_bytes} B)" if (alt.wg_bytes > pr.wg_bytes)
    return stand_in_refusal(alt, pr)
}

[structure_macro(name = vk_dispatch)]
class VkDispatch : AstStructureAnnotation {
    primaries : table<string; LensPrimary>   // "{module}`{name}" -> what a `wg_twin_of` class naming it must repeat
    // multi-kernel ordinal state: st.annotations is EMPTY at apply time (probe-verified
    // 2026-08-04), but the macro INSTANCE persists across applies — arrival order assigns each
    // instance its pipe slot; seen_ord makes a re-fired instance idempotent (keyed by name=)
    fam_count : table<string; int>   // "{module}`{family}" -> next ordinal
    seen_ord : table<string; int>    // "{module}`{name}" -> assigned ordinal

    def override apply(var st : StructurePtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {   // nolint:STYLE038 — flat emit table
        let at = st.at
        var scan <- lens_scan(st, args, VK_WORDS, fam_count, seen_ord, errors)
        return false if (!scan.ok)
        // census/override display name: the vk_dispatch name= ("<base>_spv"), not the blob
        // global's mangled default — templated instances drop [spirv_kernel] name=, and a
        // backticked "{Class}`run`spirv" makes the SPV-override filename unusable
        let disp_name = "{scan.base}_spv"
        let nb = length(scan.blist)   // the set builder's arity: one buffer/size/bits entry per declared binding
        if (scan.blist[nb - 1] >= VKD_SET_BINDINGS) {
            errors := "[vk_dispatch] {scan.base}: binding {scan.blist[nb - 1]} - the hazard rail's SetBits caps at binding {VKD_SET_BINDINGS - 1}"
            return false
        }

        let acc = lens_access(st, scan, VK_WORDS, errors)
        return false if (acc.kfn == null)
        let kfn = acc.kfn
        let rmask = acc.rmask
        let wmask = acc.wmask
        var tracked <- lens_tracked(scan)
        let write_mask = family_write_mask(st, compiling_module(), scan.bufs, tracked)
        delete tracked
        if (!stamp_readonly(st, scan.base, write_mask, errors)) {
            return false
        }
        let twin_of = find_arg(args, "wg_twin_of") ?as tString ?? ""
        if (!empty(twin_of)) {
            return twin_surface(at, scan, twin_of, spirv_blob_of(kfn), disp_name, rmask, wmask, errors)
        }
        let own_wg_x = own_width_and_alt(at, scan, args, kfn, disp_name, rmask, wmask, errors)
        return false if (own_wg_x < 0)
        primaries["{compiling_module().name}`{scan.base}"] = lens_primary_of(scan, rmask, wmask)

        // ===== the generated surface =====
        // one VkdClass global + one set builder per FAMILY (ordinal 0 generates them); every
        // instance generates its own ensure_<base> (lazy pipe slot) + enc_<base>
        let gname = "{scan.family}`vkd"
        if (scan.ord == 0) {
            if (!add_global_var(compiling_module(), gname, mk_alias_t(at, "VkdClass", false), at, false)) {
                errors := "[vk_dispatch] {scan.base}: can't add class-state global {gname}"
                return false
            }
        }

        return false if (!ensure_surface(at, scan, gname, spirv_blob_of(kfn), disp_name, rmask, wmask, own_wg_x, errors))

        // [init] census seed — the kernel joins the coverage universe at program start, device
        // or not, so a never-dispatched class kernel warns like the module-global kernels do
        {
            var cs = new ExprCall(at = at, name := "vkd_census_seed")
            cs.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
            cs.arguments |> emplace_new <| new ExprConstUInt(at = at, value = rmask)
            cs.arguments |> emplace_new <| new ExprConstUInt(at = at, value = wmask)
            var body <- array<ExpressionPtr>(cs)
            if (!add_generated(at, "{scan.base}`census", new TypeDecl(at = at, baseType = Type.tVoid), body) $(var fn : Function?) {
                    fn.flags.init = true
                    fn.flags.privateFunction = true
                }) {
                errors := "[vk_dispatch] {scan.base}: failed to add the census [init]"
                return false
            }
        }

        // set_<family>(bufs, sizes, gbits) : VkDescriptorSet — a set over the FAMILY layout
        if (scan.ord == 0) {
            var body : array<ExpressionPtr>
            var ws = new ExprCall(at = at, name := "vkd_cached_set")   //! the family's name rides along, so a set asked before the class's ensure names its class
            ws.arguments |> emplace_new <| mk_gvar(at, gname)
            ws.arguments |> emplace_new <| new ExprConstString(at = at, value := scan.family)
            ws.arguments |> emplace_new <| mk_gvar(at, "bufs")
            ws.arguments |> emplace_new <| mk_gvar(at, "sizes")
            ws.arguments |> emplace_new <| mk_gvar(at, "gbits")
            body |> push(new ExprReturn(at = at, subexpr = ws))
            var bufs_t = make_fixed_array_type(nb, new TypeDecl(at = at, baseType = Type.tUInt64))
            bufs_t.flags.constant = true
            var sizes_t = make_fixed_array_type(nb, new TypeDecl(at = at, baseType = Type.tInt64))
            sizes_t.flags.constant = true
            var gbits_t = make_fixed_array_type(nb, new TypeDecl(at = at, baseType = Type.tUInt))
            gbits_t.flags.constant = true
            if (!add_generated(at, "set_{scan.family}", mk_alias_t(at, "VkDescriptorSet", false), body) $(var fn : Function?) {
                    fn.arguments |> emplace_new <| new Variable(at = at, name := "bufs", _type = bufs_t)
                    fn.arguments |> emplace_new <| new Variable(at = at, name := "sizes", _type = sizes_t)
                    fn.arguments |> emplace_new <| new Variable(at = at, name := "gbits", _type = gbits_t)
                }) {
                errors := "[vk_dispatch] {scan.base}: failed to add set_{scan.family}"
                return false
            }
        }

        // enc_<base>(raw, h, set_, pc, params...) — push + hazard dispatch
        {
            var body : array<ExpressionPtr>
            if (scan.args_t != null) {
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
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            dp.arguments |> emplace_new <| mk_grid_dim(at, scan.grid)
            body |> push(dp)
            var fn = new Function(at = at, atDecl = at, name := "enc_{scan.base}",
                flags = FunctionFlags.generated,
                result = new TypeDecl(at = at, baseType = Type.tVoid))
            fn.arguments |> emplace_new <| new Variable(at = at, name := "raw", _type = mk_alias_t(at, "VkCommandBuffer", true))
            var ht = mk_alias_t(at, "VkHaz", false)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "h", _type = ht)
            fn.arguments |> emplace_new <| new Variable(at = at, name := "set_", _type = mk_alias_t(at, "VkDescriptorSet", false))
            if (scan.args_t != null) {
                scan.args_t.flags.ref = true
                fn.arguments |> emplace_new <| new Variable(at = at, name := "pc", _type = scan.args_t)
            }
            return false if (!lens_params(fn, at, args, scan, VK_WORDS, errors))
            fn.body = make_expression_block(body)
            fn.body |> force_at(at)
            if (!(compiling_module() |> add_function(fn))) {
                errors := "[vk_dispatch] {scan.base}: failed to add enc_{scan.base}"
                return false
            }
        }

        // enc_<base>(rl, roles, pc, params...) — the rail overload: the set over the rail's planes at `roles`, the dispatch on its command buffer
        {
            var body : array<ExpressionPtr>
            if (scan.args_t != null) {
                var pp = new ExprCall(at = at, name := "vkd_push")
                pp.arguments |> emplace_new <| new ExprField(at = at, value = mk_gvar(at, "rl"), name := "raw")
                pp.arguments |> emplace_new <| mk_gvar(at, gname)
                pp.arguments |> emplace_new <| mk_gvar(at, "pc")
                body |> push(pp)
            }
            var dp = new ExprCall(at = at, name := "vkd_rail_dispatch")
            dp.arguments |> emplace_new <| mk_gvar(at, "rl")
            dp.arguments |> emplace_new <| mk_gvar(at, gname)
            dp.arguments |> emplace_new <| new ExprConstString(at = at, value := scan.family)
            dp.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
            dp.arguments |> emplace_new <| mk_gvar(at, "roles")
            dp.arguments |> emplace_new <| mk_grid_dim(at, scan.grid)
            body |> push(dp)
            return false if (!lens_rail_overload(at, args, scan, VK_WORDS, "VtRail", false, body, errors))
        }
        return true
    }

    //! ensure_<base>() : bool - the device up, the family's layouts once, this kernel's pipe slot once
    def ensure_surface(at : LineInfo; scan : LensScan; gname, blob, disp_name : string; rmask, wmask : uint; own_wg_x : int; var errors : das_string) : bool {
        // `array<int>(b0, b1, ...)` — the gen2 array literal (ExprMakeArray wrapped in
        // to_array_move, the spirv_shader capture shape)
        var mkArr = new ExprMakeArray(at = at, makeType = new TypeDecl(at = at, baseType = Type.tInt))
        mkArr.gen2 = true
        for (bind in scan.blist) {
            mkArr.values |> emplace_new <| new ExprConstInt(at = at, value = bind)
        }
        var bl = new ExprCall(at = at, name := "to_array_move")
        bl.arguments |> emplace(mkArr)
        var ci = new ExprCall(at = at, name := "vkd_class_init")
        ci.arguments |> emplace_new <| mk_gvar(at, gname)
        ci.arguments |> emplace_new <| bl
        ci.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.args_fields * 4)
        var cp = new ExprCall(at = at, name := "vkd_class_pipe")
        cp.arguments |> emplace_new <| mk_gvar(at, gname)
        cp.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
        cp.arguments |> emplace_new <| mk_gvar(at, blob)
        cp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = rmask)
        cp.arguments |> emplace_new <| new ExprConstUInt(at = at, value = wmask)
        cp.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
        cp.arguments |> emplace_new <| new ExprConstInt(at = at, value = own_wg_x)
        var pm = new ExprCall(at = at, name := "vkd_pipe_made")
        pm.arguments |> emplace_new <| mk_gvar(at, gname)
        pm.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.ord)
        var body : array<ExpressionPtr>
        body |> push(mk_if_ret(at, mk_not(at, new ExprCall(at = at, name := "vk_moe_init")), false))
        if (scan.wg_bytes > 0) {
            // over-cap classes decline by name BEFORE the pipeline build (MoltenVK's 32 KB
            // threadgroup cap fails the COMPILE otherwise — an opaque INITIALIZATION_FAILED)
            var wf = new ExprCall(at = at, name := "vkd_wg_fits")
            wf.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.wg_bytes)
            wf.arguments |> emplace_new <| new ExprConstString(at = at, value := scan.base)
            body |> push(mk_if_ret(at, mk_not(at, wf), false))
        }
        body |> push(mk_if_do(at, mk_not(at, new ExprField(at = at, value = mk_gvar(at, gname), name := "ready")), ci))
        body |> push(mk_if_do(at, mk_not(at, pm), cp))
        body |> push(mk_ret_bool(at, true))
        if (!add_generated(at, "ensure_{scan.base}", new TypeDecl(at = at, baseType = Type.tBool), body) $(var _fn : Function?) {}) {
            errors := "[vk_dispatch] {scan.base}: failed to add ensure_{scan.base}"
            return false
        }
        return true
    }

    //! the declared workgroup width a `subgroup = "own"` class's pipeline specializes, 0 where the class keeps the tier's
    //! pin, -1 with `errors` set where the argument is refused
    def own_width_of(scan : LensScan; args : AnnotationArgumentList; kfn : Function?; var errors : das_string) : int {
        let subgroup = find_arg(args, "subgroup") ?as tString ?? ""
        return 0 if (empty(subgroup))
        if (subgroup != "own") {
            errors := "[vk_dispatch] {scan.base}: subgroup = \"{subgroup}\" - the one value is \"own\""
            return -1
        }
        let own_wg_x = spirv_own_wg_x(kfn)
        if (own_wg_x <= 0) {
            errors := "[vk_dispatch] {scan.base}: subgroup = \"own\" needs a [spirv_kernel] declaring local_size_x with local_size_x_id = 0 - the pipeline specializes that width"
            return -1
        }
        return own_wg_x
    }

    //! `own_width_of`, the class's `alt_of` seeded under it where the class names one; -1 with `errors` set on either refusal
    def own_width_and_alt(at : LineInfo; scan : LensScan; args : AnnotationArgumentList; kfn : Function?; disp_name : string; rmask, wmask : uint; var errors : das_string) : int {
        let own_wg_x = own_width_of(scan, args, kfn, errors)
        let alt_of = find_arg(args, "alt_of") ?as tString ?? ""
        return own_wg_x if (own_wg_x < 0 || empty(alt_of))
        return alt_seed(at, scan, alt_of, find_arg(args, "alt_bit") ?as tInt ?? -1, own_wg_x, spirv_blob_of(kfn), disp_name, rmask, wmask, errors) ? own_wg_x : -1
    }

    //! an `alt_of` class keeps its own dispatch surface - the race that picks it dispatches both stamps - and its words join
    //! the primary's pipe slot at program start under bit `bit` of the served set
    def alt_seed(at : LineInfo; scan : LensScan; alt_of : string; bit, own_wg_x : int; blob, disp_name : string; rmask, wmask : uint; var errors : das_string) : bool {
        let pkey = "{compiling_module().name}`{alt_of}"
        var why = key_exists(primaries, pkey) ? "" : "'{alt_of}' names no [vk_dispatch] class declared above it in this module"
        primaries |> get(pkey) $(pr) {
            why = alt_refusal(lens_primary_of(scan, rmask, wmask), pr)
        }
        if (empty(why) && (bit < 0 || bit > 62)) {
            why = "alt_bit = {bit} is not a bit of the served set (0..62)"
        }
        if (!empty(why)) {
            errors := "[vk_dispatch] {scan.base}: not an alternate of '{alt_of}' - {why}"
            return false
        }
        var seed = new ExprCall(at = at, name := "vkd_alt_seed")
        seed.arguments |> emplace_new <| new ExprConstString(at = at, value := alt_of)
        seed.arguments |> emplace_new <| mk_gvar(at, blob)
        seed.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
        seed.arguments |> emplace_new <| new ExprConstInt(at = at, value = bit)
        seed.arguments |> emplace_new <| new ExprConstInt(at = at, value = own_wg_x)
        var body <- array<ExpressionPtr>(seed)
        if (!add_generated(at, "{scan.base}`alt", new TypeDecl(at = at, baseType = Type.tVoid), body) $(var fn : Function?) {
                fn.flags.init = true
                fn.flags.privateFunction = true
            }) {
            errors := "[vk_dispatch] {scan.base}: failed to add the alternate [init]"
            return false
        }
        return true
    }

    //! a `wg_twin_of` class generates no dispatch surface: its words join the primary's pipe slot at program start, and its
    //! kernel joins the coverage universe under its own name
    def twin_surface(at : LineInfo; scan : LensScan; twin_of, blob, disp_name : string; rmask, wmask : uint; var errors : das_string) : bool {
        let pkey = "{compiling_module().name}`{twin_of}"
        if (!key_exists(primaries, pkey)) {
            errors := "[vk_dispatch] {scan.base}: wg_twin_of '{twin_of}' names no [vk_dispatch] class declared above it in this module"
            return false
        }
        var why = ""
        var over_bytes = 0
        primaries |> get(pkey) $(pr) {
            why = wg_twin_refusal(lens_primary_of(scan, rmask, wmask), pr)
            over_bytes = pr.wg_bytes
        }
        if (!empty(why)) {
            errors := "[vk_dispatch] {scan.base}: not a twin of '{twin_of}' - {why}"
            return false
        }
        var ts = new ExprCall(at = at, name := "vkd_twin_seed")
        ts.arguments |> emplace_new <| new ExprConstString(at = at, value := twin_of)
        ts.arguments |> emplace_new <| new ExprConstInt(at = at, value = over_bytes)
        ts.arguments |> emplace_new <| mk_gvar(at, blob)
        ts.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
        ts.arguments |> emplace_new <| new ExprConstInt(at = at, value = scan.wg_bytes)
        var cs = new ExprCall(at = at, name := "vkd_census_seed")
        cs.arguments |> emplace_new <| new ExprConstString(at = at, value := disp_name)
        cs.arguments |> emplace_new <| new ExprConstUInt(at = at, value = rmask)
        cs.arguments |> emplace_new <| new ExprConstUInt(at = at, value = wmask)
        var body <- array<ExpressionPtr>(ts, cs)
        if (!add_generated(at, "{scan.base}`twin", new TypeDecl(at = at, baseType = Type.tVoid), body) $(var fn : Function?) {
                fn.flags.init = true
                fn.flags.privateFunction = true
            }) {
            errors := "[vk_dispatch] {scan.base}: failed to add the twin [init]"
            return false
        }
        return true
    }
}
