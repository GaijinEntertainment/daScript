options log = true

struct Foo
	a : int

def mkf(a:Foo)
	print("a = {a}\n")

def mkf(a:int)
	print("a = {a}\n")

[export]
def test
	// let t = @@mkf
	let t = @@<(arg:Foo):void> mkf
	debug(t,"\nt=")
	return true

