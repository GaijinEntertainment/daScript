struct Foo
	a: array<int>
	b: float

[export]
def test
	var t <- {{ "one" => [[Foo b=1.]]; "two" => [[Foo b=2., a <- [{int 1;2;3}] ]] }}
	let q = sprint(t,print_flags_debugger)
	print("\n--->\n\n{q}\n----\n")
	return true
