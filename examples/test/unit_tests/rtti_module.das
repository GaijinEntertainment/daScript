require rtti

[export]
def test:bool
    verify(is_same_type(get_type_info(1),get_type_info(2)))
    verify(!is_same_type(get_type_info(1.0),get_type_info(2)))
    return true
