options gen2
options indenting = 4

require daslib/clargs
require daslib/fio
require daslib/strings_boost

let HEADER = "include/daScript/ast/ast_expressions.h"
let BIND_DIR = "src/builtin"
let BIND_MACRO = "DAS_BIND_MANAGED_FIELD("
let ALL_PATH = "utils/internal/ast-fuzz/_ast_synth_all.das"
let PRELUDE_PATH = "utils/internal/ast-fuzz/gen_prelude.das.in"
let AST_H = "include/daScript/ast/ast.h"
let VISIT_CPP = "src/ast/ast.cpp"
let VERIFY_DAS = "daslib/ast_verify.das"

struct Fld {
    owner : string
    ty : string
    name : string
}

struct Row {
    node : string
    field : string
    dasfield : string
    kind : string
    hook : string
    ptype : string
}

[CommandLineArgs]
struct Config {
    @clarg_doc = "Check the checked-in files match what would be generated; write nothing"
    verify : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def read_text(path : string) : string {
    return fread(path)
}

def struct_name(line : string) : string {
    for (tok in split_by_chars(line, " \t")) {
        if (tok == "struct" || tok == "DAS_API" || empty(tok)) {
            continue
        }
        let c = find(tok, ":")
        return c >= 0 ? slice(tok, 0, c) : tok
    }
    return ""
}

def ident_head(s : string) : bool {
    if (empty(s)) {
        return false
    }
    let c = first_character(s)
    return is_alpha(c) || c == '_'
}

def field_names(tail : string) : array<string> {
    var res : array<string>
    let semi = find(tail, ";")
    let body = semi >= 0 ? slice(tail, 0, semi) : tail
    for (part in split(body, ",")) {
        var nm = strip(part)
        let eq = find(nm, "=")
        if (eq >= 0) {
            nm = strip(slice(nm, 0, eq))
        }
        if (ident_head(nm)) {
            res |> push(nm)
        }
    }
    return <- res
}

def collect_renames() : table<string; string> {
    var m : table<string; string>
    dir(BIND_DIR) $(fn) {
        if (!starts_with(fn, "module_builtin_ast") || !(ends_with(fn, ".cpp") || ends_with(fn, ".h"))) {
            return
        }
        for (line in split(read_text("{BIND_DIR}/{fn}"), "\n")) {
            let k = find(line, BIND_MACRO)
            if (k < 0) {
                continue
            }
            let rest = slice(line, k + length(BIND_MACRO))
            let close = find(rest, ")")
            let quote = find(rest, "(\"")
            if (close < 0 || quote < 0) {
                continue
            }
            let cpp_name = slice(rest, 0, close)
            let tail = slice(rest, quote + 2)
            let endq = find(tail, "\"")
            if (endq < 0) {
                continue
            }
            let das_name = slice(tail, 0, endq)
            if (!empty(das_name) && das_name != cpp_name) {
                m[cpp_name] = das_name
            }
        }
    }
    return <- m
}

def collect_structs(var bases : table<string; string>; var flds : array<Fld>) : void { // nolint:STYLE037 — dispatch over node kinds; splitting hides the table
    flds |> reserve(512)
    for (path in fixed_array(AST_H, HEADER)) {
        var cur = ""
        for (line in split(read_text(path), "\n")) {
            let t = strip(line)
            if (starts_with(t, "struct ")) {
                let nm = struct_name(t)
                if (!empty(nm)) {
                    cur = nm
                    let c = find(t, ":")
                    if (c >= 0 && !ends_with(t, ";")) {
                        var b = strip(slice(t, c + 1))
                        b = join(split(b, "public "), "")
                        let sp = find(b, " ")
                        if (sp >= 0) {
                            b = slice(b, 0, sp)
                        }
                        let cc = find(b, "\{")
                        if (cc >= 0) {
                            b = strip(slice(b, 0, cc))
                        }
                        let dc = find(b, "::")
                        if (dc >= 0) {
                            b = slice(b, dc + 2)
                        }
                        let tb = find(b, "<")
                        if (tb >= 0) {
                            b = slice(b, 0, tb)
                        }
                        bases[cur] = b
                    }
                }
                continue
            }
            if (empty(cur) || !ends_with(t, ";") || find(t, "(") >= 0) {
                continue
            }
            var ty = ""
            if (starts_with(t, "vector<ExpressionPtr>")) {
                ty = "vecexpr"
            } elif (starts_with(t, "vector<VariablePtr>")) {
                ty = "vecvar"
            } elif (starts_with(t, "vector<MakeStructPtr>")) {
                ty = "vecmks"
            } elif (starts_with(t, "vector<string>")) {
                ty = "vecstr"
            } elif (starts_with(t, "ExpressionPtr")) {
                ty = "expr"
            } elif (starts_with(t, "TypeDeclPtr")) {
                ty = "type"
            } elif (starts_with(t, "string")) {
                ty = "str"
            } else {
                continue
            }
            let head = (ty == "vecexpr" ? "vector<ExpressionPtr>"
                : (ty == "vecmks" ? "vector<MakeStructPtr>"
                : (ty == "vecvar" ? "vector<VariablePtr>"
                : (ty == "vecstr" ? "vector<string>"
                : (ty == "expr" ? "ExpressionPtr" : (ty == "type" ? "TypeDeclPtr" : "string"))))))
            for (nm in field_names(slice(t, length(head)))) {
                flds |> emplace(Fld(owner = cur, ty = ty, name = nm))
            }
        }
    }
}

def ancestry(kind : string; bases : table<string; string>) : array<string> {
    var out : array<string>
    var k = kind
    while (!empty(k) && find_index(out, k) < 0) {
        out |> push(k)
        k = bases?[k] ?? ""
    }
    return <- out
}

def denied(nm : string) : bool {
    return (nm == "type" || nm == "aliasSubstitution" || nm == "__rtti" || nm == "at"
        || nm == "atEnclosure" || nm == "ptrType")
}

def slots_of(kind : string; bases : table<string; string>; flds : array<Fld>;
             renames : table<string; string>) : array<Fld> {
    var out : array<Fld>
    for (anc in ancestry(kind, bases)) {
        for (f in flds) {
            if (f.owner != anc || denied(f.name)) {
                continue
            }
            let dn = renames?[f.name] ?? f.name
            var seen = false
            for (o in out) {
                if (o.name == dn) {
                    seen = true
                }
            }
            if (!seen) {
                out |> emplace(Fld(owner = kind, ty = f.ty, name = dn))
            }
        }
    }
    return <- out
}

let SKIP_KINDS = fixed_array("Expression", "ExprArrayCallWithSizeOrIndex", "ExprCallFunc", "ExprConstT", "ExprLikeCall", "ExprMakeLocal", "ExprOp", "ExprSequence", "ExprTableKeysOrValues")

let STMT_ONLY = fixed_array("ExprBlock", "ExprLet", "ExprWhile", "ExprFor", "ExprIfThenElse",
    "ExprWith", "ExprUnsafe", "ExprTryCatch", "ExprReturn", "ExprBreak", "ExprContinue",
    "ExprGoto", "ExprLabel", "ExprYield", "ExprDelete", "ExprCopy", "ExprMove", "ExprClone",
    "ExprStaticAssert", "ExprAssume")

def stmt_slot(nm : string) : bool {
    return (nm == "body" || nm == "if_true" || nm == "if_false" || nm == "try_block"
        || nm == "catch_block" || nm == "list" || nm == "finalList" || nm == "_block")
}

def const_value_of(kind : string) : string { // nolint:STYLE037 — dispatch over node kinds; splitting hides the table
    if (kind == "ExprConstInt") {
        return ", value = rnd(2000) - 1000"
    }
    if (kind == "ExprConstInt8") {
        return ", value = int8(rnd(200) - 100)"
    }
    if (kind == "ExprConstInt16") {
        return ", value = int16(rnd(2000) - 1000)"
    }
    if (kind == "ExprConstInt64") {
        return ", value = int64(rnd(100000)) * 100000l"
    }
    if (kind == "ExprConstUInt") {
        return ", value = uint(rnd(2000))"
    }
    if (kind == "ExprConstUInt8") {
        return ", value = uint8(rnd(200))"
    }
    if (kind == "ExprConstUInt16") {
        return ", value = uint16(rnd(2000))"
    }
    if (kind == "ExprConstUInt64") {
        return ", value = uint64(rnd(100000)) * 100000ul"
    }
    if (kind == "ExprConstFloat") {
        return ", value = float(rnd(2000) - 1000) * 0.5f"
    }
    if (kind == "ExprConstDouble") {
        return ", value = double(rnd(2000) - 1000) * 0.25lf"
    }
    if (kind == "ExprConstBool") {
        return ", value = rnd(2) == 0"
    }
    if (kind == "ExprConstString") {
        return ", value := rname()"
    }
    if (kind == "ExprConstRange") {
        return ", value = range(rnd(50), rnd(50) + 50)"
    }
    if (kind == "ExprConstURange") {
        return ", value = urange(uint(rnd(50)), uint(rnd(50) + 50))"
    }
    if (kind == "ExprConstInt2") {
        return ", value = int2(rnd(100) - 50, rnd(100) - 50)"
    }
    if (kind == "ExprConstInt3") {
        return ", value = int3(rnd(100) - 50, rnd(100) - 50, rnd(100) - 50)"
    }
    if (kind == "ExprConstInt4") {
        return ", value = int4(rnd(100) - 50, rnd(100) - 50, rnd(100) - 50, rnd(100) - 50)"
    }
    if (kind == "ExprConstUInt2") {
        return ", value = uint2(uint(rnd(100)), uint(rnd(100)))"
    }
    if (kind == "ExprConstUInt3") {
        return ", value = uint3(uint(rnd(100)), uint(rnd(100)), uint(rnd(100)))"
    }
    if (kind == "ExprConstUInt4") {
        return ", value = uint4(uint(rnd(100)), uint(rnd(100)), uint(rnd(100)), uint(rnd(100)))"
    }
    if (kind == "ExprConstFloat2") {
        return ", value = float2(float(rnd(100)) * 0.5f, float(rnd(100)) * 0.5f)"
    }
    if (kind == "ExprConstFloat3") {
        return ", value = float3(float(rnd(100)) * 0.5f, float(rnd(100)) * 0.5f, float(rnd(100)) * 0.5f)"
    }
    if (kind == "ExprConstFloat4") {
        return ", value = float4(float(rnd(100)) * 0.5f, float(rnd(100)) * 0.5f, float(rnd(100)) * 0.5f, float(rnd(100)) * 0.5f)"
    }
    return ""
}

def node_flags_of(kind : string) : string {
    if (kind == "ExprBlock") {
        return "            e.blockFlags.isClosure = rnd(3) == 0\n            e.blockFlags.inTheLoop = rnd(4) == 0\n"
    }
    if (kind == "ExprLet") {
        return "            e.letFlags.inScope = rnd(3) == 0\n"
    }
    if (kind == "ExprCast") {
        return "            e.castFlags.reinterpretCast = rnd(3) == 0\n            e.castFlags.upcastCast = rnd(4) == 0\n"
    }
    if (kind == "ExprMakeBlock") {
        return "            e.mmFlags.isLambda = rnd(3) == 0\n"
    }
    if (kind == "ExprMakeStruct") {
        return "            e.makeStructFlags.useInitializer = rnd(3) == 0\n"
    }
    return ""
}

def bind_of(kind : string) : string {
    if (kind == "ExprVar") {
        return (
            "            if (bind_refs && !empty(pool_vars) && rnd(3) != 0) \{\n" +
            "                e.variable = pool_vars[rnd(length(pool_vars))]\n" +
            "                if (rnd(3) != 0) \{\n" +
            "                    e.name := e.variable.name\n" +
            "                \}\n" +
            "            \}\n")
    }
    if (kind == "ExprCall" || kind == "ExprNew") {
        return (
            "            if (bind_refs && !empty(pool_funcs) && rnd(3) != 0) \{\n" +
            "                e.func = pool_funcs[rnd(length(pool_funcs))]\n" +
            "            \}\n")
    }
    if (kind == "ExprConstEnumeration") {
        return (
            "            if (bind_refs && !empty(pool_enums) && rnd(4) != 0) \{\n" +
            "                e.enumType = pool_enums[rnd(length(pool_enums))]\n" +
            "            \}\n")
    }
    if (kind == "ExprField" || kind == "ExprSafeField" || kind == "ExprIsVariant"
            || kind == "ExprAsVariant" || kind == "ExprSafeAsVariant") {
        return (
            "            if (bind_refs && rnd(3) == 0) \{\n" +
            "                e.fieldIndex = rnd(4) - 1\n" +
            "            \}\n")
    }
    if (kind == "ExprAddr") {
        return (
            "            if (bind_refs && !empty(pool_funcs) && rnd(3) != 0) \{\n" +
            "                e.func = pool_funcs[rnd(length(pool_funcs))]\n" +
            "            \}\n")
    }
    return ""
}

def pool_for(fname : string; kind : string = "") : string {
    if (fname == "op") {
        // the grammar admits only these in a unary / ternary slot, so a binary name
        // there can never resolve and ExprOp1.func stays null into codegen
        if (kind == "ExprOp1") {
            return "UNARY_OPS"
        }
        if (kind == "ExprOp3") {
            return "TERNARY_OPS"
        }
        return "OPS"
    }
    if (fname == "mask") {
        return "MASKS"
    }
    if (fname == "trait" || fname == "subtrait" || fname == "extratrait") {
        return "TRAITS"
    }
    return "NAMES"
}

def emit_synth_all(kinds : array<string>; bases : table<string; string>; flds : array<Fld>; // nolint:STYLE038 — dispatch over node kinds; splitting hides the table
                   renames : table<string; string>) : string {
    let tpl = read_text(PRELUDE_PATH)
    if (empty(tpl)) {
        return ""
    }
    return build_string() $(var w) { // nolint:STYLE037 — one branch per node kind; splitting hides the table
        var knames : array<string>
        for (k in kinds) {
            if (find_index(SKIP_KINDS, k) >= 0) {
                continue
            }
            knames |> push("\"{k}\"")
        }
        var vidx : array<string>
        var bi = 0
        for (k in kinds) {
            if (find_index(SKIP_KINDS, k) >= 0) {
                continue
            }
            if (find_index(STMT_ONLY, k) < 0) {
                vidx |> push("{bi}")
            }
            bi++
        }
        w |> write("// GENERATED from gen_prelude.das.in by gen_invariants.das -- do not edit.\n")
        w |> write(tpl)
        var emitted : array<string>
        var values : array<string>
        for (k in kinds) {
            if (find_index(SKIP_KINDS, k) >= 0) {
                continue
            }
            emitted |> push(k)
            if (find_index(STMT_ONLY, k) < 0) {
                values |> push(k)
            }
        }
        w |> write("    // Uniform over every constructible Expression subclass; `stmt` widens the\n")
        w |> write("    // pool to the statement-only kinds, which are illegal in a value slot.\n")
        w |> write("    def gen_any(depth : int; stmt : bool) : ExpressionPtr \{ // nolint:STYLE037,STYLE038 — generated dispatch: one branch per node kind\n") // nolint:LINT019 — the directive above is emitted DATA, not a directive on this file
        w |> write("        if (depth >= max_depth) \{\n")
        w |> write("            return terminal()\n")
        w |> write("        \}\n")
        w |> write("        var k = stmt ? rnd({length(emitted)}) : VALUE_IDX[rnd(length(VALUE_IDX))]\n")
        w |> write("        for (_try in range(12)) \{\n")
        w |> write("            if (!(skip?[KIND_NAMES[k]] ?? false)) \{\n")
        w |> write("                break\n")
        w |> write("            \}\n")
        w |> write("            k = stmt ? rnd({length(emitted)}) : VALUE_IDX[rnd(length(VALUE_IDX))]\n")
        w |> write("        \}\n")
        for (k, ki in emitted, count()) {
            let slots <- slots_of(k, bases, flds, renames)
            var ctor = ""
            var vecs : array<Fld>
            for (f in slots) {
                if (f.ty == "expr") {
                    let gen = stmt_slot(f.name) ? "gen_stmt" : "gen_expr"
                    ctor += ", {f.name} = {gen}(depth + 1)"
                } elif (f.ty == "type") {
                    ctor += ", {f.name} = gen_type(depth + 1)"
                } elif (f.ty == "str") {
                    let pool = pool_for(f.name, k)
                    ctor += ", {f.name} := {pool}[rnd(length({pool}))]"
                } else {
                    vecs |> push(f)
                }
            }
            w |> write("        if (k == {ki}) \{\n")
            w |> write("            note(\"{k}\")\n")
            w |> write("            var e = new {k}(at = rat(){ctor}{const_value_of(k)})\n")
            let nf = node_flags_of(k)
            if (!empty(nf)) {
                w |> write(nf)
            }
            for (v in vecs) {
                w |> write("            for (_i in range(1 + rnd(3))) \{\n")
                if (v.ty == "vecexpr") {
                    let gen = stmt_slot(v.name) ? "gen_stmt" : "gen_expr"
                    w |> write("                var c = {gen}(depth + 1)\n")
                    w |> write("                e.{v.name} |> emplace(c)\n")
                } elif (v.ty == "vecmks") {
                    w |> write("                var ms = gen_make_struct(depth + 1)\n")
                    w |> write("                e.{v.name} |> emplace(ms)\n")
                } elif (v.ty == "vecvar") {
                    w |> write("                var gv <- gen_variable(depth + 1)\n")
                    w |> write("                e.{v.name} |> emplace(gv)\n")
                } else {
                    w |> write("                let si = e.{v.name} |> length()\n")
                    w |> write("                e.{v.name} |> resize(si + 1)\n")
                    w |> write("                e.{v.name}[si] := rname()\n")
                }
                w |> write("            \}\n")
            }
            let bnd = bind_of(k)
            if (!empty(bnd)) {
                w |> write(bnd)
            }
            w |> write("            rflags(e)\n")
            w |> write("            rtype(e)\n")
            w |> write("            return e\n")
            w |> write("        \}\n")
        }
        w |> write("        return terminal()\n")
        w |> write("    \}\n")
        w |> write("\}\n")
        w |> write("\n")
        w |> write("// Replaces the Nth visited node with a constant. One typed override per kind:\n")
        w |> write("// the generic visitExpression hook does not substitute, so it cannot prune.\n")
        w |> write("class public Pruner : AstVisitor \{\n")
        w |> write("    target : int = -1\n")
        w |> write("    idx : int = 0\n")
        w |> write("    hit_rtti : string\n")
        w |> write("\n")
        w |> write("    def maybe(var e : ExpressionPtr) : ExpressionPtr \{\n")
        w |> write("        let me = idx\n")
        w |> write("        idx++\n")
        w |> write("        if (me == target) \{\n")
        w |> write("            hit_rtti = e.__rtti\n")
        w |> write("            return new ExprConstInt(at = e.at, value = 0)\n")
        w |> write("        \}\n")
        w |> write("        return e\n")
        w |> write("    \}\n")
        for (k in emitted) {
            w |> write("\n")
            w |> write("    def override visit{k}(var expr : {k}?) : ExpressionPtr \{\n")
            w |> write("        return maybe(expr)\n")
            w |> write("    \}\n")
        }
        w |> write("\}\n")
        w |> write("\n")
        w |> write("let private KIND_NAMES = fixed_array({join(knames, ", ")})\n")
        w |> write("let private VALUE_IDX = fixed_array({join(vidx, ", ")})\n")
    }
}

def collect_visit_derefs() : array<Row> {
    var res : array<Row>
    let src = read_text(VISIT_CPP)
    var cur = ""
    var loopvars : array<string>
    var guarded : array<string>
    for (line in split(src, "\n")) {
        let t = strip(line)
        let vm = find(t, "::visit(Visitor & vis) \{")
        if (vm > 0 && starts_with(t, "ExpressionPtr ")) {
            cur = slice(t, length("ExpressionPtr "), vm)
            loopvars |> clear()
            guarded |> clear()
            continue
        }
        if (empty(cur)) {
            continue
        }
        if (starts_with(t, "for ( auto & ")) {
            let rest = slice(t, length("for ( auto & "))
            let sp = find(rest, " ")
            if (sp > 0) {
                loopvars |> push(slice(rest, 0, sp))
            }
            continue
        }
        if (starts_with(t, "if (")) {
            var g = strip(slice(t, 4))
            let close = find(g, ")")
            if (close > 0) {
                g = strip(slice(g, 0, close))
            }
            if (!empty(g) && find(g, " ") < 0) {
                guarded |> push(g)
            }
            continue
        }
        let arrow = find(t, "->visit(vis)")
        if (arrow < 0) {
            continue
        }
        let eq = find(t, " = ")
        if (eq < 0 || eq > arrow) {
            continue
        }
        var nm = strip(slice(t, 0, eq))
        let member = rfind(nm, "->")
        let via = member >= 0 ? slice(nm, 0, member) : ""
        if (member >= 0) {
            nm = slice(nm, member + 2)
        }
        let full = empty(via) ? nm : "{via}->{nm}"
        if (empty(nm) || !ident_head(nm)
                || (empty(via) && find_index(loopvars, nm) >= 0)
                || find_index(guarded, full) >= 0 || find_index(guarded, nm) >= 0) {
            continue
        }
        res |> emplace(Row(node = cur, field = nm, dasfield = nm,
            kind = empty(via) ? "visit_deref" : "visit_deref_via", hook = "preVisit{cur}",
            ptype = "{cur}?"))
    }
    return <- res
}

let private empty_str = ""

def unrepaired_derefs(derefs : array<Row>; renames : table<string; string>) : array<string> {
    var res : array<string>
    let ver = read_text(VERIFY_DAS)
    var cur = ""
    var bodies : table<string; string>
    for (line in split(ver, "\n")) {
        let t = strip(line)
        if (starts_with(t, "def override preVisitExpr")) {
            let op = find(t, "(")
            cur = op > 0 ? slice(t, length("def override preVisit"), op) : ""
            continue
        }
        if (starts_with(t, "def ") && !starts_with(t, "def override preVisitExpr")) {
            cur = ""
            continue
        }
        if (!empty(cur)) {
            bodies[cur] = "{bodies?[cur] ?? empty_str}\n{t}"
        }
    }
    for (d in derefs) {
        let das = renames?[d.field] ?? d.field
        let body = build_string() $(var w) {
            for (hk in keys(bodies)) {
                if (starts_with(hk, d.node)) {
                    w |> write(bodies?[hk] ?? "")
                }
            }
        }
        if (find(body, das) >= 0 || find(body, "check_") >= 0) {
            continue
        }
        res |> push("{d.node}.{d.field}")
    }
    return <- res
}

def audit_visit_guardrails(renames : table<string; string>) : bool {
    let derefs <- collect_visit_derefs()
    let missing <- unrepaired_derefs(derefs, renames)
    to_log(LOG_INFO, "gen-invariants: {length(derefs)} unguarded child deref(s) in {VISIT_CPP} visit bodies, {length(derefs) - length(missing)} repaired by {VERIFY_DAS}\n")
    if (empty(missing)) {
        return true
    }
    for (m in missing) {
        to_log(LOG_ERROR, "  NO REPAIR: {m} — visitor derefs it unguarded; add a preVisit check to {VERIFY_DAS}\n")
    }
    to_log(LOG_ERROR, "gen-invariants: {length(missing)} slot(s) would crash the verifier's own walk\n")
    return false
}

[export]
def main : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "gen-ast")
        return 2
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "gen-ast")
        return 0
    }
    if (empty(read_text(HEADER))) {
        to_log(LOG_ERROR, "cannot read {HEADER} — run from the repo root\n")
        return 2
    }
    let renames <- collect_renames()
    var bases : table<string; string>
    var flds : array<Fld>
    collect_structs(bases, flds)
    var expr_kinds : array<string> // nolint:STYLE027 — filtered by ancestry, not a plain map
    for (k in keys(bases)) {
        expr_kinds |> push(k) if (find_index(ancestry(k, bases), "Expression") > 0)
    }
    sort(expr_kinds)
    if (empty(expr_kinds)) {
        to_log(LOG_ERROR, "parsed no Expression subclasses out of the AST headers\n")
        return 2
    }
    let allsrc = emit_synth_all(expr_kinds, bases, flds, renames)
    if (!audit_visit_guardrails(renames)) {
        return 2
    }
    if (cfg.verify) {
        if (read_text(ALL_PATH) != allsrc) {
            to_log(LOG_ERROR, "{ALL_PATH} is stale — rerun gen_invariants.das\n")
            return 1
        }
        to_log(LOG_INFO, "gen-ast: up to date ({length(expr_kinds)} Expression subclasses)\n")
        return 0
    }
    if (!fwrite(ALL_PATH, allsrc)) {
        to_log(LOG_ERROR, "failed to write {ALL_PATH}\n")
        return 2
    }
    to_log(LOG_INFO, "gen-ast: {length(expr_kinds)} Expression subclass(es) -> {ALL_PATH}\n")
    return 0
}
