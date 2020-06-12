require daslib/regex
require daslib/regex_boost

[export]
def test()
	var r1 <- %regex~f*%%
	regex_debug(r1)

	let res = regex_match(r1, "stats")
	print("res = {res}\n")

    return true

