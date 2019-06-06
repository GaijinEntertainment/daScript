// options logInferPasses = true
options log = true

[export,unsafe]
def test:bool
    let t = [[ tuple<int;float> 1, 2.0]]
    assert(t._0==1 & t._1==2.0)
    let q = [[ tuple<int;float> 1, 2.0; 3, 4.0]]
    assert(q[0]._0==1 & q[0]._1==2.0)
    assert(q[1]._0==3 & q[1]._1==4.0)
    return true


