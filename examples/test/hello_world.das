// options log_infer_passes = true
// options log_generics = true

require rtti
require ast
require daslib/ast_boost

class AstFunctionAnnotation_P
    def abstract transform ( var call : smart_ptr<ExprCall>; var errors : das_string ) : ExpressionPtr
    def abstract apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    def abstract finish ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool

def log_cpp_class_method ( name:string; fntype:TypeDeclPtr )
    let rettd = describe_cpp(fntype.firstType)
    print("{rettd} {name} ( Context * __context__")
    for argN,argT,argI in fntype.argNames,fntype.argTypes,range(0,100500)
        print(", ")
        if argI==0
            print("void * ")
        else
            let argtd = describe_cpp(argT)
            print("{argtd} ")
            if argT.isRefType
                print("& ")
        print("{argN}")
    print(" )")

def log_cpp_class_method_call ( name:string; fntype:TypeDeclPtr )
    let rettd = describe_cpp(fntype.firstType)
    print("das_invoke_function<{rettd}>::invoke\n    <")
    for argN,argT,argI in fntype.argNames,fntype.argTypes,range(0,100500)
        if argI!=0
            print(",")
        if argI==0
            print("void *")
        else
            let argtd = describe_cpp(argT)
            print("{argtd}")
            if argT.isRefType
                print(" &")
    print(">\n      (__context__,getDasClassMethod(self,_das_class_method_offset[__fn_{name}]),self,\n        ")
    for argN,argI in fntype.argNames,range(0,100500)
        if argI!=0
            print(",")
        print("{argN}")
    print(")")

def log_cpp_class_adapter ( name:string; cinfo:TypeDeclPtr; log_h:bool=true; log_cpp:bool=true )
    assert(cinfo.isClass,"can only make adapter out of a class")
    var methods : array<string>
    var types : array<TypeDeclPtr>
    for fld in cinfo.structType.fields
        if !fld._type.isFunction|| "{fld.name}"=="__finalize"
            continue
        push(methods,"{fld.name}")
        push_clone(types,fld._type)
    let nmet = length(methods)
    if log_h
        print("\n// header file\n\n")
        print("class {name} \{\n")
        print("protected:\n")
        print("  enum \{\n")
        for mn,mni in methods, range(0,100500)
            print("    __fn_{mn} = {mni},\n")
        print("  };\n")
        print("protected:\n")
        print("  static int _das_class_method_offset[{nmet}];\n")
        print("public:\n")
        print("  {name} ( const StructInfo * info );\n")
        for mn,mt in methods,types
            print("  ")
            log_cpp_class_method(mn,mt)
            print(";\n")
        print("};\n")
    if log_cpp
        print("\n// cpp file\n\n")
        print("int {name}::_das_class_method_offset[{nmet}];\n\n")
        print("{name}::{name} ( const StructInfo * info ) \{\n")
        print("  if ( _das_class_method_offset[0]==0 ) \{\n")
        for mn,mni in methods, range(0,100500)
            print("    _das_class_method_offset[{mni}] = adapt_field_offset(\"{mn}\",info);\n")
        print("  \}\n\n")
        for mn,mt in methods,types
            log_cpp_class_method("{name}::{mn}",mt)
            print(" \{\n")
            print("  ")
            if !mt.firstType.isVoid
                print ( "return ")
            log_cpp_class_method_call(mn,mt)
            print(";\n")
            print("\}\n\n")
    delete types

[export]
def test

    print("\n")

    log_cpp_class_adapter("AstFunctionAnnotation", typeinfo(ast_typedecl type<AstFunctionAnnotation>))

    return true

