options gen2

require test_compile_dag_nesting
require test_compile_dag_leaf

[deadlock_nesting_probe, deadlock_leaf_probe]
def probed() {
    pass
}

[export]
def main() {
    probed()
}
