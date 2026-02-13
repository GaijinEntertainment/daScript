require daslib/flat_hash_table
require math

typedef FlatHashMap_int_Foo = TFlatHashTable < int; FooType >
typedef FlatHashMap_string_int = TFlatHashTable < string; int >

struct Foo {
    a : int
}

typedef FooType = Foo

[sideeffects]
def take_any_hash_table(a : TFlatHashTable < auto(TT); auto(QQ) >) {
    return "any"
}

[sideeffects]
def take_any_hash_map(a : TFlatHashTable < string; auto(QQ) >) {
    return "string;any"
}

typedef DictMap = TFlatHashTable < string; int > (@@hash_string)   // vs @@hash

def hash_string(s : string) : uint64 {
    var h : uint64 = 14695981039346656037ul
    for (c in s) {
        h = h ^ uint64(c)
        h = h * 1099511628211ul
    }
    return h
}

def makeRandomSequence(var src : array<string>) {
    let n = 500000
    let mod = uint(n)
    resize(src, n)
    for (i in range(n)) {
        let num = (271828183u ^ uint(i * 119)) % mod
        src[i] = "{num}"
    }
}

def dict(var tab : auto; src : array<string>) {
    tab.clear()
    var maxOcc = 0
    for (s in src) {
        maxOcc = max(++tab[s], maxOcc)
    }
    return maxOcc
}

[export]
def main {

    if (true) {
        var map1 <- FlatHashMap_int_Foo()
        var map2 <- FlatHashMap_string_int()
        verify("any" == take_any_hash_table(map1))
        verify("string;any" == take_any_hash_map(map2))
        print("hash table specialization tests passed\n")
    }

    var src : array<string>
    makeRandomSequence(src)
    if (true) {
        var tab <- DictMap()
        profile(1, "flat hash map") <| $() {
            dict(tab, src)
        }
    }
}
