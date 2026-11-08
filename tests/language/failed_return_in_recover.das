//! a return inside a recover block does not compile
options gen2
expect 30299

def f(n : int) : int {
    try {
        panic("x")
    } recover {
        return -1
    }
    return n
}

[export]
def main {
    print("{f(1)}\n")
}
