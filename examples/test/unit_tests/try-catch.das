// options plot=true,plotFile="../../test.dot"

[export]
def test:bool
	let a = 1
    let b:int?
    try
        a = deref(b)
    recover
        a = 13
    assert(a==13)
	return true
