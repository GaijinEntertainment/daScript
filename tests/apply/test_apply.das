require daslib/apply
require dastest/testing_boost public

variant FooBar
    foo : int
    bar : string

[test]
def test_apply_variant ( t:T? )
    t |> run("contents") <| @ ( t : T? )
        apply([[FooBar foo=1]]) <| $ ( name, field )
            if name!="foo"
                t->fatal("unknown field")
            static_if typeinfo(stripped_typename field) == "int"
                t |> equal( field, 1)
            else
                t->fatal("we got unsupported field here")
        apply([[FooBar bar="hello"]]) <| $ ( name, field )
            if name!="bar"
                t->fatal("unknown field")
            static_if typeinfo(stripped_typename field)=="string"
                t |> equal( field, "hello")
            else
                t->fatal("we got unsupported field here")

struct Foo
    a : int
    b : string

[test]
def test_apply_struct ( t:T? )
    t |> run("contents") <| @ ( t : T? )
        var foo = [[Foo a=1, b="hello"]]
        apply(foo) <| $ ( name, field )
            if name!="a" && name!="b"
                t->fatal("unknown field")
            static_if typeinfo(stripped_typename field)=="int"
                t |> equal( field, 1)
            static_elif typeinfo(stripped_typename field)=="string"
                t |> equal( field, "hello")
    t |> run("assign") <| @ ( t : T? )
        var foo = [[Foo a=1, b="hello"]]
        apply(foo) <| $ ( name, field )
            static_if typeinfo(stripped_typename field)=="int"
                field = 13
            static_elif typeinfo(stripped_typename field)=="string"
                field = "13"
        apply(foo) <| $ ( name, field )
            t |> equal( "{field}", "13")

[test]
def test_apply_tuple ( t:T? )
    t |> run("contents") <| @ ( t : T? )
        var foo = [[auto 1, "hello"]]
        apply(foo) <| $ ( name, field )
            if name!="_0" && name!="_1"
                t->fatal("unknown field")
            static_if typeinfo(stripped_typename field)=="int"
                t |> equal( field, 1)
            static_elif typeinfo(stripped_typename field)=="string"
                t |> equal( field, "hello")
    t |> run("assign") <| @ ( t : T? )
        var foo = [[auto 1, "hello"]]
        apply(foo) <| $ ( name, field )
            static_if typeinfo(stripped_typename field)=="int"
                field = 13
            static_elif typeinfo(stripped_typename field)=="string"
                field = "13"
        apply(foo) <| $ ( name, field )
            t |> equal( "{field}", "13")

[test]
def test_apply_named_tuple ( t:T? )
    var foo : tuple<a:int;b:string> = [[auto 1, "hello"]]
    apply(foo) <| $ ( name, field )
        if name!="a" && name!="b"
            t->fatal("unknown field")
