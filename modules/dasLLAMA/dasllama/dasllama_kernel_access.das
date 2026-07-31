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

def private write_root_name(e : Expression?; tracked : table<string>; fieldMode : bool; var node : Expression const?&) : string {
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

// Direction of the `idx`-th parameter of the same-module helper named `cname`: "write" when the
// parameter is `var` (the callee may store through it), "read" otherwise — the same rule the MSL
// and SPIR-V emitters lower to `device` vs `device const`. Returns "" when the name does not
// resolve to exactly one declaration (overloads included), which keeps the ratchet's refusal.
def private scan_param_dir(cfn : Function?; cname : string; idx : int; var dir : string&; var nfound : int&) {
    return if (cfn.name != cname || idx >= length(cfn.arguments))
    nfound++
    let pt = cfn.arguments[idx]._type
    dir = pt != null && !pt.flags.constant ? "write" : "read"
}

def private callee_param_dir(mod : Module?; cname : string; idx : int) : string {
    var dir = ""
    var nfound = 0
    mod |> for_each_function(cname) $(cfn) {
        scan_param_dir(cfn, cname, idx, dir, nfound)
    }
    mod |> for_each_generic(cname) $(cfn) {
        scan_param_dir(cfn, cname, idx, dir, nfound)
    }
    return nfound == 1 ? dir : ""
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
        let cname = string(expr.name)
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
                || cname |> ends_with("tmm2d_tg_begin") || cname |> ends_with("tmm2d_tg_step")) {
            // plain reads (the generic pass records them) — or, for the staged-GEMM protocol
            // begin/step, only untracked locals/@workgroup tiles
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
            var cur = a
            if (cur is ExprRef2Value) {
                cur = (cur as ExprRef2Value).subexpr
            }
            var bname = ""
            if (cur is ExprVar && is_tracked((cur as ExprVar).name)) {
                bname = string((cur as ExprVar).name)
            } elif (fieldMode && cur is ExprField && is_self_field(cur as ExprField, tracked)) {
                bname = string((cur as ExprField).name)
            } elif (fieldMode && cur is ExprVar && (cur as ExprVar).name == "self") {
                err = "'self' passed whole to call '{cname}' — the classifier cannot see through method helpers; use access_force with declared roles"
            }
            continue if (empty(bname))
            claimed |> insert("{bname}:{cur.at.line}:{cur.at.column}")
            whole |> emplace(WholeArg(buf = bname, callee = cname, idx = aidx))
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

def private classify_access(fn : Function?; mod : Module?; tracked : table<string>; fieldMode : bool) : AccessResult {
    var res : AccessResult
    var pending : table<string>
    var visited : table<string>
    var callees : table<string>
    analyze_one(fn, mod, tracked, fieldMode, res, callees)
    visited |> insert(string(fn.name))
    // field mode is intra-body only: free callees are shader intrinsics whose das shims would
    // false-positive on bare-name matching, and self-helpers hit the ratchet instead
    for (c in keys(callees)) {
        if (!fieldMode) {
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
        mod |> for_each_function(name) $(cfn) {
            if (cfn.name == name) {
                analyze_one(cfn, mod, tracked, fieldMode, res, next)
            }
        }
        for (c in keys(next)) {
            if (!key_exists(visited, c)) {
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
    return <- classify_access(fn, mod, tracked, false)
}

//! The field-mode twin for class-shaped kernels (metal): `tracked` names are the class's buffer
//! FIELDS — pre-infer bodies spell them as bare names (or explicit self.<name>); member access
//! on anything else (float4 components like wv.y) never matches.
def classify_field_access(fn : Function?; mod : Module?; tracked : table<string>) : AccessResult {
    return <- classify_access(fn, mod, tracked, true)
}
