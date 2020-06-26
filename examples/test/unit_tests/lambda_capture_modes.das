[export]
def test
	var a1 <- [{int 1;2}]
	var a2 <- [{int 1;2}]
	var a3 <- [{int 1;2}]
	unsafe
		var lam <- @ <| [[&a1,<-a2,:=a3]]	// due to &a1
			push(a1,1)
			push(a2,1)
			push(a3,1)
		invoke(lam)
	assert(length(a1)==3)
	assert(length(a2)==0)
	assert(length(a3)==2)
	return true
