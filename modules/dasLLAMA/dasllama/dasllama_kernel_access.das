options gen2
options indenting = 4

module dasllama_kernel_access shared public

require daslib/ast
require daslib/ast_boost
require daslib/rtti
require strings   // ends_with — generic-instance call names carry a module prefix

// Interprocedural read/write classifier over a declared set of tracked module globals — the
// shared analysis half of the GPU dispatch lenses (vulkan today, metal's @role next). Given a
// kernel function, it walks the body (recursing into same-module helpers) and reports which
// tracked globals are read and which are written. Anything it cannot prove — a tracked global
// passed whole as an argument to a call it does not model — lands in `err` so the lens can fail
// the compile instead of silently under-tracking (the strictness ratchet).

let private ACCESS_DEBUG = false   // flip for macro-time access-derivation prints

struct AccessResult {
    reads : table<string>
    writes : table<string>
    err : string   // non-empty = unanalyzable construct; the lens turns it into a compile error
}

// write-root of an assignment target: peel subscripts/fields/swizzles down to the base variable.
// In field mode the peel STOPS at the first ExprField whose name is tracked (a class-field
// buffer) and reports that name; `node` carries the claimable expression either way.
// an ExprField that names a tracked buffer AND whose base is the kernel class itself — the
// name check alone collides with float4 component access (`wv.y`), whose names shadow the
// classic buffer names (x/y/w)
// bodies are PRE-infer at lens time: buffer fields appear as bare ExprVar names, and member
// access is a parser-level ExprField with untyped base — so an ExprField counts as a buffer
// access only when written explicitly as self.<name> (float4 components like `wv.y` shadow the
// classic buffer names and must NOT match)
def private is_self_field(ef : ExprField const?; tracked : table<string>) : bool {
    if (!key_exists(tracked, string(ef.name))) {
        return false
    }
    var base = ef.value
    if (base is ExprRef2Value) {
        base = (base as ExprRef2Value).subexpr
    }
    return base is ExprVar && (base as ExprVar).name == "self"
}

def private write_root_name(e : Expression const?; tracked : table<string>; fieldMode : bool; var node : Expression const?&) : string {
    var cur = e
    while (true) {
        if (cur is ExprAt) {
            cur = (cur as ExprAt).subexpr
        } elif (cur is ExprSafeAt) {
            cur = (cur as ExprSafeAt).subexpr
        } elif (cur is ExprField) {
            let ef = cur as ExprField
            if (fieldMode && is_self_field(ef, tracked)) {
                node = cur
                return string(ef.name)
            }
            cur = ef.value
        } elif (cur is ExprSwizzle) {
            cur = (cur as ExprSwizzle).value
        } elif (cur is ExprRef2Value) {
            cur = (cur as ExprRef2Value).subexpr
        } elif (cur is ExprCall && (cur as ExprCall).name |> ends_with("i_das_ptr_add")) {
            cur = (cur as ExprCall).arguments[0]   // `(p + n)[i] = v` writes through p's buffer
        } else {
            break
        }
    }
    if (cur is ExprVar) {
        let ev = cur as ExprVar
        if (key_exists(tracked, string(ev.name))) {
            node = cur
            return string(ev.name)
        }
    }
    return ""
}

def private is_setop2(op : das_string) : bool {
    return (op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "%="
        || op == "&=" || op == "|=" || op == "^=" || op == "<<=" || op == ">>="
        || op == "&&=" || op == "||=" || op == "^^=")
}

def private is_setop1(op : das_string) : bool => op == "++" || op == "--" || op == "+++" || op == "---"

// tmm2d tensor builtins (dasMetal): pointer operands — C is written, the rest are read.
// Returns the C-argument index, or -1 for any other call. Suffix match, like the tg-protocol
// arms: resolved call names may carry a module/instance prefix.
def private tmm2d_c_arg(cname : string) : int {
    if (cname |> ends_with("tmm2d_f32_bf16_f32") || cname |> ends_with("tmm2d_q8b_f32")) {
        return 7
    }
    return cname |> ends_with("tmm2d_q8_f32") || cname |> ends_with("tmm2d_q8_f16s") ? 11 : -1
}

def private all_digits(s : string) : bool {
    var ok = !empty(s)
    peek_data(s) $(bytes) {
        for (b in bytes) {
            if (!is_number(int(b))) {
                ok = false
            }
        }
    }
    return ok
}

// The plain function name behind a resolved call. Post-infer a call carries the mangled form —
// `__::<module>`<name>` for a plain function, plus a decimal instance hash for a generic
// instance — and every instance of one generic shares its parameters' constness.
def private callee_base_name(cname : string) : string {
    var n = cname
    let cc = find(n, "::")
    if (cc >= 0) {
        n = slice(n, cc + 2)
    }
    var tick = rfind(n, "`")
    if (tick >= 0 && all_digits(slice(n, tick + 1))) {
        n = slice(n, 0, tick)
        tick = rfind(n, "`")
    }
    return tick >= 0 ? slice(n, tick + 1) : n
}

// Direction of the `idx`-th parameter of the same-module helper `cname`: "write" when the callee
// may store through it, "read" otherwise — the same rule the MSL and SPIR-V emitters lower to
// `device` vs `device const`. Every declaration matching the base name must AGREE; a genuine
// overload set that does not keeps the ratchet's refusal.
def private scan_param_dir(cfn : Function?; base : string; idx : int;
                           var dir : string&; var found : bool&; var conflict : bool&) {
    return if (callee_base_name("{cfn.name}") != base || idx >= length(cfn.arguments))
    let pt = cfn.arguments[idx]._type
    var d = pt != null && !pt.flags.constant ? "write" : "read"
    if (pt != null && pt.baseType == Type.tPointer) {
        // for a pointer the POINTEE's const decides — the pointer's own const only says whether
        // the callee may advance it, and an advancing read (`var p : T const?`) is still a read
        d = pt.firstType != null && !pt.firstType.flags.constant ? "write" : "read"
    }
    if (found && dir != d) {
        conflict = true
    }
    dir = d
    found = true
}

def private callee_param_dir(mod : Module?; cname : string; idx : int) : string {
    let base = callee_base_name(cname)
    var dir = ""
    var found = false
    var conflict = false
    mod |> for_each_function(base) $(cfn) {
        scan_param_dir(cfn, base, idx, dir, found, conflict)
    }
    mod |> for_each_generic(base) $(cfn) {
        scan_param_dir(cfn, base, idx, dir, found, conflict)
    }
    return found && !conflict ? dir : ""
}

// one tracked buffer handed WHOLE to a helper — the driver resolves the direction, since the
// module handle it needs for that lookup does not survive into an AstVisitor field
struct private WholeArg {
    buf : string
    callee : string
    idx : int
}

class private AccessVisitor : AstVisitor {
    tracked : table<string>
    whole : array<WholeArg>        // buffers passed whole to a helper (see WholeArg)
    fieldMode : bool               // tracked names are class FIELDS (metal): match self.<name>
                                   // ExprField accesses only (see is_self_field)
    reads : table<string>
    writes : table<string>
    callees : table<string>        // non-intrinsic call names, for the driver's same-module recursion
    claimed : table<string>        // pure-write target tokens, keyed name:line:column — infer
                                   // CLONES subtrees during rewrites, so pointer identity misses
    ptrs : table<string; string>   // buffer-pointer local -> its tracked buffer; the init
                                   // (`var p = unsafe(addr(buf[..]))`) is claimed, and each USE
                                   // decides the direction: a modeled intrinsic arg reads or
                                   // writes precisely, anything else restores a conservative read
    err : string

    def is_tracked(name : das_string) : bool => key_exists(tracked, string(name))

    // record a write; `also_read` covers read-modify-write forms (+=, ++). A pure store claims
    // the target node so the generic read pass does not also count it as a read.
    def note_write(target : Expression?; also_read : bool) {
        var node : Expression const?
        let name = write_root_name(target, tracked, fieldMode, node)
        if (empty(name)) {
            return
        }
        writes |> insert(name)
        if (!also_read) {
            claimed |> insert("{name}:{node.at.line}:{node.at.column}")
        }
        if (ACCESS_DEBUG) {
            print("[access-dbg] write {name} at={int(node.at.line)}:{int(node.at.column)} also_read={also_read}\n")
        }
    }

    // record buffer-pointer locals: `var p = unsafe(addr(buf[..]))` (post-infer ExprRef2Ptr;
    // pre-infer a bare `addr` call). The buf node is claimed — its direction comes from p's uses.
    def override preVisitExprLet(expr : ExprLet?) : void {
        for (v in expr.variables) {
            var init = v.init
            if (init == null) {
                continue
            }
            if (init is ExprRef2Value) {
                init = (init as ExprRef2Value).subexpr
            }
            if (init is ExprUnsafe) {   // pre-infer bodies keep the unsafe() wrapper node
                init = (init as ExprUnsafe).body
            }
            var sub : Expression?
            if (init is ExprRef2Ptr) {
                sub = (init as ExprRef2Ptr).subexpr
            } elif (init is ExprCall && (init as ExprCall).name == "addr" && length((init as ExprCall).arguments) == 1) {
                sub = (init as ExprCall).arguments[0]
            }
            continue if (sub == null || !(sub is ExprAt))
            var node : Expression const?
            let name = write_root_name(sub, tracked, fieldMode, node)
            continue if (empty(name))
            ptrs[string(v.name)] = name
            claimed |> insert("{name}:{node.at.line}:{node.at.column}")
        }
    }

    // A POINTER into a tracked buffer handed to a helper — a pointer local, or an inline
    // `addr(buf[i])`. Direction comes from the callee's parameter, exactly as for a whole
    // buffer; the node is claimed so the conservative read does not also fire.
    def bank_ptr_arg(cur : Expression const?; cname : string; aidx : int) : bool {
        var e = cur
        if (e is ExprUnsafe) {   // pre-infer bodies keep the unsafe() wrapper node
            e = (e as ExprUnsafe).body
        }
        if (e is ExprVar && key_exists(ptrs, string((e as ExprVar).name))) {
            let pn = string((e as ExprVar).name)
            claimed |> insert("{pn}:{e.at.line}:{e.at.column}")
            whole |> emplace(WholeArg(buf = ptrs[pn], callee = cname, idx = aidx))
            return true
        }
        var sub : Expression const?
        if (e is ExprRef2Ptr) {
            sub = (e as ExprRef2Ptr).subexpr
        } elif (e is ExprCall && (e as ExprCall).name == "addr" && length((e as ExprCall).arguments) == 1) {
            sub = (e as ExprCall).arguments[0]
        }
        return false if (sub == null || !(sub is ExprAt))
        var node : Expression const?
        let name = write_root_name(sub, tracked, fieldMode, node)
        return false if (empty(name))
        claimed |> insert("{name}:{node.at.line}:{node.at.column}")
        whole |> emplace(WholeArg(buf = name, callee = cname, idx = aidx))
        return true
    }

    def override preVisitExprCopy(expr : ExprCopy?) : void {
        note_write(expr.left, false)
    }

    def override preVisitExprMove(expr : ExprMove?) : void {
        note_write(expr.left, false)
    }

    def override preVisitExprClone(expr : ExprClone?) : void {
        note_write(expr.left, false)
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (is_setop2(expr.op)) {
            note_write(expr.left, true)
        }
    }

    def override preVisitExprOp1(expr : ExprOp1?) : void {
        if (is_setop1(expr.op)) {
            note_write(expr.subexpr, true)
        }
    }

    def override preVisitExprCall(expr : ExprCall?) : void {
        // POST-infer a resolved call carries its mangled name (`__::<module>`<name>`<hash>`), so
        // the intrinsic table below must match the base name or every entry misses on that pass
        let cname = callee_base_name("{expr.name}")
        // GPU intrinsics that take a whole array and write (or only read) through it:
        // dasSpirv coopmat pair (+ the cm2 tensor-addressed forms) + the dasMetal simdgroup
        // pair store through arg 1; tg_store_float4(dst, idx, v) stores through arg 0
        if (cname == "coopmatStore" || cname == "simdgroup_store" || cname == "coopmatStoreTensor") {
            if (length(expr.arguments) >= 2) {
                note_write(expr.arguments[1], false)
            }
            return
        }
        if (cname == "tg_store_float4") {
            if (!empty(expr.arguments)) {
                note_write(expr.arguments[0], false)
            }
            return
        }
        if (cname == "coopmatLoad" || cname == "simdgroup_load"
                || cname == "coopmatLoadTensor" || cname == "coopmatLoadTensorDecode"
                || cname |> ends_with("tmm2d_tg_begin") || cname |> ends_with("tmm2d_tg_step")
                || cname |> ends_with("i_das_ptr_add")) {
            // plain reads (the generic pass records them) — or, for the staged-GEMM protocol
            // begin/step, only untracked locals/@workgroup tiles. Pointer arithmetic is neither:
            // the RESULT's use decides, and write_root_name peels back through it
            return
        }
        if (cname |> ends_with("tmm2d_tg_store")) {
            if (length(expr.arguments) == 5) {
                var cur = expr.arguments[1]
                if (cur is ExprRef2Value) {
                    cur = (cur as ExprRef2Value).subexpr
                }
                if (cur is ExprVar) {
                    let pn = string((cur as ExprVar).name)
                    let bn = ptrs?[pn] ?? (key_exists(tracked, pn) ? pn : "")
                    if (!empty(bn)) {
                        claimed |> insert("{pn}:{cur.at.line}:{cur.at.column}")
                        writes |> insert(bn)
                    }
                }
            }
            return
        }
        let tci = tmm2d_c_arg(cname)
        if (tci >= 0) {
            // pointer operands resolve through the ptr-map: C writes, the rest read
            for (i in range(3, length(expr.arguments))) {
                var cur = expr.arguments[i]
                if (cur is ExprRef2Value) {
                    cur = (cur as ExprRef2Value).subexpr
                }
                continue if (!(cur is ExprVar))
                let pn = string((cur as ExprVar).name)
                let bn = ptrs?[pn] ?? (key_exists(tracked, pn) ? pn : "")
                continue if (empty(bn))
                claimed |> insert("{pn}:{cur.at.line}:{cur.at.column}")
                if (i == tci) {
                    writes |> insert(bn)
                } else {
                    reads |> insert(bn)
                }
            }
            return
        }
        callees |> insert(cname)
        // A tracked buffer passed WHOLE takes its direction from the callee's parameter — that is
        // what the emitters lower it to — so it is banked for the driver to resolve. In field mode
        // `self` passed whole stays refused (a class-method helper could touch any field), and
        // field-mode analysis does NOT recurse into free callees (bare-name matching would collide
        // with their locals).
        for (aidx, a in range(length(expr.arguments)), expr.arguments) {
            bank_call_arg(a, cname, aidx)
        }
    }

    def bank_call_arg(a : Expression const?; cname : string; aidx : int) {
        var cur = a
        if (cur is ExprRef2Value) {
            cur = (cur as ExprRef2Value).subexpr
        }
        return if (bank_ptr_arg(cur, cname, aidx))
        var bname = ""
        if (cur is ExprVar && is_tracked((cur as ExprVar).name)) {
            bname = string((cur as ExprVar).name)
        } elif (fieldMode && cur is ExprField && is_self_field(cur as ExprField, tracked)) {
            bname = string((cur as ExprField).name)
        } elif (fieldMode && cur is ExprVar && (cur as ExprVar).name == "self") {
            err = "'self' passed whole to call '{cname}' — the classifier cannot see through method helpers; use access_force with declared roles"
        }
        return if (empty(bname))
        claimed |> insert("{bname}:{cur.at.line}:{cur.at.column}")
        whole |> emplace(WholeArg(buf = bname, callee = cname, idx = aidx))
    }

    // POST-infer a method call is an ExprInvoke, not an ExprCall — the kernel body's `run()`
    // never reaches preVisitExprCall. Record the method name so the driver follows it.
    def override preVisitExprInvoke(expr : ExprInvoke?) : void {
        if (!fieldMode || !expr.isInvokeMethod || empty(expr.arguments)) {
            return
        }
        var a0 = expr.arguments[0]
        if (a0 is ExprRef2Value) {
            a0 = (a0 as ExprRef2Value).subexpr
        }
        let ef = a0 ?as ExprField
        if (ef != null) {
            callees |> insert(string(ef.name))
        }
    }

    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        if (is_tracked(expr.name) && !key_exists(claimed, "{expr.name}:{expr.at.line}:{expr.at.column}")) {
            reads |> insert(string(expr.name))
            if (ACCESS_DEBUG) {
                print("[access-dbg] read {expr.name} at={int(expr.at.line)}:{int(expr.at.column)}\n")
            }
        } elif (key_exists(ptrs, string(expr.name)) && !key_exists(claimed, "{expr.name}:{expr.at.line}:{expr.at.column}")) {
            // unmodeled use of a buffer-pointer local — conservative read of its buffer
            reads |> insert(ptrs[string(expr.name)])
        }
        return expr
    }

    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        if (fieldMode && is_self_field(expr, tracked) && !key_exists(claimed, "{expr.name}:{expr.at.line}:{expr.at.column}")) {
            reads |> insert(string(expr.name))
            if (ACCESS_DEBUG) {
                print("[access-dbg] read {expr.name} at={int(expr.at.line)}:{int(expr.at.column)} (field)\n")
            }
        }
        return expr
    }
}

def private analyze_one(fn : Function?; mod : Module?; tracked : table<string>; fieldMode : bool; var res : AccessResult; var callees : table<string>) {
    var astVisitor = new AccessVisitor()
    astVisitor.tracked := tracked
    astVisitor.fieldMode = fieldMode
    make_visitor(*astVisitor) $(adapter) {
        visit(fn, adapter)
    }
    for (r in keys(astVisitor.reads)) {
        res.reads |> insert(r)
    }
    for (w in keys(astVisitor.writes)) {
        res.writes |> insert(w)
    }
    // buffers handed whole to a helper: the callee's parameter decides. A name that does not
    // resolve to exactly one declaration keeps the ratchet — it could be written through.
    for (wa in astVisitor.whole) {
        let dir = callee_param_dir(mod, wa.callee, wa.idx)
        if (empty(dir)) {
            let escape = fieldMode ? "declare the @role (access_force)" : "declare [vk_access]"
            astVisitor.err = "tracked buffer '{wa.buf}' passed whole to call '{wa.callee}' — teach the intrinsic table or {escape}"
        } elif (dir == "write") {
            res.writes |> insert(wa.buf)
        } else {
            res.reads |> insert(wa.buf)
        }
    }
    for (c in keys(astVisitor.callees)) {
        callees |> insert(c)
    }
    if (!empty(astVisitor.err) && empty(res.err)) {
        res.err = "{fn.name}: {astVisitor.err}"
    }
    unsafe {
        delete astVisitor
    }
}

// A kernel class's method, resolved the way the emitter resolves it: through the CONCRETE
// class's field init, so an override wins over the base it replaces. Returns null for a name
// that is not a method (a free call) or an abstract one.
def private class_method(mod : Module?; st : StructurePtr; mname : string) : Function? {
    if (st == null) {
        return null
    }
    // a method call is spelled bare pre-infer (`run`) and mangled after (`Class`run`), and this
    // classifier runs at both times — match the field against either
    var short = mname
    let tick = find(short, "`")
    if (tick >= 0) {
        short = slice(short, tick + 1)
    }
    for (fld in st.fields) {
        continue if (fld.name != mname && fld.name != short)
        var ie = fld.init
        while (ie != null) {
            if (ie is ExprRef2Value) {
                ie = (ie as ExprRef2Value).subexpr
            } elif (ie is ExprCast) {
                ie = (ie as ExprCast).subexpr
            } else {
                break
            }
        }
        // at structure-macro time the address is still unresolved — only the target NAME is
        // set (`_::Class`method`), so the lookup goes through the module by name
        let ea = ie ?as ExprAddr
        return null if (ea == null)
        var nm = string(ea.target)
        let sep = find(nm, "::")
        if (sep >= 0) {
            nm = slice(nm, sep + 2)
        }
        var found : Function?
        mod |> for_each_function(nm) $(cfn) {
            if (cfn.name == nm) {
                found = cfn
            }
        }
        return found
    }
    return null
}

def private classify_access(fn : Function?; mod : Module?; st : StructurePtr; tracked : table<string>; fieldMode : bool) : AccessResult {
    var res : AccessResult
    var pending : table<string>
    var visited : table<string>
    var callees : table<string>
    analyze_one(fn, mod, tracked, fieldMode, res, callees)
    visited |> insert(string(fn.name))
    // Field mode does not recurse into FREE callees — shader intrinsics have das shims whose
    // locals would false-positive on bare-name matching. It DOES recurse into the kernel class's
    // own METHODS: they touch members directly, so without this a method-shaped kernel body
    // derives an empty access set and the lens stages no hazards at all.
    for (c in keys(callees)) {
        if (!fieldMode || class_method(mod, st, c) != null) {
            pending |> insert(c)
        }
    }
    while (!empty(pending)) {
        var name = ""
        for (p in keys(pending)) {
            name = p
            break
        }
        pending |> erase(name)
        if (key_exists(visited, name)) {
            continue
        }
        visited |> insert(name)
        var next : table<string>
        let mfn = fieldMode ? class_method(mod, st, name) : null
        if (mfn != null) {
            analyze_one(mfn, mod, tracked, fieldMode, res, next)
        } else {
            mod |> for_each_function(name) $(cfn) {
                if (cfn.name == name) {
                    analyze_one(cfn, mod, tracked, fieldMode, res, next)
                }
            }
        }
        for (c in keys(next)) {
            if (!key_exists(visited, c) && (!fieldMode || class_method(mod, st, c) != null)) {
                pending |> insert(c)
            }
        }
        delete next
    }
    delete pending
    delete visited
    delete callees
    return <- res
}

//! Classify which `tracked` globals `fn` reads and writes, recursing into same-module callees
//! (helpers access the globals directly; passing a tracked global BY ARGUMENT is refused — see
//! AccessResult.err). Recursion is name-keyed over `mod`'s functions with a visited-set guard.
def classify_global_access(fn : Function?; mod : Module?; tracked : table<string>) : AccessResult {
    var none : StructurePtr
    return <- classify_access(fn, mod, none, tracked, false)
}

//! The field-mode twin for class-shaped kernels (metal): `tracked` names are the class's buffer
//! FIELDS — pre-infer bodies spell them as bare names (or explicit self.<name>); member access
//! on anything else (float4 components like wv.y) never matches. `st` is the kernel class, so a
//! body that delegates to its own METHODS is followed through them (an override resolves against
//! the concrete class, like the emitter does); free callees stay intra-body.
def classify_field_access(fn : Function?; mod : Module?; st : StructurePtr; tracked : table<string>) : AccessResult {
    return <- classify_access(fn, mod, st, tracked, true)
}
