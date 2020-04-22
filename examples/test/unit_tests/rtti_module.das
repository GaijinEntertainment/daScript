require rtti

[export]
def test:bool
    verify(is_same_type(get_type_info(1),get_type_info(2)))
    verify(!is_same_type(get_type_info(1.0),get_type_info(2)))

	let v : variant<i:int; f:float>
	let vinfo & = get_type_info(v)
	assert(vinfo.argCount==2u)
	for i,n,t in range(0,2),arg_names(vinfo),arg_types(vinfo)
		if i==0
			assert(n=="i")
			verify(is_same_type(get_type_info(1),t,RefMatters no,ConstMatters no))
		elif i==1
			assert(n=="f")
			verify(is_same_type(get_type_info(1.0),t,RefMatters no,ConstMatters no))

    return true
