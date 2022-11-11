options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module fuzzer shared private

require daslib/faker
require daslib/constant_expression
require dastest/testing_boost

var private total_fuzz_count : int = 1000

def public fuzz ( blk : block )
    //! run block however many times
    //! ignore panic, so that we can see that runtime crashes
    for i in range(total_fuzz_count)
        try
            invoke(blk)
        recover
            pass

def public fuzz ( fuzz_count:int; blk : block )
    //! run block however many times
    //! ignore panic, so that we can see that runtime crashes
    for i in range(fuzz_count)
        try
            invoke(blk)
        recover
            pass

def public fuzz_debug ( blk : block )
    //! run block however many times
    //! do not ignore panic, so that we can see where the runtime fails
    //! this is here so that `fuzz` can be easily replaced with `fuzz_debug` for the purpose of debugging
    for i in range(total_fuzz_count)
        invoke(blk)

def public fuzz_debug ( fuzz_count:int; blk : block )
    //! run block however many times
    //! do not ignore panic, so that we can see where the runtime fails
    //! this is here so that `fuzz` can be easily replaced with `fuzz_debug` for the purpose of debugging
    for i in range(fuzz_count)
        invoke(blk)

[constant_expression(funcname)]
def public fuzz_numeric_and_vector_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float2, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4, fake |> random_float4 )

[constant_expression(funcname)]
def public fuzz_numeric_and_vector_op2_no_unint_vec ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_float2, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4, fake |> random_float4 )


[constant_expression(funcname)]
def public fuzz_numeric_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )

[constant_expression(funcname)]
def public fuzz_compareable_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_int64,  fake |> random_int64 )
    invoke ( funcname, t, fake |> random_uint64, fake |> random_uint64 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )
    invoke ( funcname, t, fake |> any_string, fake |> any_string )

[constant_expression(funcname)]
def public fuzz_eq_neq_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_int64,  fake |> random_int64 )
    invoke ( funcname, t, fake |> random_uint64, fake |> random_uint64 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )
    invoke ( funcname, t, fake |> any_string, fake |> any_string )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float2, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4, fake |> random_float4 )

[constant_expression(funcname)]
def public fuzz_numeric_vec_scal_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_uint )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_uint )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_uint )
    invoke ( funcname, t, fake |> random_float2, fake |> random_float )
    invoke ( funcname, t, fake |> random_float3, fake |> random_float )
    invoke ( funcname, t, fake |> random_float4, fake |> random_float )

[constant_expression(funcname)]
def public fuzz_numeric_scal_vec_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint,  fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint,  fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint,  fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float4 )

[constant_expression(funcname)]
def public fuzz_numeric_and_vector_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_double )
    invoke ( funcname, t, fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4 )

[constant_expression(funcname)]
def public fuzz_numeric_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_double )

[constant_expression(funcname)]
def public fuzz_numeric_and_storage_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_int8 )
    invoke ( funcname, t, fake |> random_uint8 )
    invoke ( funcname, t, fake |> random_int16 )
    invoke ( funcname, t, fake |> random_uint16 )
    invoke ( funcname, t, fake |> random_int64 )
    invoke ( funcname, t, fake |> random_uint64 )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_double )

[constant_expression(funcname)]
def public fuzz_all_ints_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_int64 )
    invoke ( funcname, t, fake |> random_uint64 )

[constant_expression(funcname)]
def public fuzz_all_unsigned_ints_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_uint64 )

[constant_expression(funcname)]
def public fuzz_int_vector_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_uint4 )

[constant_expression(funcname)]
def public fuzz_shift_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_int )

[constant_expression(funcname)]
def public fuzz_rotate_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )

[constant_expression(funcname)]
def public fuzz_float_double_or_float_vec ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_double )
    invoke ( funcname, t, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4 )

[constant_expression(funcname)]
def public fuzz_float_or_float_vec ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4 )

[constant_expression(funcname)]
def public fuzz_numeric_op4 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int, fake |> random_int, fake |> random_int, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double, fake |> random_double, fake |> random_double )

[constant_expression(funcname)]
def public fuzz_numeric_op3 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int, fake |> random_int, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double, fake |> random_double )