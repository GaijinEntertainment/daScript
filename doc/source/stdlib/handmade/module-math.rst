The MATH module contains floating point math functions and constants
(trigonometry, exponentials, clamping, interpolation, noise, and vector/matrix operations).
Floating point math in general is not bit-precise: the compiler may optimize
permutations, replace divisions with multiplications, and some functions are
not bit-exact. Use ``double`` precision types when exact results are required.

Common scalar and vectorized families include:

- Trigonometric and inverse trigonometric: ``sin``, ``cos``, ``tan``, ``asin``, ``acos``, ``atan``, ``atan2``
- Hyperbolic and inverse hyperbolic: ``sinh``, ``cosh``, ``tanh``, ``asinh``, ``acosh``, ``atanh``
- Exponential and logarithmic: ``exp``, ``exp2``, ``expm1``, ``log``, ``log2``, ``log10``, ``log1p``, ``pow``
- Core numeric helpers: ``sqrt``, ``cbrt``, ``hypot``, ``trunc``, ``floor``, ``ceil``, ``round``, ``fract``

Remainder note for floating point values:

- ``a % b`` and ``fmod(a, b)`` follow truncation-style remainder (sign follows dividend ``a``).
- ``remainder(a, b)`` follows IEEE remainder rules and can differ from ``%``/``fmod`` for the same inputs.

All functions and symbols are in "math" module, use require to get access to it.

.. code-block:: das

    require math

Example:

.. code-block:: das

    require math

        [export]
        def main() {
            print("sin(PI/2) = {sin(PI / 2.0)}\n")
            print("cos(0)    = {cos(0.0)}\n")
            print("sqrt(16)  = {sqrt(16.0)}\n")
            print("abs(-5)   = {abs(-5)}\n")
            print("clamp(15, 0, 10) = {clamp(15, 0, 10)}\n")
            print("min(3, 7) = {min(3, 7)}\n")
            print("max(3, 7) = {max(3, 7)}\n")
            print("tanh(1) = {tanh(1.0)}\n")
            print("log10(1000) = {log10(1000.0)}\n")
            let v = float3(1, 0, 0)
            print("length = {length(v)}\n")
        }
        // output:
        // sin(PI/2) = 1
        // cos(0)    = 1
        // sqrt(16)  = 4
        // abs(-5)   = 5
        // clamp(15, 0, 10) = 10
        // min(3, 7) = 3
        // max(3, 7) = 7
        // tanh(1) = 0.7615942
        // log10(1000) = 3
        // length = 1
