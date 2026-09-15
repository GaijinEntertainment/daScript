options gen2
options indenting = 4
options strict_smart_pointers = true

//! ast_cursor provides position-based AST queries — given a file, line, and column,
//! it finds the expression nodes at that cursor position, and on request the variable
//! declarations. Results are returned innermost-first, with enclosing function context
//! and extracted names.
//!
//! Typical use is implementing IDE features like goto-definition, type-of, and
//! find-references on top of the compiled AST.

module ast_cursor shared public

require daslib/ast_boost
require daslib/utf8_utils
require strings

//! A single hit at the cursor position — one AST expression node, or a variable declaration when asked for.
//! Innermost (most specific) hits come first in the result array.
struct public CursorHit {
    //! Raw pointer to the expression node (valid while the program lives); null for a variable hit.
    expr : Expression const?
    //! The variable whose declaration the cursor is on (`for`/`let` variable, function, lambda or block argument) when declarations were asked for; else null.
    variable : Variable const?
    //! The type the cursor's type name denotes (a struct, enum or alias spelled in the source) when declarations were asked for; else null.
    type_decl : TypeDecl const?
    //! Enclosing function, or null for expressions in global scope.
    func : Function const?
    //! Extracted name for named nodes (variable name, call name, field name, operator); the type's name for a type hit.
    name : string
    //! Expression RTTI type string: "ExprVar", "ExprCall", "ExprField", etc.; "Variable" or "TypeDecl" for the other hits.
    rtti : string
}

//! the position a hit's node carries
def public hit_at(hit : CursorHit) : LineInfo {
    if (hit.expr != null) return hit.expr.at
    if (hit.variable != null) return hit.variable.at
    return hit.type_decl.at
}

//! Check whether a point (line, col) is inside the span described by `at`.
//! Follows the same logic as C++ ``LineInfo::inside``.
def public cursor_inside(line, col : int; at : LineInfo) : bool {
    if (int(at.line) == 0 && int(at.last_line) == 0) return false
    let tl = int(at.line)
    let tc = int(at.column)
    let tll = int(at.last_line)
    let tlc = int(at.last_column)
    if (line < tl || line > tll) return false
    if (line == tl && line == tll) return col >= tc && col < tlc
    if (line == tl) return col >= tc
    if (line == tll) return col < tlc
    return true
}

def private expression_name(expr : ExpressionPtr) : string {
    if (expr is ExprVar) return string((expr as ExprVar).name)
    if (expr is ExprCall) return string((expr as ExprCall).name)
    if (expr is ExprField) return string((expr as ExprField).name)
    if (expr is ExprSafeField) return string((expr as ExprSafeField).name)
    if (expr is ExprOp1) return string((expr as ExprOp1).op)
    if (expr is ExprOp2) return string((expr as ExprOp2).op)
    if (expr is ExprOp3) return "?"
    if (expr is ExprLooksLikeCall) return string((expr as ExprLooksLikeCall).name)
    if (expr is ExprNamedCall) return string((expr as ExprNamedCall).name)
    return ""
}

//! A compiler-synthesized member - `Foo'__finalize`, `Foo'Foo` and the like - sits on the `class`
//! line with no source of its own; user identifiers never contain the apostrophe. A lambda or
//! generator body is `generated` too and is the user's own source; a generic instance keeps its origin.
def public is_synthesized(func : Function const?) : bool {
    return false if (func == null)
    return true if (func.flags.generated && !func.flags._lambda && !func.flags._generator && func.fromGeneric == null)
    var has_tick = false
    peek(func.name) $(n) {
        has_tick = find(n, "'") >= 0
    }
    return has_tick
}

//! The name the source spells: a generator's loop variable becomes a state field the compiler
//! names `__<name>_rename_at_<line>_<column>`.
def public source_name(name : string) : string {
    return name if (!(name |> starts_with("__")))
    let at = find(name, "_rename_at_")
    return at > 2 ? slice(name, 2, at) : name
}

def private is_ident_byte(c : uint8) : bool {
    return utf32_is_word_char(uint(c))
}

//! The identifier `col` (0-based byte column) sits on in `line_text`, and the column it starts at;
//! an empty word when the cursor is not on one.
def public word_at(line_text : string; col : int) : tuple<word : string; start : int> {
    var word = ""
    var start = -1
    peek_data(line_text) $(d) {
        let n = length(d)
        return if (col < 0 || col > n)
        var b = col
        while (b > 0 && is_ident_byte(d[b - 1])) {
            b--
        }
        var e = col
        while (e < n && is_ident_byte(d[e])) {
            e++
        }
        start = b
        word = slice(d, b, e)
    }
    return (word = word, start = start)
}

//! The 0-based byte column where `name` stands as a whole word in `line_text`, `hint` when it does
//! not: a `Structure`'s own position is one column before its name.
def public name_column(line_text : string; name : string; hint : int) : int {
    var column = hint
    peek_data(line_text) $(d) {
        let n = length(d)
        let w = length(name)
        var at = find(d, name)
        while (at >= 0) {
            let before = at == 0 || !is_ident_byte(d[at - 1])
            let after = at + w >= n || !is_ident_byte(d[at + w])
            if (before && after) {
                column = at
                return
            }
            at = find(d, name, at + 1)
        }
    }
    return column
}

//! True when `line_text` spells `name` as a whole word starting exactly at `col`.
def public spells_name(line_text : string; col : int; name : string) : bool {
    let w = word_at(line_text, col)
    return w.word == name && w.start == col
}

//! What `find_declaration_at_cursor` found: which of `DeclarationHit`'s pointers is set.
enum public DeclarationKind {
    none
    func
    structure
    enumeration
    alias
    variable
}

//! The declaration whose name the cursor is on: at most one of the pointers is set, by `kind`.
struct public DeclarationHit {
    //! which pointer below is set; `none` when the cursor is on no declaration
    kind : DeclarationKind
    //! the function or generic, for `func`
    func : Function const?
    //! the structure or class, for `structure`
    structure : Structure const?
    //! the enumeration, for `enumeration`
    enumeration : Enumeration const?
    //! the global variable, for `variable`
    variable : Variable const?
    //! the declared name; the only handle for `alias`
    name : string
    //! the declaration's own position
    @safe_when_uninitialized at : LineInfo
    //! 0-based byte column where the cursor's word starts on the cursor line; -1 for a line-only lookup
    column : int
}

def private on_line(at : LineInfo; file : string; line : int) : bool {
    return false if (int(at.line) != line)
    return true if (empty(file) || at.fileInfo == null)
    return find(string(at.fileInfo.name), file) >= 0
}

//! A declaration's name matches when the lookup is line-only (empty `word`), or when the name is the
//! cursor's word and the line spells it starting at the cursor's column - a second spelling of the
//! same name on the line (a self-call, a parameter named after the function) is not the declaration.
def private on_name(word : string; name : string; line_text : string; here : int) : bool {
    return true if (empty(word))
    return name == word && here == name_column(line_text, name, -1)
}

//! Find the declaration whose name the cursor is on - a function or generic, a structure or the parent
//! named on its `class` line, an enumeration, a typedef, a global - which `find_at_cursor` never yields.
//! `line_text` is the source line, "" for a line-only lookup; `file` is a substring of ``FileInfo.name``.
def public find_declaration_at_cursor(program : smart_ptr<Program>; file : string; line, col : int; line_text : string) : DeclarationHit {
    var hit = DeclarationHit()
    let w = word_at(line_text, col)
    let word = w.word
    let here = w.start
    let thisMod = get_this_module(program)
    ast::for_each_function(thisMod, "") $(var func : FunctionPtr) {
        return if (hit.kind != DeclarationKind.none)
        let f = func
        return if (is_synthesized(f) || !on_line(f.at, file, line) || !on_name(word, bare_name(string(f.name)), line_text, here))
        hit = DeclarationHit(kind = DeclarationKind.func, func = f, name = bare_name(string(f.name)), at = f.at, column = here)
    }
    if (hit.kind == DeclarationKind.none) {
        ast::for_each_generic(thisMod) $(var func : FunctionPtr) {
            return if (hit.kind != DeclarationKind.none)
            let f = func
            return if (is_synthesized(f) || !on_line(f.at, file, line) || !on_name(word, bare_name(string(f.name)), line_text, here))
            hit = DeclarationHit(kind = DeclarationKind.func, func = f, name = bare_name(string(f.name)), at = f.at, column = here)
        }
    }
    if (hit.kind == DeclarationKind.none) {
        ast::for_each_structure(thisMod) $(var st : StructurePtr) {
            return if (hit.kind != DeclarationKind.none)
            let s = st
            return if (!on_line(s.at, file, line))
            if (on_name(word, string(s.name), line_text, here)) {
                hit = DeclarationHit(kind = DeclarationKind.structure, structure = s, name = string(s.name), at = s.at, column = here)
            } elif (!empty(word) && s.parent != null && s.parent.name == word && here == name_column(line_text, word, -1)) {
                let p = s.parent
                hit = DeclarationHit(kind = DeclarationKind.structure, structure = p, name = word, at = p.at, column = here)
            }
        }
    }
    if (hit.kind == DeclarationKind.none) {
        ast::for_each_enumeration(thisMod) $(var en : EnumerationPtr) {
            return if (hit.kind != DeclarationKind.none)
            let e = en
            return if (!on_line(e.at, file, line) || !on_name(word, string(e.name), line_text, here))
            hit = DeclarationHit(kind = DeclarationKind.enumeration, enumeration = e, name = string(e.name), at = e.at, column = here)
        }
    }
    if (hit.kind == DeclarationKind.none) {
        ast::for_each_typedef(thisMod) $(name : string#; var value : TypeDeclPtr) {
            return if (hit.kind != DeclarationKind.none)
            let td = value
            return if (!on_line(td.at, file, line) || !on_name(word, string(name), line_text, here))
            hit = DeclarationHit(kind = DeclarationKind.alias, name = string(name), at = td.at, column = here)
        }
    }
    if (hit.kind == DeclarationKind.none) {
        ast::for_each_global(thisMod) $(var value : VariablePtr) {
            return if (hit.kind != DeclarationKind.none)
            let v = value
            return if (!on_line(v.at, file, line) || !on_name(word, string(v.name), line_text, here))
            hit = DeclarationHit(kind = DeclarationKind.variable, variable = v, name = string(v.name), at = v.at, column = here)
        }
    }
    return hit
}

//! "Animal`speak" -> "speak": the token the source spells at a method's declaration and call sites.
def public bare_name(name : string) : string {
    let ix = rfind(name, "`")
    return ix < 0 ? name : slice(name, ix + 1)
}

class CursorVisitor : AstVisitor {
    targetLine : int
    targetCol : int
    targetFile : string
    hits : array<CursorHit>
    currentFunc : Function const?
    withDeclarations : bool
    inSynthesized : bool
    lineText : string    // the cursor line's source; "" leaves type names unmatched
    typeHits : table<string>    // one type hit per name and position: a `new T()` carries T on several nodes

    def CursorVisitor(file : string; line, col : int; declarations : bool; line_text : string) {
        targetFile = file
        targetLine = line
        targetCol = col
        withDeclarations = declarations
        lineText = line_text
    }

    def in_target_file(at : LineInfo) : bool {
        return true if (empty(targetFile) || at.fileInfo == null)
        return find(string(at.fileInfo.name), targetFile) >= 0
    }

    [arch(at="ARCHITECTURE.md#cursor-variable-position")]
    def cursor_on_variable(v : Variable const?) : bool {
        return cursor_on_token(v.at, string(v.name))
    }

    def add_variable_at_cursor(v : Variable const?) : void {
        return if (!withDeclarations || inSynthesized || v == null || v.flags.generated || !in_target_file(v.at) || !cursor_on_variable(v))
        hits |> emplace(CursorHit(
            variable = v,
            func = currentFunc,
            name = string(v.name),
            rtti = "Variable"
        ))
    }

    def override preVisitFunction(fun : FunctionPtr) : void {
        currentFunc = fun
        inSynthesized = is_synthesized(fun)
    }

    def override preVisitFunctionArgument(fun : FunctionPtr; arg : VariablePtr; lastArg : bool) : void {
        add_variable_at_cursor(arg)
    }

    def override preVisitExprBlockArgument(blk : ExprBlock?; arg : VariablePtr; lastArg : bool) : void {
        add_variable_at_cursor(arg)
    }

    def override preVisitExprLetVariable(expr : ExprLet?; arg : VariablePtr; lastArg : bool) : void {
        add_variable_at_cursor(arg)
    }

    def override preVisitExprForVariable(expr : ExprFor?; svar : VariablePtr; last : bool) : void {
        add_variable_at_cursor(svar)
    }

    // a type name the source spells - `p : Point`, `new Point()`, a return type - is a TypeDecl,
    // never an expression; the implicit `self` of a method carries its class at a position the
    // source does not spell, so it never hits
    def override preVisitTypeDecl(typ : TypeDeclPtr) : void {
        return if (!withDeclarations || empty(lineText) || inSynthesized || typ == null || !in_target_file(typ.at))
        let td = typ
        let name = string(td.structType != null ? td.structType.name : td.enumType != null ? td.enumType.name : td.alias)
        return if (empty(name) || !cursor_on_token(td.at, name) || !spells_name(lineText, int(td.at.column), name))
        let key = "{int(td.at.column)}:{name}"
        return if (key_exists(typeHits, key))
        typeHits |> insert(key)
        hits |> emplace(CursorHit(
            type_decl = td,
            func = currentFunc,
            name = name,
            rtti = "TypeDecl"
        ))
    }

    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        currentFunc = null
        inSynthesized = false
        return fun
    }

    // a compiler-made variable, such as the `__this` a lambda reads its captures through, is not
    // source the cursor can be on
    def generated_variable(e : Expression const?) : bool {
        return false if (!(e is ExprVar))
        let v = e as ExprVar
        return v.variable != null && v.variable.flags.generated
    }

    // a field access spans its value and the dot; the field name has its own position
    def on_field_name(e : Expression const?) : bool {
        if (e is ExprField) {
            let f = e as ExprField
            return cursor_on_token(f.atField, string(f.name))
        }
        if (e is ExprSafeField) {
            let f = e as ExprSafeField
            return cursor_on_token(f.atField, string(f.name))
        }
        return false
    }

    def cursor_on_token(at : LineInfo; name : string) : bool {
        return false if (int(at.line) != targetLine)
        if (int(at.last_line) != int(at.line) || int(at.last_column) > int(at.column)) {
            return cursor_inside(targetLine, targetCol, at)
        }
        let c = int(at.column)
        return targetCol >= c && targetCol < c + length(name)
    }

    // a generated field's initializer is a class's own member binding - the finalizer and vtable
    // entries the compiler places on the `class` line
    def override preVisitStructureField(str : StructurePtr; decl : FieldDeclaration; last : bool) : void {
        inSynthesized = decl.flags.generated
    }

    def override visitStructureField(str : StructurePtr; decl : FieldDeclaration; last : bool) : void {
        inSynthesized = false
    }

    def override preVisitExpression(expr : ExpressionPtr) : void {
        let e = expr
        return if (inSynthesized || !in_target_file(e.at) || generated_variable(e))
        if (cursor_inside(targetLine, targetCol, e.at) || on_field_name(e)) {
            hits |> emplace(CursorHit(
                expr = e,
                func = currentFunc,
                name = expression_name(expr),
                rtti = e.__rtti
            ))
        }
    }
}

//! (lines spanned, start column, end column): a nested node spans no more lines, starts no
//! earlier and ends no later than the node holding it
def private span_key(hit : CursorHit) : int3 {
    if (hit.expr == null) {
        let c = int(hit_at(hit).column)
        return int3(0, c, c + length(hit.name))
    }
    let at = hit.expr.at
    return int3(int(at.last_line) - int(at.line), int(at.column), int(at.last_column))
}

//! sorted innermost first: fewer lines, then the later start, then the earlier end, then the
//! later-visited (deeper) node
def private innermost_first(var src : array<CursorHit>) : array<CursorHit> {
    var order : array<int>
    order |> reserve(length(src))
    for (i in range(length(src))) {
        order |> push(i)
    }
    order |> sort() $(a, b) {
        let sa = span_key(src[a])
        let sb = span_key(src[b])
        if (sa.x != sb.x) return sa.x < sb.x
        if (sa.y != sb.y) return sa.y > sb.y
        if (sa.z != sb.z) return sa.z < sb.z
        return a > b
    }
    var result : array<CursorHit>
    result |> reserve(length(src))
    for (i in order) {
        result |> emplace <| src[i]
    }
    return <- result
}

//! Find all expression nodes at the given cursor position, innermost first; `file` is a substring of ``FileInfo.name``
//! ("" matches any); nodes inside a synthesized member and compiler-made variables are never hits. With `declarations`, a cursor
//! on a `for`/`let` variable's or an argument's name is a `variable` hit and, given the line's source as `line_text`, on a type name a `type_decl` hit.
def public find_at_cursor(program : smart_ptr<Program>; file : string; line, col : int; declarations : bool = false; line_text : string = "") : array<CursorHit> {
    return <- cursor_hits(file, line, col, declarations, line_text) $(var visitor) {
        make_visitor(*visitor) $(adapter) {
            visit(program, adapter)
        }
    }
}

//! Find all expression nodes at the given cursor position within a single function body.
//! `file`, `declarations` and `line_text` mean what they mean for `find_at_cursor`.
def public find_at_cursor_in_function(func : FunctionPtr; file : string; line, col : int; declarations : bool = false; line_text : string = "") : array<CursorHit> {
    return <- cursor_hits(file, line, col, declarations, line_text) $(var visitor) {
        visitor.currentFunc = func
        make_visitor(*visitor) $(adapter) {
            visit(func, adapter)
        }
    }
}

//! one visitor's run: `walk` drives it, the hits come back innermost first
def private cursor_hits(file : string; line, col : int; declarations : bool; line_text : string; walk : block<(var visitor : CursorVisitor?) : void>) : array<CursorHit> {
    var visitor = new CursorVisitor(file, line, col, declarations, line_text)
    invoke(walk, visitor)
    var result <- innermost_first(visitor.hits)
    unsafe {
        delete visitor
    }
    return <- result
}

//! Pretty-print a CursorHit for debugging. Includes RTTI type, name, resolved type, source location, and enclosing function.
def public describe(hit : CursorHit) : string {
    return build_string() $(var w) {
        write(w, hit.rtti)
        if (!empty(hit.name)) {
            write(w, "({hit.name})")
        }
        if (hit.variable != null && hit.variable._type != null) {
            write(w, " : {describe(hit.variable._type)}")
        } elif (hit.type_decl != null) {
            write(w, " : {describe(unsafe(reinterpret<TypeDecl?>(hit.type_decl)))}")
        } elif (hit.expr != null && hit.expr._type != null) {
            write(w, " : {describe(hit.expr._type)}")
        }
        let at = hit_at(hit)
        write(w, " at {int(at.line)}:{int(at.column)}-{int(at.last_line)}:{int(at.last_column)}")
        if (hit.func != null) {
            write(w, " in {hit.func.name}")
        }
    }
}
