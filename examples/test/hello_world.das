class Foo
	i : int
	k : array<int>
	def Foo
		i = 5
	def kaboom : lambda<int>
		var counter <- @ <| ()
			return i ++
		return <- counter

class Boo : Foo
	tt : int

def takeFoo ( var f : Foo? )
	var F = new Foo()
	unsafe
		*F <- *f
	print("F = {F}\n")
	print("f = {f}\n")

[export]
def test
	var x = new Foo()
	let boom <- x->kaboom()
	assert(x.i==5)
	takeFoo ( new Boo() )
	return true
