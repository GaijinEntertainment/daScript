//! a function with a return type falls off its end with no value
//! when a break can leave its while (true) body
expect 30310

options gen2

def f(x0 : int) : int {
    var x = x0
    while (true) {
        if (x > 0) {
            break
        }
        return 1
    }
}

[export]
def main {
    print("{f(3)} {f(-1)}\n")
}
