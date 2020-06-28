struct Foo
	dummy : int

var
	g_counter = 0

def finalize ( f : Foo )
	g_counter ++

[export]
def test
	var pA = new Foo
	var pB = new Foo
	var C = [[Foo]]
	var lam <- @ <| [[<-pB,:=pC]]
		assert(pA.dummy==0)
		assert(pB.dummy==0)
		assert(C.dummy==0)
	assert ( pB==null )
	invoke(lam)
	delete lam
	assert(g_counter==2)	// b and c were deleted, a stayed do to copy, not move, not clone
	return true
