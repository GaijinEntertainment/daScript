options log = true
// options log_infer_passes = true

def apply ( a,b : int; zap : function<(x,y:int):int> )
    let res = invoke(zap,a,b)
    print("{res}\n")

[export]
def test
    apply(1,2) <| @@(x,y) => x + y
    return true
