require apply_macro

options log = true

def custom_cond
	print("in the cond\n")
	return true

[add_cond(cond="custom_cond")]
def custom_func
	print("in the func\n")

[export]
def test()
	custom_func()
    return true

