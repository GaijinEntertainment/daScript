// options log = true
// options log_nodes = true

require UnitTest

[export,unsafe]
def test
    // pdiv(1.0,0.0)
    let a = 1.0
    let b = 0.0
    print("c = {testGetDiv(a,b)}\n")
    return true
