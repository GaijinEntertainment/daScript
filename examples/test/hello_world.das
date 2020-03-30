// options log_infer_passes = true
options log = true

require UnitTest

[export]
def test
	let foo = "Россия 日本国 England"
	print  ( "foo = {foo}\n")
	printw ( "foo = {foo}\n")
    return true
