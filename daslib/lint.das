options gen2
options indenting = 4
options strict_smart_pointers = true

module lint shared private

//! Lint pass that enables paranoid compilation checks.
//!
//! When this module is required, the ``paranoid`` lint flag is turned on
//! for the current module, enabling stricter compiler diagnostics.
//! See also ``lint_everything`` for applying lint to all modules.

require daslib/strings_boost
require daslib/ast_boost
require daslib/strings_boost
require daslib/lint_config
require daslib/lint_fixups
require math

[lint_macro]
class LintEverything : AstPassMacro {
    //! Pass macro that enables lint checking for modules.
    def override apply(prog : ProgramPtr; _mod : Module?) : bool {
        let disabled <- build_lint_macro_disabled(prog)
        paranoid(prog, true, disabled)
        return true
    }
}

struct private DeadStoreEntry {
    @do_not_delete v : Variable const?
    @do_not_delete pending_expr : Expression const?
    skip : bool
    has_pending : bool
    pending_pure : bool
}

enum private Lint021CallKind {
    plain
    narrowing_cast
    poison
}

struct private Lint021Candidate {
    @do_not_delete v : Variable const?
    safe_uses : int
    disqualified : bool
}

def private is_block_terminator(expr : ExpressionPtr) : bool {
    if (expr is ExprReturn) return true
    if (expr is ExprCall) {
        let call = expr as ExprCall
        if (call.func == null) return false
        return call.name |> eq <| "panic" && call.func._module.name |> eq <| "builtin"
    }
    return false
}

//! the memory builtins that write through their first argument
let private lint031_builtin_writers <- { "memcpy", "memmove", "memset8", "memset16", "memset32", "memset64", "memset128" }

struct private RefIndex {
    @do_not_delete functions : table<Function const?>
    names : table<string>       //!< references knowable by name only (uninferred generic bodies)
    @do_not_delete structs : table<Structure?>
    @do_not_delete enums : table<Enumeration?>
}

def private unqualified(raw_name : string) : string {
    let sep = raw_name |> rfind("::")
    return sep >= 0 ? raw_name |> slice(sep + 2) : raw_name
}

def private note_ref(var self : RefIndex; fn : Function?; raw_name : string) : void {
    if (fn != null) {
        self.functions |> insert(fn)
        return
    }
    let nm = unqualified(raw_name)
    if (!empty(nm)) {
        self.names |> insert(nm)
    }
}

def private knows(self : RefIndex; fn : Function?; nm : string) : bool =>
    key_exists(self.functions, fn) || key_exists(self.names, nm)

def private note_type(var self : RefIndex; t : TypeDecl?; depth : int; skip : Structure const? = null) : void {
    return if (t == null || depth > 24)
    let st : Structure const? = t.structType
    if (st != null && st != skip) {
        self.structs |> insert(t.structType)
    }
    if (t.enumType != null) {
        self.enums |> insert(t.enumType)
    }
    self |> note_type(t.firstType, depth + 1, skip)
    self |> note_type(t.secondType, depth + 1, skip)
    for (a in t.argTypes) {
        self |> note_type(a, depth + 1, skip)
    }
}

def private note_type_names(var self : RefIndex; t : TypeDecl?; depth : int) : void {
    return if (t == null || depth > 24)
    let alias = "{t.alias}"
    if (!empty(alias)) {
        self.names |> insert(alias)
    }
    if (t.structType != null) {
        self.names |> insert("{t.structType.name}")
    }
    if (t.enumType != null) {
        self.names |> insert("{t.enumType.name}")
    }
    self |> note_type_names(t.firstType, depth + 1)
    self |> note_type_names(t.secondType, depth + 1)
    for (a in t.argTypes) {
        self |> note_type_names(a, depth + 1)
    }
}

def private lint022_calls_may_be_inlined(prog : ProgramPtr) : bool {
    return (prog.getOptimize
        && !(prog._options |> find_arg("disable_auto_inline") ?as tBool ?? prog.policies.disable_auto_inline)
        && (prog._options |> find_arg("auto_inline_functions") ?as tBool ?? prog.policies.auto_inline_functions))
}

def private lint022_block_calls_may_be_inlined(prog : ProgramPtr) : bool {
    return (prog.getOptimize
        && !(prog._options |> find_arg("disable_auto_inline") ?as tBool ?? prog.policies.disable_auto_inline))
}

def private lint022_takes_block(fn : Function?) : bool {
    for (a in fn.arguments) {
        return true if (a._type != null && a._type.baseType == Type.tBlock)
    }
    return false
}

def private lint022_enabled(prog : ProgramPtr) : bool {
    let configured = prog._options |> find_arg("_unused_private")
    if (configured is tBool) return configured as tBool
    if (lint_config_forces_on("LINT022")) return true
    return is_core_library_source(prog)
}

def private lint022_is_skipped(fn : Function?; mod : Module?) : bool {
    return (fn == null || !fn.flags.privateFunction
        || fn.flags.generated || fn.flags.isClassMethod
        || fn.fromGeneric != null || fn.isGeneric || fn.moreFlags.isTemplate
        || length(fn.annotations) > 0
        || fn.at.fileInfo == null || fn._module != mod)
}

//! a read of a LINT004 candidate and its rename, applied with its
//! declaration's
struct private Lint004Use {
    @do_not_delete v : Variable const?
    fix : LintFix
}

struct private Lint023Candidate {
    @do_not_delete v : Variable const?
    stores : int
    disqualified : bool
}

//! a variable read somewhere inside argument `slot` of the call a frame tracks
struct private Lint030Mention {
    slot : int
    @do_not_delete v : Variable const?
    //! inside a block the call runs: counts only for the outer argument
    in_block : bool
}

//! a variable a call somewhere inside argument `slot` writes through a mutable by-reference
//! parameter; `callee` names that call for the finding
struct private Lint030Write {
    slot : int
    @do_not_delete v : Variable const?
    callee : string
    //! inside a block the call runs: counts only for the outer argument
    in_block : bool
}

//! one call whose argument list the walk is inside: `slot` is the argument being walked,
//! `closure_depth` the depth the call sits at, so a read inside a lambda argument - run later,
//! not at the call - is not a mention
struct private Lint030Frame {
    @do_not_delete call : ExprCall const?
    slot : int
    closure_depth : int
    mentions : array<Lint030Mention>
    writes : array<Lint030Write>
}

//! `lint030_callee_writes`' memo, `<mangled name hash>#<k>` -> whether that function writes its
//! k-th parameter - a generic's instances key apart, since `static_if` can make one write and
//! another not; a key sits at `true` while its scan runs, so a recursive chain reads as writing
var private lint030_writers : table<string; bool>

def private lint030_bare_var(e : Expression?) : Variable const? =>
    e is ExprVar ? (e as ExprVar).variable : null

//! the variable a chain of plain field accesses is rooted at (`dr.offs`, `s.k.v`), the bare
//! variable itself, or null for any other shape; a field reached through a pointer is a value
//! a write can move, so the chain stops there
def private lint030_field_root(e : Expression?) : Variable const? {
    return lint030_bare_var(e) if (!(e is ExprField))
    let fld = e as ExprField
    return null if (fld.value == null || fld.value._type == null || fld.value._type.baseType == Type.tPointer)
    return lint030_field_root(fld.value)
}

//! one call the write scan is inside: `slot` is the argument being walked
struct private Lint030ArgFrame {
    @do_not_delete call : ExprCall const?
    slot : int
}

//! an operator that assigns to its left operand: `=` is an ExprCopy, these are the compound forms
def private lint030_assigns(op : string) : bool =>
    op |> ends_with("=") && op != "==" && op != "!=" && op != "<=" && op != ">="

def private lint030_steps(op : string) : bool =>
    op == "++" || op == "--" || op == "+++" || op == "---"

//! scans one function body for a write to `param` during the call; which compiler write marks
//! count and which are discounted is `daslib/ARCHITECTURE_LINT.md` sec.3's LINT030 entry
class private Lint030WriteScan : AstVisitor {
    @do_not_delete param : Variable const?
    writes : bool = false
    ret_depth : int = 0
    addr_depth : int = 0
    lhs_depth : int = 0
    hop_depth : int = 0
    hops : array<bool>
    args : array<Lint030ArgFrame>
    def hop_in(through_pointer : bool) : void {
        hops |> push(through_pointer)
        if (through_pointer) {
            hop_depth++
        }
    }
    def hop_out() : void {
        if (hops |> back()) {
            hop_depth--
        }
        hops |> pop()
    }
    def override preVisitExprPtr2Ref(expr : ExprPtr2Ref?) : void {
        let bare : Variable const? = lint030_bare_var(expr.subexpr)
        hop_in(bare != param)
    }
    def override visitExprPtr2Ref(var expr : ExprPtr2Ref?) : ExpressionPtr {
        hop_out()
        return expr
    }
    def override preVisitExprField(expr : ExprField?) : void {
        let through = expr.value != null && expr.value._type != null && expr.value._type.baseType == Type.tPointer
        let pointer_read = lhs_depth == 0 && expr._type != null && expr._type.baseType == Type.tPointer
        hop_in(through || pointer_read)
    }
    def override preVisitExprCopy(expr : ExprCopy?) : void {
        lhs_depth++
    }
    def override preVisitExprCopyRight(expr : ExprCopy?; right : ExpressionPtr) : void {
        lhs_depth--
    }
    def override preVisitExprMove(expr : ExprMove?) : void {
        lhs_depth++
    }
    def override preVisitExprMoveRight(expr : ExprMove?; right : ExpressionPtr) : void {
        lhs_depth--
    }
    def override preVisitExprClone(expr : ExprClone?) : void {
        lhs_depth++
    }
    def override preVisitExprCloneRight(expr : ExprClone?; right : ExpressionPtr) : void {
        lhs_depth--
    }
    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (lint030_assigns(string(expr.op))) {
            lhs_depth++
        }
    }
    def override preVisitExprOp2Right(expr : ExprOp2?; right : ExpressionPtr) : void {
        if (lint030_assigns(string(expr.op))) {
            lhs_depth--
        }
    }
    def override preVisitExprOp1(expr : ExprOp1?) : void {
        if (lint030_steps(string(expr.op))) {
            lhs_depth++
        }
    }
    def override visitExprOp1(var expr : ExprOp1?) : ExpressionPtr {
        if (lint030_steps(string(expr.op))) {
            lhs_depth--
        }
        return expr
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        hop_out()
        return expr
    }
    def override preVisitExprSafeField(expr : ExprSafeField?) : void {
        hop_in(true)
    }
    def override visitExprSafeField(var expr : ExprSafeField?) : ExpressionPtr {
        hop_out()
        return expr
    }
    def override preVisitExprCall(expr : ExprCall?) : void {
        args |> push(Lint030ArgFrame(call = expr, slot = -1))
    }
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        args |> pop()
        return expr
    }
    def override preVisitExprCallArgument(_expr : ExprCall?; _arg : ExpressionPtr; _last : bool) : void {
        if (!empty(args)) {
            args[length(args) - 1].slot++
        }
    }
    def override preVisitExprReturn(expr : ExprReturn?) : void {
        ret_depth++
    }
    def override visitExprReturn(var expr : ExprReturn?) : ExpressionPtr {
        ret_depth--
        return expr
    }
    def override preVisitExprRef2Ptr(expr : ExprRef2Ptr?) : void {
        if (ret_depth > 0) {
            addr_depth++
        }
    }
    def override visitExprRef2Ptr(var expr : ExprRef2Ptr?) : ExpressionPtr {
        if (ret_depth > 0) {
            addr_depth--
        }
        return expr
    }
    def override preVisitExprVar(expr : ExprVar?) : void {
        let v : Variable const? = expr.variable
        return if (v != param || !expr.varFlags.write || addr_depth != 0 || hop_depth != 0 || writes)
        if (!empty(args) && args[length(args) - 1].slot >= 0) {
            let top = args[length(args) - 1]
            writes = lint030_callee_writes(top.call.func, top.slot)
        } else {
            writes = true
        }
    }
}

//! whether `fn` writes its `k`-th parameter during the call (`Lint030WriteScan`): a by-value
//! parameter's writes never reach the caller, and a function with no body to read - a builtin,
//! an extern - is taken as writing
def private lint030_callee_writes(fn : Function const?; k : int) : bool {
    return true if (fn == null || k >= length(fn.arguments))
    let p = fn.arguments[k]
    return false if (p._type == null || p._type.flags.constant || !(p._type.flags.ref || p._type.isRefType))
    return true if (fn.body == null)
    let key = "{fn.getMangledNameHash}#{k}"
    if (key_exists(lint030_writers, key)) {
        return lint030_writers?[key] ?? true
    }
    lint030_writers[key] = true
    var scan = new Lint030WriteScan(param = p)
    make_visitor(*scan) $(adapter) {
        visit(fn.body, adapter)
    }
    let writes = scan.writes
    unsafe {
        delete scan
    }
    lint030_writers[key] = writes
    return writes
}

var private lint031_writers : table<string; bool>

//! The operand a node carries a value of unchanged: a cast, a reference-to-value or
//! value-to-pointer step, an intptr round trip, pointer arithmetic (a call whose result aliases
//! its first argument's pointee); null for any other node.
def private lint031_carrier(expr : Expression const?) : Expression const? {
    if (expr is ExprCast) return (expr as ExprCast).subexpr
    if (expr is ExprRef2Ptr) return (expr as ExprRef2Ptr).subexpr
    if (expr is ExprRef2Value) return (expr as ExprRef2Value).subexpr
    if (expr is ExprCall) {
        let call = expr as ExprCall
        if (lint031_is_intptr(call) || (call.func != null && call.func.flags.firstArgReturnType && !empty(call.arguments))) {
            return call.arguments[0]
        }
    }
    return null
}

def private lint031_root_name(fn : Function const?) : string => string(root_generic(fn).name)

def private lint031_is_intptr(call : ExprCall const?) : bool {
    if (call.func == null || length(call.arguments) != 1) return false
    return lint031_root_name(call.func) == "intptr"
}

def private lint031_is_integer(t : TypeDecl const?) : bool {
    let bt = t.baseType
    return (bt == Type.tInt || bt == Type.tUInt || bt == Type.tInt64 || bt == Type.tUInt64
        || bt == Type.tInt8 || bt == Type.tUInt8 || bt == Type.tInt16 || bt == Type.tUInt16)
}

//! The operand a node reaches INTO - a deref, a field, an index, a swizzle - so a write to the
//! node is a write through the operand; null otherwise. Below the target, a node that yields a
//! pointer, or a field or index that yields an integer, is a load of an address: not the operand's memory.
def private lint031_pointee_of(expr : Expression const?; is_target : bool) : Expression const? {
    let loaded_pointer = !is_target && expr._type != null && expr._type.isPointer
    if (expr is ExprPtr2Ref) {
        if (loaded_pointer) return null
        return (expr as ExprPtr2Ref).subexpr
    }
    if (loaded_pointer || (!is_target && expr._type != null && lint031_is_integer(expr._type))) return null
    if (expr is ExprField) return (expr as ExprField).value
    if (expr is ExprAt) return (expr as ExprAt).subexpr
    if (expr is ExprSwizzle) return (expr as ExprSwizzle).value
    return null
}

//! The parameter in `params` a write target is reached from; null when the target roots elsewhere.
//! `through`: the write lands in what the target holds, so a local pointer copy is followed to
//! its initializer - reassigning the copy itself writes nothing the parameter owns.
def private lint031_root(params : array<Variable const?>; target : Expression const?; through : bool) : Variable const? {
    var expr = target
    var into = through
    var is_target = true
    while (expr != null) {
        let first = is_target
        is_target = false
        if (expr is ExprVar) {
            let v = (expr as ExprVar).variable
            if (v == null) return null
            if (params |> find_index(v) >= 0) return into ? v : null
            if (v.init == null || v._type == null) return null
            let via_intptr = v.init is ExprCall && lint031_is_intptr(v.init as ExprCall)
            if (!(v._type.flags.ref || via_intptr || v._type.isPointer)) return null
            expr = v.init
            continue
        }
        let carried = lint031_carrier(expr)
        if (carried != null) {
            expr = carried
            continue
        }
        expr = lint031_pointee_of(expr, first)
        into = true
    }
    return null
}

//! scans one function body for a write through `params` the LINT031 way - the write sites and
//! the chase the rule applies to a const parameter, so a callee is judged as its caller would be
class private Lint031BodyScan : AstVisitor {
    @do_not_delete params : array<Variable const?>
    writes : bool = false
    def note(target : Expression const?; through : bool) : void {
        if (!writes && lint031_root(params, target, through) != null) {
            writes = true
        }
    }
    def override preVisitExprCopy(expr : ExprCopy?) : void {
        note(expr.left, false)
    }
    def override preVisitExprClone(expr : ExprClone?) : void {
        note(expr.left, false)
    }
    def override preVisitExprMove(expr : ExprMove?) : void {
        note(expr.left, false)
    }
    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (lint030_assigns(string(expr.op))) {
            note(expr.left, false)
        }
    }
    def override preVisitExprOp1(expr : ExprOp1?) : void {
        if (lint030_steps(string(expr.op))) {
            note(expr.subexpr, false)
        }
    }
    def override preVisitExprCallArgument(expr : ExprCall?; arg : ExpressionPtr; _last : bool) : void {
        if (lint031_slot_written(expr, arg)) {
            note(arg, true)
        }
    }
}

//! whether `fn` writes through its `k`-th parameter: a builtin answers by name (both pointers of a
//! memory builtin register as `void *`, so the type says nothing), a das function by its body, read
//! once per (function, slot); a slot still being scanned - recursion - reads as not writing
def private lint031_callee_writes(fn : Function const?; k : int) : bool {
    return false if (fn == null || k >= length(fn.arguments))
    if (fn.flags.builtIn || fn.body == null) {
        return k == 0 && lint031_builtin_writers |> key_exists(lint031_root_name(fn))
    }
    let pt = fn.arguments[k]._type
    return false if (pt == null || pt.flags.constant || !(pt.isRef || pt.isPointer)
        || (pt.isPointer && pt.firstType != null && pt.firstType.flags.constant))
    let key = "{fn.getMangledNameHash}#{k}"
    if (key_exists(lint031_writers, key)) {
        return lint031_writers?[key] ?? false
    }
    lint031_writers[key] = false
    var scan = new Lint031BodyScan()
    scan.params |> push(fn.arguments[k])
    make_visitor(*scan) $(adapter) {
        visit(fn.body, adapter)
    }
    let writes = scan.writes
    unsafe {
        delete scan
    }
    lint031_writers[key] = writes
    return writes
}

//! `call` writes through what `arg` addresses - `arg` fills a slot whose callee writes through it
def private lint031_slot_written(call : ExprCall const?; arg : ExpressionPtr) : bool {
    return false if (call.func == null)
    for (a, i in call.arguments, count()) {
        if (a == arg) return lint031_callee_writes(call.func, i)
    }
    return false
}

//! The edit for the `var` keyword just before the name at ``at``: ``drop`` deletes it (a parameter), otherwise it becomes `let`; none when the source has no such keyword
def private lint_var_keyword_fix(at : LineInfo; drop : bool) : LintFix {
    let ln = int(at.line)
    let col = int(at.column)
    let head = slice(lint_source_line(at, ln), 0, col)
    let kw = strip_right(head)
    let kw_col = length(kw) - 3
    if (kw_col < 0 || length(head) == length(kw) || !ends_with(kw, "var")) return LintFix()
    let before = kw_col > 0 ? slice(kw, kw_col - 1, kw_col) : " "
    if (before != " " && before != "(" && before != ";") return LintFix()
    return drop ? LintFix(line = ln, column = kw_col, last_line = ln, last_column = col, replacement = "") : LintFix(line = ln, column = kw_col, last_line = ln, last_column = kw_col + 3, replacement = "let")
}

//! A slot that needs a mutable value: a non-const pointer, a pointer to non-const, or a non-const ref or ref-type
def private lint_mutable_slot(t : TypeDecl const?) : bool {
    if (t == null) return false
    if (t.isPointer) return !t.flags.constant || t.firstType == null || !t.firstType.flags.constant
    return !t.flags.constant && (t.isRef || t.isRefType)
}

//! The value a field, index, dereference, cast, variant or property access reads from; null for any other node
def private lint_value_source(e : Expression const?) : Expression const? {
    let inner = lint031_pointee_of(e, true)
    return inner if (inner != null)
    let carried = lint031_carrier(e)
    return carried if (carried != null)
    if (e is ExprSafeField) return (e as ExprSafeField).value
    if (e is ExprSafeAt) return (e as ExprSafeAt).subexpr
    if (e is ExprAsVariant) return (e as ExprAsVariant).value
    if (e is ExprSafeAsVariant) return (e as ExprSafeAsVariant).value
    return null if (!(e is ExprCall))
    let call = e as ExprCall
    let prop = call.func != null && (call.func.moreFlags.propertyFunction || call.func.moreFlags.isCustomProperty)
    return prop && length(call.arguments) > 0 ? call.arguments[0] : null
}

def private lint_start(at : LineInfo) : int2 => int2(int(at.line), int(at.column))

//! The source text of each argument ``call`` spells, hidden and
//! filled-in defaults skipped; empty when one has no text
def private lint_written_args(call : ExprCall?) : array<string> {
    var texts : array<string>
    if (call.func == null) return <- texts
    for (arg, k in call.arguments, count()) {
        continue if (arg.genFlags.generated)
        break if (lint_unwritten_arg(call, k))
        let text = lint_expr_text(arg)
        if (empty(text)) {
            texts |> clear()
            return <- texts
        }
        texts |> push(text)
    }
    return <- texts
}

//! Deletes the unreachable statement ``e`` when it fills whole lines
//! and holds no comment
def private lint001_fix(e : Expression?) : LintFix {
    if (!lint_fills_lines(e, false) || lint_has_comment(lint_expr_text(e))) return LintFix()
    return LintFix(line = int(e.at.line), column = 0, last_line = int(e.at.last_line) + 1,
                   last_column = 0, replacement = "")
}

//! The shared branch of ``c ? x : x``, when evaluating ``c`` has no
//! effect to keep
def private lint008_fix(expr : ExprOp3?) : LintFix {
    if (expr.subexpr == null || !expr.subexpr.flags.noSideEffects) return LintFix()
    return lint_replace_fix(expr, lint_expr_text(expr.left))
}

//! The then-branch replacing an ``if (c) {A} else {A}`` statement on
//! its own lines when ``c`` is pure - its statements dedented in
//! place, or the bare block when one declares
def private lint009_fix(expr : ExprIfThenElse?) : LintFix {
    if (expr.cond == null || !expr.cond.flags.noSideEffects
        || !(expr.if_true is ExprBlock)) return LintFix()
    let blk = expr.if_true as ExprBlock
    if (!lint_fills_lines(expr, false) || empty(blk.list)
        || lint_has_comment(lint_expr_text(expr))) return LintFix()
    var declares = false
    for (s in blk.list) {
        declares ||= s is ExprLet
    }
    let first_e = declares ? expr.if_true : blk.list[0]
    let last_e = declares ? expr.if_true : blk.list |> back()
    if (first_e.genFlags.generated || last_e.genFlags.generated) return LintFix()
    var head = expr.at
    head.last_line = first_e.at.line
    head.last_column = first_e.at.column
    var tail = expr.at
    tail.line = last_e.at.last_line
    tail.column = last_e.at.last_column
    var edits <- [lint_fix(head, ""), lint_fix(tail, "")]
    let indent = int(expr.at.column)
    let body_indent = int(first_e.at.column)
    if (!declares && first_e.at.line != expr.at.line) {
        for (ln in range(int(first_e.at.line) + 1, int(last_e.at.last_line) + 1)) {
            let line = lint_source_line(expr.at, ln)
            continue if (empty(strip(line)))
            return LintFix() if (lint_indent_of(line) < body_indent)
            edits |> push(
                          LintFix(line = ln, column = indent, last_line = ln,
                                  last_column = body_indent, replacement = ""))
        }
    } elif (!declares && last_e.at.last_line != expr.at.line) {
        return LintFix()
    }
    return lint_merge_fixes(expr.at, edits)
}

//! ``widen(arg)`` around the 64-bit operand of a narrowing cast, or
//! the operand alone when ``widen`` is empty
def private lint018_fix(narrow : ExprCall?; widen : string) : LintFix {
    let inner = lint_expr_text(narrow.arguments[0])
    return lint_replace_fix(narrow, empty(widen) || empty(inner) ? inner : "{widen}({inner})")
}

//! ``twin(a, b)`` for ``call(narrow(a), narrow(b))`` when every
//! argument narrows a ``wide`` value
def private lint020_fix(call : ExprCall?; twin, narrow : string; wide : Type) : LintFix {
    var args : array<string>
    for (a in call.arguments) {
        return LintFix() if (!(a is ExprCall))
        let c = a as ExprCall
        return LintFix() if (c.name != narrow || length(c.arguments) != 1
                             || c.arguments[0]._type == null
            || c.arguments[0]._type.baseType != wide)
        let text = lint_expr_text(c.arguments[0])
        return LintFix() if (empty(text))
        args |> push(text)
    }
    return lint_call_fix(call, twin, args)
}

//! The last line of a structure or enumeration declared at ``at`` -
//! the ``}`` right below the last of its members, which end on
//! ``member_lines``; 0 when that line is anything else
def private lint022_struct_close(at : LineInfo; member_lines : array<int>) : int {
    var last = 0
    for (ln in member_lines) {
        last = max(last, ln)
    }
    return last > 0 && strip(lint_source_line(at, last + 1)) == "\}" ? last + 1 : 0
}

//! Deletes the unused declaration from the ``keyword`` line at ``at``
//! through line ``close`` with its ``//!`` lines and one surplus
//! blank line; none under an annotation or plain comment
def private lint022_fix(at : LineInfo; keyword : string; close : int) : LintFix {
    let first = int(at.line)
    let head = lint_source_line(at, first)
    if (close < first || !starts_with(strip_left(head), "{keyword} ")
        || lint_has_comment(head)) return LintFix()
    var top = first
    while (top > 1 && starts_with(strip(lint_source_line(at, top - 1)), "//!")) {
        top--
    }
    let above = top > 1 ? strip(lint_source_line(at, top - 1)) : ""
    if (starts_with(above, "//") || starts_with(above, "[")) return LintFix()
    var next_exists = false
    get_file_source_line(at.fileInfo, uint(close + 1)) $(_line) {
        next_exists = true
    }
    let drop_blank = empty(above) && next_exists && empty(strip(lint_source_line(at, close + 1)))
    return LintFix(line = top, column = 0, last_line = close + (drop_blank ? 2 : 1),
                   last_column = 0, replacement = "")
}

//! a marked container that outlives the call: `root` is the
//! variable it hangs off; on a field annotation, `field` is the
//! field's `.name` and `owner` the structure that declares it
struct private Lint034Container {
    key : string
    field : string
    @do_not_delete root : Variable const?
    @do_not_delete owner : Structure const?
}

//! one store into a container (`fills`) or one emptying of it;
//! `slot` is the written element's text when the event touches it
//! alone, `filled_by` the callee a store came through
struct private Lint034Event {
    container : Lint034Container
    fills : bool
    slot : string
    filled_by : string
    @do_not_delete site : Expression const?
}

//! what holds on one path through a function: the stores still in
//! place, and the containers emptied and not filled since; `reached`
//! is false once the path has left by return, break or continue
struct private Lint034State {
    reached : bool
    filled : array<Lint034Event>
    emptied : array<Lint034Container>
}

struct private Lint034Flow {
    now : Lint034State
    loop_jumps : Lint034State
    exits : Lint034State
    stale : array<Lint034Event>
    @do_not_delete finals : array<ExprBlock?>
}

//! what a call does to the `@scratch` containers that outlive it:
//! `stale` are the stores an exit leaves in place, `emptied` the
//! containers every exit leaves empty
struct private Lint034Summary {
    stale : array<Lint034Event>
    emptied : array<Lint034Container>
}

var private lint034_summary_memo : table<uint64; Lint034Summary>
var private lint034_touches_memo : table<uint64; bool>
var private lint034_reaches_memo : table<uint64; bool>
var private lint034_home_module : uint64

def private lint034_holds_pointer(t : TypeDecl const?; depth : int) : bool {
    return false if (t == null || depth > 8)
    let bt = t.baseType
    return !t.flags.smartPtr if (bt == Type.tPointer)
    if (bt == Type.tArray || bt == Type.tFixedArray) {
        return lint034_holds_pointer(t.firstType, depth + 1)
    }
    if (bt == Type.tTable) {
        return (lint034_holds_pointer(t.firstType, depth + 1)
            || lint034_holds_pointer(t.secondType, depth + 1))
    }
    if (bt == Type.tStructure && t.structType != null) {
        for (fld in t.structType.fields) {
            return true if (lint034_holds_pointer(fld._type, depth + 1))
        }
    } elif (bt == Type.tTuple || bt == Type.tVariant) {
        for (arg in t.argTypes) {
            return true if (lint034_holds_pointer(arg, depth + 1))
        }
    }
    return false
}

def private lint034_type_reaches_scratch(t : TypeDecl const?; depth : int) : bool {
    //! true when a value of type `t` carries a `@scratch` field
    //! that can hold a raw pointer
    return false if (t == null || depth > 8)
    let bt = t.baseType
    if (bt == Type.tPointer || bt == Type.tArray || bt == Type.tFixedArray) {
        return lint034_type_reaches_scratch(t.firstType, depth + 1)
    }
    return lint034_type_reaches_scratch(t.secondType, depth + 1) if (bt == Type.tTable)
    return false if (bt != Type.tStructure || t.structType == null)
    for (fld in t.structType.fields) {
        let slot = (lint_has_annotation(fld.annotation, "scratch")
            && lint034_holds_pointer(fld._type, 0))
        return true if (slot || lint034_type_reaches_scratch(fld._type, depth + 1))
    }
    return false
}

def private lint034_reaches_scratch(v : Variable const?) : bool {
    return false if (v == null)
    let key = intptr(v)
    return lint034_reaches_memo?[key] ?? false if (key_exists(lint034_reaches_memo, key))
    let slot = lint_has_annotation(v.annotation, "scratch") && lint034_holds_pointer(v._type, 0)
    let reaches = slot || lint034_type_reaches_scratch(v._type, 0)
    lint034_reaches_memo[key] = reaches
    return reaches
}

def private lint034_names_scratch_global(expr : ExprVar const?) : bool {
    //! true when `expr` names a global that reaches a `@scratch`
    //! slot able to hold a raw pointer
    return (!expr.varFlags.local && !expr.varFlags.argument && !expr.varFlags._block
        && lint034_reaches_scratch(expr.variable))
}

def private lint034_declaring(st : Structure const?; name : string) : Structure const? {
    //! the structure up `st`'s parent chain that declares field `name`
    var at = st
    while (at != null && at.parent != null) {
        var inherited = false
        for (fld in at.parent.fields) {
            inherited ||= fld.name == name
        }
        break if (!inherited)
        at = at.parent
    }
    return at
}

def private lint034_field_owner(f : ExprField const?) : Structure const? {
    //! the structure that declares the field `f` reads, through a pointer
    let vt = f.value._type
    return null if (vt == null)
    let pointee = vt.baseType == Type.tPointer && vt.firstType != null
    let st = pointee ? vt.firstType.structType : vt.structType
    return lint034_declaring(st, "{f.name}")
}

def private lint034_container(expr : Expression const?; annotated_only : bool;
                              mark : string) : Lint034Container {
    //! the container `expr` writes into; `root` stays null when
    //! the container dies with the call or, under `annotated_only`,
    //! when nothing on the way to it carries `@mark`
    var dest : Lint034Container
    var e = expr
    var annotated = false
    while (e != null) {
        if (e is ExprField) {
            let f = e as ExprField
            let marked = f.field != null && lint_has_annotation(f.field.annotation, mark)
            if (!annotated && marked) {
                annotated = true
                dest.field = ".{f.name}"
                dest.owner = lint034_field_owner(f)
            }
            e = f.value
        } elif (e is ExprVar) {
            let ev = e as ExprVar
            let v = ev.variable
            return dest if (v == null || ev.varFlags._block || ev.varFlags.thisBlock)
            annotated ||= lint_has_annotation(v.annotation, mark)
            if (ev.varFlags.local) {
                let source = v.init != null ? v.init : v.loop_source
                return dest if (v._type == null || !v._type.flags.ref || source == null)
                e = source
            } else {
                return dest if (annotated_only && !annotated)
                dest.root = v
                dest.key = "{v.name}{dest.field}"
                return dest
            }
        } else {
            e = lint_value_source(e)
        }
    }
    return dest
}

class private Lint034BorrowScan : AstVisitor {
    found : bool = false
    call_depth : int = 0
    def override preVisitExprCall(_expr : ExprCall?) : void {
        call_depth++
    }
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        call_depth--
        return expr
    }
    def override preVisitExprVar(expr : ExprVar?) : void {
        let v = expr.variable
        return if (found || call_depth > 0 || v == null || !lint034_holds_pointer(v._type, 0))
        if (expr.varFlags.local) {
            found = !(v.init != null && (v.init is ExprNew || v.init is ExprAscend))
        } else {
            found = expr.varFlags.argument || expr.varFlags._block
        }
    }
}

def private lint034_borrows(var value : Expression?) : bool {
    //! true when `value` carries a pointer the function was handed
    //! and does not own
    return false if (value == null || !lint034_holds_pointer(value._type, 0))
    var scan = new Lint034BorrowScan()
    make_visitor(*scan) $(adapter) {
        visit(value, adapter)
    }
    let found = scan.found
    unsafe {
        delete scan
    }
    return found
}

def private lint034_is_fill(origin : string) : bool {
    return (origin == "push" || origin == "push_clone" || origin == "emplace"
            || origin == "push_from" || origin == "push_clone_from"
            || origin == "emplace_from" || origin == "insert" || origin == "insert_clone")
}

def private lint034_is_empty(call : ExprCall?; origin : string) : bool {
    //! `delete c` reaches lint as a `finalize(c)` call
    return true if (origin == "clear" || origin == "finalize")
    return (origin == "resize" && length(call.arguments) == 2
        && call.arguments[1] is ExprConstInt && (call.arguments[1] as ExprConstInt).value == 0)
}

class private Lint034Events : AstVisitor {
    events : array<Lint034Event>
    def note_fill(container : Lint034Container; slot, filled_by : string;
                  site : Expression const?) : void {
        events |> push(Lint034Event(container = container, fills = true, slot = slot,
            filled_by = filled_by, site = site))
    }
    def note_empty(container : Lint034Container; slot : string; site : Expression const?) : void {
        events |> push(Lint034Event(container = container, fills = false, slot = slot, site = site))
    }
    def note_store(var left, right, site : Expression?) : void {
        let dest = lint034_container(left, true, "scratch")
        return if (dest.root == null || !lint034_holds_pointer(left._type, 0))
        if (right is ExprConstPtr) {
            note_empty(dest, describe(left), site)
        } elif (lint034_borrows(right)) {
            note_fill(dest, describe(left), "", site)
        }
    }
    def callee_container(var expr : ExprCall?; theirs : Lint034Container) : Lint034Container {
        //! the caller's name for a container a callee's summary
        //! speaks of: a global as it is, one hanging off a
        //! parameter through the argument passed for it
        let arg = lint034_arg_index(expr.func, theirs.root)
        return theirs if (arg < 0)
        var mine : Lint034Container
        return mine if (arg >= length(expr.arguments))
        mine = lint034_container(expr.arguments[arg], empty(theirs.field), "scratch")
        if (mine.root != null && !empty(theirs.field)) {
            mine.field = theirs.field
            mine.key = "{mine.root.name}{theirs.field}"
        }
        return mine
    }
    def note_call_summary(var expr : ExprCall?) : void {
        var reaches = lint034_touches(expr.func)
        for (p in expr.func.arguments) {
            reaches ||= lint034_reaches_scratch(p)
        }
        return if (!reaches)
        var summary = lint034_summary_of(expr.func)
        for (theirs in summary.emptied) {
            let mine = callee_container(expr, theirs)
            note_empty(mine, "", expr) if (mine.root != null)
        }
        for (stale in summary.stale) {
            let mine = callee_container(expr, stale.container)
            note_fill(mine, stale.slot, "{expr.func.name}", expr) if (mine.root != null)
        }
    }
    def override preVisitExprCall(var expr : ExprCall?) : void {
        return if (expr.func == null)
        let origin = string(root_generic(expr.func).name)
        let empties = lint034_is_empty(expr, origin)
        if (empty(expr.arguments) || (!empties && !lint034_is_fill(origin))) {
            note_call_summary(expr) if (!expr.func.flags.builtIn && expr.func.body != null)
            return
        }
        let dest = lint034_container(expr.arguments[0], true, "scratch")
        return if (dest.root == null)
        if (empties) {
            note_empty(dest, "", expr)
            return
        }
        for (i in range(1, length(expr.arguments))) {
            if (lint034_borrows(expr.arguments[i])) {
                note_fill(dest, "", "", expr)
                return
            }
        }
    }
    def override preVisitExprCopy(var expr : ExprCopy?) : void {
        note_store(expr.left, expr.right, expr)
    }
    def override preVisitExprMove(var expr : ExprMove?) : void {
        note_store(expr.left, expr.right, expr)
    }
}

def private lint034_events(var e : Expression?) : array<Lint034Event> {
    var scan = new Lint034Events()
    make_visitor(*scan) $(adapter) {
        visit(e, adapter)
    }
    var events <- scan.events
    unsafe {
        delete scan
    }
    return <- events
}

def private lint034_filled_at(state : Lint034State; key, slot : string) : int {
    //! the store into `key` at `slot`; any store into `key` when
    //! `slot` is "*"
    for (ev, i in state.filled, count()) {
        return i if (ev.container.key == key && (slot == "*" || ev.slot == slot))
    }
    return -1
}

def private lint034_emptied_at(emptied : array<Lint034Container>; key : string) : int {
    for (c, i in emptied, count()) {
        return i if (c.key == key)
    }
    return -1
}

def private lint034_join(var into : Lint034State; from : Lint034State) : void {
    //! two paths meet: a store on either one stays, a container
    //! stays emptied only when both emptied it
    return if (!from.reached)
    if (!into.reached) {
        into := from
        return
    }
    for (ev in from.filled) {
        into.filled |> push(ev) if (lint034_filled_at(into, ev.container.key, ev.slot) < 0)
    }
    var both <- [for (c in into.emptied); c; where lint034_emptied_at(from.emptied, c.key) >= 0]
    into.emptied <- both
}

def private lint034_apply(var flow : Lint034Flow; var e : Expression?) : void {
    return if (e == null)
    for (ev in lint034_events(e)) {
        let key = ev.container.key
        if (ev.fills) {
            flow.now.filled |> push(ev) if (lint034_filled_at(flow.now, key, ev.slot) < 0)
            let was_emptied = lint034_emptied_at(flow.now.emptied, key)
            flow.now.emptied |> erase(was_emptied) if (was_emptied >= 0)
        } elif (!empty(ev.slot)) {
            let stored = lint034_filled_at(flow.now, key, ev.slot)
            flow.now.filled |> erase(stored) if (stored >= 0)
        } else {
            var stored = lint034_filled_at(flow.now, key, "*")
            while (stored >= 0) {
                flow.now.filled |> erase(stored)
                stored = lint034_filled_at(flow.now, key, "*")
            }
            let known = lint034_emptied_at(flow.now.emptied, key) >= 0
            flow.now.emptied |> push(ev.container) if (!known)
        }
    }
}

def private lint034_exit(var flow : Lint034Flow) : void {
    //! the function returns from here: every enclosing `finally`
    //! runs first, innermost first
    var after <- flow.now
    flow.now := after
    var finals <- flow.finals
    for (i in range(length(finals))) {
        var blk = finals[length(finals) - 1 - i]
        for (s in blk.finalList) {
            lint034_flow(flow, s)
        }
    }
    for (ev in flow.now.filled) {
        var seen = false
        for (st in flow.stale) {
            seen ||= st.container.key == ev.container.key
        }
        flow.stale |> push(ev) if (!seen)
    }
    lint034_join(flow.exits, flow.now)
    flow.finals <- finals
    flow.now <- after
    flow.now.reached = false
}

def private lint034_flow_block(var flow : Lint034Flow; var blk : ExprBlock?) : void {
    let has_final = !empty(blk.finalList)
    flow.finals |> push(blk) if (has_final)
    for (s in blk.list) {
        lint034_flow(flow, s)
    }
    flow.finals |> pop() if (has_final)
    for (s in blk.finalList) {
        lint034_flow(flow, s)
    }
}

def private lint034_flow_if(var flow : Lint034Flow; var ite : ExprIfThenElse?) : void {
    lint034_apply(flow, ite.cond)
    let entry := flow.now
    lint034_flow(flow, ite.if_true)
    let after_true <- flow.now
    flow.now := entry
    lint034_flow(flow, ite.if_false)
    lint034_join(flow.now, after_true)
}

def private lint034_flow_loop(var flow : Lint034Flow; var body : Expression?) : void {
    let entry := flow.now
    var outer <- flow.loop_jumps
    // the second round's exits see what the first round's end left
    for (_round in range(2)) {
        lint034_flow(flow, body)
        lint034_join(flow.now, entry)
        lint034_join(flow.now, flow.loop_jumps)
    }
    flow.loop_jumps <- outer
}

def private lint034_flow_try(var flow : Lint034Flow; var tc : ExprTryCatch?) : void {
    let entry := flow.now
    lint034_flow(flow, tc.try_block)
    lint034_join(flow.now, entry)
    let after_try := flow.now
    lint034_flow(flow, tc.catch_block)
    lint034_join(flow.now, after_try)
}

def private lint034_flow(var flow : Lint034Flow; var e : Expression?) : void {
    //! walks one statement on the current path; a path that has
    //! left the function walks nothing
    return if (e == null || !flow.now.reached)
    if (e is ExprBlock) {
        lint034_flow_block(flow, e as ExprBlock)
    } elif (e is ExprIfThenElse) {
        lint034_flow_if(flow, e as ExprIfThenElse)
    } elif (e is ExprWith) {
        lint034_flow(flow, (e as ExprWith).body)
    } elif (e is ExprUnsafe) {
        lint034_flow(flow, (e as ExprUnsafe).body)
    } elif (e is ExprFor) {
        var loop = e as ExprFor
        for (s in loop.sources) {
            lint034_apply(flow, s)
        }
        lint034_flow_loop(flow, loop.body)
    } elif (e is ExprWhile) {
        var loop = e as ExprWhile
        lint034_apply(flow, loop.cond)
        lint034_flow_loop(flow, loop.body)
    } elif (e is ExprTryCatch) {
        lint034_flow_try(flow, e as ExprTryCatch)
    } elif (e is ExprReturn) {
        lint034_apply(flow, (e as ExprReturn).subexpr)
        lint034_exit(flow)
    } elif (e is ExprBreak || e is ExprContinue) {
        lint034_join(flow.loop_jumps, flow.now)
        flow.now.reached = false
    } else {
        lint034_apply(flow, e)
    }
}

def private lint034_arg_index(fn : Function const?; v : Variable const?) : int {
    for (p, i in fn.arguments, count()) {
        return i if (intptr(p) == intptr(v))
    }
    return -1
}

def private lint034_summarize(var fn : Function?) : Lint034Summary {
    var flow : Lint034Flow
    flow.now.reached = true
    lint034_flow(flow, fn.body)
    lint034_exit(flow) if (flow.now.reached)
    return <- Lint034Summary(stale <- flow.stale, emptied <- flow.exits.emptied)
}

def private lint034_passes_to_caller(fn : Function const?; stale : Lint034Event) : bool {
    //! true when the caller is the one to empty this container: it
    //! hangs off a parameter, or `fn` is private
    return lint034_arg_index(fn, stale.container.root) >= 0 || fn.flags.privateFunction
}

def private lint034_summary_of(var fn : Function?) : Lint034Summary {
    //! what a call to `fn` leaves its caller to empty, and what it
    //! empties for the caller
    var for_callers : Lint034Summary
    return <- for_callers if (fn == null || fn.body == null)
    let key = fn.getMangledNameHash
    if (key_exists(lint034_summary_memo, key)) {
        for_callers := lint034_summary_memo[key]
        return <- for_callers
    }
    lint034_summary_memo[key] <- for_callers
    var full = lint034_summarize(fn)
    for_callers.emptied <- full.emptied
    for (stale in full.stale) {
        for_callers.stale |> push(stale) if (lint034_passes_to_caller(fn, stale))
    }
    lint034_summary_memo[key] := for_callers
    return <- for_callers
}

class private Lint034TouchScan : AstVisitor {
    found : bool = false
    def override preVisitExprVar(expr : ExprVar?) : void {
        found ||= lint034_names_scratch_global(expr)
    }
    def override preVisitExprCall(expr : ExprCall?) : void {
        found ||= lint034_touches(expr.func)
    }
}

def private lint034_touches(fn : Function const?) : bool {
    //! true when `fn`, or a function of the module under lint that
    //! it calls, names a `@scratch` container that outlives the call
    return false if (fn == null || fn.body == null || fn.flags.builtIn
        || intptr(fn._module) != lint034_home_module)
    let key = fn.getMangledNameHash
    return lint034_touches_memo?[key] ?? false if (key_exists(lint034_touches_memo, key))
    lint034_touches_memo[key] = false
    var scan = new Lint034TouchScan()
    make_visitor(*scan) $(adapter) {
        visit(fn.body, adapter)
    }
    let found = scan.found
    unsafe {
        delete scan
    }
    lint034_touches_memo[key] = found
    return found
}

def private lint035_field_key(st : Structure const?; name : string) : string =>
    "{intptr(st)}.{name}"

def private lint035_each_pool(mod : Module?;
                              blk : block<(what, key : string; at : LineInfo) : void>) {
    //! every pointer-holding `@pool` declaration of `mod`; `key` is
    //! what `Lint035DrainScan` records when a drain reaches it
    for_each_structure(mod) $(st) {
        for (fld in st.fields) {
            continue if (!lint_has_annotation(fld.annotation, "pool")
                || !lint034_holds_pointer(fld._type, 0)
                || intptr(lint034_declaring(st, "{fld.name}")) != intptr(st))
            invoke(blk, "{st.name}.{fld.name}", lint035_field_key(st, "{fld.name}"), fld.at)
        }
    }
    for_each_global(mod) $(v) {
        return if (!lint_has_annotation(v.annotation, "pool") || !lint034_holds_pointer(v._type, 0))
        invoke(blk, "{v.name}", "{intptr(v)}", v.at)
    }
}

def private lint035_is_field(st : Structure const?; name : string; t : TypeDecl?) : bool {
    //! true when `t` is the type of `st`'s field `name`: the
    //! field itself, not a value inside it
    for (fld in st.fields) {
        return is_same_type(fld._type, t, false, false, false, false) if (fld.name == name)
    }
    return false
}

//! the `@pool` declarations the emptying calls of one module drain:
//! a field by `lint035_field_key` of its declaring structure, a
//! global by its address
class private Lint035DrainScan : AstVisitor {
    drained : table<string>
    def note_deleted(t : TypeDecl const?; depth : int) : void {
        //! a `delete` of a value of type `t` drains every pool field
        //! that value holds
        return if (t == null || depth > 8)
        let bt = t.baseType
        if (bt == Type.tPointer || bt == Type.tArray || bt == Type.tFixedArray) {
            note_deleted(t.firstType, depth + 1)
        } elif (bt == Type.tTable) {
            note_deleted(t.secondType, depth + 1)
        } elif (bt == Type.tTuple || bt == Type.tVariant) {
            for (arg in t.argTypes) {
                note_deleted(arg, depth + 1)
            }
        } elif (bt == Type.tStructure) {
            var st = t.structType
            while (st != null) {
                for (fld in st.fields) {
                    if (lint_has_annotation(fld.annotation, "pool")) {
                        drained |> insert(lint035_field_key(st, "{fld.name}"))
                    }
                    note_deleted(fld._type, depth + 1)
                }
                st = st.parent
            }
        }
    }
    def override preVisitExprCall(var expr : ExprCall?) : void {
        return if (expr.func == null || empty(expr.arguments))
        let origin = string(root_generic(expr.func).name)
        return if (!lint034_is_empty(expr, origin))
        let arg = expr.arguments[0]
        if (origin == "finalize" && lint034_container(arg, false, "pool").root != null) {
            note_deleted(arg._type, 0)
        }
        let dest = lint034_container(arg, true, "pool")
        return if (dest.root == null)
        if (dest.owner != null) {
            let name = slice(dest.field, 1)
            if (lint035_is_field(dest.owner, name, arg._type)) {
                drained |> insert(lint035_field_key(dest.owner, name))
            }
        } elif (lint_has_annotation(dest.root.annotation, "pool")
                && is_same_type(dest.root._type, arg._type, false, false, false, false)) {
            drained |> insert("{intptr(dest.root)}")
        }
    }
}

def private lint035_drained(mod : Module?) : table<string> {
    //! every `@pool` declaration an emptying call in `mod` drains; a
    //! generated finalizer empties every field it owns and drains nothing
    var scan = new Lint035DrainScan()
    make_visitor(*scan) $(adapter) {
        for_each_function(mod, "") $(fn) {
            visit(fn, adapter) if (fn.body != null && is_user_authored_body(fn))
        }
    }
    var drained <- scan.drained
    unsafe {
        delete scan
    }
    return <- drained
}

class LintVisitor : AstVisitor {
    lint034_armed : bool = false
    lint030_frames : array<Lint030Frame>
    lint030_plain_closures : array<bool>
    @do_not_delete lint014_candidates : array<Variable const?>
    @do_not_delete lint003_deferred : array<Variable const?>
    @do_not_delete lint004_deferred : array<Variable const?>
    lint004_uses : array<Lint004Use>
    @do_not_delete lint_fun : Function?
    lint_fun_names : table<string>
    lint_fun_names_ready : bool
    @do_not_delete lint_expr_stack : array<Expression?>
    @do_not_delete lint_mut_result : TypeDecl const?
    lint023_candidates : array<Lint023Candidate>
    lint023_store_nodes : table<uint64>
    @do_not_delete lint029_candidates : array<Variable const?>
    @do_not_delete lint029_mutated : array<Variable const?>
    lint029_exempt : bool = false
    @do_not_delete lint031_params : array<Variable const?>
    @do_not_delete lint031_reported : array<Variable const?>
    //! AST visitor that performs lint checks.
    exprForTerminator : array<uint64>
    compile_time_errors : bool
    noLint : bool = false
    genericDepth : int = 0
    multiple_contexts : bool = false
    collect_errors : bool = false
    errors : array<string>
    issues : array<LintIssue>
    disabled_codes : table<string>
    enabled_codes : table<string>
    branch_depth : int = 0
    ds_entries : array<DeadStoreEntry>
    funcFile : string
    lint021_candidates : array<Lint021Candidate>
    lint021_call_kind : array<Lint021CallKind>
    lint021_narrow_depth : int = 0
    lint021_poison_depth : int = 0
    lint021_closure_depth : int = 0
    lint023_deferred_depth : int = 0
    flow_jumps : bool = false
    lint022_refs : RefIndex
    @do_not_delete lint022_self_struct : Structure const? = null
    lint022_generic_sweep : bool = false
    def LintVisitor() {
        pass
    }
    def is_suppressed(text : string; at : LineInfo) : bool {
        let code = extract_lint_code(text)
        if (!empty(code)) {
            return true if (key_exists(disabled_codes, code)
                            || (!empty(enabled_codes) && !key_exists(enabled_codes, code)))
        }
        return is_lint_suppressed(at, code)
    }
    def lint_error(text : string; at : LineInfo; fix : LintFix = LintFix()) : void {
        if (noLint || is_suppressed(text, at)
                || (at.fileInfo != null && !empty(funcFile) && at.fileInfo.name != funcFile)) return
        let msg = lint_fix_message(text, fix)
        if (compile_time_errors) {
            compiling_program() |> macro_error(at, msg)
        } elif (collect_errors) {
            errors |> push("{msg} at {describe(at)}")
            var issue = make_lint_issue(text, at)
            issue.fix = fix
            issues |> push(issue)
        } else {
            error("{msg} at {describe(at)}\n")
        }
    }
    def private is_temporary_string_source(expr : Expression?) : bool {
        if (expr == null) return false
        if (expr._type != null && expr._type.flags.temporary) return true
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value).subexpr
        }
        if (inner is ExprVar) {
            let variable = (inner as ExprVar).variable
            return variable != null && variable._type != null && variable._type.flags.temporary
        }
        return false
    }
    def private is_inferred_temp_string_clone(expr : Expression?; clone_at : LineInfo) : bool {
        //! InferTypes rewrites `dst := string#_expr` to `dst = clone_string(string#_expr)` and gives the injected call the original := token location. A source-written clone_string call keeps its own location.
        if (expr == null || !(expr is ExprCall)) return false
        let call = expr as ExprCall
        return (call.func != null && call.func.name == "clone_string"
            && call.at.line == clone_at.line && call.at.column == clone_at.column)
    }
    def private is_inferred_temp_string_init(expr : Expression?; init_at : LineInfo) : bool {
        //! InferTypes rewrites `var dst := string#_expr` through clone_string. The wrapper result is a regular string, so inspect its source argument to preserve the temporary-string exception.
        if (expr == null || !(expr is ExprCall)) return false
        let call = expr as ExprCall
        let root = root_generic(call.func)
        return (root != null && root.name == "clone_string"
            && root._module != null && root._module.name == "builtin"
            && call.at.line == init_at.line && call.at.column == init_at.column
            && !empty(call.arguments) && is_temporary_string_source(call.arguments[0]))
    }
    def private source_variable_init_is_clone(v : VariablePtr) : bool {
        //! String clone initialization is lowered before lint runs. Search only between the source variable name and its initializer, so another declaration or := inside the RHS cannot produce a false positive.
        if (v == null || v.init == null || v.at.fileInfo == null || v.at.line == 0u) return false
        var found = false
        get_file_source_line(v.at.fileInfo, v.at.line) $(line) {
            var start = int(v.at.column) - 1
            if (start < 0) {
                start = 0
            }
            var finish = length(line)
            if (v.init.at.line == v.at.line && int(v.init.at.column) > start) {
                finish = int(v.init.at.column) + 1
                if (finish > length(line)) {
                    finish = length(line)
                }
            }
            if (finish > start) {
                found = find(slice(line, start, finish), ":=") >= 0
            }
        }
        return found
    }
    def private check_string_clone_init(v : VariablePtr) : void {
        if (v == null || multiple_contexts || noLint || genericDepth > 0
                || v.flags.generated || v._type == null || v.init == null
                || v._type.baseType != Type.tString
                || is_temporary_string_source(v.init)
                || is_inferred_temp_string_init(v.init, v.at)
                || !source_variable_init_is_clone(v)) return
        lint_error("LINT016: ':=' does not clone a string variable without 'options multiple_contexts'; use '=' for same-context storage, or did you mean '= clone_string(value)' for a cross-context copy", v.at)
    }
    def private check_string_clone_assign(expr : ExprCopy?) : void {
        if (expr == null || multiple_contexts || noLint || genericDepth > 0 || expr.genFlags.generated
                || expr.left == null || expr.right == null
                || expr.left._type == null || expr.right._type == null
                || expr.left._type.baseType != Type.tString
                || expr.right._type.baseType != Type.tString
                || is_temporary_string_source(expr.right)
                || is_inferred_temp_string_clone(expr.right, expr.at)
                || !expr.copy_flags.fromClone) return
        lint_error("LINT016: ':=' does not clone a string without 'options multiple_contexts'; use '=' for same-context storage, or did you mean 'clone_string(value)' for a cross-context copy", expr.at)
    }
    def private check_string_push_clone(expr : ExprCall?) : void {
        if (expr == null || multiple_contexts || noLint || genericDepth > 0 || expr.genFlags.generated
                || expr.func == null || length(expr.arguments) < 2) return
        let root = root_generic(expr.func)
        if (root == null || root.name != "push_clone"
                || root._module == null || root._module.name != "builtin") return
        let dst = expr.arguments[0]
        let value = expr.arguments[1]
        if (dst == null || value == null || dst._type == null || value._type == null
                || dst._type.baseType != Type.tArray || dst._type.firstType == null
                || dst._type.firstType.baseType != Type.tString
                || value._type.baseType != Type.tString
                || is_temporary_string_source(value)) return
        lint_error("LINT016: 'push_clone' does not clone a string without 'options multiple_contexts'; use 'push(value)' for same-context storage, or did you mean 'push(clone_string(value))' for a cross-context copy", expr.at)
    }
    //! the callee-body memos outlive a program: the runner, the MCP server and the LSP keep one
    //! process across programs, and an edited body keeps its mangled name
    def override preVisitProgram(_prog : ProgramPtr) : void {
        lint030_writers |> clear()
        lint031_writers |> clear()
        lint034_summary_memo |> clear()
        lint034_touches_memo |> clear()
        lint034_reaches_memo |> clear()
    }

    def override preVisitFunction(var fun : FunctionPtr) : void {
        noLint = lint022_generic_sweep
        lint_mut_result = fun.result
        funcFile = fun.at.fileInfo != null ? "{fun.at.fileInfo.name}" : ""
        lint_fun = fun
        lint034_armed = false
        lint034_home_module = intptr(fun._module)
        lint_fun_names_ready = false
        lint022_self_struct = null
        if (fun.flags.isClassMethod || fun.flags.generated) {
            if (fun.result != null && fun.result.structType != null) {
                lint022_self_struct = fun.result.structType
            } elif (!empty(fun.arguments) && fun.arguments[0]._type != null) {
                lint022_self_struct = fun.arguments[0]._type.structType
            }
        }
        if (fun.fromGeneric != null || fun.flags.generated) {
            genericDepth++
        }
        branch_depth = 0
        ds_entries |> clear()
        lint031_params |> clear()
        lint031_reported |> clear()
        lint021_candidates |> clear()
        lint021_call_kind |> clear()
        lint021_narrow_depth = 0
        lint021_poison_depth = 0
        lint021_closure_depth = 0
        lint023_deferred_depth = 0
        lint030_frames |> clear()
        lint030_plain_closures |> clear()
        flow_jumps = false
        if (fun.moreFlags.isTemplate) {
            noLint = true
            return
        }
        for (ann in fun.annotations) {
            if (ann.annotation.name == "no_lint") {
                noLint = true
                break
            }
        }
    }
    def override preVisitExprReturn(expr : ExprReturn?) : void {
        mut_use(expr.subexpr) if (lint_mut_result != null && !lint_mut_result.flags.constant)
        if (expr.subexpr != null && expr.subexpr is ExprVar) {
            let ev = expr.subexpr as ExprVar
            let idx = lint014_candidates |> find_index(ev.variable)
            if (idx >= 0) {
                lint014_candidates |> erase(idx)
            }
            let idx29 = lint029_candidates |> find_index(ev.variable)
            if (idx29 >= 0) {
                lint029_candidates |> erase(idx29)
            }
        }
    }

    def mut_use(e : Expression const?) : void {
        //! ``e`` is used where a mutable value is required: the variable it reads from is no longer a LINT003 / LINT014 candidate.
        if (e == null) return
        if (e is ExprVar) {
            let v = (e as ExprVar).variable
            let ai = v != null && v._type.isPointer ? lint014_candidates |> find_index(v) : -1
            lint014_candidates |> erase(ai) if (ai >= 0)
            let li = lint003_deferred |> find_index(v)
            lint003_deferred |> erase(li) if (li >= 0)
            mut_use(v.loop_source) if (v != null)
        } elif (e is ExprOp3) {
            mut_use((e as ExprOp3).left)
            mut_use((e as ExprOp3).right)
        } elif (e is ExprNullCoalescing) {
            mut_use((e as ExprNullCoalescing).subexpr)
            mut_use((e as ExprNullCoalescing).defaultValue)
        } else {
            mut_use(lint_value_source(e))
        }
    }

    def mut_use_arguments(call : ExprCallFunc?) : void {
        return if (call.func == null)
        for (a, i in call.arguments, count()) {
            mut_use(a) if (i < length(call.func.arguments) && (lint_mutable_slot(call.func.arguments[i]._type) || lint031_callee_writes(call.func, i)))
        }
    }

    def lint034_report(var fun : FunctionPtr) : void {
        return if (!lint034_armed || flow_jumps || noLint || genericDepth != 0
            || key_exists(disabled_codes, "LINT034"))
        var summary = lint034_summarize(fun)
        for (stale in summary.stale) {
            continue if (lint034_passes_to_caller(fun, stale))
            let key = stale.container.key
            let through = empty(stale.filled_by) ? "" : " through `{stale.filled_by}`"
            lint_error("LINT034: @scratch `{key}` still holds a pointer that `{fun.name}` " +
                "received from its caller{through} when `{fun.name}` returns. The caller may " +
                "free that object, and the next reader or garbage collection then reads " +
                "freed memory. Empty `{key}` on every exit; one `finally` block covers them all. " +
                "A container that owns what it holds across calls - a free list, a cache - is " +
                "marked @pool instead.",
                stale.site.at)
        }
    }
    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        if (!noLint && genericDepth == 0) {
            for (v in lint014_candidates) {
                lint_error("LINT014: mutable argument {v.name}: {describe(v._type)} is never written; declare it without 'var'", v.at, lint_var_keyword_fix(v.at, true))
            }
            for (v in lint003_deferred) {
                lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at, lint_var_keyword_fix(v.at, false))
            }
        }
        for (v in lint004_deferred) {
            lint_error("LINT004: variable '{v.name}' is used and should be named without " +
                "underscore prefix", v.at, lint004_fix(v))
        }
        lint014_candidates |> clear()
        lint003_deferred |> clear()
        lint004_deferred |> clear()
        lint004_uses |> clear()
        if (!noLint && genericDepth == 0) {
            for (c in lint023_candidates) {
                if (!c.disqualified && c.stores > 0 && !flow_jumps) {
                    lint_error("LINT023: mutable by-value argument {c.v.name}: {describe(c.v._type, false, false, false)} is written but never read - the caller never sees a write to a by-value copy; declare it `{describe(c.v._type, false, false, false)}&` if it is an out-parameter, otherwise the write is dead", c.v.at)
                }
            }
        }
        lint023_candidates |> clear()
        lint023_store_nodes |> clear()
        if (!noLint && genericDepth == 0) {
            for (v in lint029_candidates) {
                let structish = (v._type != null
                    && (v._type.baseType == Type.tStructure || v._type.baseType == Type.tHandle))
                continue if (structish && lint029_mutated |> find_index(v) < 0)
                lint_error("LINT029: by-ref argument {v.name}: {describe(v._type, false, false, false)} is mutated - every call site sees the write; declare the parameter const and return the result instead if this function should be pure", v.at)
            }
        }
        lint029_candidates |> clear()
        lint029_mutated |> clear()
        if (!noLint && genericDepth == 0) {
            for (e in ds_entries) {
                if (!e.skip && e.has_pending && e.pending_pure && e.pending_expr != null) {
                    let vname = "{e.v.name}"
                    lint_error("LINT010: dead store of '{vname}': value written but never read before scope exit", e.pending_expr.at)
                }
            }
        }
        if (!noLint && genericDepth == 0) {
            for (c in lint021_candidates) {
                if (!c.disqualified && c.safe_uses > 0) {
                    lint_error("LINT021: 64-bit local '{c.v.name}' is never consumed as 64-bit - every use narrows through int(...)/uint(...); narrow once at the declaration (or use length() int math), or lift the sinks to 64-bit (range64, int64 resize/reserve/erase); nolint if the 64-bit math under a cast is load-bearing", c.v.at)
                }
            }
        }
        lint021_candidates |> clear()
        lint034_report(fun)
        noLint = false
        if (fun.fromGeneric != null || fun.flags.generated) {
            genericDepth--
        }
        return <- fun
    }
    def override preVisitExprBlock(blk : ExprBlock?) : void {
        exprForTerminator |> push(0ul)
        let bf = blk.blockFlags
        if (!noLint && genericDepth == 0 && (bf.isClosure || bf.isLambdaBlock || bf.isGeneratorBlock)) {
            branch_depth++
            lint021_closure_depth++
            lint023_deferred_depth++
            lint030_plain_closures |> push(!bf.isLambdaBlock && !bf.isGeneratorBlock)
        }
    }
    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (!empty(exprForTerminator)) {
            exprForTerminator |> pop()
        }
        let bf = blk.blockFlags
        if (!noLint && genericDepth == 0 && (bf.isClosure || bf.isLambdaBlock || bf.isGeneratorBlock)) {
            branch_depth--
            lint021_closure_depth--
            lint023_deferred_depth--
            lint030_plain_closures |> pop()
        }
        return <- blk
    }
    def override preVisitExprBlockExpression(_blk : ExprBlock?; expr : ExpressionPtr) : void {
        if (noLint || genericDepth > 0 || !(expr is ExprOp1)) return
        let op1 = expr as ExprOp1
        if (op1.op == "++" || op1.op == "--" || op1.op == "+++" || op1.op == "---") {
            if (op1.subexpr != null && op1.subexpr is ExprVar) {
                lint023_store_nodes |> insert(intptr(op1.subexpr))
            }
        }
    }
    def override visitExprBlockExpression(_blk : ExprBlock?; var expr : ExpressionPtr) : ExpressionPtr {
        let lb = exprForTerminator |> back()
        if (lb != 0ul) {
            lint_error("LINT001: unreachable code", expr.at, lint001_fix(expr))
        }
        if (!noLint && genericDepth == 0 && expr is ExprOp1 && !expr.genFlags.generated) {
            let op1 = expr as ExprOp1
            if (op1.op == "+" || op1.op == "-") {
                lint_error("LINT015: free-floating unary '{op1.op}' expression as a statement - its value is silently discarded; this is almost always a continuation line missing its parens, wrap the multi-line RHS in (...)", expr.at)
            }
        }
        if (is_block_terminator(expr)) {
            exprForTerminator |> pop()
            exprForTerminator |> push(intptr(expr))
        }
        return <- expr
    }
    def override preVisitExprLabel(_expr : ExprLabel?) : void {
        if (!empty(exprForTerminator)) {
            exprForTerminator |> pop()
        }
        exprForTerminator |> push(0ul)
        note_flow_jump()
    }

    def override preVisitExprCast(expr : ExprCast?) : void {
        if (lint022_generic_sweep) {
            lint022_refs |> note_type_names(expr.castType, 0)
        }
        if (noLint || genericDepth > 0
            || !expr.castFlags.reinterpretCast || expr.genFlags.alwaysSafe
            || expr.subexpr == null || expr.subexpr._type == null || expr.castType == null
            || expr.castType.isVoidPointer != expr.subexpr._type.isVoidPointer) return
        if (is_same_type(expr.castType, expr.subexpr._type, false, true, true, false)) {
            lint_error("LINT005: redundant reinterpret cast; target type is the same as source type", expr.at)
        }
    }

    def override preVisitExprForVariable(expr : ExprFor?; v : VariablePtr; _last : bool) : void {
        if (expr.canShadow) return
        validate_var(v, false)
    }

    def private is_inline_temp_name(name : string) : bool {
        if (!(name |> starts_with("_inl")) || length(name) <= 4) return false
        var digit = false
        peek_data(name) $(bytes) {
            digit = is_number(int(bytes[4]))
        }
        return digit
    }

    def validate_var(v : VariablePtr; can_make_const : bool) {
        if (v.flags.generated || v.flags.marked_used) return
        let name = string(v.name)
        if (name |> starts_with("__") || find(name, "`") >= 0 || is_inline_temp_name(name)) return
        if (name |> starts_with("_")) {
            if (name |> ends_with("_")) return
            if (v.access_flags.access_ref || v.access_flags.access_pass || v.access_flags.access_get || v.access_flags.access_fold) {
                if (genericDepth == 0) {
                    lint004_deferred |> push(v)
                } else {
                    lint_error("LINT004: variable '{v.name}' is used and should be named without " +
                        "underscore prefix", v.at)
                }
            }
            return
        }
        let unused_fix = genericDepth == 0 ? underscore_fix(v) : LintFix()
        if (v.isAccessUnused) {
            lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an " +
                "underscore prefix if you really need it)", v.at, unused_fix)
            return
        }
        if (!v.access_flags.access_ref && !v.access_flags.access_pass && v.init != null && v.init |> is_expr_const() && !v.access_flags.access_fold) {
            lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an " +
                "underscore prefix if you really need it)", v.at, unused_fix)
            return
        }
        if (!v.access_flags.access_ref && !v.access_flags.access_pass && !v.access_flags.access_get && (v.init == null || v.init.flags.noSideEffects) && !v.access_flags.access_fold) {
            lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an " +
                "underscore prefix if you really need it)", v.at, unused_fix)
            return
        }
        if (!v.access_flags.access_get && !v.access_flags.access_ref && (v.init == null)) {
            lint_error("LINT002: variable {v.name}: {describe(v._type)} is never used", v.at,
                       unused_fix)
            return
        }
        if (can_make_const && (v.flags.inScope || v.access_info.access_info_pass_mutable)) return
        if (can_make_const && v._type.baseType != Type.tPointer && !v._type.flags.constant && !v.access_flags.access_ref && v._type.canCloneFromConst) {
            lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at, lint_var_keyword_fix(v.at, false))
            return
        }
        if (can_make_const && v._type.baseType == Type.tPointer && !v._type.flags.constant && !(v.access_flags.access_ref || v.access_flags.access_pass) && v._type.canCloneFromConst) {
            lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at, lint_var_keyword_fix(v.at, false))
            return
        }
        if (can_make_const && !v._type.flags.constant
                && (v._type.baseType == Type.tPointer || (!v.access_flags.access_ref && v.flags.init_via_move && !v._type.canCloneFromConst))) {
            lint003_deferred |> push(v)
        }
    }

    def validate_argument(v : VariablePtr; code, kind : string; renamable : bool) {
        //! ``renamable``: no call site can name the argument from outside the
        //! module, so the underscore prefix is a fix
        if (noLint || genericDepth > 0 || v.flags.generated || v.flags.marked_used) return
        let name = string(v.name)
        if (name |> starts_with("_") || find(name, "`") >= 0) return
        if (v.isAccessUnused) {
            lint_error("{code}: unused {kind} {v.name}: {describe(v._type)} (add an underscore " +
                "prefix or [unused_argument({v.name})] if intentional)", v.at, renamable
                    ? underscore_fix(v) : LintFix())
        }
    }

    //! whether the function being walked declares or reads ``name``
    def fun_name_taken(name : string) : bool {
        if (!lint_fun_names_ready && lint_fun != null) {
            lint_fun_names <- lint_function_names(lint_fun)
            lint_fun_names_ready = true
        }
        return key_exists(lint_fun_names, name)
    }

    //! prefixes the unused ``v`` with an underscore, unless the function
    //! already names the prefixed spelling
    def underscore_fix(v : Variable const?) : LintFix {
        let name = string(v.name)
        if (fun_name_taken("_{name}")) return LintFix()
        return lint_token_fix(v.at, int(v.at.line), int(v.at.column), name, "_{name}")
    }

    //! renames a used ``_name`` local and every read of it, when each
    //! read is spelled in source and the function names the bare spelling
    //! nowhere
    def lint004_fix(v : Variable const?) : LintFix {
        let name = string(v.name)
        let bare = slice(name, 1)
        if (!is_alpha(first_character(bare)) || fun_name_taken(bare)) return LintFix()
        var edits <- [lint_token_fix(v.at, int(v.at.line), int(v.at.column), name, bare)]
        for (u in lint004_uses) {
            edits |> push(u.fix) if (u.v == v)
        }
        for (e in edits) {
            return LintFix() if (e.line == 0)
        }
        return lint_merge_fixes(v.at, edits)
    }

    def validate_unwritten_var_argument(fun : FunctionPtr; v : VariablePtr) {
        if (noLint || genericDepth > 0 || v.flags.generated || v.flags.marked_used) return
        let name = string(v.name)
        if (v._type != null && v._type.isPointer && !v._type.flags.constant && lint_mutable_slot(v._type) && !(name |> starts_with("_")) && find(name, "`") < 0
                && fun.name != "finalize" && fun.flags.privateFunction && !fun.moreFlags.addressTaken
                && !v.access_info.access_info_pass_mutable && !v.isAccessUnused) {
            lint014_candidates |> push(v)
            return
        }
        if ((name |> starts_with("_")) || find(name, "`") >= 0
                || fun.name == "finalize" || fun.moreFlags.addressTaken
                || v._type == null || v._type.flags.constant
                || !(v._type.flags.ref || v._type.isRefType)
                || v.access_flags.access_ref || v.access_info.access_info_pass_mutable
                || v.isAccessUnused) {
            return
        }
        lint014_candidates |> push(v)
    }

    def lint029_collect_candidate(fun : FunctionPtr; v : VariablePtr) {
        if (lint029_exempt || noLint || genericDepth > 0 || v.flags.generated || v.flags.marked_used || v.isAccessUnused) return
        let name = string(v.name)
        if ((name |> starts_with("_")) || find(name, "`") >= 0
                || fun.name == "finalize" || fun.moreFlags.addressTaken
                || v._type == null || v._type.flags.constant
                || !(v._type.flags.ref || v._type.isRefType)
                || !(v.access_flags.access_ref || v.access_info.access_info_pass_mutable)) {
            return
        }
        lint029_candidates |> push(v)
    }

    def lint023_collect_candidate(v : VariablePtr) {
        if (noLint || genericDepth > 0 || v.flags.generated || v.flags.marked_used || v.isAccessUnused) return
        let name = string(v.name)
        if ((name |> starts_with("_")) || find(name, "`") >= 0
                || v._type == null || v._type.flags.constant
                || v._type.flags.ref || v._type.isRefType) {
            return
        }
        lint023_candidates |> push(Lint023Candidate(v = v, stores = 0, disqualified = false))
    }

    def lint023_find(v : Variable const?) : int {
        for (i in range(length(lint023_candidates))) {
            if (lint023_candidates[i].v == v) return i
        }
        return -1
    }

    def lint023_mark_store_target(lhs : ExpressionPtr) : void {
        if (lhs == null) return
        if (lhs is ExprVar) {
            lint023_store_nodes |> insert(intptr(lhs))
        } elif (lhs is ExprSwizzle) {
            let sw = lhs as ExprSwizzle
            if (sw.value != null && sw.value is ExprVar) {
                lint023_store_nodes |> insert(intptr(sw.value))
            }
        }
    }

    def lint023_note_use(expr : ExprVar?) : void {
        let ci = lint023_find(expr.variable)
        if (ci < 0 || lint023_candidates[ci].disqualified) return
        if (expr.varFlags.under_clone || key_exists(lint023_store_nodes, intptr(expr))) {
            lint023_candidates[ci].stores++
        } elif (lint023_candidates[ci].stores > 0 || lint023_deferred_depth > 0) {
            lint023_candidates[ci].disqualified = true
        }
    }

    def override preVisitFunctionArgument(fun : FunctionPtr; arg : VariablePtr; _lastArg : bool) : void {
        if (lint022_generic_sweep) {
            lint022_refs |> note_type_names(arg._type, 0)
        }
        if (arg._type != null && arg._type.flags.constant && !arg._type.flags._implicit
                && (arg._type.isRef || arg._type.isPointer)) {
            lint031_params |> push(arg)
        }
        if (fun.flags.isClassMethod) return
        for (ann in fun.annotations) {
            if (ann.annotation.name == "extern") return
        }
        validate_argument(arg, "LINT012", "function argument", fun.flags.privateFunction)
        validate_unwritten_var_argument(fun, arg)
        lint023_collect_candidate(arg)
        lint029_collect_candidate(fun, arg)
    }

    def lint031_note_write(target : Expression const?; at : LineInfo) : void {
        lint031_note_write_root(target, false, at)
    }

    def lint031_note_write_root(target : Expression const?; through : bool; at : LineInfo) : void {
        if (noLint || genericDepth > 0 || empty(lint031_params)) return
        let root = lint031_root(lint031_params, target, through)
        if (root == null || lint031_reported |> find_index(root) >= 0) return
        lint031_reported |> push(root)
        let kind = root._type.isPointer ? "pointer" : "reference"
        lint_error("LINT031: `{root.name}` is a const {kind} parameter the body writes through at line {int(at.line)} - the const was stripped on the way (a reinterpret, an intptr round trip, or a writable slot of a call), so the declared type denies a write the function performs; declare the parameter `var`", root.at)
    }

    //! the chase runs first: a callee body is scanned only for an argument rooted at a const parameter
    def lint031_note_call_argument(call : ExprCall?; arg : ExpressionPtr) : void {
        if (noLint || genericDepth > 0 || empty(lint031_params)) return
        if (lint031_root(lint031_params, arg, true) != null && lint031_slot_written(call, arg)) {
            lint031_note_write_root(arg, true, call.at)
        }
    }

    def private lint029_note_mutation(v : Variable const?) : void {
        if (v != null && lint029_candidates |> find_index(v) >= 0
                && lint029_mutated |> find_index(v) < 0) {
            lint029_mutated |> push(v)
        }
    }

    def override preVisitExprField(expr : ExprField?) : void {
        if (noLint || genericDepth > 0 || expr.value == null) return
        var obj = expr.value
        if (obj is ExprRef2Value) {
            obj = (obj as ExprRef2Value).subexpr
        }
        return if (!(obj is ExprVar) || (expr._type != null
            && (expr._type.baseType == Type.tPointer || expr._type.baseType == Type.tHandle)))
        lint029_note_mutation((obj as ExprVar).variable)
    }

    def override preVisitExprBlockArgument(_blk : ExprBlock?; arg : VariablePtr; _lastArg : bool) : void {
        validate_argument(arg, "LINT013", "block argument", true)
    }

    def lint017_long_twin(fname : string; icall : ExprCall?) : string {
        let nargs = length(icall.arguments)
        if (nargs == 0 || icall.arguments[0]._type == null) return ""
        let bt = icall.arguments[0]._type.baseType
        if (fname == "length" && (bt == Type.tArray || bt == Type.tTable || bt == Type.tString)) return "long_length"
        if (fname == "capacity" && (bt == Type.tArray || bt == Type.tTable)) return "long_capacity"
        if (fname == "count" && (bt == Type.tArray || bt == Type.tIterator)) return "long_count"
        if (fname == "find_index" && bt == Type.tArray && nargs == 2) return "long_find_index"
        if ((fname == "fread" || fname == "fwrite") && bt == Type.tPointer && nargs == 2) return "long_{fname}"
        return ""
    }

    //! The 64-bit twin of the 32-bit count ``call`` - ``long_length(x)``
    //! for ``length(x)`` - with its written arguments; "" when it has
    //! none
    def lint017_twin_call(call : ExprCall?) : string {
        let name = string(call.func != null
                          && call.func.fromGeneric != null ? call.func.fromGeneric.name : call.name)
        let twin = lint017_long_twin(name, call)
        if (empty(twin)) return ""
        let args <- lint_written_args(call)
        return empty(args) ? "" : "{twin}({join(args, ", ")})"
    }

    def lint017_is_32bit_parse(fname : string; icall : ExprCall?) : bool {
        if (fname != "to_int" && fname != "to_uint") return false
        let nargs = length(icall.arguments)
        if (nargs == 0 || icall.arguments[0]._type == null) return false
        return icall.arguments[0]._type.baseType == Type.tString
    }

    // The remedy repeats the site's own string and hex arguments, and keeps a mixed-sign
    // site's outer cast, so only the parse width changes.
    def lint017_parse_remedy(cast_name : string; iname : string; icall : ExprCall?) : string {
        let str_arg = describe(icall.arguments[0])
        let hex = length(icall.arguments) >= 2 ? describe(icall.arguments[1]) : "false"
        return lint017_widened_parse(cast_name, iname, str_arg, hex)
    }

    def lint017_widened_parse(cast_name, iname, str_arg, hex : string) : string {
        let widened = "{iname}64({str_arg}, {hex})"
        let same_sign = cast_name == (iname == "to_int" ? "int64" : "uint64")
        return same_sign ? widened : "{cast_name}({widened})"
    }

    def check_lint017_32bit_cast(expr : ExprCall?) : void {
        if (noLint || genericDepth > 0
            || (expr.name != "int64" && expr.name != "uint64")
            || length(expr.arguments) != 1) return
        let inner = expr.arguments[0]
        if (!(inner is ExprCall)) return
        let icall = inner as ExprCall
        let iname = string(icall.func != null && icall.func.fromGeneric != null
            ? icall.func.fromGeneric.name
            : icall.name)
        let twin = lint017_long_twin(iname, icall)
        if (!empty(twin)) {
            let call = lint017_twin_call(icall)
            let fix = empty(call) ? LintFix() : lint_replace_fix(expr, expr.name == "int64"
                    ? call : "{expr.name}({call})")
            lint_error("LINT017: {expr.name}({iname}(...)) widens an already-32-bit result, so " +
                "the 2^31 limit is still hit inside {iname} (as a wrap, or as the panic guard on " +
                "array/table/string length) before the cast runs; call {twin}(...) instead",
                expr.at, fix)
            return
        }
        if (lint017_is_32bit_parse(iname, icall)) {
            let args <- lint_written_args(icall)
            let fix = empty(args) || length(args) > 2 ? LintFix() : lint_replace_fix(expr,
                lint017_widened_parse(string(expr.name), iname, args[0],
                                      length(args) == 2 ? args[1] : "false"))
            lint_error("LINT017: {expr.name}({iname}(...)) widens a 32-bit string parse - " +
                "{iname} answers 0 for any value that overflows 32 bits, so the cast asks for a " +
                "range the parse cannot give; call " +
                "{lint017_parse_remedy(string(expr.name), iname, icall)} instead", expr.at, fix)
        }
    }

    def lint024_scan(expr : ExpressionPtr; var has_product : bool&; var has_call : bool&) : void {
        if (expr is ExprOp2) {
            let op2 = expr as ExprOp2
            let op = string(op2.op)
            if (op == "*") {
                has_product = true
            }
            if (op == "*" || op == "+" || op == "-") {
                lint024_scan(op2.left, has_product, has_call)
                lint024_scan(op2.right, has_product, has_call)
                return
            }
        }
        if (expr is ExprCall) {
            has_call = true
        }
    }

    //! The edits widening every leaf operand of the sum or product
    //! ``expr``; a line-0 edit marks one with no text
    def lint024_widen(expr : Expression?; wide : string) : array<LintFix> {
        if (expr is ExprOp2) {
            let op2 = expr as ExprOp2
            if (op2.op == "*" || op2.op == "+" || op2.op == "-") {
                var edits <- lint024_widen(op2.left, wide)
                edits |> push_from(lint024_widen(op2.right, wide))
                return <- edits
            }
        }
        let text = lint_expr_text(expr)
        let twin = expr is ExprCall ? lint017_twin_call(expr as ExprCall) : ""
        var leaf = "{wide}({text})"
        if (empty(text)) {
            leaf = ""
        } elif (expr is ExprConstInt && text == "{(expr as ExprConstInt).value}"
                && (wide == "int64" || (expr as ExprConstInt).value >= 0)) {
            leaf = wide == "int64" ? "{text}l" : "{text}ul"
        } elif (expr is ExprConstUInt && wide == "uint64"
                && text == "{(expr as ExprConstUInt).value}u") {
            leaf = "{text}l"
        } elif (!empty(twin)) {
            leaf = wide == "int64" ? twin : "{wide}({twin})"
        }
        return <- [lint_replace_fix(expr, leaf)]
    }

    //! ``int64(a * b)`` rewritten to widen every operand of the 32-bit
    //! sum or product - ``int64(a) * int64(b)`` - parenthesized unless
    //! its parent takes it bare
    def lint024_fix(expr : ExprCall?) : LintFix {
        let wide = string(expr.name)
        let inner = expr.arguments[0]
        var edits <- lint024_widen(inner, wide)
        for (e in edits) {
            return LintFix() if (e.line == 0)
        }
        var head = expr.at
        head.last_line = inner.at.line
        head.last_column = inner.at.column
        var tail = expr.at
        tail.line = inner.at.last_line
        tail.column = inner.at.last_column
        if (strip(lint_span_text(head)) != "{wide}("
            || strip(lint_span_text(tail)) != ")") return LintFix()
        let n = length(lint_expr_stack)
        let parent = n >= 2 ? lint_expr_stack[n - 2] : null
        let top = string((inner as ExprOp2).op)
        var bare = (parent == null || parent is ExprLet || parent is ExprReturn
                || parent is ExprCopy || parent is ExprMove || parent is ExprClone)
        if (!bare && parent is ExprCall) {
            let call = parent as ExprCall
            bare = empty(call.arguments) || call.arguments[0] != expr || !(
                lint_start(call.at) == lint_start(expr.at))
        } elif (!bare && parent is ExprOp2) {
            let op2 = parent as ExprOp2
            let op = string(op2.op)
            bare = (ends_with(op, "=") || op == "<" || op == ">" || op == "&&" || op == "||"
                    || op == "^^"
                || ((op == "+" || op == "-") && (top == "*" || op2.left == expr))
                || (op == "*" && top == "*" && op2.left == expr))
        }
        edits |> push(lint_fix(head, bare ? "" : "("))
        edits |> push(lint_fix(tail, bare ? "" : ")"))
        return lint_merge_fixes(expr.at, edits)
    }

    def check_lint024_widened_product(expr : ExprCall?) : void {
        if (noLint || genericDepth > 0
            || (expr.name != "int64" && expr.name != "uint64")
            || length(expr.arguments) != 1) return
        let inner = expr.arguments[0]
        if (inner._type == null || !(inner is ExprOp2)) return
        let bt = inner._type.baseType
        if (bt != Type.tInt && bt != Type.tUInt) return
        var has_product = false
        var has_call = false
        lint024_scan(inner, has_product, has_call)
        if (!has_product || !has_call) return
        lint_error("LINT024: {expr.name}(...) widens a product that already wrapped - every " +
            "factor is 32-bit, so the multiply overflows at 2^31 before the cast runs; widen the " +
            "factors instead: {expr.name}(a) * {expr.name}(b)", expr.at, lint024_fix(expr))
    }

    def lint018_check_arg(expr : ExprCall?; fname : string; idx : int; isMem : bool) : bool {
        let a = expr.arguments[idx]
        if (!(a is ExprCall)) return false
        let scall = a as ExprCall
        if (scall.name != "int" || length(scall.arguments) != 1) return false
        let src = scall.arguments[0]._type
        if (src == null) return false
        let bt = src.baseType
        if (isMem) {
            if (bt != Type.tInt64 && bt != Type.tUInt64 && bt != Type.tUInt) return false
            lint_error("LINT018: int(...) truncates the {fname} size above 2^31; {fname} has " +
                "uint/int64/uint64 overloads, so drop the cast", expr.at, lint018_fix(scall, ""))
        } elif (bt == Type.tInt64) {
            lint_error("LINT018: int(...) truncates the {fname} argument above 2^31; {fname} has " +
                "an int64 overload, so drop the cast", expr.at, lint018_fix(scall, ""))
        } elif (bt == Type.tUInt64) {
            lint_error("LINT018: int(...) truncates the {fname} argument above 2^31; {fname} has " +
                "an int64 overload - widen with int64(...) instead of narrowing", expr.at,
                lint018_fix(scall, "int64"))
        } else {
            return false
        }
        return true
    }

    def check_lint018_mem_size_narrowing(expr : ExprCall?) : void {
        if (noLint || genericDepth > 0) return
        let fname = string(expr.func != null && expr.func.fromGeneric != null
            ? expr.func.fromGeneric.name
            : expr.name)
        let nargs = length(expr.arguments)
        if ((fname == "memcpy" || fname == "memmove" || fname == "memcmp") && nargs == 3) {
            lint018_check_arg(expr, fname, 2, true)
            return
        }
        if (nargs < 2 || expr.arguments[0]._type == null) return
        let bt = expr.arguments[0]._type.baseType
        if (bt == Type.tArray) {
            if (nargs == 2 && (fname == "resize" || fname == "resize_no_init"
                    || fname == "resize_and_init" || fname == "reserve" || fname == "erase")) {
                lint018_check_arg(expr, fname, 1, false)
            } elif (nargs == 3 && fname == "resize_and_init") {
                lint018_check_arg(expr, fname, 1, false)
            } elif (nargs == 3 && fname == "erase") {
                if (!lint018_check_arg(expr, fname, 1, false)) {
                    lint018_check_arg(expr, fname, 2, false)
                }
            }
        } elif (bt == Type.tTable) {
            if (nargs == 2 && fname == "reserve") {
                lint018_check_arg(expr, fname, 1, false)
            }
        }
    }

    def check_lint020_range_narrowing(expr : ExprCall?) : void {
        if (noLint || genericDepth > 0) return
        let isRange = expr.name == "range"
        let isURange = expr.name == "urange"
        let isInterval = expr.name == "interval"
        if (!isRange && !isURange && !isInterval) return
        let nargs = length(expr.arguments)
        if (nargs < 1 || nargs > 2) return
        for (a in expr.arguments) {
            if (!(a is ExprCall)) { continue; }
            let scall = a as ExprCall
            if ((scall.name != "int" && scall.name != "uint") || length(scall.arguments) != 1) { continue; }
            let src = scall.arguments[0]._type
            if (src == null || (src.baseType != Type.tInt64 && src.baseType != Type.tUInt64)) { continue; }
            let wide = isURange ? Type.tUInt64 : Type.tInt64
            let narrow = isURange ? "uint" : "int"
            if (isInterval) {
                lint_error("LINT020: {scall.name}(...) truncates a 64-bit bound above 2^31; " +
                    "interval has an int64 form, so drop the cast", expr.at, lint020_fix(expr,
                    "interval", narrow, wide))
            } else {
                let twin = isRange ? "range64" : "urange64"
                lint_error("LINT020: {expr.name}({scall.name}(...)) truncates a 64-bit bound " +
                    "above 2^31; use {twin}(...) on the 64-bit values directly - arrays and " +
                    "pointers index with int64/uint64 as-is", expr.at, lint020_fix(expr, twin,
                    narrow, wide))
            }
            return
        }
    }

    def lint021_is_narrow_cast(expr : ExprCall?) : bool {
        if ((expr.name != "int" && expr.name != "uint") || length(expr.arguments) != 1) return false
        let t = expr.arguments[0]._type
        return t != null && (t.baseType == Type.tInt64 || t.baseType == Type.tUInt64)
    }

    def lint021_is_transparent(expr : ExprCall?) : bool {
        let fname = string(expr.func != null && expr.func.fromGeneric != null
            ? expr.func.fromGeneric.name
            : expr.name)
        return fname == "max" || fname == "abs"
    }

    //! the variables `call` writes through a mutable by-reference parameter it writes during the
    //! call - a bare variable in a `var` slot of `&` or `isRefType` type - into `slot` of the frame
    //! that holds `call` as an argument
    def private lint030_note_writes(call : ExprCall const?; slot : int; in_block : bool;
                                    var into : array<Lint030Write>) : void {
        return if (call.func == null)
        for (k in range(min(length(call.arguments), length(call.func.arguments)))) {
            let p = call.func.arguments[k]
            continue if (p._type == null || p._type.flags.constant || !(p._type.flags.ref || p._type.isRefType)
                || !lint030_callee_writes(call.func, k))
            let v = lint030_bare_var(call.arguments[k])
            if (v != null) {
                into |> push(Lint030Write(slot = slot, v = v, callee = "{call.name}",
                                          in_block = in_block))
            }
        }
    }

    //! 0: code at closure depth `depth` runs with a call at
    //! `frame_depth`; 1: in a plain block the call runs; -1: later
    def private lint030_nesting(frame_depth, depth : int) : int {
        return 0 if (depth == frame_depth)
        return 1 if (depth == frame_depth + 1 && back(lint030_plain_closures))
        return -1
    }

    //! whether argument `slot` of `call` is `v` itself, or a plain field of it, handed over by
    //! reference: the callee then sees its final state whatever the order, so that read is not
    //! a dependency
    def private lint030_passed_by_ref(call : ExprCall const?; slot : int; v : Variable const?) : bool {
        return false if (call.func == null || slot >= length(call.func.arguments))
        let p = call.func.arguments[slot]
        return p._type != null && (p._type.flags.ref || p._type.isRefType) && lint030_field_root(call.arguments[slot]) == v
    }

    def private lint030_check(frame : Lint030Frame) : void {
        return if (noLint || genericDepth > 0 || frame.call == null)
        var reported : table<int>
        for (w in frame.writes) {
            continue if (w.in_block || key_exists(reported, w.slot))
            for (m in frame.mentions) {
                continue if (m.in_block || m.slot == w.slot || m.v != w.v
                    || lint030_passed_by_ref(frame.call, m.slot, m.v))
                lint_error("LINT030: argument {m.slot + 1} reads `{m.v.name}` while the call `{w.callee}(...)` in argument {w.slot + 1} writes it by reference - the order arguments are evaluated in is not defined, so the read sees the value before or after the write by the compiler's choice; run the writing call as a statement of its own first", frame.call.at)
                reported |> insert(w.slot)
                break
            }
        }
    }

    def override preVisitExprCallArgument(expr : ExprCall?; arg : ExpressionPtr; _last : bool) : void {
        if (!empty(lint030_frames)) {
            lint030_frames[length(lint030_frames) - 1].slot++
        }
        lint031_note_call_argument(expr, arg)
    }

    def override preVisitExprCall(expr : ExprCall?) : void {
        mut_use_arguments(expr)
        lint034_armed ||= lint034_touches(expr.func)
        lint030_frames |> emplace(Lint030Frame(call = expr, slot = -1, closure_depth = lint021_closure_depth))
        var kind = Lint021CallKind.plain
        if (!noLint && genericDepth == 0) {
            if (lint021_is_narrow_cast(expr)) {
                kind = Lint021CallKind.narrowing_cast
                lint021_narrow_depth++
            } elif (lint021_narrow_depth > 0 && !lint021_is_transparent(expr)) {
                kind = Lint021CallKind.poison
                lint021_poison_depth++
            }
        }
        lint021_call_kind |> push(kind)
        lint022_note(expr.func, "{expr.name}")
        check_string_push_clone(expr)
        check_lint017_32bit_cast(expr)
        check_lint024_widened_product(expr)
        check_lint018_mem_size_narrowing(expr)
        check_lint020_range_narrowing(expr)
    }

    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (!empty(lint030_frames)) {
            var frame <- lint030_frames[length(lint030_frames) - 1]
            lint030_frames |> pop()
            lint030_check(frame)
            if (!empty(lint030_frames)) {
                let parent = length(lint030_frames) - 1
                let slot = lint030_frames[parent].slot
                let nesting = lint030_nesting(lint030_frames[parent].closure_depth,
                                              frame.closure_depth)
                if (slot >= 0 && nesting >= 0) {
                    let in_block = nesting > 0
                    for (m in frame.mentions) {
                        lint030_frames[parent].mentions |> push(Lint030Mention(slot = slot,
                            v = m.v, in_block = in_block))
                    }
                    for (w in frame.writes) {
                        lint030_frames[parent].writes |> push(Lint030Write(slot = slot,
                            v = w.v, callee = w.callee, in_block = in_block))
                    }
                    lint030_note_writes(expr, slot, in_block, lint030_frames[parent].writes)
                }
            }
        }
        if (!empty(lint021_call_kind)) {
            let kind = lint021_call_kind |> back()
            lint021_call_kind |> pop()
            if (kind == Lint021CallKind.narrowing_cast) {
                lint021_narrow_depth--
            } elif (kind == Lint021CallKind.poison) {
                lint021_poison_depth--
            }
        }
        return <- expr
    }

    def lint022_note(fn : Function?; raw_name : string) : void {
        lint022_refs |> note_ref(fn, raw_name)
    }

    def override preVisitStructure(st : StructurePtr) : void {
        lint022_self_struct = st
    }

    def override visitStructure(var st : StructurePtr) : StructurePtr {
        lint022_self_struct = null
        return <- st
    }

    def override preVisitExpression(var expr : ExpressionPtr) : void {
        lint_expr_stack |> push(expr)
        lint022_refs |> note_type(expr._type, 0, lint022_self_struct)
        if (lint022_generic_sweep) {
            lint022_refs |> note_type_names(expr._type, 0)
        }
        if (expr is ExprConstString && !expr.genFlags.generated) {
            let nm = string((expr as ExprConstString).value)
            if (!empty(nm)) {
                lint022_refs.names |> insert(nm)
            }
        }
    }

    def override visitExpression(var expr : ExpressionPtr) : ExpressionPtr {
        if (!empty(lint_expr_stack) && lint_expr_stack |> back() == expr) {
            lint_expr_stack |> pop()
        }
        return expr
    }

    def override preVisitExprAddr(expr : ExprAddr?) : void {
        lint022_note(expr.func, "{expr.target}")
    }

    def override preVisitExprNew(expr : ExprNew?) : void {
        if (lint022_generic_sweep) {
            lint022_refs |> note_type_names(expr.typeexpr, 0)
        }
        lint022_note(expr.func, "{expr.name}")
        mut_use_arguments(expr)
    }

    def override preVisitExprOp1(expr : ExprOp1?) : void {
        lint022_note(expr.func, "{expr.name}")
        mut_use(expr.subexpr) if (expr.op == "++" || expr.op == "--" || expr.op == "+++" || expr.op == "---")
        if (lint030_steps(string(expr.op))) {
            lint031_note_write(expr.subexpr, expr.at)
        }
    }

    def lint022_is_unreferenced(fn : Function?; mod : Module?) : bool {
        return (!lint022_is_skipped(fn, mod)
            && !(lint022_refs |> knows(fn, "{fn.name}")))
    }

    def lint022_collect_generic_refs(mod : Module?) : void {
        let saved_noLint = noLint
        lint022_generic_sweep = true
        make_visitor(self) $(adapter) {
            for_each_generic(mod, "") $(gfn) {
                visit(gfn, adapter)
            }
        }
        lint022_generic_sweep = false
        noLint = saved_noLint
    }

    def lint022_note_decl_types(mod : Module?) : void {
        for_each_function(mod, "") $(fn) {
            return if (fn.flags.generated)
            let self_st = (fn.flags.isClassMethod && !empty(fn.arguments)
                && fn.arguments[0]._type != null) ? fn.arguments[0]._type.structType : null
            for (a in fn.arguments) {
                lint022_refs |> note_type(a._type, 0, self_st)
            }
            lint022_refs |> note_type(fn.result, 0, self_st)
        }
        for_each_generic(mod, "") $(gfn) {
            for (a in gfn.arguments) {
                lint022_refs |> note_type(a._type, 0)
            }
            lint022_refs |> note_type(gfn.result, 0)
        }
        for_each_structure(mod) $(st) {
            if (st.parent != null) {
                lint022_refs.structs |> insert(st.parent)
            }
            for (f in st.fields) {
                lint022_refs |> note_type(f._type, 0, st)
            }
        }
        for_each_global(mod) $(v) {
            lint022_refs |> note_type(v._type, 0)
        }
        for_each_typedef(mod) $(_name, td) {
            lint022_refs |> note_type(td, 0)
        }
    }

    def lint022_struct_unreferenced(st : Structure?; mod : Module?) : bool {
        return (st != null && st.flags.privateStructure && !st.flags.generated
            && length(st.annotations) == 0
            && st.at.fileInfo != null && st._module == mod
            && !key_exists(lint022_refs.structs, st)
            && !key_exists(lint022_refs.names, "{st.name}"))
    }

    def lint022_enum_unreferenced(en : Enumeration?; mod : Module?) : bool {
        return (en != null && en.isPrivate
            && length(en.annotations) == 0
            && en.at.fileInfo != null && en._module == mod
            && !key_exists(lint022_refs.enums, en)
            && !key_exists(lint022_refs.names, "{en.name}"))
    }

    def lint022_has_candidate(prog : ProgramPtr; mod : Module?; check_functions : bool) : bool {
        var any = false
        if (check_functions) {
            for_each_function(mod, "") $(fn) {
                if (lint022_is_unreferenced(fn, mod)) {
                    any = true
                }
            }
        }
        for_each_structure(mod) $(st) {
            if (lint022_struct_unreferenced(st, mod)) {
                any = true
            }
        }
        for_each_enumeration(mod) $(en) {
            if (lint022_enum_unreferenced(en, mod)) {
                any = true
            }
        }
        return any
    }

    def lint022_check_unused_private(prog : ProgramPtr) : void {
        return if (key_exists(disabled_codes, "LINT022")
                    || (!empty(enabled_codes) && !key_exists(enabled_codes, "LINT022"))
                    || !lint022_enabled(prog))
        let mod = prog.getThisModule
        return if (mod == null)
        let check_functions = !lint022_calls_may_be_inlined(prog)
        let skip_block_functions = lint022_block_calls_may_be_inlined(prog)
        lint022_note_decl_types(mod)
        return if (!lint022_has_candidate(prog, mod, check_functions))
        lint022_collect_generic_refs(mod)
        if (check_functions) {
            for_each_function(mod, "") $(fn) {
                return if (!lint022_is_unreferenced(fn, mod) || (skip_block_functions && lint022_takes_block(fn)))
                let close = fn.body != null && empty(strip(slice(lint_source_line(fn.at,
                    int(fn.body.at.last_line)),
                    int(fn.body.at.last_column)))) ? int(fn.body.at.last_line) : 0
                lint_report_at_decl("LINT022: private function '{fn.name}' is never used; " +
                    "remove it, or make it public if another module needs it", fn.at,
                    lint022_fix(fn.at, "def", close))
            }
        }
        for_each_structure(mod) $(st) {
            return if (!lint022_struct_unreferenced(st, mod))
            let kind = st.flags.isClass ? "class" : "structure"
            lint_report_at_decl("LINT022: private {kind} '{st.name}' is never used; remove " +
                "it, or make it public if another module needs it", st.at,
                lint022_fix(st.at, kind == "class" ? "class" : "struct",
                            lint022_struct_close(st.at,
                                                 [for (f in st.fields); int(f.at.last_line);
                                                  where !f.flags.parentType])))
        }
        for_each_enumeration(mod) $(en) {
            return if (!lint022_enum_unreferenced(en, mod))
            lint_report_at_decl("LINT022: private enumeration '{en.name}' is never used; " +
                "remove it, or make it public if another module needs it", en.at,
                lint022_fix(en.at, "enum",
                            lint022_struct_close(en.at, [for (e in en.list); int(e.at.last_line)])))
        }
    }

    def lint035_report(what : string; at : LineInfo) : void {
        lint_report_at_decl("LINT035: @pool `{what}` can hold raw pointers, and nothing in " +
            "this module empties it. A pool owns what it holds, so its module carries the " +
            "drain the shutdown or drop path runs: `clear`, `resize(.., 0)` or `delete` on " +
            "the container, or a `delete` of the value that holds it. Add that drain, or mark " +
            "the container @scratch if its contents are dead when the function that fills it " +
            "returns.", at, LintFix())
    }

    def lint035_check_pool_drains(prog : ProgramPtr) : void {
        return if (key_exists(disabled_codes, "LINT035")
                    || (!empty(enabled_codes) && !key_exists(enabled_codes, "LINT035")))
        let mod = prog.getThisModule
        return if (mod == null)
        var any = false
        lint035_each_pool(mod) $(_what, _key; _at) {
            any = true
        }
        return if (!any)
        var drained <- lint035_drained(mod)
        lint035_each_pool(mod) $(what, key; at) {
            lint035_report(what, at) if (!key_exists(drained, key))
        }
        delete drained
    }

    def lint_report_at_decl(msg : string; at : LineInfo; fix : LintFix) : void {
        //! Reports at a declaration no function walk visited: retargets ``funcFile`` so ``lint_error``'s own-file gate admits a symbol from any file of the module.
        funcFile = at.fileInfo != null ? "{at.fileInfo.name}" : ""
        lint_error(msg, at, fix)
    }

    def override canVisitWithAliasSubexpression(_expr : ExprAssume?) : bool => false

    def override preVisitExprLet(expr : ExprLet?) : void {
        for (v in expr.variables) {
            validate_var(v, true)
        }
    }

    def is_const_zero(expr : ExpressionPtr) : bool {
        if (expr == null) return false
        if (expr is ExprConstInt) return (expr as ExprConstInt).value == 0
        if (expr is ExprConstUInt) return (expr as ExprConstUInt).value == 0u
        if (expr is ExprConstInt64) return (expr as ExprConstInt64).value == 0l
        if (expr is ExprConstUInt64) return (expr as ExprConstUInt64).value == 0ul
        if (expr is ExprConstFloat) return (expr as ExprConstFloat).value == 0.0f
        if (expr is ExprConstDouble) return (expr as ExprConstDouble).value == 0.0lf
        return false
    }

    def expr_equal(a : ExpressionPtr; b : ExpressionPtr; require_pure : bool = true) : bool {
        if (a == null || b == null
            || (require_pure && (!a.flags.noSideEffects || !b.flags.noSideEffects))) return false
        return describe(a) == describe(b)
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        lint022_note(expr.func, "{expr.name}")
        let op = string(expr.op)
        mut_use(expr.left) if (ends_with(op, "=") && op != "==" && op != "!=" && op != "<=" && op != ">=")
        if (noLint || genericDepth > 0) return
        if (length(op) >= 2 && op |> ends_with("=") && op != "==" && op != "!=" && op != "<=" && op != ">=") {
            lint023_mark_store_target(expr.left)
            lint031_note_write(expr.left, expr.at)
        }
        if ((op == "/" || op == "%" || op == "/=" || op == "%=") && is_const_zero(expr.right)) {
            lint_error("LINT006: division by zero ({op})", expr.at)
            return
        }
        let op_same_suspicious = (
            op == "==" || op == "!=" ||
            op == "<"  || op == ">"  ||
            op == "<=" || op == ">=" ||
            op == "-"  || op == "/"  || op == "%" ||
            op == "&&" || op == "||" ||
            op == "&"  || op == "|"  || op == "^" ||
            op == "-=" || op == "/=" || op == "%="
        )
        if (op_same_suspicious && expr_equal(expr.left, expr.right, false)
                && !(expr.left.__rtti |> starts_with("ExprConst"))) {
            lint_error("LINT007: left and right operands of '{op}' are the same", expr.at)
        }
    }

    def override preVisitExprOp3(expr : ExprOp3?) : void {
        lint022_note(expr.func, "{expr.name}")
        if (noLint || genericDepth > 0) return
        if (expr_equal(expr.left, expr.right, false)) {
            lint_error("LINT008: both branches of ternary '?:' are equivalent", expr.at,
                       lint008_fix(expr))
        }
    }

    def override preVisitExprIfThenElse(expr : ExprIfThenElse?) : void {
        if (noLint || genericDepth > 0) return
        if (expr.if_false != null && expr_equal(expr.if_true, expr.if_false, false)) {
            lint_error("LINT009: 'then' branch is equivalent to 'else' branch", expr.at,
                       lint009_fix(expr))
        }
        branch_depth++
    }
    def override visitExprIfThenElse(var expr : ExprIfThenElse?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) {
            branch_depth--
        }
        return <- expr
    }

    //! stands LINT010 and LINT023 down for the rest of this function (ARCHITECTURE_LINT.md, LINT023)
    def private note_flow_jump() : void {
        if (noLint || genericDepth > 0) return
        flow_jumps = true
        for (e in ds_entries) {
            e.skip = true
        }
    }

    def private ds_find(v : VariablePtr) : int {
        let vc : Variable const? = v
        for (i in range(length(ds_entries))) {
            if (ds_entries[i].v == vc) return i
        }
        return -1
    }

    def private lint021_find(v : VariablePtr) : int {
        let vc : Variable const? = v
        for (i in range(length(lint021_candidates))) {
            if (lint021_candidates[i].v == vc) return i
        }
        return -1
    }

    def override preVisitExprLetVariable(_let : ExprLet?; v : VariablePtr; _last : bool) : void {
        if (lint022_generic_sweep) {
            lint022_refs |> note_type_names(v._type, 0)
        }
        check_string_clone_init(v)
        mut_use(v.init) if (v._type != null && !v._type.flags.constant)
        if (noLint || genericDepth > 0) return
        if (v._type != null && !v._type.flags.ref && v.init != null && !v.flags.generated
                && (v._type.baseType == Type.tInt64 || v._type.baseType == Type.tUInt64)) {
            lint021_candidates |> emplace(Lint021Candidate(v = v, safe_uses = 0, disqualified = false))
        }
        if ((v._type != null && v._type.flags.ref)
            || v.flags.inScope
            || v.access_flags.access_fold
            || !v.access_flags.access_get) return
        let has_init = v.init != null
        let init_pure = has_init && v.init.flags.noSideEffects
        ds_entries |> emplace(DeadStoreEntry(
            v = v,
            pending_expr = v.init,
            skip = flow_jumps,
            has_pending = has_init,
            pending_pure = init_pure
        ))
    }

    def override preVisitGlobalLetVariable(v : VariablePtr; _last : bool) : void {
        check_string_clone_init(v)
    }

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (lint022_generic_sweep) {
            lint022_refs.names |> insert("{expr.name}")
        }
        if (lint004_deferred |> find_index(expr.variable) >= 0) {
            let name = string(expr.name)
            let fix = expr.genFlags.generated ? LintFix() : lint_token_fix(expr.at,
                int(expr.at.line), int(expr.at.column), name, slice(name, 1))
            lint004_uses |> push(Lint004Use(v = expr.variable, fix = fix))
        }
        if (!empty(lint030_frames)) {
            let top = length(lint030_frames) - 1
            let nesting = lint030_nesting(lint030_frames[top].closure_depth, lint021_closure_depth)
            if (lint030_frames[top].slot >= 0 && nesting >= 0) {
                lint030_frames[top].mentions |> push(Lint030Mention(
                    slot = lint030_frames[top].slot, v = expr.variable, in_block = nesting > 0))
            }
        }
        if (noLint || genericDepth > 0) return
        lint034_armed ||= lint034_names_scratch_global(expr)
        if (key_exists(lint023_store_nodes, intptr(expr))) {
            lint029_note_mutation(expr.variable)
        }
        lint023_note_use(expr)
        let ci = lint021_find(expr.variable)
        if (ci >= 0 && !lint021_candidates[ci].disqualified) {
            if (expr.varFlags.write || lint021_closure_depth > 0
                    || lint021_narrow_depth == 0 || lint021_poison_depth > 0) {
                lint021_candidates[ci].disqualified = true
            } else {
                lint021_candidates[ci].safe_uses++
            }
        }
        let i = ds_find(expr.variable)
        if (i < 0 || ds_entries[i].skip) return
        let vf = expr.varFlags
        if (branch_depth > 0 || (vf.write && !vf.under_clone)) {
            ds_entries[i].skip = true
            return
        }
        if (vf.write) return
        ds_entries[i].has_pending = false
    }

    def private ds_handle_store(lhs : ExpressionPtr; rhs : ExpressionPtr; var store_expr : ExpressionPtr) : void {
        if (lhs == null || !(lhs is ExprVar)) return
        let ev = lhs as ExprVar
        let i = ds_find(ev.variable)
        if (i < 0 || ds_entries[i].skip || branch_depth > 0) return
        if (ds_entries[i].has_pending && ds_entries[i].pending_pure && ds_entries[i].pending_expr != null) {
            let vname = "{ev.variable.name}"
            lint_error("LINT010: dead store of '{vname}': overwritten without intervening read", ds_entries[i].pending_expr.at)
        }
        ds_entries[i].has_pending = true
        ds_entries[i].pending_expr = store_expr
        ds_entries[i].pending_pure = rhs != null && rhs.flags.noSideEffects
    }

    def override preVisitExprCopy(expr : ExprCopy?) : void {
        mut_use(expr.left)
        mut_use(expr.right)
        lint023_mark_store_target(expr.left)
        lint031_note_write(expr.left, expr.at)
    }
    def override preVisitExprClone(expr : ExprClone?) : void {
        mut_use(expr.left)
        mut_use(expr.right)
        lint023_mark_store_target(expr.left)
        lint031_note_write(expr.left, expr.at)
    }
    def override preVisitExprMove(expr : ExprMove?) : void {
        mut_use(expr.left)
        mut_use(expr.right)
        lint023_mark_store_target(expr.left)
        lint031_note_write(expr.left, expr.at)
    }
    def override preVisitExprInvoke(expr : ExprInvoke?) : void {
        return if (length(expr.arguments) == 0 || expr.arguments[0]._type == null)
        let bt = expr.arguments[0]._type
        for (a, i in expr.arguments, count()) {
            mut_use(a) if (i > 0 && i - 1 < length(bt.argTypes) && lint_mutable_slot(bt.argTypes[i - 1]))
        }
    }
    def override preVisitExprDelete(expr : ExprDelete?) : void {
        mut_use(expr.subexpr)
    }
    def override preVisitExprAt(expr : ExprAt?) : void {
        mut_use(expr.subexpr) if (expr.subexpr._type != null && expr.subexpr._type.isGoodTableType)
    }
    def override preVisitExprMakeArray(expr : ExprMakeArray?) : void {
        for (v in expr.values) {
            mut_use(v)
        }
    }
    def override preVisitExprMakeTuple(expr : ExprMakeTuple?) : void {
        for (v in expr.values) {
            mut_use(v)
        }
    }
    def override preVisitExprMakeStruct(expr : ExprMakeStruct?) : void {
        for (st in expr.structs) {
            for (mf in *st) {
                mut_use(mf.value)
            }
        }
    }
    def override preVisitExprMakeVariant(expr : ExprMakeVariant?) : void {
        for (mf in expr.variants) {
            mut_use(mf.value)
        }
    }
    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        check_string_clone_assign(expr)
        ds_handle_store(expr.left, expr.right, expr)
        return <- expr
    }
    def override visitExprClone(var expr : ExprClone?) : ExpressionPtr {
        ds_handle_store(expr.left, expr.right, expr)
        return <- expr
    }
    def override visitExprMove(var expr : ExprMove?) : ExpressionPtr {
        ds_handle_store(expr.left, expr.right, expr)
        return <- expr
    }

    def override preVisitExprGoto(_expr : ExprGoto?) : void {
        note_flow_jump()
    }
    def override preVisitExprFor(_expr : ExprFor?) : void {
        if (!noLint && genericDepth == 0) {
            branch_depth++
            lint023_deferred_depth++
        }
    }
    def override visitExprFor(var expr : ExprFor?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) {
            branch_depth--
            lint023_deferred_depth--
        }
        return <- expr
    }
    def override preVisitExprWhile(_expr : ExprWhile?) : void {
        if (!noLint && genericDepth == 0) {
            branch_depth++
            lint023_deferred_depth++
        }
    }
    def override visitExprWhile(var expr : ExprWhile?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) {
            branch_depth--
            lint023_deferred_depth--
        }
        return <- expr
    }
    def override preVisitExprTryCatch(_expr : ExprTryCatch?) : void {
        if (!noLint && genericDepth == 0) branch_depth++
    }
    def override visitExprTryCatch(var expr : ExprTryCatch?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) branch_depth--
        return <- expr
    }
    def override visitExprConstFloat(var expr : ExprConstFloat?) : ExpressionPtr {
        if (!noLint && genericDepth == 0 && expr.inexactFloatPromotion) {
            lint_error("LINT011: int literal does not exactly represent as float", expr.at)
        }
        return <- expr
    }
    def override visitExprConstDouble(var expr : ExprConstDouble?) : ExpressionPtr {
        if (!noLint && genericDepth == 0 && expr.inexactFloatPromotion) {
            lint_error("LINT011: int literal does not exactly represent as double", expr.at)
        }
        return <- expr
    }
}

def public paranoid(prog : ProgramPtr; compile_time_errors : bool) {
    //! Runs the paranoid lint visitor on the program to check for common coding issues.
    let empty_set : table<string>
    paranoid(prog, compile_time_errors, empty_set)
}

def public paranoid(prog : ProgramPtr; compile_time_errors : bool; disabled_codes : table<string>) {
    //! Filter-aware compile-time variant. ``disabled_codes`` suppresses
    //! the listed rule IDs (e.g. "LINT003") alongside the usual
    //! ``// nolint:CODE`` directives.
    var astVisitor = new LintVisitor(
        compile_time_errors = compile_time_errors,
        lint029_exempt = lint029_source_exempt(prog),
        multiple_contexts = prog._options |> find_arg("multiple_contexts") ?as tBool ?? prog.policies.multiple_contexts)
    astVisitor.disabled_codes := disabled_codes
    make_visitor(*astVisitor) $(adapter) {
        visit(prog, adapter)
    }
    astVisitor->lint022_check_unused_private(prog)
    astVisitor->lint035_check_pool_drains(prog)
    unsafe {
        delete astVisitor
    }
}

def public paranoid_collect(prog : ProgramPtr; var errors : array<string>) : int {
    //! Runs the paranoid lint visitor and collects errors as strings.
    //! Returns the number of lint issues found.
    let empty_set : table<string>
    return paranoid_collect(prog, errors, empty_set, empty_set)
}

def public paranoid_collect(prog : ProgramPtr; var errors : array<string>;
                            disabled_codes, enabled_codes : table<string>) : int {
    //! Filter-aware overload. `disabled_codes` is a denylist; `enabled_codes`
    //! is a whitelist (empty == all). Codes use the bare form (e.g. "LINT002").
    var astVisitor = new LintVisitor(
        compile_time_errors = false,
        collect_errors = true,
        lint029_exempt = lint029_source_exempt(prog),
        multiple_contexts = prog._options |> find_arg("multiple_contexts") ?as tBool ?? prog.policies.multiple_contexts)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    make_visitor(*astVisitor) $(adapter) {
        visit(prog, adapter)
    }
    astVisitor->lint022_check_unused_private(prog)
    astVisitor->lint035_check_pool_drains(prog)
    let count = length(astVisitor.errors)
    errors |> push_from(astVisitor.errors)
    unsafe {
        delete astVisitor
    }
    return count
}

def public paranoid_collect_issues(prog : ProgramPtr; var issues : array<LintIssue>;
                                   disabled_codes, enabled_codes : table<string>) : int {
    //! Structured twin of ``paranoid_collect`` — same filters, but emits
    //! ``LintIssue`` records (rule code + position) instead of display strings.
    var astVisitor = new LintVisitor(
        compile_time_errors = false,
        collect_errors = true,
        lint029_exempt = lint029_source_exempt(prog),
        multiple_contexts = prog._options |> find_arg("multiple_contexts") ?as tBool ?? prog.policies.multiple_contexts)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    make_visitor(*astVisitor) $(adapter) {
        visit(prog, adapter)
    }
    astVisitor->lint022_check_unused_private(prog)
    astVisitor->lint035_check_pool_drains(prog)
    let count = length(astVisitor.issues)
    issues |> push_from(astVisitor.issues)
    unsafe {
        delete astVisitor
    }
    return count
}

def private is_nolint_space(b : uint8) : bool => int(b) == ' ' || int(b) == '\t'

def private comment_start_outside_literals(d : array<uint8> const# implicit) : int {
    let n = length(d)
    var p = 0
    while (p < n) {
        let c = int(d[p])
        if (c == '"' || c == '\'') {
            p++
            while (p < n && int(d[p]) != c) {
                p += (int(d[p]) == '\\' && p + 1 < n) ? 2 : 1
            }
            p++
        } elif (c == '/' && p + 1 < n && int(d[p + 1]) == '/') {
            return p
        } else {
            p++
        }
    }
    return -1
}

def private stale_scan_line(bytes : array<uint8> const# implicit; fname : string; line_no : int;
                            disabled_codes, enabled_codes : table<string>;
                            var stale_codes : array<string>; var self_suppressed : bool&) : void {
    let n = length(bytes)
    var p = comment_start_outside_literals(bytes)
    return if (p < 0)
    p += 2
    while (p < n && (int(bytes[p]) == '/' || int(bytes[p]) == '!')) {
        p++
    }
    while (p < n && is_nolint_space(bytes[p])) {
        p++
    }
    return if (find(bytes, "nolint:", p) != p)
    while (p < n) {
        while (p < n && is_nolint_space(bytes[p])) {
            p++
        }
        break if (p >= n)
        if (find(bytes, "nolint:", p) == p) {
            p += length("nolint:")
            while (p < n) {
                while (p < n && is_nolint_space(bytes[p])) {
                    p++
                }
                let tok_start = p
                while (p < n && int(bytes[p]) != ',' && !is_nolint_space(bytes[p])) {
                    p++
                }
                break if (p == tok_start)
                let code = slice(bytes, tok_start, p)
                if (code == "LINT019") {
                    self_suppressed = true
                } elif (is_rule_code(code)
                        && !key_exists(disabled_codes, code)
                        && (empty(enabled_codes) || key_exists(enabled_codes, code))
                        && !was_nolint_consumed(fname, line_no, code)) {
                    stale_codes |> push(code)
                }
                while (p < n && is_nolint_space(bytes[p])) {
                    p++
                }
                break if (p >= n || int(bytes[p]) != ',')
                p++
            }
        } else {
            while (p < n && !is_nolint_space(bytes[p])) {
                p++
            }
        }
    }
}

def private uninstanced_generic_lines(prog : ProgramPtr; fname : string) : array<int2> {
    let mod = prog.getThisModule
    var instanced : table<int>
    for_each_function(mod, "") $(fn) {
        let root = root_generic(fn.fromGeneric)
        if (root != null && root._module == mod) {
            instanced |> insert(int(root.at.line))
        }
    }
    var lines : array<int2>
    for_each_generic(mod, "") $(g) {
        if (g.body != null && g.at.fileInfo != null && g.at.fileInfo.name == fname && !key_exists(instanced, int(g.at.line))) {
            lines |> push(int2(int(g.at.line), int(g.body.at.last_line)))
        }
    }
    return <- lines
}

//! The edit for a stale ``nolint:``: rewrite its list without ``stale``, or strip the comment once nothing is left
def private lint019_fix(line : string; line_no : int; stale : array<string>) : LintFix {
    var comment = -1
    peek_data(line) $(bytes) {
        comment = comment_start_outside_literals(bytes)
    }
    let at = comment < 0 ? -1 : find(line, "nolint:", comment)
    return LintFix() if (at < 0)
    let list_end = at + 7 + length(split(slice(line, at + 7), " ")[0])
    return LintFix() if (list_end == at + 7)
    let kept <- [for (code in split(slice(line, at + 7, list_end), ",")); code; where find_index(stale, code) < 0]
    return LintFix(line = line_no, column = at + 7, last_line = line_no, last_column = list_end, replacement = join(kept, ",")) if (!empty(kept))
    let code_end = length(strip_right(slice(line, 0, comment)))
    return LintFix(line = line_no, column = code_end, last_line = line_no, last_column = length(line), replacement = "")
}

//! LINT019 - stale nolint detection: reports every leading ``// nolint:CODE`` directive in
//! `source` (recognition rule: stale_scan_line) whose code suppressed nothing this process.
//! Call AFTER all lint passes ran; a partial run under-consumes and would lie.
def public stale_nolint_collect(prog : ProgramPtr; fname, source : string; disabled_codes, enabled_codes : table<string>; var issues : array<LintIssue>) : int {
    return 0 if (key_exists(disabled_codes, "LINT019")
                    || (!empty(enabled_codes) && !key_exists(enabled_codes, "LINT019")))
    var count = 0
    var line_no = 0
    var stale_codes : array<string>
    stale_codes |> reserve(4)
    let skipped <- uninstanced_generic_lines(prog, fname)
    for (raw_line in split(source, "\n")) {
        line_no++
        let line = raw_line |> ends_with("\r") ? slice(raw_line, 0, length(raw_line) - 1) : raw_line
        continue if (find(line, "nolint:") < 0 || find_index_if(skipped, $(r) => line_no >= r.x && line_no <= r.y) >= 0)
        stale_codes |> clear()
        var self_suppressed = false
        peek_data(line) $(bytes) {
            stale_scan_line(bytes, fname, line_no, disabled_codes, enabled_codes,
                stale_codes, self_suppressed)
        }
        if (!self_suppressed) {
            issues |> reserve(length(issues) + length(stale_codes))
            let fix = lint019_fix(line, line_no, stale_codes)
            for (code, k in stale_codes, count()) {
                let msg = "LINT019: stale nolint - {code} suppresses nothing on this line; remove it, or add LINT019 to the list if the rule fires only in downstream compiles (generic bodies - instantiations elsewhere fire at the generic's line - macro-template lines, option-gated rules)"
                issues |> push(
                               LintIssue(code = "LINT019",
                                         message = msg,
                    file = fname,
                    line = line_no, column = 0, last_line = line_no, last_column = 0,
                    fix = k == 0 ? fix : LintFix()))
                count++
            }
        }
    }
    return count
}

def private is_rule_code(code : string) : bool {
    var prefix_len = 0
    if (code |> starts_with("LINT") || code |> starts_with("PERF")) {
        prefix_len = 4
    } elif (code |> starts_with("STYLE")) {
        prefix_len = 5
    } else {
        return false
    }
    var digits = 0
    var ok = true
    peek_data(code) $(bytes) {
        let n = length(bytes)
        for (i in range(prefix_len, n)) {
            if (!is_number(int(bytes[i]))) {
                ok = false
            }
            digits++
        }
    }
    return ok && digits > 0
}
