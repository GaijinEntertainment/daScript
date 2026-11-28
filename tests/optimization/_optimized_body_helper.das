options gen2

module _optimized_body_helper public

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require dastest/testing_boost
require strings
require daslib/strings_boost

def public body_of(t : T?; nm : string) : string {
    //! the optimized body of the compiling module's function `nm` as describe() prints it, without the
    //! statements the coverage lane injects - their arguments collide with the substrings tests assert
    var body = ""
    compiling_module() |> for_each_function("{nm}") $(func) {
        if (empty(body)) {
            body = "{describe(func.body)}"
        }
    }
    t |> success(!empty(body), "target function {nm} found")
    let clean <- [for (ln in split(body, "\n")); ln; where find(ln, "coverage(") < 0]
    return join(clean, "\n")
}

def public body_of_module(t : T?; mod, nm : string) : string {
    //! the optimized body of function `nm` of required module `mod`
    var body = ""
    find_module(this_program(), mod) |> for_each_function(nm) $(func) {
        if (empty(body)) {
            body = "{describe(func.body)}"
        }
    }
    t |> success(!empty(body), "function {nm} of module {mod} found")
    return body
}

def public instance_body_of(t : T?; nm : string) : string {
    //! the body of the generic instance whose name holds `nm`
    var body = ""
    compiling_module() |> for_each_function("") $(func) {
        if (empty(body) && find("{func.name}", nm) >= 0) {
            body = "{describe(func.body)}"
        }
    }
    t |> success(!empty(body), "instance not found")
    let clean <- [for (ln in split(body, "\n")); ln; where find(ln, "coverage(") < 0]
    return join(clean, "\n")
}
