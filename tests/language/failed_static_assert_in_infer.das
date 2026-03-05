options gen2
// options log=true,log_infer_passes=false,log_optimization_passes=true,optimize=true

expect 40100

[marker(no_coverage)]
def fold(a : array<auto(numt)>; var from : numt) {
    for (x in a) {
        from += x
    }
    static_assert(typeinfo sizeof(type<numt>) == 4, "only works for small types")
    return from
}

[marker(no_coverage)]
def join(a : array<auto(numt)>) {
    var b = numt(0)
    return fold(a, b)
}

[marker(no_coverage)]
def test(msg) {
    let a : array<uint64>
    let q = join(a)
    return string(q) + msg
}

[marker(no_coverage)]
def test : bool {
    feint(test("this"))
    return true
}

