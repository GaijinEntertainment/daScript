// A tuple whose element is dimensioned by a lambda returning the tuple. Resolving the lambda's
// result alias copies the tuple's type, unresolved dimension and lambda included.
options gen2
expect 30109, 30804, 30807

tuple T {
    a : int[invoke(@() : T => default<T>)]
}

[export]
def main {
    pass
}
