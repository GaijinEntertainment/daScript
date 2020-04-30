struct Foo
	t : int

[export]
def operator + ( a, b : Foo )
	print("here\n")
	return [[Foo a.t + b.t]]

[export]
def test()
	let c = [[Foo t=1]] + [[Foo t=2]]	// + at 24:11
	print("t ={c}\n")
	return true
