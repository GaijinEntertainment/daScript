
.. _stdlib_fuzzer:

======
Fuzzer
======

.. das:module:: fuzzer

The FUZZER module implements fuzz testing infrastructure for daslang programs.
It generates random inputs for functions and verifies they do not crash or
produce unexpected errors, helping discover edge cases and robustness issues.

All functions and symbols are in "fuzzer" module, use require to get access to it.

.. code-block:: das

    require daslib/fuzzer



++++++++++++
Fuzzer tests
++++++++++++

  *  :ref:`fuzz (fuzz_count: int; blk: block\<():void\>) <function-fuzzer_fuzz_int_block_ls__c_void_gr_>`
  *  :ref:`fuzz (blk: block\<():void\>) <function-fuzzer_fuzz_block_ls__c_void_gr_>`
  *  :ref:`fuzz_all_ints_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_all_ints_op1_auto_Faker_string_0x7c>`
  *  :ref:`fuzz_all_unsigned_ints_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_all_unsigned_ints_op1_auto_Faker_string_0x86>`
  *  :ref:`fuzz_compareable_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_compareable_op2_auto_Faker_string_0xe6>`
  *  :ref:`fuzz_debug (fuzz_count: int; blk: block\<():void\>) <function-fuzzer_fuzz_debug_int_block_ls__c_void_gr_>`
  *  :ref:`fuzz_debug (blk: block\<():void\>) <function-fuzzer_fuzz_debug_block_ls__c_void_gr_>`
  *  :ref:`fuzz_eq_neq_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_eq_neq_op2_auto_Faker_string_0xf3>`
  *  :ref:`fuzz_float_double_or_float_vec_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_float_double_or_float_vec_op1_auto_Faker_string_0x8e>`
  *  :ref:`fuzz_float_double_or_float_vec_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_float_double_or_float_vec_op2_auto_Faker_string_0xad>`
  *  :ref:`fuzz_float_double_or_float_vec_op3 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_float_double_or_float_vec_op3_auto_Faker_string_0x173>`
  *  :ref:`fuzz_float_or_float_vec_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_float_or_float_vec_op1_auto_Faker_string_0x99>`
  *  :ref:`fuzz_float_or_float_vec_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_float_or_float_vec_op2_auto_Faker_string_0xa3>`
  *  :ref:`fuzz_int_vector_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_int_vector_op2_auto_Faker_string_0x127>`
  *  :ref:`fuzz_numeric_and_storage_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_and_storage_op1_auto_Faker_string_0x6c>`
  *  :ref:`fuzz_numeric_and_vector_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_and_vector_op1_auto_Faker_string_0x40>`
  *  :ref:`fuzz_numeric_and_vector_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_and_vector_op2_auto_Faker_string_0xb8>`
  *  :ref:`fuzz_numeric_and_vector_op2_no_unint_vec (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_and_vector_op2_no_unint_vec_auto_Faker_string_0xcb>`
  *  :ref:`fuzz_numeric_and_vector_signed_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_and_vector_signed_op1_auto_Faker_string_0x53>`
  *  :ref:`fuzz_numeric_op1 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_op1_auto_Faker_string_0x62>`
  *  :ref:`fuzz_numeric_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_op2_auto_Faker_string_0xdc>`
  *  :ref:`fuzz_numeric_op3 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_op3_auto_Faker_string_0x14b>`
  *  :ref:`fuzz_numeric_op4 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_op4_auto_Faker_string_0x17f>`
  *  :ref:`fuzz_numeric_scal_vec_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_scal_vec_op2_auto_Faker_string_0x118>`
  *  :ref:`fuzz_numeric_vec_scal_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_numeric_vec_scal_op2_auto_Faker_string_0x109>`
  *  :ref:`fuzz_rotate_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_rotate_op2_auto_Faker_string_0x143>`
  *  :ref:`fuzz_shift_op2 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_shift_op2_auto_Faker_string_0x135>`
  *  :ref:`fuzz_vec_mad_op3 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_vec_mad_op3_auto_Faker_string_0x164>`
  *  :ref:`fuzz_vec_op3 (t: auto; var fake: Faker; funcname: string) : auto <function-fuzzer_fuzz_vec_op3_auto_Faker_string_0x155>`


fuzz
^^^^

.. _function-fuzzer_fuzz_int_block_ls__c_void_gr_:

.. das:function:: fuzz(fuzz_count: int; blk: block<():void>)

run block however many times
ignore panic, so that we can see that runtime crashes


:Arguments: * **fuzz_count** : int

            * **blk** : block<void>

.. _function-fuzzer_fuzz_block_ls__c_void_gr_:

.. das:function:: fuzz(blk: block<():void>)

----

.. _function-fuzzer_fuzz_all_ints_op1_auto_Faker_string_0x7c:

.. das:function:: fuzz_all_ints_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: int, uint, int64, uint64


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_all_unsigned_ints_op1_auto_Faker_string_0x86:

.. das:function:: fuzz_all_unsigned_ints_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: uint, uint64


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_compareable_op2_auto_Faker_string_0xe6:

.. das:function:: fuzz_compareable_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: int, uint, float, double, int64, uint64, string


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string


fuzz_debug
^^^^^^^^^^

.. _function-fuzzer_fuzz_debug_int_block_ls__c_void_gr_:

.. das:function:: fuzz_debug(fuzz_count: int; blk: block<():void>)

run block however many times
do not ignore panic, so that we can see where the runtime fails
this is here so that `fuzz` can be easily replaced with `fuzz_debug` for the purpose of debugging


:Arguments: * **fuzz_count** : int

            * **blk** : block<void>

.. _function-fuzzer_fuzz_debug_block_ls__c_void_gr_:

.. das:function:: fuzz_debug(blk: block<():void>)

----

.. _function-fuzzer_fuzz_eq_neq_op2_auto_Faker_string_0xf3:

.. das:function:: fuzz_eq_neq_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: int, uint, int64, uint64, float, double, string, int2, int3, int4, uint2, uint3, uint4, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_float_double_or_float_vec_op1_auto_Faker_string_0x8e:

.. das:function:: fuzz_float_double_or_float_vec_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: float, double, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_float_double_or_float_vec_op2_auto_Faker_string_0xad:

.. das:function:: fuzz_float_double_or_float_vec_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: float, double, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_float_double_or_float_vec_op3_auto_Faker_string_0x173:

.. das:function:: fuzz_float_double_or_float_vec_op3(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes three numeric or vector arguments.
arguments are: float, double, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_float_or_float_vec_op1_auto_Faker_string_0x99:

.. das:function:: fuzz_float_or_float_vec_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: float, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_float_or_float_vec_op2_auto_Faker_string_0xa3:

.. das:function:: fuzz_float_or_float_vec_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: float, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_int_vector_op2_auto_Faker_string_0x127:

.. das:function:: fuzz_int_vector_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: int, uint, int2, int3, int4, uint2, uint3, uint4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_and_storage_op1_auto_Faker_string_0x6c:

.. das:function:: fuzz_numeric_and_storage_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: int, uint, int8, uint8, int16, uint16, int64, uint64, float, double


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_and_vector_op1_auto_Faker_string_0x40:

.. das:function:: fuzz_numeric_and_vector_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: int, uint, float, double, string, int2, int3, int4, uint2, uint3, uint4, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_and_vector_op2_auto_Faker_string_0xb8:

.. das:function:: fuzz_numeric_and_vector_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: int, uint, float, double, int2, int3, int4, uint2, uint3, uint4, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_and_vector_op2_no_unint_vec_auto_Faker_string_0xcb:

.. das:function:: fuzz_numeric_and_vector_op2_no_unint_vec(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: int, uint, float, double, int2, int3, int4, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_and_vector_signed_op1_auto_Faker_string_0x53:

.. das:function:: fuzz_numeric_and_vector_signed_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: int, uint, float, double, string, int2, int3, int4, uint2, uint3, uint4, float2, float3, float4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_op1_auto_Faker_string_0x62:

.. das:function:: fuzz_numeric_op1(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes single numeric or vector argument.
arguments are: int, uint, float, double


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_op2_auto_Faker_string_0xdc:

.. das:function:: fuzz_numeric_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes two numeric or vector arguments.
arguments are: int, uint, float, double


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_op3_auto_Faker_string_0x14b:

.. das:function:: fuzz_numeric_op3(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes three numeric or vector arguments.
arguments are: int, uint, float, double


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_op4_auto_Faker_string_0x17f:

.. das:function:: fuzz_numeric_op4(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes four numeric or vector arguments.
arguments are: int, uint, float, double


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_scal_vec_op2_auto_Faker_string_0x118:

.. das:function:: fuzz_numeric_scal_vec_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes vector and matching scalar on the left
arguments pairs are: int2,int; int3,int; uint2,uint; uint3,uint; uint4,uint; int4,int; float2,float; float3,float; float4,float


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_numeric_vec_scal_op2_auto_Faker_string_0x109:

.. das:function:: fuzz_numeric_vec_scal_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes vector and matching scalar on the right
arguments pairs are: int2,int; int3,int; uint2,uint; uint3,uint; uint4,uint; int4,int; float2,float; float3,float; float4,float


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_rotate_op2_auto_Faker_string_0x143:

.. das:function:: fuzz_rotate_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes numeric or vector argument, with matching rotate type on the right.
arguments are: int, uint


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_shift_op2_auto_Faker_string_0x135:

.. das:function:: fuzz_shift_op2(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes numeric or vector argument, with matching shift type on the right.
arguments are: int, uint, int2, int3, int4, uint2, uint3, uint4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_vec_mad_op3_auto_Faker_string_0x164:

.. das:function:: fuzz_vec_mad_op3(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes three numeric or vector arguments.
arguments are: float2, float3, float4, int2, int3, int4, uint2, uint3, uint4 second argument is float, int, uint accordingly


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string

.. _function-fuzzer_fuzz_vec_op3_auto_Faker_string_0x155:

.. das:function:: fuzz_vec_op3(t: auto; fake: Faker; funcname: string) : auto

fuzzes generic function that takes three numeric or vector arguments.
arguments are: float2, float3, float4, int2, int3, int4, uint2, uint3, uint4


:Arguments: * **t** : auto

            * **fake** :  :ref:`Faker <struct-faker-Faker>`

            * **funcname** : string


