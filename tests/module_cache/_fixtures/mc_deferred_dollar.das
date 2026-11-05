options gen2

require daslib/rtti
require daslib/ast

[export]
def main {
    program_for_each_registered_module() $(mod) {
        if (mod.name == "$") {
            var n = 0
            module_for_each_function(mod) $(_fn) {
                n++
            }
            print("DOLLAR {n}\n")
        }
    }
}
