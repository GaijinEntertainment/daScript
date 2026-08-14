options gen2
options persistent_heap

require dastest/testing_boost
require math
require daslib/strings_boost

let N = 100000
let M = 50000

var g_sink : int64

def private make_ints(n : int) {
    var res <- [for (i in range(n)); i]
    return <- res
}

def private make_keys(n : int; prefix : string) {
    var res <- [for (i in range(n)); "{prefix}key_{i % 97}_padding_tail"]
    return <- res
}

def private gen_ints(n : int) : iterator<int> {
    return <- generator<int> {
        for (i in range(n)) {
            yield i
        }
        return false
    }
}

def private gen_ints_while(n : int) : iterator<int> {
    return <- generator<int> {
        var i = 0
        while (i < n) {
            yield i
            i++
        }
        return false
    }
}

[benchmark]
def iteration(b : B?) {
    let arr <- make_ints(N)
    var dim : int[1024]
    for (i in range(1024)) {
        dim[i] = i
    }
    b |> run("range/{N}", N) {
        var s = 0
        for (i in range(N)) {
            s = (s << 1) ^ i
        }
        g_sink += int64(s)
    }
    b |> run("array/{N}", N) {
        var s = 0
        for (x in arr) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
    b |> run("fixed_array/1024", 1024) {
        var s = 0
        for (x in dim) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
    b |> run("each_iterator/{N}", N) {
        var s = 0
        for (x in each(arr)) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
    let small <- make_ints(8)
    b |> run("array_setup/8", 1) {
        var s = 0
        for (x in small) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
    b |> run("each_iterator_setup/8", 1) {
        var s = 0
        for (x in each(small)) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
    b |> run("generator/{N}", N) {
        var s = 0
        for (x in gen_ints(N)) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
    b |> run("generator_while/{N}", N) {
        var s = 0
        for (x in gen_ints_while(N)) {
            s = (s << 1) ^ x
        }
        g_sink += int64(s)
    }
}

def private double_it(x : int) : int => x * 2

def private rec_call(x : int) : int {
    if (x <= 0) return 0
    return x + rec_call(x - 1)
}

[benchmark]
def calls(b : B?) {
    let ia <- make_ints(M)
    var lam <- @(x : int) : int {
        return x * 2
    }
    let fp = @@double_it
    b |> run("block_invoke", 1) {
        g_sink++
    }
    b |> run("static_call/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ rec_call(ia[i] & 1)
        }
        g_sink += int64(s)
    }
    b |> run("lambda_invoke/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ invoke(lam, ia[i])
        }
        g_sink += int64(s)
    }
    b |> run("fnptr_invoke/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ invoke(fp, ia[i])
        }
        g_sink += int64(s)
    }
    unsafe {
        delete lam
    }
}

[benchmark]
def compare(b : B?) {
    let ka <- make_keys(M, "")
    let kb <- make_keys(M, "")
    let kd <- make_keys(M, "z")
    let ia <- make_ints(M)
    b |> run("int_equ/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ia[i] == i) {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
    b |> run("string_equ_match/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ka[i] == kb[i]) {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
    b |> run("string_equ_first_byte/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ka[i] == kd[i]) {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
    let kp <- [for (s in ka); s]
    b |> run("string_equ_literal/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ka[i] == "key_5_padding_tail") {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
    b |> run("string_equ_same_ptr/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ka[i] == kp[i]) {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
    b |> run("string_less/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ka[i] < kd[i]) {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
}

let K = 1000

[benchmark]
def containers(b : B?) {
    let ia <- make_ints(M)
    let ka <- make_keys(M, "")
    var ti : table<int; int>   // nolint:STYLE027 - the insert loop is setup for the probe below
    for (i in range(M)) {
        ti |> insert(i, i * 2)
    }
    var ts : table<string; int>   // nolint:STYLE027 - the insert loop is setup for the probe below
    for (i in range(M)) {
        ts |> insert(ka[i], i)
    }
    b |> run("table_int_get/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ (ti?[i] ?? 0)
        }
        g_sink += int64(s)
    }
    b |> run("table_str_get/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ (ts?[ka[i]] ?? 0)
        }
        g_sink += int64(s)
    }
    b |> run("array_push/{K}", K) {
        var a : array<int>   // nolint:STYLE027 - the push loop IS the measurement
        for (i in range(K)) {
            a |> push(i)
        }
        g_sink += long_length(a)
        delete a
    }
    b |> run("float4_dot/{M}", M) {
        var f = 0.
        for (i in range(M)) {
            let v = float4(float(ia[i]), 1., 2., 3.)
            f = f * 0.5 + dot(v, v)
        }
        g_sink += int64(f)
    }
    delete ti
    delete ts
}

class Shape {
    def abstract area(k : int) : int
}

class Circle : Shape {
    def override area(k : int) : int {
        return k * 2
    }
}

class Square : Shape {
    def override area(k : int) : int {
        return k * 3
    }
}

struct private Bag {
    xs : array<int>
    name : string
}

[benchmark]
def dispatch(b : B?) {
    let ia <- make_ints(M)
    var shapes : array<Shape?>
    for (i in range(M)) {
        if (i % 2 == 0) {
            shapes |> push(new Circle())
        } else {
            shapes |> push(new Square())
        }
    }
    b |> run("class_method/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ shapes[i]->area(ia[i] & 7)
        }
        g_sink += int64(s)
    }
    var circle = new Circle()
    b |> run("class_method_mono/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s = (s << 1) ^ circle->area(ia[i] & 7)
        }
        g_sink += int64(s)
    }
    unsafe {
        delete circle
    }
    b |> run("table_insert/{M}", M) {
        var t : table<int; int>   // nolint:STYLE027 - the insert loop IS the measurement
        for (i in range(M)) {
            t |> insert(i, i)
        }
        g_sink += long_length(t)
        delete t
    }
    b |> run("build_string/{K}", K) {
        var n = 0
        for (i in range(K)) {
            let s = build_string() $(var w) {
                w |> write("ab")
                w |> write(i)
            }
            n += length(s)
        }
        g_sink += int64(n)
    }
    var proto <- Bag(xs <- make_ints(8), name = "proto")
    b |> run("struct_clone/{K}", K) {
        var n = 0
        for (_i in range(K)) {
            var c := proto
            n += length(c.xs)
            delete c
        }
        g_sink += int64(n)
    }
    b |> run("try_recover/{M}", M) {
        var s = 0
        for (i in range(M)) {
            try {
                s = (s << 1) ^ ia[i]
            } recover {
                s = 0
            }
        }
        g_sink += int64(s)
    }
    delete proto
    unsafe {
        for (sh in shapes) {
            delete sh
        }
        delete shapes
    }
}

[benchmark]
def pipelines(b : B?) {
    let src <- make_ints(K)
    let ka <- make_keys(K, "")
    b |> run("sort_block/{K}", K) {
        var a := src
        a |> sort() $(x, y : int) => x > y
        g_sink += int64(a[0])
        delete a
    }
    b |> run("sort_default/{K}", K) {
        var a := src
        a |> sort()
        g_sink += int64(a[0])
        delete a
    }
    b |> run("array_clone/{K}", K) {
        var a := src
        g_sink += long_length(a)
        delete a
    }
    b |> run("finally_scope/{M}", M) {
        var s = 0
        for (i in range(M)) {
            {
                s = (s << 1) ^ i
            } finally {
                s ^= 1
            }
        }
        g_sink += int64(s)
    }
    b |> run("string_find/{K}", K) {
        var n = 0
        for (i in range(K)) {
            n += find(ka[i], "padding")
        }
        g_sink += int64(n)
    }
}

struct private Pt {
    x, y : int
    z : float
}

[benchmark]
def interop(b : B?) {
    let ia <- make_ints(M)
    let pts <- [for (i in range(M)); Pt(x = i, y = i * 2, z = float(i))]
    b |> run("hash_int/{M}", M) {
        var h = 0ul
        for (i in range(M)) {
            h += hash(ia[i])
        }
        g_sink += int64(h)
    }
    b |> run("hash_struct/{M}", M) {
        var h = 0ul
        for (p in pts) {
            h += hash(p)
        }
        g_sink += int64(h)
    }
    b |> run("interp_string/{M}", M) {
        var n = 0
        for (i in range(M)) {
            n += length("v={i}")
        }
        g_sink += int64(n)
    }
    b |> run("sqrt_float/{M}", M) {
        var f = 0.
        for (i in range(M)) {
            f = f * 0.5 + sqrt(float(ia[i]))
        }
        g_sink += int64(f)
    }
}
