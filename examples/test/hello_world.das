// options log = true, logInferPasses = true

def boo(a:int=1;b:int)
    return a + b

def boo(a,b,c:int)
    return a + b + c

def test_function_infer
    let t1 = boo([a=2,b=3])
    assert(t1==5)
    let t2 = boo([b=3])
    assert(t2==4)
    let t3 = boo([a=1,b=2,c=3])
    assert(t3==6)

def hoo(a=1;b)
    return a + b

def hoo(a,b,c)
    return a + b + c

def test_generic_infer
    let t1 = hoo([a=2,b=3])
    assert(t1==5)
    let t2 = hoo([b=3])
    assert(t2==4)
    let t3 = hoo([a=1,b=2,c=3])
    assert(t3==6)

[export]
def test
    test_function_infer()
    test_generic_infer()
    return true

/*
boo()
30304: no matching function or generic function boo (  )
candidates:
 boo ( a : int; b : int ) : int
    missing argument b
 boo ( a : int; b : int; c : int ) : int
    missing argument a
    missing argument b
    missing argument c

boo(1,2.0)
30304: no matching function or generic function boo ( int, float )
candidates:
 boo ( a : int; b : int ) : int
    invalid argument b, expecting (int) passing (float)
 boo ( a : int; b : int; c : int ) : int
    invalid argument b, expecting (int) passing (float)
    missing argument c

boo([b=3.0])
30304: no matching function or generic boo
candidates:
 boo ( a : int; b : int ) : int
    invalid argument b, expecting (int) passing (float)
 boo ( a : int; b : int; c : int ) : int
    while looking for argument b, can't skip function argument a because it has no default value

boo([b=3,a=2])
30304: no matching function or generic boo
candidates:
 boo ( a : int; b : int ) : int
    can't match argument a, its submitted out of order
 boo ( a : int; b : int; c : int ) : int
    while looking for argument b, can't skip function argument a because it has no default value
*/

