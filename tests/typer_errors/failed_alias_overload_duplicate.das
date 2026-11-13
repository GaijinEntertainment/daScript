//! Two overloads whose parameter types become the same type once a
//! forward-declared typedef resolves must not compile (30702), called
//! or not.
options gen2
expect 30702

def dup_alias_pick(a : DupAliasInt) : int => 1

def dup_alias_pick(a : int) : int => 2

typedef DupAliasInt = int

[export]
def main {
}
