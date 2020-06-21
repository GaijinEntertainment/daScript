require daslib/apply

struct Ups
	t : int

struct Foo
	i : int
	s : string
	u : Ups

[export]
def test
	let foo : Foo
	apply(foo) <| $ ( name:string; value )
		print("{name} = {value}\n")
	return true
