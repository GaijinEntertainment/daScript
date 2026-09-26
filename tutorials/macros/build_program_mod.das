options gen2

module build_program_mod public

require daslib/ast_boost public
require daslib/templates_boost public

// These helpers build functions as AST nodes, for tutorial 21 to put into
// modules of a program it assembles itself. Nothing here runs at compile time:
// they are ordinary functions, called while the host script runs.

def public make_greet_function() : FunctionPtr {
    //! Builds `def greet(name : string) { print("hello, {name}!\n") }` one node at a time.
    var greeting = new ExprStringBuilder(at = LineInfo())
    greeting.elements |> emplace_new(new ExprConstString(at = LineInfo(), value := "hello, "))
    greeting.elements |> emplace_new(new ExprVar(at = LineInfo(), name := "name"))
    greeting.elements |> emplace_new(new ExprConstString(at = LineInfo(), value := "!\n"))
    var call = new ExprCall(at = LineInfo(), name := "print")
    call.arguments |> emplace(greeting)
    var body = new ExprBlock(at = LineInfo())
    body.list |> emplace(call)
    var fn = new Function(at = LineInfo(), atDecl = LineInfo(), name := "greet",
        result = new TypeDecl(at = LineInfo(), baseType = Type.tVoid))
    fn.arguments |> emplace_new(new Variable(at = LineInfo(), name := "name",
        _type = new TypeDecl(at = LineInfo(), baseType = Type.tString)))
    fn.body = body
    return fn
}

def public make_main_function() : FunctionPtr {
    //! Builds `main` from quoted code. It calls `greet` from the greeter module and `sqrt` from math.
    var fn = qmacro_function("main") $() {
        greet("world")
        print("sqrt(2) = {sqrt(2.0)}\n")
    }
    fn.flags.exports = true
    return fn
}

def public make_broken_function() : FunctionPtr {
    //! Builds a function that passes an int where `greet` takes a string, so inference rejects it.
    return qmacro_function("broken") $() {
        greet(42)
    }
}
