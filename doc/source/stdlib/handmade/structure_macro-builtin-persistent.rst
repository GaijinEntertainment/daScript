Specifies, that a class or a structure is allocated on the C++ heap, as oppose to Daslang heap::

    [persistent]
    class Foo   // this class will say in memory even after the heap is destroyed or reset
        count : int = 0
        def Foo ( ic : int )
            count = ic

