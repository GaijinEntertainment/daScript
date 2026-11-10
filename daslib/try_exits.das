options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module try_exits shared public

require daslib/ast_boost

//! the exits a `try` or `recover` arm takes past its own end: a
//! `break` or `continue` aimed at a loop around the try, a `return`
//! of the enclosing function
struct public TryExits {
    hasBreak : bool
    hasContinue : bool
    hasReturn : bool
}

//! true when any exit leaves the arm
def public any_exit(exits : TryExits) => exits.hasBreak || exits.hasContinue || exits.hasReturn

class TryExitScanner : AstVisitor {
    found : TryExits
    loopDepth : int = 0

    def override preVisitExprForBody(ffor : ExprFor?) {
        loopDepth++
    }
    def override visitExprFor(var ffor : ExprFor?) : ExpressionPtr {
        loopDepth--
        return ffor
    }
    def override preVisitExprWhileBody(wh : ExprWhile?; body : ExpressionPtr) {
        loopDepth++
    }
    def override visitExprWhile(var wh : ExprWhile?) : ExpressionPtr {
        loopDepth--
        return wh
    }
    def override preVisitExprBreak(that : ExprBreak?) {
        found.hasBreak ||= loopDepth == 0
    }
    def override preVisitExprContinue(that : ExprContinue?) {
        found.hasContinue ||= loopDepth == 0
    }
    def override preVisitExprReturn(expr : ExprReturn?) {
        found.hasReturn ||= expr.returnFunc != null
    }
}

//! the exits `body` takes past its own end; a `break` or `continue`
//! inside a loop the body itself holds stays the loop's, a `return`
//! counts only when it returns the function
def public scan_exits(body : ExpressionPtr) : TryExits {
    var scanner = new TryExitScanner()
    make_visitor(*scanner) $(adp) {
        body |> visit(adp)
    }
    let found = scanner.found
    unsafe {
        delete scanner
    }
    return found
}

//! the exits either arm of `tc` takes past its own end
def public scan_try_exits(tc : ExprTryCatch?) : TryExits {
    let a = scan_exits(tc.try_block)
    let b = scan_exits(tc.catch_block)
    return TryExits(hasBreak = a.hasBreak || b.hasBreak,
        hasContinue = a.hasContinue || b.hasContinue,
        hasReturn = a.hasReturn || b.hasReturn)
}
