// options log=true, print_var_access=true, print_ref=true

require testProfile

include ../config.das

let TOTAL_NUMBERS = 10000
let TOTAL_TIMES = 100

[sideeffects]
def f2s(nums)
    var summ_q = 0
    for i in 0..TOTAL_TIMES
        for s in nums
            summ_q += length(to_string(s))
    return summ_q

def mk_float(i)
	return float(i) + float(i)/float(TOTAL_NUMBERS)

[export,no_jit,no_aot]
def main
	var nums : array<float> <- [{for i in range(TOTAL_NUMBERS); mk_float(i)}]
	// reference summ
	var summ = 0
    for j in 0..TOTAL_TIMES
        for i in range(TOTAL_NUMBERS)
            summ += length(to_string(mk_float(i)))
    var f1 = 0
    profile(20,"f2s") <|
        f1 = f2s(nums)
    assert(f1==summ)
    if is_cpp_time()
        cpp_label()
        var f4 = 0
        profile(20,"f2s") <|
            f4 = test_f2s(nums, TOTAL_NUMBERS, TOTAL_TIMES)
        //assert(f4==summ)
        run_files([[TestFile
            luauFile = "f2s.luau",
            luaFile = "f2s.lua",
            luaJitFile = "f2s.lua",
            // nutFile = "f2s.nut",
            // nutModifiedFile = "f2s.nut",
            // asFile = "f2s.as",
            // jsFile = "f2s.js",
            // monoFile = "f2s.cs", // this one - interpreter goes bonkers
            dotNetFile = "f2s.cs",
        ]])

// string
//  "DAS AOT", "f2s", 0.102156, 20
//  "DAS JIT", "f2s", 0.104075, 20
//  "DAS INTERPRETER", "f2s", 0.112619, 20
// to_string
//  "DAS AOT", "f2s", 0.075944, 20
//  "DAS JIT", "f2s", 0.078881, 20
//  "DAS INTERPRETER", "f2s", 0.087431, 20