options gen2

//! a bare script: it runs, but no [test] names what it proves

[export]
def main() {
    let x = 1 + 1
    verify(x == 2)
}
