options log = true, logCpp = true

[sideeffects]
def printA
    print("A\n")

[sideeffects,noaot]
def printB
    print("B\n")

[export]
def test
    printA()
    printB()
    return true
