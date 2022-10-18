options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_jit_intrin shared private

require llvm/llvm_boost
require llvm/llvm_jit_common
require daslib/ast_boost

let g_intrin_lookup <- {{
// $, aka builtin
    // misc
        "$::length" => @@intrinsic_builtin_length;
    // type c-tors
        "$::range" => @@intrinsic_builtin_range;
        "$::urange" => @@intrinsic_builtin_range;
        "$::float" => @@intrinsic_builtin_float;
        "$::float4" => @@intrinsic_builtin_float4;
    // bits
        "$::clz" => @@intrinsic_bit_nzp_op1;
        "$::ctz" => @@intrinsic_bit_nzp_op1;
        "$::popcnt" => @@intrinsic_bit_op1;
// math
    "math::rcp" => @@intrinsic_math_rcp;
    "math::rcp_est"=> @@intrinsic_math_rcp;
    "math::exp" => @@intrinsic_math_float_op1;
    "math::sqrt" => @@intrinsic_math_float_op1
}}

def public lookup_intinsic(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    var result :  LLVMOpaqueValue?
    let call_name = "{expr.func._module.name}::{expr.func.name}"
    g_intrin_lookup |> find_if_exists(call_name) <| $ ( pfun )
        result = *pfun |> invoke(expr, arguments)
    return result

// intrinsics, $ aka builtin

def intrinsic_builtin_length(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume argType = expr.arguments[0]._type
    if argType.isGoodArrayType
        var arr = LLVMBuildLoad(g_builder, arguments[0], "arr")
        return LLVMBuildExtractValue(g_builder, arr, JIT_ARRAY_FIELD_SIZE, "array.size")
    else
        return null

def intrinsic_builtin_float(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    if length(arguments)==0
        return LLVMConstReal(LLVMFloatType(), 0.0lf)
    assume argType = expr.arguments[0]._type
    if argType.baseType==Type tFloat
        return arguments[0]
    elif argType.baseType==Type tDouble
        return LLVMBuildFPTrunc(g_builder, arguments[0], LLVMFloatType(), "double_to_float")
    elif argType.isSignedInteger
        return LLVMBuildSIToFP(g_builder, arguments[0], LLVMFloatType(), "int_to_float")
    elif argType.isUnsignedInteger
        return LLVMBuildUIToFP(g_builder, arguments[0], LLVMFloatType(), "uint_to_float")
    else
        failed_E(expr, "float({describe(argType)}) is not supported (yet?)")
        return null

def intrinsic_builtin_float4(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume resType = expr._type
    if length(arguments) == 0
        return build_broadcast_vector (resType, LLVMConstReal(LLVMFloatType(), 0.0lf))
    elif length(arguments) == 1
        assume argType = expr.arguments[0]._type
        if argType.baseType==Type tFloat
            return build_broadcast_vector(resType,arguments[0])
        elif argType.baseType==Type tDouble
            return build_broadcast_vector(resType,LLVMBuildFPTrunc(g_builder, arguments[0], LLVMFloatType(), "double_to_float"))
        elif argType.isSignedInteger
            return build_broadcast_vector(resType,LLVMBuildSIToFP(g_builder, arguments[0], LLVMFloatType(), "int_to_float"))
        elif argType.isUnsignedInteger
            return build_broadcast_vector(resType,LLVMBuildUIToFP(g_builder, arguments[0], LLVMFloatType(), "uint_to_float"))
        else
            failed_E(expr, "float4({describe(argType)}) is not supported (yet?)")
            return null
    else
        failed_E(expr, "float4 of {length(arguments)} arguments is not supported (yet?)")
        return null


def intrinsic_builtin_range(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    var x, y : LLVMOpaqueValue?
    if expr.arguments.length==1
        x = LLVMConstInt(LLVMInt32Type(), 0ul, 0)
        y = arguments[0]
    else
        x = arguments[0]
        y = arguments[1]
    return LLVMBuildInt2_xy(g_builder, x, y, string(expr.name))

let private bit_op1_name <- {{
    "clz" => "ctlz";
    "ctz" => "cttz";
    "popcnt" => "ctpop"
}}

def intrinsic_bit_nzp_op1(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume opType = expr.arguments[0]._type
    var op_name : string
    bit_op1_name |> find_if_exists(string(expr.name)) <| $ ( pname )
        op_name = *pname
    if op_name==""
        failed_E(expr, "{expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var sqrt_name = "llvm.{op_name}.i32"
    if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
        if opType.baseType==Type tInt64 || opType.baseType==Type tUInt64
            sqrt_name = "llvm.{op_name}.i64"
    else
        failed_E(expr, "{expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var args <- [{auto[] arguments[0]; LLVMConstInt(LLVMInt1Type(), 0ul, 0)}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))

def intrinsic_bit_op1(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume opType = expr.arguments[0]._type
    var op_name : string
    bit_op1_name |> find_if_exists(string(expr.name)) <| $ ( pname )
        op_name = *pname
    if op_name==""
        failed_E(expr, "{expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var sqrt_name = "llvm.{op_name}.i32"
    if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
        if opType.baseType==Type tInt64 || opType.baseType==Type tUInt64
            sqrt_name = "llvm.{op_name}.i64"
    else
        failed_E(expr, "{expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var args <- [{auto[] arguments[0]}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))

// intrinsics, math

def intrinsic_math_rcp(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume argType = expr.arguments[0]._type
    if argType.baseType==Type tFloat || (argType.isVectorType && argType.vectorBaseType==Type tFloat)
        var one = build_broadcast_vector (argType, LLVMConstReal(LLVMFloatType(), 1.0lf))
        return LLVMBuildFDiv(g_builder, one, arguments[0], "rcp")
    else
        failed_E(expr, "{expr.name}({describe(argType)}) is not supported (yet?)")
        return null

def intrinsic_math_float_op1(expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    var sqrt_name = "llvm.{expr.name}.f32"
    assume opType = expr.arguments[0]._type
    if opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
        if opType.baseType==Type tDouble
            sqrt_name = "llvm.{expr.name}.f64"
    else
        failed_E(expr, "{expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var args <- [{auto[] arguments[0]}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))

