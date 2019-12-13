options log = true
options print_ref = true
options print_var_access = true

[sideeffects]
def foo(var src;z:int)
	var t = 0
	for x in src
		t += x
	return t

[export]
def test
	var src = [[int[] 1;2;3;4;5]]
	let res = foo(src,0)
	print("res={res}\n")
    return true
