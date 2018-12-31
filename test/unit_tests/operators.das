def charg(t:int):int
	t++
	return t

def test:bool
	let(x:int=1;y:int)
		y=charg(x)
		assert(x==1 & y==2)
    // cond
    let(a,b,c:int)
        true ? a : b = 1            // true returns left
        assert(a==1 & b==0)
        false ? a : b = 2           // false returns right
        assert(a==1 & b==2)
        true ? (c+=1) : (c+=2)      // only one siede is calculated
        assert(c==1)
        false ? (c+=3) : (c+=4)     // only one side
        assert(c==5)
    return true

