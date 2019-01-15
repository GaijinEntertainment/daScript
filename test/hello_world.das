options log=true

def add(x,y:int;a:block<(x:int;y:int):int>)
    return invoke(a,x,y)

[export]
def test
    let x : int = 13
    let t = add(1,2) <| $(x,y)
        return x+y
    print("t={t}\n")
    return true
