Result types are used internally in the compiler to store in the cache.

Manually they would be defined like so:
    typedef Result1 = tuple<success:bool; value:int; endpos:int>;
for every possible rule type

