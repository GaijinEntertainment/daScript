options gen2
options rtti

require dastest/testing_boost public
require strings
require daslib/fio
require _tools_common

require tools/goto_definition public
require tools/type_of public
require tools/find_references public
require tools/common public

[test(description = "goto_definition resolves a function call to the line of its definition.", timeout = 1200)]
def test_goto_definition_function(t : T?) {
    t |> run("resolves function call to definition") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "33", "13"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: function") >= 0, "should be function kind")
        t |> success(find(text, "Symbol: distance") >= 0, "should resolve to distance")
        t |> success(find(text, "Line: 14") >= 0, "should point to line 14")
    }
}

[test(description = "goto_definition resolves a variable reference to its declaration line.", timeout = 1200)]
def test_goto_definition_variable(t : T?) {
    t |> run("resolves variable reference to declaration") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "33", "22"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: variable") >= 0, "should be variable kind")
        t |> success(find(text, "Symbol: p1") >= 0, "should resolve to p1")
        t |> success(find(text, "Line: 31") >= 0, "should point to line 31")
    }
}

[test(description = "goto_definition resolves a field access to the struct field declaration.", timeout = 1200)]
def test_goto_definition_field(t : T?) {
    t |> run("resolves field access to struct field") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "15", "16"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: field") >= 0, "should be field kind")
        t |> success(find(text, "Point.x") >= 0, "should resolve to Point.x")
        t |> success(find(text, "Line: 10") >= 0, "should point to line 10")
    }
}

[test(description = "goto_definition resolves a builtin call and reports it as a builtin.", timeout = 1200)]
def test_goto_definition_builtin(t : T?) {
    t |> run("resolves builtin function") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "34", "5"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: builtin") >= 0, "should be builtin kind")
        t |> success(find(text, "print") >= 0, "should resolve to print")
    }
}

[test(description = "goto_definition lands on the class, parent, loop variable, type name and own name it points at, and source_line reads lines 1-based without carriage returns.", timeout = 1200)]
def test_goto_definition_declarations(t : T?) {
    t |> run("a class name is the class, never the finalizer the compiler puts on its line") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "46", "8"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Symbol: Square") >= 0 && find(text, "Kind: struct") >= 0, "the class: {text}")
        t |> success(find(text, "__finalize") < 0, "not the synthesized finalizer: {text}")
    }
    t |> run("the parent named on a derived class line is the parent") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "46", "16"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Symbol: Shape") >= 0 && find(text, "Line: 42") >= 0, "the parent's declaration: {text}")
    }
    t |> run("a for loop variable's use navigates to its header") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "69", "17"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Symbol: i") >= 0 && find(text, "Line: 68") >= 0, "the header: {text}")
    }
    t |> run("a type name in an expression, a parameter and a constructor is the type") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "66", "21"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Symbol: Square") >= 0 && find(text, "Line: 46") >= 0 && find(text, "Column: 6") >= 0, "new Square() names the class: {text}")
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "14", "24"), text, is_error)
        t |> success(find(text, "Symbol: Point") >= 0 && find(text, "Line: 9") >= 0, "a parameter's type: {text}")
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "62", "14"), text, is_error)
        t |> success(find(text, "Symbol: Value") >= 0 && find(text, "Kind: typedef") >= 0 && find(text, "Line: 31") >= 0, "a variant constructor's type lands on the typedef: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "66", "21", "file", ""), text, is_error)
        t |> success(find(text, "structure 'Square'") >= 0 && find(text, ":66:20") >= 0, "references from the type name in new: {text}")
        parse_result(do_type_of(fixture_path("_fixture_refs.das"), "66", "21"), text, is_error)
        t |> success(find(text, "TypeDecl(Square) : Square") >= 0, "type_of on a type name: {text}")
    }
    t |> run("a function's own name is its definition") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "14", "6"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Symbol: add_points") >= 0 && find(text, "Kind: function") >= 0 && find(text, "Line: 14") >= 0, "the definition: {text}")
    }
    t |> run("a global's own name, a for variable's header and a class line report the cursor's column") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "36", "5"), text, is_error)
        t |> success(find(text, "Symbol: GLOBAL_SCALE") >= 0 && find(text, "Kind: variable") >= 0 && find(text, "Line: 36") >= 0, "the global: {text}")
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "68", "10"), text, is_error)
        t |> success(find(text, "Symbol: i") >= 0 && find(text, "Kind: variable") >= 0 && find(text, "Line: 68") >= 0, "the for variable on its header: {text}")
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "46", "8"), text, is_error)
        t |> success(find(text, "Column: 6") >= 0, "the class name's column on its own line: {text}")
        parse_result(do_type_of(fixture_path("_fixture_refs.das"), "68", "10"), text, is_error)
        t |> success(find(text, "Variable(i) : int") >= 0, "type_of on a for variable's header: {text}")
        t |> equal(source_line(fixture_path("_fixture_refs.das"), 68), "    for (i in range(2)) \{", "the subtools' line reader is 1-based")
        t |> equal(source_line(fixture_path("_fixture_refs.das"), 100000), "", "past the end of the file the line is empty")
    }
    t |> run("a CRLF file's lines come back without the carriage return") <| @(t : T?) {
        var terr : string
        let dir = create_temp_directory("mcp_source_line_crlf", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let file = path_join(dir, "crlf.das")
        fwrite(file, "let a = 1\r\nlet b = 2\r\nlet c = 3\r\n")
        t |> equal(source_line(file, 2), "let b = 2", "the line, not the line plus \\r")
        t |> equal(length(source_line(file, 3)), length("let c = 3"), "the last line too")
        t |> equal(source_line(file, 5), "", "past the end of the file the line is empty")
        rmdir_rec(dir)
    }
}

[test(description = "goto_definition lands a method call on the override the receiver runs and an enum type name on the enum, in agreement with find_references and type_of.", timeout = 1200)]
def test_goto_definition_members_and_types(t : T?) {
    t |> run("a method call lands on the method the receiver's class runs, a field's own name is the field") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "78", "15"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Symbol: Square`area") >= 0 && find(text, "Kind: function") >= 0 && find(text, "Line: 47") >= 0, "s->area() is the override, not the class: {text}")
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "67", "20"), text, is_error)
        t |> success(find(text, "Symbol: Square`area") >= 0 && find(text, "Line: 47") >= 0, "shape->area() too: {text}")
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "10", "5"), text, is_error)
        t |> success(find(text, "Symbol: x") >= 0 && find(text, "Kind: field") >= 0 && find(text, "Line: 10") >= 0, "the field's declaration: {text}")
        parse_result(do_type_of(fixture_path("_fixture_refs.das"), "78", "15"), text, is_error)
        t |> success(text |> starts_with("ExprField(area)") && find(text, "ExprRef2Value") < 0, "type_of on a method call: the method field first, never the compiler's ref-to-value conversion: {text}")
    }
    t |> run("an enum type name in a parameter is the enum") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_refs.das"), "73", "15"), text, is_error)
        t |> success(find(text, "Symbol: Color") >= 0 && find(text, "Kind: enum") >= 0 && find(text, "Line: 3") >= 0, "the enum: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "73", "15", "file", "true"), text, is_error)
        t |> success(find(text, "enumeration 'Color'") >= 0 && find(text, ":73:") >= 0 && find(text, ":60:") >= 0, "references from the parameter's type: the parameter and the constant's use (unoptimized, the folded let keeps its site): {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "3", "6", "file", "true"), text, is_error)
        t |> success(find(text, "enumeration 'Color'") >= 0 && find(text, ":73:") >= 0 && find(text, ":60:") >= 0 && find(text, ":3:") < 0, "from the declaration's name: the same sites, never the declaration: {text}")
        parse_result(do_type_of(fixture_path("_fixture_refs.das"), "73", "15"), text, is_error)
        t |> success(find(text, "TypeDecl(Color) : Color") >= 0, "type_of on the enum type name: {text}")
    }
}

[test(description = "goto_definition says so when the cursor is on no expression.", timeout = 1200)]
def test_goto_definition_no_expression(t : T?) {
    t |> run("reports no expression at empty position") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "1", "1"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No expression found") >= 0, "should report no expression")
    }
}

[test(description = "goto_definition includes a source snippet of the definition it found.", timeout = 1200)]
def test_goto_definition_snippet(t : T?) {
    t |> run("includes source snippet") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "33", "13"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def distance") >= 0, "snippet should contain function definition")
    }
}

[test(description = "type_of on a function call reports the call and its result type.", timeout = 1200)]
def test_type_of_call(t : T?) {
    t |> run("function call returns result type") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "33", "13"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprCall(distance)") >= 0, "should show ExprCall")
        t |> success(find(text, "float") >= 0, "should return float type")
    }
}

[test(description = "type_of on a variable reference reports the variable and its struct type.", timeout = 1200)]
def test_type_of_variable(t : T?) {
    t |> run("variable reference shows struct type with location") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "33", "22"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprVar(p1)") >= 0, "should show ExprVar")
        t |> success(find(text, "Point") >= 0, "should show Point type")
    }
}

[test(description = "type_of on a field access reports the field and its type.", timeout = 1200)]
def test_type_of_field(t : T?) {
    t |> run("field access shows field type") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "15", "16"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprField(x)") >= 0, "should show ExprField")
        t |> success(find(text, "float") >= 0, "should return float type")
    }
}

[test(description = "type_of says so when the cursor is on no expression.", timeout = 1200)]
def test_type_of_no_expression(t : T?) {
    t |> run("reports no expression at empty position") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "1", "1"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No expression found") >= 0, "should report no expression")
    }
}

[test(description = "find_references on a function lists both its call and its address references.", timeout = 1200)]
def test_find_references_function(t : T?) {
    t |> run("finds function call and addr references") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "57", "14", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func") >= 0, "should identify as function")
        t |> success(find(text, "add_points") >= 0, "should name add_points")
        t |> success(find(text, "[call]") >= 0, "should find call reference")
        t |> success(find(text, "[addr]") >= 0, "should find addr reference (@@add_points)")
    }
}

[test(description = "find_references on a variable lists every use of that variable.", timeout = 1200)]
def test_find_references_variable(t : T?) {
    t |> run("finds variable references") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "57", "25", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "variable") >= 0, "should identify as variable")
        t |> success(find(text, "p1") >= 0, "should name p1")
        t |> success(find(text, "Found 2 reference") >= 0, "should find 2 references")
    }
}

[test(description = "find_references on a field lists plain and safe-navigation accesses.", timeout = 1200)]
def test_find_references_field(t : T?) {
    t |> run("finds field references including safe field") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "15", "24", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Point.x") >= 0, "should identify as Point.x field")
        t |> success(find(text, "[field]") >= 0, "should find field references")
        t |> success(find(text, "[safe field]") >= 0, "should find safe field reference (?.x)")
        t |> success(find(text, "Found 6 reference") >= 0, "should find 6 references")
    }
}

[test(description = "find_references resolves the function behind an address-of expression.", timeout = 1200)]
def test_find_references_addr(t : T?) {
    t |> run("finds function from @@addr expression") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "59", "30", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func") >= 0, "should identify as function")
        t |> success(find(text, "add_points") >= 0, "should name add_points")
        t |> success(find(text, "[call]") >= 0, "should find call reference")
        t |> success(find(text, "[addr]") >= 0, "should find addr reference")
    }
}

[test(description = "find_references resolves an invoke to the function pointer variable it calls.", timeout = 1200)]
def test_find_references_invoke(t : T?) {
    t |> run("invoke resolves to function pointer variable") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "23", "12", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "variable") >= 0, "should identify as variable")
        t |> success(find(text, "'f'") >= 0, "should name f")
    }
}

[test(description = "find_references started from a function or struct definition finds its uses.", timeout = 1200)]
def test_find_references_from_declaration(t : T?) {
    t |> run("finds references from function definition") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "14", "5", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func") >= 0, "should identify as function")
        t |> success(find(text, "add_points") >= 0, "should name add_points")
        t |> success(find(text, "[call]") >= 0, "should find call reference")
    }
    t |> run("finds references from struct definition") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "9", "1", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "structure") >= 0, "should identify as structure")
        t |> success(find(text, "Point") >= 0, "should name Point")
        t |> success(find(text, "[type ref]") >= 0, "should find type references")
    }
}

[test(description = "find_references on a bitfield alias lists the alias references and never the declaration line.", timeout = 1200)]
def test_find_references_alias(t : T?) {
    t |> run("finds alias references from bitfield declaration") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "26", "1", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "alias") >= 0, "should identify as alias")
        t |> success(find(text, "Flags") >= 0, "should name Flags")
        t |> success(find(text, "[alias ref]") >= 0, "should find alias references")
        t |> success(find(text, ":26:") < 0, "the declaration's own line is its binding, not a reference: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "26", "10", "file", ""), text, is_error)
        t |> success(find(text, ":38:") >= 0 && find(text, ":26:") < 0, "from the name on the declaration: the parameter's type, not the declaration: {text}")
    }
    t |> run("finds alias references from variant usage") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "62", "13", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "alias") >= 0, "should identify as alias")
        t |> success(find(text, "Value") >= 0, "should name Value")
    }
}

[test(description = "find_references started from a global declaration finds its uses.", timeout = 1200)]
def test_find_references_global_variable(t : T?) {
    t |> run("finds global variable references from declaration") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "36", "5", "file", "true"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "variable") >= 0, "should identify as variable")
        t |> success(find(text, "GLOBAL_SCALE") >= 0, "should name GLOBAL_SCALE")
        t |> success(find(text, "Found 1 reference") >= 0, "should find 1 reference (usage in main)")
    }
}

[test(description = "find_references on classes, methods, fields, parents and finalizers reports their own sites and never the compiler-synthesized ones.", timeout = 1200)]
def test_find_references_class(t : T?) {
    t |> run("a class name resolves the class, not the finalizer the compiler puts on its line") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "46", "8", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "structure 'Square'") >= 0, "should identify the class: {text}")
        t |> success(find(text, "__finalize") < 0, "never the synthesized finalizer: {text}")
        t |> success(find(text, "Found 2 reference") >= 0, "the new site and the parameter type, no implicit self: {text}")
        t |> success(find(text, ":66:20 in main [type ref]") >= 0 && find(text, ":77:") >= 0, "the new site and the parameter's type: {text}")
    }
    t |> run("a method's sites reference its function, through -> and the implicit self; an override is its own method") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "47", "18", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func 'Square`area'") >= 0, "the override's own function: {text}")
        t |> success(find(text, ":67:") >= 0 && find(text, ":78:") >= 0 && find(text, "[method call]") >= 0, "shape->area() and s->area() call the override: {text}")
        t |> success(find(text, ":47:") < 0, "never the declaration's own line: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "43", "9", "file", ""), text, is_error)
        t |> success(find(text, "func 'Shape`area'") >= 0 && find(text, "Found 0 reference") >= 0, "the base method has no site of its own - every receiver is a Square: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "78", "15", "file", ""), text, is_error)
        t |> success(find(text, "func 'Square`area'") >= 0 && find(text, ":67:") >= 0, "from a call site the same set: {text}")
    }
    t |> run("a field's declaration name references its accesses") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "10", "5", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "field 'Point.x'") >= 0, "the field: {text}")
        t |> success(find(text, ":15:") >= 0 && find(text, ":19:") >= 0 && find(text, ":64:") >= 0, "a.x, p?.x and p3.x: {text}")
        t |> success(find(text, ":10:") < 0, "never the declaration's own line: {text}")
        t |> success(find(text, ":86:") < 0, "Other.x is a different field with the same name: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "86", "14", "file", ""), text, is_error)
        t |> success(find(text, "field 'Other.x'") >= 0 && find(text, ":86:") >= 0 && find(text, ":15:") < 0, "from an access, the other field's own sites: {text}")
    }
    t |> run("a parent named on a derived class line references the parent") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "42", "8", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "structure 'Shape'") >= 0, "should identify the base class: {text}")
        t |> success(find(text, "Found 1 reference") >= 0, "the derived class line only, no implicit self of its method: {text}")
        t |> success(find(text, ":46:15 [parent]") >= 0, "the parent mention on the derived class line: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "46", "16", "file", ""), text, is_error)
        t |> success(find(text, "structure 'Shape'") >= 0, "the cursor on the parent name resolves the parent: {text}")
    }
    t |> run("a user-written finalize is itself, with no synthesized caller") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "48", "11", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func 'finalize'") >= 0, "should identify the user finalizer: {text}")
        t |> success(find(text, "Found 0 reference") >= 0, "the synthesized finalizer chain is not a caller: {text}")
    }
}

[test(description = "find_references pairs a for loop variable in its header with its body uses.", timeout = 1200)]
def test_find_references_for_variable(t : T?) {
    t |> run("a for loop variable in its header pairs with its body use") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "68", "10", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "variable 'i'") >= 0, "should identify the loop variable: {text}")
        t |> success(find(text, ":69:16 in main [variable]") >= 0, "the body use: {text}")
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "69", "17", "file", ""), text, is_error)
        t |> success(find(text, "variable 'i'") >= 0 && find(text, ":69:16") >= 0, "from the use, the same variable: {text}")
    }
}

[test(description = "find_references says so and echoes the 1-based position when the cursor is on nothing.", timeout = 1200)]
def test_find_references_no_expression(t : T?) {
    t |> run("reports no expression at empty position") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "2", "1", "file", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No expression or declaration found") >= 0, "should report nothing found")
        t |> success(find(text, ":2:1") >= 0, "the message echoes the caller's 1-based position: {text}")
    }
}

[test(description = "goto_definition appends the C++ location of a builtin only when with_cpp_source is set.", timeout = 1200)]
def test_goto_definition_with_cpp_source(t : T?) {
    t |> run("redirect off (default) — no C++: line") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "34", "5", "", "", "", false), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "Kind: builtin") >= 0, "should resolve to a builtin -- {cpp_diag(text)}")
    }
    t |> run("redirect on — appends C++: line for builtin") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "34", "5", "", "", "", true, "src/builtin"), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "Kind: builtin") >= 0, "should still resolve to a builtin -- {cpp_diag(text)}")
        t |> success(find(text, "C++:") >= 0, "C++ line should appear when with_cpp_source=true -- {cpp_diag(text)}")
    }
}
