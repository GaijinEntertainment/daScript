options log = true
options rtti = true

require rtti

def fooFn ( a : int[3]; b : bitfield<one;two> )
	print("a = {a} b = {b}\n")

[export]
def test()
	let fooT <- get_typedecl(@@fooFn)
	print("fooT = {fooT}\n")
	return true
