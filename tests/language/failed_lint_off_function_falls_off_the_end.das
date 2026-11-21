//! under options lint = false a function that returns a value on
//! only one branch still does not compile
options gen2
options lint = false

expect 30310

def pick(n : int) : int {
    if (n > 5) {
        return 2
    }
}

[export]
def main {
    feint("{pick(3)}\n")
}
