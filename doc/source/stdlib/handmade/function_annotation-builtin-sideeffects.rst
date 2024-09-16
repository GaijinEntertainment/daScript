Indicates that the function should be treated as if it has side-effects. For example it will not be optimized out::

    [sideeffects]
    def foo
        var a = 1
        a ++

    [export]
    def main
        foo()   // will actually call foo(), even though it does nothing and will be optimized out otherwise

