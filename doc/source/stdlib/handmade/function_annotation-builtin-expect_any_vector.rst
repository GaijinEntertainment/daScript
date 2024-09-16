Indicates that function can only accept das::vector templates.
Here is an example from builtin.das::

    [expect_any_vector(arr)]
    def back ( var arr : auto(TT) ==const ) : auto &
        let n = _::length(arr)
        if n == 0
            panic("vector is empty")
        unsafe
            return arr[n-1]

