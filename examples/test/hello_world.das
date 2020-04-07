// options log_infer_passes = true
options log = true

options optimize = false

options log_cpp = true

struct Foo
	t	: variant<i_value:uint;f_value:float>?

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
	assert(t as i_value == 0x3f800000)
	set_variant_index(t, typeinfo(variant_index<f_value> t))		// unsafe operation
	assert(t as f_value == 1.0)

    assert(t ?as f_value ?? 2.0 == 1.0)
    assert(t ?as i_value ?? 1u == 1u)

	t as f_value = 2.0
	set_variant_index(t, typeinfo(variant_index<i_value> t))		// unsafe operation
	assert(t as i_value == 0x40000000)

	t as i_value = 0x3f800000
	set_variant_index(t, typeinfo(variant_index<f_value> t))		// unsafe operation
	assert(t as f_value == 1.0)

	var paniced = false
	try
		t as i_value = 0x40000000									// will assert, because t is f_value
	recover
		paniced = true
	assert(paniced)

	var f : Foo
	var fp : Foo?
	assert ( fp?.t?.i_value ?? 13u == 13u )							// unsafe ?.i_value
	fp = addr(f)
	assert ( fp?.t?.i_value ?? 14u == 14u )							// unsafe ?.i_value
	f.t = addr(t)
	assert ( fp?.t?.i_value ?? 15u == 0x3f800000 )					// unsafe ?.i_value
    assert ( fp?.t ?as i_value ?? 15u == 15u )

    return true
