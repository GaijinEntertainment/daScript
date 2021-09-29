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
