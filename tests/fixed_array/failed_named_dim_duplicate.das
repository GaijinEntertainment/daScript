//! Two overloads whose parameter types become the same type once a
//! named dimension folds must not compile (30702), called or not.
options gen2
expect 30702

let DUP_DIM = 3

def dup_pick(a : int[DUP_DIM]) : int => 1

def dup_pick(a : int[3]) : int => 2

[export]
def main {
}
