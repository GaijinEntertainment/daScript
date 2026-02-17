
.. _stdlib_faker:

=====
Faker
=====

.. das:module:: faker

Random test-data generator.

The ``Faker`` struct produces random values for every built-in type
(integers, floats, vectors, strings, dates, booleans) using
configurable ranges. Used by ``fuzzer`` for fuzz testing.

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

  *  :ref:`Faker (var rng: iterator\<uint\>) : Faker <function-faker_Faker_iterator_ls_uint_gr_>`

.. _function-faker_Faker_iterator_ls_uint_gr_:

.. das:function:: Faker(rng: iterator<uint>) : Faker

Constructs a Faker instance with the given random number generator.


:Arguments: * **rng** : iterator<uint>


+++++++++++++
Random values
+++++++++++++

  *  :ref:`random_double (var faker: Faker) : double <function-faker_random_double_Faker>`
  *  :ref:`random_float (var faker: Faker) : float <function-faker_random_float_Faker>`
  *  :ref:`random_float2 (var faker: Faker) : float2 <function-faker_random_float2_Faker>`
  *  :ref:`random_float3 (var faker: Faker) : float3 <function-faker_random_float3_Faker>`
  *  :ref:`random_float3x3 (var faker: Faker) : float3x3 <function-faker_random_float3x3_Faker>`
  *  :ref:`random_float3x4 (var faker: Faker) : float3x4 <function-faker_random_float3x4_Faker>`
  *  :ref:`random_float4 (var faker: Faker) : float4 <function-faker_random_float4_Faker>`
  *  :ref:`random_float4x4 (var faker: Faker) : float4x4 <function-faker_random_float4x4_Faker>`
  *  :ref:`random_int (var faker: Faker) : int <function-faker_random_int_Faker>`
  *  :ref:`random_int16 (var faker: Faker) : int16 <function-faker_random_int16_Faker>`
  *  :ref:`random_int2 (var faker: Faker) : int2 <function-faker_random_int2_Faker>`
  *  :ref:`random_int3 (var faker: Faker) : int3 <function-faker_random_int3_Faker>`
  *  :ref:`random_int4 (var faker: Faker) : int4 <function-faker_random_int4_Faker>`
  *  :ref:`random_int64 (var faker: Faker) : int64 <function-faker_random_int64_Faker>`
  *  :ref:`random_int8 (var faker: Faker) : int8 <function-faker_random_int8_Faker>`
  *  :ref:`random_range (var faker: Faker) : range <function-faker_random_range_Faker>`
  *  :ref:`random_range64 (var faker: Faker) : range64 <function-faker_random_range64_Faker>`
  *  :ref:`random_uint (var faker: Faker) : uint <function-faker_random_uint_Faker>`
  *  :ref:`random_uint16 (var faker: Faker) : uint16 <function-faker_random_uint16_Faker>`
  *  :ref:`random_uint2 (var faker: Faker) : uint2 <function-faker_random_uint2_Faker>`
  *  :ref:`random_uint3 (var faker: Faker) : uint3 <function-faker_random_uint3_Faker>`
  *  :ref:`random_uint4 (var faker: Faker) : uint4 <function-faker_random_uint4_Faker>`
  *  :ref:`random_uint64 (var faker: Faker) : uint64 <function-faker_random_uint64_Faker>`
  *  :ref:`random_uint8 (var faker: Faker) : uint8 <function-faker_random_uint8_Faker>`
  *  :ref:`random_urange (var faker: Faker) : urange <function-faker_random_urange_Faker>`
  *  :ref:`random_urange64 (var faker: Faker) : urange64 <function-faker_random_urange64_Faker>`

.. _function-faker_random_double_Faker:

.. das:function:: random_double(faker: Faker) : double

Generates random double.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float_Faker:

.. das:function:: random_float(faker: Faker) : float

Generates random float.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float2_Faker:

.. das:function:: random_float2(faker: Faker) : float2

Generates random float2.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float3_Faker:

.. das:function:: random_float3(faker: Faker) : float3

Generates random float3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float3x3_Faker:

.. das:function:: random_float3x3(faker: Faker) : float3x3

Generates random float3x3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float3x4_Faker:

.. das:function:: random_float3x4(faker: Faker) : float3x4

Generates random float3x4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float4_Faker:

.. das:function:: random_float4(faker: Faker) : float4

Generates random float4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_float4x4_Faker:

.. das:function:: random_float4x4(faker: Faker) : float4x4

Generates random float4x4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int_Faker:

.. das:function:: random_int(faker: Faker) : int

Generates random integer.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int16_Faker:

.. das:function:: random_int16(faker: Faker) : int16

Generates random int16.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int2_Faker:

.. das:function:: random_int2(faker: Faker) : int2

Generates random int2.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int3_Faker:

.. das:function:: random_int3(faker: Faker) : int3

Generates random int3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int4_Faker:

.. das:function:: random_int4(faker: Faker) : int4

Generates random int4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int64_Faker:

.. das:function:: random_int64(faker: Faker) : int64

Generates random int64


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_int8_Faker:

.. das:function:: random_int8(faker: Faker) : int8

Generates random int8.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_range_Faker:

.. das:function:: random_range(faker: Faker) : range

Generates random range.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_range64_Faker:

.. das:function:: random_range64(faker: Faker) : range64

Generates random range64.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint_Faker:

.. das:function:: random_uint(faker: Faker) : uint

Generates random unsigned integer.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint16_Faker:

.. das:function:: random_uint16(faker: Faker) : uint16

Generates random uint16.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint2_Faker:

.. das:function:: random_uint2(faker: Faker) : uint2

Generates random uint2.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint3_Faker:

.. das:function:: random_uint3(faker: Faker) : uint3

Generates random uint3.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint4_Faker:

.. das:function:: random_uint4(faker: Faker) : uint4

Generates random uint4.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint64_Faker:

.. das:function:: random_uint64(faker: Faker) : uint64

Generates random uint64


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_uint8_Faker:

.. das:function:: random_uint8(faker: Faker) : uint8

Generates random uint8.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_urange_Faker:

.. das:function:: random_urange(faker: Faker) : urange

Generates random urange.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_random_urange64_Faker:

.. das:function:: random_urange64(faker: Faker) : urange64

Generates random urange64.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`


++++++++++++++
Random strings
++++++++++++++

  *  :ref:`any_char (var faker: Faker) : int <function-faker_any_char_Faker>`
  *  :ref:`any_enum (var faker: Faker; enum_value: auto(TT)) : TT <function-faker_any_enum_Faker_autoTT_0x13e>`
  *  :ref:`any_file_name (var faker: Faker) : string <function-faker_any_file_name_Faker>`
  *  :ref:`any_float (var faker: Faker) : string <function-faker_any_float_Faker>`
  *  :ref:`any_hex (var faker: Faker) : string <function-faker_any_hex_Faker>`
  *  :ref:`any_int (var faker: Faker) : string <function-faker_any_int_Faker>`
  *  :ref:`any_set (var faker: Faker) : uint[8] <function-faker_any_set_Faker>`
  *  :ref:`any_string (var faker: Faker) : string <function-faker_any_string_Faker>`
  *  :ref:`any_uint (var faker: Faker) : string <function-faker_any_uint_Faker>`
  *  :ref:`long_string (var faker: Faker) : string <function-faker_long_string_Faker>`
  *  :ref:`number (var faker: Faker) : string <function-faker_number_Faker>`
  *  :ref:`positive_int (var faker: Faker) : string <function-faker_positive_int_Faker>`

.. _function-faker_any_char_Faker:

.. das:function:: any_char(faker: Faker) : int

Generates random char. (1 to 255 range)


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_enum_Faker_autoTT_0x13e:

.. das:function:: any_enum(faker: Faker; enum_value: auto(TT)) : TT

Generates random enumeration value.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

            * **enum_value** : auto(TT)

.. _function-faker_any_file_name_Faker:

.. das:function:: any_file_name(faker: Faker) : string

Generates random file name.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_float_Faker:

.. das:function:: any_float(faker: Faker) : string

Generates random float string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_hex_Faker:

.. das:function:: any_hex(faker: Faker) : string

Generates random integer hex string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_int_Faker:

.. das:function:: any_int(faker: Faker) : string

Generates random integer string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_set_Faker:

.. das:function:: any_set(faker: Faker) : uint[8]

Generates random set (uint[8])


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_string_Faker:

.. das:function:: any_string(faker: Faker) : string

Generates a string of random characters. The string is anywhere between 0 and regex::re_gen_get_rep_limit() characters long.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_any_uint_Faker:

.. das:function:: any_uint(faker: Faker) : string

Generates random unsigned integer string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_long_string_Faker:

.. das:function:: long_string(faker: Faker) : string

Generates a long string of random characters. The string is anywhere between 0 and faker.max_long_string characters long.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_number_Faker:

.. das:function:: number(faker: Faker) : string

Generates random number string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_positive_int_Faker:

.. das:function:: positive_int(faker: Faker) : string

Generates random positive integer string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`


+++++++++++++
Date and time
+++++++++++++

  *  :ref:`date (var faker: Faker) : string <function-faker_date_Faker>`
  *  :ref:`day (var faker: Faker) : string <function-faker_day_Faker>`
  *  :ref:`is_leap_year (year: uint) : bool <function-faker_is_leap_year_uint>`
  *  :ref:`month (var faker: Faker) : string <function-faker_month_Faker>`
  *  :ref:`week_day (year: uint; month: uint; day: uint) : int <function-faker_week_day_uint_uint_uint>`
  *  :ref:`week_day (year: int; month: int; day: int) : int <function-faker_week_day_int_int_int>`

.. _function-faker_date_Faker:

.. das:function:: date(faker: Faker) : string

Generates random date string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_day_Faker:

.. das:function:: day(faker: Faker) : string

Generates random day string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`

.. _function-faker_is_leap_year_uint:

.. das:function:: is_leap_year(year: uint) : bool

Returns true if year is leap year.


:Arguments: * **year** : uint

.. _function-faker_month_Faker:

.. das:function:: month(faker: Faker) : string

Generates random month string.


:Arguments: * **faker** :  :ref:`Faker <struct-faker-Faker>`


week_day
^^^^^^^^

.. _function-faker_week_day_uint_uint_uint:

.. das:function:: week_day(year: uint; month: uint; day: uint) : int

Returns week day for given date.


:Arguments: * **year** : uint

            * **month** : uint

            * **day** : uint

.. _function-faker_week_day_int_int_int:

.. das:function:: week_day(year: int; month: int; day: int) : int

----


