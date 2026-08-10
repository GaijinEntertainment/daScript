The MATH_BITS module provides bit-level reinterpretation between integer and
floating point representations — ``int_bits_to_float``, ``uint_bits_to_float``,
``float_bits_to_int``, ``float_bits_to_uint`` (plus the 64-bit ``double``
forms and 2/3/4-lane vector overloads) — as well as the ``cast_to_*`` helpers
that pack and unpack values through a ``float4`` payload.

All functions and symbols are in "math_bits" module, use require to get access to it.

.. code-block:: das

    require daslib/math_bits

Example:

.. code-block:: das

    require daslib/math_bits

    [export]
    def main() {
        let f = uint_bits_to_float(0x3F800000u)
        print("uint_bits_to_float(0x3F800000) = {f}\n")
        let back = float_bits_to_uint(1.0)
        print("float_bits_to_uint(1.0) = {back}\n")
    }
    // output:
    // uint_bits_to_float(0x3F800000) = 1
    // float_bits_to_uint(1.0) = 0x3f800000
