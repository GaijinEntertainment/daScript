
.. _stdlib_faker:

=====
Faker
=====

The FAKER module implements collection of random data generators for use in testing and otherwise.

All functions and symbols are in "faker" module, use require to get access to it. ::

    require daslib/faker

++++++++++
Structures
++++++++++

.. _struct-faker-Faker:

.. das:attribute:: Faker

Instance of the faker with all the settings inside.

:Fields: * **min_year** : uint = 0x7bc - minimal faker's year

         * **total_years** : uint = 0x2a - faker year's range

         * **rnd** : iterator<uint> = each_random_uint(13) - fakers random number generator

         * **max_long_string** : uint = 0x1000 - maximal length of generated string


+++++++++++
Constructor
+++++++++++

  *  :ref:`Faker (var rng: iterator\<uint\>) : Faker <function-_at_faker_c__c_Faker_1_ls_u_gr_G>` 

.. _function-_at_faker_c__c_Faker_1_ls_u_gr_G:

.. das:function:: Faker(rng: iterator<uint>) : Faker

|detail/function-faker-Faker-0xbf733ac794066d1b|

:Arguments: * **rng** : iterator<uint>

+++++++++++++
Random values
+++++++++++++

  *  :ref:`random_int (var faker: Faker) : int <function-_at_faker_c__c_random_int_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint (var faker: Faker) : uint <function-_at_faker_c__c_random_uint_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_int8 (var faker: Faker) : int8 <function-_at_faker_c__c_random_int8_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint8 (var faker: Faker) : uint8 <function-_at_faker_c__c_random_uint8_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_int16 (var faker: Faker) : int16 <function-_at_faker_c__c_random_int16_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint16 (var faker: Faker) : uint16 <function-_at_faker_c__c_random_uint16_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float (var faker: Faker) : float <function-_at_faker_c__c_random_float_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_int2 (var faker: Faker) : int2 <function-_at_faker_c__c_random_int2_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_range (var faker: Faker) : range <function-_at_faker_c__c_random_range_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_range64 (var faker: Faker) : range64 <function-_at_faker_c__c_random_range64_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_int3 (var faker: Faker) : int3 <function-_at_faker_c__c_random_int3_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_int4 (var faker: Faker) : int4 <function-_at_faker_c__c_random_int4_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint2 (var faker: Faker) : uint2 <function-_at_faker_c__c_random_uint2_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_urange (var faker: Faker) : urange <function-_at_faker_c__c_random_urange_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_urange64 (var faker: Faker) : urange64 <function-_at_faker_c__c_random_urange64_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint3 (var faker: Faker) : uint3 <function-_at_faker_c__c_random_uint3_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint4 (var faker: Faker) : uint4 <function-_at_faker_c__c_random_uint4_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float2 (var faker: Faker) : float2 <function-_at_faker_c__c_random_float2_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float3 (var faker: Faker) : float3 <function-_at_faker_c__c_random_float3_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float4 (var faker: Faker) : float4 <function-_at_faker_c__c_random_float4_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float3x3 (var faker: Faker) : float3x3 <function-_at_faker_c__c_random_float3x3_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float3x4 (var faker: Faker) : float3x4 <function-_at_faker_c__c_random_float3x4_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_float4x4 (var faker: Faker) : float4x4 <function-_at_faker_c__c_random_float4x4_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_int64 (var faker: Faker) : int64 <function-_at_faker_c__c_random_int64_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_uint64 (var faker: Faker) : uint64 <function-_at_faker_c__c_random_uint64_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`random_double (var faker: Faker) : double <function-_at_faker_c__c_random_double_S_ls_faker_c__c_Faker_gr_>` 

.. _function-_at_faker_c__c_random_int_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int(faker: Faker) : int

Generates random integer.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint(faker: Faker) : uint

Generates random unsigned integer.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_int8_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int8(faker: Faker) : int8

Generates random int8.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint8_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint8(faker: Faker) : uint8

Generates random uint8.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_int16_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int16(faker: Faker) : int16

Generates random int16.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint16_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint16(faker: Faker) : uint16

Generates random uint16.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float(faker: Faker) : float

Generates random float.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_int2_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int2(faker: Faker) : int2

Generates random int2.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_range_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_range(faker: Faker) : range

Generates random range.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_range64_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_range64(faker: Faker) : range64

Generates random range64.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_int3_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int3(faker: Faker) : int3

Generates random int3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_int4_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int4(faker: Faker) : int4

Generates random int4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint2_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint2(faker: Faker) : uint2

Generates random uint2.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_urange_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_urange(faker: Faker) : urange

Generates random urange.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_urange64_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_urange64(faker: Faker) : urange64

Generates random urange64.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint3_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint3(faker: Faker) : uint3

Generates random uint3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint4_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint4(faker: Faker) : uint4

Generates random uint4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float2_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float2(faker: Faker) : float2

Generates random float2.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float3_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float3(faker: Faker) : float3

Generates random float3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float4_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float4(faker: Faker) : float4

Generates random float4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float3x3_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float3x3(faker: Faker) : float3x3

Generates random float3x3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float3x4_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float3x4(faker: Faker) : float3x4

Generates random float3x4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_float4x4_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_float4x4(faker: Faker) : float4x4

Generates random float4x4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_int64_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_int64(faker: Faker) : int64

Generates random int64


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_uint64_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_uint64(faker: Faker) : uint64

Generates random uint64


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_random_double_S_ls_faker_c__c_Faker_gr_:

.. das:function:: random_double(faker: Faker) : double

Generates random double.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

++++++++++++++
Random strings
++++++++++++++

  *  :ref:`long_string (var faker: Faker) : string <function-_at_faker_c__c_long_string_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_string (var faker: Faker) : string <function-_at_faker_c__c_any_string_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_file_name (var faker: Faker) : string <function-_at_faker_c__c_any_file_name_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_set (var faker: Faker) : uint[8] <function-_at_faker_c__c_any_set_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_char (var faker: Faker) : int <function-_at_faker_c__c_any_char_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`number (var faker: Faker) : string <function-_at_faker_c__c_number_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`positive_int (var faker: Faker) : string <function-_at_faker_c__c_positive_int_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_int (var faker: Faker) : string <function-_at_faker_c__c_any_int_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_hex (var faker: Faker) : string <function-_at_faker_c__c_any_hex_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_float (var faker: Faker) : string <function-_at_faker_c__c_any_float_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`any_uint (var faker: Faker) : string <function-_at_faker_c__c_any_uint_S_ls_faker_c__c_Faker_gr_>` 

.. _function-_at_faker_c__c_long_string_S_ls_faker_c__c_Faker_gr_:

.. das:function:: long_string(faker: Faker) : string

Generates a long string of random characters. The string is anywhere between 0 and faker.max_long_string characters long.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_string_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_string(faker: Faker) : string

Generates a string of random characters. The string is anywhere between 0 and regex::re_gen_get_rep_limit() characters long.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_file_name_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_file_name(faker: Faker) : string

Generates random file name.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_set_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_set(faker: Faker) : uint[8]

Generates random set (uint[8])


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_char_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_char(faker: Faker) : int

Generates random char. (1 to 255 range)


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_number_S_ls_faker_c__c_Faker_gr_:

.. das:function:: number(faker: Faker) : string

Generates random number string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_positive_int_S_ls_faker_c__c_Faker_gr_:

.. das:function:: positive_int(faker: Faker) : string

Generates random positive integer string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_int_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_int(faker: Faker) : string

Generates random integer string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_hex_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_hex(faker: Faker) : string

Generates random integer hex string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_float_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_float(faker: Faker) : string

Generates random float string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_any_uint_S_ls_faker_c__c_Faker_gr_:

.. das:function:: any_uint(faker: Faker) : string

Generates random unsigned integer string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

+++++++++++++
Date and time
+++++++++++++

  *  :ref:`month (var faker: Faker) : string <function-_at_faker_c__c_month_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`day (var faker: Faker) : string <function-_at_faker_c__c_day_S_ls_faker_c__c_Faker_gr_>` 
  *  :ref:`is_leap_year (year: uint) : bool <function-_at_faker_c__c_is_leap_year_Cu>` 
  *  :ref:`week_day (year: uint; month: uint; day: uint) : int <function-_at_faker_c__c_week_day_Cu_Cu_Cu>` 
  *  :ref:`week_day (year: int; month: int; day: int) : int <function-_at_faker_c__c_week_day_Ci_Ci_Ci>` 
  *  :ref:`date (var faker: Faker) : string <function-_at_faker_c__c_date_S_ls_faker_c__c_Faker_gr_>` 

.. _function-_at_faker_c__c_month_S_ls_faker_c__c_Faker_gr_:

.. das:function:: month(faker: Faker) : string

Generates random month string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_day_S_ls_faker_c__c_Faker_gr_:

.. das:function:: day(faker: Faker) : string

Generates random day string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 

.. _function-_at_faker_c__c_is_leap_year_Cu:

.. das:function:: is_leap_year(year: uint) : bool

Returns true if year is leap year.


:Arguments: * **year** : uint

.. _function-_at_faker_c__c_week_day_Cu_Cu_Cu:

.. das:function:: week_day(year: uint; month: uint; day: uint) : int

Returns week day for given date.


:Arguments: * **year** : uint

            * **month** : uint

            * **day** : uint

.. _function-_at_faker_c__c_week_day_Ci_Ci_Ci:

.. das:function:: week_day(year: int; month: int; day: int) : int

Returns week day for given date.


:Arguments: * **year** : int

            * **month** : int

            * **day** : int

.. _function-_at_faker_c__c_date_S_ls_faker_c__c_Faker_gr_:

.. das:function:: date(faker: Faker) : string

Generates random date string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>` 


