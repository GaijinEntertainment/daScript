options gen2
options no_unused_block_arguments = true
options no_unused_function_arguments = false

require dastest/testing_boost public
require math

struct ThreeInts {
    a : int
    b : int
    c : int
}

// typeinfo leaves this dimension unresolved in the first pass
let UNRESOLVED_DIM = typeinfo sizeof(type<ThreeInts>) / 4

struct WithArr {
    a : array<int>
}

variant HeapVariant {
    p : int
    w : WithArr
}

tuple HeapTuple {
    n : int
    a : array<int>
}

[test(description = "clone between two inferred variant locals copies the source's case and value")]
def test_clone_variant_between_inferred_locals(t : T?) {
    var src = HeapVariant(p = 1)
    var dst = HeapVariant(p = 2)
    dst := src
    t |> success(dst is p)
    t |> equal(dst as p, 1)
}

[test(description = "clone into an inferred variant local deep-copies a heap-owning case, so a later write to the source does not reach it")]
def test_clone_variant_deep_copies_heap_alternative(t : T?) {
    var src = HeapVariant(w = WithArr(a <- [1, 2, 3]))
    var dst = HeapVariant(p = 2)
    dst := src
    (src as w).a[0] = 100
    t |> success(dst is w)
    t |> equal(length((dst as w).a), 3)
    t |> equal((dst as w).a[0], 1)
}

[test(description = "clone from an inferred let variant into a var copies the heap-owning case")]
def test_clone_variant_from_inferred_let(t : T?) {
    let src = HeapVariant(w = WithArr(a <- [4, 5]))
    var dst = HeapVariant(p = 2)
    dst := src
    t |> success(dst is w)
    t |> equal((dst as w).a[1], 5)
}

[test(description = "clone between two inferred named-tuple locals deep-copies the array field")]
def test_clone_tuple_between_inferred_locals(t : T?) {
    var src = HeapTuple(n = 1, a <- [1, 2])
    var dst = HeapTuple(n = 2)
    dst := src
    src.a[0] = 100
    t |> equal(dst.n, 1)
    t |> equal(length(dst.a), 2)
    t |> equal(dst.a[0], 1)
}

def lambda_over_unresolved_dim(a : int[UNRESOLVED_DIM]) : int {
    let fn <- @(x : int[UNRESOLVED_DIM]) : int {
        return x[1] + length(x)
    }
    return invoke(fn, a)
}

[test(description = "a lambda whose argument is a fixed array with a dimension unresolved on the first infer pass compiles and indexes it")]
def test_lambda_with_unresolved_dim_argument(t : T?) {
    let x = fixed_array(10, 20, 30)
    t |> equal(lambda_over_unresolved_dim(x), 23)
}

def iaac_join(a : array<auto(numt)>) : numt {
    var tf : numt = numt(0)
    for (x in a) {
        tf += x
    }
    return tf
}

[unsafe_operation]
def iaac_findObject(objects : table<string; auto(TVal)>; guid : string) : TVal? {
    var foundObj : TVal?
    get(objects, guid) $(obj) {
        foundObj = unsafe(addr<TVal?>(obj))
    }
    return foundObj
}

[test(description = "an auto(T) alias bound from a container element works as a local's type, a constructor and a pointer result type")]
def test_infer_alias_and_alias_ctor(t : T?) {
    t |> run("generic join with auto alias") @(t : T?) {
        let a <- [1.0, 2.0, 3.0]
        t |> equal(iaac_join(a), 6.0)
    }
    t |> run("generic findObject with auto alias") @(t : T?) {
        let tab <- {"one" => 1, "two" => 2}
        unsafe {
            t |> equal(iaac_findObject(tab, "one") ?? 0, 1)
            t |> equal(iaac_findObject(tab, "two") ?? 0, 2)
        }
    }
}

def iaa_fold(a : array<auto(numt)>; var from : numt) : numt {
    for (x in a) {
        from += x
    }
    return from
}

[test(description = "an auto(T) alias bound by one argument types a later argument and the result")]
def test_infer_alias_argument(t : T?) {
    t |> run("fold with auto alias argument") @(t : T?) {
        let a <- [1.0, 2.0, 3.0]
        t |> equal(iaa_fold(a, 13.0), 19.0)
    }
}

def irc_logT(x : auto(numT)&) : string => typeinfo typename(type<numT>)

def irc_logTNoRef(x : auto(numT) -&) : string => typeinfo typename(type<numT>)

def irc_logTNoRefNoConst(x : auto(numT) -const -&) : string => typeinfo typename(type<numT>)

def irc_aliasLogT(x : auto(numT)&; y : numT) : string => typeinfo typename(type<numT>)

def irc_aliasLogTNoRef(x : auto(numT); y : numT -&) : string {
    let _z : auto(passT) = y
    return typeinfo typename(type<passT>)
}

def irc_aliasLogTNoRefNoConst(x : auto(numT); y : numT -const -&) : string {
    var _z : auto(passT) = y
    return typeinfo typename(type<passT>)
}

[test(description = "the -& and -const contracts strip reference and constness from an inferred auto(T), on the argument and through an alias")]
def test_infer_remove_ref_const(t : T?) {
    t |> run("auto ref inference") @(t : T?) {
        let x : auto const = 1
        t |> equal(irc_logT(x), "int const&")
        t |> equal(irc_logTNoRef(x), "int const")
        t |> equal(irc_logTNoRefNoConst(x), "int")
    }
    t |> run("alias ref inference") @(t : T?) {
        let x : auto const = 1
        t |> equal(irc_aliasLogT(x, x), "int const&")
        t |> equal(irc_aliasLogTNoRef(x, x), "int const")
        t |> equal(irc_aliasLogTNoRefNoConst(x, x), "int")
    }
}

def armr_abs(i : int) : int => abs(i)

def armr_foo(var bar : array<int>) : array<int> {
    var b & = unsafe(bar)
    b |> reserve(length(b) + 10)
    for (x in range(10)) {
        push(b, x)
    }
    return <- b
}

[test(description = "a local reference to an array argument grows the caller's array, and returning it by move empties the caller's variable")]
def test_auto_ref_and_move_ret(t : T?) {
    t |> run("auto ref and move return") @(t : T?) {
        var a <- [for (x in range(10)); 10 - x]
        var tf <- armr_foo(a)
        t |> equal(length(a), 0)
        t |> equal(length(tf), 20)
        for (x, y in range(-10, 10), tf) {
            t |> equal(armr_abs(x), y)
        }
    }
}

struct AiFoo {
    bar : int
}

let intnum = 5
let floatnum = 5.5
let stringval = "hello, world"

let some_a = "test"
let some_b <- [some_a]

def ai_foo(b : block<(a : int) : int>) => invoke(b, 3)

def foobar(b : block<(a : int; b : float) : string>) => invoke(b, 1, 2.0) + "bar"

def madd(a, b, c) => a * b + c

def take_arr(a : int[]) {
    var summ = 0
    for (x in a) {
        summ += x
    }
    return summ
}

def random_pick(source : string[]; var seed : int4&) : string => "pick_1"

def random_pick(source : tuple<string; float>[]; var seed : int4&) : string => "pick_2"

[sideeffects]
def qq(a : auto(T1); b : block<(a1 : T1; a2 : T2) : bool>; c : auto(T2)) {
    pass
}

def fn3 => 3
def fn2 => fn3() + 1
def fn1 => fn2() + 2

[test(description = "auto infers globals, fixed arrays, dims, pointers, containers, block argument and return types, function results, generics, tuples and dim-typed overloads")]
def test_auto_infer(t : T?) {
    t |> run("global infer") @(t : T?) {
        t |> equal(length(some_b), 1)
    }
    t |> run("fixed array copy and const") @(t : T?) {
        let a : int[10]
        let b = a
        let _c : auto const = b
        t |> equal(length(a), 10)
    }
    t |> run("array size deduction") @(t : T?) {
        t |> equal(take_arr(fixed_array<int>(1, 2, 3, 4)), 10)
    }
    t |> run("pointer auto-infer") @(t : T?) {
        var pFoo : AiFoo? = new AiFoo
        var pFooI : auto? = pFoo
        pFooI.bar = 1
        pFoo = pFooI
        t |> equal(pFoo.bar, 1)
    }
    t |> run("container auto-infer") @(t : T?) {
        var TT_A : array<int>
        let _TT_iA : array<auto> <- TT_A
        var TT_T : table<string; int>
        let _TT_iT : table<auto; auto> <- TT_T
        let _TT_ikT : table<auto; int> <- TT_T
        let _TT_ivT : table<string; auto> <- TT_T
    }
    t |> run("block return type infer") @(t : T?) {
        let f1 = ai_foo() $(arg : int) {
            return arg + 1
        }
        t |> equal(f1, 4)
    }
    t |> run("block argument default infer") @(t : T?) {
        let f2 = ai_foo() $(arg = 5) {
            return arg + 2
        }
        t |> equal(f2, 5)
    }
    t |> run("block full forward infer") @(t : T?) {
        let f3 = ai_foo() $(arg) {
            return arg + 3
        }
        t |> equal(f3, 6)
    }
    t |> run("block two arguments infer") @(t : T?) {
        let blah = foobar() $(ia, fb) {
            t |> equal(ia, 1)
            t |> equal(fb, 2.0)
            return "foo"
        }
        t |> equal(blah, "foobar")
    }
    t |> run("function return type infer") @(t : T?) {
        t |> equal(fn1(), 6)
    }
    t |> run("generic type deduction") @(t : T?) {
        t |> equal(madd(1, 2, 3), 5)
        t |> equal(madd(1.0, 2.0, 3.0), 5.0)
    }
    t |> run("tuple auto-infer") @(t : T?) {
        let ZZT = ("two", 2)                // nolint:LINT002 - read by typeinfo below
        t |> equal(typeinfo is_dim(ZZT), false)
        let ZZA = fixed_array(("two", 2))   // nolint:LINT002 - read by typeinfo below
        t |> equal(typeinfo is_dim(ZZA), true)
        t |> equal(typeinfo dim(ZZA), 1)
    }
    t |> run("overload by dim type") @(t : T?) {
        let s1 : string[2]
        let s2 : tuple<string; float>[3]
        var seed : int4
        t |> equal(random_pick(s1, seed), "pick_1")
        t |> equal(random_pick(s2, seed), "pick_2")
    }
    t |> run("block generic with alias") @(t : T?) {
        qq(1, $[unused_argument(XX, YY)] (XX, YY) => true, 2.0)
    }
}

// fuzzer-reached shapes: a regression here is a compiler crash or a
// spurious error, not a wrong value. A template structure's typedef
// holds a block in the dim of its own alias; template type
// expressions are never inferred, so the block reaches every
// post-infer visitor untyped and outside any function.
struct template r {
    def v => 0
    typedef r = r<iterator<a>[ @ {0 = (0, C())}]>
}

// a computed goto inside a captured block: its label lives in the
// enclosing function, which the block's own label table does not
// carry
def label_target : int => 6

[export]
def computed_goto_in_a_captured_block {
    invoke($ {
        goto label_target()
    })
    label 6:
    pass
}

// a type macro allocated with new, in the dim of an option result
// type that also holds an auto: the macro type carries no alias name,
// so an alias lookup must not match the whole result type
def new_of_type_macro_in_option_result : auto(v) | int[(new<$T>())] => 1

// a typedef over an option type sets an alias on the option, which
// the TypeDecl serializer must write
typedef option_with_an_alias = int | float

// a result type whose nested element is larger than INT_MAX: the
// debug-info walk must not ask it for a size; AOT skips it, as MSVC
// refuses any object past 0x7fffffff bytes
[export, no_aot]
def result_larger_than_int_max => new array<int[2000000000]>()

[test(description = "fuzzer-found shapes - a block in a template typedef's dim, a computed goto in a captured block, a type macro in an option result's dim, an aliased option typedef, a result type over INT_MAX - compile and run")]
def test_type_inference_fuzz(t : T?) {
    t |> equal(new_of_type_macro_in_option_result(), 1)
}
