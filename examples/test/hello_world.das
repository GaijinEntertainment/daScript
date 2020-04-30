struct Foo
	t : int

[export]
def operator + ( a, b : Foo )
	print("here\n")
	return [[Foo a.t + b.t]]


def add(a,b)
	print("a + b = {a + b}\n")

[export]
def test()
	add(1,2)
	return true
