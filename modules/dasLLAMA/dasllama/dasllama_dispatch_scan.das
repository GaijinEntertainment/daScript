options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_dispatch_scan shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require strings
require daslib/strings_boost
require dasllama_kernel_access

// The member scan the two discrete dispatch lenses ([vk_dispatch], [cuda_dispatch]) share: the annotation's arguments
// and the family ordinal, the member walk, the kernel method and its derived access, and the encoder's params=
// arguments. Every refusal speaks the calling lens's words; what a lens generates stays in the lens.

//! the words a lens's refusals carry
struct LensWords {
    lens : string           //! the annotation as the refusals name it, `[vk_dispatch]`
    kernel : string         //! the kernel method's annotation, `spirv_kernel`
    grid_note : string      //! the tail of the 1D-grid refusal
    uniform_note : string   //! what the @uniform refusal points the author at
    args_many : string      //! the refusal of a second args member
    args_word : string      //! the args member's name in its own refusals
}

//! one @ssbo member: its name, binding and declared @role
struct LensBuf {
    name : string
    binding : int
    role : string
}

//! a dispatch class's arguments and members, as the lens generates from them
struct LensScan {
    ok : bool
    base : string
    grid : string
    kernel_prefix : string
    family : string
    multi_kernel : bool
    ord : int                   //! the instance's arrival ordinal within its family
    bufs : array<LensBuf>       //! the @ssbo members in declaration order
    @do_not_delete args_t : TypeDeclPtr   //! the args struct's type, cloned; null when the class has none
    args_fields : int
    wg_bytes : int              //! the @workgroup members' footprint
    blist : array<int>          //! the distinct bindings, ascending
}

//! the instance's arrival ordinal within its family: the macro instance persists across applies, so the first apply of a
//! name takes the family's next ordinal and a re-fired one keeps its own
def private lens_ordinal(base, family : string; var fam_count, seen_ord : table<string; int>) : int {
    let mname = "{compiling_module().name}"
    let bkey = "{mname}`{base}"
    let fkey = "{mname}`{family}"
    return seen_ord?[bkey] ?? 0 if (key_exists(seen_ord, bkey))
    let ord = fam_count?[fkey] ?? 0
    seen_ord[bkey] = ord
    fam_count[fkey] = ord + 1
    return ord
}

//! the annotation's name=, grid=, kernel= and family= and the family ordinal: `ok` false with `errors` set at the first refusal
def private lens_args(args : AnnotationArgumentList; w : LensWords; var fam_count, seen_ord : table<string; int>;
                      var errors : das_string) : LensScan {
    let base = find_arg(args, "name") ?as tString ?? ""
    let grid = find_arg(args, "grid") ?as tString ?? ""
    let kernel_prefix = find_arg(args, "kernel") ?as tString ?? ""
    let famArg = find_arg(args, "family") ?as tString ?? ""
    var s <- LensScan(base = base, grid = grid, kernel_prefix = kernel_prefix, family = empty(famArg) ? base : famArg,
        multi_kernel = !empty(kernel_prefix))
    if (empty(base)) {
        errors := "{w.lens}: 'name' (the generated surface's base name) is required"
        return <- s
    }
    if (!s.multi_kernel && !empty(famArg)) {
        errors := "{w.lens} {base}: family= without kernel= - a family instance must name its [{w.kernel}] method"
        return <- s
    }
    s.ord = lens_ordinal(base, s.family, fam_count, seen_ord)
    if (empty(grid)) {
        errors := "{w.lens} {base}: 'grid' is required (1D: int | param | param/coverage)"
        return <- s
    }
    if (find(grid, ",") >= 0) {
        errors := "{w.lens} {base}: grid '{grid}' - only a 1D grid is supported{w.grid_note}"
        return <- s
    }
    let dparts <- split(grid, "/")
    if (length(dparts) > 2) {
        errors := "{w.lens} {base}: grid '{strip(grid)}' has more than one '/' (int | param | param/coverage)"
        return <- s
    }
    s.ok = true
    return <- s
}

//! the args member's struct: 4-byte scalar fields alone, cloned into `s.args_t`; false with `errors` set at the first refusal
def private lens_push_member(field : FieldDeclaration; var s : LensScan; w : LensWords; var errors : das_string) : bool {
    if (s.args_t != null) {
        errors := "{w.lens} {s.base}: {w.args_many}"
        return false
    }
    if (field._type == null || field._type.baseType != Type.tStructure || field._type.structType == null) {
        errors := "{w.lens} {s.base}: {w.args_word} member '{field.name}' must be a struct"
        return false
    }
    for (pf in field._type.structType.fields) {
        let bt = pf._type != null ? pf._type.baseType : Type.none
        if (bt != Type.tInt && bt != Type.tUInt && bt != Type.tFloat) {
            errors := "{w.lens} {s.base}: {w.args_word} field '{pf.name}' must be a 4-byte scalar (int/uint/float)"
            return false
        }
        s.args_fields++
    }
    s.args_t = clone_type(unsafe(reinterpret<TypeDeclPtr>(field._type)))
    return true
}

//! the member walk: the @ssbo members with their bindings and roles, the args member, the @workgroup footprint; false with
//! `errors` set at the first refusal
def private lens_members(st : StructurePtr; var s : LensScan; w : LensWords; var errors : das_string) : bool {
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
                errors := "{w.lens} {s.base}: member '{field.name}' is @uniform - {w.uniform_note}"
                return false
            }
        }
        if (is_wg) {
            s.wg_bytes += field._type.sizeOf
            continue
        }
        if (is_push) {
            return false if (!lens_push_member(field, s, w, errors))
            continue
        }
        continue if (!is_ssbo)
        let binding = find_arg(field.annotation, "binding") ?as tInt ?? -1
        if (binding < 0) {
            errors := "{w.lens} {s.base}: @ssbo member '{field.name}' has no @binding"
            return false
        }
        let role = find_arg(field.annotation, "role") ?as tString ?? ""
        if (!role_ok(role)) {
            errors := "{w.lens} {s.base}: member '{field.name}' has unknown @role '{role}' (read/write/readwrite/weight/alias)"
            return false
        }
        s.bufs |> push(LensBuf(name = "{field.name}", binding = binding, role = role))
    }
    if (empty(s.bufs)) {
        errors := "{w.lens} {s.base}: no @ssbo members to bind"
        return false
    }
    return true
}

//! `ok` false with `errors` set at the first refusal; `blist` holds the distinct bindings ascending
def lens_scan(var st : StructurePtr; args : AnnotationArgumentList; w : LensWords; var fam_count, seen_ord : table<string; int>;
              var errors : das_string) : LensScan {
    var s <- lens_args(args, w, fam_count, seen_ord, errors)
    return <- s if (!s.ok)
    s.ok = false
    kargs_as(st, "push_constant")
    return <- s if (!lens_members(st, s, w, errors))
    var covered : table<int>
    for (bf in s.bufs) {
        if (!key_exists(covered, bf.binding)) {
            covered |> insert(bf.binding)
            s.blist |> push(bf.binding)
        }
    }
    sort(s.blist)
    delete covered
    s.ok = true
    return <- s
}

//! the @ssbo members' names: the set the access classifier tracks
def lens_tracked(s : LensScan) : table<string> {
    var tracked : table<string>
    for (bf in s.bufs) {
        tracked |> insert(bf.name)
    }
    return <- tracked
}

//! the refusal of member `bf`'s declared @role against the role the kernel body derives, `dr`; "" when it holds
def private role_refusal(bf : LensBuf; dr : string; s : LensScan; res : AccessResult) : string {
    let decl = bf.role
    if (decl == "weight") {
        return key_exists(res.writes, bf.name) ? "member '{bf.name}' declares @role='weight' but the kernel body writes it" : ""
    } elif (s.multi_kernel) {
        let cov = (empty(dr) || decl == "" || decl == "alias" || decl == dr
            || (decl == "readwrite" && (dr == "read" || dr == "write")))
        return cov ? "" : "member '{bf.name}' declares @role='{decl}' but kernel '{s.kernel_prefix}' derives '{dr}' - the declared role must cover every kernel"
    } elif (decl != "" && decl != "alias" && decl != dr) {
        let drName = empty(dr) ? "unused" : dr
        return "member '{bf.name}' declares @role='{decl}' but the body derives '{drName}' - fix whichever is wrong"
    } elif (decl == "" && empty(dr)) {
        return "@ssbo member '{bf.name}' is never accessed by the kernel body - declare @role (weight/alias) or drop the binding"
    }
    return ""
}

//! the class's kernel method; null with `errors` set when the class has none or a kern= prefix names none or several
def private lens_kernel(st : StructurePtr; s : LensScan; w : LensWords; var errors : das_string) : Function? {
    var nmatch = 0
    var kfn = find_kernel_fn(st, compiling_module(), w.kernel, s.kernel_prefix, nmatch)
    if (empty(s.kernel_prefix)) {
        if (kfn == null || nmatch != 1) {
            errors := "{w.lens} {s.base}: expected exactly one [{w.kernel}] method on {st.name} (found {nmatch}) - multi-kernel classes pick per instance via kernel="
            return null
        }
    } elif (kfn == null) {
        errors := "{w.lens} {s.base}: no [{w.kernel}] method '{s.kernel_prefix}' on {st.name}"
        return null
    } elif (nmatch != 1) {
        errors := "{w.lens} {s.base}: kernel='{s.kernel_prefix}' names {nmatch} [{w.kernel}] methods on {st.name} - a prefix picks exactly one"
        return null
    }
    return kfn
}

//! every declared @role checked against the derived masks: `kfn` null with `errors` set at the first refusal
def lens_access(st : StructurePtr; s : LensScan; w : LensWords; var errors : das_string) : tuple<kfn : Function?; rmask : uint; wmask : uint> {
    let kfn = lens_kernel(st, s, w, errors)
    return (kfn = null, rmask = 0u, wmask = 0u) if (kfn == null)
    var tracked <- lens_tracked(s)
    var res <- classify_field_access(kfn, compiling_module(), st, tracked)
    if (!empty(res.err)) {
        access_fail(errors, "{w.lens} {s.base}: {res.err}", res, tracked)
        return (kfn = null, rmask = 0u, wmask = 0u)
    }
    var rmask = 0u
    var wmask = 0u
    for (bf in s.bufs) {
        let refusal = role_refusal(bf, derived_role(res, bf.name), s, res)
        if (!empty(refusal)) {
            access_fail(errors, "{w.lens} {s.base}: {refusal}", res, tracked)
            return (kfn = null, rmask = 0u, wmask = 0u)
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
    return (kfn = kfn, rmask = rmask, wmask = wmask)
}

def private is_int64_param(params, name : string) : bool {
    for (p in split(params, ",")) {
        var inscope nt <- split(p, ":")
        return true if (length(nt) == 2 && strip(nt[0]) == name && strip(nt[1]) == "int64")
    }
    return false
}

//! every name a divide-form grid reads must be an int64 param (the generated ceil-div math is int64); false with `errors` set
def lens_params(var fn : Function?; at : LineInfo; args : AnnotationArgumentList; s : LensScan; w : LensWords; var errors : das_string) : bool {
    let params = find_arg(args, "params") ?as tString ?? ""
    if (!empty(strip(params))) {
        for (p in split(params, ",")) {
            var inscope nt <- split(p, ":")
            if (length(nt) != 2) {
                errors := "{w.lens} {s.base}: param '{strip(p)}' must be 'name : type'"
                return false
            }
            var ok = true
            var pt = param_type(at, strip(nt[1]), ok)
            if (!ok) {
                errors := "{w.lens} {s.base}: param '{strip(p)}' has unsupported type '{strip(nt[1])}' (int/uint/int64/uint64)"
                return false
            }
            pt.flags.constant = true
            fn.arguments |> emplace_new <| new Variable(at = at, name := "{strip(nt[0])}", _type = pt)
        }
    }
    return true if (find(s.grid, "/") < 0)
    for (gtok in split(s.grid, "/")) {
        let gs = strip(gtok)
        continue if (is_digit_tok(gs))
        if (!is_int64_param(params, gs)) {
            errors := "{w.lens} {s.base}: grid '{s.grid}' references '{gs}', which must be declared an int64 param (the generated ceil-div math is int64)"
            return false
        }
    }
    return true
}
