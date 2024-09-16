A contract to mark function argument to be a static array::

    [expect_dim(arg)]
    def test(arg)
        print("arg={arg}\n")

    [export]
    def main
        test(1)                     // compilation error
        test(fixed_array(1,2,3))    // int[3] is ok

