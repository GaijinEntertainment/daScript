require dastest/testing_boost
require daslib/json_boost


struct Tree
    name: string
    kind: int
    children: array<Tree?>


[test(name="json base types")]
def base_types(t: T?)
    equal(t, JV(1) as _number, JV(1.0) as _number)
    equal(t, JV(1u) as _number, JV(1.0) as _number)


[test(name="json arrays")]
def arrays(t: T?)
    let arr1 = JV(1., 2.)
    let arr2 = JV([{int[] 1; 2}])

    let raw1 & = arr1 as _array
    let raw2 & = arr2 as _array
    equal(t, length(raw1), length(raw2))
    for i, j in raw1, raw2
        equal(t, i as _number, j as _number)


[test(name="json tables")]
def tables(t: T?)
    var tab1 = JV({{ "foo" => [{auto[] "a"; "b"}] }})
    success(t, tab1 is _object)
    equal(t, length(tab1 as _object), 1)
    success(t, (tab1 as _object) |> key_exists("foo"), "`foo` should be in table")
    success(t, (tab1 as _object)["foo"] is _array, "`foo` should be _array value")
    equal(t, length((tab1 as _object)["foo"] as _array), 2)


struct Foo
    b: bool
    ia: array<int> <- [{auto 1; 2; 3; 4}]

[test(name="json structs")]
def structs(t: T?)
    let data <- [[Foo() b=true]]
    let foo = JV(data)
    success(t, foo is _object)
    equal(t, length(foo as _object), 2)
    let fooJv & = foo as _object
    success(t, fooJv |> key_exists("b"))
    success(t, fooJv |> key_exists("ia"))
    fooJv |> find("b") <| $(bVal)
        equal(t, *bVal as _bool, true)
    fooJv |> find("ia") <| $(bVal)
        success(t, *bVal is _array)
        equal(t, length(*bVal as _array), 4)
        for i, j in data.ia, *bVal as _array
            equal(t, double(i), j as _number)


class SomeData
    a: int = 1
    b: string = "1"
    c = true
    d: Foo?


[test(name="json classes")]
def classes(t: T?)
    t->skip("disabled, while js(void?) exists")
    var data = new SomeData()
    var foo <- Foo()
    unsafe
        data.d = addr(foo)
    var jv = JV(*data)
    success(t, jv is _object)
    equal(t, length(jv as _object), 4)
    var jvJv & = jv as _object
    success(t, jvJv |> key_exists("a"))
    success(t, jvJv |> key_exists("b"))
    success(t, jvJv |> key_exists("c"))
    success(t, jvJv |> key_exists("d"))

    equal(t, jvJv["a"] as _number, 1lf)
    equal(t, jvJv["b"] as _string, "1")
    equal(t, jvJv["c"] as _bool, true)
    success(t, jvJv["d"] is _object)
    success(t, (jvJv["d"] as _object)["ia"] is _array)
