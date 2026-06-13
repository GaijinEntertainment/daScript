options gen2

// Tutorial 19 — Custom module options (add_module_option)
//
// Most `options` are built into the compiler (gen2, persistent_heap, rtti,
// ...). A module can also define its OWN option so that files requiring it
// may set it. The mechanism is add_module_option:
//
//   - The module registers an option name + type from a macro_function
//     (see add_module_option_mod.das, Section 1). After that, the parser
//     accepts `options <name> = ...` in any file that requires the module.
//   - A pass macro in the module reads the flag with find_arg and changes
//     behavior accordingly (Section 2 of the module).
//
// Here we require add_module_option_mod and switch on its `trace_compile`
// flag. Its lint pass then prints a compile-time note for each module it
// sees. Flip the option to false (or delete the line) and the notes vanish
// — the program still compiles, because the option name stays registered.
//
// require:  tutorials/macros/add_module_option_mod.das
// Run: daslang.exe tutorials/macros/19_add_module_option.das

require add_module_option_mod
options trace_compile = true

def greet(name : string) {
    print("Hello, {name}!\n")
}

def add(a, b : int) : int => a + b

[export]
def main() {
    greet("world")
    print("2 + 3 = {add(2, 3)}\n")
}

// Expected output (the [trace_compile] line is emitted at COMPILE time):
// [trace_compile] module '<main>' — 3 function(s)
// Hello, world!
// 2 + 3 = 5
