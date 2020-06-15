
options log = true

struct Foo
	a,b  : int

struct Bar : Foo
	c : int

def setFoo ( var f : Foo; a,b:int )
	f.a = a
	f.b = b

def mkFoo ( var doop )
	doop.a = 13
	print(" doop = {doop}\n")

[export]
def test()
	var b = [[Bar a=1,b=2,c=3]]
	setFoo(b,4,5)
	print("b = {b}\n")

	var pf : Foo? = new Bar
	print("pf = {pf}\n")

	mkFoo ( new Foo )
	mkFoo ( new Bar )

	return true



