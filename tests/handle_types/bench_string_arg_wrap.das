options gen2
require dastest/testing_boost

require UnitTest

[benchmark]
def bench_string_arg_wrap(b : B?) {
    let nul_str : string
    let lit = "abcd"
    var acc = 0l
    b |> run("4 string args") {
        acc += test_four_strings(nul_str, lit, nul_str, lit)
    }
    let nul_ptr : void? = null
    let some_ptr = unsafe(reinterpret<void?>(lit))
    b |> run("4 pointer args") {
        acc += test_four_pointers(nul_ptr, some_ptr, nul_ptr, some_ptr)
    }
    feint("{acc}")
}
