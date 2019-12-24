options log = true

[comment(message="yes",track=(one,2,3.14))]
struct Foo
	a : int

[export]
def test
	print("ok\n")
    return true
