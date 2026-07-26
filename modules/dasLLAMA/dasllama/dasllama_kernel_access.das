options gen2
options indenting = 4

module dasllama_kernel_access shared public

require daslib/ast
require daslib/ast_boost
require daslib/rtti

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

class private AccessVisitor : AstVisitor {
    tracked : table<string>
    fieldMode : bool               // tracked names are class FIELDS (metal): match self.<name>
                                   // ExprField accesses only (see is_self_field)
    reads : table<string>
    writes : table<string>
    callees : table<string>        // non-intrinsic call names, for the driver's same-module recursion
    claimed : table<string>        // pure-write target tokens, keyed name:line:column — infer
                                   // CLONES subtrees during rewrites, so pointer identity misses
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
                || cname == "coopmatLoadTensor" || cname == "coopmatLoadTensorDecode") {
            return   // src argument is a plain read — the generic read pass records it
        }
        callees |> insert(cname)
        // the ratchet: a tracked buffer passed WHOLE to a call we do not model could be written
        // through a var parameter — refuse to guess. In field mode `self` passed whole is the
        // same hole (a class-method helper could touch any field), and field-mode analysis does
        // NOT recurse into free callees (bare-name matching would collide with their locals).
        for (a in expr.arguments) {
            var cur = a
            if (cur is ExprRef2Value) {
                cur = (cur as ExprRef2Value).subexpr
            }
            let escape = fieldMode ? "declare the @role (access_force)" : "declare [vk_access]"
            if (cur is ExprVar && is_tracked((cur as ExprVar).name)) {
                err = "tracked buffer '{(cur as ExprVar).name}' passed whole to call '{cname}' — teach the intrinsic table or {escape}"
            } elif (fieldMode && cur is ExprField && is_self_field(cur as ExprField, tracked)) {
                err = "tracked buffer '{(cur as ExprField).name}' passed whole to call '{cname}' — teach the intrinsic table or {escape}"
            } elif (fieldMode && cur is ExprVar && (cur as ExprVar).name == "self") {
                err = "'self' passed whole to call '{cname}' — the classifier cannot see through method helpers; use access_force with declared roles"
            }
        }
    }

    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        if (is_tracked(expr.name) && !key_exists(claimed, "{expr.name}:{expr.at.line}:{expr.at.column}")) {
            reads |> insert(string(expr.name))
            if (ACCESS_DEBUG) {
                print("[access-dbg] read {expr.name} at={int(expr.at.line)}:{int(expr.at.column)}\n")
            }
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

def private analyze_one(fn : Function?; tracked : table<string>; fieldMode : bool; var res : AccessResult; var callees : table<string>) {
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
    analyze_one(fn, tracked, fieldMode, res, callees)
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
                analyze_one(cfn, tracked, fieldMode, res, next)
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
