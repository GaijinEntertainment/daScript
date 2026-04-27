The STRINGS_CONVERT module provides soft-failing string-to-numeric conversions
that return ``Result<T; ConversionError>`` instead of panicking or silently
returning zero. Use these when parsing untrusted input where you need to
distinguish between not-a-number, overflow, and trailing-garbage.

All functions and symbols are in the "strings_convert" module, use require to get access to it.

.. code-block:: das

    require daslib/strings_convert

Example:

.. code-block:: das

    require daslib/strings_convert

    [export]
    def main() {
        let r = try_to_int("42")
        if (is_ok(r)) {
            print("parsed: {unwrap(r)}\n")
        }
        let bad = try_to_int("nope")
        if (is_err(bad)) {
            print("error: {unwrap_err(bad)}\n")
        }
    }
    // output:
    // parsed: 42
    // error: invalid_argument
