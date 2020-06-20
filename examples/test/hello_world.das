require daslib/apply

struct Foo
	i : int
	s : string

def visit(name: string; value)
	print("{name} = {value}\n")

[export]
def test
	var foo : Foo
	apply(foo,"visit")
	return true
