require strings
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
    t |> equal( reference[0] as a, 13.)
    t |> equal( reference[1] as b, "hello")
    // load
    var test : FooBar[2]
    data |> mem_archive_load(test)
    delete data
    t |> equal( test[0] as a, 13.)
    t |> equal( test[1] as b, "hello")

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
    t |> equal( length(test), length(reference))
    for k in keys(reference)
        t |> equal( reference?[k] ?? -1, test?[k] ?? -100500)

[test]
def test_save_and_load_mem_array ( t:T? )
    // save
    var reference <- [{for tt in range(10); tt+1}]
    var data <- mem_archive_save(reference)
    // load
    var test : array<int>
    data |> mem_archive_load(test)
    delete data
    t |> equal( length(test), length(reference))
    for i,j in test,reference
        t |> equal( i, j)

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
        t |> equal( i, j+1)

struct Foo
    a : float
    b : string

[test]
def test_save_and_load_struct ( t:T? )
    // save
    var reference = [[Foo a=13.,b="hello"]]
    var data <- mem_archive_save(reference)
    t |> equal( reference.a, 13.)
    t |> equal( reference.b, "hello")
    // load
    var test : Foo
    data |> mem_archive_load(test)
    delete data
    t |> equal( test.a, 13.)
    t |> equal( test.b, "hello")

[test]
def test_save_and_load_tuple ( t:T? )
    // save
    var reference = [[auto 13.,"hello"]]
    var data <- mem_archive_save(reference)
    t |> equal( reference._0, 13.)
    t |> equal( reference._1, "hello")
    // load
    var test : tuple<float;string>
    data |> mem_archive_load(test)
    delete data
    t |> equal( test._0, 13.)
    t |> equal( test._1, "hello")

[test]
def test_save_and_load_mem_archive ( t:T? )
    // save
    var reference = 13ul
    var data <- mem_archive_save(reference)
    t |> equal( reference, 13ul)
    t |> equal( length(data), typeinfo(sizeof reference))
    // load
    var test : uint64
    data |> mem_archive_load(test)
    delete data
    t |> equal( reference, 13ul)
    t |> equal( test, 13ul)


def test_save_and_load_string ( t:T?; ref : string )
    // save
    var reference = ref
    var data <- mem_archive_save(reference)
    t |> equal( reference, ref)
    t |> equal( length(data), typeinfo(sizeof type<int>) + length(ref))
    // load
    var test : string
    data |> mem_archive_load(test)
    delete data
    t |> equal( reference, ref)
    t |> equal( test, ref)

[test]
def test_save_and_load_string ( t:T? )
    test_save_and_load_string(t, "13ul")

[test]
def test_save_and_load_empty_string ( t:T? )
    test_save_and_load_string(t, "")

[test]
def test_save_and_load_manual ( t:T? )
    var reference = 13
    // saving
    var mems = new MemSerializer()
    var sarch = [[Archive reading=false, stream=mems]]
    sarch |> serialize(reference)
    var data <- mems->extractData()
    t |> equal( length(data), typeinfo(sizeof reference))
    t |> equal( reference, 13)
    // restoring
    var test : int
    var memr = new MemSerializer(data)
    var rarch = [[Archive reading=true, stream=memr]]
    rarch |> serialize(test)
    if !memr->OK()
        t->fatal(memr->getLastError())
    delete data
    // compare
    t |> equal( reference, test)
    t |> equal( test, 13)
    t |> equal( reference, 13)

[export]
def main
    pass

