options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require daslib/ast_boost

let MAX_FIND_RESULTS = 50

def module_display_name(mname : string) : string {
    return mname == "$" ? "builtin" : mname
}

def write_location(var w : StringBuilderWriter; at) {
    if (int(at.line) > 0) {
        write(w, "  // line {int(at.line)}")
    }
}

def write_cpp_name(var w : StringBuilderWriter; cppName : string) {
    if (!empty(cppName)) {
        write(w, "  // C++ {cppName}")
    }
}

def get_func_cpp_name(func) : string {
    let fptr = get_ptr(func)
    if (fptr.flags.builtIn) {
        let bfn = fptr as BuiltInFunction
        if (bfn != null) {
            return string(bfn.cppName)
        }
    }
    return ""
}

def struct_has_matching_field(value; query : string) : bool {
    for (fld in value.fields) {
        if (find(string(fld.name), query) >= 0) {
            return true
        }
    }
    return false
}

def handled_has_matching_field(value : Annotation const; query : string) : bool {
    var found = false
    var ann = unsafe(reinterpret<BasicStructureAnnotation?> value)
    for_each_field(*ann) <| $(name, cppName, xtype, offset) {
        if (find(name, query) >= 0) {
            found = true
        }
    }
    return found
}

def write_struct_fields_highlighted(var w : StringBuilderWriter; value; query : string; field_search : bool) {
    for (fld in value.fields) {
        let fname = string(fld.name)
        if (field_search && find(fname, query) >= 0) {
            write(w, "  > {fld.name} : {describe(fld._type)}\n")
        } else {
            write(w, "    {fld.name} : {describe(fld._type)}\n")
        }
    }
}

def write_handled_fields_highlighted(var w : StringBuilderWriter; value : Annotation const; query : string; field_search : bool) {
    var ann = unsafe(reinterpret<BasicStructureAnnotation?> value)
    for_each_field(*ann) <| $(name, cppName, xtype, offset) {
        let prefix = field_search && find(name, query) >= 0 ? "  > " : "    "
        if (!empty(cppName) && cppName != name) {
            write(w, "{prefix}{name} : {describe(xtype)}  // C++ {cppName}\n")
        } else {
            write(w, "{prefix}{name} : {describe(xtype)}\n")
        }
    }
}

def search_module(var results : array<string>; var total_found : int&; mname : string; mod : Module?; query : string; kind : string) {
    let dname = module_display_name(mname)
    // functions
    if (total_found < MAX_FIND_RESULTS && (empty(kind) || kind == "function")) {
        for_each_function(mod, "") <| $(func) {
            if (total_found >= MAX_FIND_RESULTS) {
                return
            }
            if (func.flags.privateFunction || func.flags._lambda || func.flags.generated) {
                return
            }
            let fname = string(func.name)
            if (find(fname, query) < 0) {
                return
            }
            var entry = build_string() <| $(var w) {
                write(w, "  [{dname}] def {fname}")
                write_func_signature(w, func)
                write_cpp_name(w, get_func_cpp_name(func))
            }
            results |> push(entry)
            total_found++
        }
    }
    // generics
    if (total_found < MAX_FIND_RESULTS && (empty(kind) || kind == "generic")) {
        for_each_generic(mod) <| $(func) {
            if (total_found >= MAX_FIND_RESULTS) {
                return
            }
            if (func.flags.privateFunction || func.flags.generated) {
                return
            }
            let fname = string(func.name)
            if (fname |> starts_with("```") || find(fname, query) < 0) {
                return
            }
            var entry = build_string() <| $(var w) {
                write(w, "  [{dname}] def {fname}")
                write_func_signature(w, func)
                write_cpp_name(w, get_func_cpp_name(func))
            }
            results |> push(entry)
            total_found++
        }
    }
    // structs/classes (match by name or field name)
    if (total_found < MAX_FIND_RESULTS && (empty(kind) || kind == "struct" || kind == "field")) {
        for_each_structure(mod) <| $(value) {
            if (total_found >= MAX_FIND_RESULTS) {
                return
            }
            if (value.flags.isLambda || value.flags._generator || value.flags.generated || value.flags.privateStructure) {
                return
            }
            let name_match = find(string(value.name), query) >= 0
            let field_match = struct_has_matching_field(value, query)
            if (kind == "field" && !field_match) {
                return
            }
            if (kind != "field" && !name_match && !field_match) {
                return
            }
            var entry = build_string() <| $(var w) {
                if (value.flags.isClass) {
                    write(w, "  [{dname}] class {value.name}")
                } else {
                    write(w, "  [{dname}] struct {value.name}")
                }
                write_location(w, value.at)
                write(w, "\n")
                write_struct_fields_highlighted(w, value, query, field_match && !name_match)
            }
            results |> push(entry)
            total_found++
        }
    }
    // handled types via annotations (match by name or field name)
    if (total_found < MAX_FIND_RESULTS && (empty(kind) || kind == "handled" || kind == "struct" || kind == "field")) {
        module_for_each_annotation(mod) <| $(value) {
            if (total_found >= MAX_FIND_RESULTS) {
                return
            }
            if (!value.isBasicStructureAnnotation) {
                return
            }
            let name_match = find(string(value.name), query) >= 0
            let field_match = handled_has_matching_field(value, query)
            if (kind == "field" && !field_match) {
                return
            }
            if (kind != "field" && !name_match && !field_match) {
                return
            }
            var entry = build_string() <| $(var w) {
                write(w, "  [{dname}] handled {value.name}")
                write_cpp_name(w, string(value.cppName))
                write(w, "\n")
                write_handled_fields_highlighted(w, value, query, field_match && !name_match)
            }
            results |> push(entry)
            total_found++
        }
    }
    // enums
    if (total_found < MAX_FIND_RESULTS && (empty(kind) || kind == "enum")) {
        for_each_enumeration(mod) <| $(value) {
            if (total_found >= MAX_FIND_RESULTS) {
                return
            }
            if (value.isPrivate) {
                return
            }
            if (find(string(value.name), query) < 0) {
                return
            }
            var entry = build_string() <| $(var w) {
                write(w, "  [{dname}] enum {value.name}")
                write_location(w, value.at)
                write(w, "\n")
                for (en in value.list) {
                    write(w, "    {en.name}\n")
                }
            }
            results |> push(entry)
            total_found++
        }
    }
    // globals
    if (total_found < MAX_FIND_RESULTS && (empty(kind) || kind == "global")) {
        for_each_global(mod) <| $(value) {
            if (total_found >= MAX_FIND_RESULTS) {
                return
            }
            if (value.flags.private_variable) {
                return
            }
            if (find(string(value.name), query) < 0) {
                return
            }
            var entry = build_string() <| $(var w) {
                write(w, "  [{dname}] {value.name} : {describe(value._type)}")
                write_location(w, value.at)
            }
            results |> push(entry)
            total_found++
        }
    }
}

def do_find_symbol(query : string; kind : string = ""; file : string = "") : string {
    if (empty(query)) {
        return make_tool_result("'query' must not be empty", true)
    }
    if (!empty(file)) {
        // compile the file and search all its modules (including builtin $)
        return compile_and_simulate(file) <| $(program; issues) {
            var results : array<string>
            var total_found = 0
            program_for_each_module(program) <| $(mod) {
                let mname = string(mod.name)
                if (mname == "__main__" || empty(mname)) {
                    return
                }
                search_module(results, total_found, mname, mod, query, kind)
            }
            return format_find_results(results, total_found, query, kind)
        }
    } else {
        // search all registered C++ modules (including builtin $)
        var results : array<string>
        var total_found = 0
        program_for_each_registered_module() <| $(mod) {
            let mname = string(mod.name)
            if (mname == "__main__" || empty(mname)) {
                return
            }
            search_module(results, total_found, mname, mod, query, kind)
        }
        return make_tool_result(format_find_results(results, total_found, query, kind))
    }
}

def format_find_results(results : array<string>; total_found : int; query : string; kind : string) : string {
    if (empty(results)) {
        return "No symbols found matching '{query}'" + (empty(kind) ? "" : " (kind={kind})")
    }
    let truncated = total_found >= MAX_FIND_RESULTS ? "\n(truncated at {MAX_FIND_RESULTS} results)" : ""
    return build_string() <| $(var writer) {
        write(writer, "Found {length(results)} symbol(s) matching '{query}':\n")
        for (r in results) {
            write(writer, "{r}\n")
        }
        write(writer, truncated)
    }
}
