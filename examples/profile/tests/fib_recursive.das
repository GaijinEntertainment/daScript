// options log=true, print_var_access=true, print_ref=true

require testProfile

include ../config.inc

[sideeffects]
def fibR(n)
    if (n < 2)
        return n
    return fibR(n - 1) + fibR(n - 2)

[export,no_aot,no_jit]
def main
	var f3 = 0
    profile(20,"fibbonacci recursive") <|
        f3 = fibR(31)
	assert(f3==1346269)
    if is_cpp_time()
        cpp_label()
        var f5 = 0
        profile(20,"fibbonacci recursive") <|
            f5 = testProfile::testFibR(31)
        assert(f5==1346269)
        run_files([[TestFile
            luaFile = "fib_recursive.lua",
            luaJitFile = "fib_recursive.lua",
            nutFile = "fib_recursive.nut",
            asFile = "fib_recursive.as",
            jsFile = "fib_recursive.js",
            monoFile = "fib_recursive.cs"
        ]])


