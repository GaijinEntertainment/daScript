//! a goto from inside a block literal to a label outside it does not
//! compile
options gen2
expect 30819

[export]
def main {
    invoke($() {
        goto label 50
    })
    label 50:
    pass
}
