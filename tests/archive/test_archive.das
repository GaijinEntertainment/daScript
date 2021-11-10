require daslib/archive
require dastest/testing_boost public

variant FooBar
    a : float
    b : string

[test]
def test_save_and_load_variant ( t:T? )
    // save
    var reference = [[auto [[FooBar a=13.]]; [[FooBar b="hello"]]  ]]
    var data <- mem_archive_save(reference)
    equal(t, reference[0] as a, 13.)
    equal(t, reference[1] as b, "hello")
    // load
    var test : FooBar[2]
    data |> mem_archive_load(test)
    delete data
    equal(t, test[0] as a, 13.)
    equal(t, test[1] as b, "hello")

[test]
def test_save_and_load_mem_table ( t:T? )
    // save
    var reference <- {{
        "one" => 1;
        "two" => 2
    }}
    var data <- mem_archive_save(reference)
    // load
    var test : table<string;int>
    data |> mem_archive_load(test)
    delete data
    equal(t, length(test), length(reference))
    for k in keys(reference)
        equal(t, reference[k], test?[k] ?? -100500)

[test]
def test_save_and_load_mem_array ( t:T? )
    // save
    var reference <- [{for t in range(10); t+1}]
    var data <- mem_archive_save(reference)
    // load
    var test : array<int>
    data |> mem_archive_load(test)
    delete data
    equal(t, length(test), length(reference))
    for i,j in test,reference
        equal(t, i, j)

[test]
def test_save_and_load_mem_dim ( t:T? )
    // save
    var reference = [[int 1;2;3;4;5]]
    var data <- mem_archive_save(reference)
    // load
    var test : int[5]
    data |> mem_archive_load(test)
    delete data
    for i,j in test,range(5)
        equal(t, i, j+1)

struct Foo
    a : float
    b : string

[test]
def test_save_and_load_struct ( t:T? )
    // save
    var reference = [[Foo a=13.,b="hello"]]
    var data <- mem_archive_save(reference)
    equal(t, reference.a, 13.)
    equal(t, reference.b, "hello")
    // load
    var test : Foo
    data |> mem_archive_load(test)
    delete data
    equal(t, test.a, 13.)
    equal(t, test.b, "hello")

[test]
def test_save_and_load_tuple ( t:T? )
    // save
    var reference = [[auto 13.,"hello"]]
    var data <- mem_archive_save(reference)
    equal(t, reference._0, 13.)
    equal(t, reference._1, "hello")
    // load
    var test : tuple<float;string>
    data |> mem_archive_load(test)
    delete data
    equal(t, test._0, 13.)
    equal(t, test._1, "hello")

[test]
def test_save_and_load_mem_archive ( t:T? )
    // save
    var reference = 13ul
    var data <- mem_archive_save(reference)
    equal(t, reference, 13ul)
    equal(t, length(data), typeinfo(sizeof reference))
    // load
    var test : uint64
    data |> mem_archive_load(test)
    delete data
    equal(t, reference, 13ul)
    equal(t, test, 13ul)

[test]
def test_save_and_load_string ( t:T? )
    // save
    var reference = "13ul"
    var data <- mem_archive_save(reference)
    equal(t, reference, "13ul")
    equal(t, length(data), typeinfo(sizeof type<int>) + 5)
    // load
    var test : string
    data |> mem_archive_load(test)
    delete data
    equal(t, reference, "13ul")
    equal(t, test, "13ul")

[test]
def test_save_and_load_manual ( t:T? )
    var reference = 13
    // saving
    var mems = new MemSerializer()
    var sarch = [[Archive reading=false, stream=mems]]
    sarch |> serialize(reference)
    var data <- mems->extractData()
    equal(t, length(data), typeinfo(sizeof reference))
    equal(t, reference, 13)
    // restoring
    var test : int
    var memr = new MemSerializer(data)
    var rarch = [[Archive reading=true, stream=memr]]
    rarch |> serialize(test)
    if !memr->OK()
        t->fatal(memr->getLastError())
    delete data
    // compare
    equal(t, reference, test)
    equal(t, test, 13)
    equal(t, reference, 13)

[export]
def main
    pass

