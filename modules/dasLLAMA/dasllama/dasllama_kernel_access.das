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

struct AccessResult {
    reads : table<string>
    writes : table<string>
    err : string   // non-empty = unanalyzable construct; the lens turns it into a compile error
}

// write-root of an assignment target: peel subscripts/fields/swizzles down to the base variable
def private write_root(e : Expression?) : ExprVar? {
    var cur = e
    while (true) {
        if (cur is ExprAt) {
            cur = (cur as ExprAt).subexpr
        } elif (cur is ExprSafeAt) {
            cur = (cur as ExprSafeAt).subexpr
        } elif (cur is ExprField) {
            cur = (cur as ExprField).value
        } elif (cur is ExprSwizzle) {
            cur = (cur as ExprSwizzle).value
        } elif (cur is ExprRef2Value) {
            cur = (cur as ExprRef2Value).subexpr
        } else {
            break
        }
    }
    return cur is ExprVar ? cur as ExprVar : null
}

def private is_setop2(op : das_string) : bool {
    return (op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "%="
        || op == "&=" || op == "|=" || op == "^=" || op == "<<=" || op == ">>="
        || op == "&&=" || op == "||=" || op == "^^=")
}

def private is_setop1(op : das_string) : bool => op == "++" || op == "--" || op == "+++" || op == "---"

class private AccessVisitor : AstVisitor {
    tracked : table<string>
    reads : table<string>
    writes : table<string>
    callees : table<string>        // non-intrinsic call names, for the driver's same-module recursion
    claimed : table<uint64>        // ExprVar nodes already classified as pure-write targets
    err : string

    def is_tracked(name : das_string) : bool => key_exists(tracked, string(name))

    // record a write; `also_read` covers read-modify-write forms (+=, ++). A pure store claims
    // the target node so the generic ExprVar pass does not also count it as a read.
    def note_write(target : Expression?; also_read : bool) {
        var v = write_root(target)
        if (v == null || !is_tracked(v.name)) {
            return
        }
        writes |> insert(string(v.name))
        if (!also_read) {
            claimed |> insert(intptr(v))
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
        // dasSpirv intrinsics that take a whole array and write (or only read) through it
        if (cname == "coopmatStore") {
            if (length(expr.arguments) >= 2) {
                note_write(expr.arguments[1], false)
            }
            return
        }
        if (cname == "coopmatLoad") {
            return   // src argument is a plain read — the generic ExprVar pass records it
        }
        callees |> insert(cname)
        // the ratchet: a tracked global passed WHOLE to a call we do not model could be written
        // through a var parameter — refuse to guess
        for (a in expr.arguments) {
            var cur = a
            if (cur is ExprRef2Value) {
                cur = (cur as ExprRef2Value).subexpr
            }
            if (cur is ExprVar && is_tracked((cur as ExprVar).name)) {
                err = "tracked global '{(cur as ExprVar).name}' passed whole to call '{cname}' — teach the intrinsic table or declare [vk_access]"
            }
        }
    }

    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        if (is_tracked(expr.name) && !key_exists(claimed, intptr(expr))) {
            reads |> insert(string(expr.name))
        }
        return expr
    }
}

def private analyze_one(fn : Function?; tracked : table<string>; var res : AccessResult; var callees : table<string>) {
    var astVisitor = new AccessVisitor()
    astVisitor.tracked := tracked
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

//! Classify which `tracked` globals `fn` reads and writes, recursing into same-module callees
//! (helpers access the globals directly; passing a tracked global BY ARGUMENT is refused — see
//! AccessResult.err). Recursion is name-keyed over `mod`'s functions with a visited-set guard.
def classify_global_access(fn : Function?; mod : Module?; tracked : table<string>) : AccessResult {
    var res : AccessResult
    var pending : table<string>
    var visited : table<string>
    var callees : table<string>
    analyze_one(fn, tracked, res, callees)
    visited |> insert(string(fn.name))
    for (c in keys(callees)) {
        pending |> insert(c)
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
                analyze_one(cfn, tracked, res, next)
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
