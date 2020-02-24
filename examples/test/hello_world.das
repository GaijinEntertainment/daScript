options log = true

[export]
def test
    var const_tab <- {{ "one"=>1; "two"=>2 }}
	find_for_edit(const_tab,"one") <| $(pVal)
		if pVal != null
			print("val = {pVal}\n")
			*pVal = 4
	find(const_tab,"one") <| $(pVal)
		if pVal != null
			print("val = {pVal}\n")
	return true

