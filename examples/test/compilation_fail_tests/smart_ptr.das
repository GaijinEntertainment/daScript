expect 31300:1

require UnitTest

def pass_by_value_is_unsafe ( p:smart_ptr<TestObjectSmart> )	// this will fail if enabled in CodeOfPolicies
	pass

def clone_smart_ptr_is_unsafe
	var a, b : smart_ptr<TestObjectSmart>
	a <- b

[export]
def test
    return true
