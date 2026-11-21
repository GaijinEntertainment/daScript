//! under options lint = false a block that returns a value on only
//! one branch still does not compile
options gen2
options lint = false

expect 30304

def call(n : int; b : block<(n : int) : int>) : int {
    return invoke(b, n)
}

[export]
def main {
    let r = call(3) $(n) {
        if (n > 5) {
            return 2
        }
    }
    feint("{r}\n")
}
