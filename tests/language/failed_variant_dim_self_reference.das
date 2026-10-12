// A variant whose field is dimensioned by a named construction of the variant itself. The
// alias resolves to a copy of the variant's type, and that copy carries the unresolved
// dimension - the construction - so every infer pass nested one more copy of it.
options gen2
expect 30109, 30807

variant V {
    f : int[V(a = 7)]
}

[export]
def main {
    pass
}
