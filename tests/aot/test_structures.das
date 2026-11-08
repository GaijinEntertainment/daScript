options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require math

struct Point {
    x : float
    y : float
}

def distance(a, b : Point) : float {
    let dx = a.x - b.x
    let dy = a.y - b.y
    return sqrt(dx * dx + dy * dy)
}

[test(description = "Struct construction and field reads work in AOT")]
def test_struct_basics(t : T?) {
    t |> run("struct creation and access") @(t : T?) {
        let p = Point(x = 3.0, y = 4.0)
        t |> equal(p.x, 3.0)
        t |> equal(p.y, 4.0)
    }
    t |> run("struct function") @(t : T?) {
        let a = Point(x = 0.0, y = 0.0)
        let b = Point(x = 3.0, y = 4.0)
        t |> equal(distance(a, b), 5.0)
    }
}

struct Counter {
    value : int
}

def inc(var c : Counter) {
    c.value++
}

def get_value(c : Counter) : int => c.value

[test(description = "Struct field mutation is visible afterwards in AOT")]
def test_struct_mutation(t : T?) {
    t |> run("struct pass by ref") @(t : T?) {
        var c = Counter(value = 0)
        inc(c)
        inc(c)
        inc(c)
        t |> equal(get_value(c), 3)
    }
}

class Animal {
    name : string
    def abstract speak() : string
}

class Dog : Animal {
    def override speak() : string => "Woof"
}

class Cat : Animal {
    def override speak() : string => "Meow"
}

[test(description = "Class construction, fields and methods work in AOT")]
def test_classes(t : T?) {
    t |> run("class hierarchy") @(t : T?) {
        unsafe {
            var inscope dog <- new Dog()
            dog.name = "Rex"
            t |> equal(dog.name, "Rex")
            t |> equal(dog->speak(), "Woof")

            var inscope cat <- new Cat()
            cat.name = "Whiskers"
            t |> equal(cat.name, "Whiskers")
            t |> equal(cat->speak(), "Meow")
        }
    }
}

[test(description = "Array creation, push, index and iteration work in AOT")]
def test_arrays(t : T?) {
    t |> run("array operations") @(t : T?) {
        var arr : array<int>
        arr |> reserve(5)
        for (i in range(5)) {
            arr |> push(i * i)
        }
        t |> equal(length(arr), 5)
        t |> equal(arr[0], 0)
        t |> equal(arr[1], 1)
        t |> equal(arr[2], 4)
        t |> equal(arr[3], 9)
        t |> equal(arr[4], 16)
    }
    t |> run("table operations") @(t : T?) {
        var tab : table<string; int>
        unsafe {
            tab["one"] = 1
            tab["two"] = 2
            tab["three"] = 3
        }
        t |> equal(tab?["one"] ?? -1, 1)
        t |> equal(tab?["two"] ?? -1, 2)
        t |> equal(tab?["missing"] ?? -1, -1)
    }
}

[test(description = "Variant construction and access work in AOT")]
def test_variant(t : T?) {
    t |> run("tuple") @(t : T?) {
        let tup = 42 => "hello"
        t |> equal(tup._0, 42)
        t |> equal(tup._1, "hello")
    }
}

// Coverage: AOT codegen when a struct and an enum share a name (#3065, fuzzer).
// daslang keeps structs and enums in separate symbol tables, so this compiles; AOT
// used to emit `struct var1` and `enum class var1` into the same namespace -- a hard
// C++ redefinition. The emitter now disambiguates by prepending `_S`/`_E` (`_Svar1` / `_Evar1`).
//
// The struct cannot be referenced by its (now ambiguous) bare name, but it is still
// emitted as a this-module type, and that emission is what forces the collision in the
// generated C++. The test below exercises the enum at runtime (its access sites take
// the renamed `_Evar1::...` form); the regression itself is the generated C++ compiling.

struct var1 {
    a : int
}

enum var1 {
    x
    y
}

[test(description = "A struct and an enum sharing a name emit distinct C++ names and the enum works at run time in AOT")]
def test_struct_enum_same_name(t : T?) {
    t |> run("aot enum shadowed by same-named struct") @(t : T?) {
        let e = var1.x
        t |> equal(e == var1.x, true)
        t |> equal(e == var1.y, false)
        t |> equal(int(var1.y), 1)
    }
}

// AOT emits structures in by-value dependency order, not source order: Outer embeds Inner,
// which is declared later, and Inner's function-typed field names Outer in its signature. A
// sort that reads the signature as a by-value dependency sees a cycle and falls back to source
// order, where the generated C++ embeds an incomplete type. Holder and Inc are the other shape:
// a genuine container cycle (each holds an array of the other), which the sort cannot place and
// appends unsorted - legal C++, because TArray<T> holds no T by value. The AOT build of this
// file is the gate; the assertions only keep the shapes alive.

struct Outer {
    inner : Inner
    tag : int
}

struct Inner {
    on_outer : function<(o : Outer) : int>
    weight : float
}

struct Holder {
    arr : array<Inc>
    n : int
}

struct Inc {
    back : array<Holder>
    w : float
}

def outer_tag(o : Outer) : int => o.tag * 2

[test(description = "Structures embedding each other by value or by container are emitted in a compilable order in AOT")]
def test_struct_order(t : T?) {
    t |> run("an outer struct embeds a later-declared inner whose signature names the outer") @(t : T?) {
        var o = Outer(tag = 21, inner = Inner(on_outer = @@outer_tag, weight = 0.5))
        t |> equal(invoke(o.inner.on_outer, o), 42)
        t |> equal(o.inner.weight, 0.5)
    }
    t |> run("two structs holding arrays of each other sort as a container cycle") @(t : T?) {
        var h = Holder(n = 3)
        var inc = Inc(w = 1.5)
        h.arr |> emplace(inc)
        var inner = Holder(n = 7)
        h.arr[0].back |> emplace(inner)
        t |> equal(h.arr[0].back[0].n, 7)
        t |> equal(h.arr[0].w, 1.5)
        delete h
    }
}
