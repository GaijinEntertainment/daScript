options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_list_module_api(module_name : string; filter : string = ""; section : string = "") : string {
    // write a temp stub file that requires the module
    let stub_path = "{get_das_root()}/_mcp_stub.das"
    let stub = "require {module_name}\n[export]\ndef main\n    pass\n"
    fopen(stub_path, "w") <| $(f) {
        fwrite(f, stub)
    }
    var inscope access <- make_file_access("")
    var result : string
    var had_error = false
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(stub_path, access, unsafe(addr(mg)), cop) <| $(ok; program; issues) {
                if (!ok) {
                    result = "Failed to compile stub requiring '{module_name}':\n{issues}"
                    had_error = true
                } else {
                    simulate(program) <| $(sok; ctx; serrors) {
                        if (!sok) {
                            result = "Simulation failed:\n{serrors}"
                            had_error = true
                            return
                        }
                        // find the target module
                        var target_mod : Module?
                        program_for_each_module(program) <| $(mod) {
                            let mname = string(mod.name)
                            if (mname == module_name || "{module_name}" |> ends_with("/{mname}")) {
                                target_mod = mod
                            }
                        }
                        if (target_mod == null) {
                            result = "Module '{module_name}' not found after compilation"
                            had_error = true
                            return
                        }
                        result = build_string() <| $(var writer) {
                            write_module_api(writer, target_mod, filter, section)
                        }
                        if (empty(result)) {
                            result = "(empty module)"
                        }
                    }
                }
            }
        }
    }
    return make_tool_result(result, had_error)
}

def is_operator_function(name : string) : bool {
    let ch = character_at(name, 0)
    return !is_alpha(ch) && !is_number(ch) && ch != '_'
}

def arg_needs_documenting(tt) : bool {
    return tt.baseType != Type.fakeContext && tt.baseType != Type.fakeLineInfo
}

def write_module_func(var w : StringBuilderWriter; func) {
    write(w, "  def {func.name}")
    var has_args = false
    for (i in range(length(func.arguments))) {
        if (!arg_needs_documenting(func.arguments[i]._type)) {
            continue
        }
        if (!has_args) {
            write(w, "(")
            has_args = true
        } else {
            write(w, "; ")
        }
        write(w, "{func.arguments[i].name} : {describe(func.arguments[i]._type)}")
    }
    if (has_args) {
        write(w, ")")
    }
    if (func.result != null && !func.result.isVoid) {
        write(w, " : {describe(func.result)}")
    }
    if (int(func.at.line) > 0) {
        write(w, "  // line {int(func.at.line)}")
    }
}

def name_matches_filter(name : string; filter : string) : bool {
    return empty(filter) || find(name, filter) >= 0
}

def want_section(section : string; name : string) : bool {
    return empty(section) || section == name
}

def write_module_api(var writer : StringBuilderWriter; mod : Module?; filter : string = ""; section : string = "") {
    write(writer, "Module: {mod.name}\n")
    var funcs : array<string>
    var generics : array<string>
    var structs : array<string>
    var enums : array<string>
    var globals : array<string>
    // functions via AST
    if (want_section(section, "functions")) {
        for_each_function(mod, "") <| $(func) {
            if (func.flags.privateFunction || func.flags._lambda || func.flags.generated) {
                return
            }
            if (is_operator_function(string(func.name))) {
                return
            }
            if (!name_matches_filter(string(func.name), filter)) {
                return
            }
            var entry = build_string() <| $(var w) {
                write_module_func(w, func)
            }
            funcs |> push(entry)
        }
    }
    // generics via AST
    if (want_section(section, "generics")) {
        for_each_generic(mod) <| $(func) {
            if (func.flags.privateFunction || func.flags.generated || string(func.name) |> starts_with("```")) {
                return
            }
            if (is_operator_function(string(func.name))) {
                return
            }
            if (!name_matches_filter(string(func.name), filter)) {
                return
            }
            var entry = build_string() <| $(var w) {
                write_module_func(w, func)
            }
            generics |> push(entry)
        }
    }
    // structures via AST
    if (want_section(section, "structs")) {
        for_each_structure(mod) <| $(value) {
        if (value.flags.isLambda || value.flags._generator || value.flags.generated || value.flags.privateStructure) {
            return
        }
        if (!name_matches_filter(string(value.name), filter)) {
            return
        }
        var entry = build_string() <| $(var w) {
            if (value.flags.isClass) {
                write(w, "  class {value.name}")
            } else {
                write(w, "  struct {value.name}")
            }
            if (int(value.at.line) > 0) {
                write(w, "  // line {int(value.at.line)}")
            }
            write(w, "\n")
            for (fld in value.fields) {
                write(w, "    {fld.name} : {describe(fld._type)}\n")
            }
        }
        structs |> push(entry)
        }
    }
    // enumerations via AST
    if (want_section(section, "enums")) {
        for_each_enumeration(mod) <| $(value) {
            if (value.isPrivate) {
                return
            }
            if (!name_matches_filter(string(value.name), filter)) {
                return
            }
            var entry = build_string() <| $(var w) {
                write(w, "  enum {value.name}")
                if (int(value.at.line) > 0) {
                    write(w, "  // line {int(value.at.line)}")
                }
                write(w, "\n")
                for (en in value.list) {
                    write(w, "    {en.name}\n")
                }
            }
            enums |> push(entry)
        }
    }
    // globals via AST
    if (want_section(section, "globals")) {
        for_each_global(mod) <| $(value) {
            if (value.flags.private_variable) {
                return
            }
            if (!name_matches_filter(string(value.name), filter)) {
                return
            }
            var line = "  {value.name} : {describe(value._type)}"
            if (int(value.at.line) > 0) {
                line = "{line}  // line {int(value.at.line)}"
            }
            globals |> push("{line}\n")
        }
    }
    if (!empty(funcs)) {
        write(writer, "Functions ({length(funcs)}):\n")
        for (f in funcs) {
            write(writer, "{f}\n")
        }
    }
    if (!empty(generics)) {
        write(writer, "Generics ({length(generics)}):\n")
        for (g in generics) {
            write(writer, "{g}\n")
        }
    }
    if (!empty(structs)) {
        write(writer, "Structs/Classes:\n")
        for (s in structs) {
            write(writer, s)
        }
    }
    if (!empty(enums)) {
        write(writer, "Enumerations:\n")
        for (e in enums) {
            write(writer, e)
        }
    }
    if (!empty(globals)) {
        write(writer, "Globals:\n")
        for (g in globals) {
            write(writer, g)
        }
    }
}
