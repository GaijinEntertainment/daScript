require daslib/regex
require daslib/regex_boost

[export]
def test()
	var r1 <- %regex~[\w\.+-]+@[\w\.-]+\.[\w\.-]+%%
	regex_debug(r1)
    return true

