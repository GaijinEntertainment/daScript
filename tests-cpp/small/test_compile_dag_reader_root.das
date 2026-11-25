options gen2

require test_compile_dag_reader
require test_compile_dag_sibling

[export]
def main() {
    assert(compile_dag_reader_probe() + compile_dag_sibling_probe() == 3)
}
