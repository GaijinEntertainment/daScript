//! Two generics whose argument types become the same type once a
//! named dimension folds must not compile (30702), called or not.
options gen2
expect 30702

let DUP_GENERIC_DIM = 3

def dup_generic_pick(_a : int[DUP_GENERIC_DIM]; _b : auto) : int => 1

def dup_generic_pick(_a : int[3]; _b : auto) : int => 2

[export]
def main {
}
