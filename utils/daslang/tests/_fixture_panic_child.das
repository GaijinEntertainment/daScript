options gen2

//! a child for test_process.das: panics out of main, so the parent
//! reads the exit code an uncaught runtime exception carries out
[export]
def main() : int {
    panic("a fixture panic")
    return 0
}
