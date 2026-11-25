options gen2

require test_compile_dag_leaf
require test_compile_dag_base

[export]
def main() {
    assert(compile_dag_base() == 1)
}
