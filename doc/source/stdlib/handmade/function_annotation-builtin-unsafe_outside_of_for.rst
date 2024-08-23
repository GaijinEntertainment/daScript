Marks function as unsafe to be called outside of the sources `for` loop::


    [unsafe_outside_of_for,nodiscard]
    def keys(a:table<auto(keyT);auto(valT)>|#) : iterator<keyT & const>
        ...

    [export]
    def main
        ...
        keys(arr) // will not compile, need to use `var keys <- unsafe(keys(arr))` instead

        for k in keys(arr)  // this is fine
            ...

