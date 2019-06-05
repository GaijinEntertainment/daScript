// options logInferPasses = true
// options logCpp=true

[export,unsafe]
def test:bool
    var t : tuple < int; float; string >
    debug(t,"\nt=")
    print("t={t}\n")
    print("sizeof t = {typeinfo(sizeof type tuple <int;float;string>)}\n")
    var q = t
    print("q={q}\n")
    q._0 = 1
    q._1 = 2.0
    q._2 = "3"
    print("q={q}\n")
    debug(q,"\nq=")
    var pq = addr(q)
    print("pq={pq}\n")
    print("{pq._0} {pq._1} {pq._2}\n")
    print("{pq?._0 ?? 0} {pq?._1 ?? 0.0}\n")
    var qq = new tuple < int; float >
    print ( "qq = {qq}\n" )
    qq._0 = 1
    qq._1 = 2.0
    print ( "qq = {qq}\n" )
    delete qq
    return true


