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

def make_tool_result(text : string; is_error : bool = false) : string {
    var result : ToolResult
    result.isError = is_error
    result.content |> emplace(ContentItem(_type = "text", text = text))
    return sprint_json(result, false)
}

def write_func_signature(var writer : StringBuilderWriter; func : FunctionPtr) {
    if (length(func.arguments) > 0) {
        write(writer, "(")
        for (i in range(length(func.arguments))) {
            if (i > 0) {
                write(writer, "; ")
            }
            write(writer, "{func.arguments[i].name} : {describe(func.arguments[i]._type)}")
        }
        write(writer, ")")
    }
    if (func.result != null) {
        write(writer, " : {describe(func.result)}")
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
                write(w, "  // line {int(func.at.line)}")
            }
            generic_instances |> push(line)
        } elif (func.flags.generated) {
            return
        } elif (func.flags.isClassMethod) {
            line = build_string() <| $(var w) {
                write(w, "  def {func.name}")
                write_func_signature(w, func)
                write(w, "  // line {int(func.at.line)}")
            }
            class_methods |> push(line)
        } else {
            line = build_string() <| $(var w) {
                write(w, "  def {func.name}")
                write_func_signature(w, func)
                write(w, "  // line {int(func.at.line)}")
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
