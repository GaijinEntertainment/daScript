Indicates that the function is generic, regardless of its argument types. Generic functions will be instanced in the calling module::

    [generic]
    def random_seed ( seed:int )    // this is now a generic function
        return int4(seed,seed+1,seed+2,seed+3)

