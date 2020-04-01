// options log_infer_passes = true
options log = true

[export,unsafe]
def test

	var t : variant<i_value:uint;f_value:float>
	assert(typeinfo(sizeof t)==8)
	t.f_value = 1.0													// unsafe operation
	assert(t.i_value==0x3f800000)									// unsafe operation
	assert(variant_index(t)==0)

	assert(typeinfo(variant_index<i_value> t)==0)
	assert(typeinfo(variant_index<f_value> t)==1)

	assert(typeinfo(safe_variant_index<i_value> t)==0)
	assert(typeinfo(safe_variant_index<f_value> t)==1)
	assert(typeinfo(safe_variant_index<fg> t)==-1)
	assert(typeinfo(safe_variant_index<g> type int)==-1)

	assert(t is i_value)

	set_variant_index(t, typeinfo(variant_index<f_value> t))		// unsafe operation
	assert(t is f_value)


    return true
