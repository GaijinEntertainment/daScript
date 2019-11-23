options log = true

[unsafe]
def lock ( Tab : table<auto(keyT);auto(valT)>; blk : block<(t : table<keyT;valT>#):void >  )
    print("lock-n-load\n")
    __builtin_table_lock(Tab)
    invoke(blk,reinterpret<table<auto(keyT);auto(valT)> const!>(Tab))
    __builtin_table_unlock(Tab)

[unsafe]
def lock ( Tab : table<auto(keyT);auto(valT)>#; blk : block<(t : table<keyT;valT>#):void >  )
    print("just pass\n")
    invoke(blk,Tab)

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

