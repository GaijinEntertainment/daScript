options gen2
options rtti = true
options strict_smart_pointers = false
options no_aot

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require daslib/strings_boost

let FIXTURES = "{get_das_root()}/modules/dasOpenGL/tests"

//! `ok` is the compile verdict; `blk` runs on the program either way, `issues` carries the errors
def private compile_fixture(name : string; blk : block<(ok : bool; prog : smart_ptr<Program>; issues : string) : void>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.no_optimizations = true
            compile_file("{FIXTURES}/{name}", access, unsafe(addr(mg)), cop) $(ok, program, issues) {
                invoke(blk, ok, program, "{issues}")
            }
        }
    }
}

//! a late-bound extern call is renamed `__dasbind__<name>@@<lib>#<n>` by late binding; this reads `<name>` back
def private gl_name(name : das_string) : string {
    let full = string(name)
    if (!(full |> starts_with("__dasbind__"))) return full
    return slice(full, length("__dasbind__"), find(full, "@@"))
}

//! each call as `name(arg, arg, ...)` in statement order, nested blocks flattened; a non-call statement reads as "?"
def private calls_of(blk : ExprBlock?) : array<string> {
    var calls : array<string>
    for (st in blk.list) {
        if (st is ExprBlock) {
            calls |> push_from(calls_of(st as ExprBlock))
        } elif (st is ExprCall) {
            let call = st as ExprCall
            var inscope args <- [for (arg in call.arguments); describe(arg)]
            calls |> push("{gl_name(call.name)}({join(args, ", ")})")
        } else {
            calls |> push("?")
        }
    }
    return <- calls
}

def private args_and_calls_of(func : FunctionPtr) : string {
    let args <- [for (arg in func.arguments); string(arg.name)]
    let calls <- calls_of(unsafe(reinterpret<ExprBlock?>(func.body)))
    return "{join(args, " ")} -> {join(calls, "; ")}"
}

[test]
def test_vertex_buffer_emits_both_binders(t : T?) {
    compile_fixture("_vertex_buffer_fixture.das") $(ok, prog, issues) {
        if (!ok) {
            t |> failure("the fixture does not compile: {issues}")
            return
        }
        var binders : table<int; string>
        for_each_function(get_this_module(prog), "bind_vertex_buffer") $(func) {
            binders[length(func.arguments)] = args_and_calls_of(func)
        }
        t |> equal(length(binders), 2, "one pointer-offset binder and one named binder")
        t |> equal(binders?[3] ?? "", "ptrd self vindex -> " +
            "glEnableVertexAttribArray((vindex + 0x0)); " +
            "glVertexAttribPointer((vindex + 0x0), 3, 0x1406, false, 20, ptrd, 0); " +
            "glEnableVertexAttribArray((vindex + 0x1)); " +
            "glVertexAttribPointer((vindex + 0x1), 2, 0x1406, false, 20, ptrd, 12)",
            "xyz at 0 then uv at 12, GL_FLOAT (0x1406), stride 20")
        t |> equal(binders?[5] ?? "", "vao vbo self vindex binding -> " +
            "glVertexArrayVertexBuffer(vao, binding, vbo, 0, 20); " +
            "glEnableVertexArrayAttrib(vao, (vindex + 0x0)); " +
            "glVertexArrayAttribFormat(vao, (vindex + 0x0), 3, 0x1406, false, 0x0); " +
            "glVertexArrayAttribBinding(vao, (vindex + 0x0), binding); " +
            "glEnableVertexArrayAttrib(vao, (vindex + 0x1)); " +
            "glVertexArrayAttribFormat(vao, (vindex + 0x1), 2, 0x1406, false, 0xc); " +
            "glVertexArrayAttribBinding(vao, (vindex + 0x1), binding)",
            "the buffer rides the binding slot with stride 20; relative offsets 0 and 12")
        var instances : table<int; string>
        for_each_function(get_this_module(prog), "") $(func) {
            if (func.fromGeneric == null || find("{func.name}", "bind_vertex_buffer") < 0) return
            instances[length(func.arguments)] = join([for (arg in func.arguments); string(arg.name)], " ")
        }
        t |> equal(length(instances), 2, "each call shape instantiates its own overload")
        t |> equal(instances?[2] ?? "", "ptrd vindex")
        t |> equal(instances?[4] ?? "", "vao vbo vindex binding")
    }
}

[test]
def test_vertex_buffer_rejects_unsupported_fields(t : T?) {
    compile_fixture("_vertex_buffer_fixture_reject_int.das") $(ok, _prog, issues) {
        t |> success(!ok, "an int field is rejected")
        t |> success(find(issues, "field id is missing type information") >= 0, issues)
    }
    compile_fixture("_vertex_buffer_fixture_reject_double.das") $(ok, _prog, issues) {
        t |> success(!ok, "a double field is rejected")
        t |> success(find(issues, "field weight type") >= 0 && find(issues, "is not supported") >= 0, issues)
    }
    compile_fixture("_vertex_buffer_fixture_reject_twice.das") $(ok, _prog, issues) {
        t |> success(!ok, "a second [vertex_buffer] on one struct is rejected")
        t |> success(find(issues, "can't add bind_vertex_buffer") >= 0, issues)
    }
}
