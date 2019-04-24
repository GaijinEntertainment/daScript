// options log = true, logInferPasses = true

options rtti=true

require rtti

struct Foo
    [min=0,max=3] a : int = 2
    [bArg=true, sArg="text", fArg=2.3] b : float[3]
    s : string = "blah blah"

enum BooHoo
    boo = 1
    hoo = 2

[sideeffects]
def test_annotations
    let t : Foo
    let info : TypeInfo & = getTypeInfo(t)
    let sinfo : StructInfo & = deref(info.structType)
    print("struct {sinfo.name}\n")
    for field in sinfo
        print("\t")
        var first = true
        if field.annotation_arguments != null
            print("[")
            let args : AnnotationArguments & = deref(field.annotation_arguments)
            for arg in args
                if first
                    first = false
                else
                    print(",")
                print("{arg.name}=")
                if arg.basicType==Type tInt
                    print("{arg.iValue}")
                elif arg.basicType==Type tFloat
                    print("{arg.fValue}")
                elif arg.basicType==Type tString
                    print("\"{arg.sValue}\"")
                elif arg.basicType==Type tBool
                    print("{arg.bValue}")
            print("] ")
        print("{field.name} = ")
        if field.basicType==Type tInt
            print("{field.iValue}")
        elif field.basicType==Type tFloat
            print("{field.fValue}")
        elif field.basicType==Type tString
            print("\"{field.sValue}\"")
        elif field.basicType==Type tBool
            print("{field.bValue}")
        print("\n")


[sideeffects]
def test_structs
    let t : Foo
    let info : TypeInfo & = getTypeInfo(t)
    let sinfo : StructInfo & = deref(info.structType)
    debug(info,"\ninfo = ")
    debug(sinfo,"\nsinfo = ")
    for idx in range(0,int(sinfo.count))
        debug(sinfo[idx],"\nfield = ")
    for field in sinfo
        debug(field,"\nfield is ")

[sideeffects]
def test_enums
    let et : BooHoo
    let etinfo : TypeInfo & = getTypeInfo(et)
    let einfo : EnumInfo & = deref(etinfo.enumType)
    debug(etinfo,"\netinfo = ")
    debug(einfo,"\neinfo = ")
    for idx in range(0,int(einfo.count))
        debug(einfo[idx],"\nenum value = ")
    for field in einfo
        debug(field,"\nenum value is ")

def describeType(info)
    let bt = info.basicType
    if bt==Type tHandle
        print("{info.annotation_or_name.name}")
    elif bt==Type tStructure
        print("{info.structType.name}")
    else
        print("{bt} ")

def logFuncInfo(info:FuncInfo)
    print("def {info.name}")
    if info.count != 0u
        print("(")
        for arg in info
            print("{arg.name} : ")
            describeType(arg)
        print(")")
    print("\n")

[sideeffects]
def test_functions()
    for fi in range(0,getTotalFunctions())
        let finfo : FuncInfo & = getFunctionInfo(fi)
        logFuncInfo(finfo)

[export]
def test
    test_structs()
    test_enums()
    test_functions()
    test_annotations()
    return true
