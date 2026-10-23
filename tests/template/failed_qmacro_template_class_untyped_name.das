options gen2
expect 50503, 30838

// qmacro_template_class reports its own error, not an exception, when
// its name argument has no type

require daslib/templates_boost
require daslib/delegate

[export]
def main() {
    let q = qmacro_template_class(no_such_name, type<DelegateVoid>)
    print("{q}\n")
}
