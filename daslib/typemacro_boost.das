options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module typemacro_boost shared private

require daslib/ast_boost
require daslib/macro_boost
require daslib/templates_boost public
require daslib/ast_boost public
require strings

def public int64_to_enum(_enu : auto(ET); value : int64) : ET {
    static_if (typeinfo sizeof(type<ET>) == 1) {
        return unsafe(reinterpret<ET>(int8(value)))
    } static_elif (typeinfo sizeof(type<ET>) == 2) {
        return unsafe(reinterpret<ET>(int16(value)))
    } else {
        return unsafe(reinterpret<ET>(int(value)))
    }
}

[macro_function]
def append_expressions(var blk : ExprBlock?; var eblk : ExpressionPtr) {
    for (e in (eblk as ExprBlock).list) {
        blk.list |> emplace(e)
    }
}

[macro_function]
def is_typedecl_ptr(t : TypeDeclPtr) {
    if (t.baseType != Type.tPointer || !t.flags.smartPtr) {
        return false
    }
    assume pt = t.firstType
    if (pt == null) {
        return false
    }
    if (pt.baseType != Type.tHandle || pt.annotation.name != "TypeDecl" || pt.annotation._module.name != "ast") {
        return false
    }
    return true
}

[macro_function]
def public get_string_const(expr : ExpressionPtr) : string {
    if (expr is ExprConstString) {
        return string((expr as ExprConstString).value)
    } elif (expr is ExprAddr) {
        return string((expr as ExprAddr).target)
    }
    macro_error(compiling_program(), expr.at, "expecting string constant or function address")
    return ""
}

[macro_function]
def public typemacro_argument(dimExpr; index : int; var constType : auto(ExprConstType); var defaultValue : auto(ValueT)) : ValueT {
    if (index < length(dimExpr)) {
        if (dimExpr[index].__rtti == typeinfo undecorated_typename(type<ExprConstType>)) {
            defaultValue = unsafe(reinterpret<ExprConstType?>(dimExpr[index])).value
        } else {
            panic("expecting {typeinfo undecorated_typename(type<ExprConstType>)} constant")
        }
    }
    return defaultValue
}

[macro_function]
def public typemacro_argument(dimExpr; index : int; var constType : ExprConstString; var defaultValue : auto(ValueT)) : ValueT {
    if (index < length(dimExpr)) {
        if (dimExpr[index].__rtti == "ExprConstString") {
            defaultValue := unsafe(reinterpret<ExprConstString?>(dimExpr[index])).value
        } elif (dimExpr[index].__rtti == "ExprAddr") {
            defaultValue := string(unsafe(reinterpret<ExprAddr?>(dimExpr[index])).target)
        } else {
            panic("expecting string constant or function address")
        }
    }
    return defaultValue
}

[function_macro(name="typemacro_function")]
class TypeMacroMacro : AstFunctionAnnotation {
    //! This macro converts function into a type macro.
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let className = "{func.name}`TypeMacro"
        var inscope parentType <- typeinfo ast_typedecl(type<ast::AstTypeMacro>)
        if (parentType == null) {
            errors := "ast::AstTypeMacro not found"
            return false
        }
        var inscope cls <- make_class(className, parentType.structType, compiling_module())
        cls.flags &= ~StructureFlags.genCtor
        let cvtFnName = "{func.name}`cvt`arguments"
        var inscope cvtFn <- qmacro_function(cvtFnName) <| $(prog : ProgramPtr; mod : Module?; td, passT : TypeDeclPtr) : TypeDeclPtr {
            return <- default<TypeDeclPtr>
        }
        compiling_module() |> add_function(cvtFn)
        let visitName = className + "`visit"
        var inscope visitFn <- qmacro_method(visitName, cls) <| $(var slf : $t(cls); prog : ProgramPtr; mod : Module?; td, passT : TypeDeclPtr) : TypeDeclPtr {
            return <- $c(cvtFnName)(prog, mod, td, passT)
        }
        compiling_module() |> add_function(visitFn)
        var field = find_structure_field(get_ptr(cls), "visit")
        if (field == null) {
            errors := "visit field does not exist"
            return false
        }
        field.init |> move_new <| qmacro(cast<auto>(@@$i(visitName)))
        let name =  ("name", RttiValue(tString = string(func.name)))
        cls |> append_annotation("ast_boost", "type_macro", [
            name
        ])
        compiling_module() |> add_structure(cls)
        return true
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup; args, progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        for (ann in func.annotations) {
            if (ann.annotation.name == "typemacro_function") {
                if (!(find_arg(ann.arguments, "patched") is nothing)) {
                    return true
                }
            }
        }
        // we expect at least 2 arguments
        if (length(func.arguments) < 2) {
            errors := "expecting at least 2 argumentst (macroArgument: TypeDeclPtr, passArgument: TypeDeclPtr)"
            return false
        }
        let cvtFnName = "{func.name}`cvt`arguments"
        var inscope cvtFn <- compiling_module() |> find_unique_function(cvtFnName)
        if (cvtFn == null) {
            errors := "cvt function not found"
            return false
        }
        var body = cvtFn.body as ExprBlock
        body.list |> clear
        var inscope retCall <- qmacro($c(func.name)(td, passT))
        // allowed are basic types (int, float, string, @@function etc) and TypeDeclPtr
        for (ai in range(2, length(func.arguments))) {
            let argIndex = ai - 2
            let macroArgIndex = ai - 1
            assume argType = func.arguments[ai]._type
            assume argDefaultValue = func.arguments[ai].init
            if (length(argType.dim) > 0) {
                errors := "expecting non-array arguments"
                return false
            }
            if (argDefaultValue == null) {
                var inscope checkE <- qmacro_block() <| $() {
                    if ($v(macroArgIndex) >= length(td.dimExpr)) {
                        macro_error(compiling_program(), td.at, "expecting {$v(string(func.arguments[ai].name))} argument")
                        return <- TypeDeclPtr()
                    }
                }
                append_expressions(body, checkE)
            }
            if (argType.baseType == Type.tInt) {
                var inscope defaultValue <- argDefaultValue != null ? clone_expression(argDefaultValue) : quote(0)
                (retCall as ExprCall).arguments |> emplace_new <| qmacro(typemacro_argument(td.dimExpr, $v(macroArgIndex), type<ExprConstInt>, $e(defaultValue)))
            } elif (argType.baseType == Type.tBool) {
                var inscope defaultValue <- argDefaultValue != null ? clone_expression(argDefaultValue) : quote(false)
                (retCall as ExprCall).arguments |> emplace_new <| qmacro(typemacro_argument(td.dimExpr, $v(macroArgIndex), type<ExprConstBool>, $e(defaultValue)))
            } elif (argType.baseType == Type.tString) {
                var inscope defaultValue <- argDefaultValue != null ? clone_expression(argDefaultValue) : quote("")
                (retCall as ExprCall).arguments |> emplace_new <| qmacro(typemacro_argument(td.dimExpr, $v(macroArgIndex), type<ExprConstString>, $e(defaultValue)))
            } elif (is_typedecl_ptr(argType)) {
                if (argDefaultValue != null) {
                    errors := "TypeDeclPtr argument cannot have default value"
                    return false
                }
                (retCall as ExprCall).arguments |> emplace_new <| qmacro(
                    (td.dimExpr[$v(macroArgIndex)] is ExprTypeDecl) ? (td.dimExpr[$v(macroArgIndex)] as ExprTypeDecl).typeexpr : td.dimExpr[$v(macroArgIndex)]._type)
            } else {
                errors := "unsupported argument type {describe(argType)}"
                return false
            }
        }
        var inscope retExpr <- qmacro_block() <| $() {
            return <- $e(retCall)
        }
        append_expressions(body, retExpr)
        for (ann in func.annotations) {
            if (ann.annotation.name == "typemacro_function") {
                astChanged = true
                ann.arguments |> add_annotation_argument("patched", true)
            }
        }
        return true
    }
}

[structure_macro(name="typemacro_template")]
class TypeMacroTemplate : AstStructureAnnotation { }

[macro_function]
def public make_typemacro_template_instance(instance_type, template_type : Structure?; ex : array<tuple<string, string>> = array<tuple<string, string>>()) {
    var extra : array<tuple<string, RttiValue>>
    extra.emplace(("parent", RttiValue(tString = "{template_type._module.name}::{template_type.name}")), 0)
    for (e in ex) {
        extra.emplace((e._0, RttiValue(tString = e._1)), 0)
    }
    unsafe {
        var append_structure <- reinterpret<smart_ptr<Structure>>(instance_type)
        append_structure.append_annotation("typemacro_boost", "typemacro_template", extra)
    }
}

[macro_function]
def public is_typemacro_template_instance(passArgument, templateType : TypeDeclPtr; extra : array<tuple<string, string>> = array<tuple<string, string>>()) : bool {
    //! template instance is determined by having parent == template.parent
    if (passArgument == null || passArgument.baseType != Type.tStructure || passArgument.structType == null) {
        return false
    }
    if (!templateType.isStructure) {
        panic("macro implementation error. templateType {describe(templateType)} is not a structure")
        return false
    }
    for (ann in passArgument.structType.annotations) {
        if (ann.annotation.name == "typemacro_template") {
            let parentName = find_arg(ann.arguments, "parent")
            return false if (!(parentName is tString))
            return false if ((parentName as tString) != "{templateType.structType._module.name}::{templateType.structType.name}")
            for (ex in extra) {
                let arg = find_arg(ann.arguments, ex._0)
                return false if (!(arg is tString))
                return false if ((arg as tString) != ex._1)
            }
            return true
        }
    }
    return false
}

struct public TypeMacroTemplateArgument {
    name : string
    argument_type : TypeDeclPtr
    inferred_type : TypeDeclPtr
}

[macro_function]
def public infer_struct_aliases(structType : Structure?; var args : array<TypeMacroTemplateArgument>) : bool {
    for (arg in args) {
        arg.inferred_type |> move_new <| get_structure_alias(structType, arg.name)
        if (arg.inferred_type == null) {
            return false
        }
    }
    return true
}

[macro_function]
def public infer_template_types(passArgument : TypeDeclPtr; var args  : array<TypeMacroTemplateArgument>) : TypeDeclPtr {
    for (arg in args) {
        assume templateType = arg.argument_type
        assume inferredTemplateType = arg.inferred_type
        if (templateType.isAutoOrAlias) {
            var inscope resT <- infer_generic_type(templateType, inferredTemplateType, true, true)
            return <- TypeDeclPtr() if (resT == null)
            return <- TypeDeclPtr() if (!inferredTemplateType |> is_same_type(resT, RefMatters.no, ConstMatters.no, TemporaryMatters.yes))
            compiling_program() |> update_alias_map(templateType, resT)
        } else {
            return <- TypeDeclPtr() if (!inferredTemplateType |> is_same_type(templateType, RefMatters.no, ConstMatters.no, TemporaryMatters.yes))
        }
    }
    var inscope resT <- clone_type(passArgument)
    resT.flags &= ~TypeDeclFlags.ref
    return <- resT
}

[macro_function]
def public add_structure_aliases(structType : Structure?; var args : array<TypeMacroTemplateArgument>) {
    for (arg in args) {
        structType |> add_structure_alias(arg.name, arg.argument_type)
    }
}

[macro_function]
def public verify_arguments(var args : array<TypeMacroTemplateArgument>) : bool {
    for (arg in args) {
        if (arg.argument_type.isAutoOrAlias) {
            macro_error(compiling_program(), arg.argument_type.at, "cannot infer template argument '{arg.name}'")
            return false
        }
    }
    return true
}

[macro_function]
def public template_structure_name(base : Structure?; arguments : array<TypeMacroTemplateArgument>; extra : array<tuple<string, string>> = array<tuple<string, string>>()) : string {
    return build_string() <| $(builder) {
        builder.write(base.name)
        builder.write("<")
        for (i in range(length(arguments))) {
            if (i > 0) {
                builder.write(",")
            }
            builder.write(describe(arguments[i].argument_type, false, false, true))
        }
        for (e in extra) {
            builder.write(",")
            builder.write(e._1)
        }
        builder.write(">")
    }
}

[function_macro(name="typemacro_template_function")]
class TemplateTypeMacroMacro : AstFunctionAnnotation {
    //! This one converts function into a type macro that uses template arguments.
    //! For example [typemacro_template(TFlatHashTable)] def makeFlatHashTable ( macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr; hashFunctionName : string) : TypeDeclPtr { ... }
    //! We generate the body that handles template argument inference and instantiation.
    /* LIKE THIS:
        var inscope template_type <- typeinfo ast_typedecl(type<TFlatHashTable>)
        var inscope template_arguments <- [
            TypeMacroTemplateArgument(name="KeyType",  argument_type <- clone_type(KeyType)),
            TypeMacroTemplateArgument(name="ValueType", argument_type <- clone_type(ValueType))
        ]
        var extra_template_arguments <- [
            ("hashFunctionName", hashFunctionName)
        ]
        // code bellow can be auto-generated
        if (passArgument != null) {
            return <- TypeDeclPtr() if (!is_typemacro_template_instance(passArgument, template_type, extra_template_arguments))
            return <- TypeDeclPtr() if (!infer_struct_aliases(passArgument.structType, template_arguments))
            return <- infer_template_types(passArgument, template_arguments)
        } else {
            return <- TypeDeclPtr() if (!verify_arguments(template_arguments))
            var struct_name = template_structure_name(template_type.structType, template_arguments, extra_template_arguments)
            var existing_struct = compiling_program().find_unique_structure(struct_name)
            return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = template_type.at) if (existing_struct != null)
            var inscope resType <- qmacro_template_class(struct_name, type<TFlatHashTable>)
            make_typemacro_template_instance(resType.structType, template_type.structType, extra_template_arguments)
            add_structure_aliases(resType.structType, template_arguments)
            return <- resType
        }
    */
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        // we expect at least 2 arguments
        if (length(func.arguments) < 2) {
            errors := "expecting at least 2 arguments (macroArgument: TypeDeclPtr, passArgument: TypeDeclPtr)"
            return false
        }
        // expect one and only one unique annotation argument
        if (length(args) != 1) {
            errors := "expecting [typemacro_template(TemplateTypeName)]"
            return false
        }
        return true
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup; args, progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        for (ann in func.annotations) {
            if (ann.annotation.name == "typemacro_template_function") {
                if (!(find_arg(ann.arguments, "patched") is nothing)) {
                    return true
                }
            }
        }
        // lets get the template type name
        let template_type_name = string(args[0].name)
        // we append to the existing function body
        var body = func.body as ExprBlock
        body.list.clear()
        // add template_type
        var inscope aliasT <- new TypeDecl(baseType = Type.alias, alias := template_type_name)
        body.list |> emplace_new <| qmacro_expr() {
            var inscope template_type <- typeinfo ast_typedecl(type<$t(aliasT)>)
        }
        // we need to build template arguments list
        var inscope mk_array_expr <- new ExprMakeArray(at = func.at, makeType <- new TypeDecl(baseType = Type.autoinfer))
        for (i in range(2, length(func.arguments))) {
            assume argType = func.arguments[i]._type
            if (is_typedecl_ptr(argType)) {
                let argName = string(func.arguments[i].name)
                var inscope mk_tuple <- qmacro(TypeMacroTemplateArgument(
                    name = "{$v(argName)}",
                    argument_type <- clone_type($i(argName))
                ))
                mk_array_expr.values |> emplace(mk_tuple)
            }
        }
        body.list |> emplace_new <| qmacro_expr() {
            var inscope template_arguments <- to_array_move($e(mk_array_expr))
        }
        // we need to build extra arguments list
        var inscope mk_extra_array_expr <- new ExprMakeArray(at = func.at, makeType <- new TypeDecl(baseType = Type.autoinfer))
        for (i in range(2, length(func.arguments))) {
            assume argType = func.arguments[i]._type
            if (!is_typedecl_ptr(argType)) {
                let argName = string(func.arguments[i].name)
                var inscope mk_tuple <- qmacro((
                    "{$v(argName)}",
                    "{$i(argName)}"
                ))
                mk_extra_array_expr.values |> emplace(mk_tuple)
            }
        }
        if (length(mk_extra_array_expr.values) == 0) {
            body.list |> emplace_new <| qmacro_expr() {
                var inscope extra_template_arguments : array<tuple<string, string>>
            }
        } else {
            body.list |> emplace_new <| qmacro_expr() {
                var inscope extra_template_arguments <- to_array_move($e(mk_extra_array_expr))
            }
        }
        // now we add boilerplate
        body.list |> emplace_new <| qmacro_block() {
            if (passArgument != null) {
                return <- TypeDeclPtr() if (!is_typemacro_template_instance(passArgument, template_type, extra_template_arguments))
                return <- TypeDeclPtr() if (!infer_struct_aliases(passArgument.structType, template_arguments))
                return <- infer_template_types(passArgument, template_arguments)
            } else {
                return <- TypeDeclPtr() if (!verify_arguments(template_arguments))
                var struct_name = template_structure_name(template_type.structType, template_arguments, extra_template_arguments)
                var existing_struct = compiling_program().find_unique_structure(struct_name)
                return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = template_type.at) if (existing_struct != null)
                var inscope resType <- qmacro_template_class(struct_name, type<$t(aliasT)>)
                make_typemacro_template_instance(resType.structType, template_type.structType, extra_template_arguments)
                add_structure_aliases(resType.structType, template_arguments)
                return <- resType
            }
        }
        // mark it as patched
        astChanged = true
        for (ann in func.annotations) {
            if (ann.annotation.name == "typemacro_template_function") {
                astChanged = true
                ann.arguments |> add_annotation_argument("patched", true)
            }
        }
        return true
    }
}

[structure_macro(name="template_structure")]
class TemplateStructure : AstStructureAnnotation {
    //! This macro creates typemacro function and associates it with the structure.
    //! It also creates the typemacro_template_function to associate with it.
    //! For example:
    //! [template_structure(KeyType,ValueType)] struct template TFlatHashTable { ... }
    //! creates:
    //! 1) [typemacro_function] def TFlatHashTable (macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
    //!     return <- make`template`TFlatHashTable(macroArgument, passArgument, KeyType, ValueType)
    //!  }
    //! 2) [typemacro_template_function(TFlatHashTable)] def make`template`TFlatHashTable (macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
    //!     return <- default<TypeDeclPtr>
    //!  }
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        if (length(args) == 0) {
            errors := "expecting at least one template argument name"
            return false
        }
        let makeFuncName = "make`template`{st.name}"
        var inscope ctxFnArguments : array<VariablePtr>
        for (arg in args) {
            if (true) {
                var inscope argVar <- new Variable(
                    name := string(arg.name),
                    at = st.at
                )
                if (arg.basicType == Type.tBool) {
                    argVar._type |> move_new <| new TypeDecl(baseType = Type.alias, alias := "TypeDeclPtr")
                } elif (arg.basicType == Type.tInt) {
                    argVar._type |> move_new <| new TypeDecl(baseType = Type.tInt)
                    argVar.init |> move_new <| qmacro($v(arg.iValue))
                } elif (arg.basicType == Type.tString) {
                    argVar._type |> move_new <| new TypeDecl(baseType = Type.tString)
                    argVar.init |> move_new <| qmacro($v(string(arg.sValue)))
                } else {
                    errors := "unsupported template argument {arg.name} type {arg.basicType}"
                    return false
                }
                ctxFnArguments.emplace(argVar)
            }
        }
        var inscope makeFunction <- qmacro_function(makeFuncName) <| $(macroArgument, passArgument : TypeDeclPtr; $a(ctxFnArguments)) : TypeDeclPtr {
            return <- default<TypeDeclPtr>
        }
        append_annotation(makeFunction, "typemacro_boost", "typemacro_template_function", [
             (("{st.name}", RttiValue(tBool=true))
        )])
        if (!(compiling_module() |> add_function(makeFunction))) {
            panic("can't add function {makeFuncName}")
        }
        let typeMacroFunctionName = string(st.name)
        var inscope callMacroFunction <- qmacro($c(makeFuncName)(macroArgument, passArgument))
        for (arg in args) {
            (callMacroFunction as ExprCall).arguments |> emplace_new <| qmacro($i(string(arg.name)))
        }
        var inscope typeMacroFunction <- qmacro_function(typeMacroFunctionName) <| $(macroArgument, passArgument : TypeDeclPtr; $a(ctxFnArguments)) : TypeDeclPtr {
            return <- $e(callMacroFunction)
        }
        append_annotation(typeMacroFunction, "typemacro_boost", "typemacro_function", [])
        if (!(compiling_module() |> add_function(typeMacroFunction))) {
            panic("can't add function {typeMacroFunctionName}")
        }
        return true
    }
}

[macro_function]
def public is_custom_work_done(structType : Structure?) : bool {
    for (ann in structType.annotations) {
        if (ann.annotation.name == "template_structure") {
            if (!(find_arg(ann.arguments, "custom_work_done") is nothing)) {
                return true
            }
        }
    }
    return false
}

[macro_function]
def public mark_custom_work_done(var structType : Structure?) {
    for (ann in structType.annotations) {
        if (ann.annotation.name == "template_structure") {
            ann.arguments |> add_annotation_argument("custom_work_done", true)
        }
    }
}
