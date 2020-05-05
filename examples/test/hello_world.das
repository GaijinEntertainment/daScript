options log_infer_passes = true

[export]
def test()
	var t <- {{ "one" => @(x:int):int => x+x ; "two" => @(x:int):int => x }}
	let z = invoke(t["two"],1)
	print("z = {z}\n")
	return true
