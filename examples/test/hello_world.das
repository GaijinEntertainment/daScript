options log=true
options log_cpp=true

[sideeffects]
def gotoN(n:int)
    var res = -1
    goto n
    label 0:
    res = 0
    goto label 3
    label 1:
    res = 1
    goto label 3
    label 2:
    res = 2
    goto label 3
    label 3:
    return res

[sideeffects]
def N_goto()
    verify(gotoN(0)==0)
    verify(gotoN(1)==1)
    verify(gotoN(2)==2)
    verify(gotoN(3)==-1)

[export]
def test()
    N_goto()
    return true
