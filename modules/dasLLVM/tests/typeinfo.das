require dastest/testing_boost

require jit
require rtti

[jit]
def take_it ( t : auto(TT) )
    let tt = typeinfo(typename type<TT>)
    print("name is {tt}\n")

[jit]
def test_typeinfo
    take_it(type<int>)
    take_it(type<int?>)
    take_it(type<array<int>>)

class Foo
    a : int

[jit]
def public class_info_stub ( cl ) : StructInfo const?
    // this is a copy of rtti::class_info
    static_if typeinfo(is_pointer cl)
        if cl == null
            return null
        static_if typeinfo(is_class *cl)
            unsafe
                return reinterpret<StructInfo const?>((reinterpret<TypeInfo const?>((*cl).__rtti)).structType)
        else
            concept_assert(false,"not a pointer to a class")
            return null
    static_elif typeinfo(is_class cl)
        unsafe
            return reinterpret<StructInfo const?>((reinterpret<TypeInfo const?>(cl.__rtti)).structType)
    static_elif typeinfo(is_lambda cl)
        unsafe
            var l = reinterpret<int8??> addr(cl)
            if l == null
                return null
            var pti = reinterpret<TypeInfo const??> (l[0] - 16)
            return (*pti).structType
    else
        concept_assert(false,"this type does not have class info. its not a class or a lambda with options rtti")
        return null


[jit]
def test_rtti : StructInfo const?
    var f = new Foo()
    return class_info_stub(f)

[test]
def test_type_info ( t:T? )
    t |> run("typeinfo") <| @ ( t : T? )
        test_typeinfo()
        t |> success ( is_jit_function(@@<(a:int):void> take_it) )
        t |> success ( is_jit_function(@@<(a:int?):void> take_it) )
        t |> success ( is_jit_function(@@<(a:array<int>):void> take_it) )
        t |> success ( is_jit_function(@@test_typeinfo) )
    t |> run("typeinfo nojits") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_rtti) )
        print("rtti = {test_rtti()}\n")
