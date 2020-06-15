
options log = true

struct Foo
	a,b  : int

struct Bar : Foo
	c : int

def setFoo ( var f : Foo; a,b:int )
	f.a = a
	f.b = b

def setFoo2 ( var f : Foo; a,b )
	f.a = a
	f.b = b

[export]
def test()
	var b = [[Bar a=1,b=2,c=3]]
	setFoo2(b,4,5)
	print("b = {b}\n")
	return true



