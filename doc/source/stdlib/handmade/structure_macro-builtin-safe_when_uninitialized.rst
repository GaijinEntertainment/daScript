Marks structure as safe to be used when uninitialized::

    [safe_when_uninitialized]
    struct Foo      // it is safe to use variables of type Foo, without initializing them (they will be initialized with 0s)
        data_bool:bool=true
        data_int:int = 1
        data_float:float = 3.14
        data_bar:Bar <- Bar()
        data_uint_3 : uint[3] = [[ uint 1u; 2u; 3u ]]

