options gen2

require test_compile_dag_fork

[export]
def main() {
    assert(fork_probe_value() == 1)
}
