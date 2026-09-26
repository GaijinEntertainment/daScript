options gen2
options multiple_contexts

// Tutorial 21 — Building a program from AST nodes
//
// `compile` and `compile_file` (Language Tutorial 44) turn source text into a
// program. `make_program` skips the text: the host script builds each module
// out of AST nodes, and the compiler takes it from there — inference,
// optimization, simulation — exactly as it does for a parsed file.
//
//   make_program(cop)       one compile: its module group, builtin.das, its gc scope
//   make_module(name)       builds a module and compiles it right away, like a
//                           `require`d file; answers the Module? for later requires
//   make_main_module()      builds the program's own module; answers the program
//
// Two programs are built below:
//   1. greeter module + main module that requires it and math — then it runs
//   2. a main module with a type error — the errors come back on the program
//
// Run: daslang tutorials/macros/21_build_program.das

require daslib/debugger
require build_program_mod

def print_errors(program : ProgramPtr) {
    for (err in program.errors) {
        print("  error: {err.what}\n")
    }
}

// --- 1. A program that runs ---
// The greeter module is compiled as soon as its block ends, so the main module
// can require it. `math` is a C++ module: `get_module` finds it, and the same
// `add_module_require` puts it into the main module's reach.

def build_and_run() {
    print("=== greeter + main ===\n")
    using() $(var cop : CodeOfPolicies) {
        cop.threadlock_context = true   // invoke_in_context locks the program's context
        make_program(cop) $ {
            let greeter = make_module("greeter") $(var mod) {
                var greet = make_greet_function()
                mod |> add_function(greet)
            }
            var inscope program <- make_main_module() $(var mmain) {
                mmain |> add_module_require(greeter, false)
                mmain |> add_module_require(get_module("math"), false)
                var main_fn = make_main_function()
                mmain |> add_function(main_fn)
            }
            if (program.failed) {
                print_errors(program)
                return
            }
            simulate(program) $(sok; context; serrors) {
                if (!sok) {
                    print("simulate error: {serrors}\n")
                    return
                }
                unsafe(invoke_in_context(context, "main"))
            }
        }
    }
}

// --- 2. A program that fails to compile ---
// `broken` passes an int to `greet(name : string)`. The main module's
// inference rejects it, and the program comes back with `failed` set and the
// errors in `program.errors` — the same errors a parsed file would report.

def build_broken() {
    print("\n=== a type error ===\n")
    using() $(var cop : CodeOfPolicies) {
        make_program(cop) $ {
            let greeter = make_module("greeter") $(var mod) {
                var greet = make_greet_function()
                mod |> add_function(greet)
            }
            var inscope program <- make_main_module() $(var mmain) {
                mmain |> add_module_require(greeter, false)
                var broken = make_broken_function()
                mmain |> add_function(broken)
            }
            if (program.failed) {
                print("compile failed, as expected\n")
                print_errors(program)
            }
        }
    }
}

[export]
def main() {
    build_and_run()
    build_broken()
}

// Expected output:
//   === greeter + main ===
//   hello, world!
//   sqrt(2) = 1.4142135
//
//   === a type error ===
//   compile failed, as expected
//     error: no matching functions or generics: greet(int const)
