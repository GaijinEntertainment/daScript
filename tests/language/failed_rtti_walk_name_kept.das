//! the FuncInfo a module_for_each_function walk hands its block is
//! temporary, so keeping its name past the walk without a clone does
//! not compile
expect 30341

options gen2
require daslib/rtti

[export]
def main {
    var kept : array<string>
    module_for_each_function(get_module("$")) $(info) {
        kept |> push(info.name)
    }
    print("{length(kept)}\n")
}
