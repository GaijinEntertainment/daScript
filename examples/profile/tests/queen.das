require testProfile

include ../config.das

options solid_context

let N = 8

def isplaceok ( a:int[8]; n,c:int )
    let cmn = c - n
    let cpn = c + n
    for i in range(0, n)
        let ai = a[i]
        if ai == c || ai == cmn + i || ai  == cpn - i
            return false
    return true

var solutions : int

def addqueen ( var a:int[8]; n:int )
    if n == N
        solutions++
    else
        for c in range(0, N)
            if isplaceok(a, n, c)
                a[n] = c
                addqueen(a, n + 1)

def test()
    solutions = 0
    var a : int[8]
    addqueen(a,0)
    assert(solutions == 92)

[export,no_jit,no_aot]
def main
    profile(20, "queen")  <|
        test()
    if is_cpp_time()
        cpp_label()
        profile(20, "queen")  <|
            let size = testQueens()
            assert(size == 92)
        run_files([[TestFile
            luauFile = "queen.luau",
            luaFile = "queen.lua",
            luaJitFile = "queen.lua",
            // monoFile = "queen.cs",
            dotNetFile = "queen.cs",
        ]])

