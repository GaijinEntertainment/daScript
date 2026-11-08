options gen2
require UnitTest
require math
require dastest/testing_boost public
require daslib/contracts

let allow_broken = false

struct Foo {
    a : int
}

struct Bar : Foo {
    b : int
}

struct Far : Bar {
    c : int
}

def test(_a : Foo; _b : Foo) => "foo foo"
def test(_a : Foo; _b : Bar) => "foo bar"
def test(_a : Bar; _b : Foo) => "bar foo"

def test_distance(t : T?) {
    t |> equal(test(Foo(uninitialized), Foo(uninitialized)), "foo foo")
    t |> equal(test(Foo(uninitialized), Bar(uninitialized)), "foo bar")
    t |> equal(test(Bar(uninitialized), Foo(uninitialized)), "bar foo")
    t |> equal(test(Far(uninitialized), Bar(uninitialized)), "foo bar")
    static_if (allow_broken) {
        test(Bar(uninitialized), Bar(uninitialized))
    }
}

def gen_1(a : Foo; b) => "Foo *"
def gen_1(a; b) => "* *"

def gen_2(a : Foo; b) => "Foo *"
def gen_2(a : Bar; b) => "Bar *"

def gen_3(arr) => "*"
def gen_3(arr : auto[]) => "[*]"
def gen_3(arr : auto[3]) => "[3]"

def gen_4(arr) => "*"
def gen_4(arr : array<auto>) => "array<*>"

def gen_5(arr : array<auto?>) => "<*?>"
def gen_5(arr : array<auto>) => "<*>"

def gen_6(arr) => "*"
def gen_6(arr : table<auto; auto?>) => "<*:*?>"
def gen_6(arr : table<int; auto>) => "<int:*>"
def gen_6(arr : table<int; auto?>) => "<int:*?>"

def gen_7(arr : tuple<auto; auto?; int>) => "<*:*?:int>"
def gen_7(arr : tuple<auto; auto; auto>) => "<*:*:*>"

def gen_8(arr : auto) => "*"
def gen_8(arr : function<(arg : auto; arg2 : int) : auto?>) => "(*:int):?"
def gen_8(arr : function<(arg : auto; arg2 : int) : auto>) => "(*:int):*"

[test(description = "overload resolution picks the most specialized generic: smallest cast distance, concrete over auto, derived over base, fixed dim, container, pointer, table, tuple and function subtypes")]
def test_partial_specialization(t : T?) {
    t |> run("cast distance") @(t : T?) {
        test_distance(t)
    }
    t |> run("non-auto more specialized than auto") @(t : T?) {
        t |> equal(gen_1(Foo(uninitialized), Foo(uninitialized)), "Foo *")
    }
    t |> run("exact type more specialized than base") @(t : T?) {
        t |> equal(gen_2(Bar(uninitialized), Foo(uninitialized)), "Bar *")
    }
    t |> run("dim specialization") @(t : T?) {
        t |> equal(gen_3(fixed_array<int>(1, 2, 3, 4)), "[*]")
        t |> equal(gen_3(fixed_array<int>(1, 2, 3)), "[3]")
    }
    t |> run("container specialization") @(t : T?) {
        let ta : array<int>
        t |> equal(gen_4(ta), "array<*>")
    }
    t |> run("pointer subtype specialization") @(t : T?) {
        let tp : array<int?>
        t |> equal(gen_5(tp), "<*?>")
    }
    t |> run("table multi-subtype specialization") @(t : T?) {
        let tt : table<int; int?>
        t |> equal(gen_6(tt), "<int:*?>")
        let tf : table<float; int?>
        t |> equal(gen_6(tf), "<*:*?>")
    }
    t |> run("tuple subtype specialization") @(t : T?) {
        let tu : tuple<float; int?; int>
        t |> equal(gen_7(tu), "<*:*?:int>")
    }
    t |> run("function signature specialization") @(t : T?) {
        let fn : function<(arg : float; arg2 : int) : void?>
        t |> equal(gen_8(fn), "(*:int):?")
    }
}

def on_armor_hit_fx_effect_es(visual_armor_hit_dot_effectName : string) {
    var tm : float3x4
    identity(tm)
    return UnitTest::start_effect(visual_armor_hit_dot_effectName, tm)
}

[test(description = "a module-qualified call to a bound generic resolves and returns its result, also from inside a function whose result type it infers")]
def test_fully_qualified_generic_name(t : T?) {
    t |> success(!on_armor_hit_fx_effect_es("test"))
    var tm : float3x4
    identity(tm)
    UnitTest::start_effect("test-test", tm)
}

def instantiated(x : auto) : int => 1

def user(x : auto) : int => instantiated(x)

def holds_void_pointer(x : auto) : bool => typeinfo is_void_pointer(x._0)

[test(description = "a generic over a tuple instantiates for a void? tuple after instances for other pointer tuples")]
def test_a_generic_over_a_tuple_instantiates_for_void_ptr_after_other_pointers(t : T?) {
    var a : int?
    var b : float?
    var c : void?
    t |> equal(user((a,)) + user((b,)) + user((c,)), 3)
}

[test(description = "a void? tuple gets its own generic instance, distinct from the int? tuple instance made before it")]
def test_a_void_ptr_tuple_gets_its_own_instance_after_one_other_pointer(t : T?) {
    var a : int?
    var c : void?
    t |> equal(holds_void_pointer((a,)), false)
    t |> equal(holds_void_pointer((c,)), true)
}

[test(description = "a variant or an iterator over void? gets its own generic instance beside the int? one")]
def test_a_variant_or_iterator_over_void_ptr_gets_its_own_instance(t : T?) {
    var va : variant<v : int?>
    let vc : variant<v : void?>
    t |> equal(user(va) + user(vc), 2)
    let ia : iterator<int?>
    let ic : iterator<void?>
    t |> equal(user(ia) + user(ic), 2)
}

[sideeffects]
def take3(t : float3) {
    feint("t={t}")
}

[export]
def indexFail(arr : Point3Array) {
    take3(arr[0])
    let t & = unsafe(arr[1])
    take3(t)
}

[export]
def swizzleFail(battleAreaPoints : Point3Array) {
    var pointStart : float2
    pointStart.x = battleAreaPoints[0].x
    feint("{pointStart}")
}

[export]
def arrayAliasFail(t : T?) {
    let foo : array<Point3> <- [float3(1, 2, 3), float3(2, 3, 4), float3(3, 4, 5)]
    temp_array_alias_example(foo) $(tfoo) {
        var f = float3(0)
        for (it in tfoo) {
            f += it
        }
        t |> equal(f, float3(6, 9, 12))
    }
}

[test(description = "a C++ type alias of float3 (Point3) behaves as float3 in typename, fields, properties, swizzles, arithmetic and aliased arrays")]
def test_type_alias(t : T?) {
    var a = UnitTest::getSamplePoint3()  // nolint:LINT003 - `let` would make typename "float3 const" and break the assert below
    t |> equal(typeinfo typename(a), "float3")
    t |> equal(UnitTest::doubleSamplePoint3(a), float3(0, 2, 4))
    // also swizzle
    var foo : TestObjectFoo
    t |> success(typeinfo is_dim(foo.fooArray) && typeinfo dim(foo.fooArray) == 3)
    foo.hit = UnitTest::getSamplePoint3()
    t |> success(foo.hit.x == 0.0 && foo.hit.y == 1.0 && foo.hit.z == 2.0)
    let qq = foo.hit
    t |> success(qq.x == 0.0 && qq.y == 1.0 && qq.z == 2.0)
    let qq1 = UnitTest::getSamplePoint3()
    t |> success(qq1.x == 0.0 && qq1.y == 1.0 && qq1.z == 2.0)
    let qq2 = foo.hitPos
    t |> success(qq2.x == 0.0 && qq2.y == 1.0 && qq2.z == 2.0)
    var qq3 = foo.hitPos  // nolint:LINT003 - the var twin of qq2 is the tested construct
    t |> success(qq3.x == 0.0 && qq3.y == 1.0 && qq3.z == 2.0)
    let qq4 = foo.hitPosRef * float3(1)
    t |> success(qq4.x == 0.0 && qq4.y == 1.0 && qq4.z == 2.0)
    var hitDir = normalize(-foo.hit |> UnitTest::doubleSamplePoint3())
    let nhd = normalize(-float3(0, 2, 4))
    t |> success(hitDir.x == nhd.x && hitDir.y == nhd.y && hitDir.z == nhd.z)
    hitDir = normalize(-foo.hit)
    let nhdh = normalize(-float3(0, 1, 2))
    t |> success(hitDir.x == nhdh.x && hitDir.y == nhdh.y && hitDir.z == nhdh.z)
    t |> equal(foo.wordFoo, "foo")

    testPoint3Array() $(arr) {
        t |> equal(arr |> length, 10)
        for (p in arr) {
            t |> success(p.x == 1.0 && p.y == 2.0 && p.z == 3.0)
        }
    }

    arrayAliasFail(t)
}

[type_function, unused_argument(a)]
def take(a : auto(TT); a1, a2 : int) => typeinfo sizeof(type<TT>) + a1 + a2

[test(description = "a [type_function] takes a type<T> argument and computes from its size")]
def test_type_function(t : T?) {
    let size = take(type<int>, 1, 2)
    t |> equal(7, size)
}

def foo(var a : int& | auto) {
    a = 42
}

def bar(var a : auto | int&) {
    a = 42
}

[test(description = "an option type picks its first matching alternative, so int& | auto binds by reference and auto | int& by value")]
def test_option_type(t : T?) {
    t |> run("preserve ref") @(t : T?) {
        var a : int
        bar(a)              // match foo(auto), not foo(int&)
        t |> equal(0, a)
        foo(a)              // match foo(int &), not foo(auto)
        t |> equal(42, a)
    }
}

def take_not(a) => 1

[!expect_dim(a)]
def take_not(a) => 2

[expect_any_tuple(a) || expect_any_variant(a)]
def take_or(a) => 1

def take_or(a) => 2

[expect_any_tuple(a) && expect_any_variant(b)]
def take_and(a, b) => 1

def take_and(a, b) => 2

[expect_any_tuple(a) ^^ expect_any_variant(b)]
def take_xor(a, b) => 1

def take_xor(a, b) => 2

variant CtFoo {
    i : int
    f : float
}

[test(description = "contract annotations combine with !, ||, && and ^^ to gate which generic overload a call picks")]
def test_logical_ops(t : T?) {
    t |> run("!") @(t : T?) {
        let a : int[3]
        t |> equal(1, take_not(a))
        t |> equal(2, take_not(13))
    }
    t |> run("||") @(t : T?) {
        t |> equal(2, take_or(13))
        t |> equal(1, take_or((1, "2", 3.)))
        t |> equal(1, take_or(CtFoo(i = 1)))
    }
    t |> run("&&") @(t : T?) {
        t |> equal(2, take_and(13, 13))
        t |> equal(2, take_and(13, CtFoo(i = 1)))
        t |> equal(2, take_and((1, "2", 3.), 13))
        t |> equal(1, take_and((1, "2", 3.), CtFoo(i = 1)))
    }
    t |> run("^^") @(t : T?) {
        t |> equal(2, take_xor(13, 13))
        t |> equal(1, take_xor(13, CtFoo(i = 1)))
        t |> equal(1, take_xor((1, "2", 3.), 13))
        t |> equal(2, take_xor((1, "2", 3.), CtFoo(i = 1)))
    }
}

[sideeffects]
def take_i(a : auto ==&) => false

[sideeffects]
def take_i(a : auto& ==&) => true

[sideeffects]
def tam(var a : auto(TT)& ==&) : array<TT -const -&> {
    var A : array<TT -const -&>
    A |> emplace(a)
    return <- A
}

[sideeffects]
def tam(var a : auto(TT) ==&) : array<TT -const -&> {
    var A : array<TT -const -&>
    A |> push(a + 1)
    return <- A
}

[test(description = "the ==& contract routes a reference argument and a value argument to different generic overloads")]
def test_builtin_contracts(t : T?) {
    t |> run("==&") @(t : T?) {
        var a = 1
        t |> equal(true, take_i(a))
        t |> equal(false, take_i(1))
        let A <- tam(1)
        t |> equal(1, length(A))
        t |> equal(2, A[0])
        let B <- tam(a)
        t |> equal(1, length(B))
        t |> equal(1, B[0])
        t |> equal(0, a)
    }
}
