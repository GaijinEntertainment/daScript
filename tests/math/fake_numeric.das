options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module fake_numeric shared private

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

[call_macro(name="fake_numeric")]
class FakeNumericMacro : AstCallMacro
    //! This macro convert fake_numeric(func_id) to the comprehensive faker test::
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments)==1,prog,expr.at,"expecting fake_numeric(func_name), incorrect number of arguments")
        assume cid = expr.arguments[0]
        macro_verify(cid is ExprVar,prog,expr.at,"expecting fake_numeric(func_name), func_name is not a function id")
        let func_name = string((cid as ExprVar).name)
        return qmacro_block <|
            t |> run($v(func_name)) <| @@ ( t : T? )
                var fake <- Faker()
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_double) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float2) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float3) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float4) )


[call_macro(name="fake_numeric2")]
class FakeNumericMacro2 : AstCallMacro
    //! This macro convert fake_numeric(func_id) to the comprehensive faker test::
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments)==1,prog,expr.at,"expecting fake_numeric2(func_name), incorrect number of arguments")
        assume cid = expr.arguments[0]
        macro_verify(cid is ExprVar,prog,expr.at,"expecting fake_numeric2(func_name), func_name is not a function id")
        let func_name = string((cid as ExprVar).name)
        return qmacro_block <|
            t |> run($v(func_name)) <| @@ ( t : T? )
                var fake <- Faker()
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float,  fake |> random_float ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_double, fake |> random_double ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float2, fake |> random_float2 ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float3, fake |> random_float3 ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float4, fake |> random_float4 ) )


[call_macro(name="fake_float")]
class FakeFloatMacro : AstCallMacro
    //! This macro convert fake_numeric(func_id) to the comprehensive faker test::
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments)==1,prog,expr.at,"expecting fake_numeric(func_name), incorrect number of arguments")
        assume cid = expr.arguments[0]
        macro_verify(cid is ExprVar,prog,expr.at,"expecting fake_numeric(func_name), func_name is not a function id")
        let func_name = string((cid as ExprVar).name)
        return qmacro_block <|
            t |> run($v(func_name)) <| @@ ( t : T? )
                var fake <- Faker()
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float2) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float3) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float4) )


[call_macro(name="fake_float2")]
class FakeFloatMacro2 : AstCallMacro
    //! This macro convert fake_numeric(func_id) to the comprehensive faker test::
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments)==1,prog,expr.at,"expecting fake_numeric2(func_name), incorrect number of arguments")
        assume cid = expr.arguments[0]
        macro_verify(cid is ExprVar,prog,expr.at,"expecting fake_numeric2(func_name), func_name is not a function id")
        let func_name = string((cid as ExprVar).name)
        return qmacro_block <|
            t |> run($v(func_name)) <| @@ ( t : T? )
                var fake <- Faker()
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float,  fake |> random_float ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float2, fake |> random_float2 ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float3, fake |> random_float3 ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float4, fake |> random_float4 ) )

[call_macro(name="fake_float_234")]
class FakeFloatMacro_234 : AstCallMacro
    //! This macro convert fake_numeric(func_id) to the comprehensive faker test::
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments)==1,prog,expr.at,"expecting fake_numeric(func_name), incorrect number of arguments")
        assume cid = expr.arguments[0]
        macro_verify(cid is ExprVar,prog,expr.at,"expecting fake_numeric(func_name), func_name is not a function id")
        let func_name = string((cid as ExprVar).name)
        return qmacro_block <|
            t |> run($v(func_name)) <| @@ ( t : T? )
                var fake <- Faker()
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float2) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float3) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float4) )


[call_macro(name="fake_float2_234")]
class FakeFloatMacro2_234 : AstCallMacro
    //! This macro convert fake_numeric(func_id) to the comprehensive faker test::
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments)==1,prog,expr.at,"expecting fake_numeric2(func_name), incorrect number of arguments")
        assume cid = expr.arguments[0]
        macro_verify(cid is ExprVar,prog,expr.at,"expecting fake_numeric2(func_name), func_name is not a function id")
        let func_name = string((cid as ExprVar).name)
        return qmacro_block <|
            t |> run($v(func_name)) <| @@ ( t : T? )
                var fake <- Faker()
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float2, fake |> random_float2 ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float3, fake |> random_float3 ) )
                fuzz <|
                    t |> accept ( $c(func_name)(fake |> random_float4, fake |> random_float4 ) )
