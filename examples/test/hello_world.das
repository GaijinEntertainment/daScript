// options log = true, logInferPasses = true

require rtti

struct Foo
    a : int
    b : float[3]

enum BooHoo
    boo = 1
    hoo = 2

[export]
def test
    let t : Foo
    let info : TypeInfo & = getTypeInfo(t)
    let sinfo : StructInfo & = deref(info.structType)
    debug(info,"\ninfo = ")
    debug(sinfo,"\nsinfo = ")
    for idx in range(0,int(sinfo.count))
        debug(sinfo[idx],"\nfield = ")
    for field in sinfo
        debug(field,"\nfield is ")
    let et : BooHoo
    let etinfo : TypeInfo & = getTypeInfo(et)
    let einfo : EnumInfo & = deref(etinfo.enumType)
    debug(etinfo,"\netinfo = ")
    debug(einfo,"\neinfo = ")
    for idx in range(0,int(einfo.count))
        debug(einfo[idx],"\nenum value = ")
    for field in einfo
        debug(field,"\nenum value is ")
    return true
