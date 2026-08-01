options gen2
require dastest/testing_boost public

// Regression pin for the AST-serializer dangling-Function crash: a ctor referenced ONLY
// from a struct field initializer (`w : WithInit = WithInit()`) was never marked used -
// MarkSymbolUse records function refs solely under a function/global context - so
// removeUnusedSymbols freed it while the surviving structure's field-init expression kept
// the raw pointer, and dastest's --ser sweep read a dangling Function* in getMangledName
// (nondeterministic SIGSEGV, all three extended_checks CI lanes). The structs below stay
// deliberately UNUSED by any function so the ctors' only anchor is the field init; the CI
// serialization sweep over tests/ is what exercises the pin.

struct PlainPod {
    a : int
    b : float
}

struct WithInit {
    x : int = 7
}

struct Nested {
    p : PlainPod
    w : WithInit = WithInit()
}

[safe_when_uninitialized]
struct OptedOut {
    x : int = 7
}

variant PodVariant {
    i : int
    f : float
}

variant InitVariant {
    i : int
    w : WithInit
}

[test]
def test_field_init_ctor_survives_marking(t : T?) {
    t |> run("the structs above exist and stay otherwise unused") @(t : T?) {
        t |> success(true, "the pin is the file itself under the --ser sweep")
    }
}
