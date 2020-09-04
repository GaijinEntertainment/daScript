options debugger = true

[export]
def test
	var x = 5
	breakpoint()
	x = 6
	for t in range(0,10)
		print("x={x} t={t}\n")
		x ++
		x += 3
	return true
