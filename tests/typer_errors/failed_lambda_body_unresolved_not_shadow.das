//! A lambda whose body never infers reports the body's unresolved
//! types, not a shadowing error on its argument: a lambda argument
//! may share the enclosing function's argument name.
options gen2
expect 31329, 31302, 31327, 31330

def run_it(t : int; b : lambda<(t : int) : void>) {
    invoke(b, t)
}

def outer(t : int) {
    t |> run_it() @(t : int) {
        let h = ([], "x")
        print("{t} {h}\n")
    }
}

[export]
def main() {
    outer(1)
}
