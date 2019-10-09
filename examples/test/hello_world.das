// options log_infer_passes=true

require bbb

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
