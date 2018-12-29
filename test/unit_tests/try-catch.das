def test:bool
	let (a:int=1;b:int?)
		try
			a = b->
		catch
			a = 13
		assert(a==13)
	return true
