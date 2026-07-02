// LSP navigation subtool — one compile, one op, LSP-shaped JSON to stdout.
//
// Invocation (CLI flags like -project_root go BEFORE the script path):
//   daslang utils/lsp/subtools/nav.das -- definition <file> <line> <character> [project]
//   daslang utils/lsp/subtools/nav.das -- hover <file> <line> <character> [project]
//   daslang utils/lsp/subtools/nav.das -- references <file> <line> <character> <include_decl> [project]
//   daslang utils/lsp/subtools/nav.das -- documentSymbol <file> [project]
//   daslang utils/lsp/subtools/nav.das -- workspaceSymbol <query> <context_file> [project]
//
// <line>/<character> are LSP positions (0-based line, UTF-16 units); this tool
// converts to/from daslang's 1-based byte positions — it has the file text.
// Locations carry a "path" field; the supervisor converts paths to file:// URIs.
//
// Fresh process per call by design (see utils/lsp/ROADMAP.md): no macro-state
// leaks, no binary locks, crash isolation.
options gen2
options rtti
options indenting = 4

require lsp_common
require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/ast_cursor
require daslib/fio
require daslib/strings_boost
require strings
require math

// LSP SymbolKind
let SK_CLASS = 5
let SK_METHOD = 6
let SK_FIELD = 8
let SK_ENUM = 10
let SK_INTERFACE = 11
let SK_FUNCTION = 12
let SK_VARIABLE = 13
let SK_CONSTANT = 14
let SK_ENUM_MEMBER = 22
let SK_STRUCT = 23

let MAX_WORKSPACE_SYMBOLS = 100

struct NavLocation {
    path : string
    @rename = "range" _range : LspRange
}

struct LocationsResult {
    ok : bool
    message : string
    locations : array<NavLocation>
}

struct HoverResult {
    ok : bool
    message : string
    contents : string
    hasRange : bool
    @rename = "range" _range : LspRange
}

struct DocSymbolChild {
    name : string
    detail : string
    kind : int
    @rename = "range" _range : LspRange
    selectionRange : LspRange
}

struct DocSymbol {
    name : string
    detail : string
    kind : int
    @rename = "range" _range : LspRange
    selectionRange : LspRange
    children : array<DocSymbolChild>
}

struct DocSymbolsResult {
    ok : bool
    message : string
    symbols : array<DocSymbol>
}

struct WsSymbol {
    name : string
    kind : int
    containerName : string
    path : string
    @rename = "range" _range : LspRange
}

struct WsSymbolsResult {
    ok : bool
    message : string
    symbols : array<WsSymbol>
}

// ===== shared plumbing =====

var FILE_LINES : table<string; array<string>>

def private with_file_lines(path : string; blk : block<(lines : array<string>) : void>) {
    if (!key_exists(FILE_LINES, path)) {
        FILE_LINES[path] <- load_file_lines(path)
    }
    // no FILE_LINES mutation while the ref lives — blk only reads the lines
    let lines & = unsafe(FILE_LINES[path])
    invoke(blk, lines)
}

// span of `word` starting at daslang position (1-based line, byte column)
def private word_range(path : string; line, column : int; word : string) : LspRange {
    var rng : LspRange
    with_file_lines(path) $(lines) {
        let start = make_position(lines, line, column)
        rng = LspRange(
            start = start,
            end = LspPosition(line = start.line, character = start.character + max(1, utf16_length(word))))
    }
    return rng
}

def private span_range(path : string; line, column, last_line, last_column : int) : LspRange {
    var rng : LspRange
    with_file_lines(path) $(lines) {
        rng = make_range(lines, line, column, last_line, last_column)
    }
    return rng
}

def private lineinfo_file(at : LineInfo) : string {
    if (at.fileInfo != null) return string(at.fileInfo.name)
    return ""
}

def private push_word_location(var out : array<NavLocation>; at : LineInfo; word : string) {
    let f = lineinfo_file(at)
    if (empty(f) || int(at.line) <= 0) return
    out |> push(NavLocation(
        path = to_generic_path(f),
        _range = word_range(f, int(at.line), int(at.column), word)))
}

// nav-profile compile: unoptimized AST (faithful positions), everything exported
def private compile_nav(file, project : string; blk : block<(program : smart_ptr<Program>) : void>) : bool {
    var compiled_ok = false
    var inscope access <- make_file_access(project)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; _issues) {
                if (ok && program != null) {
                    compiled_ok = true
                    invoke(blk, program)
                }
            }
        }
    }
    return compiled_ok
}

// incoming LSP position -> daslang (1-based line, 0-based byte column)
def private request_cursor(file : string; line0, char16 : int; var line : int&; var col : int&) {
    line = line0 + 1
    var text = ""
    with_file_lines(file) $(lines) {
        if (line0 >= 0 && line0 < length(lines)) {
            text = lines[line0]
        }
    }
    col = utf16_to_byte_offset(text, char16)
}

def private write_signature(var w : StringBuilderWriter; func) {
    var has_args = false
    for (arg in func.arguments) {
        let bt = arg._type.baseType
        continue if (bt == Type.fakeContext || bt == Type.fakeLineInfo)
        if (!has_args) {
            write(w, "(")
            has_args = true
        } else {
            write(w, "; ")
        }
        write(w, "{arg.name} : {describe(arg._type)}")
    }
    if (has_args) {
        write(w, ")")
    }
    if (func.result != null && !func.result.isVoid) {
        write(w, " : {describe(func.result)}")
    }
}

// query matching: case-insensitive substring, "=foo" for exact (same as MCP find_symbol)
def private symbol_matches(name, query : string) : bool {
    if (!empty(query) && first_character(query) == '=') return name == slice(query, 1)
    return find(to_lower(name), to_lower(query)) >= 0
}

// ===== definition resolution (port of utils/mcp/subtools/goto_definition.das) =====

struct Definition {
    found : bool
    kind : string
    name : string       // bare name — used to size the location's range
    file : string
    line : int
    column : int
    signature : string  // for functions: "def name(args) : ret"
    cppName : string    // builtins / handled types with no das source
}

def private field_value_struct(expr : Expression const?) : Structure const? {
    var value_type : TypeDecl const?
    if (expr is ExprField) {
        let f = expr as ExprField
        if (f.value != null) {
            value_type = f.value._type
        }
    } elif (expr is ExprSafeField) {
        let f = expr as ExprSafeField
        if (f.value != null) {
            value_type = f.value._type
        }
    }
    // unwrap pointer types (Foo? -> Foo)
    if (value_type != null && value_type.structType == null && value_type.firstType != null) {
        value_type = value_type.firstType
    }
    return value_type != null ? value_type.structType : null
}

def private resolve_definition(hit : CursorHit) : Definition {
    let expr = hit.expr
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            let variable = v.variable
            return Definition(
                found = true,
                kind = "variable",
                name = string(variable.name),
                file = lineinfo_file(variable.at),
                line = int(variable.at.line),
                column = int(variable.at.column))
        }
    }
    if (expr is ExprCall) {
        let c = expr as ExprCall
        if (c.func != null) {
            let signature = build_string() $(var w) {
                write(w, "def {c.func.name}")
                write_signature(w, *c.func)
            }
            if (c.func.flags.builtIn) {
                let bfn = c.func as BuiltInFunction
                return Definition(
                    found = true,
                    kind = "builtin",
                    name = string(c.func.name),
                    signature = signature,
                    cppName = bfn != null ? string(bfn.cppName) : "")
            }
            return Definition(
                found = true,
                kind = "function",
                name = string(c.func.name),
                file = lineinfo_file(c.func.at),
                line = int(c.func.at.line),
                column = int(c.func.at.column),
                signature = signature)
        }
    }
    if (expr is ExprField || expr is ExprSafeField) {
        let field_name = string(expr is ExprField ? (expr as ExprField).name : (expr as ExprSafeField).name)
        let st = field_value_struct(expr)
        if (st != null) {
            for (fld in st.fields) {
                if (fld.name == field_name) {
                    return Definition(
                        found = true,
                        kind = "field",
                        name = field_name,
                        file = lineinfo_file(fld.at),
                        line = int(fld.at.line),
                        column = int(fld.at.column))
                }
            }
        }
    }
    if (expr is ExprConstEnumeration) {
        let e = expr as ExprConstEnumeration
        if (e.enumType != null) {
            let en = e.enumType
            return Definition(
                found = true,
                kind = "enum",
                name = string(en.name),
                file = lineinfo_file(en.at),
                line = int(en.at.line),
                column = int(en.at.column))
        }
    }
    if (expr._type != null) {
        let td = expr._type
        if (!empty(td.alias)) {
            return Definition(
                found = true,
                kind = "typedef",
                name = string(td.alias),
                file = lineinfo_file(td.at),
                line = int(td.at.line),
                column = int(td.at.column))
        }
        if (td.isHandle && td.annotation != null) {
            return Definition(
                found = true,
                kind = "handled",
                name = string(td.annotation.name),
                cppName = string(td.annotation.cppName))
        }
        if (td.structType != null) {
            let st = td.structType
            return Definition(
                found = true,
                kind = st.flags.isClass ? "class" : "struct",
                name = string(st.name),
                file = lineinfo_file(st.at),
                line = int(st.at.line),
                column = int(st.at.column))
        }
        if (td.enumType != null) {
            let en = td.enumType
            return Definition(
                found = true,
                kind = "enum",
                name = string(en.name),
                file = lineinfo_file(en.at),
                line = int(en.at.line),
                column = int(en.at.column))
        }
    }
    return Definition()
}

def private run_definition(file : string; line0, char16 : int; project : string) : LocationsResult {
    var res = LocationsResult(ok = true)
    var line, col : int
    request_cursor(file, line0, char16, line, col)
    let compiled = compile_nav(file, project) $(program) {
        var hits <- find_at_cursor(program, file, line, col)
        for (hit in hits) {
            let defn = resolve_definition(hit)
            if (defn.found && !empty(defn.file) && defn.line > 0) {
                res.locations |> push(NavLocation(
                    path = to_generic_path(defn.file),
                    _range = word_range(defn.file, defn.line, defn.column, defn.name)))
                return
            }
            if (defn.found && !empty(defn.cppName)) {
                res.message = "{defn.kind} '{defn.name}' has no das source (C++ {defn.cppName})"
                return
            }
        }
        res.message = empty(hits) ? "no expression at cursor" : "could not resolve a definition at cursor"
    }
    if (!compiled) {
        res.ok = false
        res.message = "file does not compile — fix diagnostics first"
    }
    return <- res
}

// ===== hover =====

def private hover_contents(hit : CursorHit; defn : Definition) : string {
    return build_string() $(var w) {
        write(w, "```daslang\n")
        if (!empty(defn.signature)) {
            write(w, "{defn.signature}\n")
        } elif (!empty(hit.name) && hit.expr != null && hit.expr._type != null) {
            write(w, "{hit.name} : {describe(hit.expr._type)}\n")
        } elif (hit.expr != null && hit.expr._type != null) {
            write(w, "{describe(hit.expr._type)}\n")
        } else {
            write(w, "{hit.rtti}\n")
        }
        write(w, "```")
        if (defn.found) {
            write(w, "\n\n{defn.kind}")
            if (!empty(defn.file) && defn.line > 0) {
                write(w, " — defined at {to_generic_path(defn.file)}:{defn.line}")
            }
            if (!empty(defn.cppName)) {
                write(w, " — C++ `{defn.cppName}`")
            }
        }
        if (hit.func != null) {
            write(w, "\nin `{hit.func.name}`")
        }
    }
}

def private run_hover(file : string; line0, char16 : int; project : string) : HoverResult {
    var res = HoverResult(ok = true)
    var line, col : int
    request_cursor(file, line0, char16, line, col)
    let target = to_generic_path(file)
    let compiled = compile_nav(file, project) $(program) {
        let hits <- find_at_cursor(program, file, line, col)
        if (empty(hits)) {
            res.message = "no expression at cursor"
            return
        }
        // innermost hit that carries a name or a type; fall back to innermost
        var pick = 0
        for (idx in range(length(hits))) {
            if (!empty(hits[idx].name) || (hits[idx].expr != null && hits[idx].expr._type != null)) {
                pick = idx
                break
            }
        }
        let hit = hits[pick]
        res.contents = hover_contents(hit, resolve_definition(hit))
        if (hit.expr != null && to_generic_path(lineinfo_file(hit.expr.at)) == target && int(hit.expr.at.line) > 0) {
            res.hasRange = true
            res._range = span_range(file, int(hit.expr.at.line), int(hit.expr.at.column),
                int(hit.expr.at.last_line), int(hit.expr.at.last_column))
        }
    }
    if (!compiled) {
        res.ok = false
        res.message = "file does not compile — fix diagnostics first"
    }
    return res
}

// ===== references (port of utils/mcp/subtools/find_references.das) =====

enum RefTargetKind {
    none
    func
    variable
    structure
    field
    enumeration
    alias
}

struct RefTarget {
    kind : RefTargetKind
    // pointers stored as void? for identity comparison (avoids const mismatch)
    target_ptr : void?
    field_struct_ptr : void?
    field_name : string
    alias_name : string
    name : string       // bare name — sizes each reference's range
    decl_file : string  // declaration site, for includeDeclaration
    decl_line : int
    decl_column : int
}

def private to_void(p) : void? {
    return unsafe(reinterpret<void?> p)
}

def private identify_target(hit : CursorHit) : RefTarget {
    let expr = hit.expr
    if (expr is ExprCall) {
        let c = expr as ExprCall
        if (c.func != null) {
            return RefTarget(kind = RefTargetKind.func, target_ptr = to_void(c.func), name = string(c.func.name),
                decl_file = lineinfo_file(c.func.at), decl_line = int(c.func.at.line), decl_column = int(c.func.at.column))
        }
    }
    if (expr is ExprAddr) {
        let a = expr as ExprAddr
        if (a.func != null) {
            return RefTarget(kind = RefTargetKind.func, target_ptr = to_void(a.func), name = string(a.func.name),
                decl_file = lineinfo_file(a.func.at), decl_line = int(a.func.at.line), decl_column = int(a.func.at.column))
        }
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            let variable = v.variable
            return RefTarget(kind = RefTargetKind.variable, target_ptr = to_void(variable), name = string(variable.name),
                decl_file = lineinfo_file(variable.at), decl_line = int(variable.at.line), decl_column = int(variable.at.column))
        }
    }
    if (expr is ExprField || expr is ExprSafeField) {
        let fname = string(expr is ExprField ? (expr as ExprField).name : (expr as ExprSafeField).name)
        let st = field_value_struct(expr)
        if (st != null) {
            var t = RefTarget(kind = RefTargetKind.field, field_struct_ptr = to_void(st), field_name = fname, name = fname)
            for (fld in st.fields) {
                if (fld.name == fname) {
                    t.decl_file = lineinfo_file(fld.at)
                    t.decl_line = int(fld.at.line)
                    t.decl_column = int(fld.at.column)
                }
            }
            return t
        }
    }
    if (expr is ExprConstEnumeration) {
        let e = expr as ExprConstEnumeration
        if (e.enumType != null) {
            return RefTarget(kind = RefTargetKind.enumeration, target_ptr = to_void(e.enumType), name = string(e.enumType.name),
                decl_file = lineinfo_file(e.enumType.at), decl_line = int(e.enumType.at.line), decl_column = int(e.enumType.at.column))
        }
    }
    if (expr._type != null) {
        let td = expr._type
        if (!empty(td.alias)) {
            return RefTarget(kind = RefTargetKind.alias, alias_name = string(td.alias), name = string(td.alias),
                decl_file = lineinfo_file(td.at), decl_line = int(td.at.line), decl_column = int(td.at.column))
        }
        if (td.structType != null) {
            return RefTarget(kind = RefTargetKind.structure, target_ptr = to_void(td.structType), name = string(td.structType.name),
                decl_file = lineinfo_file(td.structType.at), decl_line = int(td.structType.at.line), decl_column = int(td.structType.at.column))
        }
        if (td.enumType != null) {
            return RefTarget(kind = RefTargetKind.enumeration, target_ptr = to_void(td.enumType), name = string(td.enumType.name),
                decl_file = lineinfo_file(td.enumType.at), decl_line = int(td.enumType.at.line), decl_column = int(td.enumType.at.column))
        }
    }
    return RefTarget()
}

// declarations at the cursor line (function/struct/enum/typedef/global defs are
// not expressions — find_at_cursor misses them)
def private identify_declaration_target(program : smart_ptr<Program>; file : string; line : int) : RefTarget {
    let thisMod = get_this_module(program)
    let target = to_generic_path(file)
    var result : RefTarget
    ast::for_each_function(thisMod, "") $(var func : FunctionPtr) {
        return if (result.kind != RefTargetKind.none)
        let f = func
        if (int(f.at.line) == line && to_generic_path(lineinfo_file(f.at)) == target) {
            result = RefTarget(kind = RefTargetKind.func, target_ptr = to_void(f), name = string(f.name),
                decl_file = lineinfo_file(f.at), decl_line = int(f.at.line), decl_column = int(f.at.column))
        }
    }
    return result if (result.kind != RefTargetKind.none)
    ast::for_each_structure(thisMod) $(var st : StructurePtr) {
        return if (result.kind != RefTargetKind.none)
        let s = st
        if (int(s.at.line) == line && to_generic_path(lineinfo_file(s.at)) == target) {
            result = RefTarget(kind = RefTargetKind.structure, target_ptr = to_void(s), name = string(s.name),
                decl_file = lineinfo_file(s.at), decl_line = int(s.at.line), decl_column = int(s.at.column))
        }
    }
    return result if (result.kind != RefTargetKind.none)
    ast::for_each_enumeration(thisMod) $(var en : EnumerationPtr) {
        return if (result.kind != RefTargetKind.none)
        let e = en
        if (int(e.at.line) == line && to_generic_path(lineinfo_file(e.at)) == target) {
            result = RefTarget(kind = RefTargetKind.enumeration, target_ptr = to_void(e), name = string(e.name),
                decl_file = lineinfo_file(e.at), decl_line = int(e.at.line), decl_column = int(e.at.column))
        }
    }
    return result if (result.kind != RefTargetKind.none)
    ast::for_each_typedef(thisMod) $(name : string#; var value : TypeDeclPtr) {
        return if (result.kind != RefTargetKind.none)
        let td = value
        if (int(td.at.line) == line && to_generic_path(lineinfo_file(td.at)) == target) {
            result = RefTarget(kind = RefTargetKind.alias, alias_name = string(name), name = string(name),
                decl_file = lineinfo_file(td.at), decl_line = int(td.at.line), decl_column = int(td.at.column))
        }
    }
    return result if (result.kind != RefTargetKind.none)
    ast::for_each_global(thisMod) $(var value : VariablePtr) {
        return if (result.kind != RefTargetKind.none)
        let v = value
        if (int(v.at.line) == line && to_generic_path(lineinfo_file(v.at)) == target) {
            result = RefTarget(kind = RefTargetKind.variable, target_ptr = to_void(v), name = string(v.name),
                decl_file = lineinfo_file(v.at), decl_line = int(v.at.line), decl_column = int(v.at.column))
        }
    }
    return result
}

struct RawRef {
    file : string
    line : int
    column : int
}

class RefVisitor : AstVisitor {
    target : RefTarget
    refs : array<RawRef>

    def RefVisitor(t : RefTarget) {
        target = t
    }

    def add_ref(at : LineInfo) : void {
        if (int(at.line) == 0 || at.fileInfo == null) return
        refs |> push(RawRef(file = string(at.fileInfo.name), line = int(at.line), column = int(at.column)))
    }

    def override preVisitExprCall(expr : ExprCall?) : void {
        if (target.kind == RefTargetKind.func && expr.func != null && to_void(expr.func) == target.target_ptr) {
            add_ref(expr.at)
        }
    }

    def override preVisitExprAddr(expr : ExprAddr?) : void {
        if (target.kind == RefTargetKind.func && expr.func != null && to_void(expr.func) == target.target_ptr) {
            add_ref(expr.at)
        }
    }

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (target.kind == RefTargetKind.variable && expr.variable != null && to_void(expr.variable) == target.target_ptr) {
            add_ref(expr.at)
        }
    }

    def override preVisitExprField(expr : ExprField?) : void {
        if (target.kind != RefTargetKind.field || expr.name != target.field_name) return
        if (to_void(field_value_struct(expr)) == target.field_struct_ptr) {
            add_ref(expr.atField)
        }
    }

    def override preVisitExprSafeField(expr : ExprSafeField?) : void {
        if (target.kind != RefTargetKind.field || expr.name != target.field_name) return
        if (to_void(field_value_struct(expr)) == target.field_struct_ptr) {
            add_ref(expr.atField)
        }
    }

    def override preVisitExprConstEnumeration(expr : ExprConstEnumeration?) : void {
        if (target.kind == RefTargetKind.enumeration && expr.enumType != null && to_void(expr.enumType) == target.target_ptr) {
            add_ref(expr.at)
        }
    }

    def override preVisitExprConstBitfield(expr : ExprConstBitfield?) : void {
        if (target.kind == RefTargetKind.alias && expr._type != null && !empty(expr._type.alias) && expr._type.alias == target.alias_name) {
            add_ref(expr.at)
        }
    }

    def override preVisitTypeDecl(typ : TypeDeclPtr) : void {
        let td = typ
        if (target.kind == RefTargetKind.structure && td.structType != null && to_void(td.structType) == target.target_ptr) {
            add_ref(td.at)
        }
        if (target.kind == RefTargetKind.enumeration && td.enumType != null && to_void(td.enumType) == target.target_ptr) {
            add_ref(td.at)
        }
        if (target.kind == RefTargetKind.alias && !empty(td.alias) && td.alias == target.alias_name) {
            add_ref(td.at)
        }
    }
}

def private run_references(file : string; line0, char16 : int; include_decl : bool; project : string) : LocationsResult {
    var res = LocationsResult(ok = true)
    var line, col : int
    request_cursor(file, line0, char16, line, col)
    let compiled = compile_nav(file, project) $(program) {
        var target : RefTarget
        var hits <- find_at_cursor(program, file, line, col)
        for (hit in hits) {
            target = identify_target(hit)
            break if (target.kind != RefTargetKind.none)
        }
        if (target.kind == RefTargetKind.none) {
            target = identify_declaration_target(program, file, line)
        }
        if (target.kind == RefTargetKind.none) {
            res.message = "no referenceable symbol at cursor"
            return
        }
        if (include_decl && !empty(target.decl_file) && target.decl_line > 0) {
            res.locations |> push(NavLocation(
                path = to_generic_path(target.decl_file),
                _range = word_range(target.decl_file, target.decl_line, target.decl_column, target.name)))
        }
        var visitor = new RefVisitor(target)
        make_visitor(*visitor) $(adapter) {
            visit_modules(program, adapter)
        }
        // a ref site can repeat across generic instantiations — dedupe by position
        var seen : table<string>
        for (r in visitor.refs) {
            let key = "{r.file}:{r.line}:{r.column}"
            continue if (key_exists(seen, key))
            seen |> insert(key)
            res.locations |> push(NavLocation(
                path = to_generic_path(r.file),
                _range = word_range(r.file, r.line, r.column, target.name)))
        }
        unsafe {
            delete visitor
        }
    }
    if (!compiled) {
        res.ok = false
        res.message = "file does not compile — fix diagnostics first"
    }
    return <- res
}

// ===== documentSymbol =====

def private decl_ranges(path : string; at : LineInfo; name : string; var full : LspRange&; var selection : LspRange&) {
    selection = word_range(path, int(at.line), int(at.column), name)
    full = (int(at.last_line) > 0 ?
        span_range(path, int(at.line), int(at.column), int(at.last_line), int(at.last_column)) :
        selection)
    // LSP requires selectionRange ⊆ range
    if (full.end.line < selection.end.line || (full.end.line == selection.end.line && full.end.character < selection.end.character)) {
        full.end = selection.end
    }
}

def private is_outline_function(func : Function const?) : bool {
    return false if (func.flags._lambda || func.flags.isClassMethod ||
        (func.flags.generated && func.fromGeneric == null))
    var plain_named = true  // generic instances carry a ` in the name
    peek(func.name) $(n) {
        plain_named = find(n, "`") < 0
    }
    return plain_named
}

def private collect_outline_function(var res : DocSymbolsResult; var seen_funcs : table<string>; target : string; func : Function const?) {
    return if (!is_outline_function(func) || int(func.at.line) <= 0)
    let f = lineinfo_file(func.at)
    return if (to_generic_path(f) != target)
    let key = "{func.name}:{int(func.at.line)}"
    return if (key_exists(seen_funcs, key))
    seen_funcs |> insert(key)
    let detail = build_string() $(var w) {
        write_signature(w, *func)
    }
    res.symbols |> emplace(make_doc_symbol(f, func.at, string(func.name), detail, SK_FUNCTION))
}

def private make_doc_symbol(path : string; at : LineInfo; name, detail : string; kind : int) : DocSymbol {
    var sym = DocSymbol(name = name, detail = detail, kind = kind)
    decl_ranges(path, at, name, sym._range, sym.selectionRange)
    return <- sym
}

def private push_doc_child(var sym : DocSymbol; path : string; at : LineInfo; name, detail : string; kind : int) {
    var child = DocSymbolChild(name = name, detail = detail, kind = kind)
    decl_ranges(path, at, name, child._range, child.selectionRange)
    sym.children |> emplace(child)
}

def private run_document_symbol(file, project : string) : DocSymbolsResult {
    var res = DocSymbolsResult(ok = true)
    let target = to_generic_path(file)
    let compiled = compile_nav(file, project) $(program) {
        let thisMod = get_this_module(program)
        var seen_funcs : table<string>
        ast::for_each_function(thisMod, "") $(var func : FunctionPtr) {
            collect_outline_function(res, seen_funcs, target, func)
        }
        ast::for_each_generic(thisMod) $(var func : FunctionPtr) {
            collect_outline_function(res, seen_funcs, target, func)
        }
        ast::for_each_structure(thisMod) $(var st : StructurePtr) {
            let s = st
            return if (s.flags.isLambda || s.flags._generator || s.flags.generated || int(s.at.line) <= 0)
            let f = lineinfo_file(s.at)
            return if (to_generic_path(f) != target)
            var sym <- make_doc_symbol(f, s.at, string(s.name), "", s.flags.isClass ? SK_CLASS : SK_STRUCT)
            for (fld in s.fields) {
                push_doc_child(sym, f, fld.at, string(fld.name), describe(fld._type), SK_FIELD)
            }
            if (s.flags.isClass) {
                ast::for_each_function(thisMod, "") $(var func : FunctionPtr) {
                    let fn = func
                    return if (!fn.flags.isClassMethod || empty(fn.arguments) ||
                        (fn.flags.generated && fn.fromGeneric == null))
                    let self_type = fn.arguments[0]._type
                    return if (self_type == null || self_type.structType == null || self_type.structType.name != s.name)
                    let detail = build_string() $(var w) {
                        write_signature(w, *fn)
                    }
                    push_doc_child(sym, f, fn.at, string(fn.name), detail, SK_METHOD)
                }
            }
            res.symbols |> emplace(sym)
        }
        ast::for_each_enumeration(thisMod) $(var en : EnumerationPtr) {
            let e = en
            return if (int(e.at.line) <= 0)
            let f = lineinfo_file(e.at)
            return if (to_generic_path(f) != target)
            var sym <- make_doc_symbol(f, e.at, string(e.name), "", SK_ENUM)
            for (entry in e.list) {
                push_doc_child(sym, f, entry.at, string(entry.name), "", SK_ENUM_MEMBER)
            }
            res.symbols |> emplace(sym)
        }
        ast::for_each_typedef(thisMod) $(name : string#; var value : TypeDeclPtr) {
            let td = value
            return if (int(td.at.line) <= 0)
            let f = lineinfo_file(td.at)
            return if (to_generic_path(f) != target)
            res.symbols |> emplace(make_doc_symbol(f, td.at, string(name), describe(value), SK_INTERFACE))
        }
        ast::for_each_global(thisMod) $(var value : VariablePtr) {
            let v = value
            return if (v.flags.generated || int(v.at.line) <= 0)
            let f = lineinfo_file(v.at)
            return if (to_generic_path(f) != target)
            let is_const = v._type != null && v._type.flags.constant
            res.symbols |> emplace(make_doc_symbol(f, v.at, string(v.name),
                v._type != null ? describe(v._type) : "", is_const ? SK_CONSTANT : SK_VARIABLE))
        }
    }
    if (!compiled) {
        res.ok = false
        res.message = "file does not compile — fix diagnostics first"
    }
    // declaration order, not walk order
    res.symbols |> sort() $(a, b) => a._range.start.line < b._range.start.line
    return <- res
}

// ===== workspaceSymbol =====

def private push_ws_symbol(var res : WsSymbolsResult; var count : int&; mname : string; at : LineInfo; name : string; kind : int) {
    return if (count >= MAX_WORKSPACE_SYMBOLS || int(at.line) <= 0)
    let f = lineinfo_file(at)
    return if (empty(f))
    res.symbols |> emplace(WsSymbol(
        name = name,
        kind = kind,
        containerName = mname == "$" ? "builtin" : mname,
        path = to_generic_path(f),
        _range = word_range(f, int(at.line), int(at.column), name)))
    count++
}

def private collect_ws_function(var res : WsSymbolsResult; var count : int&; mname, query : string; func : Function const?) {
    return if (!is_outline_function(func))
    if (symbol_matches(string(func.name), query)) {
        push_ws_symbol(res, count, mname, func.at, string(func.name), SK_FUNCTION)
    }
}

def private run_workspace_symbol(query, context_file, project : string) : WsSymbolsResult {
    var res = WsSymbolsResult(ok = true)
    if (empty(context_file)) {
        res.message = "no open document to anchor the search"
        return <- res
    }
    var count = 0
    let compiled = compile_nav(context_file, project) $(program) {
        program_for_each_module(program) $(mod) {
            return if (count >= MAX_WORKSPACE_SYMBOLS)
            let mname = string(mod.name)
            ast::for_each_function(mod, "") $(var func : FunctionPtr) {
                collect_ws_function(res, count, mname, query, func)
            }
            ast::for_each_generic(mod) $(var func : FunctionPtr) {
                collect_ws_function(res, count, mname, query, func)
            }
            ast::for_each_structure(mod) $(var st : StructurePtr) {
                let s = st
                return if (s.flags.isLambda || s.flags._generator || s.flags.generated)
                if (symbol_matches(string(s.name), query)) {
                    push_ws_symbol(res, count, mname, s.at, string(s.name), s.flags.isClass ? SK_CLASS : SK_STRUCT)
                }
            }
            ast::for_each_enumeration(mod) $(var en : EnumerationPtr) {
                let e = en
                if (symbol_matches(string(e.name), query)) {
                    push_ws_symbol(res, count, mname, e.at, string(e.name), SK_ENUM)
                }
            }
            ast::for_each_typedef(mod) $(name : string#; var value : TypeDeclPtr) {
                let td = value
                if (symbol_matches(string(name), query)) {
                    push_ws_symbol(res, count, mname, td.at, string(name), SK_INTERFACE)
                }
            }
            ast::for_each_global(mod) $(var value : VariablePtr) {
                let v = value
                if (symbol_matches(string(v.name), query)) {
                    push_ws_symbol(res, count, mname, v.at, string(v.name),
                        v._type != null && v._type.flags.constant ? SK_CONSTANT : SK_VARIABLE)
                }
            }
        }
        if (count >= MAX_WORKSPACE_SYMBOLS) {
            res.message = "truncated at {MAX_WORKSPACE_SYMBOLS} symbols"
        }
    }
    if (!compiled) {
        res.ok = false
        res.message = "context file does not compile — fix diagnostics first"
    }
    return <- res
}

// ===== dispatch =====

[export]
def main() : int {
    let raw <- get_command_line_arguments()
    let user <- lsp_user_args(raw)
    if (empty(user)) {
        print(sprint_json(LocationsResult(message = "missing op"), false))
        return 1
    }
    let op = user[0]
    if (op == "definition" || op == "hover") {
        if (length(user) < 4) {
            print(sprint_json(LocationsResult(message = "{op}: expected <file> <line> <character> [project]"), false))
            return 1
        }
        let file = user[1]
        let line0 = to_int(user[2])
        let char16 = to_int(user[3])
        let project = length(user) > 4 ? user[4] : ""
        if (op == "definition") {
            print(sprint_json(run_definition(file, line0, char16, project), false))
        } else {
            print(sprint_json(run_hover(file, line0, char16, project), false))
        }
    } elif (op == "references") {
        if (length(user) < 5) {
            print(sprint_json(LocationsResult(message = "references: expected <file> <line> <character> <include_decl> [project]"), false))
            return 1
        }
        let project = length(user) > 5 ? user[5] : ""
        print(sprint_json(run_references(user[1], to_int(user[2]), to_int(user[3]), user[4] == "true", project), false))
    } elif (op == "documentSymbol") {
        if (length(user) < 2) {
            print(sprint_json(DocSymbolsResult(message = "documentSymbol: expected <file> [project]"), false))
            return 1
        }
        print(sprint_json(run_document_symbol(user[1], length(user) > 2 ? user[2] : ""), false))
    } elif (op == "workspaceSymbol") {
        if (length(user) < 3) {
            print(sprint_json(WsSymbolsResult(message = "workspaceSymbol: expected <query> <context_file> [project]"), false))
            return 1
        }
        print(sprint_json(run_workspace_symbol(user[1], user[2], length(user) > 3 ? user[3] : ""), false))
    } else {
        print(sprint_json(LocationsResult(message = "unknown op '{op}'"), false))
        return 1
    }
    return 0
}
