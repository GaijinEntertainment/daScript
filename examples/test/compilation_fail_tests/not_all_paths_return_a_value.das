expect 30102

def test_return_int_vs_void(arg:bool)
    if arg
        return 13        // 30102: incompatible return type, int vs void

