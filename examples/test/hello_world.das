options log = true, optimize=false

struct Foo
    a:int
    b:float
    c:double
    e:Foo?
    // f:string

[export]
def test
    let f : Foo
    let typename_f = typeinfo(typename f)
    let sizeof_foo  = typeinfo(sizeof type Foo)
    let typename_foo = typeinfo(typename type Foo)
    let is_pod_foo = typeinfo(is_pod type Foo)
    let is_raw_foo = typeinfo(is_raw type Foo)
    let f_has_a = typeinfo(has_field<a> f)
    let foo_has_aa = typeinfo(has_field<aa> type Foo)
    let is_struct_foo = typeinfo(is_raw type Foo)
    let is_struct_int = typeinfo(is_raw type int)
    // let has_ab = typeinfo(has_field<ab> type int)
    let is_ref_f = typeinfo(is_ref f)
    let is_ref_foo = typeinfo(is_ref type Foo)
    let is_ref_int = typeinfo(is_ref type int)
    let is_const_f = typeinfo(is_const f)
    let is_const_int_const = typeinfo(is_const type int const)
    let is_pointer_f = typeinfo(is_pointer f)
    let is_pointer_f_e = typeinfo(is_pointer f.e)
    return true
