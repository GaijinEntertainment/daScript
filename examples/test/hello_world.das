options log = true
options log_stack = true

require UnitTest

[export]
def test
    let t = takeDummy(makeDummy())
    print ( "t = {t}\n" )
    return true
