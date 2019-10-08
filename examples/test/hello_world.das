options log_infer_passes=true

struct A
	i : int

let
    a = {{ "a" => [[A i = 1]]; "b" => [[A i = 2]]}}

[sideeffects]
def testFn(ch : string)
	var res : string
	find(a, "ttt")  <| $(templ: A?)
		if templ == null
			return
	return <- res

[export]
def test()
    testFn("blah")
    return true
