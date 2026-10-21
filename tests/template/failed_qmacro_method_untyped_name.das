options gen2
expect 50503, 30838

// qmacro_method reports its own error, not an exception, when its
// name argument has no type

require daslib/ast_boost
require daslib/templates_boost

struct Host {
    x : int
}

[export]
def main() {
    let st = find_unique_structure(compiling_program(), "Host")
    let fn = qmacro_method(no_such_name, st) $() {
        pass
    }
    print("{fn != null}\n")
}
