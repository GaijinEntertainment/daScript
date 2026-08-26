options gen2
options indenting = 4

module mc_generic_origin_mod

// Both put() overloads instance the same builtin `push` generic, so the second call site
// resolves through the locked generic-instance name __::builtin`push`<hash>. Recovering
// that call needs every candidate in the library to carry its origin generic, which is
// what a module restored from -module-cache used to lose.
def word(v : float) : uint {
    return unsafe(reinterpret<uint>(v))
}

def put(var words : array<uint>; v : float) {
    words |> push(word(v))
}

def put(var words : array<uint>; v : int) {
    words |> push(unsafe(reinterpret<uint>(v)))
}

def public go(a : int; b : float) : int {
    var words : array<uint>
    put(words, a)
    put(words, b)
    return length(words)
}
