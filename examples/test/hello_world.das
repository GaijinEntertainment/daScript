options log = true

struct Foo
	a : int

def take ( a& )
	print("a = {a}\n")

[export]
def test()
	take([[Foo a = 2]])
	let t = 2
	take(t)
	return true
