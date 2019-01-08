options log=true, logInferPasses=false, optimize=true

def twoPlusTwo
    return 4

def test
    let t = twoPlusTwo() + 1
    print("t={t}\n")
    return true
