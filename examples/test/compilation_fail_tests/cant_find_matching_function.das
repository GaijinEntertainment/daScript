expect 30304, 30303

def madd(a;b:int;c)
    return a*b+c

def madd(a,b;c:int)
    return a*b+c

def join(a:array<auto>)
    var t = 0
    for x in a
        t += x              // 30303: no matching operator '+=' with arguments (int&, float&)
    return t

def test
    madd(1,2,3)             // 30304: no matching generic function (2 candidates)
    let x : array<int>
    let y : array<float>
    let tx = join(x)
    let ty = join(y)
    return true
