require rtti

[export]
def test:bool
    verify(isSameType(getTypeInfo(1),getTypeInfo(2)))
    verify(!isSameType(getTypeInfo(1.0),getTypeInfo(2)))
    return true
