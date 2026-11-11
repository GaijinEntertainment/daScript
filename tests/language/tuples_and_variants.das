options gen2
require UnitTest
require dastest/testing_boost public
require daslib/match
require daslib/apply

// a variant case written without a type is an index-only case: it has
// an index and no value

variant Tags {
    nada
    nothing
    zilch
    something
    anything : int
}

typedef Anon = variant<nada; nothing; zilch; something; anything : int>

variant TailTag {
    value : int
    done
}

variant Owner {
    none
    items : array<int>
}

struct Holder {
    tag : Tags
    value : int
}

variant Light {
    red
    green
}

struct Signal {
    light : Light
    count : int
}

def apply_visits(v : Tags) : string {
    var seen = ""
    apply(v) $(name, value) {
        seen += "{name}={value};"
    }
    return seen
}

def apply_visits_with_return(v : Tags) : string {
    var seen = ""
    apply(v) $(name, value) {
        seen += "{name}={value};"
        return
    }
    return seen
}

def Tags(doubled : int) : Tags => Tags(anything = doubled * 2)

def case_label(v : Tags) : string {
    return match (v) {
        Tags(nada) => "nada"
        Tags(nothing) => "nothing"
        Tags(zilch) => "zilch"
        Tags(something) => "something"
        Tags(anything = x) => "anything {x}"
    }
}

[test(description = "an all-bare variant is one int, a bare case adds nothing to size or alignment, and the alias and anonymous spellings are one type")]
def test_bare_case_layout(t : T?) {
    t |> run("all-bare variant is one int") @(t : T?) {
        t |> equal(typeinfo sizeof(type<variant<nada; nothing; zilch>>), 4)
        t |> equal(typeinfo alignof(type<variant<nada; nothing; zilch>>), 4)
    }
    t |> run("bare case adds nothing to size or alignment") @(t : T?) {
        t |> equal(typeinfo sizeof(type<Tags>), 8)
        t |> equal(typeinfo alignof(type<Tags>), 4)
    }
    t |> run("all-bare variant held by value") @(t : T?) {
        var s = Signal(light = Light(green), count = 1)
        t |> equal(typeinfo sizeof(s), 8)
        t |> success(s.light is green)
        var lights <- [Light(red), Light(green), Light(red)]
        var greens = 0
        for (l in lights) {
            greens += l is green ? 1 : 0
        }
        t |> equal(greens, 1)
        s.light = lights[1]
        t |> success(s.light is green)
    }
    t |> run("alias and anonymous spelling are one type") @(t : T?) {
        let v : Tags
        let w : Anon = v
        t |> success(w is nada)
    }
}

[test(description = "zeroed memory is case 0 and each bare case holds its own index, also after a payload case")]
def test_bare_case_indices(t : T?) {
    t |> run("zero memory is case 0") @(t : T?) {
        let v : Tags
        t |> equal(variant_index(v), 0)
        t |> success(v is nada)
        t |> success(!(v is nothing))
        let h : Holder
        t |> success(h.tag is nada)
        t |> success(default<Tags> is nada)
    }
    t |> run("each bare case is its own index") @(t : T?) {
        t |> equal(typeinfo variant_index<nada>(type<Tags>), 0)
        t |> equal(typeinfo variant_index<nothing>(type<Tags>), 1)
        t |> equal(typeinfo variant_index<zilch>(type<Tags>), 2)
        t |> equal(typeinfo variant_index<something>(type<Tags>), 3)
        t |> equal(typeinfo variant_index<anything>(type<Tags>), 4)
        t |> equal(typeinfo safe_variant_index < nope > (type<Tags>), -1)
    }
    t |> run("a bare case after a payload case") @(t : T?) {
        var v : TailTag
        t |> success(v is value)
        v = TailTag(done)
        t |> success(v is done)
        t |> equal(variant_index(v), 1)
    }
}

[test(description = "a bare case constructs by name and switches by assignment, safe access on it is null, and copy, clone and arrays keep the case")]
def test_bare_case_construct(t : T?) {
    t |> run("construct by name, switch by assignment") @(t : T?) {
        var v = Tags(nothing)
        t |> success(v is nothing)
        t |> equal(variant_index(v), 1)
        v = Tags(zilch)
        t |> success(v is zilch)
        v = Tags(anything = 7)
        t |> success(v is anything)
        t |> equal(v as anything, 7)
        v = Tags(something)
        t |> success(v is something)
        t |> success(!(v is anything))
    }
    t |> run("safe access on a bare case is null") @(t : T?) {
        var v = Tags(zilch)
        t |> equal(v ?as anything ?? -1, -1)
        v = Tags(anything = 3)
        t |> equal(v ?as anything ?? -1, 3)
    }
    t |> run("copy and clone keep the case") @(t : T?) {
        let v = Tags(something)
        let w = v
        t |> success(w is something)
        let c := v
        t |> success(c is something)
        let none_owner : Owner
        let none_clone := none_owner
        t |> success(none_clone is none)
        let items_owner = Owner(items <- [1, 2, 3])
        let items_clone := items_owner
        t |> success(items_clone is items)
        t |> equal(length(items_clone as items), 3)
    }
    t |> run("array of mixed cases") @(t : T?) {
        let arr <- [Tags(nada), Tags(anything = 1), Tags(zilch), Tags(anything = 2)]
        var bare = 0
        var sum = 0
        for (v in arr) {
            if (v is anything) {
                sum += v as anything
            } else {
                bare++
            }
        }
        t |> equal(bare, 2)
        t |> equal(sum, 3)
    }
}

[test(description = "match covers bare cases without a wildcard and print spells a bare case by its name")]
def test_bare_case_match_and_print(t : T?) {
    t |> run("match covers bare cases without a wildcard") @(t : T?) {
        t |> equal(case_label(Tags(nada)), "nada")
        t |> equal(case_label(Tags(nothing)), "nothing")
        t |> equal(case_label(Tags(zilch)), "zilch")
        t |> equal(case_label(Tags(something)), "something")
        t |> equal(case_label(Tags(anything = 42)), "anything 42")
    }
    t |> run("print spells the case name") @(t : T?) {
        t |> equal("{Tags(zilch)}", "zilch")
        t |> equal("{Tags(anything = 5)}", "5")
    }
}

[test(description = "apply visits no value for a bare case, on both the inline and the escaping-return path")]
def test_bare_case_apply(t : T?) {
    t |> run("apply visits nothing for a bare case, inline path") @(t : T?) {
        t |> equal(apply_visits(Tags(zilch)), "")
        t |> equal(apply_visits(Tags(anything = 4)), "anything=4;")
    }
    t |> run("apply visits nothing for a bare case, escaping-return path") @(t : T?) {
        t |> equal(apply_visits_with_return(Tags(nothing)), "")
        t |> equal(apply_visits_with_return(Tags(anything = 6)), "anything=6;")
    }
}

[test(description = "a function may share a bare-case variant's name, type names spell a bare case bare, and JSON round-trips a bare case as null")]
def test_bare_case_names(t : T?) {
    t |> run("a function may share the typedef's name") @(t : T?) {
        let v = Tags(21)
        t |> success(v is anything)
        t |> equal(v as anything, 42)
        t |> success(Tags(nada) is nada)
        let anything = 5
        t |> equal(Tags(anything) as anything, 10)
    }
    t |> run("type names spell a bare case bare") @(t : T?) {
        t |> equal(typeinfo typename(type<Tags>),
            "variant<nada;nothing;zilch;something;anything:int>")
        t |> equal(sprint(Tags(nothing), print_flags.namesAndDimensions),
            "variant<nada; nothing; zilch; something; anything:int> = variant(nothing)")
    }
    t |> run("JSON writes null for a bare case and reads it back") @(t : T?) {
        t |> equal(sprint_json(Tags(nada), false), "\{\"nada\":null\}")
        t |> equal(sprint_json(Tags(anything = 3), false), "\{\"anything\":3\}")
        var scanned : Tags
        t |> success(sscan_json("\{\"nothing\":null\}", scanned))
        t |> success(scanned is nothing)
        t |> success(sscan_json("\{\"anything\":4\}", scanned))
        t |> equal(scanned ?as anything ?? -1, 4)
    }
}

struct VrFoo {
    t   : variant<i_value : uint; f_value : float>?
}

typedef IorA = variant< i : int; a : array<int>; f : float; t : table<int; int> >

variant U_F {             // syntax sugar for typedef U_F = variant<...>
    i_value : uint
    f_value : float
}

[sideeffects]
def test_ifs(t : T?) {
    let i = makeSampleI()
    t |> success(i is i_value)
    t |> success(i as i_value == 1)
    let f = makeSampleF()
    t |> success(f is f_value)
    t |> success(f as f_value == 2.0)
    let s = makeSampleS()
    t |> success(s is s_value)
    t |> equal(s as s_value, "3")
}

variant FooIS {
    i : int
    s : string
}

[sideeffects]
def test_short_notation(t : T?) {
    let e = FooIS(i = 1)
    t |> success(e is i)
    t |> success(e as i == 1)
    let ee = fixed_array<FooIS>(1, "2")
    t |> success((ee[0] is i) && (ee[0] as i == 1))
    t |> success((ee[1] is s) && (ee[1] as s == "2"))
}

[test(description = "a variant overlays its cases in one slot: index queries, is, as, ?as, unsafe index switches, a panicking as on the wrong case, clone and delete, C++ variants and short-notation construction")]
def test_variant(t : T?) {  // nolint:STYLE038 - flat list of independent test arms

    t |> equal(typeinfo sizeof(type<U_F>), 8)

    var u : U_F
    t |> equal(typeinfo sizeof(u), 8)
    unsafe {
        u.f_value = 1.0                                                 // unsafe operation
        t |> equal(u.i_value, 0x3f800000)                               // unsafe operation
    }
    t |> equal(variant_index(u), 0)

    t |> equal(typeinfo variant_index<i_value>(u), 0)
    t |> equal(typeinfo variant_index<f_value>(u), 1)

    t |> equal(typeinfo safe_variant_index < i_value > (u), 0)
    t |> equal(typeinfo safe_variant_index < f_value > (u), 1)
    t |> equal(typeinfo safe_variant_index < fg > (u), -1)
    t |> equal(typeinfo safe_variant_index < g > (type<int>), -1)

    t |> success(u is i_value)
    t |> equal(u as i_value, 0x3f800000)
    unsafe(set_variant_index(u, typeinfo variant_index<f_value>(u)))        // unsafe operation
    t |> equal(u as f_value, 1.0)

    t |> equal(u ?as f_value ?? 2.0, 1.0)
    t |> equal(u ?as i_value ?? 1u, 1u)

    u as f_value = 2.0
    unsafe(set_variant_index(u, typeinfo variant_index<i_value>(u)))        // unsafe operation
    t |> equal(u as i_value, 0x40000000)

    u as i_value = 0x3f800000
    unsafe(set_variant_index(u, typeinfo variant_index<f_value>(u)))        // unsafe operation
    t |> equal(u as f_value, 1.0)

    var paniced = false
    try {
        // will assert, because u is f_value
        u as i_value = 0x40000000
    } recover {
        paniced = true
    }
    t |> success(paniced)

    // r2v `as` read (value consumed directly, e.g. in arithmetic) must
    // check the index too
    var paniced_r2v = false
    try {
        // will assert, because u is f_value
        let y = (u as i_value) + 1u
        feint("{y}")
    } recover {
        paniced_r2v = true
    }
    t |> success(paniced_r2v)
    t |> equal((u as f_value) + 1.0, 2.0)

    t |> equal(unsafe(u.f_value), 1.0)
    u = U_F(i_value = 0x40000000)
    t |> equal(u as i_value, 0x40000000)
    u = U_F(f_value = 1.0)

    let t2 = [U_F(i_value=1u), U_F(f_value=2.0)]
    t |> equal(length(t2), 2)
    t |> success(t2[0] is i_value && t2[0] as i_value == 1u)
    t |> success(t2[1] is f_value && t2[1] as f_value == 2.0)

    var x : IorA
    unsafe {
        set_variant_index(x, 1)
        x.a <- [for (tt in range(10)); tt]
    }
    let y := x
    t |> success(y is a)
    for (tt in range(10)) {
        t |> equal(y as a[tt], tt)
    }
    delete x
    t |> success(x is i)
    t |> success(y is a)
    for (tt in range(10)) {
        t |> equal(y as a[tt], tt)
    }

    test_ifs(t)

    test_short_notation(t)
}

struct WithArr {
    a : array<int>
}

variant BigVariant {
    big : int4[4]
    w : WithArr
}

def filled_big() : BigVariant => (BigVariant(
    big = fixed_array(int4(-1), int4(-1), int4(-1), int4(-1))))

[test(description = "cloning a heap-owning case into a variant whose bytes another case wrote switches the case and deep-copies the array")]
def test_clone_into_a_case_whose_bytes_another_case_wrote(t : T?) {
    var src : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    var dst : BigVariant <- filled_big()
    dst := src
    (src as w).a[0] = 100
    t |> success(dst is w)
    t |> equal(length((dst as w).a), 3)
    t |> equal((dst as w).a[0], 1)
}

[test(description = "cloning a value case over a heap-owning case switches the variant back to the value case with its bytes")]
def test_clone_back_into_a_value_case(t : T?) {
    let src = filled_big()
    var dst : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    dst := src
    t |> success(dst is big)
    t |> equal((dst as big)[3], int4(-1))
}

[test(description = "cloning into a variant already on the same heap-owning case replaces its array with the source's")]
def test_clone_into_the_same_case(t : T?) {
    var src : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    var dst : BigVariant <- BigVariant(w = WithArr(a <- [7]))
    dst := src
    t |> success(dst is w)
    t |> equal(length((dst as w).a), 3)
    t |> equal((dst as w).a[2], 3)
}

[test(description = "cloning a heap-owning variant into itself keeps its case and its array")]
def test_clone_into_itself(t : T?) {
    var v : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    v := v
    t |> success(v is w)
    t |> equal(length((v as w).a), 3)
    t |> equal((v as w).a[2], 3)
}

struct VsA {
    a : int
}

struct VsBB {
    b : int
}

variant VsFoo {
    a : VsA
    b : VsBB
}

let vs_foo <- fixed_array<VsFoo>(
    VsFoo(a = VsA(a = 1)),
    VsFoo(b = VsBB(b = 2))
);

[test(description = "a global fixed array of variants with struct cases keeps each element's case and payload")]
def test_multiple_variants(t : T?) {
    t |> success(vs_foo[0] is a)
    if (vs_foo[0] is a) {
        t |> equal(1, (vs_foo[0] as a).a)
    }
    t |> success(vs_foo[1] is b)
    if (vs_foo[1] is b) {
        t |> equal(2, (vs_foo[1] as b).b)
    }
}

// tuple creation, named fields, positional access, sizeof, heap
// allocation, make_tuple syntax, fixed_array of tuples, and
// finalize/clone
def testI(arg : tuple<i : int; f : float>) => arg.i == 1

def testF(arg : tuple<i : int; f : float>) => arg.f == 3.14

def overload_pick(_x : tuple<int; float>) => 1

def overload_pick(_x : tuple<x : int; y : float>) => 2

// return-position shorthand: a positional bare-var tuple promotes to
// the declared named-tuple result type
def make_hit_return(eid : int; distSq : float) : tuple<eid : int; distSq : float> => (eid, distSq)

def make_hit_locals() : tuple<eid : int; distSq : float> {
    let eid = 17
    let distSq = 4.25
    return (eid, distSq)
}

[test(description = "tuples build, name and index their fields, live on the heap and in fixed arrays, promote bare-variable literals to a named target only as a fallback, and clone deep while delete zeroes")]
def test_tuple(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("basic creation and named fields") @(t : T?) {
        let ttt = (i = 1, f = 3.14)
        t |> equal(testI(ttt), true)
        t |> equal(testF(ttt), true)
        t |> equal(ttt._first, 1)
    }
    t |> run("positional access and assignment") @(t : T?) {
        var tp : tuple<int; float; string>
        // sizeof is either 16 (64-bit) or 12 (32-bit)
        let sz = typeinfo sizeof(tp)
        t |> equal(sz == 16 || sz == 12, true)
        tp._0 = 1
        tp._1 = 2.0
        tp._2 = "3"
        let q = tp
        t |> equal(q._0, 1)
        t |> equal(q._1, 2.0)
        t |> equal(q._2, "3")
    }
    t |> run("pointer access") @(t : T?) {
        var tp : tuple<int; float; string>
        tp._0 = 1
        tp._1 = 2.0
        tp._2 = "3"
        var pq = unsafe(addr(tp))
        t |> equal(pq._0, 1)
        t |> equal(pq._1, 2.0)
        t |> equal(pq._2, "3")
        t |> equal(pq?._0 ?? 0, 1)
        t |> equal(pq?._1 ?? 0.0, 2.0)
        t |> equal(pq?._2 ?? "nothing", "3")
    }
    t |> run("heap allocated tuple") @(t : T?) {
        var qq = new <tuple<int; float>>
        qq._0 = 1
        qq._1 = 2.0
        t |> equal(qq?._0 ?? 0, 1)
        t |> equal(qq?._1 ?? 0.0, 2.0)
        unsafe {
            delete qq
        }
    }
    t |> run("make tuple syntax") @(t : T?) {
        let mkt = (1, 2.0)
        t |> equal(mkt._0, 1)
        t |> equal(mkt._1, 2.0)
    }
    t |> run("shorthand named into array of named tuples") @(t : T?) {
        var arr : array<tuple<eid : int; distSq : float>>
        let eid = 7
        let distSq = 2.5
        arr |> push((eid, distSq))
        t |> equal(length(arr), 1)
        t |> equal(arr[0].eid, 7)
        t |> equal(arr[0].distSq, 2.5)
    }
    t |> run("shorthand named single element") @(t : T?) {
        let eid = 9
        let tup : tuple<eid : int> = (eid,)
        t |> equal(tup.eid, 9)
    }
    t |> run("shorthand named via typed var init") @(t : T?) {
        let eid = 11
        let distSq = 4.5
        let tup : tuple<eid : int; distSq : float> = (eid, distSq)
        t |> equal(tup.eid, 11)
        t |> equal(tup.distSq, 4.5)
    }
    t |> run("unnamed target keeps unnamed (no shorthand)") @(t : T?) {
        var arr : array<tuple<int; float>>
        let k = 11
        let v = 4.5
        arr |> push((k, v))   // target is unnamed, no promotion
        t |> equal(arr[0]._0, 11)
        t |> equal(arr[0]._1, 4.5)
    }
    t |> run("overload resolution prefers unnamed tuple when both present") @(t : T?) {
        let x = 123
        let y = 321.
        // (x, y) is an unnamed tuple<int;float> - an exact match for the
        // unnamed overload; the shorthand-to-named promotion fires only when
        // no candidate matches
        t |> equal(overload_pick((x, y)), 1)
    }
    t |> run("explicit named tuple literal picks named overload") @(t : T?) {
        let x = 123
        let y = 321.
        t |> equal(overload_pick((x = x, y = y)), 2)
    }
    t |> run("mixed expression tuple stays unnamed") @(t : T?) {
        var arr : array<tuple<int; int>>
        let a = 1
        arr |> push((a, a + 1))   // 2nd elem not bare ExprVar -> never tagged
        t |> equal(arr[0]._0, 1)
        t |> equal(arr[0]._1, 2)
    }
    t |> run("shorthand named via return from named-tuple-typed fn") @(t : T?) {
        let r = make_hit_return(7, 2.5)
        t |> equal(r.eid, 7)
        t |> equal(r.distSq, 2.5)
    }
    t |> run("shorthand named via return using local bare vars") @(t : T?) {
        let r = make_hit_locals()
        t |> equal(r.eid, 17)
        t |> equal(r.distSq, 4.25)
    }
    t |> run("return-promoted tuple pushes into named-tuple array") @(t : T?) {
        var arr : array<tuple<eid : int; distSq : float>>
        arr |> push(make_hit_return(3, 1.5))
        t |> equal(arr[0].eid, 3)
        t |> equal(arr[0].distSq, 1.5)
    }
    t |> run("fixed array of tuples") @(t : T?) {
        let mkta = fixed_array<tuple<int; float>>((1, 2.0), (3, 4.0))
        t |> equal(mkta[0]._0, 1)
        t |> equal(mkta[0]._1, 2.0)
        t |> equal(mkta[1]._0, 3)
        t |> equal(mkta[1]._1, 4.0)
        let ti = fixed_array(1, 2, 3, 4)
        t |> equal(typeinfo sizeof(ti), 16)
        t |> equal(ti[0], 1)
        t |> equal(ti[3], 4)
        let tt = fixed_array((1, 2), (3, 4))
        t |> equal(typeinfo sizeof(tt), 16)
        t |> equal(tt[0]._0, 1)
        t |> equal(tt[1]._1, 4)
    }
    t |> run("finalize and clone") @(t : T?) {
        var x : tuple<int; float; array<int>>
        x._0 = 1
        x._1 = 2.0
        x._2 <- [for (xx in range(10)); xx]
        let y := x
        t |> equal(y._0, 1)
        t |> equal(y._1, 2.0)
        for (i in range(10)) {
            t |> equal(y._2[i], i)
        }
        delete x
        t |> equal(x._0, 0)
        t |> equal(x._1, 0.0)
        t |> equal(length(x._2), 0)
        // y should still be intact (it was cloned)
        t |> equal(y._0, 1)
        t |> equal(y._1, 2.0)
        for (i in range(10)) {
            t |> equal(y._2[i], i)
        }
    }
}

// `_` is the discard in tuple destructuring: it binds nothing, so it
// can repeat - within one pattern, across patterns, and in for-loop
// destructures
def two_of_three() : tuple<int; float; string> => (1, 2.0, "three")

[test(description = "the _ discard repeats freely within and across tuple destructures, including for-loop destructures")]
def test_tuple_destructure_discard(t : T?) {
    t |> run("discard repeats freely") @(t : T?) {
        let (_, _, third) = two_of_three()
        t |> equal("three", third)
        let (first, _, _) = two_of_three()
        t |> equal(1, first)
        let (_, mid, _) = two_of_three()
        t |> equal(2.0, mid)
    }
    t |> run("discard repeats across for-loop destructures") @(t : T?) {
        var pairs <- [(1, "one"), (2, "two")]
        var total = 0
        for ((k, _) in pairs) {
            total += k
        }
        for ((_, v) in pairs) {
            total += length(v)
        }
        t |> equal(9, total)
    }
}

def te_foo => (1, "2", 3.0)

[test(description = "let (a, b, c) = f() expands a returned tuple into typed locals that keep the tuple's layout")]
def test_tuple_expansion(t : T?) {
    let (i, s, f) = te_foo()
    t |> equal(i, 1)
    t |> equal(s, "2")
    t |> equal(f, 3.0)
    t |> equal(typeinfo sizeof(type<tuple<int; string; float>>), typeinfo sizeof(i`s`f))
}

// a tuple whose inner pointer is non-const passes to a parameter
// whose inner pointer is const: isSameType propagates isPassType into
// tuple argTypes (src/ast/ast_typedecl.cpp)
struct Loc {
    a : int
}

struct Node {
    at : Loc
}

typedef NodePtr = Node?

var ats : array<tuple<string, Loc const?>>

var generic_hits : int = 0
var exact_hits : int = 0

def take(a : array<auto(TT)>; b : TT) {
    generic_hits ++
}

def takeng(_a : array<tuple<string, Loc const?>>; _b : tuple<string, Loc const?>) {
    exact_hits ++
}

def feed(var a : NodePtr&) {
    take(ats, ("test", unsafe(addr(a.at))))
    takeng(ats, ("test", unsafe(addr(a.at))))
}

[test(description = "a tuple with a non-const inner pointer is accepted by a generic and by an exact parameter whose inner pointer is const")]
def test_permissive_tuple_const(t : T?) {
    t |> run("tuple with const-pointer field accepts non-const-pointer argument") @(t : T?) {
        generic_hits = 0
        exact_hits = 0
        var e : Node
        e.at.a = 42
        var ep : NodePtr = unsafe(addr(e))
        feed(ep)
        t |> equal(generic_hits, 1)
        t |> equal(exact_hits, 1)
    }
}

struct RelocatedSpec {
    slot : int
    name : string
}

def relocated_specs(n : int) {
    var specs : array<RelocatedSpec>
    if (n == 0) return <- (<- specs, false)
    specs |> push(RelocatedSpec(slot = n, name = "x"))
    return <- (<- specs, true)
}

def relocated_destructure(base : int; at : int) : int {
    if (base < 0) return -1
    var inscope (xs, ok) <- relocated_specs(base)
    return ok ? length(xs) + base + at : 0
}

[test(description = "an inscope destructure after an early return compiles in a function with an argument named at")]
def test_destructure_after_early_return(t : T?) {
    t |> equal(relocated_destructure(10, 100), 111)
    t |> equal(relocated_destructure(-1, 100), -1)
    t |> equal(relocated_destructure(0, 100), 0)
}
