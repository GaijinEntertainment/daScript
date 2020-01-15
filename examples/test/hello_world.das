options log = true
options stack = 65536

def stover(count) : int
	print("count = {count}\n")
	if count == 1000
		return 1
	return stover(count+1)

let
	qq = stover(1)

[export]
def test
	print("qq = {qq}\n")
    return true
