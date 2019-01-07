// options logInferPasses=true

def args(a:auto(numT);b:numT;c:string)
	return 2

def madd(a,b,c)
    return a*b+c

def madd(a,b;c:int)
    return a*b+c

def test
	args(1,2.0,"3")
	madd(1,2,3)
	return true
