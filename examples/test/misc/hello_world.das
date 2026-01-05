require daslib/hash_table
require math

typedef DictMap = $FlatHashMap < string; int >

/*
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
*/

def take_any_hash_map(a : $FlatHashMap < auto(TT); auto(QQ) >) {
    print("Taking ANY TFlatHashMap<{typeinfo typename(type<TT>)};{typeinfo typename(type<QQ>)}>\n")
}

def take_any_hash_map(a : $FlatHashMap < string; auto(QQ) >) {
    print("Taking STRING TFlatHashMap<string;{typeinfo typename(type<QQ>)}>\n")
}

[export]
def main {
    var tab <- DictMap()
    take_any_hash_map(tab)

    typedef IntMap = $FlatHashMap < int; float >
    var tab1 <- IntMap()
    take_any_hash_map(tab1)
    /*
    var src : array<string>
    makeRandomSequence(src)
    if (true) {
        var tab : table<string; int>
        profile(20, "dictionary") <| $() {
            dict(tab, src)
        }
    }
    if (true) {
        var tab <- DictMap()
        profile(20, "flat hash map") <| $() {
            dict(tab, src)
        }
    }
    */
}




