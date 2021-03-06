options indenting = 4
options no_aot = true

module cpp_bind

require fio
require rtti
require ast
require daslib/ast_boost

def log_cpp_class_method ( cpp_file:file; name:string; fntype:TypeDeclPtr )
    var cft <- clone_type(fntype.firstType)
    cft.flags |= TypeDeclFlags smartPtrNative
    let rettd = describe_cpp(cft)
    fwrite(cpp_file, "{rettd} {name} ( Context * __context__, Func __funcCall__")
    for argN,argT,argI in fntype.argNames,fntype.argTypes,range(100500)
        fwrite(cpp_file, ", ")
        if argI==0
            fwrite(cpp_file, "void * ")
        else
            let argtd = describe_cpp(argT)
            fwrite(cpp_file, "{argtd} ")
            if argT.isRefType
                fwrite(cpp_file, "& ")
        fwrite(cpp_file, "{argN}")
    fwrite(cpp_file, " )")

def log_cpp_class_method_call ( cpp_file:file; name:string; fntype:TypeDeclPtr )
    let rettd = describe_cpp(fntype.firstType)
    fwrite(cpp_file, "das_invoke_function<{rettd}>::invoke\n      <")
    for argN,argT,argI in fntype.argNames,fntype.argTypes,range(100500)
        if argI!=0
            fwrite(cpp_file, ",")
        if argI==0
            fwrite(cpp_file, "void *")
        else
            let argtd = describe_cpp(argT)
            fwrite(cpp_file, "{argtd}")
            if argT.isRefType
                fwrite(cpp_file, " &")
    fwrite(cpp_file, ">\n        (__context__,__funcCall__,\n          ")
    for argN,argI in fntype.argNames,range(100500)
        if argI!=0
            fwrite(cpp_file, ",")
        fwrite(cpp_file, "{argN}")
    fwrite(cpp_file, ")")
    if fntype.firstType.flags.smartPtr
        fwrite(cpp_file,".marshal()")

def log_cpp_class_adapter ( cpp_file:file; name:string; cinfo:TypeDeclPtr )
    assert(cinfo.isClass,"can only make adapter out of a class")
    var methods : array<string>
    var types : array<TypeDeclPtr>
    for fld in cinfo.structType.fields
        if !fld._type.isFunction|| "{fld.name}"=="__finalize"
            continue
        push(methods,"{fld.name}")
        push_clone(types,fld._type)
    let nmet = length(methods)
    fwrite(cpp_file, "class {name} \{\n")
    fwrite(cpp_file, "protected:\n")
    fwrite(cpp_file, "  enum \{\n")
    for mn,mni in methods, range(100500)
        fwrite(cpp_file, "    __fn_{mn} = {mni},\n")
    fwrite(cpp_file, "  };\n")
    fwrite(cpp_file, "protected:\n")
    fwrite(cpp_file, "  static int _das_class_method_offset[{nmet}];\n")
    fwrite(cpp_file, "public:\n")
    fwrite(cpp_file, "  {name} ( const StructInfo * info ) \{\n")
    fwrite(cpp_file, "    if ( _das_class_method_offset[0]==0 ) \{\n")
    for mn,mni in methods, range(100500)
        fwrite(cpp_file, "      _das_class_method_offset[__fn_{mn}] = adapt_field_offset(\"{mn}\",info);\n")
    fwrite(cpp_file, "    \}\n")
    fwrite(cpp_file, "  \}\n")
    for mn,mt in methods,types
        // get
        fwrite(cpp_file, "  static __forceinline Func get_{mn} ( void * self ) \{\n")
        fwrite(cpp_file, "    return getDasClassMethod(self,_das_class_method_offset[__fn_{mn}]);\n")
        fwrite(cpp_file, "  \}\n")
        // invoke
        fwrite(cpp_file, "  static __forceinline ")
        log_cpp_class_method(cpp_file, "invoke_{mn}",mt)
        fwrite(cpp_file, " \{\n")
        fwrite(cpp_file, "    ")
        if !mt.firstType.isVoid
            fwrite(cpp_file, "return ")
        log_cpp_class_method_call(cpp_file, mn,mt)
        fwrite(cpp_file, ";\n")
        fwrite(cpp_file, "  \}\n")
    fwrite(cpp_file, "};\n")
    fwrite(cpp_file, "int {name}::_das_class_method_offset[{nmet}];\n\n")
    delete types
