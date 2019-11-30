options log = true
// options log_infer_passes = true

let
    ONE = 1

[unsafe]
def getOnePtr : int const?
    return addr(ONE)

def printValue(a:auto;name:string)
    if typeinfo(is_temp a)
        print("temp {name} = {a}\n")
    else
        print("{name} = {a}\n")

[export]
def test
    var t <- {{ "one" => 1; "two" => 2 }}
    print("t = {t}\n")
    var tClone : table<string;int>
    lock ( t ) <| $ ( localT )
        debug(localT,"localT = ")
        let l = length(localT)
        print("length(localT) = {l}\n")
        tClone := localT
    var fSi = 13
    lock ( t ) <| $ ( localT )
        let fS = find(localT, "one")
        if fS != null
            fSi = *fS
            print ( "found it, its {fSi}\n" )
    var q : table<string;int const?>
    q["null"] = null
    q["one"] = getOnePtr()
    printValue(q,"q")
    lock ( q ) <| $ ( localQ )
        let pI = localQ["null"]
        print ( "pI = {pI}\n" )
        let j = deref(localQ["one"])
        print ( "j = {j}\n" )
        lock(localQ) <| $ ( localQQ )
            printValue ( localQQ, "qq" )
    return true

