options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module algorithm shared public

def reverse ( var a:array<auto> )
    let l = length(a)
    let half = l/2
    let lm1 = l-1
    for i in range(half)
        swap(a[i], a[lm1-i])

def combine ( a,b:array<auto(TT)> )
    var c : array<TT>
    reserve(c, length(a) + length(b))
    for t in a
        c |> push(t)
    for t in b
        c |> push(t)
    return <- c

def lower_bound ( a:array<auto(TT)>; f,l:int; val : TT const-& )
    assert(f>=0 && f<=l,"lower bound first out of range")
    assert(l>=f && l<=length(a),"lower bound last out of range")
    var count = l - f
    var first = f
    while count > 0
        let step = count / 2
        let it = first + step
        if a[it] < val
            first += step + 1
            count -= step + 1
        else
            count = step
    return first

def lower_bound ( a:array<auto(TT)>; val : TT const-& )
    return lower_bound(a,0,length(a),val)

def lower_bound ( a:array<auto(TT)>; f, l : int; less : block<(a:TT const-&):bool> )
    assert(f>=0 && f<=l,"lower bound first out of range")
    assert(l>=f && l<=length(a),"lower bound last out of range")
    var count = l - f
    var first = f
    while count > 0
        let step = count / 2
        let it = first + step
        if invoke(less,a[it])
            first += step + 1
            count -= step + 1
        else
            count = step
    return first

def lower_bound ( a:array<auto(TT)>; less : block<(a:TT const-&):bool> )
    return lower_bound(a, 0, length(a), less )

def binary_search ( a:array<auto(TT)>; val : TT const-& )
    let first = lower_bound(a, val)
    return (first!=length(a)) && (!(val<a[first]))

def binary_search ( a:array<auto(TT)>; f, last : int; val : TT const-& )
    let first = lower_bound(a, f, last, val)
    return (first!=last) && (!(val<a[first]))

