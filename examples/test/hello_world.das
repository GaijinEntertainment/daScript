// options log_infer_passes = true
options log = true

require UnitTest

[export]
def test
	let foo = "кошка 日本国 cat"
	print  ( "foo = {foo}\n")
	printw ( "foo = {foo}\n")
    return true
