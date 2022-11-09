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
        "$::uint8" => @@intrinsic_builtin_uint8;
        "$::int" => @@intrinsic_builtin_int;
    // bits
        "$::clz" => @@intrinsic_bit_nzp_op1;
        "$::ctz" => @@intrinsic_bit_nzp_op1;
        "$::popcnt" => @@intrinsic_bit_op1;
    // variant
        "$::set_variant_index" => @@intrinsic_builtin_set_variant_index;
        "$::variant_index" => @@intrinsic_builtin_variant_index;
// math
    "math::rcp" => @@intrinsic_math_rcp;
    "math::rcp_est"=> @@intrinsic_math_rcp;
    "math::exp" => @@intrinsic_math_float_op1;
    "math::sqrt" => @@intrinsic_math_float_op1;
    "math::min" => @@intrinsic_math_minmax;
    "math::max" => @@intrinsic_math_minmax
}}

def public lookup_intinsic(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    var result :  LLVMOpaqueValue?
    let call_name = "{expr.func._module.name}::{expr.func.name}"
    g_intrin_lookup |> find_if_exists(call_name) <| $ ( pfun )
        result = *pfun |> invoke(g_builder, expr, arguments)
    return result

// intrinsics, $ aka builtin

def intrinsic_builtin_variant_index(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    return LLVMBuildLoad2(g_builder, LLVMInt32Type(), arguments[0], "$::variant_index")

def intrinsic_builtin_set_variant_index(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    return LLVMBuildStore2(g_builder, LLVMInt32Type(), arguments[1], arguments[0])

def intrinsic_builtin_length(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume argType = expr.arguments[0]._type
    if argType.isGoodArrayType
        var arr = LLVMBuildLoad(g_builder, arguments[0], "arr")
        return LLVMBuildExtractValue(g_builder, arr, uint(JIT_ARRAY SIZE), "array.size")
    else
        return null

def intrinsic_builtin_uint8(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    if length(arguments)==0
        return LLVMConstInt(LLVMInt8Type(), 0ul, 0)
    assume argType = expr.arguments[0]._type
    if argType.baseType==Type tInt8 || argType.baseType==Type tUInt8
        return arguments[0]
    elif argType.baseType==Type tFloat || argType.baseType==Type tDouble
        return LLVMBuildFPToUI(g_builder, arguments[0], LLVMInt8Type(), "fp_to_uint8")
    elif argType.isInteger
        return LLVMBuildTruncOrBitCast(g_builder, arguments[0], LLVMInt8Type(), "integer_to_uint8")
    else
        failed_E(expr, "uint8({describe(argType)}) is not supported (yet?)")
        return null

def intrinsic_builtin_int(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    if length(arguments)==0
        return LLVMConstInt(LLVMInt32Type(), 0ul, 0)
    assume argType = expr.arguments[0]._type
    if argType.baseType==Type tInt || argType.baseType==Type tUInt
        return arguments[0]
    elif argType.baseType==Type tFloat || argType.baseType==Type tDouble
        return LLVMBuildFPToSI(g_builder, arguments[0], LLVMInt32Type(), "fp_to_int")
    elif argType.isInteger
        if argType.sizeOf>=4
            return LLVMBuildTruncOrBitCast(g_builder, arguments[0], LLVMInt32Type(), "trunc_integer_to_int")
        else
            return LLVMBuildSExtOrBitCast(g_builder, arguments[0], LLVMInt32Type(), "sext_integer_to_int")
    else
        failed_E(expr, "uint8({describe(argType)}) is not supported (yet?)")
        return null


def intrinsic_builtin_float(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
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

def intrinsic_builtin_float4(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume resType = expr._type
    if length(arguments) == 0
        return build_broadcast_vector (g_builder, resType, LLVMConstReal(LLVMFloatType(), 0.0lf))
    elif length(arguments) == 1
        assume argType = expr.arguments[0]._type
        if argType.baseType==Type tFloat
            return build_broadcast_vector(g_builder, resType,arguments[0])
        elif argType.baseType==Type tDouble
            return build_broadcast_vector(g_builder, resType,LLVMBuildFPTrunc(g_builder, arguments[0], LLVMFloatType(), "double_to_float"))
        elif argType.isSignedInteger
            return build_broadcast_vector(g_builder, resType,LLVMBuildSIToFP(g_builder, arguments[0], LLVMFloatType(), "int_to_float"))
        elif argType.isUnsignedInteger
            return build_broadcast_vector(g_builder, resType,LLVMBuildUIToFP(g_builder, arguments[0], LLVMFloatType(), "uint_to_float"))
        else
            failed_E(expr, "float4({describe(argType)}) is not supported (yet?)")
            return null
    else
        failed_E(expr, "float4 of {length(arguments)} arguments is not supported (yet?)")
        return null


def intrinsic_builtin_range(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
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

def build_op_name ( op_name:string; opType:TypeDeclPtr ) : string
    if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
        if opType.baseType==Type tInt64 || opType.baseType==Type tUInt64
            return "llvm.{op_name}.i64"
        else
            return "llvm.{op_name}.i32"
    elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
        if opType.baseType==Type tDouble
            return "llvm.{op_name}.f64"
        else
            return "llvm.{op_name}.f32"
    else
        failed("{op_name}({describe(opType)}) is not supported (yet?)")
    return ""

def intrinsic_bit_nzp_op1(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume opType = expr.arguments[0]._type
    var op_name : string
    bit_op1_name |> find_if_exists(string(expr.name)) <| $ ( pname )
        op_name = *pname
    if op_name==""
        failed_E(expr, "intrinsic {expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var sqrt_name = build_op_name(op_name, opType)
    if sqrt_name==""
        failed_E(expr, "intrinsic type for {expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var args <- [{auto[] arguments[0]; LLVMConstInt(LLVMInt1Type(), 0ul, 0)}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))

def intrinsic_bit_op1(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume opType = expr.arguments[0]._type
    var op_name : string
    bit_op1_name |> find_if_exists(string(expr.name)) <| $ ( pname )
        op_name = *pname
    if op_name==""
        failed_E(expr, "intrinsic {expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var sqrt_name = build_op_name(op_name, opType)
    if sqrt_name==""
        failed_E(expr, "intrinsic type {expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var args <- [{auto[] arguments[0]}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))

let private op2_name <- {{
    "min" => "minimum";
    "max" => "maximum"
}}

def intrinsic_op2(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume opType = expr.arguments[0]._type
    var op_name : string
    op2_name |> find_if_exists(string(expr.name)) <| $ ( pname )
        op_name = *pname
    if op_name==""
        failed_E(expr, "intrinsic {expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var sqrt_name = build_op_name(op_name, opType)
    if sqrt_name==""
        failed_E(expr, "intrinsic type {expr.name}({describe(opType)}) is not supported (yet?)")
        return null
    var args <- [{auto[] arguments[0]; arguments[1]}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))

def intrinsic_math_minmax(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume opType = expr.arguments[0]._type
    var op_name : string
    if opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
        var pred = expr.name=="min" ? LLVMRealPredicate LLVMRealOLT : LLVMRealPredicate LLVMRealOGT
        var cond = LLVMBuildFCmp(g_builder, pred, arguments[0], arguments[1], "")
        return LLVMBuildSelect( g_builder, cond, arguments[0], arguments[1], "{expr.name}")
    elif opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
        op_name = expr.name=="min" ? "min" : "max"
        if opType.isSignedIntegerOrIntVec
            op_name = "s{op_name}"
        else
            op_name = "u{op_name}"
    else
        failed_E(expr, "{expr.name} {describe(expr._type)} is not supported (yet?)")
        return null
    let sqrt_name = build_op_name(op_name, opType)
    var args <- [{auto[] arguments[0]; arguments[1]}]
    var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
    var id = LLVMLookupIntrinsicID(sqrt_name)
    var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
    if decl==null
        failed_E(expr, "missing intrinsic {sqrt_name}")
        return null
    return LLVMBuildCall(g_builder, decl, args, string(expr.name))


// intrinsics, math

def intrinsic_math_rcp(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
    assume argType = expr.arguments[0]._type
    if argType.baseType==Type tFloat || (argType.isVectorType && argType.vectorBaseType==Type tFloat)
        var one = build_broadcast_vector (g_builder, argType, LLVMConstReal(LLVMFloatType(), 1.0lf))
        return LLVMBuildFDiv(g_builder, one, arguments[0], "rcp")
    else
        failed_E(expr, "{expr.name}({describe(argType)}) is not supported (yet?)")
        return null

def intrinsic_math_float_op1(g_builder:LLVMOpaqueBuilder?; expr:smart_ptr<ExprCallFunc>; arguments:array<LLVMOpaqueValue?>) : LLVMOpaqueValue?
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

