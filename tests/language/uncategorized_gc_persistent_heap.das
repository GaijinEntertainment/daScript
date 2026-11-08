options gen2
options persistent_heap = true
options gc

require dastest/testing_boost public

def test_aka_func(t : T?; var x aka X : int) {
    let tx = X
    t |> equal(tx, x)
}

def call_block(blk : block<(arg : int) : void>) {
    invoke(blk, 5)
}

[test(description = "an aka alias names the same variable as the original for locals, function and block arguments, and for-loop iterators")]
def test_aka_everything(t : T?) {
    t |> run("basic aka") @(t : T?) {
        var x aka X = 5
        X = 13
        t |> equal(x, 13)
    }
    t |> run("aka function") @(t : T?) {
        test_aka_func(t, 5)
    }
    t |> run("aka block") @(t : T?) {
        call_block() $(x aka X) {
            let tx = x
            t |> equal(tx, X)
        }
    }
    t |> run("nested aka block") @(t : T?) {
        var tot = 0
        var same = 0
        for (x aka X in range(10)) {
            for (x_ in range(10)) {
                tot ++
                if (x_ == X) {
                    same ++
                }
            }
        }
        t |> equal(tot, 100)
        t |> equal(same, 10)
    }
}

struct Foo {
    a : array<int>
}

[test(description = "a lambda capturing a const null struct pointer reads it inside the body and is deleted without finalizing the capture")]
def test_lambda_capture_const(var t : T?) {
    let f : Foo?    // we are capturing const value here, which needs a finalizer
    var l <- @ {// value will not be finalized, because its captured as const
        let lenF =  f != null ? length(f.a) : 0
        t |> equal(lenF, 0)
    }
    invoke(l)
    unsafe { delete l; }
}

[test(description = "a table whose slots are all values or tombstones after interleaved inserts and erases still inserts and keeps exactly the live keys")]
def test_tombstones(t : T?) {
    var tab : table<int; int>
    unsafe {
        tab[2] = 2          // note : i'm too lazy to change this into tab |> insert(2, 2)
        tab |> erase(2)
        tab[2] = 2
        tab[5] = 5
        tab |> erase(5)
        tab[7] = 7
        tab |> erase(7)
        tab[8] = 8
        tab[9] = 9
        tab |> erase(9)
        tab[10] = 10
        tab |> erase(10)
        tab[11] = 11
        tab |> erase(11)
        tab[12] = 12
        tab |> erase(12)
        tab[13] = 13
        tab |> erase(13)
        tab[14] = 14        // entire table is values or tombstones
        tab |> erase(14)
        tab[15] = 15
    }
    t |> equal(3, length(tab))
    for (key in keys(tab)) {
        if (!(key == 15 || key == 8 || key == 2)) {
            t |> failure("unexpected key {key}")
        }
    }
}

[test(description = "float4 color-named fields and swizzles read and write the same components as xyzw")]
def test_vector_fields(t : T?) {
    t |> run("basic fields") @(t : T?) {
        var v : float4
        v.r = 1.
        v.g = 2.
        v.b = 3.
        v.a = 4.
        t |> equal(float4(1., 2., 3., 4.), v)
    }
    t |> run("swizzle") @(t : T?) {
        var v : float4
        v.rg = float2(1., 2.)
        v.ba = float2(3., 4.)
        t |> equal(float4(1., 2., 3., 4.), v)
        t |> equal(float4(4., 3., 2., 1.), v.abgr)
    }
}
