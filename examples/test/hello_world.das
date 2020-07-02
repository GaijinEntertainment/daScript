def r_true(a:int)
	print("{a} - true\n")
	return true

def r_false(a:int)
	print("{a} - false\n")
	return false


[export]
def test
	let t = r_false(1) && r_true(2)
	return true
