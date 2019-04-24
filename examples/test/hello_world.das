// options log = true, logInferPasses = true

require rtti

struct Foo
    a : int
    b : float[3]

[export]
def test
    let t : Foo
    let info : TypeInfo & = getTypeInfo(t)
    debug(info,"info=")
    let sinfo : StructInfo & = deref(info.structType)
    for idx in range(0,int(sinfo.fieldsSize))
        debug(sinfo[idx],"\nfield = ")
    for field in sinfo
        debug(field,"\nfield is ")
    return true
