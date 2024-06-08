[unsafe_when_uninitialized]
struct Foo
	a : int = 5
	b : int


[export]
def main
	var a = [[Foo b=13]]
	print("a = {a}\n")
