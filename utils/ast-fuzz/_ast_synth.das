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

def private add_synth_functions(mod : Module?; seedInt : int; count : int; depth : int) : int {
    var seed = random_seed(seedInt * 31 + 17)
    var added = 0
    for (i in range(count)) {
        var g = new GenAll(seed = random_seed(seedInt + i * 6151), max_depth = depth,
            at = LineInfo())
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
        let skip_kinds = prog._options |> find_arg("_ast_synth_skip") ?as tString ?? ""
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
                fill_pools(g, pool, bind_refs, pre_typed)
                for (sk in split(skip_kinds, ",")) {
                    if (!empty(strip(sk))) {
                        g.skip[strip(sk)] := true
                    }
                }
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
