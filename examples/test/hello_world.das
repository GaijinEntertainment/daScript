options printRef = true, printVarAccess = true, log = true, logOptimizationPasses = true

def strcat(a,b)
	let c = a
	c += b					// this will not prevent folding
    return c

def make_name(x)
    return strcat("hello_",x)

def make_another_name(x)
	print("{x}\n")			// this will prevent folding
	return x + "_ok"

let
    g_name = "_global_ok"

def make_g_name(x)
    return x + g_name

struct Foo
	a : int = 19
	b : float = 20.

def up_foo(a)
	a.a ++

def name_foo()
	let t = Foo()
	up_foo(t)
	return "{t}"		// this will fold

[export]
def test
    let t = make_name("world")
    print("\n{t}\n")
	let q = make_another_name("modify_arg")
	print("\n{q}\n")
	let w = name_foo()
	print("\n{w}\n")
    let g = make_g_name("touch_global")
    print("\n{g}\n")
    g_name = "gaa"   // this says, global changes
    return true
