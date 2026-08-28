options gen2
options indenting = 4

module _ast_synth private

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify
require daslib/templates_boost
require _ast_synth_all
require daslib/ast_print
require daslib/random
require math
require strings
require daslib/strings_boost

var private g_synthed = false
// --synth-skip, applied to every generator instance. Filling it at one creation site only
// leaves the flag silently inert everywhere else, which makes a bisect report the wrong kind.
var private g_skip_kinds = ""
// How many levels the nesting kinds (15, 16) pile on. Legal source can nest this deep, so
// a compiler that descends recursively has to survive it.
var private g_mut_depth = 8

let private HANDLED_TYPES = fixed_array(
    "$:das_string", "$:clock", "$:HashBuilder",
    "ast_core:TypeDecl", "ast_core:Expression", "ast_core:Structure",
    "ast_core:Function", "ast_core:Variable", "ast_core:Enumeration",
    "ast_core:MakeStruct", "ast_core:FieldDeclaration",
    "rtti_core:Context", "rtti_core:Program", "rtti_core:Module",
    "rtti_core:LineInfo", "rtti_core:TypeInfo", "rtti_core:StructInfo",
    "strings:StringBuilderWriter", "math:float4x4", "math:float3x3")

struct private DeclPool {
    @do_not_delete structs : array<Structure?>
    @do_not_delete enums : array<Enumeration?>
    @do_not_delete annots : array<TypeAnnotation?>
    @do_not_delete vars : array<VariablePtr>
    @do_not_delete funcs : array<Function?>
}

def private apply_skip(var g : GenAll?) : void {
    for (sk in split(g_skip_kinds, ",")) {
        if (!empty(strip(sk))) {
            g.skip[strip(sk)] := true
        }
    }
}

def private harvest_annotations(var pool : DeclPool) : void {
    for (spec in HANDLED_TYPES) {
        let c = find(spec, ":")
        if (c < 0) {
            continue
        }
        var m = get_module(slice(spec, 0, c))
        if (m == null) {
            continue
        }
        var ta = module_find_type_annotation(m, slice(spec, c + 1))
        if (ta != null) {
            pool.annots |> push(ta)
        }
    }
}

def private add_synth_decls(mod : Module?; var pool : DeclPool; seedInt : int; count : int;
                            depth : int) : void {
    var g = new GenAll(seed = random_seed(seedInt * 131 + 7), max_depth = depth, at = LineInfo())
    apply_skip(g)
    pool.structs |> reserve(count)
    pool.enums |> reserve(count)
    pool.vars |> reserve(count)
    for (i in range(count)) {
        var st <- new Structure(at = LineInfo(), name := "SynthS{seedInt}_{i}")
        for (f in range(1 + i % 3)) {
            add_structure_field(st, "f{f}", g.gen_type(depth), null)
        }
        var stp = st
        if (mod |> add_structure(st)) {
            pool.structs |> push(stp)
        }
        var en <- new Enumeration(at = LineInfo(), name := "SynthE{seedInt}_{i}")
        en.baseType = Type.tInt
        for (e in range(1 + i % 3)) {
            let ei = length(en.list)
            en.list |> resize(ei + 1)
            en.list[ei].name := "e{e}"
            en.list[ei].value = new ExprConstInt(at = LineInfo(), value = e)
        }
        pool.enums |> push(en)
        var gv <- new Variable(at = LineInfo(), name := "synth_g{seedInt}_{i}",
            _type = new TypeDecl(at = LineInfo(), baseType = Type.tInt),
            init = new ExprConstInt(at = LineInfo(), value = i))
        var gvp = gv
        if (mod |> add_variable(gv)) {
            pool.vars |> push(gvp)
        }
    }
    unsafe {
        delete g
    }
    harvest_annotations(pool)
}

def private fill_pools(var g : GenAll?; var pool : DeclPool; bind_refs : bool; pre_typed : bool) : void {
    g.pool_structs |> push_from(pool.structs)
    g.pool_enums |> push_from(pool.enums)
    g.pool_annots |> push_from(pool.annots)
    g.pool_vars |> push_from(pool.vars)
    g.pool_funcs |> push_from(pool.funcs)
    g.bind_refs = bind_refs
    g.pre_typed = pre_typed
}

// Typed mode: whole functions with a real result type and a body that returns
// it, so the program is well-typed and inference runs to the end. Every function
// may call the ones added before it, which keeps the call graph a DAG.
// Structures whose fields are all from the typed universe, so a make-struct
// literal for them is well-typed and the make-local paths get exercised.
def private add_typed_structs(mod : Module?; var g : GenAll?; seedInt, count : int) : void {
    for (i in range(count)) {
        var ts = TypedStruct(name = "TS{seedInt}_{i}")
        var st <- new Structure(at = g.at, name := ts.name)
        let nfields = 1 + i % 3
        ts.fields |> reserve(nfields)
        for (f in range(nfields)) {
            let fc = VALUE_CODES[g.rnd(length(VALUE_CODES))]
            ts.fields |> push(fc)
            add_structure_field(st, "f{f}", g.ty_decl(fc), null)
        }
        var stp = st
        if (mod |> add_structure(st)) {
            g.ts_ptr |> push(stp)
            g.ts_fields |> push_clone(ts)
        }
    }
}

// A callee whose trailing parameter is a block and whose leading parameters all have
// defaults. Calling it with the block alone forces inference down the piped-landing
// search, which nothing else in the generator reaches.
def private add_piped_helpers(mod : Module?; var g : GenAll?; seedInt : int;
                              at : LineInfo) : int {
    var added = 0
    for (i in range(2)) {
        let name = "tpipe_{seedInt}_{i}"
        var fn <- new Function(at = at, atDecl = at, name := name, result = g.ty_decl(T_INT))
        for (a in range(1 + i)) {
            var av <- new Variable(at = at, name := "p{a}", _type = g.ty_decl(T_INT),
                init = new ExprConstInt(at = at, value = a + 1))
            fn.arguments |> emplace(av)
        }
        var bv <- new Variable(at = at, name := "blk", _type = g.block_type(T_INT))
        fn.arguments |> emplace(bv)
        var call = new ExprInvoke(at = at, name := "invoke")
        var bref = new ExprVar(at = at, name := "blk")
        call.arguments |> emplace(bref)
        var parg = new ExprVar(at = at, name := "p0")
        call.arguments |> emplace(parg)
        var body = new ExprBlock(at = at)
        var r = new ExprReturn(at = at, subexpr = call)
        body.list |> emplace(r)
        fn.body = body
        verify_function(fn)
        if (mod |> add_function(fn)) {
            added++
            g.piped_fns |> push(name)
        }
    }
    return added
}

// Two generic shapes with `auto` arguments and an `auto` result: every call site
// instantiates them again, which is the only way the generic machinery runs at all.
def private add_generic_passthrough(mod : Module?; seedInt : int; at : LineInfo;
                                    var known : array<TypedFn>) : int {
    var added = 0
    for (k in range(2))  {
        let two = k == 1
        var sig = TypedFn(name = two ? "tgen_{seedInt}_op" : "tgen_{seedInt}_id",
            kind = two ? FN_GENERIC_OP : FN_GENERIC_ID)
        var fn <- new Function(at = at, atDecl = at, name := sig.name,
            result = new TypeDecl(at = at, baseType = Type.autoinfer))
        for (a in range(two ? 2 : 1)) {
            var av <- new Variable(at = at, name := "a{a}",
                _type = new TypeDecl(at = at, baseType = Type.autoinfer))
            fn.arguments |> emplace(av)
        }
        var body = new ExprBlock(at = at)
        var value : ExpressionPtr
        if (two) {
            value = new ExprOp2(at = at, op := "+", left = new ExprVar(at = at, name := "a0"),
                right = new ExprVar(at = at, name := "a1"))
        } else {
            value = new ExprVar(at = at, name := "a0")
        }
        var r = new ExprReturn(at = at, subexpr = value)
        body.list |> emplace(r)
        fn.body = body
        verify_function(fn)
        // an auto-argument function is a generic, and generics live in their own
        // module slot - add_function would file it where no call site looks
        if (mod |> add_generic(fn)) {
            added++
            known |> push_clone(sig)
        }
    }
    return added
}

// Run mode: give the victim a main that calls every synthesized function with literal
// arguments and prints each result, so the same program can be executed twice - with and
// without the optimizer - and the two outputs compared.
def private emit_run_main(mod : Module?; var g : GenAll?; sigs : array<TypedFn>;
                          at : LineInfo) : void {
    var body = new ExprBlock(at = at)
    for (sig in sigs) {
        if (sig.kind != FN_CONCRETE) {
            continue
        }
        var call = new ExprCall(at = at, name := sig.name)
        for (a in sig.args) {
            var arg = g.gen_val(a, g.max_depth - 1)
            call.arguments |> emplace(arg)
        }
        if (sig.ret == T_VOID) {
            body.list |> emplace(call)
            continue
        }
        // print only what has a stable textual form; a pointer would differ per run
        if (sig.ret >= T_STRUCT) {
            var sink = new ExprLet(at = at)
            var sv <- new Variable(at = at, name := "sink_{length(body.list)}",
                _type = g.ty_decl(sig.ret), init = call)
            sink.variables |> emplace(sv)
            body.list |> emplace(sink)
            continue
        }
        var sb = new ExprStringBuilder(at = at)
        // the leading newline keeps the marker at the start of a line even when a
        // generated body printed something without one
        var tag = new ExprConstString(at = at, value := "\nR {sig.name} ")
        sb.elements |> emplace(tag)
        sb.elements |> emplace(call)
        var nl = new ExprConstString(at = at, value := "\n")
        sb.elements |> emplace(nl)
        var pr = new ExprCall(at = at, name := "print")
        pr.arguments |> emplace(sb)
        body.list |> emplace(pr)
    }
    for_each_function(mod, "main") $(var fn) {
        fn.body = body
        fn.not_inferred()
    }
}

def private add_synth_typed_functions(mod : Module?; seedInt : int; count, depth,
                                      size : int; at : LineInfo; run_mode : bool) : int {
    var known : array<TypedFn>
    var added = add_generic_passthrough(mod, seedInt, at, known)
    var decls = new GenAll(seed = random_seed(seedInt * 131 + 7), max_depth = depth,
        run_mode = run_mode, at = at)
    apply_skip(decls)
    add_typed_structs(mod, decls, seedInt, 1 + abs(random_int(decls.seed)) % 3)
    added += add_piped_helpers(mod, decls, seedInt, at)
    for (i in range(count)) {
        var g = new GenAll(seed = random_seed(seedInt + i * 7919), max_depth = depth,
            run_mode = run_mode, at = at)
        apply_skip(g)
        g.typed_fns |> push_clone_from(known)
        g.ts_ptr |> push_from(decls.ts_ptr)
        g.ts_fields |> push_clone_from(decls.ts_fields)
        g.piped_fns |> push_clone_from(decls.piped_fns)
        var sig = TypedFn(name = "tsynth_{seedInt}_{i}", kind = FN_CONCRETE,
            ret = g.rnd(4) == 0 ? T_VOID : g.any_code())
        var fn <- new Function(at = at, atDecl = at, name := sig.name,
            result = g.ty_decl(sig.ret))
        let nargs = g.rnd(4)
        sig.args |> reserve(nargs)
        for (a in range(nargs)) {
            let ac = g.any_code()
            sig.args |> push(ac)
            let an = "a{a}"
            var av <- new Variable(at = at, name := an, _type = g.ty_decl(ac))
            fn.arguments |> emplace(av)
            g.tv_push(an, ac, false)
        }
        g.res_code = sig.ret
        fn.body = g.gen_typed_body(size)
        verify_function(fn)
        unsafe {
            delete g
        }
        if (mod |> add_function(fn)) {
            added++
            known |> push_clone(sig)
        }
    }
    if (run_mode) {
        decls.typed_fns |> push_clone_from(known)
        emit_run_main(mod, decls, known, at)
    }
    unsafe {
        delete decls
    }
    return added
}

def private add_synth_functions(mod : Module?; seedInt : int; count : int; depth : int) : int {
    var seed = random_seed(seedInt * 31 + 17)
    var added = 0
    for (i in range(count)) {
        var g = new GenAll(seed = random_seed(seedInt + i * 6151), max_depth = depth,
            at = LineInfo())
        apply_skip(g)
        let nargs = abs(random_int(seed)) % 3
        // the body is replaced with random statements below, so a non-void result is
        // error[30310] every time and the program never reaches codegen
        var fn <- new Function(at = LineInfo(), atDecl = LineInfo(),
            name := "synth_gen_{seedInt}_{i}",
            result = new TypeDecl(at = LineInfo(), baseType = Type.tVoid))
        for (a in range(nargs)) {
            var av <- new Variable(at = LineInfo(), name := "a{a}", _type = g.gen_type(depth))
            fn.arguments |> emplace(av)
        }
        fn.body = g.gen_stmt(0)
        verify_function(fn)
        unsafe {
            delete g
        }
        if (mod |> add_function(fn)) {
            added++
        }
    }
    return added
}

// Mutation of a REAL program's AST, as opposed to synthesis from nothing. Every edit is
// one a person could have typed - a different operator, a different literal, a swapped
// argument, another declared type - so a crash it provokes is a compiler bug rather than
// a shape only a macro can build. One deterministic edit per run (the Nth candidate node
// gets kind K), which makes a finding reproducible and already close to minimal.
let private MUT_OPS = fixed_array("+", "-", "*", "%", "==", "!=", "<", ">", "<=", ">=",
    "&&", "||", "&", "|", "^", "<<", ">>")

class private Mutator : AstVisitor {
    at_target : int = -1
    kind : int = 0
    idx : int = 0
    applied : string
    seed : int4

    def pick(n : int) : int {
        return n <= 1 ? 0 : abs(random_int(seed)) % n
    }

    def hit() : bool {
        let me = idx
        idx++
        return me == at_target
    }

    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (kind == 0 && hit()) {
            let was = "{expr.op}"
            expr.op := MUT_OPS[pick(length(MUT_OPS))]
            applied = "op2 {was} -> {expr.op}"
        }
        return expr
    }

    def override visitExprConstInt(var expr : ExprConstInt?) : ExpressionPtr {
        if (kind == 1 && hit()) {
            let was = expr.value
            expr.value = pick(3) == 0 ? 0 : (pick(2) == 0 ? -1 : 2147483647)
            applied = "int {was} -> {expr.value}"
        }
        return expr
    }

    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (kind == 2 && hit() && length(expr.arguments) >= 2) {
            var a <- expr.arguments[0]
            var b <- expr.arguments[1]
            expr.arguments[0] <- b
            expr.arguments[1] <- a
            applied = "swapped first two arguments of {expr.name}"
        }
        return expr
    }

    // 8: the same assignment written as a move, or as a clone
    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        if (kind == 8 && hit()) {
            var l <- expr.left
            var r <- expr.right
            if (pick(2) == 0) {
                applied = "copy -> move"
                return new ExprMove(at = expr.at, op := "<-", left = l, right = r)
            }
            applied = "copy -> clone"
            return new ExprClone(at = expr.at, op := ":=", left = l, right = r)
        }
        return expr
    }

    // 9: put one statement inside a construct that changes nothing at runtime but does
    // change the context inference sees - unsafe, try/recover, or a bare scope
    // 10: run one statement through a closure instead of inline
    def override visitExprBlock(var expr : ExprBlock?) : ExpressionPtr {
        if (kind == 9 && hit() && !empty(expr.list)) {
            let at = pick(length(expr.list))
            var inner = new ExprBlock(at = expr.at)
            var stmt <- expr.list[at]
            inner.list |> emplace(stmt)
            let form = pick(3)
            if (form == 0) {
                applied = "statement wrapped in unsafe"
                expr.list[at] = new ExprUnsafe(at = expr.at, body = inner)
            } elif (form == 1) {
                applied = "statement wrapped in try/recover"
                expr.list[at] = new ExprTryCatch(at = expr.at, try_block = inner,
                    catch_block = new ExprBlock(at = expr.at))
            } else {
                applied = "statement wrapped in a bare scope"
                expr.list[at] = inner
            }
        }
        if (kind == 10 && hit() && !empty(expr.list)) {
            let at = pick(length(expr.list))
            var body = new ExprBlock(at = expr.at)
            body.blockFlags.isClosure = true
            body.returnType = new TypeDecl(at = expr.at, baseType = Type.tVoid)
            var stmt <- expr.list[at]
            body.list |> emplace(stmt)
            var mb = new ExprMakeBlock(at = expr.at, _block = body)
            var inv = new ExprInvoke(at = expr.at, name := "invoke")
            inv.arguments |> emplace(mb)
            expr.list[at] = inv
            applied = "statement moved into an invoked closure"
        }
        // 16: the same statement, but reached through g_mut_depth nested closures
        if (kind == 16 && hit() && !empty(expr.list)) {
            let at = pick(length(expr.list))
            var stmt <- expr.list[at]
            for (_i in range(g_mut_depth)) {
                var body = new ExprBlock(at = expr.at)
                body.blockFlags.isClosure = true
                body.returnType = new TypeDecl(at = expr.at, baseType = Type.tVoid)
                body.list |> emplace(stmt)
                var inv = new ExprInvoke(at = expr.at, name := "invoke")
                var mb = new ExprMakeBlock(at = expr.at, _block = body)
                inv.arguments |> emplace(mb)
                stmt <- inv
            }
            expr.list[at] <- stmt
            applied = "statement nested in {g_mut_depth} closures"
        }
        if (kind == 11 && hit() && !empty(expr.list)) {
            let at = pick(length(expr.list))
            var dup = clone_expression(expr.list[at])
            expr.list |> emplace(dup, at + 1)
            applied = "statement duplicated"
        }
        return expr
    }

    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        if (kind == 3 && hit()) {
            let was = "{expr.name}"
            expr.name := "{was}_"
            expr.variable = null
            expr.varFlags = ExprVarFlags(0)
            applied = "var {was} -> {expr.name}"
        }
        return expr
    }

    // 5: drop the declared type and make inference derive it
    // 6: same declaration, but const
    // 7: same declaration, but a container of the old type
    def override visitExprLetVariable(var expr : ExprLet?; var arg : VariablePtr;
                                      lastArg : bool) : VariablePtr {
        if (kind == 5 && hit() && arg._type != null && arg.init != null) {
            applied = "let type {describe(arg._type)} -> auto"
            arg._type = new TypeDecl(at = arg.at, baseType = Type.autoinfer)
            return arg
        }
        if (kind == 6 && hit() && arg._type != null) {
            applied = "let {describe(arg._type)} -> const"
            arg._type.flags.constant = true
            return arg
        }
        if (kind == 7 && hit() && arg._type != null) {
            let was = describe(arg._type)
            var outer = new TypeDecl(at = arg.at, baseType = Type.tArray)
            outer.firstType = clone_type(arg._type)
            arg._type = outer
            applied = "let type {was} -> array of it"
            return arg
        }
        // 15: the same declaration, but nested g_mut_depth containers deep. Legal source
        // can spell this, so a recursive descent over the type has to survive it.
        if (kind == 15 && hit() && arg._type != null && arg.init == null) {
            let was = describe(arg._type)
            for (_i in range(g_mut_depth)) {
                var outer = new TypeDecl(at = arg.at, baseType = Type.tArray)
                outer.firstType = clone_type(arg._type)
                arg._type = outer
            }
            applied = "let type {was} -> array nested {g_mut_depth} deep"
            return arg
        }
        if (kind == 4 && hit() && arg._type != null) {
            let was = describe(arg._type)
            var g = new GenAll(seed = seed, max_depth = 2, at = arg.at)
            apply_skip(g)
            arg._type = g.ty_decl(VALUE_CODES[pick(length(VALUE_CODES))])
            let now = describe(arg._type)
            unsafe {
                delete g
            }
            applied = "let type {was} -> {now}"
        }
        return arg
    }

    // 12: same argument, but const
    // 13: the last argument gains a default value
    def override visitFunctionArgument(var fun : FunctionPtr; var arg : VariablePtr;
                                       lastArg : bool) : VariablePtr {
        if (kind == 12 && hit() && arg._type != null) {
            applied = "argument {arg.name} {describe(arg._type)} -> const"
            arg._type.flags.constant = true
            return arg
        }
        if (kind == 13 && hit() && lastArg && arg.init == null && arg._type != null &&
            !arg._type.flags.ref) {
            let bt = arg._type.baseType
            if (bt == Type.tInt) {
                arg.init := new ExprConstInt(at = arg.at, value = 0)
                applied = "argument {arg.name} default = 0"
            } elif (bt == Type.tFloat) {
                arg.init := new ExprConstFloat(at = arg.at, value = 0.0)
                applied = "argument {arg.name} default = 0."
            } elif (bt == Type.tBool) {
                arg.init := new ExprConstBool(at = arg.at, value = false)
                applied = "argument {arg.name} default = false"
            } elif (bt == Type.tString) {
                arg.init := new ExprConstString(at = arg.at, value := "")
                applied = "argument {arg.name} default = empty string"
            }
        }
        return arg
    }

    // 14: the result type is dropped, so inference has to derive it from the body
    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        if (kind == 14 && hit() && fun.result != null &&
            fun.result.baseType != Type.autoinfer) {
            applied = "result type {describe(fun.result)} -> auto"
            fun.result = new TypeDecl(at = fun.at, baseType = Type.autoinfer)
        }
        return fun
    }
}

// One pass that mutates nothing, to learn how many candidates of this kind exist.
def private count_kind(prog : ProgramPtr; mod : Module?; kind : int) : int {
    var m = new Mutator(at_target = -1, kind = kind, seed = random_seed(1))
    make_visitor(*m) $(adapter) {
        visit_module(prog, adapter, mod)
    }
    let n = m.idx
    unsafe {
        delete m
    }
    return n
}

def private apply_one(prog : ProgramPtr; mod : Module?; at_target, kind : int;
                      seed : int4; var what : string&) : bool {
    var m = new Mutator(at_target = at_target, kind = kind, seed = seed)
    make_visitor(*m) $(adapter) {
        visit_module(prog, adapter, mod)
    }
    what = m.applied
    unsafe {
        delete m
    }
    return !empty(what)
}

// Several edits at once. Each one on its own is something a person could type, but the
// combination puts inference in a state no single edit reaches - which is where a
// well-formed program still has a chance of breaking it.
def private mutate_module_many(prog : ProgramPtr; mod : Module?; seedInt, count,
                               nkinds : int) : bool {
    var seed = random_seed(seedInt * 104729 + 7)
    var applied = 0
    for (i in range(count)) {
        let kind = abs(random_int(seed)) % nkinds
        let n = count_kind(prog, mod, kind)
        if (n < 1) {
            continue
        }
        let at = abs(random_int(seed)) % n
        var what : string
        if (apply_one(prog, mod, at, kind, seed, what)) {
            applied++
            to_log(LOG_ERROR, "AST-MUT[{i}]: kind={kind} at={at}/{n}: {what}\n")
        }
    }
    to_log(LOG_ERROR, "AST-MUT: {applied} edit(s) applied\n")
    if (applied == 0) {
        return false
    }
    for_each_function(mod, "") $(var fn) {
        fn.not_inferred()
    }
    return true
}

def private mutate_module(prog : ProgramPtr; mod : Module?; seedInt, at_target,
                          kind : int) : bool {
    var m = new Mutator(at_target = at_target, kind = kind,
        seed = random_seed(seedInt * 7919 + 13))
    make_visitor(*m) $(adapter) {
        visit_module(prog, adapter, mod)
    }
    let nodes = m.idx
    let what = m.applied
    unsafe {
        delete m
    }
    to_log(LOG_ERROR, "AST-MUT: kind={kind} at={at_target} of {nodes} candidate(s): {empty(what) ? "nothing" : what}\n")
    if (empty(what)) {
        return false
    }
    for_each_function(mod, "") $(var fn) {
        fn.not_inferred()
    }
    return true
}

[infer_macro]
class AstSynthMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool { // nolint:STYLE038 — dispatch over node kinds; splitting hides the table
        if (g_synthed || mod == null || !empty(mod.name) || mod.moduleFlags.builtIn
                || !(prog._options |> find_arg("_enable_ast_synth") ?as tBool ?? false)) {
            return false
        }
        let seedInt = prog._options |> find_arg("_ast_synth_seed") ?as tInt ?? 1
        let size = prog._options |> find_arg("_ast_synth_size") ?as tInt ?? 12
        let depth = prog._options |> find_arg("_ast_synth_depth") ?as tInt ?? 4
        let prune = prog._options |> find_arg("_ast_synth_prune") ?as tInt ?? -1
        g_skip_kinds = prog._options |> find_arg("_ast_synth_skip") ?as tString ?? ""
        let rand_flags = prog._options |> find_arg("_ast_synth_rflags") ?as tBool ?? false
        let report = prog._options |> find_arg("_ast_synth_report") ?as tBool ?? false
        let dump = prog._options |> find_arg("_ast_synth_dump") ?as tBool ?? false
        let keep = prog._options |> find_arg("_ast_synth_keep") ?as tInt ?? 0
        let only = prog._options |> find_arg("_ast_synth_only") ?as tString ?? ""
        g_synthed = true
        var n = 0
        var all_kinds : table<string; int>
        let nfuncs = prog._options |> find_arg("_ast_synth_funcs") ?as tInt ?? 0
        let ndecls = prog._options |> find_arg("_ast_synth_decls") ?as tInt ?? 0
        let bind_refs = prog._options |> find_arg("_ast_synth_bind") ?as tBool ?? false
        let pre_typed = prog._options |> find_arg("_ast_synth_pretype") ?as tBool ?? false
        let mutCount = prog._options |> find_arg("_ast_synth_mut_count") ?as tInt ?? 0
        g_mut_depth = prog._options |> find_arg("_ast_synth_mut_depth") ?as tInt ?? 8
        // Only the module being compiled can be mutated: a required module is already
        // inferred by the time this macro runs, so edits to it are never re-checked.
        var target : Module?
        program_for_each_module(prog) $(m) {
            if (empty(m.name)) {
                target = m
            }
        }
        if (target == null) {
            return false
        }
        if (mutCount > 0) {
            return mutate_module_many(prog, target, seedInt, mutCount, 17)
        }
        let mutAt = prog._options |> find_arg("_ast_synth_mut_at") ?as tInt ?? -1
        if (mutAt >= 0) {
            let mutKind = prog._options |> find_arg("_ast_synth_mut_kind") ?as tInt ?? 0
            return mutate_module(prog, target, seedInt, mutAt, mutKind)
        }
        let typed = prog._options |> find_arg("_ast_synth_typed") ?as tBool ?? false
        if (typed) {
            var vat = LineInfo()
            for_each_function(mod, "") $(fn) {
                vat = fn.at
            }
            let run_mode = prog._options |> find_arg("_ast_synth_run") ?as tBool ?? false
            let nadded = add_synth_typed_functions(mod, seedInt, nfuncs > 0 ? nfuncs : 3,
                depth, size, vat, run_mode)
            to_log(LOG_ERROR, "AST-SYNTH-TYPED: added {nadded} function(s)\n")
            to_log(LOG_ERROR, "AST-SYNTH: seed={seedInt} typed depth={depth} size={size}\n")
            return nadded > 0
        }
        var pool : DeclPool
        if (ndecls > 0) {
            add_synth_decls(mod, pool, seedInt, ndecls, depth)
            to_log(LOG_ERROR, "AST-SYNTH-DECLS: {length(pool.structs)} struct(s), {length(pool.enums)} enum(s), {length(pool.vars)} global(s), {length(pool.annots)} handled type(s)\n")
        }
        if (nfuncs > 0) {
            let nadded = add_synth_functions(mod, seedInt, nfuncs, depth)
            to_log(LOG_ERROR, "AST-SYNTH-FUNCS: added {nadded}/{nfuncs}\n")
        }
        if (bind_refs) {
            for_each_function(mod, "") $(var pfn) {
                pool.funcs |> push(pfn)
            }
        }
        for_each_function(mod, "") $(var fn) {
            if (!starts_with(fn.name, "synth_")) {
                return
            }
            n++
            if (!empty(only) && fn.name != only) {
                return
            }
            {
                var g = new GenAll(seed = random_seed(seedInt + n * 7919), max_depth = depth,
                    rand_flags = rand_flags, at = fn.at)
                apply_skip(g)
                fill_pools(g, pool, bind_refs, pre_typed)
                var blk = new ExprBlock(at = fn.at)
                for (_i in range(size)) {
                    var st = g.gen_expr(0)
                    blk.list |> emplace(st)
                }
                if (keep > 0) {
                    while (length(blk.list) > keep) {
                        blk.list |> erase(length(blk.list) - 1)
                    }
                }
                fn.body = blk
                if (dump) {
                    let txt = build_string() $(var w) {
                        printFunc(fn, unsafe(addr(w)))
                    }
                    to_log(LOG_ERROR, "AST-SYNTH-DUMP {fn.name}\n{txt}\n")
                }
                for (k, v in keys(g.kinds), values(g.kinds)) {
                    all_kinds[k] = (all_kinds?[k] ?? 0) + v
                }
                unsafe {
                    delete g
                }
                fn.not_inferred()
            }
        }
        if (n == 0) {
            to_log(LOG_ERROR, "AST-SYNTH: no synth_* function in the victim\n")
            return false
        }
        if (prune >= 0) {
            var pr = new _ast_synth_all::Pruner(target = prune)
            make_visitor(*pr) $(adapter) {
                visit_module(prog, adapter, mod)
            }
            to_log(LOG_ERROR, "AST-SYNTH-PRUNE: target={prune} nodes={pr.idx} replaced={pr.hit_rtti}\n")
            unsafe {
                delete pr
            }
            if (dump) {
                for_each_function(mod, "") $(var fn2) {
                    if (!starts_with(fn2.name, "synth_")) {
                        return
                    }
                    let txt2 = build_string() $(var w2) {
                        printFunc(fn2, unsafe(addr(w2)))
                    }
                    to_log(LOG_ERROR, "AST-SYNTH-PRUNED {fn2.name}\n{txt2}\n")
                }
            }
        }
        to_log(LOG_ERROR, "AST-SYNTH: seed={seedInt} bodies={n} size={size} depth={depth} kinds={length(all_kinds)}\n")
        if (report) {
            for (k, v in keys(all_kinds), values(all_kinds)) {
                to_log(LOG_ERROR, "AST-SYNTH-KIND {k} {v}\n")
            }
        }
        return true
    }
}
