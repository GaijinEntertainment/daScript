def add(a,b:int):int
    return a + b

[export]
def test
    let fn = @add
    let t = invoke(fn, 1, 2)
    assert(t==3)
	let failed = false
	try
		let fn2 : function<(a,b:int):int>
		let t2 = invoke(fn2, 1, 2)
	catch
		failed = true
	assert(failed,"has to fail, invoking null function")
    return true

