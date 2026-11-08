options gen2

require UnitTest
require dastest/testing_boost public

[checkRange]
struct Foo {
    @min = 0@max = 3 a : int = 1
    @min = 10@max = 20 b : int = 15
}

struct TestObjectFooNative {
    fooData : int
}

struct TestObjectBarNative {
    fooPtr : TestObjectFooNative?
    barData : float
}

[sideeffects]
def test_native(t : T?) {
    var a : TestObjectFooNative?
    var b : TestObjectBarNative?
    var idummy : int
    var fdummy : float
    a?.fooData ?? idummy = 1
    t |> equal(idummy, 1)
    a = new TestObjectFooNative
    a?.fooData ?? idummy = 2
    t |> success(a.fooData == 2 && idummy == 1)
    b?.barData ?? fdummy = 1.0
    t |> equal(fdummy, 1.0)
    b = new TestObjectBarNative
    b?.barData ?? fdummy = 2.0
    t |> success(b.barData == 2.0 && fdummy == 1.0)
    b?.fooPtr?.fooData ?? idummy = 3
    t |> equal(idummy, 3)
    b.fooPtr = a
    b?.fooPtr?.fooData ?? idummy = 4
    t |> success(b.fooPtr.fooData == 4 && idummy == 3)
}

[sideeffects]
def test_handled(t : T?) {
    var a : TestObjectFoo?
    var b : TestObjectBar?
    var idummy : int
    var fdummy : float
    a?.fooData ?? idummy = 1
    t |> equal(idummy, 1)
    a = new TestObjectFoo
    a?.fooData ?? idummy = 2
    t |> success(a.fooData == 2 && idummy == 1)
    b?.barData ?? fdummy = 1.0
    t |> equal(fdummy, 1.0)
    b = new TestObjectBar
    b?.barData ?? fdummy = 2.0
    t |> success(b.barData == 2.0 && fdummy == 1.0)
    b?.fooPtr?.fooData ?? idummy = 3
    t |> equal(idummy, 3)
    b.fooPtr = a
    b?.fooPtr?.fooData ?? idummy = 4
    t |> success(b.fooPtr.fooData == 4 && idummy == 3)
    a.fooData = 7
    t |> equal(a.propAdd13, 20)
    t |> equal(b.getFoo.fooData, 7)
    t |> equal(deref(b.getFooPtr).fooData, 7)
    t |> success(deref(a) == deref(b.fooPtr))
    t |> success(!(deref(a) != deref(b.fooPtr)))
    // call member
    hit_me(*a, 1, 2)
    t |> equal(a.fooData, 3)
    unsafe {
        delete a
        delete b
    }
}

[export]
def test_fancy_container(a : array<FancyClass>) {
    feint("a = {a}\n")
}

def test_fancy(t : T?) {
    var fc0 = FancyClass()
    t |> equal(fc0.value, 13)
    var fc = FancyClass(1, 2)
    t |> equal(fc.value, 3)
    using() $(var f : FancyClass) {
        t |> equal(f.value, 13)
    }
    using(1, 3) $(var f : FancyClass) {
        t |> equal(f.value, 4)
    }
    // otherwise we leak hidden std::string inside fancy class
    deleteFancyClass(fc0)
    deleteFancyClass(fc)
}

def test_nc_alias(t : T?) {
    var lookingAt = new TestObjectFoo
    let lookPos = lookingAt.lookAt ?? float3()
    t |> equal(lookPos, float3())
    unsafe {
        delete lookingAt
    }
}

def is_valid_eid(e : EntityId) : bool => !!e

def test_handle_to_bool(t : T?) {
    // operator!(EntityId):bool is a type-changing conversion: !!x must
    // keep both nots, never fold to x
    let valid = EntityId(0)
    let invalid = make_invalid_id()
    t |> success(!invalid)              // single ! : "is invalid" -> true
    // !!valid : "is valid" -> true ; wrong fold -> EntityId(0) -> false
    t |> success(!(!valid))
    t |> success(!!valid)
    t |> success(!(!!invalid))          // !!invalid -> false, so !(..) -> true
    t |> success(is_valid_eid(valid))   // bool-returning function : exercises the return boundary
    t |> success(!is_valid_eid(invalid))
    var hit = 0
    if (!!valid) { hit = 1 }            // branch boundary
    t |> equal(hit, 1)
}

[test(description = "?. with ?? assignment writes through a live native or handled pointer chain and falls back on null, handled properties, members, constructors and using blocks work, and !! on a handle stays a bool conversion")]
def test_handle(t : T?) {
    test_native(t)
    test_handled(t)
    test_fancy(t)
    test_nc_alias(t)
    test_handle_to_bool(t)
}

def cmresTest(a : int) => TestObjectFoo(fooData = a)

struct OverFoo {
    a : int
    foo : TestObjectFoo
}

var {
    gstackref = TestObjectFoo(fooData = 6)
    gstackrefa : TestObjectFoo[2]
}

[test(description = "a handled type made by field initializer lands intact in a global, a global fixed array, a local, a cmres return, a struct field and a new allocation")]
def test_make_handle(t : T?) {
    // stack-ref initialization
    gstackrefa[0] = TestObjectFoo(fooData = 1)
    gstackrefa[1] = TestObjectFoo(fooData = 2)
    t |> equal(gstackref.fooData, 6)
    t |> success(gstackrefa[0].fooData == 1 && gstackrefa[1].fooData == 2)
    // regular local variable
    let local_t = TestObjectFoo(fooData = 5)
    t |> equal(local_t.fooData, 5)
    // cmres variable
    let cmres_t = cmresTest(13)
    t |> equal(cmres_t.fooData, 13)
    // mixed in the other type
    let mixed = fixed_array(OverFoo(a = 1, foo = TestObjectFoo(fooData = 1)),
                            OverFoo(a = 2, foo = TestObjectFoo(fooData = 2)))
    t |> success(mixed[0].foo.fooData == 1 && mixed[1].foo.fooData == 2)
    // ascend
    var ascend_t <- new TestObjectFoo(fooData = 1)
    t |> equal(ascend_t.fooData, 1)
    unsafe {
        delete ascend_t
    }
    t |> success(true)
}

struct S1 {
    a : int64 = 1l
    b : int64 = 2l
    c : int = 3
}

struct S2 : S1 {
    d : int = 4
}

[cpp_layout(pod=false)]
struct CppS1 {
    vtable : void?
    b : int64 = 2l
    c : int = 3
}

[cpp_layout(pod=false)]
struct CppS2 : CppS1 {
    d : int = 4
}

[test(description = "a plain derived struct packs its first field into the base's tail padding, while a [cpp_layout] struct matches the C++ compiler's size and field offsets")]
def test_cpp_layout(t : T?) {
    t |> equal(typeinfo sizeof(type<S1>), 24)
    t |> equal(typeinfo sizeof(type<S2>), 24)
    t |> equal(typeinfo offsetof < d > (type<S2>), 20)
    t |> equal(typeinfo sizeof(type<CppS1>), CppS1Size())
    t |> equal(typeinfo sizeof(type<CppS2>), CppS2Size())
    t |> equal(typeinfo offsetof < d > (type<CppS2>), CppS2DOffset())
}

[test(description = "each() over a bound std::vector visits every element in order, so find_index_if returns the matching element's index")]
def test_each_std_vector(t : T?) {
    testFooArray() $(arr) {
        t |> equal(length(arr), 10)
        t |> equal(capacity(arr), 16)
        let index = find_index_if(each(arr)) $(foo) {
            return foo.fooData == 5
        }
        t |> equal(index, 5)
    }
}

[test(description = "indexing a bound std::vector by position reads the element stored there")]
def test_stdvec_r2v(t : T?) {
    testFooArray() $(arr) {
        for (i in range(arr |> length)) {
            t |> equal(arr[i].fooData, i)
        }
    }
}
