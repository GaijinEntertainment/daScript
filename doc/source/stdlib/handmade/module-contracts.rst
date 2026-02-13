The CONTRACTS module provides compile-time type constraints for generic function
arguments. Annotations like ``[expect_any_array]``, ``[expect_any_enum]``,
``[expect_any_numeric]``, and ``[expect_any_struct]`` restrict which types
can instantiate a generic parameter, producing clear error messages on mismatch.

All functions and symbols are in "contracts" module, use require to get access to it. ::

    require daslib/contracts

Example: ::

    require daslib/contracts

        [!expect_dim(a)]
        def process(a) {
            return "scalar"
        }

        [expect_dim(a)]
        def process(a) {
            return "array"
        }

        [export]
        def main() {
            var arr : int[3]
            print("{process(42)}\n")
            print("{process(arr)}\n")
        }
        // output:
        // scalar
        // array
