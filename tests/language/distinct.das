options gen2
require dastest/testing_boost public
require UnitTest
require _distinct_fixture

typedef distinct Id = int
typedef distinct OtherId = int
typedef distinct Name = string
typedef distinct Handle = void?
typedef distinct Pos = float3
typedef distinct Meters = float

def operator +(a, b : Meters) : Meters => Meters(*a + *b)

def describe(x : int) : string => "int {x}"

def describe(x : Id) : string => "id {*x}"

def generic_keep(x : auto(TT)) : TT => x

struct Player {
    id : Id
    hp : int
}

variant IdOrName {
    ident : Id
    title : Name
}

[test(description = "a distinct type constructs from its underlying value, reads and writes through deref, and defaults to zero over int, string, pointer and vector underlyings")]
def test_distinct_construct_and_deref(t : T?) {
    t |> run("construct, read, write") @(t : T?) {
        var a = Id(5)
        t |> equal(*a, 5)
        *a = 7
        t |> equal(*a, 7)
    }
    t |> run("let handle derefs const") @(t : T?) {
        let b = Id(3)
        t |> equal(*b, 3)
    }
    t |> run("default is zero") @(t : T?) {
        let z : Id
        t |> success(z == Id(0))
        t |> success(default<Id> == Id(0))
    }
    t |> run("default over string, pointer, vector underlyings") @(t : T?) {
        t |> success(default<Name> == Name(""))
        t |> success(default<Handle> == Handle(null))
        t |> equal((*default<Pos>).y, 0.0)
    }
}

[test(description = "== and != on a distinct type borrow the underlying comparison")]
def test_distinct_equality(t : T?) {
    t |> run("== and != borrow") @(t : T?) {
        let a = Id(7)
        let b = Id(7)
        let c = Id(8)
        t |> success(a == b)
        t |> success(a != c)
    }
}

[test(description = "a distinct type over a string, a pointer or a vector carries and returns its underlying value")]
def test_distinct_underlyings(t : T?) {
    t |> run("string underlying") @(t : T?) {
        let n = Name("boris")
        t |> equal(*n, "boris")
        t |> success(n == Name("boris"))
    }
    t |> run("pointer underlying") @(t : T?) {
        let h = Handle(null)
        t |> success(h == Handle(null))
    }
    t |> run("vector underlying") @(t : T?) {
        let p = Pos(float3(1.0, 2.0, 3.0))
        t |> equal((*p).y, 2.0)
    }
}

[test(description = "a user operator on a distinct type applies, overloads on the distinct and its underlying coexist, and two distincts over one underlying stay unrelated")]
def test_distinct_operators_and_overloads(t : T?) {
    t |> run("user-defined operator +") @(t : T?) {
        let m = Meters(2.0) + Meters(3.0)
        t |> equal(*m, 5.0)
    }
    t |> run("overloads on distinct vs underlying coexist") @(t : T?) {
        t |> equal(describe(5), "int 5")
        t |> equal(describe(Id(5)), "id 5")
    }
    t |> run("two distincts over the same underlying are unrelated") @(t : T?) {
        // Id and OtherId are both int underneath; describe(Id) must not
        // accept OtherId
        t |> equal(typeinfo typename(default<Id>), "Id")
        t |> equal(typeinfo typename(default<OtherId>), "OtherId")
    }
}

[test(description = "a distinct type works as an array element and as a struct, variant and tuple field")]
def test_distinct_containers(t : T?) {
    t |> run("array of distinct") @(t : T?) {
        let arr <- [Id(1), Id(2)]
        t |> equal(*arr[1], 2)
        t |> success(arr[0] == Id(1))
    }
    t |> run("struct field") @(t : T?) {
        var p = Player(id = Id(42), hp = 100)
        t |> equal(*p.id, 42)
        p.id = Id(43)
        t |> success(p.id == Id(43))
    }
    t |> run("variant field") @(t : T?) {
        var v : IdOrName
        unsafe {
            v.ident = Id(9)
        }
        t |> success(v is ident)
        t |> success((v as ident) == Id(9))
    }
    t |> run("tuple field") @(t : T?) {
        let tp = (Id(1), Name("x"))
        t |> success(tp._0 == Id(1))
    }
}

[test(description = "auto(T) binds the distinct type itself, typeinfo is_distinct reports it, and the type of its deref is the underlying")]
def test_distinct_generics(t : T?) {
    t |> run("auto(T) binds the distinct type") @(t : T?) {
        let g = generic_keep(Meters(4.0))
        t |> equal(typeinfo typename(g), "Meters const")
        t |> equal(*g, 4.0)
    }
    t |> run("typeinfo is_distinct") @(t : T?) {
        t |> success(typeinfo is_distinct(type<Id>))
        t |> success(!typeinfo is_distinct(type<int>))
    }
    t |> run("typedecl of deref reaches the underlying") @(t : T?) {
        let m = Meters(2.5)
        let under : typedecl(*m) = 1.5
        t |> equal(under, 1.5)
    }
}

[test(description = "a public distinct type declared in another module constructs and derefs here")]
def test_distinct_cross_module(t : T?) {
    t |> run("public distinct from another module") @(t : T?) {
        let id = make_pub_id(11)
        t |> equal(pub_id_value(id), 11)
        t |> success(id == PubId(11))
        t |> success(id != PubId(12))
    }
}

typedef distinct Conn = int

var g_conn_eq_calls = 0

def operator ==(a, b : Conn) : bool {
    g_conn_eq_calls++
    return *a == *b
}

[test(description = "a user == on a distinct type is called instead of the borrowed one, while != still borrows when only == is defined")]
def test_distinct_user_operator_beats_borrow(t : T?) {
    t |> run("a user-defined == on the distinct type is the one called") @(t : T?) {
        g_conn_eq_calls = 0
        let a = Conn(1)
        let b = Conn(1)
        let c = Conn(2)
        t |> success(a == b)
        t |> success(!(a == c))
        t |> equal(g_conn_eq_calls, 2)
    }
    t |> run("!= still borrows when only == is defined") @(t : T?) {
        g_conn_eq_calls = 0
        let a = Conn(1)
        let c = Conn(2)
        t |> success(a != c)
        t |> success(!(a != Conn(1)))
        t |> equal(g_conn_eq_calls, 0)
    }
}

typedef distinct Cid = int

var g_cid_ops : string

def operator +(a, b : Cid) : Cid {
    g_cid_ops += "+,"
    return Cid(*a + *b)
}
def operator -(a, b : Cid) : Cid {
    g_cid_ops += "-,"
    return Cid(*a - *b)
}
def operator *(a, b : Cid) : Cid {
    g_cid_ops += "*,"
    return Cid(*a * *b)
}
def operator /(a, b : Cid) : Cid {
    g_cid_ops += "/,"
    return Cid(*a / *b)
}
def operator %(a, b : Cid) : Cid {
    g_cid_ops += "%,"
    return Cid(*a % *b)
}
def operator -(a : Cid) : Cid {
    g_cid_ops += "neg,"
    return Cid(-*a)
}
def operator !(a : Cid) : bool {
    g_cid_ops += "!,"
    return *a == 0
}
def operator <(a, b : Cid) : bool {
    g_cid_ops += "<,"
    return *a < *b
}
def operator <=(a, b : Cid) : bool {
    g_cid_ops += "<=,"
    return *a <= *b
}
def operator >(a, b : Cid) : bool {
    g_cid_ops += ">,"
    return *a > *b
}
def operator >=(a, b : Cid) : bool {
    g_cid_ops += ">=,"
    return *a >= *b
}
def operator +=(var a : Cid&; b : Cid) {
    g_cid_ops += "+=,"
    a = Cid(*a + *b)
}
def operator -=(var a : Cid&; b : Cid) {
    g_cid_ops += "-=,"
    a = Cid(*a - *b)
}
def operator [] (a : Cid; i : int) : int {
    g_cid_ops += "[],"
    return *a * i
}
def operator ++(var a : Cid&) {
    g_cid_ops += "p++,"
    a = Cid(*a + 1)
}
def ++operator(var a : Cid &) {
    g_cid_ops += "++p,"
    a = Cid(*a + 1)
}
def operator --(var a : Cid&) {
    g_cid_ops += "p--,"
    a = Cid(*a - 1)
}
def --operator(var a : Cid &) {
    g_cid_ops += "--p,"
    a = Cid(*a - 1)
}

struct Counter {
    ops : string
}

def operator ++(var c : Counter) {
    c.ops += "p++,"
}
def ++operator(var c : Counter) {
    c.ops += "++p,"
}

[test(description = "user arithmetic, ordering, unary, index, compound-assignment and prefix and postfix increment operators on a distinct type each call their own overload")]
def test_distinct_user_operators(t : T?) {
    t |> run("arithmetic, ordering, unary, index") @(t : T?) {
        g_cid_ops = ""
        let a = Cid(6)
        let b = Cid(3)
        t |> equal(*(a + b), 9)
        t |> equal(*(a - b), 3)
        t |> equal(*(a * b), 18)
        t |> equal(*(a / b), 2)
        t |> equal(*(a % b), 0)
        t |> equal(*(-a), -6)
        t |> success(!Cid(0))
        t |> success(b < a)
        t |> success(b <= a)
        t |> success(a > b)
        t |> success(a >= b)
        t |> equal(a[2], 12)
        t |> equal(g_cid_ops, "+,-,*,/,%,neg,!,<,<=,>,>=,[],")
    }
    t |> run("compound assignment and both increment forms take the reference") @(t : T?) {
        g_cid_ops = ""
        var a = Cid(6)
        a += Cid(4)
        a -= Cid(1)
        a++
        ++a
        a--
        --a
        t |> equal(*a, 9)
        t |> equal(g_cid_ops, "+=,-=,p++,++p,p--,--p,")
    }
    t |> run("postfix and prefix increments are separate operators on a struct too") @(t : T?) {
        var c : Counter
        c++
        ++c
        t |> equal(c.ops, "p++,++p,")
    }
}

[test(description = "a C++-registered distinct type crosses the extern boundary as itself and stays nominal - constructed, compared and written through deref")]
def test_cpp_registered_distinct(t : T?) {
    t |> run("C++ extern takes and returns the distinct type") @(t : T?) {
        let id = NativeId(6)
        let nxt = native_id_next(id)
        t |> success(nxt == NativeId(7))
        t |> equal(*nxt, 7)
    }
    t |> run("nominal: NativeId is not int") @(t : T?) {
        var v = NativeId(1)
        *v = 3
        t |> equal(*v, 3)
        t |> success(NativeId(3) == v)
    }
}

typedef distinct TkMeters = int
typedef distinct TkFeet = int
typedef distinct TkLabel = string

[test(description = "a table keyed by a distinct type hashes the underlying value: two distincts over int key separate tables, and insert, erase, get, keys, pointer index and sets all reach the key")]
def test_distinct_table_keys(t : T?) {
    t |> run("two tables keyed by different distinct types over int stay apart") @(t : T?) {
        var by_meters : table<TkMeters; string>
        var by_feet : table<TkFeet; string>
        by_meters[TkMeters(3)] = "three meters"
        by_feet[TkFeet(3)] = "three feet"
        by_feet[TkFeet(10)] = "ten feet"
        t |> equal(by_meters?[TkMeters(3)] ?? "missing", "three meters")
        t |> equal(by_feet?[TkFeet(3)] ?? "missing", "three feet")
        t |> equal(by_feet?[TkFeet(10)] ?? "missing", "ten feet")
        t |> equal(by_meters?[TkMeters(10)] ?? "missing", "missing")
        t |> equal(length(by_meters), 1)
        t |> equal(length(by_feet), 2)
        t |> success(key_exists(by_feet, TkFeet(10)), "key_exists sees the feet key")
        t |> success(!key_exists(by_meters, TkMeters(10)), "the meters table never saw 10")
    }
    t |> run("erase and re-insert through the distinct key") @(t : T?) {
        var tab : table<TkMeters; int>   // nolint:STYLE031 - the index-insert path is under test
        tab[TkMeters(1)] = 10
        tab[TkMeters(2)] = 20
        t |> success(erase(tab, TkMeters(1)), "erase of a present key answers true")
        t |> success(!erase(tab, TkMeters(1)), "erase of an absent key answers false")
        t |> equal(length(tab), 1)
        tab[TkMeters(1)] = 11
        t |> equal(tab?[TkMeters(1)] ?? 0, 11)
    }
    t |> run("iteration and keys() yield the distinct key") @(t : T?) {
        var tab <- { TkMeters(4) => "four", TkMeters(6) => "six" }
        var total = 0
        for (k, v in keys(tab), values(tab)) {
            total += *k
            t |> success(v == "four" || v == "six", "the value rides with its key")
        }
        t |> equal(total, 10)
        var keys_only_total = 0
        for (k in keys(tab)) {
            keys_only_total += *k
        }
        t |> equal(keys_only_total, 10)
    }
    t |> run("get and a pointer index reach the key through the distinct") @(t : T?) {
        var tab : table<TkMeters; string>
        tab[TkMeters(1)] = "one"
        let found = tab |> get(TkMeters(1)) $(v) {
            t |> equal(v, "one")
        }
        t |> success(found, "get visits the present key")
        t |> success(!(tab |> get(TkMeters(2)) $(v) {}), "get skips the absent key")
        var ptab = unsafe(addr(tab))
        t |> equal(ptab?[TkMeters(1)] ?? "missing", "one")
        t |> equal(ptab?[TkMeters(2)] ?? "missing", "missing")
    }
    t |> run("a distinct over string keys a table too") @(t : T?) {
        let tab <- { TkLabel("alpha") => 1, TkLabel("beta") => 2 }
        t |> equal(tab?[TkLabel("beta")] ?? 0, 2)
        t |> equal(tab?[TkLabel("gamma")] ?? 0, 0)
        t |> success(key_exists(tab, TkLabel("alpha")), "the string underneath hashes the key")
    }
    t |> run("a set keyed by a distinct type") @(t : T?) {
        var seen : table<TkFeet>   // nolint:STYLE031 - a literal with a repeated key cannot compile
        seen |> insert(TkFeet(7))
        seen |> insert(TkFeet(7))
        seen |> insert(TkFeet(8))
        t |> equal(length(seen), 2)
        t |> success(key_exists(seen, TkFeet(8)), "set membership through the distinct key")
    }
}
