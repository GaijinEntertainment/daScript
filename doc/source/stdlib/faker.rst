
.. _stdlib_faker:

=====
Faker
=====

.. include:: detail/faker.rst

The FAKER module implements collection of random data generators for use in testing and otherwise.

All functions and symbols are in "faker" module, use require to get access to it. ::

    require daslib/faker

++++++++++++
Type aliases
++++++++++++

.. _alias-BitRepresentation64:

.. das:attribute:: BitRepresentation64 is a variant type

+---+-------+
+ui2+uint[2]+
+---+-------+
+d  +double +
+---+-------+


64-bit representation of a float

.. _struct-faker-Faker:

.. das:attribute:: Faker



Faker fields are

+---------------+--------------+
+min_year       +uint          +
+---------------+--------------+
+total_years    +uint          +
+---------------+--------------+
+rnd            +iterator<uint>+
+---------------+--------------+
+max_long_string+uint          +
+---------------+--------------+


Instance of the faker with all the settings inside.

+++++++++++
Constructor
+++++++++++

  *  :ref:`Faker (rng:iterator\<uint\> -const) : faker::Faker <function-_at_faker_c__c_Faker_1_ls_u_gr_G>` 

.. _function-_at_faker_c__c_Faker_1_ls_u_gr_G:

.. das:function:: Faker(rng: iterator<uint>)

Faker returns  :ref:`faker::Faker <struct-faker-Faker>` 

+--------+--------------+
+argument+argument type +
+========+==============+
+rng     +iterator<uint>+
+--------+--------------+


|function-faker-Faker|

+++++++++++++
Random values
+++++++++++++

  *  :ref:`random_int (faker:faker::Faker -const) : int <function-_at_faker_c__c_random_int_S_ls_Faker_gr_>` 
  *  :ref:`random_uint (faker:faker::Faker -const) : uint <function-_at_faker_c__c_random_uint_S_ls_Faker_gr_>` 
  *  :ref:`random_float (faker:faker::Faker -const) : float <function-_at_faker_c__c_random_float_S_ls_Faker_gr_>` 
  *  :ref:`random_float2 (faker:faker::Faker -const) : float2 <function-_at_faker_c__c_random_float2_S_ls_Faker_gr_>` 
  *  :ref:`random_float3 (faker:faker::Faker -const) : float3 <function-_at_faker_c__c_random_float3_S_ls_Faker_gr_>` 
  *  :ref:`random_float4 (faker:faker::Faker -const) : float4 <function-_at_faker_c__c_random_float4_S_ls_Faker_gr_>` 
  *  :ref:`random_float3x3 (faker:faker::Faker -const) : math::float3x3 <function-_at_faker_c__c_random_float3x3_S_ls_Faker_gr_>` 
  *  :ref:`random_float3x4 (faker:faker::Faker -const) : math::float3x4 <function-_at_faker_c__c_random_float3x4_S_ls_Faker_gr_>` 
  *  :ref:`random_float4x4 (faker:faker::Faker -const) : math::float4x4 <function-_at_faker_c__c_random_float4x4_S_ls_Faker_gr_>` 
  *  :ref:`random_double (faker:faker::Faker -const) : double <function-_at_faker_c__c_random_double_S_ls_Faker_gr_>` 

.. _function-_at_faker_c__c_random_int_S_ls_Faker_gr_:

.. das:function:: random_int(faker: Faker)

random_int returns int

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random integer.

.. _function-_at_faker_c__c_random_uint_S_ls_Faker_gr_:

.. das:function:: random_uint(faker: Faker)

random_uint returns uint

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random unsigned integer.

.. _function-_at_faker_c__c_random_float_S_ls_Faker_gr_:

.. das:function:: random_float(faker: Faker)

random_float returns float

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float.

.. _function-_at_faker_c__c_random_float2_S_ls_Faker_gr_:

.. das:function:: random_float2(faker: Faker)

random_float2 returns float2

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float2.

.. _function-_at_faker_c__c_random_float3_S_ls_Faker_gr_:

.. das:function:: random_float3(faker: Faker)

random_float3 returns float3

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float3.

.. _function-_at_faker_c__c_random_float4_S_ls_Faker_gr_:

.. das:function:: random_float4(faker: Faker)

random_float4 returns float4

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float4.

.. _function-_at_faker_c__c_random_float3x3_S_ls_Faker_gr_:

.. das:function:: random_float3x3(faker: Faker)

random_float3x3 returns  :ref:`math::float3x3 <handle-math-float3x3>` 

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float3x3.

.. _function-_at_faker_c__c_random_float3x4_S_ls_Faker_gr_:

.. das:function:: random_float3x4(faker: Faker)

random_float3x4 returns  :ref:`math::float3x4 <handle-math-float3x4>` 

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float3x4.

.. _function-_at_faker_c__c_random_float4x4_S_ls_Faker_gr_:

.. das:function:: random_float4x4(faker: Faker)

random_float4x4 returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float4x4.

.. _function-_at_faker_c__c_random_double_S_ls_Faker_gr_:

.. das:function:: random_double(faker: Faker)

random_double returns double

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random double.

++++++++++++++
Random strings
++++++++++++++

  *  :ref:`long_string (faker:faker::Faker -const) : string <function-_at_faker_c__c_long_string_S_ls_Faker_gr_>` 
  *  :ref:`any_string (faker:faker::Faker -const) : string <function-_at_faker_c__c_any_string_S_ls_Faker_gr_>` 
  *  :ref:`any_file_name (faker:faker::Faker -const) : string <function-_at_faker_c__c_any_file_name_S_ls_Faker_gr_>` 
  *  :ref:`any_set (faker:faker::Faker -const) : uint[8] <function-_at_faker_c__c_any_set_S_ls_Faker_gr_>` 
  *  :ref:`any_char (faker:faker::Faker -const) : int <function-_at_faker_c__c_any_char_S_ls_Faker_gr_>` 
  *  :ref:`number (faker:faker::Faker -const) : string <function-_at_faker_c__c_number_S_ls_Faker_gr_>` 
  *  :ref:`positive_int (faker:faker::Faker -const) : string <function-_at_faker_c__c_positive_int_S_ls_Faker_gr_>` 
  *  :ref:`any_int (faker:faker::Faker -const) : string <function-_at_faker_c__c_any_int_S_ls_Faker_gr_>` 
  *  :ref:`any_hex (faker:faker::Faker -const) : string <function-_at_faker_c__c_any_hex_S_ls_Faker_gr_>` 
  *  :ref:`any_float (faker:faker::Faker -const) : string <function-_at_faker_c__c_any_float_S_ls_Faker_gr_>` 
  *  :ref:`any_uint (faker:faker::Faker -const) : string <function-_at_faker_c__c_any_uint_S_ls_Faker_gr_>` 

.. _function-_at_faker_c__c_long_string_S_ls_Faker_gr_:

.. das:function:: long_string(faker: Faker)

long_string returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates a long string of random characters. The string is anywhere between 0 and faker.max_long_string characters long.

.. _function-_at_faker_c__c_any_string_S_ls_Faker_gr_:

.. das:function:: any_string(faker: Faker)

any_string returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates a string of random characters. The string is anywhere between 0 and regex::re_gen_get_rep_limit() characters long.

.. _function-_at_faker_c__c_any_file_name_S_ls_Faker_gr_:

.. das:function:: any_file_name(faker: Faker)

any_file_name returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random file name.

.. _function-_at_faker_c__c_any_set_S_ls_Faker_gr_:

.. das:function:: any_set(faker: Faker)

any_set returns uint[8]

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random set (uint[8])

.. _function-_at_faker_c__c_any_char_S_ls_Faker_gr_:

.. das:function:: any_char(faker: Faker)

any_char returns int

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random char. (1 to 255 range)

.. _function-_at_faker_c__c_number_S_ls_Faker_gr_:

.. das:function:: number(faker: Faker)

number returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random number string.

.. _function-_at_faker_c__c_positive_int_S_ls_Faker_gr_:

.. das:function:: positive_int(faker: Faker)

positive_int returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random positive integer string.

.. _function-_at_faker_c__c_any_int_S_ls_Faker_gr_:

.. das:function:: any_int(faker: Faker)

any_int returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random integer string.

.. _function-_at_faker_c__c_any_hex_S_ls_Faker_gr_:

.. das:function:: any_hex(faker: Faker)

any_hex returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random integer hex string.

.. _function-_at_faker_c__c_any_float_S_ls_Faker_gr_:

.. das:function:: any_float(faker: Faker)

any_float returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random float string.

.. _function-_at_faker_c__c_any_uint_S_ls_Faker_gr_:

.. das:function:: any_uint(faker: Faker)

any_uint returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random unsigned integer string.

+++++++++++++
Date and time
+++++++++++++

  *  :ref:`month (faker:faker::Faker -const) : string <function-_at_faker_c__c_month_S_ls_Faker_gr_>` 
  *  :ref:`day (faker:faker::Faker -const) : string <function-_at_faker_c__c_day_S_ls_Faker_gr_>` 
  *  :ref:`is_leap_year (year:uint const) : bool <function-_at_faker_c__c_is_leap_year_Cu>` 
  *  :ref:`week_day (year:uint const;month:uint const;day:uint const) : int <function-_at_faker_c__c_week_day_Cu_Cu_Cu>` 
  *  :ref:`week_day (year:int const;month:int const;day:int const) : int <function-_at_faker_c__c_week_day_Ci_Ci_Ci>` 
  *  :ref:`date (faker:faker::Faker -const) : string <function-_at_faker_c__c_date_S_ls_Faker_gr_>` 

.. _function-_at_faker_c__c_month_S_ls_Faker_gr_:

.. das:function:: month(faker: Faker)

month returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random month string.

.. _function-_at_faker_c__c_day_S_ls_Faker_gr_:

.. das:function:: day(faker: Faker)

day returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random day string.

.. _function-_at_faker_c__c_is_leap_year_Cu:

.. das:function:: is_leap_year(year: uint const)

is_leap_year returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+year    +uint const   +
+--------+-------------+


Returns true if year is leap year.

.. _function-_at_faker_c__c_week_day_Cu_Cu_Cu:

.. das:function:: week_day(year: uint const; month: uint const; day: uint const)

week_day returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+year    +uint const   +
+--------+-------------+
+month   +uint const   +
+--------+-------------+
+day     +uint const   +
+--------+-------------+


Returns week day for given date.

.. _function-_at_faker_c__c_week_day_Ci_Ci_Ci:

.. das:function:: week_day(year: int const; month: int const; day: int const)

week_day returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+year    +int const    +
+--------+-------------+
+month   +int const    +
+--------+-------------+
+day     +int const    +
+--------+-------------+


Returns week day for given date.

.. _function-_at_faker_c__c_date_S_ls_Faker_gr_:

.. das:function:: date(faker: Faker)

date returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+faker   + :ref:`faker::Faker <struct-faker-Faker>` +
+--------+------------------------------------------+


Generates random date string.

