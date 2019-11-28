options log = true

def foo(a = "112")
	print(a)

[export]
def test
	foo()
    return true

