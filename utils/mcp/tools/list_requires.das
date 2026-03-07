options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def module_kind(mod : Module?) : string {
    if (mod == null) {
        return "missing"
    }
    if (!empty(mod.fileName)) {
        return string(mod.fileName)
    }
    return "builtin"
}

def do_list_requires(file : string; project : string = "") : string {
    return compile_program(file, true, false, project) <| $(program; issues) {
        // direct requires from allRequireDecl
        var direct : array<string>
        var direct_modules : array<string>
        for_each_require_declaration(program) <| $(var mod : Module?; mod_name : string#; mod_file : string#; var is_public : bool; at : LineInfo&) {
            let pub_marker = is_public ? " public" : ""
            let source = "  // {module_kind(mod)}"
            direct |> push("  require {mod_name}{pub_marker}{source}")
            direct_modules |> push(string(mod_name))
        }
        // transitive: walk dependencies of the main module, skip "$" (always implicit)
        var transitive : array<string>
        let thisMod = get_this_module(program)
        var visited : table<string; bool>
        for (dm in direct_modules) {
            visited |> insert(dm, true)
        }
        visited |> insert("$", true)
        // collect deps from each direct dependency recursively
        var queue : array<Module?>
        module_for_each_dependency(thisMod) <| $(var dep : Module?; var is_pub : bool) {
            let dep_name = string(dep.name)
            if (!key_exists(visited, dep_name)) {
                visited |> insert(dep_name, true)
                queue |> push(dep)
            }
        }
        var idx = 0
        while (idx < length(queue)) {
            let mod = queue[idx]
            idx++
            let mod_name = string(mod.name)
            transitive |> push("  {mod_name}  // {module_kind(mod)}")
            module_for_each_dependency(mod) <| $(var dep : Module?; var is_pub : bool) {
                let dep_name = string(dep.name)
                if (!key_exists(visited, dep_name)) {
                    visited |> insert(dep_name, true)
                    queue |> push(dep)
                }
            }
        }
        sort(transitive)
        return build_string() <| $(var w) {
            if (!empty(direct)) {
                write(w, "Direct requires:\n")
                for (d in direct) {
                    write(w, "{d}\n")
                }
            }
            if (!empty(transitive)) {
                if (!empty(direct)) {
                    write(w, "\n")
                }
                write(w, "Transitive dependencies ({length(transitive)}):\n")
                for (t in transitive) {
                    write(w, "{t}\n")
                }
            }
            if (empty(direct) && empty(transitive)) {
                write(w, "No requires found.\n")
            }
        }
    }
}
