// options plot=true,plotFile="../../test.dot"

[export]
def test:bool
	let (a:int=1;b:int?)
		try
			a = deref(b)
		recover
			a = 13
		assert(a==13)
	return true
