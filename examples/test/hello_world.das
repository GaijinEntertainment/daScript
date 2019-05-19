options logNodes=true, logCpp=true, logStack=true

[export]
def test:bool
    for x in [[int[2] 1;1]]
        assert(x == 1)
    return true

