options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module contracts shared private

require ast
require daslib/ast_boost

def public isYetAnotherVectorTemplate ( var td:TypeDeclPtr )
    return td.isHandle && td.annotation!=null && td.annotation.is_any_vector

class IsAnyType : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        if args.length != 1
            errors := "expecting at least one argument"
            return false
        for argv in args                    // note: N^2
            let val = get_annotation_argument_value(argv)
            if !(val is tBool)
                errors := "invalid argument type {argv.name}"
                return false
            var found = false
            for fna in func.arguments
                if fna.name==argv.name
                    found = true
                    break
            if !found
                errors := "function {func.name} does not have argument {argv.name}"
                return false
        return true
    def override isSpecialized : bool
        return true

[contract(name="expect_any_array")]
class IsAnyArrayMacro : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if !(typ.baseType==Type tArray || length(typ.dim)!=0 || isYetAnotherVectorTemplate(typ))
                        errors := "argument {argv.name} is not a vector, array, or [], it is {describe(typ)}"
                        return false
        return true

[contract(name="expect_any_enum")]
class IsAnyEnumMacro : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if length(typ.dim)!=0 || (typ.baseType!=Type tEnumeration && typ.baseType!=Type tEnumeration8 && typ.baseType!=Type tEnumeration16)
                        errors := "argument {argv.name} is not an enumeration"
                        return false
        return true

[contract(name="expect_any_vector_type")]
class IsAnyVectorType : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if length(typ.dim)!=0 || !isVectorType(typ.baseType)
                        errors := "argument {argv.name} is not a vector type (int2, float3, range, etc)"
                        return false
        return true

[contract(name="expect_any_struct")]
class IsAnyStructMacro : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if typ.baseType!=Type tStructure || typ.structType==null || typ.structType.flags.isClass
                        errors := "argument {argv.name} is not a structure, it is {describe(typ)}"
                        return false
        return true

[contract(name="expect_any_numeric")]
class IsAnyNumericMacro : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if !typ.isNumeric
                        errors := "argument {argv.name} is not a numeric, it is {describe(typ)}"
                        return false
        return true

[contract(name="expect_any_workhorse")]
class IsAnyWorkhorse : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if !typ.isWorkhorseType
                        errors := "argument {argv.name} is not a workhorse type, it is {describe(typ)}"
                        return false
        return true

[contract(name="expect_any_workhorse_raw")]
class IsAnyWorkhorseNonPtrMacro : IsAnyType
    def override isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        for fna,typ in func.arguments,types // note: N^2
            for argv in decl.arguments
                if fna.name==argv.name
                    if !typ.isWorkhorseType && !typ.isPointer && !typ.isString
                        errors := "argument {argv.name} is not a workhorse raw type, it is {describe(typ)}"
                        return false
        return true