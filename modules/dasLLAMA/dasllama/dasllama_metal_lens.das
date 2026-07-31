options gen2
options indenting = 4

module dasllama_metal_lens shared private

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/rtti
require strings
require daslib/strings_boost   // split / strip — the grid & params micro-format tokenizers
require das_metal   // nolint:STYLE030 — MetalBuffer/MetalComputeEncoder are referenced only inside qmacro_type(type<...>), which STYLE030 can't see; the require is load-bearing for macro-time type resolution
require dasllama_kernel_access   // the shared body-walk classifier (field mode) — @role's read/write axis is DERIVED

// The dasLLAMA Metal kernel-dispatch lens. [metal_dispatch] reads a [metal_kernel] class's
// @ssbo/@uniform field annotations and generates the enc_* dispatch builder that the hand-
// written twin used to be: binds in @binding order (@off names an offset param), hz from
// @role/@span, gate, dispatch. Variation points (fmt dispatchers, dummy binds, pso twins)
// stay hand-written as thin wrappers over generated builders. Every @ssbo on a lensed class
// MUST declare a @role (the compile-time half of the strictness ratchet; runtime STRICT
// covers the un-lensed escapes).

struct private BufField {
    name : string
    binding : int
    role : string
    hzrole : string  // the direction hz staging emits — resolved role, or an alias view's OWN
                     // derived access (the tracker keys ranges by buffer, so staging both views
                     // unions naturally; hand-written builders encoded that union on the owner)
    ssbo : bool      // @ssbo (vs @uniform) — only ssbo fields join the access derivation
    // a struct-typed @uniform is a kargs value: it binds by VALUE (kn_kargs), so the builder
    // takes the struct itself, not a MetalBuffer?. Null = an ordinary buffer field.
    @do_not_delete kt : TypeDeclPtr
    off : string     // @off = "<param>" — bind at a caller-passed uint64 offset ("" = 0)
    rng : string     // @span = "<a*b*4>" product over params — exact hz length ("" = whole buffer)
}

def private mk_kn_pipeline(at : LineInfo; pso : string) : ExpressionPtr {
    var c = new ExprCall(at = at, name := "kn_pipeline")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := "enc")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := pso)
    return c
}

def private mk_kn_tgmem(at : LineInfo; tgmem : string) : ExpressionPtr {
    var c = new ExprCall(at = at, name := "kn_tgmem")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := "enc")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := tgmem)
    c.arguments |> emplace_new <| new ExprConstInt(at = at, value = 0)
    return c
}

def private mk_off_arg(at : LineInfo; off : string) : Expression? {
    if (empty(off)) {
        return new ExprConstUInt64(at = at, value = 0ul)
    }
    return new ExprVar(at = at, name := off)
}

def private mk_kn_kargs(at : LineInfo; pname : string; binding : int) : ExpressionPtr {
    var c = new ExprCall(at = at, name := "kn_kargs")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := "enc")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := pname)
    c.arguments |> emplace_new <| new ExprConstInt(at = at, value = binding)
    return c
}

def private mk_kn_buffer(at : LineInfo; pname : string; binding : int; off : string) : ExpressionPtr {
    var c = new ExprCall(at = at, name := "kn_buffer")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := "enc")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := pname)
    c.arguments |> emplace_new <| mk_off_arg(at, off)
    c.arguments |> emplace_new <| new ExprConstInt(at = at, value = binding)
    return c
}

// The type of a struct-typed @uniform (the kargs form), null for an ordinary buffer field. At
// structure-macro time the declared type may still be an unresolved ALIAS, so the parameter gets
// a CLONE of exactly what the field declared and infer resolves both the same way.
def private kargs_type_of(ft : TypeDecl const?; is_ssbo : bool) : TypeDeclPtr {
    if (is_ssbo || ft == null || (ft.baseType != Type.tStructure && empty(ft.alias))) {
        return null
    }
    return clone_type(unsafe(reinterpret<TypeDeclPtr>(ft)))
}

// one bind statement: a kargs value binds by value, everything else is a buffer at an offset
def private mk_bind(at : LineInfo; bf : BufField) : ExpressionPtr {
    return (bf.kt == null ? mk_kn_buffer(at, "b{bf.name}", bf.binding, bf.off)
        : mk_kn_kargs(at, "{bf.name}", bf.binding))
}

// ... and its builder parameter: the struct itself for kargs, a buffer handle otherwise
def private mk_builder_param(at : LineInfo; var bf : BufField) : VariablePtr {
    if (bf.kt == null) {
        return new Variable(at = at, name := "b{bf.name}", _type = const_t(qmacro_type(type<MetalBuffer?>)))
    }
    // kn_kargs takes the struct as `var` (it needs its address), so the builder's parameter is a
    // mutable by-ref struct — the `var ka : Foo` a hand-written builder would have declared
    bf.kt.flags.ref = true
    return new Variable(at = at, name := "{bf.name}", _type = bf.kt)
}

def private mk_var_call(at : LineInfo; call_name, arg0 : string) : ExpressionPtr {
    var c = new ExprCall(at = at, name := call_name)
    c.arguments |> emplace_new <| new ExprVar(at = at, name := arg0)
    return c
}

def private mk_uint_cast(at : LineInfo; var inner : Expression?) : Expression? {
    var c = new ExprCall(at = at, name := "uint")
    c.arguments |> emplace_new <| inner
    return c
}

// every character must be a digit — a "4l"/"4x" typo falls through to the name path, where the
// generated builder fails loudly (unresolved variable) instead of silently truncating via to_int
def private is_digit_tok(s : string) : bool {
    if (empty(s)) {
        return false
    }
    var all_digits = true
    peek_data(s) $(arr) {
        for (c in arr) {
            if (!is_number(int(c))) {
                all_digits = false
                break
            }
        }
    }
    return all_digits
}

// @span product: factors 'param' | int, folded left — "n*4" -> uint64(n * 4l)
def private mk_range_len(at : LineInfo; spec : string) : Expression? {
    var e : Expression?
    for (tok in split(spec, "*")) {
        let t = strip(tok)
        var f : Expression?
        if (is_digit_tok(t)) {
            f = new ExprConstInt64(at = at, value = int64(to_int(t)))
        } else {
            f = new ExprVar(at = at, name := t)
        }
        if (e == null) {
            e = f
        } else {
            e = new ExprOp2(at = at, op := "*", left = e, right = f)
        }
    }
    var c = new ExprCall(at = at, name := "uint64")
    c.arguments |> emplace_new <| e
    return c
}

// hz_read/hz_write for one field: whole-buffer, or (@off, @span) exact
def private mk_hz(at : LineInfo; call_name : string; bf : BufField) : ExpressionPtr {
    var c = new ExprCall(at = at, name := call_name)
    c.arguments |> emplace_new <| new ExprVar(at = at, name := "b{bf.name}")
    if (!empty(bf.rng)) {
        c.arguments |> emplace_new <| mk_off_arg(at, bf.off)
        c.arguments |> emplace_new <| mk_range_len(at, bf.rng)
    }
    return c
}

// One grid dim: "1" -> 1u ; "name" -> uint(name) ; "name/cov" -> ceil. A literal coverage folds
// the bias — uint((name + 255l) / 256l) — matching the hand-written spelling; a named coverage
// (global or param) emits uint((name + cov - 1l) / cov).
def private mk_grid_dim(at : LineInfo; spec : string) : Expression? {
    let s = strip(spec)
    if (s == "1") {
        return new ExprConstUInt(at = at, value = 1u)
    }
    let parts <- split(s, "/")
    if (length(parts) == 1) {
        return mk_uint_cast(at, new ExprVar(at = at, name := s))
    }
    let name = strip(parts[0])
    let cov = strip(parts[1])
    var div : Expression?
    if (is_digit_tok(cov)) {
        let cv = int64(to_int(cov))
        var add = new ExprOp2(at = at, op := "+", left = new ExprVar(at = at, name := name), right = new ExprConstInt64(at = at, value = cv - 1l))
        div = new ExprOp2(at = at, op := "/", left = add, right = new ExprConstInt64(at = at, value = cv))
    } else {
        var add = new ExprOp2(at = at, op := "+", left = new ExprVar(at = at, name := name), right = new ExprVar(at = at, name := cov))
        var sub = new ExprOp2(at = at, op := "-", left = add, right = new ExprConstInt64(at = at, value = 1l))
        div = new ExprOp2(at = at, op := "/", left = sub, right = new ExprVar(at = at, name := cov))
    }
    return mk_uint_cast(at, div)
}

// grid = comma-separated per WorkGroupID dim; padded to uint3 with trailing 1u.
def private mk_grid(at : LineInfo; grid : string) : Expression? {
    let dims <- split(grid, ",")
    var c = new ExprCall(at = at, name := "uint3")
    for (i in range(3)) {
        if (i < length(dims)) {
            c.arguments |> emplace_new <| mk_grid_dim(at, dims[i])
        } else {
            c.arguments |> emplace_new <| new ExprConstUInt(at = at, value = 1u)
        }
    }
    return c
}

// tg = threads-per-threadgroup: int constant, or "param" | "name*int" (e.g. "g_gemv_rows*32")
def private mk_tg_dim(at : LineInfo; tgi : int; tgs : string) : Expression? {
    if (empty(tgs)) {
        return new ExprConstUInt(at = at, value = uint(tgi))
    }
    let parts <- split(tgs, "*")
    if (length(parts) == 1) {
        return mk_uint_cast(at, new ExprVar(at = at, name := strip(tgs)))
    }
    let name = strip(parts[0])
    let mult = int64(to_int(strip(parts[1])))
    var mul = new ExprOp2(at = at, op := "*", left = new ExprVar(at = at, name := name), right = new ExprConstInt64(at = at, value = mult))
    return mk_uint_cast(at, mul)
}

def private mk_tg(at : LineInfo; tgi : int; tgs : string) : Expression? {
    var c = new ExprCall(at = at, name := "uint3")
    c.arguments |> emplace_new <| mk_tg_dim(at, tgi, tgs)
    c.arguments |> emplace_new <| new ExprConstUInt(at = at, value = 1u)
    c.arguments |> emplace_new <| new ExprConstUInt(at = at, value = 1u)
    return c
}

def private mk_kn_dispatch(at : LineInfo; grid : string; tgi : int; tgs : string) : Expression? {
    var c = new ExprCall(at = at, name := "kn_dispatch")
    c.arguments |> emplace_new <| new ExprVar(at = at, name := "enc")
    c.arguments |> emplace_new <| mk_grid(at, grid)
    c.arguments |> emplace_new <| mk_tg(at, tgi, tgs)
    return c
}

// map a params-string type token to the param TypeDecl (the size/offset scalars the grid references)
// a generated builder param is non-`var`, so its type carries the trailing const (the const model)
def private const_t(var t : TypeDecl?) : TypeDecl? {
    t.flags.constant = true
    return t
}

def private param_type(tok : string; var ok : bool&) : TypeDecl? {
    if (tok == "int64") { return qmacro_type(type<int64>) }
    if (tok == "uint64") { return qmacro_type(type<uint64>) }
    if (tok == "int") { return qmacro_type(type<int>) }
    if (tok == "uint") { return qmacro_type(type<uint>) }
    ok = false
    return qmacro_type(type<int64>)
}

def private role_ok(role : string) : bool {
    // alias = a second view of a buffer another field already declares (hazard lives there);
    // weight = never written during a step (untracked by design); "" = derived from the body.
    // read/write/readwrite stay legal as cross-checked overrides of the derivation.
    return (role == "" || role == "read" || role == "write" || role == "readwrite" ||
        role == "weight" || role == "alias")
}

// the [metal_kernel] method of a lensed class — the body the access classifier walks
def private find_kernel_fn(st : StructurePtr; mod : Module?; var nmatch : int&) : Function? {
    var kfn : Function?
    mod |> for_each_function("") $(fn) {
        if (!empty(fn.arguments) && fn.arguments[0]._type.structType == st) {
            for (ann in fn.annotations) {
                if (ann.annotation.name == "metal_kernel") {
                    nmatch++
                    if (kfn == null) {
                        kfn = fn
                    }
                }
            }
        }
    }
    return kfn
}

// derived direction of one tracked field off the classifier result
def private derived_role(res : AccessResult; name : string) : string {
    let r = key_exists(res.reads, name)
    let w = key_exists(res.writes, name)
    return w ? (r ? "readwrite" : "write") : (r ? "read" : "")
}

[structure_macro(name = metal_dispatch)]
class MetalDispatch : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let at = st.at
        let fn_name = find_arg(args, "name") ?as tString ?? ""
        let pso = find_arg(args, "pso") ?as tString ?? ""
        let tgmem = find_arg(args, "tgmem") ?as tString ?? ""
        let tgi = find_arg(args, "tg") ?as tInt ?? -1
        let tgs = find_arg(args, "tg") ?as tString ?? ""
        let grid = find_arg(args, "grid") ?as tString ?? ""

        if (empty(fn_name)) {
            errors := "[metal_dispatch]: 'name' (the enc_ builder name) is required"
            return false
        }
        if (empty(pso)) {
            errors := "[metal_dispatch]: 'pso' (the pipeline global name) is required"
            return false
        }
        if (tgi <= 0 && empty(tgs)) {
            errors := "[metal_dispatch] {fn_name}: 'tg' must be a positive int constant or a 'param' / 'name*int' string"
            return false
        }
        if (empty(grid)) {
            errors := "[metal_dispatch] {fn_name}: 'grid' is required (per WorkGroupID dim: 1 | param | param/coverage)"
            return false
        }
        // micro-grammar validation up front — a malformed spec must fail the compile here, not
        // silently emit wrong dispatch geometry (mk_* helpers assume validated input)
        if (!empty(strip(tgs))) {
            let tparts <- split(tgs, "*")
            if (length(tparts) > 2) {
                errors := "[metal_dispatch] {fn_name}: tg '{tgs}' has more than one '*' (int const | param | name*int)"
                return false
            }
            if (length(tparts) == 2 && !is_digit_tok(strip(tparts[1]))) {
                errors := "[metal_dispatch] {fn_name}: tg '{tgs}' multiplier '{strip(tparts[1])}' must be an integer literal"
                return false
            }
        }
        for (dspec in split(grid, ",")) {
            let dparts <- split(dspec, "/")
            if (length(dparts) > 2) {
                errors := "[metal_dispatch] {fn_name}: grid dim '{strip(dspec)}' has more than one '/' (1 | param | param/coverage)"
                return false
            }
        }

        // Collect @ssbo/@uniform buffer fields in @binding order. @workgroup + hidden/method
        // fields are skipped; field-annotation validity is msl_emit's job, not the lens's.
        var bufs : array<BufField>
        bufs |> reserve(length(st.fields))
        for (field in st.fields) {
            var is_ssbo = false
            var is_buf = false
            var is_wg = false
            for (annDecl in field.annotation) {
                if (annDecl.name == "ssbo") {
                    is_ssbo = true
                    is_buf = true
                } elif (annDecl.name == "uniform") {
                    is_buf = true
                } elif (annDecl.name == "workgroup") {
                    is_wg = true
                }
            }
            if (is_wg || !is_buf) continue
            let binding = find_arg(field.annotation, "binding") ?as tInt ?? -1
            if (binding < 0) {
                errors := "[metal_dispatch] {fn_name}: field '{field.name}' is @ssbo/@uniform but has no @binding"
                return false
            }
            let role = find_arg(field.annotation, "role") ?as tString ?? ""
            if (!role_ok(role)) {
                errors := "[metal_dispatch] {fn_name}: field '{field.name}' has unknown @role '{role}' (read/write/readwrite/weight/alias)"
                return false
            }
            let off = find_arg(field.annotation, "off") ?as tString ?? ""
            let rangeStr = find_arg(field.annotation, "span") ?as tString ?? ""
            // a struct-typed @uniform is a kargs value: it binds by VALUE, so the builder takes
            // the struct, not a buffer, and the emitter's `constant Foo&` parameter reads it
            var kt = kargs_type_of(field._type, is_ssbo)
            bufs |> push(BufField(name = "{field.name}", binding = binding, role = role, ssbo = is_ssbo, kt = kt, off = off, rng = rangeStr))
        }
        sort(bufs) $(a, b) => a.binding < b.binding

        // The read/write axis is DERIVED from the kernel body (shared classifier, field mode).
        // A declared read/write/readwrite is a cross-checked assertion (mismatch = compile
        // error); weight/alias stay declared — frame policy and hazard ownership are not kernel
        // facts — but a body WRITE to either is refused. access_force=true restores the fully
        // declared contract (every @ssbo must carry a @role) when a body is unanalyzable.
        let accessForce = find_arg(args, "access_force") ?as tBool ?? false
        for (bf in bufs) {   // default: staging follows the (declared or later-derived) role
            bf.hzrole = bf.role
        }
        if (accessForce) {
            for (bf in bufs) {
                if (bf.ssbo && empty(bf.role)) {
                    errors := "[metal_dispatch] {fn_name}: access_force=true — @ssbo field '{bf.name}' must declare a @role"
                    return false
                }
            }
        } else {
            var tracked : table<string>
            for (bf in bufs) {
                if (bf.ssbo) {
                    tracked |> insert(bf.name)
                }
            }
            var nmatch = 0
            let kfn = find_kernel_fn(st, compiling_module(), nmatch)
            if (kfn == null) {
                errors := "[metal_dispatch] {fn_name}: no [metal_kernel] method on {st.name} to derive buffer access from (access_force=true to declare instead)"
                delete tracked
                return false
            }
            if (nmatch != 1) {
                errors := "[metal_dispatch] {fn_name}: {nmatch} [metal_kernel] methods matched {st.name} — expected exactly one"
                delete tracked
                return false
            }
            var res <- classify_field_access(kfn, compiling_module(), st, tracked)
            if (!empty(res.err)) {
                errors := "[metal_dispatch] {fn_name}: {res.err} (or access_force=true + declared roles)"
                delete res
                delete tracked
                return false
            }
            for (bf in bufs) {
                if (!bf.ssbo) continue
                let dr = derived_role(res, bf.name)
                let decl = bf.role
                if (decl == "weight") {
                    // a written weight is a contract violation
                    if (key_exists(res.writes, bf.name)) {
                        errors := "[metal_dispatch] {fn_name}: field '{bf.name}' declares @role='weight' but the kernel body writes it"
                        delete res
                        delete tracked
                        return false
                    }
                } elif (decl == "alias") {
                    // an alias view stages its OWN derived access: the tracker keys ranges by
                    // buffer, so both views' staging unions into the buffer-level truth (a write
                    // through the alias must not go unstaged just because the sibling is read-only)
                    bf.hzrole = derived_role(res, bf.name)
                } elif (empty(decl)) {
                    if (empty(dr)) {
                        errors := "[metal_dispatch] {fn_name}: @ssbo field '{bf.name}' is never accessed by the kernel body — declare @role (weight/alias) or drop the binding"
                        delete res
                        delete tracked
                        return false
                    }
                    bf.role = dr
                    bf.hzrole = dr
                } elif (decl != dr) {
                    let drName = empty(dr) ? "unused" : dr
                    var rk <- [for (k in keys(res.reads)); k]
                    var wk <- [for (k in keys(res.writes)); k]
                    sort(rk)   // keys() order is unspecified — sorted for deterministic diagnostics
                    sort(wk)
                    errors := "[metal_dispatch] {fn_name}: field '{bf.name}' declares @role='{decl}' but the body derives '{drName}' — fix whichever is wrong (access_force=true to override) [derived reads: {join(rk, ",")} writes: {join(wk, ",")}]"
                    delete rk
                    delete wk
                    delete res
                    delete tracked
                    return false
                }
            }
            delete res
            delete tracked
        }

        // Emit the dispatch sequence, mirroring the hand-written enc_ builder exactly: binds in
        // binding order, then all hz_reads, then all hz_writes (hz staging is order-free — the
        // gate scans the whole staged set — but reads-then-writes matches the hand-written order).
        var body : array<ExpressionPtr>
        body |> reserve(length(bufs) * 3 + 4)
        body |> push(mk_kn_pipeline(at, pso))
        if (!empty(tgmem)) {
            body |> push(mk_kn_tgmem(at, tgmem))
        }
        for (bf in bufs) {
            body |> push(mk_bind(at, bf))
        }
        for (bf in bufs) {
            if (bf.hzrole == "read" || bf.hzrole == "readwrite") {
                body |> push(mk_hz(at, "hz_read", bf))
            }
        }
        for (bf in bufs) {
            if (bf.hzrole == "write" || bf.hzrole == "readwrite") {
                body |> push(mk_hz(at, "hz_write", bf))
            }
        }
        body |> push(mk_var_call(at, "hz_gate", "enc"))
        body |> push(mk_kn_dispatch(at, grid, tgi, tgs))

        // Build the builder: void return, enc + one MetalBuffer? per @ssbo/@uniform field (its
        // @off uint64 param interleaved right after — shared @off names emit once) + the declared
        // size scalars (params=) the grid references.
        var fn = new Function(at = at, atDecl = at, name := fn_name,
            flags = FunctionFlags.generated,
            result = new TypeDecl(at = at, baseType = Type.tVoid))
        fn.arguments |> emplace_new <| new Variable(at = at, name := "enc", _type = const_t(qmacro_type(type<MetalComputeEncoder?>)))
        var seen_offs : table<string>
        for (bf in bufs) {
            fn.arguments |> emplace_new <| mk_builder_param(at, bf)
            if (!empty(bf.off) && !key_exists(seen_offs, bf.off)) {
                seen_offs |> insert(bf.off)
                fn.arguments |> emplace_new <| new Variable(at = at, name := "{bf.off}", _type = const_t(qmacro_type(type<uint64>)))
            }
        }
        let params = find_arg(args, "params") ?as tString ?? ""
        if (!empty(strip(params))) {
            for (p in split(params, ",")) {
                let nt <- split(p, ":")
                if (length(nt) != 2) {
                    errors := "[metal_dispatch] {fn_name}: param '{strip(p)}' must be 'name : type'"
                    return false
                }
                var ok = true
                var pt = param_type(strip(nt[1]), ok)
                if (!ok) {
                    errors := "[metal_dispatch] {fn_name}: param '{strip(p)}' has unsupported type '{strip(nt[1])}' (int/uint/int64/uint64)"
                    return false
                }
                fn.arguments |> emplace_new <| new Variable(at = at, name := "{strip(nt[0])}", _type = const_t(pt))
            }
        }
        fn.body = make_expression_block(body)
        fn.body |> force_at(at)
        // dump = true drops the generated flag + exports, so ast_dump shows the builder for the
        // byte-identity proof against its hand-written twin; remove after the proof.
        if (find_arg(args, "dump") ?as tBool ?? false) {
            fn.flags = FunctionFlags.exports
        }
        if (!(compiling_module() |> add_function(fn))) {
            errors := "[metal_dispatch]: failed to add generated function '{fn_name}'"
            return false
        }
        return true
    }
}

// ===== the manual-dispatch census (the scheduling-unification ratchet) =====
// Fires once per module in the require chain; engine metal modules only (dasllama*metal*).
// Two checks, one migration opt-out (`options _metal_manual_dispatch = true` — loud,
// module-local, reviewed; end state = the kn_* replay engine alone):
//   1. no direct metal_dispatch_threadgroups — every dispatch goes through the capture rail
//   2. every @ssbo field on every structure carries a @role — closes the un-lensed escape
//      of the strictness ratchet (runtime STRICT covers only rail-side dispatches)

class private MetalCensusVisitor : AstVisitor {
    nkargs : int      // per-function tallies; the driver resets and checks them around each visit
    ndispatch : int
    def override preVisitExprCall(var expr : ExprCall?) : void {
        let base = callee_base_name("{expr.name}")
        if (base == "metal_dispatch_threadgroups") {
            macro_error(compiling_program(), expr.at, "direct metal_dispatch_threadgroups in a dasLLAMA engine module — dispatch through the kn_*/[metal_dispatch] capture rail (migration opt-out: options _metal_manual_dispatch = true)")
        }
        if (base == "kn_kargs") {
            nkargs++
        } elif (base == "kn_dispatch") {
            ndispatch++
        }
    }
}

[lint_macro]
class MetalManualDispatchCensus : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let mname = "{mod.name}"
        return true if (!(mname |> starts_with("dasllama")) || find(mname, "metal") < 0
            || (prog._options |> find_arg("_metal_manual_dispatch") ?as tBool ?? false))
        var v = new MetalCensusVisitor()
        make_visitor(*v) $(adapter) {
            for_each_function(mod, "") $(var fn) {
                v.nkargs = 0
                v.ndispatch = 0
                visit(fn, adapter)
                // A builder that binds kargs must bind them on EVERY dispatch path. A branch that
                // loses its kn_kargs leaves the kernel reading an unbound argument buffer — the
                // GPU produces garbage with no error anywhere.
                if (v.nkargs > 0 && v.nkargs != v.ndispatch) {
                    macro_error(prog, fn.at, "{fn.name}: {v.nkargs} kn_kargs for {v.ndispatch} kn_dispatch — every dispatch path must bind its kargs")
                }
            }
        }
        unsafe {
            delete v
        }
        for_each_structure(mod) $(var st) {
            // lensed classes DERIVE the read/write axis from the kernel body ([metal_dispatch]
            // enforces derivability there); a bare @ssbo is only an error on un-lensed classes,
            // whose hand-written builders still consume declared intent
            var lensed = false
            for (ann in st.annotations) {
                if (ann.annotation.name == "metal_dispatch") {
                    lensed = true
                }
            }
            // un-lensed classes: the declared roles are the hand-written builders' contract —
            // cross-check them against the same body derivation the lens uses, so every
            // declaration in the module is machine-verified, not trust-me
            var kfn : Function?
            var tracked : table<string>
            for (field in st.fields) {
                var is_ssbo = false
                for (annDecl in field.annotation) {
                    if (annDecl.name == "ssbo") {
                        is_ssbo = true
                    }
                }
                continue if (!is_ssbo)
                if (!lensed) {
                    tracked |> insert("{field.name}")
                }
                let role = find_arg(field.annotation, "role") ?as tString ?? ""
                if (empty(role)) {
                    pass   // optional everywhere: undeclared = the body derivation is the truth
                } elif (!role_ok(role)) {
                    macro_error(prog, field.at, "@ssbo field '{field.name}' of {st.name} has unknown @role '{role}' (read/write/readwrite/weight/alias)")
                }
            }
            if (!lensed && !empty(tracked)) {
                var nmatch = 0
                kfn = find_kernel_fn(st, mod, nmatch)
                if (kfn != null && nmatch == 1) {
                    var res <- classify_field_access(kfn, mod, st, tracked)
                    if (empty(res.err)) {   // an unanalyzable body keeps the declared contract as-is
                        for (field in st.fields) {
                            let role = find_arg(field.annotation, "role") ?as tString ?? ""
                            continue if (role != "read" && role != "write" && role != "readwrite")
                            let r = key_exists(res.reads, "{field.name}")
                            let w = key_exists(res.writes, "{field.name}")
                            let dr = w ? (r ? "readwrite" : "write") : (r ? "read" : "")
                            if (dr != role) {
                                let drName = empty(dr) ? "unused" : dr
                                macro_error(prog, field.at, "@ssbo field '{field.name}' of {st.name} declares @role='{role}' but the kernel body derives '{drName}' — fix the stale declaration")
                            }
                        }
                    }
                    delete res
                }
            }
            delete tracked
        }
        return true
    }
}
