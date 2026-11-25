options gen2

module _optimized_body_helper public

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
