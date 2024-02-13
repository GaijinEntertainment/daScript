require dastest/testing_boost

require daslib/dynamic_cast_rtti

class Foo
    def hello
        print("hello from Foo!\n")

class Bar : Foo
    def override hello
        print("hello from Bar!\n")


[test]
def test_is_instance_of ( t:T? )
    var f = new Foo()
    f->hello()
    var b = new Bar()
    b->hello()
    t |> success ( is_instance_of(f,type<Foo>) )
    t |> success ( is_instance_of(b,type<Foo>) )
    t |> success ( is_instance_of(b,type<Bar>) )
    t |> success ( !is_instance_of(f,type<Bar>) )

[test]
def test_dynamic_type_cast ( t:T? )
    var f = new Foo()
    f->hello()
    var b = new Bar()
    b->hello()
    var f2 = b |> dynamic_type_cast(type<Foo>)
    t |> equal ( intptr(b), intptr(f2) )
    var b2 = f |> dynamic_type_cast(type<Bar>)
    t |> equal ( null, b2 )


