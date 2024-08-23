Similar to regular `assert` function, but always happens at compilation time::

    def take_small(a)
        static_assert(typeinfo(sizeof a)<=16,"a is too big")


