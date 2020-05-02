options rtti = true

require rtti

def take ( a : auto! )
	print("const\n")

def take ( var a : auto! )
	print("value\n")

[export]
def test()
	let c = 1
	take(c)
	var v = 1
	take(v)
	return true
