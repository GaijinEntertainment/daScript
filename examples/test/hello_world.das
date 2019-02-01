options log=true // , logOptimizationPasses=true

def strcat(a,b)
	let c = a
	c += b					// this will not prevent folding
    return c

def make_name(x)
    return strcat("hello_",x)

def make_another_name(x)
	print("{x}\n")			// this will prevent folding
	return x + "_ok"

struct Foo
	a : int = 5
	b : float = 6.

def name_foo()
	let t = Foo()
	debug(t,"t=")
	return "blah"

[export]
def test
    let t = make_name("world")
    print("\n{t}\n")
	let q = make_another_name("modify_arg")
	print("\n{q}\n")
	let w = name_foo()
	print("\n{w}\n")
    return true
