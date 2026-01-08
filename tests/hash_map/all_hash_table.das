options gen2
require dastest/testing_boost public

require daslib/flat_hash_table public

options no_aliasing = false

struct Foo {
    a : int
}

[sideeffects]
def take_any_hash_table(a : $FlatHashTable < auto(TT); auto(QQ) >) {
    return "any"
}

[sideeffects]
def take_any_hash_map(a : $FlatHashTable < string; auto(QQ) >) {
    return "string;any"
}

[sideeffects]
def take_any_hash_map(a) {
    return "nothing"
}

def take_dict_map(a : $FlatHashTableEx < auto(TT); auto(TT) > (@@hash_string)) {
    return "dict_map"
}

typedef FlatHashMap_int_Foo = $FlatHashTable < int; Foo >
typedef FlatHashMap_string_int = $FlatHashTable < string; int >
typedef DictMap = $FlatHashTableEx < string; int > (@@hash_string)   // vs @@hash

def hash_string(s : string) : uint64 {
    var h : uint64 = 14695981039346656037ul
    for (c in s) {
        h = h ^ uint64(c)
        h = h * 1099511628211ul
    }
    return h
}

[test]
def specialization(t : T?) {
    var map1 <- FlatHashMap_int_Foo()
    var map2 <- FlatHashMap_string_int()
    var map3 : table<string; int>
    var map4 <- DictMap()
    t |> equal("any", take_any_hash_table(map1))
    t |> equal("string;any", take_any_hash_map(map2))
    t |> equal("nothing", take_any_hash_map(map3))
    t |> equal("dict_map", take_dict_map(map4))
}

def table_ops(t : T?; var a : auto(AnyTable_int_to_Foo); var e : Foo; var et : Foo implicit) {
    for (i in range(10)) {
        a[i] = Foo(a = i)
    }
    t |> equal(10, a.length())
    a.clear()
    t |> equal(0, a.length())
    t |> success(a.empty())
    a[0] = e
    a[1] = e
    t |> equal(1, a[0].a)
    t |> equal(1, a[1].a)
    t |> equal(2, a.length())
    a.clear()
    a[0] = e
    a[1] = e
    var b := a
    a := b
    t |> equal(1, b[0].a)
    t |> equal(1, b[1].a)
    t |> equal(2, b.length())
    a.erase(0)
    a.erase(1)
    t |> equal(0, a.length())
    a := b
    t |> equal(1, a[0].a)
    t |> equal(1, a[1].a)
    t |> equal(2, a.length())
    for (k in a.keys()) {
        t |> equal(1, a[k].a)
    }
    for (k in a.keys()) {
        t |> success(k == 0 || k == 1)
    }
    for (v in a.values()) {
        t |> equal(1, v.a)
    }
    var found = a.get(0) <| $(v) {
        t |> equal(1, v.a)
    }
    t |> success(found)
    found = a.get(1) <| $(v) {
        t |> equal(1, v.a)
    }
    t |> success(found)
    found = a.get(2) <| $(v) {
        t |> failure("we should not be here\n")
    }
    t |> success(!found)
}

[test]
def all_ops(t : T?) {
    t |> run("FlatHashMap") <| @(t : T?) {
        var tab <- FlatHashMap_int_Foo()
        t |> success(tab.empty())
        t |> equal(0, tab.length())
        table_ops(t, tab, Foo(a = 1), Foo(a = 2))
    }
}



