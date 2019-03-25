// options printRef = true, printVarAccess = true, log = true// , logOptimizationPasses = true
options logCpp=true, optimize=false

enum Flags
    flag_a = 1
    flag_b = 2

struct Foo
    flags : Flags
    more_flags : Flags = Flags flag_a
    i : int

struct Bar
    f : Foo = Foo()
    s : string = "text here"

[export]
def test
    let b = Bar()
    print("b={b}\n")
    return true
