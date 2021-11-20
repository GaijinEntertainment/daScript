module function_annotation_macro

require ast
require daslib/ast_boost

[function_annotation(name="unique_transaction")]
class UniqueTransaction : AstFunctionAnnotation
    [unsafe] def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        print("we call {call.name}\n")
        return [[ExpressionPtr]]


