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
