options gen2
require dastest/testing_boost public

require daslib/enum_trait

enum Foo {
    one
    two
    three
}

[test]
def test_enum_conversions(t : T?) {
    t |> run("enum to string") <| @(t : T?) {
        let s = "{Foo.two}"
        t |> equal("two", s)
        let q = string(Foo.two)
        t |> equal("two", q)
    }
    t |> run("string to enum") <| @(t : T?) {
        let q = to_enum(type<Foo>, "two")
        t |> equal(Foo.two, q)
        let r = to_enum(type<Foo>, "five", Foo.three)
        t |> equal(Foo.three, r)
    }
}


[string_to_enum]
enum Bar {
    one
    two
    three
}

[test]
def test_enum_macro_conversions(t : T?) {
    t |> run("enum macro") <| @(t : T?) {
        let b = Bar("three")                // this conversion can panic
        t |> equal(Bar.three, b)
        let s = "{b}"
        t |> equal("three", s)
        let q = string(Bar.two)
        t |> equal("two", q)
        let r = to_enum(type<Bar>, "one")   // conversion via standard path
        t |> equal(Bar.one, r)
        let bb = Bar("five", Bar.two)       // conversion with the default value
        t |> equal(Bar.two, bb)
    }
}

bitfield Baz {
    alpha
    beta
    gamma
    alpha_gamma = Baz.alpha | Baz.gamma
}

[test]
def test_bitfield_conversions(t : T?) {
    t |> run("bitfield operations") <| @(t : T?) {
        var b = Baz.alpha | Baz.gamma
        t |> equal(Baz.alpha_gamma, b)
        let has_alpha = (b & Baz.alpha) != bitfield(0)
        t |> equal(true, has_alpha)
        let has_beta = (b & Baz.beta) != bitfield(0)
        t |> equal(false, has_beta)
        b &= ~Baz.alpha
        let has_alpha_after = (b & Baz.alpha) != bitfield(0)
        t |> equal(false, has_alpha_after)
        let all3 = Baz.beta | Baz.alpha_gamma
        t |> equal(Baz.alpha | Baz.beta | Baz.gamma, all3)
    }
}
