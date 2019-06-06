// options logInferPasses = true
options log = true

[export,unsafe]
def test:bool
    let ti = [[auto 1; 2; 3; 4]]
    assert(typeinfo(sizeof ti)==16)
    assert(ti[0]==1 & ti[1]==2 & ti[2]==3 & ti[3]==4)
    let tt = [[auto 1, 2; 3, 4]]
    assert(typeinfo(sizeof tt)==16)
    assert(tt[0]._0==1 & tt[0]._1==2 & tt[1]._0==3 & tt[1]._1==4)
    return true


