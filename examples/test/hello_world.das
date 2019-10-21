options log=true

[export]
def test()
    var i = 0
    label 0:
    label 1:
    print("i={i}\n")
    i ++
    if i < 10
        goto label 1
        print("unreachable\n")
    label 2:
    return true
