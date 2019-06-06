// options logCpp=true

[export,unsafe]
def test:bool
    var t : tuple < int; float; string >
    assert(typeinfo(sizeof t)==16)
    t._0 = 1
    t._1 = 2.0
    t._2 = "3"
    var q = t
    assert(q._0==1)
    assert(q._1==2.0)
    assert(q._2=="3")
    var pq = addr(q)
    assert(pq._0==1)
    assert(pq._1==2.0)
    assert(pq._2=="3")
    assert(pq?._0 ?? 0==1)
    assert(pq?._1 ?? 0.0==2.0)
    assert(pq?._2 ?? "nothing" =="3")
    var qq = new tuple < int; float >
    qq._0 = 1
    qq._1 = 2.0
    assert(qq?._0 ?? 0==1)
    assert(qq?._1 ?? 0.0==2.0)
    delete qq
    // make tuple
    let mkt = [[ tuple<int;float> 1, 2.0]]
    assert(mkt._0==1 & mkt._1==2.0)
    let mkta = [[ tuple<int;float> 1, 2.0; 3, 4.0]]
    assert(mkta[0]._0==1 & mkta[0]._1==2.0)
    assert(mkta[1]._0==3 & mkta[1]._1==4.0)
    // make tuple array, auto
    let ti = [[auto 1; 2; 3; 4]]
    assert(typeinfo(sizeof ti)==16)
    assert(ti[0]==1 & ti[1]==2 & ti[2]==3 & ti[3]==4)
    let tt = [[auto 1, 2; 3, 4]]
    assert(typeinfo(sizeof tt)==16)
    assert(tt[0]._0==1 & tt[0]._1==2 & tt[1]._0==3 & tt[1]._1==4)
    let tq = [[auto 1, 2.0]]
    assert(tq._0==1 & tq._1==2.0)
    return true

