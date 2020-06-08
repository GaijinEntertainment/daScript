module ast_boost

require rtti
require ast

def isExpression(t:TypeDeclPtr; top:bool=true) : bool
    if t==null
        return false
    elif t.dim.length!=0
        return false
    elif t.baseType==Type tHandle
        if "{t.annotation.annotationModule.name}" == "ast"
            return starts_with("{t.annotation.name}","Expr")
        else
            return false
    elif t.baseType==Type tPointer & top
        return isExpression(t.firstType,false)
    else
        return false


class MacroMacro : AstFunctionAnnotation
    [unsafe] def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        compiling_program().flags |= ProgramFlags needMacroModule
        func.flags |= FunctionFlags init
        var blk <- new [[ExprBlock() at=func.at]]
        var ifm <- new [[ExprCall() at=func.at, name:="is_compiling_macros"]]
        var ife <- new [[ExprIfThenElse() at=func.at, cond<-ExpressionPtr(ifm), if_true<-func.body]]
        push_back(blk.list,ife)
        func.body <- ExpressionPtr(blk)
        return true

class TagFunctionAnnotation : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for arg in args
            let val = get_annotation_argument_value(arg)
            if !(val is tBool)
                errors := "tag_function annotation expects only tag names"
                return false
        return true

class TagStructureAnnotation : AstStructureAnnotation
    def override apply ( var str:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for arg in args
            let val = get_annotation_argument_value(arg)
            if !(val is tBool)
                errors := "tag_structure annotation expects only tag names"
                return false
        return true

def for_each_tag_function(mod:Module?; tag:string; blk : block<(var func:FunctionPtr):void> )
    for_each_function(mod, "") <| $ ( func )
        for ann in func.annotations
            if "{ann.annotation.name}" == "tag_function"
                for t in ann.arguments
                    if "{t.name}" == tag
                        invoke(blk, func)

def find_arg(argn:string; args:AnnotationArgumentList) : RttiValue
    for a in args
        if "{a.name}" == argn
            return get_annotation_argument_value(a)
    return RttiValue_nothing()


class TagFunctionMacro : AstStructureAnnotation
    [unsafe] def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        let ann_name = "{st.name}"
        let argTag = find_arg("tag", args)
        if argTag is nothing
            panic("missing tag argument")
        elif !(argTag is tString)
            panic("tag must be string")
        let tag = argTag as tString
        var blk <- setup_macro("__setup_tag_functions", st.at)
        var cll <- new [[ExprCall() at=st.at, name:="setup_tag_annotation"]]
        push_back(cll.arguments,new [[ExprConstString() at=st.at, value:=ann_name]])
        push_back(cll.arguments,new [[ExprConstString() at=st.at, value:=tag]])
        var nee <- new [[ExprNew() at=st.at, initializer=true]]
        nee.typeexpr <- new [[TypeDecl() baseType=Type tStructure, structType=reinterpret<Structure?> st]]
        push_back(cll.arguments, nee)
        push_back(blk.list, cll)
        return true

def apply_tag_annotation(tag:string; ann:FunctionAnnotationPtr)
    for_each_tag_function(this_module(), tag) <| $ ( func )
        add_function_annotation(func, ann)

[unsafe]
def setup_tag_annotation(name,tag:string; classPtr)
    var ann <- make_function_annotation(name, classPtr)
    apply_tag_annotation(tag, ann)
    this_module() |> add_function_annotation(ann)
    delete ann

[unsafe]
def find_unique_function ( mod : Module?; name : string )
    var res : FunctionPtr
    var count = 0
    for_each_function(mod, name) <| $ ( func )
        res <- func
        count ++
    if count > 1
        panic("more than one function encountered")
    return res

[unsafe]
def setup_macro(name: string; at:LineInfo) : ExprBlock?
    compiling_program().flags |= ProgramFlags needMacroModule
    var fn <- compiling_module() |> find_unique_function(name)
    if fn == null
        fn <- new [[Function() at=at, atDecl=at, name:="{name}"]]
        fn.flags |= FunctionFlags generated
        fn.flags |= FunctionFlags init
        fn.result <- new [[TypeDecl() baseType=Type tVoid]]
        var blk <- new [[ExprBlock() at=at]]
        var iblk <- new [[ExprBlock() at=at]]
        var ifm <- new [[ExprCall() at=at, name:="is_compiling_macros"]]
        var ife <- new [[ExprIfThenElse() at=at, cond<-ExpressionPtr(ifm), if_true:=ExpressionPtr(iblk)]]
        push_back(blk.list,ife)
        fn.body <- ExpressionPtr(blk)
        compiling_module() |> add_function(fn)
        return reinterpret<ExprBlock?> iblk
    else
        if fn.body.__rtti != "ExprBlock"
            panic("expecting func.ExprBlock")
        var blk = reinterpret<ExprBlock?> fn.body
        if blk.list.length!=1 | blk.list[0].__rtti!="ExprIfThenElse"
            panic("expecting is_compiling_macros")
        var ife = reinterpret<ExprIfThenElse?> blk.list[0]
        if ife.if_true.__rtti != "ExprBlock"
            panic("expecting ife.if_true.ExprBlock")
        return reinterpret<ExprBlock?> ife.if_true

// replacing ExprIsVariant(value,name) => ExprOp2('==",value.__rtti,"name")
// if value is ast::Expr*
class BetterRttiVisitor : AstVariantMacro
    def override visitExprIsVariant(prog:ProgramPtr; mod:Module?;expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        if isExpression(expr.value.typeDecl)
            var vdr <- new [[ExprField() at=expr.at, name:="__rtti", value <- clone_expression(expr.value)]]
            var cna <- new [[ExprConstString() at=expr.at, value:=expr.name]]
            var veq <- new [[ExprOp2() at=expr.at, op:="==", left<-ExpressionPtr(vdr), right<-ExpressionPtr(cna)]]
            return ExpressionPtr(veq)
        return [[ExpressionPtr]]

[_macro]
def setup
    if is_compiling_macros()
        add_new_function_annotation("macro", new MacroMacro())
        add_new_function_annotation("tag_function",new TagFunctionAnnotation())
        add_new_structure_annotation("tag_structure",new TagStructureAnnotation())
        add_new_structure_annotation("tag_function_macro",new TagFunctionMacro())
        add_new_variant_macro("better_rtti_in_expr", new BetterRttiVisitor())
