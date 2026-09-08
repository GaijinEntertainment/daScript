options gen2

module aot_standalone private
require daslib/fio
require daslib/rtti
require strings
require math

require daslib/match
require daslib/strings_boost
require daslib/ast_boost
require daslib/templates_boost
require daslib/functional

require daslib/ast_print_flags
require daslib/aot_constants
require daslib/aot_cpp

options strict_smart_pointers = false


struct StandaloneContextCfg {
    //! Configuration for standalone context generation.
    context_name : string;
    class_name : string;
    cpp_output_dir : string;
    cross_platform : bool;
    //! the context links C++ modules beyond the builtin one, so its constructor registers them
    registers_modules : bool
};

def aotFunctionName(str : string) {
    return replace(str, "`", "__")
}

def writeStandaloneContextMethods(var prog : ProgramPtr; var logs : StringBuilderWriter; prefix : string; declare_only : bool; cfg : StandaloneContextCfg) {
    let fnn = collectProgramUsedFunctions(prog, false, false);

    var coll = new BlockVariableCollector();

    for (fn in fnn) {
        if (!fn.flags.exports || fn._module != prog.getThisModule) continue;
        if (declare_only) {
            write(logs, "    ");
        }
        write(logs, "auto {prefix}{aotFunctionName(string(fn.origin != null ? fn.origin.name : fn.name))} ( ");
        var vars : array<string>
        vars |> reserve(length(fn.arguments))
        for (variable in fn.arguments) {
            let type_str = build_string() $(var wr) {
                if (isLocalVec(variable._type)) {
                    describeLocalCppType(unsafe(addr(wr)), variable._type, cfg.cross_platform);
                } else {
                    write(wr, describeCppType(variable._type, DescribeConfig(cross_platform = cfg.cross_platform)));
                }
            }
            vars.push("{type_str} {variable._type.isRefType ? "& " : ""}{coll.getVarName(variable)}")
        }
        let vars_str = join(vars, ", ");
        write(logs, "{vars_str}{!fn.arguments.empty() ? " " : ""}) -> ")
        describeLocalCppType(unsafe(addr(logs)), fn.result, cfg.cross_platform, CpptSubstitureRef.no, CpptSkipConst.yes);
        if (declare_only) {
            write(logs, ";\n");
        } else {
            let args_str = (fn.arguments |> each()
                                        |> map(@(v : VariablePtr) { return coll.getVarName(v); })
                                        |> join(", "))
            let maybe_comma = !empty(args_str) ? ", " : "";
            write(logs, " \{\n    return {aotFuncName(fn)}(this{maybe_comma}{args_str});\n\}\n\n");
        }
    }
}

def collectInitFunctionAotNames(program : ProgramPtr; var context : Context) : array<string> {
    //! In the simulated context's run order (late init last).
    var initMnhOrder : array<uint64>
    context |> for_each_init_function($(mnh : uint64) {
        initMnhOrder |> push(mnh)
    });
    var initMnhSet : table<uint64>
    for (mnh in initMnhOrder) {
        initMnhSet |> insert(mnh)
    }
    var mnhToAotName : table<uint64; string>
    let thisModule = program.getThisModule
    program.get_ptr() |> for_each_module_no_order($(pm) {
        pm |> for_each_module_function($(pfun) {
            if (function_index(program, pfun) < 0 || !is_used(program, pfun) || !(initMnhSet |> key_exists(pfun.getMangledNameHash))) {
                return
            }
            if (pm != thisModule) {
                macro_error(program, pfun.at, "standalone AOT cannot run [init] function: {pm.name}::{pfun.name} lives outside the main module")
            } elif (pfun.flags.noAot) {
                macro_error(program, pfun.at, "standalone AOT cannot run [init] function: {pfun.name} is [no_aot]")
            } else {
                mnhToAotName[pfun.getMangledNameHash] = aotFuncName(pfun)
            }
        });
    });
    var initNames : array<string>
    for (mnh in initMnhOrder) {
        let name = mnhToAotName?[mnh] ?? ""
        if (!empty(name)) {
            initNames |> push(name)
        } elif (!program.flags.failToCompile) {
            macro_error(program, LineInfo(), "standalone AOT cannot run [init] function: mangled name hash {mnh} is not in the emitted program")
        }
    }
    return <- initNames
}

//! The globals the context carries, in the order the constructor initializes them; a used global
//! without an index is the compiler's error, collected like every other emit error.
def private usedGlobals(var program : ProgramPtr) : array<Variable?> {
    var globals : array<Variable?>
    if (program.totalVariables > 0) {
        program.get_ptr() |> for_each_module_no_order($(pm) {
            pm |> for_each_global($(pvar) {
                if (!is_used(program, pvar)) return
                if (variable_index(program, pvar) < 0) {
                    macro_error(program, pvar.at, "standalone AOT: global {pvar.name} is used but was never allocated an index")
                    return
                }
                globals.push(pvar);
            });
        });
    }
    return <- globals
}

//! `options gc`: the program collects its own heaps.
def private standaloneCollects(program : ProgramPtr) : bool {
    return program._options |> find_arg("gc") ?as tBool ?? false
}

//! A `VarInfo` per global, made before the helper dumps its tables so every type they name is in
//! the dump: the collector walks each global through its info, the way the interpreter's
//! simulate step wires it.
def private makeGlobalVarInfos(program : ProgramPtr; var helper : AotDebugInfoHelper?; globals : array<Variable?>) : array<tuple<index : int; info : VarInfo?>> {
    return <- [for (pvar in globals); (index = variable_index(program, pvar), info = helper.helper |> make_variable_debug_info(pvar))]
}

def private writeGlobalVarInfos(var helper : AotDebugInfoHelper?; infos : array<tuple<index : int; info : VarInfo?>>; var tw : StringBuilderWriter) {
    for (entry in infos) {
        let suffix = "_gvar_{entry.index}"
        helper->writeDim(unsafe(addr(tw)), entry.info, suffix)
        helper->writeArgTypes(unsafe(addr(tw)), entry.info, suffix)
        helper->writeArgNames(unsafe(addr(tw)), entry.info, suffix)
        write(tw, "VarInfo __gvar_info_{entry.index} = \{ {helper->describeCppVarFuncInfo(entry.info.name, entry.info, suffix)} \};\n")
    }
}

//! the sealed lookup, written as the constant arrays the generated constructor adopts: a NameLookup::StaticTable
//! named `<prefix>_table` and the four arrays it points at
def private writeLookupTables(var tw : StringBuilderWriter; prefix : string; lookup : NameLookup?) {
    write(tw, "    static const NameLookup::Entry {prefix}_entries[] = \{\n")
    for (i in urange(name_lookup_mnh_slots(lookup))) {
        write(tw, "        \{0x{name_lookup_entry_mnh(lookup, i):x}ull, {name_lookup_entry_value(lookup, i):d}u, {name_lookup_entry_index(lookup, i):d}u, {name_lookup_entry_next(lookup, i)}, 0\},\n")
    }
    write(tw, "    \};\n")
    write(tw, "    static const NameLookup::NameSlot {prefix}_names[] = \{\n")
    for (i in urange(name_lookup_name_slots(lookup))) {
        write(tw, "        \{0x{name_lookup_name_hash(lookup, i):x}ull, {name_lookup_name_head(lookup, i)}, 0\},\n")
    }
    write(tw, "    \};\n")
    write(tw, "    static const uint32_t {prefix}_mnh_disp[] = \{")
    for (i in urange(name_lookup_mnh_buckets(lookup))) {
        write(tw, " {name_lookup_mnh_disp(lookup, i):d}u,")
    }
    write(tw, " \};\n")
    write(tw, "    static const uint32_t {prefix}_name_disp[] = \{")
    for (i in urange(name_lookup_name_buckets(lookup))) {
        write(tw, " {name_lookup_name_disp(lookup, i):d}u,")
    }
    write(tw, " \};\n")
    write(tw, "    static const NameLookup::StaticTable {prefix}_table = \{ {name_lookup_mnh_buckets(lookup):d}u, {name_lookup_mnh_slots(lookup):d}u, {name_lookup_name_buckets(lookup):d}u, {name_lookup_name_slots(lookup):d}u, {name_lookup_count(lookup):d}u, 0u, {prefix}_mnh_disp, {prefix}_name_disp, {prefix}_entries, {prefix}_names \};\n")
}

//! the standalone context's function lookup: its table is dense, one slot per emitted function in emission
//! order, so the index is the position in `usedFunctions`, never the compiler's function index
def private writeFunctionLookup(var tw : StringBuilderWriter; usedFunctions : array<Function?>) {
    var lookup = unsafe(name_lookup_create())
    var index = 0u
    for (pfun in usedFunctions) {
        name_lookup_insert(lookup, pfun.getMangledNameHash, string(pfun.name), index, index)
        index++
    }
    name_lookup_seal(lookup)
    writeLookupTables(tw, "fn_lookup", lookup)
    unsafe {
        name_lookup_destroy(lookup)
    }
}

//! the standalone context's global lookup: the value is the offset InitGlobalVariable assigns at construction -
//! the 16-byte-rounded running size of the shared or the plain globals, in declaration order - and the
//! generated constructor verifies every global against it, so the two computations cannot drift apart silently
def private writeVariableLookup(program : ProgramPtr; var tw : StringBuilderWriter; globals : array<Variable?>) {
    var lookup = unsafe(name_lookup_create())
    var globalsSize = 0u
    var sharedSize = 0u
    for (pvar in globals) {
        let size = (uint(pvar._type.sizeOf) + 15u) & ~15u
        let offset = pvar.flags.global_shared ? sharedSize : globalsSize
        if (pvar.flags.global_shared) {
            sharedSize += size
        } else {
            globalsSize += size
        }
        name_lookup_insert(lookup, hash(pvar |> get_mangled_name()), string(pvar.name), uint(variable_index(program, pvar)), offset)
    }
    name_lookup_seal(lookup)
    writeLookupTables(tw, "var_lookup", lookup)
    unsafe {
        name_lookup_destroy(lookup)
    }
}

def writeStandaloneCtor(cfg : StandaloneContextCfg; initFunctions : string; var tw : StringBuilderWriter, program : ProgramPtr; lookupVariableTable : array<Variable?>; var context : Context) {   // nolint:STYLE038 - flat emitter - one write per ctor section

    let min_init_stack = 16384
    let stack_arg = program._options |> find_arg("stack")
    let requested_stack = stack_arg ?as tInt ?? int(program.policies.stack)
    let stack_base = requested_stack > 0 && stack_arg is tInt ? requested_stack : max(requested_stack, min_init_stack)
    let stack_size = stack_base + int(program.globalInitStackSize)
    let usedFunctions <- collectProgramUsedFunctions(program, true, false)
    let usedFunctionCount = length(usedFunctions)
    write(tw, "{cfg.class_name}::{cfg.class_name}() : Context({stack_size}/*stack*/) \{\n");
    write(tw, "    auto & context = *this;\n");
    write(tw, "    CodeOfPolicies policies;");
    write(tw, "    policies.debugger = {program.policies.debugger} /*policies.debugger*/;\n");
    write(tw, "    policies.persistent_heap = {program._options |> find_arg("persistent_heap") ?as tBool ?? program.policies.persistent_heap};\n");
    let heap_hint = uint(program._options |> find_arg("heap_size_hint") ?as tInt ?? int(program.policies.heap_size_hint))
    let string_heap_hint = uint(program._options |> find_arg("string_heap_size_hint") ?as tInt ?? int(program.policies.string_heap_size_hint))
    write(tw, "    policies.heap_size_hint = {heap_hint:d};\n");
    write(tw, "    policies.string_heap_size_hint = {string_heap_hint:d};\n");
    write(tw, "    policies.track_allocations = {program._options |> find_arg("track_allocations") ?as tBool ?? program.policies.track_allocations};\n");
    write(tw, "    context.setup({program.totalVariables}/*totalVariables*/, {program.globalStringHeapSize:d} /*globalStringHeapSize*/, policies, \{\});\n");
    // the nano context has no collector flag
    if (standaloneCollects(program)) {
        write(tw, "    context.gcEnabled = true;\n");
    }

    write(tw, "     // start totalVariables\n");
    for (pvar in lookupVariableTable) {
        let vindex = variable_index(program, pvar)
        write(tw, "    InitGlobalVar(context, &context.globalVariables[{vindex}/*pvar->index*/], GlobalVarInfo(\"")
        write(tw, "{pvar.name}\", \"{pvar |> get_mangled_name()}\", {pvar._type.sizeOf}, {pvar.flags.global_shared}));\n");
        write(tw, "    context.globalVariables[{vindex}].debugInfo = &__gvar_info_{vindex};\n");
    }
    write(tw, "     // end totalVariables\n\n");
    write(tw, "    context.allocateGlobalsAndShared();\n");
    write(tw, "    if ( context.globals ) memset(context.globals, 0, context.getGlobalSize());\n");
    write(tw, "    context.totalVariables = {program.totalVariables}/*totalVariables*/;\n");
    write(tw, "    context.functions = (SimFunction *) context.code->allocate( {usedFunctionCount}/*totalFunctions*/*sizeof(SimFunction) );\n");
    write(tw, "    context.totalFunctions = {usedFunctionCount}/*totalFunctions*/;\n");
    write(tw, "    bool anyPInvoke = false;\n");

    write(tw, "    if ( anyPInvoke || {program.policies.threadlock_context || program.policies.debugger}");
    write(tw, "/*(policies.threadlock_context || policies.debugger)*/ ) \{\n");
    write(tw, "        context.contextMutex = new recursive_mutex;\n");
    write(tw, "    }\n");
    writeFunctionLookup(tw, usedFunctions)
    write(tw, "    context.functionLookup = make_shared<NameLookup>();\n");
    write(tw, "    context.functionLookup->adopt(fn_lookup_table);\n");

    if (!empty(initFunctions)) {
        write(tw, "     // start totalFunctions\n");
        write(tw, "    struct FunctionStorage \{ int idx; FunctionInfo funcInfo; FuncInfo* debugInfo; \};\n");
        write(tw, "    FunctionStorage usedFunctions[] = \{\n");
        write(tw, "{initFunctions}");
        write(tw, "    \};\n");
        write(tw, "    // end totalFunctions\n");
        write(tw, "    vector<pair<uint64_t, SimFunction*>> id_to_funcs;\n");
        write(tw, "    for (const auto& [index, func_info, debug_info]: usedFunctions) \{\n");
        write(tw, "        InitAotFunction(context, &context.functions[index], func_info);\n");
        write(tw, "        context.functions[index].debugInfo = debug_info;\n");
        write(tw, "        DAS_VERIFYF(context.functionLookup->valueByMnh(func_info.mnh)==uint32_t(index), \"standalone function '%s' sits at index %d, its emitted lookup says %u\", func_info.name.c_str(), int(index), context.functionLookup->valueByMnh(func_info.mnh));\n");
        write(tw, "        id_to_funcs.emplace_back(func_info.aotHash, &context.functions[index]);\n");
        write(tw, "        anyPInvoke |= func_info.pinvoke;\n");
        write(tw, "    \}\n");
    }

    writeVariableLookup(program, tw, lookupVariableTable)
    write(tw, "    context.variableLookup = make_shared<NameLookup>();\n");
    write(tw, "    context.variableLookup->adopt(var_lookup_table);\n");
    write(tw, "    for ( int i=0, is=context.totalVariables; i!=is; ++i ) \{\n");
    write(tw, "        auto & gvar = context.globalVariables[i];\n");
    write(tw, "        DAS_VERIFYF(context.variableLookup->valueByMnh(gvar.mangledNameHash)==gvar.offset, \"standalone global '%s' sits at offset %u, its emitted lookup says %u\", gvar.name, gvar.offset, context.variableLookup->valueByMnh(gvar.mangledNameHash));\n");
    write(tw, "    \}\n");
    write(tw, "    context.tabAdLookup = make_shared<das_hash_map<uint64_t,uint64_t>>();\n");
    program.get_ptr() |> for_each_module_no_order($(pm) {
        pm |> for_each_annotation_ordered($(k; v) {
            write(tw, "    (*context.tabAdLookup)[{k : x}] = {v};\n");
        });
    });

    if (!empty(initFunctions)) {
        write(tw, "    FillFunction(context, getGlobalAotLibrary(), id_to_funcs);\n");
    }
    write(tw, "    __init_script(&context, true);\n");
    for (initFn in collectInitFunctionAotNames(program, context)) {
        write(tw, "    {initFn}(&context);\n");
    }
    write(tw, "\}\n");
}

def writeStandaloneContext(var program : ProgramPtr, initFunctions : string, var header : StringBuilderWriter, var source : StringBuilderWriter; cfg : StandaloneContextCfg; globals : array<Variable?>; var context : Context) {

    write(header, "\n\n");
    //! the module scope is the FIRST base: constructed before Context, destroyed after it, so the
    //! registry outlives the context's own teardown
    let bases = cfg.registers_modules ? "public StandaloneModuleScope, public Context" : "public Context"
    write(header, "class {cfg.class_name} : {bases} \{\n");
    write(header, "public:\n");
    write(header, "    {cfg.class_name}();\n");
    writeStandaloneContextMethods(program, header, "", true, cfg);
    write(header, "\};\n");

    writeStandaloneContextMethods(program, source, "{cfg.class_name}::", false, cfg);
    writeStandaloneCtor(cfg, initFunctions, source, program, globals, context);
}

class StandaloneContextGen : CppAot {
    //! AOT visitor specialized for standalone context code generation.

    def StandaloneContextGen(var program_ : ProgramPtr; var ss_ : StringBuilderWriter?, var coll : BlockVariableCollector?, cp : bool) {
        ss <- ss_;
        collector <- coll;
        program := program_;
        cross_platform = cp;
        prologue = program._options |> find_arg("aot_prologue") ?as tBool ?? false;
        solidContext = program.policies.solid_context || (program._options |> find_arg("solid_context") ?as tBool ?? false);
        helper.cross_platform = cp;
    }

    used_functions : table<string>;

    def override preVisitGlobalLet(prog : ProgramPtr) {
        CppAot`preVisitGlobalLet(self, prog);
        var globals : array<Variable?>;
        prog.get_ptr() |> for_each_module($(pm) {
            pm |> for_each_global($(pvar) {
                if (variable_index(prog, pvar) < 0 || !is_used(prog, pvar)
                        || pvar._module == prog.getThisModule) return ;
                globals.push(pvar);
            });
        });
        for (variable in globals) {
            preVisitGlobalLetVariable(variable, false);
            if (variable.init != null) {
                preVisitGlobalLetVariableInit(variable, variable.init);
                visit_expression(variable.init, adapter);
            }
            let varn <- visitGlobalLetVariable(variable, false);
        }
    }

    def override preVisitProgramBody(var prog : ProgramPtr, that : Module?) {
        declarations = ss |> string_builder_str();
        ss |> string_builder_clear();
        write(*ss, "\n");
        // every used function of every module, declared once (a shared generic instance has one name)
        let fnn = collectProgramUsedFunctions(prog, true, false);
        var inline_fns : array<string>
        inline_fns |> reserve(length(fnn))
        for (pfun in fnn) {
            let name = aotFuncName(pfun)
            if (used_functions |> key_exists(name)) continue
            inline_fns.push(describeCppFunc(pfun, collector, cross_platform, true, true));
            used_functions.insert(name);
        }
        let sep = ";\n";
        let maybe_sem = inline_fns |> empty() ? "" : sep;
        write(*ss, "{inline_fns |> join(sep)}{maybe_sem}\n")
    }
    tw : StringBuilderWriter ?
};


def writeModuleDeclarations(var header, source : StringBuilderWriter; registrations : array<StandaloneModuleEntry>) {
    //! the header carries the registry include (the class derives from its scope base); the
    //! source carries the module declarations its registration table takes addresses of
    if (empty(registrations)) return
    write(header, "#include \"daScript/simulate/standalone_modules.h\"\n");
    write(source, "#include \"daScript/simulate/standalone_modules.h\"\n");
    for (entry in registrations) {
        write(source, "DECLARE_MODULE({entry.mod.cppClassName});\n");
    }
}

def writeModuleRegistration(var source : StringBuilderWriter; registrations : array<StandaloneModuleEntry>) {
    //! This TU's linked C++ modules join the process-wide list before main; the first context's
    //! `StandaloneModuleScope` base (standalone_modules.h) registers the union once, and the last
    //! context's destruction shuts down, so several contexts in one binary share one lifetime.
    if (empty(registrations)) return
    write(source, "static const StandaloneModule das_standalone_modules[] = \{\n");
    for (entry in registrations) {
        write(source, "    \{ \"{entry.mod.name}\", &::register_{entry.mod.cppClassName}, {entry.rank} \},\n");
    }
    write(source, "\};\n");
    write(source, "static const bool das_standalone_modules_added = standaloneAddModules(das_standalone_modules, {length(registrations)});\n\n");
}

def writeRegistration(var header : StringBuilderWriter;
                      var source : StringBuilderWriter;
                      initFunctions : string;
                      var program : ProgramPtr;
                      cfg : StandaloneContextCfg;
                      registrations : array<StandaloneModuleEntry>;
                      globals : array<Variable?>;
                      var context : Context) {
    write(source, "using namespace {program.thisNamespace};\n");
    write(header, "namespace {cfg.context_name} \{\n");
    write(source, "namespace {cfg.context_name} \{\n");
    dumpRegisterAot(unsafe(addr(source)), program, context, true, cfg.cross_platform);
    writeModuleRegistration(source, registrations);
    writeStandaloneContext(program, initFunctions, header, source, cfg, globals, context);
    write(header, "\} // namespace {cfg.context_name}\n");
    write(source, "\} // namespace {cfg.context_name}\n");
}

def GetFunctionInfo(pfun : Function?; index : int; info : string) {
    return build_string() $(tw) {
        let args = ["\"{pfun.name}\"",
                    "\"{pfun |> get_mangled_name()}\"",
                    "0x{pfun.getMangledNameHash:x}",
                    "0x{pfun.aotHash:x}",
                    "{pfun.totalStackSize:d}",
                    "{pfun.flags.unsafeOperation}",
                    "{pfun.flags.fastCall}",
                    "{pfun._module.moduleFlags.builtIn}",
                    "{pfun._module.moduleFlags.promoted}",
                    "{pfun.result.isRefType && !pfun.result.flags.ref}",
                    "{pfun.moreFlags.pinvoke}"]
        write(tw, "        \{{index}, FunctionInfo({args|>join(", ")}), &{info}\},\n");
    }
}

def addFunctionInfo(fnn : array<Function?>; var helper : AotDebugInfoHelper?) {
    var lookupFunctionTable : array<tuple<Function?, FuncInfo?>>
    lookupFunctionTable |> reserve(length(fnn))
    for (pfun in fnn) {
        let info = helper.helper |> make_function_debug_info(pfun);
        lookupFunctionTable.push((pfun, info));
    }

    //! the table is dense: a slot per function the context still reaches, in emission order,
    //! so a standalone context carries no row for what the interpreter would have removed
    return build_string() $(tw) {
        var index = 0
        for ((fun_info, info) in lookupFunctionTable) {
            write(tw, GetFunctionInfo(fun_info, index, funcInfoName(info)));
            index++
        }
    }
}


def genStandaloneSrc(var program : ProgramPtr;
                     var header : StringBuilderWriter;
                     var source : StringBuilderWriter; cfg : StandaloneContextCfg;
                     var coll : BlockVariableCollector?; globals : array<Variable?>) {
    var initFunctions : string;
    var type_defs = "";
    let ctx_generated = build_string() $(tw) {
        type_defs = build_string() $(tmp_writer) {
            var cpp_aot = new CppAot(program := program, collector = coll, ss = unsafe(addr(tmp_writer)), cross_platform = cfg.cross_platform);
            make_visitor(*cpp_aot) $(aot_adapter) {
                cpp_aot.adapter := aot_adapter
                dumpDependencies(program, cpp_aot, true);
            }
            unsafe {
                delete cpp_aot
            }
        }
        build_string() $(tmp_writer) {
            var gen = new StandaloneContextGen(program, unsafe(addr(tmp_writer)), coll, cfg.cross_platform);
            make_visitor(*gen) $(adapter) {
                gen.adapter := adapter
                program |> visit_module(adapter, program.getThisModule);
                // a generic instance several modules instantiated is one C++ function: emit the first copy only
                var emitted : table<string>
                for (pfun in collectProgramUsedFunctions(program, false, false)) {
                    emitted |> insert(aotFuncName(pfun))
                }
                var foreign <- foreignUsedFunctions(program, false)
                for (fn in foreign) {
                    let name = aotFuncName(fn)
                    if (emitted |> key_exists(name)) continue
                    emitted |> insert(name)
                    visit(fn, adapter)
                }
            }

            initFunctions = addFunctionInfo(collectProgramUsedFunctions(program, true, false), gen.helper);
            let globalInfos <- makeGlobalVarInfos(program, gen.helper, globals)
            type_defs += gen.declarations;
            gen.declarations = "";
            write(tw, gen.str());
            writeGlobalVarInfos(gen.helper, globalInfos, tw)
            unsafe {
                delete gen.collector
                delete gen
            }
        }
    }
    write(header, type_defs);
    write(source, "namespace {program.thisNamespace} \{\n");
    write(source, "{ctx_generated}");
    write(source, "\} // namespace {program.thisNamespace}\n");
    return initFunctions
}

def private isMarkedNoAot(pfun : Function?) : bool {
    for (ann in pfun.annotations) {
        if (ann.annotation.name == "no_aot") {
            return true
        }
    }
    return false
}

def private checkAllUsedFunctionsCanAot(program : ProgramPtr) {
    program.get_ptr() |> for_each_module_no_order($(pm) {
        pm |> for_each_module_function($(pfun) {
            if (function_index(program, pfun) < 0 || !is_used(program, pfun) || pfun.flags.builtIn) {
                return
            }
            if (pfun.flags.noAot) {
                if (isMarkedNoAot(pfun)) {
                    macro_error(program, pfun.at, "standalone AOT cannot emit function {pfun.name}: it is [no_aot], and a standalone context has no interpreter")
                } else {
                    macro_error(program, pfun.at, "standalone AOT cannot emit function {pfun.name}: it uses a type AOT cannot express, and a standalone context has no interpreter")
                }
            }
        });
    });
}

def private prepareProgramForEmission(var program : ProgramPtr; context : Context) : BlockVariableCollector? {
    var noAotMarker = new NoAotMarker();
    var unmarked <- foreignUsedFunctions(program, true)
    make_visitor(*noAotMarker) $(adapter_no_aot) {
        visit(program, adapter_no_aot)
        for (fn in unmarked) {
            visit(fn, adapter_no_aot)
        }
    }
    checkAllUsedFunctionsCanAot(program)
    var foreign <- foreignUsedFunctions(program, false)

    var pmarker = new PrologueMarker();
    make_visitor(*pmarker) $(adapter_p) {
        visit(program, adapter_p)
        for (fn in foreign) {
            visit(fn, adapter_p)
        }
    }

    setAotHashes(program, context);

    var flags = new SetPrinterFlags();
    make_visitor(*flags) $(flags_adapter) {
        visit(program, flags_adapter)
        for (fn in foreign) {
            visit(fn, flags_adapter)
        }
    }
    unsafe {
        delete flags
        delete pmarker
        delete noAotMarker
    }

    var coll = new BlockVariableCollector();
    make_visitor(*coll) $(adapter_coll) {
        visit(program, adapter_coll)
        for (fn in foreign) {
            visit(fn, adapter_coll)
        }
        // foreign globals initialize in __init_script too: their temporaries collect under the null key
        let thisModule = program.getThisModule
        program.get_ptr() |> for_each_module_no_order($(pm) {
            if (pm == thisModule) return
            pm |> for_each_global($(pvar) {
                if (variable_index(program, pvar) < 0 || !is_used(program, pvar) || pvar.init == null) return
                visit_expression(pvar.init, adapter_coll)
            })
        })
    }
    return coll
}

def public runStandaloneVisitor(var program : ProgramPtr, modules : array<string>; registrations : array<StandaloneModuleEntry>; var pctx : smart_ptr<Context>; cfg : StandaloneContextCfg) : bool {
    //! Runs the standalone AOT visitor on the program to generate C++ source and header files.
    //! `registrations` are the C++ modules the generated constructor registers, in order.
    //! Returns false (writing nothing) when emission collected errors.
    assume context = *pctx;

    var coll = prepareProgramForEmission(program, context)
    let globals <- usedGlobals(program)

    let mod = program.getThisModule;
    let mod_name = mod.moduleFlags.promoted ? "" : string(mod.name);
    set_aot_main_module_name(cfg.context_name)
    var source_content = ""
    let header_content = build_string() $(header) {
    source_content = build_string() $(source) {
        write(header, "#pragma once\n");
        if (!empty(mod_name)) {
            write(header, "// Module {mod_name}\n");
        }
        write(header, "{AOT_INCLUDES}");
        write(header, "{join(modules, "")}");

        if (!empty(mod_name)) {
            write(source, "// Module {mod_name}\n");
        }

        write(source, "#include \"daScript/simulate/standalone_ctx_utils.h\"\n");
        write(source, "{join(modules, "")}");
        writeModuleDeclarations(header, source, registrations);
        write(source, "#include \"{mod.name.empty() ? cfg.context_name : string(mod.name)}.das.h\"\n\n");
        write(source, AOT_HEADERS);

        write(source, "namespace das \{\n");
        write(header, "namespace das \{\n");
            let initFunctions = genStandaloneSrc(program, header, source, cfg, coll, globals);
            writeRegistration(header, source, initFunctions, program, cfg, registrations, globals, context);
        write(source, "\} // namespace das\n");
        write(header, "\} // namespace das\n");

        write(source, AOT_FOOTER);

    }
    }
    set_aot_main_module_name("")

    if (log_aot_emit_errors(program)) {
        return false
    }

    let cur_mod = mod.name.empty() ? cfg.context_name : string(mod.name);
    let outputFile = "{cfg.cpp_output_dir}/{cur_mod}.das";
        fopen("{outputFile}.h", "wb") $(fw) {
            if (fw != null) {
                fwrite(fw, header_content)
            } else {
                print("Couldn't create output file {outputFile}.h")
            }
        }
    fopen("{outputFile}.cpp", "wb") $(fw) {
        if (fw != null) {
            fwrite(fw, source_content)
        } else {
            print("Couldn't create output file {outputFile}.cpp")
        }
    }
    return true
}

[export]
def public standalone_aot(input : string; output_dir : string; cross_platform : bool, paranoid_validation : bool; cop : CodeOfPolicies) {
    //! Compiles a daslang file and generates standalone AOT C++ code in the given output directory.
    return standalone_aot(input, output_dir, cross_platform, paranoid_validation, cop) $(var _program : ProgramPtr) {}
}

[export, unused_argument(paranoid_validation)]
def public standalone_aot(input : string; output_dir : string; cross_platform : bool, paranoid_validation : bool; cop : CodeOfPolicies; before_emit : block<(var program : ProgramPtr) : void>) {
    //! Compiles a daslang file and generates standalone AOT C++ code in the given output directory.
    //! `before_emit` receives the compiled program, so a driver can read annotations
    //! without compiling the program a second time.
    let file_name = input |> split_by_chars("/\\") |> back()
    let ctx_name = (file_name |> split("."))[0]
    var cfg = StandaloneContextCfg(context_name = ctx_name,
                                   class_name = "Standalone",
                                   cpp_output_dir = output_dir,
                                   cross_platform = cross_platform)
    var ok = false
    using() $(var mg : ModuleGroup) {
        var inscope access <- make_file_access("")
        compile_and_simulate(input, access, unsafe(addr(mg)), cop) $(var program : ProgramPtr; var pctx : smart_ptr<Context>) {
            let (modules_str, noAotModule) = getRequiredModulesFor(program, true)
            if (program._options |> find_arg("no_aot") ?as tBool ?? false) {
                panic("Standalone context called on non aot module {input}")
            } elif (noAotModule) {
                panic("Standalone context called on non aot module {input}")
            } else {
                before_emit |> invoke(program)
                let registrations <- standaloneModuleRegistration(program)
                cfg.registers_modules = !empty(registrations)
                ok = runStandaloneVisitor(program, modules_str, registrations, pctx, cfg)
            }
        }
    }
    return ok
}
