require dastest/testing_boost

require llvm/llvm_jit
require jit

struct Foo
    a : int = 1234
    b : float = 1234.0
    c : string = "1234"
    d : int = 1234

struct Bar
    a : int = 13
    foo : Foo
    b : int

[jit]
def struct_field ( var f : Foo )
    f.a = 1
    f.b = 2.0
    f.c = "hello"
    return f.d

[jit]
def struct_ptr_field ( var f : Foo? )
    f.a = 11
    f.b = 22.0
    f.c = "hhello"
    return f.d

[jit]
def struct_ctor_call
    return Foo()

[jit]
def make_struct_cmres
    return [[Foo a=1, c="bar"]]

[jit]
def make_struct_cmres_with_init
    return [[Foo() a=1, c="bar"]]

[jit]
def make_struct_heap
    return new [[Foo a=1, c="bar"]]

[jit]
def make_struct_heap_with_init
    return new [[Foo() a=1, c="bar"]]

[jit]
def make_struct_with_substruct_cmres
    return [[Bar a=1, foo=[[Foo a=2, b=3.]], b=4]]

[jit]
def make_struct_with_substruct_cmres_with_init
    return [[Bar() b=10, foo=[[Foo() a=2, b=3.]]]]

[jit]
def make_struct_with_substruct_heap
    return new [[Bar a=1, foo=[[Foo a=2, b=3.]], b=4]]

[jit]
def make_struct_with_substruct_heap_with_init
    return new [[Bar() b=10, foo=[[Foo() a=2, b=3.]]]]

[jit]
def make_struct_with_field_call_to_cmres
    return [[Bar a=1, foo=Foo(), b=4]]

[jit]
def make_struct_on_stack(which:bool)
    var a = [[Foo a=1, b=2.0, c="3", d=4]]
    var b = [[Foo a=2, b=3.0, c="4", d=5]]
    return which ? a : b

[jit]
def make_struct_with_substruct_on_stack(which:bool)
    var a = [[Bar a=1, foo=[[Foo a=2, b=3.]], b=4]]
    var b = [[Bar a=2, foo=[[Foo a=3, b=4.]], b=5]]
    return which ? a : b

[jit]
def new_struct_on_heap
    return new Foo

[jit]
def new_struct_with_ctor_on_heap
    return new Foo()

[test]
def test_struct ( t:T? )
    t |> run("struct field") <| @ ( t : T? )
        var f = [[Foo d=13]]
        t |> success ( is_jit_function(@@struct_field) )
        var res = struct_field(f)
        t |> equal ( 13, res )
        t |> equal ( 1, f.a )
        t |> equal ( 2.0, f.b )
        t |> equal ( "hello", f.c )
    t |> run("struct ptr field") <| @ ( t : T? )
        var f = [[Foo d=14]]
        t |> success ( is_jit_function(@@struct_ptr_field) )
        var res = struct_ptr_field(unsafe(addr(f)))
        t |> equal ( 14, res )
        t |> equal ( 11, f.a )
        t |> equal ( 22.0, f.b )
        t |> equal ( "hhello", f.c )
    t |> run("struct ctor call") <| @ ( t : T? )
        var f = struct_ctor_call()
        t |> success ( is_jit_function(@@struct_ctor_call) )
        t |> equal ( 1234, f.a )
        t |> equal ( 1234.0, f.b )
        t |> equal ( "1234", f.c )
        t |> equal ( 1234, f.d )
    t |> run("make struct cmres") <| @ ( t : T? )
        var f = make_struct_cmres()
        t |> success ( is_jit_function(@@make_struct_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 0.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 0, f.d )
    t |> run("make struct cmres with init") <| @ ( t : T? )
        var f = make_struct_cmres_with_init()
        t |> success ( is_jit_function(@@make_struct_cmres_with_init) )
        t |> equal ( 1, f.a )
        t |> equal ( 1234.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 1234, f.d )
    t |> run("make struct with substruct cmres") <| @ ( t : T? )
        var f = make_struct_with_substruct_cmres()
        t |> success ( is_jit_function(@@make_struct_with_substruct_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 2, f.foo.a )
        t |> equal ( 3., f.foo.b )
        t |> equal ( "", f.foo.c )
        t |> equal ( 0, f.foo.d )
        t |> equal ( 4, f.b )
    t |> run("make struct with substruct cmres with init") <| @ ( t : T? )
        var f = make_struct_with_substruct_cmres_with_init()
        t |> success ( is_jit_function(@@make_struct_with_substruct_cmres_with_init) )
        t |> equal ( 13, f.a )
        t |> equal ( 2, f.foo.a )
        t |> equal ( 3., f.foo.b )
        t |> equal ( "1234", f.foo.c )
        t |> equal ( 1234, f.foo.d )
        t |> equal ( 10, f.b )
    t |> run("make struct with substruct equal to call to cmres") <| @ ( t : T? )
        var f = make_struct_with_field_call_to_cmres()
        t |> success ( is_jit_function(@@make_struct_with_field_call_to_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 1234,  f.foo.a )
        t |> equal ( 1234., f.foo.b )
        t |> equal ( "1234",f.foo.c )
        t |> equal ( 1234,  f.foo.d )
        t |> equal ( 4, f.b )
    t |> run("make struct on stack") <| @ ( t : T? )
        var a = make_struct_on_stack(true)
        var b = make_struct_on_stack(false)
        t |> success ( is_jit_function(@@make_struct_on_stack) )
        t |> equal ( 1,   a.a )
        t |> equal ( 2.,  a.b )
        t |> equal ( "3", a.c )
        t |> equal ( 4,   a.d )
        t |> equal ( 2,   b.a )
        t |> equal ( 3.,  b.b )
        t |> equal ( "4", b.c )
        t |> equal ( 5,   b.d )
    t |> run("make struct with substruct on stack") <| @ ( t : T? )
        var a = make_struct_with_substruct_on_stack(true)
        var b = make_struct_with_substruct_on_stack(false)
        t |> success ( is_jit_function(@@make_struct_with_substruct_on_stack) )
        t |> equal ( 1,  a.a )
        t |> equal ( 2,  a.foo.a )
        t |> equal ( 3., a.foo.b )
        t |> equal ( "", a.foo.c )
        t |> equal ( 0,  a.foo.d )
        t |> equal ( 4,  a.b )
        t |> equal ( 2,  b.a )
        t |> equal ( 3,  b.foo.a )
        t |> equal ( 4., b.foo.b )
        t |> equal ( "", b.foo.c )
        t |> equal ( 0,  b.foo.d )
        t |> equal ( 5,  b.b )
    t |> run("new struct on heap") <| @ ( t : T? )
        t |> success ( is_jit_function(@@new_struct_on_heap) )
        var f = new_struct_on_heap()
        t |> equal ( 0, f.a )
        t |> equal ( 0., f.b )
        t |> equal ( "", f.c )
        t |> equal ( 0, f.d )
    t |> run("new struct with ctor on heap") <| @ ( t : T? )
        t |> success ( is_jit_function(@@new_struct_with_ctor_on_heap) )
        var f = new_struct_with_ctor_on_heap()
        t |> equal ( 1234, f.a )
        t |> equal ( 1234., f.b )
        t |> equal ( "1234", f.c )
        t |> equal ( 1234, f.d )
    t |> run("make struct heap") <| @ ( t : T? )
        var f = make_struct_heap()
        t |> success ( is_jit_function(@@make_struct_heap) )
        t |> equal ( 1, f.a )
        t |> equal ( 0.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 0, f.d )
    t |> run("make struct heap with init") <| @ ( t : T? )
        var f = make_struct_heap_with_init()
        t |> success ( is_jit_function(@@make_struct_heap_with_init) )
        t |> equal ( 1, f.a )
        t |> equal ( 1234.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 1234, f.d )
    t |> run("make struct with substruct heap") <| @ ( t : T? )
        var f = make_struct_with_substruct_heap()
        t |> success ( is_jit_function(@@make_struct_with_substruct_heap) )
        t |> equal ( 1, f.a )
        t |> equal ( 2, f.foo.a )
        t |> equal ( 3., f.foo.b )
        t |> equal ( "", f.foo.c )
        t |> equal ( 0, f.foo.d )
        t |> equal ( 4, f.b )
    t |> run("make struct with substruct heap with init") <| @ ( t : T? )
        var f = make_struct_with_substruct_heap_with_init()
        t |> success ( is_jit_function(@@make_struct_with_substruct_heap_with_init) )
        t |> equal ( 13, f.a )
        t |> equal ( 2, f.foo.a )
        t |> equal ( 3., f.foo.b )
        t |> equal ( "1234", f.foo.c )
        t |> equal ( 1234, f.foo.d )
        t |> equal ( 10, f.b )
