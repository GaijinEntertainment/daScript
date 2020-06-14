
[export]
def test()
	var tab <- {{ "one"=>1; "two"=> 2 }}
	let i = tab?["three"] ?? 3
	print("i = {i}\n")
	return true



