struct Foo
	dummy : int
	def operator . x : int
		return dummy
	def operator . x := ( value : int )
		dummy = value
	def test ( v : int ) : int
		x := v
		return x


[export]
def main
    var f = new Foo()
	f.x := 5
	let t = f.x
	print("t = {t}\n")


options log_infer_passes
