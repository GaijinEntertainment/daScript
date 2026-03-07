options gen2
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/json_boost public
require ast public
require rtti public
require strings public
require fio public

struct ContentItem {
    @rename = "type" _type : string
    text : string
}

struct ToolResult {
    content : array<ContentItem>
    isError : bool
}

// query matching: case-insensitive substring by default, "=foo" for exact case-sensitive
def symbol_matches(name, query : string) : bool {
    if (length(query) > 0 && character_at(query, 0) == '=') {
        return name == slice(query, 1)
    }
    return find(to_lower(name), to_lower(query)) >= 0
}

def make_tool_result(text : string; is_error : bool = false) : string {
    var result : ToolResult
    result.isError = is_error
    result.content |> emplace(ContentItem(_type = "text", text = text))
    return sprint_json(result, false)
}

def compile_and_simulate(file : string; project : string = ""; blk : block<(program : smart_ptr<Program>; issues : string) : string>) : string {
    return compile_program(file, false, false, project) <| blk
}

def compile_program(file : string; _export_all : bool; blk : block<(program : smart_ptr<Program>; issues : string) : string>) : string {
    return compile_program(file, _export_all, false, "") <| blk
}

def compile_program(file : string; _export_all : bool; _no_opt : bool; project : string = ""; blk : block<(program : smart_ptr<Program>; issues : string) : string>) : string {
    var inscope access <- make_file_access(project)
    var result : string
    var had_error = false
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            if (_export_all) {
                cop.export_all = true
            }
            if (_no_opt) {
                cop.no_optimizations = true
            }
            compile_file(file, access, unsafe(addr(mg)), cop) <| $(ok; program; issues) {
                if (!ok) {
                    result = "Compilation failed:\n{issues}"
                    had_error = true
                } else {
                    simulate(program) <| $(sok; ctx; serrors) {
                        if (!sok) {
                            result = "Simulation failed:\n{serrors}"
                            had_error = true
                            return
                        }
                        result = invoke(blk, program, string(issues))
                    }
                }
            }
        }
    }
    return make_tool_result(result, had_error)
}

def arg_needs_documenting(tt) : bool {
    return tt.baseType != Type.fakeContext && tt.baseType != Type.fakeLineInfo
}

def write_func_signature(var w : StringBuilderWriter; func) {
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

def write_handled_type(var w : StringBuilderWriter; value : Annotation const) {
    write(w, "  handled {value.name}")
    var ann = unsafe(reinterpret<BasicStructureAnnotation?> value)
    basic_struct_for_each_parent(*ann) <| $(parent) {
        write(w, " : {parent.name}")
    }
    if (!empty(value.cppName)) {
        write(w, "  // C++ {value.cppName}")
    }
    write(w, "\n")
    for_each_field(*ann) <| $(name, cppName, xtype, offset) {
        if (!empty(cppName) && cppName != name) {
            write(w, "    {name} : {describe(xtype)}  // C++ {cppName}\n")
        } else {
            write(w, "    {name} : {describe(xtype)}\n")
        }
    }
}

def write_function_list(var writer : StringBuilderWriter; program : smart_ptr<Program>) {
    let thisMod = get_this_module(program)
    var user_funcs : array<string>
    var generic_instances : array<string>
    var class_methods : array<string>
    for_each_function(thisMod, "") <| $(func) {
        if (func.flags.builtIn) {
            return
        }
        var line = ""
        if (func.flags._lambda) {
            return
        }
        if (func.fromGeneric != null) {
            line = build_string() <| $(var w) {
                write(w, "  def {func.fromGeneric.name}")
                write_func_signature(w, func)
            }
            generic_instances |> push(line)
        } elif (func.flags.generated) {
            return
        } elif (func.flags.isClassMethod) {
            line = build_string() <| $(var w) {
                write(w, "  def {func.name}")
                write_func_signature(w, func)
            }
            class_methods |> push(line)
        } else {
            line = build_string() <| $(var w) {
                write(w, "  def {func.name}")
                write_func_signature(w, func)
            }
            user_funcs |> push(line)
        }
    }
    if (!empty(user_funcs)) {
        write(writer, "Functions:\n")
        for (f in user_funcs) {
            write(writer, "{f}\n")
        }
    }
    if (!empty(class_methods)) {
        write(writer, "Class methods:\n")
        for (f in class_methods) {
            write(writer, "{f}\n")
        }
    }
    if (!empty(generic_instances)) {
        write(writer, "Generic instances:\n")
        for (f in generic_instances) {
            write(writer, "{f}\n")
        }
    }
}
