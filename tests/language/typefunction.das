require dastest/testing_boost public

[type_function]
def take ( a : auto(TT); a1,a2 : int )
	return typeinfo(sizeof type<TT>) + a1 + a2

[test]
def test_type_function ( t : T? )
    let size = take<int>(1,2)   // note syntax
    t |> equal(7, size)
